/* ===========================================================================

    Copyright (c) The ART Development Team
    --------------------------------------

    For a comprehensive list of the members of the development team, and a
    description of their respective contributions, see the file
    "ART_DeveloperList.txt" that is distributed with the libraries.

    This file is part of the Advanced Rendering Toolkit (ART) libraries.

    ART is free software: you can redistribute it and/or modify it under the
    terms of the GNU General Public License as published by the Free Software
    Foundation, either version 3 of the License, or (at your option) any
    later version.

    ART is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with ART.  If not, see <http://www.gnu.org/licenses/>.

=========================================================================== */

#define ART_MODULE_NAME     ArcFreelist

#import "ART_Foundation_Datastructures.h"
#import "ArcFreelist.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

@implementation ArcFreelist

- (id) init
        : (Class) newClass
{
    self = [ super init ];

    if ( self )
    {
        classToInstantiate         = newClass;
        freelist                   = ARLIST_EMPTY;
        instanceActivationMethod   = 0;
        instanceDeactivationMethod = 0;
    }
    
    return self;
}

- (id) init
        : (Class) newClass
        : (SEL) activationMethod
        : (SEL) deactivationMethod
{
    self =
        [ self init
            :   newClass
            ];
    
    if ( self )
    {
        instanceActivationMethod   = activationMethod;
        instanceDeactivationMethod = deactivationMethod;
    }
    
    return self;
}

- (id) obtainInstance
{
    id  objectFromFreelist;

    if ( ! arlist_pop_id( & freelist, & objectFromFreelist ) )
        objectFromFreelist =
            [ [ [ classToInstantiate alloc ] init_ART_GV
                : art_gv ] init ];

    if ( instanceActivationMethod )
        [ objectFromFreelist
            performSelector
                :   instanceActivationMethod
                ];

//   uncomment this to see tons of debug output about which instances
//   get requested from the freelist

//debugprintf("obtain %s\n",[objectFromFreelist cStringClassName] )
//debugprintf("o")

    return objectFromFreelist;
}

- (void) releaseInstance
        : (ArcObject *) instance
{
    if ( instanceDeactivationMethod )
        [ instance
            performSelector
                :   instanceDeactivationMethod
                ];
    
//   uncomment this to see tons of debug output about which instances
//   get handed back to the freelist

//debugprintf("release %s\n",[instance cStringClassName] )
//debugprintf(".")

    arlist_add_id( & freelist, instance );
}

- (void) dealloc
{
    unsigned int  freelist_length = arlist_length( & freelist );

    for ( unsigned int i = 0; i < freelist_length; i++ )
    {
        id  objectFromFreelist;

        arlist_pop_id(
            & freelist,
            & objectFromFreelist
            );

        RELEASE_OBJECT(objectFromFreelist);
    }

    [ super dealloc ];
}

@end

// ===========================================================================
