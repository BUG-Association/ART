/* ===========================================================================

    Copyright (c) 1996-2018 The ART Development Team
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

#define ART_MODULE_NAME     ArcAreaLightsourceSampling

#import "ArpLightsourceSampling_Categories.h"
#import "LightsourceSamplingCommonMacros.h"
#import "ArpRayCasting_Categories.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArcAreaLightsource( RaySampling )

- (BOOL) sampleLightsource
        : (      ArcPointContext *)              illuminatedPoint /* optional */
        : (      ArLightsourceSamplingContext *) samplingContext
        : (const ArWavelength *)                 wavelength
        : (      ArSamplingRegion *)             samplingRegion
        : (      ArcSurfacePoint **)             sampledPoint
        : (      ArDirectionCosine *)            sampledDirection
        : (      ArPDFValue *)                   illuminationProbability /* optional */
        : (      ArPDFValue *)                   emissionProbability /* optional */
        : (      ArLightSample *)                lightSample
{
    /* -----------------------------------------------------------------------

        Random selection of the actual starting patch based on the precomputed
        relative emission of each of them (i.e. the brightest patch gets to
        start more paths than the others).

    --------------------------------------------------------------------aw- */
    
    double  powerThreshold = [ RANDOM_GENERATOR valueFromNewSequence ];
    int     i = 0;

    while( patch[i].lightsourceRadiantPowerPercentile < powerThreshold ) i++;
    
    /* -----------------------------------------------------------------------

        We then generate a random point on the selected patch. Since CSG
        operations might have clipped the part of the patch a random point
        lies on, we have to repeat this random point generation until we
        obtain a valid surface point.

        This rejection sampling is wasteful, but the only viable option if
        one wants to have the luxury of arbitrarily shaped, CSG-modelled
        lightsources.

    --------------------------------------------------------------------aw- */
    
    do
    {
        /* ------------------------------------------------------------------

            Generation of a random 2D point on the surface of the selected
            patch, and setting of the correct path index for the 2D surface
            point.

        ---------------------------------------------------------------aw- */

        Pnt2DE  start2D;

        PNT2DE_PATCHINDEX(start2D) = patch[i].index;

        [ patch[i].parameterisation getRandomPoint
            : & PNT2DE_COORD(start2D)
            :   RANDOM_GENERATOR
            ];

        /* ------------------------------------------------------------------

            Transformation from 2D surface point -> 3D object space point.

        ---------------------------------------------------------------aw- */

        Pnt3DE  start3D;

        [ MAPPING_2D_TO_3D getObjectSpacePnt3DE_for_Pnt2DE
            : & traversalState
            : & start2D
            : & start3D
            ];

        [ shape calculateNormalForLocalPnt3DE
            : & traversalState
            : & start3D
            ];

        /* -------------------------------------------------------------------

            Transformation from 3D object space -> 3D world coordinates.

        ----------------------------------------------------------------aw- */

        pnt3de_trafo_p(
              TRAFO_OBJSPACE_TO_WORLDSPACE,
            & start3D
            );

        /* -------------------------------------------------------------------

            Generation of a pointHit data structure for the new 3D point.
            This is where the test whether the point is valid is performed;
            a valid surfaceInfo struct is the sign for a valid surface point.

        ----------------------------------------------------------------aw- */

        [ RAYCASTER getArcSurfacePoint_for_WorldPnt3DE
            :   ((ArNode <ArpRayCasting>*)ENTIRE_SCENE)
            : & start3D
            :   shape
            :   sampledPoint
            ];
    }
    while ( ! *sampledPoint );

    if(illuminatedPoint)
    {
        // only one possible direction can be generated
        
        vec3d_pp_sub_v(
           & ARCPOINTCONTEXT_WORLDSPACE_POINT(*sampledPoint),
           & ARCPOINTCONTEXT_WORLDSPACE_POINT(illuminatedPoint),
           & ARDIRECTIONCOSINE_VECTOR(*sampledDirection)
           );
        
        vec3d_norm_v(
            & ARDIRECTIONCOSINE_VECTOR(*sampledDirection)
            );
        
        ARDIRECTIONCOSINE_COSINE(*sampledDirection) =
            vec3d_vv_dot(
              & ARDIRECTIONCOSINE_VECTOR(*sampledDirection),
              & ARCSURFACEPOINT_WORLDSPACE_NORMAL(*sampledPoint)
            );
        
        if(illuminationProbability)
        {
            double pdf =
                  patch[i].percentOfLightsourceRadiantPower
                / patch[i].area;
            
            arpdfvalue_dd_init_p(
                pdf,
                pdf,
                illuminationProbability
              );
        }
        
        if(emissionProbability)
        {
            ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
        }
    }
    else
    {
        ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
    }
    
    [ SHAPE_SURFACE_MATERIAL calculateEmittedLightSample
        :   *sampledPoint
        :   sampledDirection
        :   wavelength
        :   samplingRegion
        :   lightSample
        ];
    
    return YES;
}

- (void) sampleProbability
        : (      ArcPointContext *)              illuminatedPoint
        : (      ArcSurfacePoint *)              lightSamplePoint
        : (const ArDirectionCosine *)            lightSampleDirection
        : (const ArWavelength *)                 wavelength
        : (const ArSamplingRegion *)             samplingRegion
        : (      ArPDFValue *)                   illuminationProbability /* optional */
        : (      ArPDFValue *)                   emissionProbability /* optional */
{
    if(illuminationProbability)
    {
        double pdf =
              patch[*samplingRegion].percentOfLightsourceRadiantPower
            / patch[*samplingRegion].area;

        arpdfvalue_dd_init_p(
            pdf,
            pdf,
            illuminationProbability
          );
    }
    
    if(emissionProbability)
    {
        ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
    }
}


@end

// ===========================================================================
