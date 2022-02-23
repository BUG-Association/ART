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

#ifndef _ARCRAYENDPOINT_H_
#define _ARCRAYENDPOINT_H_

#include "ART_Foundation.h"

ART_MODULE_INTERFACE(ArcRayEndpoint)

#import "ART_MiscellaneousClasses.h"
#include "ArcPointInSpace.h"

@interface ArcRayEndpoint
        : ArcPointInSpace <ArpRayEndpoint, ArpRayEndPointEvalEnv>
{
@public
    double  t;
    Ray3D   worldspace_incoming_ray;
    Ray3D   objectspace_incoming_ray;
}

@end


#define ARCRAYENDPOINT_TRAVERSALSTATE \
    ARCPOINTINSPACE_TRAVERSALSTATE
#define ARCRAYENDPOINT_NEXT(__endpoint) \
    ((ArcRayEndpoint *)(ARCPOINTINSPACE_NEXT(__endpoint)))
#define ARCRAYENDPOINT_PREV(__endpoint) \
    ((ArcRayEndpoint *)(ARCPOINTINSPACE_PREV(__endpoint)))
#define ARCRAYENDPOINT_OBJECTSPACE_POINT \
    ARCPOINTINSPACE_OBJECTSPACE_POINT
#define ARCRAYENDPOINT_SET_OBJECTSPACE_POINT \
    ARCPOINTINSPACE_SET_OBJECTSPACE_POINT
#define ARCRAYENDPOINT_WORLDSPACE_POINT \
    ARCPOINTINSPACE_WORLDSPACE_POINT
#define ARCRAYENDPOINT_SET_WORLD_POINT \
    ARCPOINTINSPACE_SET_WORLDSPACE_POINT


#define ARCRAYENDPOINT_VOLUME_MATERIAL_REF \
    ARCPOINTINSPACE_VOLUME_MATERIAL_REF
#define ARCRAYENDPOINT_VOLUME_MATERIAL \
    ARCPOINTINSPACE_VOLUME_MATERIAL


#define ARCRAYENDPOINT_VOLUME_MATERIAL_RETAIN_REF(__e,__r) \
    COPY_NODE_REF( \
        *(__r), \
        ARCRAYENDPOINT_VOLUME_MATERIAL_REF(__e) \
        );

#define ARCRAYENDPOINT_TRAFO \
    ARCPOINTINSPACE_TRAFO

#define ARCRAYENDPOINT_T(__endpoint) \
    (__endpoint)->t

#define ARCRAYENDPOINT_OBJECTSPACE_INCOMING_RAY(__endpoint) \
    (__endpoint)->objectspace_incoming_ray
#define ARCRAYENDPOINT_OBJECTSPACE_INCOMING_RAY_VECTOR(__endpoint) \
    RAY3D_VECTOR(ARCRAYENDPOINT_OBJECTSPACE_INCOMING_RAY(__endpoint))
#define ARCRAYENDPOINT_OBJECTSPACE_INCOMING_VECTOR \
    ARCRAYENDPOINT_OBJECTSPACE_INCOMING_RAY_VECTOR

#define ARCRAYENDPOINT_WORLDSPACE_INCOMING_RAY(__endpoint) \
    (__endpoint)->worldspace_incoming_ray
#define ARCRAYENDPOINT_WORLDSPACE_INCOMING_RAY_VECTOR(__endpoint) \
    RAY3D_VECTOR(ARCRAYENDPOINT_WORLDSPACE_INCOMING_RAY(__endpoint))
#define ARCRAYENDPOINT_WORLDSPACE_INCOMING_VECTOR \
    ARCRAYENDPOINT_WORLDSPACE_INCOMING_RAY_VECTOR
#define ARCRAYENDPOINT_WORLDSPACE_VECTOR \
    ARCRAYENDPOINT_WORLDSPACE_INCOMING_RAY_VECTOR

#define ARCRAYENDPOINT_OBJECTSPACE_POINT_IS_VALID \
    ARCPOINTINSPACE_OBJECTSPACE_POINT_IS_VALID
#define ARCRAYENDPOINT_WORLDSPACE_POINT_IS_VALID \
    ARCPOINTINSPACE_WORLDSPACE_POINT_IS_VALID

#define ARCRAYENDPOINT_FLAG_OBJECTSPACE_POINT_AS_VALID \
    ARCPOINTINSPACE_FLAG_OBJECTSPACE_POINT_AS_VALID
#define ARCRAYENDPOINT_FLAG_WORLD_POINT_AS_VALID \
    ARCPOINTINSPACE_FLAG_WORLDSPACE_POINT_AS_VALID

#define ARCRAYENDPOINT_NEXT_PTR(__endpoint)   ARCPOINTINSPACE_NEXT(__endpoint)
#define ARCRAYENDPOINT_PREV_PTR(__endpoint)   ARCPOINTINSPACE_PREV(__endpoint)

#define ARCRAYENDPOINT_OBJECTSPACE_PX(__i)  XC(ARCRAYENDPOINT_OBJECTSPACE_POINT(__i))
#define ARCRAYENDPOINT_OBJECTSPACE_PY(__i)  YC(ARCRAYENDPOINT_OBJECTSPACE_POINT(__i))
#define ARCRAYENDPOINT_OBJECTSPACE_PZ(__i)  ZC(ARCRAYENDPOINT_OBJECTSPACE_POINT(__i))
#define ARCRAYENDPOINT_WORLDSPACE_PX(__i)   XC(ARCRAYENDPOINT_WORLDSPACE_POINT(__i))
#define ARCRAYENDPOINT_WORLDSPACE_PY(__i)   YC(ARCRAYENDPOINT_WORLDSPACE_POINT(__i))
#define ARCRAYENDPOINT_WORLDSPACE_PZ(__i)   ZC(ARCRAYENDPOINT_WORLDSPACE_POINT(__i))


#endif // _ARCRAYENDPOINT_H_

// ===========================================================================
