/* ===========================================================================

    Copyright (c) 1996-2019 The ART Development Team
    ------------------------------------------------

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

#define ART_MODULE_NAME     ArcEvaluationEnvironment

#include "ART_Foundation.h"

#import "ArcEvaluationEnvironment.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

#define ARPEVALENV_IMPLEMENTATION_FOR_TYPE(_Type) \
\
- (void) add ## _Type \
        : (unsigned long) key \
        : (const _Type *) value \
{ \
} \
\
- (void) add ## _Type ## s \
        : (unsigned long) key \
        : (unsigned long) number \
        : (const _Type *) valueArray \
{ \
} \
\
- (unsigned long) get ## _Type \
        : (unsigned long) key \
        : (_Type *) value \
{ \
    return 0; \
} \
\
- (unsigned long) get ## _Type ## s \
        : (unsigned long) key \
        : (unsigned long) number \
        : (_Type *) valueArray \
{ \
    return 0; \
} \
\
- (unsigned long) del ## _Type \
        : (unsigned long) key \
        : (_Type *) value \
{ \
    return 0; \
} \
\
- (unsigned long) del ## _Type ## s \
        : (unsigned long) key \
        : (unsigned long) number \
        : ( _Type *) valueArray \
{ \
    return 0; \
}

@implementation ArcEvaluationEnvironment

- init
{
    self = [ super init ];
    
    if ( self )
    {
        variableBindings = artable_alloc_init();
    }
    
    return self;
}

- (void) clearCache
{
    artable_free_contents(
          variableBindings
        );
}

- copy
{
    ArcEvaluationEnvironment  * copiedInstance = [ super copy ];

    artable_copy(
          variableBindings,
          copiedInstance->variableBindings
        );

    return copiedInstance;
}

- (void) dealloc
{
    artable_free(
          variableBindings
        );

    [ super dealloc ];
}

ARPEVALENV_IMPLEMENTATION_FOR_TYPE(Byte)
ARPEVALENV_IMPLEMENTATION_FOR_TYPE(Long)
ARPEVALENV_IMPLEMENTATION_FOR_TYPE(Double)
ARPEVALENV_IMPLEMENTATION_FOR_TYPE(Pnt2D)
ARPEVALENV_IMPLEMENTATION_FOR_TYPE(Vec2D)
ARPEVALENV_IMPLEMENTATION_FOR_TYPE(Pnt3D)
ARPEVALENV_IMPLEMENTATION_FOR_TYPE(Vec3D)

@end

// ===========================================================================
