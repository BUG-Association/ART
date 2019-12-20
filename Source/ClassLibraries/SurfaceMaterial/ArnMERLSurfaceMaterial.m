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


#define ART_MODULE_NAME     ArnMERLSurfaceMaterial

#import "ArnMERLSurfaceMaterial.h"

#import "SurfaceMaterialMacros.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    [ ArnMERLSurfaceMaterial registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

#include "merl.h"

/* ===========================================================================
    'ArnMERLSurfaceMaterial'
=========================================================================== */
@implementation ArnMERLSurfaceMaterial

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnMERLSurfaceMaterial)

ARPSURFACEMATERIAL_DEFAULT_NONDIFFUSE_NONEMISSIVE_IMPLEMENTATION
ARPSURFACEMATERIAL_DEFAULT_SURFACETYPE_IMPLEMENTATION(
      ARSURFACETYPE_GENERIC_REFLECTIONS,
      YES
    )

- (id) init
{
    self =
        [ super  init ];
    
    return self;
}
    
- (void) dealloc
{
    ardoublearray_free_contents(&brdf);
	[ super dealloc ];
}

- (void) getMeasuredSample
    : (const Vec3D               *) localI
    : (const Vec3D               *) localO
    : (const ArWavelength        *) incomingWavelength
    : (      ArPathDirection      ) pathDirection
    : (      ArAttenuationSample *) attenuationSample
{
    Vec3D li, lo;

	vec3d_v_negate_v(localI, &li);
    
    double r, g, b;
    lookup_brdf_val(ardoublearray_array(&brdf), &li, localO, &r, &g, &b);

	ArRGB rgb = ARRGB(r, g, b);
	
	ArSpectrum500 s;
    rgb_to_s500(art_gv, &rgb, &s);

	ArSpectralSample temp_ss;
    sps_s500w_init_s(
        art_gv,
        & s,
          incomingWavelength,
        & temp_ss
        );
                
    arattenuationsample_s_init_a(
          art_gv,
        & temp_ss,
          attenuationSample
        );
}

- (BOOL) calculateBidirectionalAttenuationSample
        : (      ArcIntersection               *) incomingDirectionAndLocation
        : (      struct ArDirectionCosine      *) outgoingDirection
        : (      ArPathDirection                ) pathDirection
        : (      ArBSDFSampleGenerationContext *) context
        : (const ArWavelength                  *) incomingWavelength
        : (const ArWavelength                  *) outgoingWavelength
        : (      ArPDFValue                    *) sampleProbability
        : (      ArPDFValue                    *) reverseSampleProbability
        : (      ArAttenuationSample           *) attenuationSample
{

	if ( INCOMING_COSINE_WORLDSPACE <= 0.0 )
		return NO;
	
    if ( OUTGOING_COSINE_WORLDSPACE > 0.0 )
    {
        if ( ! arwavelength_ww_equal(
                    art_gv,
                    incomingWavelength,
                    outgoingWavelength
             ) )
        {
            return NO;
        }
            
        if ( sampleProbability )
        {
            arpdfvalue_d_init_p(
                  OUTGOING_COSINE_WORLDSPACE / MATH_PI,
                  sampleProbability
                );
        }
            
        if ( reverseSampleProbability )
        {
            arpdfvalue_d_init_p(
                  INCOMING_COSINE_WORLDSPACE / MATH_PI,
                  reverseSampleProbability
               );
        }
            
        if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE )
        {
            ART_ERRORHANDLING_FATAL_ERROR_WITH_CODE(
                ART_ERROR_SURFACE_MATERIAL_NOT_POLARISATION_CAPABLE,
                "There is no sensible way to use a MERL surface in a "
                "polarisation renderer. Use Torrance Sparrow, or a layered "
                "surface instead!"
                );
        }
        else
        {
            // Gives localI & localO
            TRANSFORM_BRDF_DIRECTIONS_TO_LOCAL_SYSTEM;    

            [ self getMeasuredSample
                : & localI
                : & localO
                :   incomingWavelength
                :   pathDirection
                :   attenuationSample
              ];
        }
		
        return YES;
    }
    else
    {
        if ( sampleProbability ) *sampleProbability = ARPDFVALUE_ZERO;
        if ( reverseSampleProbability ) *reverseSampleProbability = ARPDFVALUE_ZERO;

        ARATTENUATIONSAMPLE_VVV_PD_INIT_AS_BLOCKER_A(
            & INCOMING_VECTOR_WORLDSPACE,
            & SURFACE_NORMAL_WORLDSPACE,
            & OUTGOING_VECTOR_WORLDSPACE,
              pathDirection,
              attenuationSample
            );

        return NO;
    }
}

- (BOOL) calculateSingleBSDFSample
        : (      ArcIntersection               *) incomingDirectionAndLocation
        : (      ArPathDirection                ) pathDirection
        : (      ArBSDFSampleGenerationContext *) context
        : (const ArWavelength                  *) incomingWavelength
        : (      ArWavelength                  *) sampledWavelength
        : (      ArDirectionCosine             *) sampledDirection
        : (      ArPDFValue                    *) sampleProbability
        : (      ArPDFValue                    *) reverseSampleProbability
        : (      ArAttenuationSample           *) attenuationSample
{
	Trafo3D  local2world;

    trafo3d_v_local2world_from_worldspace_normal_t(
        & SURFACE_NORMAL_WORLDSPACE,
        & local2world
        );
	
    Vec3D  localO;
    
    SAMPLE_HEMISPHERE_COSINE_WEIGHTED(
        localO,
        ARDIRECTIONCOSINE_VECTOR(*sampledDirection)
        );

    ARDIRECTIONCOSINE_COSINE(*sampledDirection) =
        vec3d_vv_dot(
            & ARDIRECTIONCOSINE_VECTOR(*sampledDirection),
            & SURFACE_NORMAL_WORLDSPACE
            );
    
    *sampledWavelength = *incomingWavelength;
    
    arpdfvalue_d_init_p(
          SAMPLED_COSINE_WORLDSPACE / MATH_PI,
          sampleProbability
        );
        
    if(reverseSampleProbability)
    {
        arpdfvalue_d_init_p(
              INCOMING_COSINE_WORLDSPACE / MATH_PI,
              reverseSampleProbability
            );
    }
        
    if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE )
    {
        ART_ERRORHANDLING_FATAL_ERROR_WITH_CODE(
            ART_ERROR_SURFACE_MATERIAL_NOT_POLARISATION_CAPABLE,
            "There is no sensible way to use a MERL surface in a "
            "polarisation renderer. Use Torrance Sparrow, or a layered "
            "surface instead!"
            );
    }
    else
    {
        // Gives localI
        TRANSFORM_BSDFSAMPLE_DIRECTIONS_TO_LOCAL_SYSTEM;

        [ self getMeasuredSample
            : & localI
            : & localO
            :   incomingWavelength
            :   pathDirection
            :   attenuationSample
          ];
    }

	return YES;
}

- (void) code
    : (ArcObject <ArpCoder>*) coder
{
    [ super code: coder ];

    arpcoder_ardoublearray(coder, &brdf);
}

@end

// ===========================================================================
