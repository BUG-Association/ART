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

#include "ART_Foundation.h"

ART_MODULE_INTERFACE(ArpEvaluationEnvironment)

/* ---------------------------------------------------------------------------
    'ArEvaluationEnvironmentType'
        This enum describes capabilities of an evaluation environment using
        bit masks. Each bit represents one set of (disjunct) functionality.
 --------------------------------------------------------------------------- */
typedef enum ArEvaluationEnvironmentType
{
    arevalenv_none              = 0x00000000,   // eval env not needed at all
    arevalenv_generic           = 0x00000001,

    arevalenv_point             = 0x00000002 | arevalenv_generic,
    arevalenv_surfacepoint      = 0x00000004 | arevalenv_point,
    arevalenv_intersection      = 0x00000008 | arevalenv_surfacepoint,
    arevalenv_pointinspace      = 0x00000010 | arevalenv_point,
    arevalenv_rayendpoint       = 0x00000020 | arevalenv_pointinspace
}
ArEvaluationEnvironmentType;

/* ---------------------------------------------------------------------------
    'AREVALENV_TYPE_CHECK'
        Macro to check whether the caller (evaluator) of an expression can
        provide required type of evaluation environment.
 --------------------------------------------------------------------------- */
#define AREVALENV_TYPE_CHECK(_param_node, _available_type, _param_name) \
    { \
        ArEvaluationEnvironmentType evalEnvType = \
                [ _param_node evalEnvType ]; \
        if ((evalEnvType & _available_type) != evalEnvType) \
            ART_ERRORHANDLING_FATAL_ERROR( \
                    "%s: parameter \"" \
                    _param_name \
                    "\" requests incompatible evaluation environment!", \
                    [ self cStringClassName ]); \
    }

/* ---------------------------------------------------------------------------
    'add...s'
        Stores a typed vector of values in the environment.
    'get...s'
        Retrieves a typed vector of values from the environment, and returns
        the number of values retrieved.
    'del...s'
        Deletes a typed vector of values from the environment, and returns
        the number of values deleted.
--------------------------------------------------------------------------- */
#define ARPEVALENV_ACCESS_FUNCTIONS_FOR_TYPE(_Type) \
\
- (void) add ## _Type \
        : (unsigned long) key \
        : (const _Type *) value \
        ; \
\
- (void) add ## _Type ## s \
        : (unsigned long) key \
        : (unsigned long) number \
        : (const _Type *) valueArray \
        ; \
\
- (unsigned long) get ## _Type \
        : (unsigned long) key \
        : (_Type *) value \
        ; \
\
- (unsigned long) get ## _Type ## s \
        : (unsigned long) key \
        : (unsigned long) number \
        : (_Type *) valueArray \
        ; \
\
- (unsigned long) del ## _Type \
        : (unsigned long) key \
        : (_Type *) value \
        ; \
\
- (unsigned long) del ## _Type ## s \
        : (unsigned long) key \
        : (unsigned long) number \
        : ( _Type *) valueArray \
        ;

/* ===========================================================================
    'ArpEvaluationEnvironment'
=========================================================================== */
@protocol ArpEvaluationEnvironment

ARPEVALENV_ACCESS_FUNCTIONS_FOR_TYPE(Byte)
ARPEVALENV_ACCESS_FUNCTIONS_FOR_TYPE(Long)
ARPEVALENV_ACCESS_FUNCTIONS_FOR_TYPE(Double)
ARPEVALENV_ACCESS_FUNCTIONS_FOR_TYPE(Pnt2D)
ARPEVALENV_ACCESS_FUNCTIONS_FOR_TYPE(Vec2D)
ARPEVALENV_ACCESS_FUNCTIONS_FOR_TYPE(Pnt3D)
ARPEVALENV_ACCESS_FUNCTIONS_FOR_TYPE(Vec3D)

- (void) clearCache
    ;

@end

/* ===========================================================================
    'ArpPointEvalEnv'
=========================================================================== */
@protocol ArpPointEvalEnv <ArpEvaluationEnvironment>

- (const Pnt3D *) getWorldSpaceCoords
    ;

- (const Pnt3D *) getObjectSpaceCoords
    ;

@end

/* ===========================================================================
    'ArpSurfacePointEvalEnv'
=========================================================================== */
@protocol ArpSurfacePointEvalEnv <ArpPointEvalEnv>

- (const Vec3D *) getWorldSpaceNormal
    ;

- (const Vec3D *) getObjectSpaceNormal
    ;

- (const Pnt2D *) getTextureCoords
    ;

@end

/* ===========================================================================
    'ArpIntersectionEvalEnv'
=========================================================================== */
@protocol ArpIntersectionEvalEnv <ArpSurfacePointEvalEnv>

// TODO

@end

/* ===========================================================================
    'ArpPointInSpaceEvalEnv'
=========================================================================== */
@protocol ArpPointInSpaceEvalEnv <ArpPointEvalEnv>

// TODO

@end

/* ===========================================================================
    'ArpRayEndPointEvalEnv'
=========================================================================== */
@protocol ArpRayEndPointEvalEnv <ArpPointInSpaceEvalEnv>

// TODO

@end

// ===========================================================================
