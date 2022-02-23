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

#define ART_MODULE_NAME     ArcSurfacePointForShape

#import "ArcSurfacePointForShape.h"

#import "ArnRayCaster.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


void arcsurfacepoint_for_singular_shape(
        id                  shape,
        ArnRayCaster      * rayCaster,
        ArcSurfacePoint  ** surfacePoint
        )
{
    if ( ARNRAYCASTER_TEST_SHAPE(rayCaster) == shape )
    {
        *surfacePoint =
            [ ARNRAYCASTER_SURFACEPOINT_FREELIST(rayCaster)
                obtainInstance
                ];

        ARCSURFACEPOINT_SHAPE(*surfacePoint) =
            ARNRAYCASTER_TEST_SHAPE(rayCaster);

        ARCSURFACEPOINT_VOLUME_MATERIAL_OUTSIDE_REF(*surfacePoint) = ARNODEREF_NONE;

        ARCSURFACEPOINT_VOLUME_MATERIAL_INSIDE_REF(*surfacePoint) = ARNODEREF_NONE;

        ARCSURFACEPOINT_SET_WORLDSPACE_POINT(
            *surfacePoint,
            ARNRAYCASTER_TEST_WORLD_PNT3D(rayCaster)
            );

        ARCSURFACEPOINT_WORLDSPACE_NORMAL(*surfacePoint) =
            ARNRAYCASTER_TEST_WORLD_PNT3DE_NORMAL(rayCaster);

        ARCSURFACEPOINT_STATUS_FLAGS(*surfacePoint) |=
            arspflag_worldspace_normal_valid;

        ARCSURFACEPOINT_SET_OBJECTSPACE_POINT(
            *surfacePoint,
            ARNRAYCASTER_TEST_PNT3D(rayCaster)
            );

        ARCSURFACEPOINT_OBJECTSPACE_NORMAL(*surfacePoint) =
            ARNRAYCASTER_TEST_PNT3DE_NORMAL(rayCaster);

        ARCSURFACEPOINT_STATUS_FLAGS(*surfacePoint) |=
            arspflag_objectspace_normal_valid;

        ARCSURFACEPOINT_FACE_ID(*surfacePoint) =
            ARNRAYCASTER_TEST_PNT3DE_FACEINDEX(rayCaster);
    }
}

void arcsurfacepoint_for_shape(
        id                  shape,
        ArnRayCaster      * rayCaster,
        ArcSurfacePoint  ** surfacePoint
        )
{
    if ( ARNRAYCASTER_TEST_SHAPE(rayCaster) == shape )
    {
        *surfacePoint =
            [ ARNRAYCASTER_SURFACEPOINT_FREELIST(rayCaster)
                obtainInstance
                ];

        ARCSURFACEPOINT_SHAPE(*surfacePoint) =
            ARNRAYCASTER_TEST_SHAPE(rayCaster);

        ARCSURFACEPOINT_VOLUME_MATERIAL_OUTSIDE_REF(*surfacePoint) = ARNODEREF_NONE;

        COPY_OBJECT_REF(
            ARNRAYCASTER_VOLUME_MATERIAL_REF(rayCaster),
            ARCSURFACEPOINT_VOLUME_MATERIAL_INSIDE_REF(*surfacePoint)
            );

        ARCSURFACEPOINT_SET_WORLDSPACE_POINT(
            *surfacePoint,
            ARNRAYCASTER_TEST_WORLD_PNT3D(rayCaster)
            );

        ARCSURFACEPOINT_WORLDSPACE_NORMAL(*surfacePoint) =
            ARNRAYCASTER_TEST_WORLD_PNT3DE_NORMAL(rayCaster);

        ARCSURFACEPOINT_STATUS_FLAGS(*surfacePoint) |=
            arspflag_worldspace_normal_valid;

        ARCSURFACEPOINT_SET_OBJECTSPACE_POINT(
            *surfacePoint,
            ARNRAYCASTER_TEST_PNT3D(rayCaster)
            );

        ARCSURFACEPOINT_OBJECTSPACE_NORMAL(*surfacePoint) =
            ARNRAYCASTER_TEST_PNT3DE_NORMAL(rayCaster);

        ARCSURFACEPOINT_STATUS_FLAGS(*surfacePoint) |=
            arspflag_objectspace_normal_valid;

        ARCSURFACEPOINT_FACE_ID(*surfacePoint) =
            ARNRAYCASTER_TEST_PNT3DE_FACEINDEX(rayCaster);
    }
}

// ===========================================================================
