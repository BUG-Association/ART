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

#ifndef _ARCPOINTINSPACE_H_
#define _ARCPOINTINSPACE_H_

#include "ART_Foundation.h"

ART_MODULE_INTERFACE(ArcPointInSpace)

#include "ArcPointContext.h"
#include "ArNode.h"
    
@interface ArcPointInSpace
        : ArcPointContext <ArpPointInSpaceEvalEnv>
{
@public
    ArNodeRef                     materialRef;
}

@end


#define ARCPOINTINSPACE_NEXT                    ARCPOINTCONTEXT_NEXT
#define ARCPOINTINSPACE_PREV                    ARCPOINTCONTEXT_PREV
#define ARCPOINTINSPACE_TRAVERSALSTATE          ARCPOINTCONTEXT_TRAVERSALSTATE

#define ARCPOINTINSPACE_TRAFO                   ARCPOINTCONTEXT_TRAFO
#define ARCPOINTINSPACE_VOLUME_MATERIAL_REF(__sp) \
    (__sp)->materialRef
#define ARCPOINTINSPACE_VOLUME_MATERIAL(__sp) \
    ( (ArNode <ArpVolumeMaterial> *) \
    ARNODEREF_POINTER(ARCPOINTINSPACE_VOLUME_MATERIAL_REF(__sp)) )

#define ARCPOINTINSPACE_VOLUME_MATERIAL_OUTSIDE_REF(__sp) \
    (__sp)->materialOutsideRef
#define ARCPOINTINSPACE_VOLUME_MATERIAL_OUTSIDE(__sp) \
    ( (ArNode <ArpVolumeMaterial> *) \
    ARNODEREF_POINTER(ARCPOINTINSPACE_VOLUME_MATERIAL_OUTSIDE_REF(__sp)) )


//   ------   object space point access macros   -----------------------------


#define ARCPOINTINSPACE_OBJECTSPACE_POINT_IS_VALID \
    ARCPOINTCONTEXT_OBJECTSPACE_POINT_IS_VALID
#define ARCPOINTINSPACE_FLAG_OBJECTSPACE_POINT_AS_VALID \
    ARCPOINTCONTEXT_FLAG_OBJECTSPACE_POINT_AS_VALID
#define ARCPOINTINSPACE_OBJECTSPACE_POINT \
    ARCPOINTCONTEXT_OBJECTSPACE_POINT
#define ARCPOINTINSPACE_SET_OBJECTSPACE_POINT \
    ARCPOINTCONTEXT_SET_OBJECTSPACE_POINT


//   ------   world space point access macros   ------------------------------


#define ARCPOINTINSPACE_WORLDSPACE_POINT_IS_VALID \
    ARCPOINTCONTEXT_WORLDSPACE_POINT_IS_VALID
#define ARCPOINTINSPACE_WORLDSPACE_POINT \
    ARCPOINTCONTEXT_WORLDSPACE_POINT
#define ARCPOINTINSPACE_SET_WORLDSPACE_POINT \
    ARCPOINTCONTEXT_SET_WORLDSPACE_POINT
#define ARCPOINTINSPACE_FLAG_WORLDSPACE_POINT_AS_VALID \
    ARCPOINTCONTEXT_FLAG_WORLDSPACE_POINT_AS_VALID



#endif // _ARCPOINTINSPACE_H_

// ===========================================================================
