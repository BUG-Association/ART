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

#ifndef _ARCPOINTCONTEXT_H_
#define _ARCPOINTCONTEXT_H_

#include "ART_Foundation.h"

ART_MODULE_INTERFACE(ArcPointContext)

#import "ArTraversalState.h"
#import "ArpEvaluationEnvironment.h"

@class ArcPhaseInterfaceCache;

typedef enum
{
    arpcflag_all_variables_are_invalid = 0x00,
    arpcflag_objectspace_point_valid   = 0x01,
    arpcflag_worldspace_point_valid    = 0x02
}
ArPointContextFlags;

@interface ArcPointContext
        : ArcObject <ArpPointEvalEnv>
{
@public
    ArcPointContext      * next;
    ArcPointContext      * prev;
    ArPointContextFlags    pc_status_flags;
    ArTraversalState       traversalState;
    Pnt3D                  objectspace_point;
    Pnt3D                  worldspace_point;
}

- (void) prepareForUse
        : (ArcPhaseInterfaceCache *) phaseInterfaceCache
        ;

- (void) activate
        ;

- (void) deactivate
        ;

- (void) debugprintf
        ;

@end

#define ARCPOINTCONTEXT_NEXT(__sp)              (__sp)->next
#define ARCPOINTCONTEXT_PREV(__sp)              (__sp)->prev
#define ARCPOINTCONTEXT_STATUS_FLAGS(__sp)      (__sp)->pc_status_flags
#define ARCPOINTCONTEXT_TRAVERSALSTATE(__sp)    (__sp)->traversalState


//   ------   world space point access macros   ------------------------------


#define ARCPOINTCONTEXT_WORLDSPACE_POINT_IS_VALID(__sp) \
\
((  ARCPOINTCONTEXT_STATUS_FLAGS(__sp) \
  & arpcflag_worldspace_point_valid ) != 0 )

#define ARCPOINTCONTEXT_WORLDSPACE_POINT(__sp)  (__sp)->worldspace_point

#define ARCPOINTCONTEXT_SET_WORLDSPACE_POINT(__sp,__p) \
{ \
    ARCPOINTCONTEXT_FLAG_WORLDSPACE_POINT_AS_VALID(__sp); \
    (__sp)->worldspace_point = (__p); \
}

#define ARCPOINTCONTEXT_FLAG_WORLDSPACE_POINT_AS_VALID(__sp) \
{ \
    ARCPOINTCONTEXT_STATUS_FLAGS(__sp) |= arpcflag_worldspace_point_valid; \
}


//   ------   object space point access macros   -----------------------------


#define ARCPOINTCONTEXT_OBJECTSPACE_POINT(__sp) (__sp)->objectspace_point

#define ARCPOINTCONTEXT_OBJECTSPACE_POINT_IS_VALID(__sp) \
\
((  ARCPOINTCONTEXT_STATUS_FLAGS(__sp) \
  & arpcflag_objectspace_point_valid ) != 0 )

#define ARCPOINTCONTEXT_SET_OBJECTSPACE_POINT(__sp,__p) \
{ \
    ARCPOINTCONTEXT_STATUS_FLAGS(__sp) |= arpcflag_objectspace_point_valid; \
    (__sp)->objectspace_point = (__p); \
}


//   ------   traversal state component access macros   ----------------------


#define ARCPOINTCONTEXT_ENVIRONMENT_MATERIAL_REF(__sp) \
    ARTS_ENVIRONMENT_MATERIAL_REF(ARCPOINTCONTEXT_TRAVERSALSTATE(__sp))
#define ARCPOINTCONTEXT_ENVIRONMENT_MATERIAL(__sp) \
    ARTS_ENVIRONMENT_MATERIAL(ARCPOINTCONTEXT_TRAVERSALSTATE(__sp))

#define ARCPOINTCONTEXT_SURFACE_MATERIAL_REF(__sp) \
    ARTS_SURFACE_MATERIAL_REF(ARCPOINTCONTEXT_TRAVERSALSTATE(__sp))
#define ARCPOINTCONTEXT_SURFACE_MATERIAL(__sp) \
    ARTS_SURFACE_MATERIAL(ARCPOINTCONTEXT_TRAVERSALSTATE(__sp))

#define ARCPOINTCONTEXT_TRAFO_REF(__sp) \
    ARTS_TRAFO_REF(ARCPOINTCONTEXT_TRAVERSALSTATE(__sp))
#define ARCPOINTCONTEXT_TRAFO(__sp) \
    ARTS_TRAFO(ARCPOINTCONTEXT_TRAVERSALSTATE(__sp))

#define ARCPOINTCONTEXT_VARIABLES(__sp) \
    ARTS_VARIABLES(ARCPOINTCONTEXT_TRAVERSALSTATE(__sp))

#endif // _ARCPOINTCONTEXT_H_

// ===========================================================================
