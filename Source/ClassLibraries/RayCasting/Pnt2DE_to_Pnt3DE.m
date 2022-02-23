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

#define ART_MODULE_NAME     Pnt2DE_to_Pnt3DE

#import "Pnt2DE_to_Pnt3DE.h"
#import "FoundationAssertionMacros.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


void pnt2de_to_world_pnt3de(
        const Pnt2DE               * inPoint2D,
              ArTraversalState     * traversalState,
              ArNode <ArpShape>    * shapeThePointLiesOn,
              ArNode <ArpMapping>  * shapeParameterisation,
              Pnt3DE               * outPoint3D
        )
{
    [ shapeParameterisation getObjectSpacePnt3DE_for_Pnt2DE
        :   traversalState
        :   inPoint2D
        :   outPoint3D
        ];

    [ shapeThePointLiesOn calculateNormalForLocalPnt3DE
        :   traversalState
        :   outPoint3D
        ];

    pnt3de_trafo_p(
        ARTS_TRAFO(*traversalState),
        outPoint3D
        );
}

void pnt2de_to_tested_world_pnt3de(
        const Pnt2DE               * inPoint2D,
              ArTraversalState     * traversalState,
              ArNode <ArpShape>    * shapeThePointLiesOn,
              ArNode <ArpMapping>  * shapeParameterisation,
              ArnRayCaster         * rayCaster,
              Pnt3DE               * outPoint3D,
              BOOL                 * outValid
        )
{
    pnt2de_to_world_pnt3de(
        inPoint2D,
        traversalState,
        shapeThePointLiesOn,
        shapeParameterisation,
        outPoint3D
        );

    if ( rayCaster )
    {
        *outValid = NO;

        ArcSurfacePoint  * surfacePoint = 0;

        ASSERT_PROTOCOL_SUPPORT(
            ARTS_WORLD(*traversalState),
            ArpRayCasting
            );

        [ rayCaster getArcSurfacePoint_for_WorldPnt3DE
            :   ((ArNode <ArpRayCasting> *)ARTS_WORLD(*traversalState))
            :   outPoint3D
            :   shapeThePointLiesOn
            : & surfacePoint
            ];

        if ( surfacePoint )
        {
            *outValid = YES;

            if( ARCSURFACEPOINT_HAS_BEEN_CSG_SUBTRACTED(surfacePoint) )
                vec3d_d_mul_v(
                     -1.0,
                    & PNT3DE_NORMAL(*outPoint3D)
                    );

            [ ARNRAYCASTER_SURFACEPOINT_FREELIST(rayCaster) releaseInstance
                :   surfacePoint
                ];
        }
    }
    else
        ART_ERRORHANDLING_FATAL_ERROR(
            "pnt2de_to_tested_world_pnt3de called "
            "without valid raycaster object"
            );
}

// ===========================================================================
