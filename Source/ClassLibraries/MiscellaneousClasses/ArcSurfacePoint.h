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

#ifndef _ARCSURFACEPOINT_H_
#define _ARCSURFACEPOINT_H_

#include "ART_Foundation.h"
#import "ArcPhaseInterfaceIsotropic.h"


ART_MODULE_INTERFACE(ArcSurfacePoint)

#include "ArcPointContext.h"

typedef enum
{
    arspflag_all_variables_are_invalid = 0x00,
    arspflag_texture_coords_valid      = 0x01,
    arspflag_objectspace_normal_valid  = 0x02,
    arspflag_worldspace_normal_valid   = 0x04
}
ArSurfacePointFlags;

@interface ArcSurfacePoint
        : ArcPointContext <ArpSurfacePointEvalEnv>
{
@public
    ArSurfacePointFlags           sp_status_flags;
    Pnt2D                         texture_coordinates;
    Vec3D                         objectspace_normal;
    Vec3D                         worldspace_normal;
    ArSurfaceType                 surfaceType;
    ArFaceOnShape                 faceOnShape;
    ArNode <ArpShape>           * shape;
    ArNodeRef                     materialInsideRef;
    ArNodeRef                     materialOutsideRef;
    ArcPhaseInterfaceIsotropic  * phaseInterface;
}

- (void) flipOrientation
        ;

@end

#define ARCSURFACEPOINT_NEXT                    ARCPOINTCONTEXT_NEXT
#define ARCSURFACEPOINT_PREV                    ARCPOINTCONTEXT_PREV
#define ARCSURFACEPOINT_TRAVERSALSTATE          ARCPOINTCONTEXT_TRAVERSALSTATE
#define ARCSURFACEPOINT_STATUS_FLAGS(__sp)      (__sp)->sp_status_flags

#define ARCSURFACEPOINT_TEXTURE_COORDS(__sp)    (__sp)->texture_coordinates
#define ARCSURFACEPOINT_TRAFO                   ARCPOINTCONTEXT_TRAFO
#define ARCSURFACEPOINT_ENVIRONMENT_MATERIAL    ARCPOINTCONTEXT_ENVIRONMENT_MATERIAL
#define ARCSURFACEPOINT_SURFACE_MATERIAL        ARCPOINTCONTEXT_SURFACE_MATERIAL
#define ARCSURFACEPOINT_SURFACETYPE(__sp)       (__sp)->surfaceType
#define ARCSURFACEPOINT_SHAPE(__sp)             (__sp)->shape
#define ARCSURFACEPOINT_PHASEINTERFACE(__sp)    (__sp)->phaseInterface

#define ARCSURFACEPOINT_VOLUME_MATERIAL_INSIDE_REF(__sp) \
    (__sp)->materialInsideRef
#define ARCSURFACEPOINT_VOLUME_MATERIAL_INSIDE(__sp) \
    ( (ArNode <ArpVolumeMaterial> *) \
    ARNODEREF_POINTER(ARCSURFACEPOINT_VOLUME_MATERIAL_INSIDE_REF(__sp)) )
#define ARCSURFACEPOINT_VOLUME_MATERIAL_OUTSIDE_REF(__sp) \
    (__sp)->materialOutsideRef
#define ARCSURFACEPOINT_VOLUME_MATERIAL_OUTSIDE(__sp) \
    ( (ArNode <ArpVolumeMaterial> *) \
    ARNODEREF_POINTER(ARCSURFACEPOINT_VOLUME_MATERIAL_OUTSIDE_REF(__sp)) )

#define ARCSURFACEPOINT_FACE_TYPE(__sp)           \
    ARFACE_ON_SHAPE_TYPE((__sp)->faceOnShape)
#define ARCSURFACEPOINT_FACE_ID(__sp)           \
    ARFACE_ON_SHAPE_FACE((__sp)->faceOnShape)
#define ARCSURFACEPOINT_SURFACE_IS_REFRACTIVE(__sp) \
    ARSURFACETYPE_IS_REFRACTIVE(ARCSURFACEPOINT_SURFACETYPE(__sp))
#define ARCSURFACEPOINT_SURFACE_IS_EMITTER(__sp) \
    ARSURFACETYPE_IS_EMITTER(ARCSURFACEPOINT_SURFACETYPE(__sp))
#define ARCSURFACEPOINT_SURFACE_IS_DIRECT_ILLUMINATION_MATTE(__sp) \
    ARSURFACETYPE_IS_DIRECT_ILLUMINATION_MATTE( \
        ARCSURFACEPOINT_SURFACETYPE(__sp))
#define ARCSURFACEPOINT_SURFACE_IS_MATTE(__sp) \
    ARSURFACETYPE_IS_MATTE(ARCSURFACEPOINT_SURFACETYPE(__sp))

#define ARCSURFACEPOINT_TEXTURE_COORDS_ARE_VALID(__sp) \
    (( ARCSURFACEPOINT_STATUS_FLAGS(__sp) \
    & arspflag_texture_coords_valid ) != 0 )
#define ARCSURFACEPOINT_FLAG_TEXTURE_COORDS_AS_VALID(__sp) \
    (ARCSURFACEPOINT_STATUS_FLAGS(__sp) |= arspflag_texture_coords_valid )


//   ------   object space point access macros   -----------------------------


#define ARCSURFACEPOINT_OBJECTSPACE_POINT_IS_VALID \
    ARCPOINTCONTEXT_OBJECTSPACE_POINT_IS_VALID
#define ARCSURFACEPOINT_FLAG_OBJECTSPACE_POINT_AS_VALID \
    ARCPOINTCONTEXT_FLAG_OBJECTSPACE_POINT_AS_VALID
#define ARCSURFACEPOINT_OBJECTSPACE_POINT \
    ARCPOINTCONTEXT_OBJECTSPACE_POINT
#define ARCSURFACEPOINT_SET_OBJECTSPACE_POINT \
    ARCPOINTCONTEXT_SET_OBJECTSPACE_POINT


//   ------   object space normal access macros   ----------------------------


#define ARCSURFACEPOINT_OBJECTSPACE_NORMAL(__sp)      (__sp)->objectspace_normal

#define ARCSURFACEPOINT_OBJECTSPACE_NORMAL_IS_VALID(__sp) \
    (( ARCSURFACEPOINT_STATUS_FLAGS(__sp) & arspflag_objectspace_normal_valid ) != 0 )
#define ARCSURFACEPOINT_FLAG_OBJECTSPACE_NORMAL_AS_VALID(__sp) \
    (ARCSURFACEPOINT_STATUS_FLAGS(__sp) |= arspflag_objectspace_normal_valid )

#define ARCSURFACEPOINT_SET_OBJECTSPACE_NORMAL(__sp,__n) \
{ \
    ARCSURFACEPOINT_FLAG_OBJECTSPACE_NORMAL_AS_VALID(__sp); \
    (__sp)->objectspace_normal = (__n); \
}


//   ------   world space point access macros   ------------------------------


#define ARCSURFACEPOINT_WORLDSPACE_POINT_IS_VALID \
    ARCPOINTCONTEXT_WORLDSPACE_POINT_IS_VALID
#define ARCSURFACEPOINT_WORLDSPACE_POINT \
    ARCPOINTCONTEXT_WORLDSPACE_POINT
#define ARCSURFACEPOINT_SET_WORLDSPACE_POINT \
    ARCPOINTCONTEXT_SET_WORLDSPACE_POINT
#define ARCSURFACEPOINT_FLAG_WORLDSPACE_POINT_AS_VALID \
    ARCPOINTCONTEXT_FLAG_WORLDSPACE_POINT_AS_VALID


//   ------   world space normal access macros   -----------------------------


#define ARCSURFACEPOINT_WORLDSPACE_NORMAL(__sp)      (__sp)->worldspace_normal

#define ARCSURFACEPOINT_WORLDSPACE_NORMAL_IS_VALID(__sp) \
    (( ARCSURFACEPOINT_STATUS_FLAGS(__sp) & arspflag_worldspace_normal_valid ) != 0 )
#define ARCSURFACEPOINT_FLAG_WORLDSPACE_NORMAL_AS_VALID(__sp) \
    (ARCSURFACEPOINT_STATUS_FLAGS(__sp) |= arspflag_worldspace_normal_valid )

#define ARCSURFACEPOINT_SET_WORLDSPACE_NORMAL(__sp,__n) \
{ \
    ARCSURFACEPOINT_FLAG_WORLDSPACE_NORMAL_AS_VALID(__sp); \
    (__sp)->worldspace_normal = (__n); \
}

#define ARCSURFACEPOINT_HAS_BEEN_CSG_SUBTRACTED(__sp) \
    (  ARCSURFACEPOINT_FACE_TYPE(__sp) & arface_on_shape_has_been_CSG_subtracted )


#endif // _ARCSURFACEPOINT_H_

// ===========================================================================
