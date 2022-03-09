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

#define ART_MODULE_NAME     ArcPointContext

#import "ArcPointContext.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArcPointContext

- (id) init
{
    self = [ super init ];

    if ( self )
    {
        traversalState = ARTS_EMPTY;
    }
    
    return self;
}

- (void) prepareForUse
        : (ArcPhaseInterfaceCache *) phaseInterfaceCache
{
}

- (void) activate
{
    next = 0;
    prev = 0;

    pc_status_flags = arpcflag_all_variables_are_invalid;

    artraversalstate_clear( & traversalState );
}

- (void) deactivate
{
}

- (void) dealloc
{
    artraversalstate_free_contents( & traversalState );

    [ super dealloc ];
}

- (void) debugprintf
{
    debugprintf(
        "Objectspace location: "PNT3D_FORMAT("% 5.3f")"\n",
        PNT3D_P_PRINTF(objectspace_point)
        );

    debugprintf(
        "Worldspace location:  "PNT3D_FORMAT("% 5.3f")"\n",
        PNT3D_P_PRINTF(worldspace_point)
        );

    artraversalstate_debugprintf( & traversalState );
}

- (const Pnt3D *) getWorldSpaceCoords
{
    return & worldspace_point;
}

- (const Pnt3D *) getObjectSpaceCoords
{
    return & objectspace_point;
}

/* ---------------------------------------------------------------------------
    'TRAVERSAL_STATE_EVAL_ENV_ACCESS_FUNCTIONS'
        Forward method calls to evaluation environment stored in traversal
        state. This is necessary because ArcPointContext is not a descendant
        of ArcEvaluationEnvironment.
 --------------------------------------------------------------------------- */
#define TRAVERSAL_STATE_EVAL_ENV_ACCESS_FUNCTIONS(_Type) \
\
- (void) add ## _Type \
        : (unsigned long) key \
        : (const _Type *) value \
{ \
    [ ARTS_VARIABLES(traversalState) add ## _Type \
            : key \
            : value \
            ]; \
} \
\
- (void) add ## _Type ## s \
        : (unsigned long) key \
        : (unsigned long) number \
        : (const _Type *) valueArray \
{ \
    [ ARTS_VARIABLES(traversalState) add ## _Type ## s \
            : key \
            : number \
            : valueArray \
            ]; \
} \
\
- (unsigned long) get ## _Type \
        : (unsigned long) key \
        : (_Type *) value \
{ \
    return \
        [ ARTS_VARIABLES(traversalState) get ## _Type \
                : key \
                : value \
                ]; \
} \
\
- (unsigned long) get ## _Type ## s \
        : (unsigned long) key \
        : (unsigned long) number \
        : (_Type *) valueArray \
{ \
    return \
        [ ARTS_VARIABLES(traversalState) get ## _Type ## s \
                : key \
                : number \
                : valueArray \
                ]; \
} \
\
- (unsigned long) del ## _Type \
        : (unsigned long) key \
        : (_Type *) value \
{ \
    return \
        [ ARTS_VARIABLES(traversalState) del ## _Type \
                : key \
                : value \
                ]; \
} \
\
- (unsigned long) del ## _Type ## s \
        : (unsigned long) key \
        : (unsigned long) number \
        : ( _Type *) valueArray \
{ \
    return \
        [ ARTS_VARIABLES(traversalState) del ## _Type ## s \
                : key \
                : number \
                : valueArray \
                ]; \
}


TRAVERSAL_STATE_EVAL_ENV_ACCESS_FUNCTIONS(Byte)
TRAVERSAL_STATE_EVAL_ENV_ACCESS_FUNCTIONS(Long)
TRAVERSAL_STATE_EVAL_ENV_ACCESS_FUNCTIONS(Double)
TRAVERSAL_STATE_EVAL_ENV_ACCESS_FUNCTIONS(Pnt2D)
TRAVERSAL_STATE_EVAL_ENV_ACCESS_FUNCTIONS(Vec2D)
TRAVERSAL_STATE_EVAL_ENV_ACCESS_FUNCTIONS(Pnt3D)
TRAVERSAL_STATE_EVAL_ENV_ACCESS_FUNCTIONS(Vec3D)

- (void) clearCache
{
    [ ARTS_VARIABLES(traversalState) clearCache ];
}

@end

// ===========================================================================
