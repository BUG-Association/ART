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

#define ART_MODULE_NAME     ArcSurfacePoint

#import "ArcSurfacePoint.h"
#import "ArcPhaseInterfaceCache.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArcSurfacePoint

- (id) init
{
    self = [ super init ];

    return self;
}

- (void) prepareForUse
        : (ArcPhaseInterfaceCache *) phaseInterfaceCache
{
    [ super prepareForUse
        :   phaseInterfaceCache
        ];

    //   This is a weak link, so we just remember it, but do not
    //   release it later.

    ARCSURFACEPOINT_PHASEINTERFACE(self) =
        [ phaseInterfaceCache getPhaseInterfaceForMaterials
            :   ARCSURFACEPOINT_VOLUME_MATERIAL_OUTSIDE(self)
            :   ARCSURFACEPOINT_VOLUME_MATERIAL_INSIDE(self)
            ];

    ARCSURFACEPOINT_SURFACETYPE(self) =
        [  ARCSURFACEPOINT_SURFACE_MATERIAL(self) setupSurface
            :   self
            :   0
            ];
}

- (void) flipOrientation
{
    ArNodeRef  temp =
        materialInsideRef;

    materialInsideRef = materialOutsideRef;
    materialOutsideRef = temp;
}

- (void) activate
{
    [ super activate ];

    sp_status_flags = arspflag_all_variables_are_invalid;

    shape           = 0;
    surfaceType     = 0;
    faceOnShape     = ARFACE_ON_SHAPE_NONE;

    materialInsideRef  = ARNODEREF_NONE;
    materialOutsideRef = ARNODEREF_NONE;
}

- (void) deactivate
{
    RELEASE_NODE_REF(materialInsideRef);
    RELEASE_NODE_REF(materialOutsideRef);

    [ super deactivate ];
}

- (void) dealloc
{
    RELEASE_NODE_REF(materialInsideRef);
    RELEASE_NODE_REF(materialOutsideRef);

    [ super dealloc ];
}

- (void) debugprintf
{
    [ super debugprintf ];

    debugprintf(
        "Shape: %p %s\n",
        shape,
        [ shape cStringClassName ]
        );
}

- (const Vec3D *) getWorldSpaceNormal
{
    return & worldspace_normal;
}

- (const Vec3D *) getObjectSpaceNormal
{
    return & objectspace_normal;
}

- (const Pnt2D *) getTextureCoords
{
    if ( ! ARCSURFACEPOINT_TEXTURE_COORDS_ARE_VALID(self) )
    {

        id mapping =
            [ ARCSURFACEPOINT_SHAPE(self) createMappingFor
                :   armapping_default
                ];

        Pnt3DE  p3;
        
        PNT3DE_FACEINDEX(p3) = ARCSURFACEPOINT_FACE_ID(self);
        PNT3DE_COORD(p3) = ARCSURFACEPOINT_OBJECTSPACE_POINT(self);
        PNT3DE_NORMAL(p3) = ARCSURFACEPOINT_OBJECTSPACE_NORMAL(self);
        
        Pnt2DE  p2;

        [ mapping getPnt2DE_for_ObjectSpacePnt3DE
            : & ARCSURFACEPOINT_TRAVERSALSTATE(self)
            : & p3
            : & p2
            ];

//   uncomment this to see a stream of all mapping translations
//   best used in conjunction with -j1

//        debugprintf("(%5.3f|%5.3f|%5.3f)->(%5.3f|%5.3f)\n", PNT3DE_XC(p3),PNT3DE_YC(p3),PNT3DE_ZC(p3),PNT2DE_XC(p2),PNT2DE_YC(p2))

        ARCSURFACEPOINT_TEXTURE_COORDS(self) = PNT2DE_COORD(p2);
        
        ARCSURFACEPOINT_FLAG_TEXTURE_COORDS_AS_VALID(self);
    }


    return & texture_coordinates;
}

@end

// ===========================================================================
