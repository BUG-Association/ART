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

#define ART_MODULE_NAME     ArnLambertSurfaceMaterial

#import "ArnLambertSurfaceMaterial.h"

#import "SurfaceMaterialMacros.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnLambertSurfaceMaterial registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

/* ===========================================================================
    'ArnLambertSurfaceMaterial'
=========================================================================== */
@implementation ArnLambertSurfaceMaterial

#define SUB_SPECTRUM2D      ((ArNode <ArpSpectrum2D>*) ARNUNARY_SUBNODE)

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnLambertSurfaceMaterial)
ARPSURFACEMATERIAL_DEFAULT_NONEMISSIVE_IMPLEMENTATION
ARPSURFACEMATERIAL_DEFAULT_WAVELENGTH_SHIFTING_SURFACETYPE_IMPLEMENTATION(
    arsurface_generates_perfectly_diffuse_reflections,
    YES
    )

- (BOOL) calculateAlbedoSampleAtWavelength
        : (      ArcSurfacePoint *) location
        : (const ArWavelength *) wavelength
        : (      ArSpectralSample *) albedo
{
    [ SUB_COLOUR getSpectralSample
        :   location
        :   wavelength
        :   albedo
        ];

    return YES;
}

- (BOOL) sampleWavelengthShift
        : (      ArcIntersection               *) incomingDirectionAndLocation
        : (      ArPathDirection                ) pathDirection
        : (      ArBSDFSampleGenerationContext *) context
        : (const ArWavelength                  *) incomingWavelength
        : (      ArWavelength                  *) sampledWavelength
        : (      ArPDFValue                    *) shiftProbability
{
    if ( [ SUB_COLOUR isFluorescent ] )
    {
        ArSpectralSample dummyAttenuation;
        
        return
            [ SUB_SPECTRUM2D randomWavelengthShift
                :   incomingDirectionAndLocation
                :   incomingWavelength
                :   RANDOM_GENERATOR
                :   pathDirection
                :   sampledWavelength
                : & dummyAttenuation
                :   shiftProbability
                ];
    }
    else
    {
        *sampledWavelength = *incomingWavelength;
        *shiftProbability  = ARPDFVALUE_UNIT_INFINITY;
        return YES;
    }
}

- (BOOL) calculateWavelengthShiftProbability
        : (      ArcIntersection               *) incomingDirectionAndLocation
        : (      ArPathDirection                ) pathDirection
        : (      ArBSDFSampleGenerationContext *) context
        : (const ArWavelength                  *) incomingWavelength
        : (const ArWavelength                  *) outgoingWavelength
        : (      ArPDFValue                    *) shiftProbability
{
    if ( [ SUB_COLOUR isFluorescent ] )
    {
        ArSpectralSample dummyAttenuation;
        return
            [ SUB_SPECTRUM2D attenuationForWavelengthShift
                :   incomingDirectionAndLocation
                :   incomingWavelength
                :   outgoingWavelength
                :   pathDirection
                : & dummyAttenuation
                :   shiftProbability
                ];
    }
    else
    {
        if ( ! arwavelength_ww_equal(
                    art_gv,
                    incomingWavelength,
                    outgoingWavelength
              ) )
        {
            return NO;
        }
        
        *shiftProbability = ARPDFVALUE_UNIT_INFINITY;
        return YES;
    }
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
    if ( OUTGOING_COSINE_WORLDSPACE > 0.0 )
    {
        if ( [ SUB_COLOUR isFluorescent ] )
        {
            ArSpectralSample attenuation;
            if(![ SUB_SPECTRUM2D attenuationForWavelengthShift
                    :   incomingDirectionAndLocation
                    :   incomingWavelength
                    :   outgoingWavelength
                    :   pathDirection
                    : & attenuation
                    :   sampleProbability
                ])
                return NO;
            
            if(sampleProbability)
            {
                arpdfvalue_d_mul_p(
                      OUTGOING_COSINE_WORLDSPACE / MATH_PI,
                      sampleProbability
                    );
            }
            // todo: reverseSampleProbability
            
            ARATTENUATIONSAMPLE_VVV_PD_C_INIT_DEPOLARISING_A(
                & INCOMING_VECTOR_WORLDSPACE,
                & SURFACE_NORMAL_WORLDSPACE,
                & OUTGOING_VECTOR_WORLDSPACE,
                  pathDirection,
                & attenuation,
                  attenuationSample
            );
        }
        else
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
                ArReferenceFrame  refframe_entry, refframe_exit;

                arreframe_vvv_pd_init_rf_rf(
                      art_gv,
                    & INCOMING_VECTOR_WORLDSPACE,
                    & SURFACE_NORMAL_WORLDSPACE,
                    & OUTGOING_VECTOR_WORLDSPACE,
                      pathDirection,
                    & refframe_entry,
                    & refframe_exit
                    );

                [ SUB_COLOUR getDepolarisingAttenuationSample
                    :   incomingDirectionAndLocation
                    : & refframe_entry
                    : & refframe_exit
                    :   incomingWavelength
                    :   attenuationSample
                    ];
            }
            else
            {
                [ SUB_COLOUR getAttenuationSample
                    :   incomingDirectionAndLocation
                    :   incomingWavelength
                    :   attenuationSample
                    ];
            }
        }

        arattenuationsample_d_mul_a(
            art_gv,
            MATH_1_DIV_PI,
            attenuationSample
            );

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
    
    if ( [ SUB_COLOUR isFluorescent ] )
    {
        ArSpectralSample attenuation;
        
        if(![ SUB_SPECTRUM2D randomWavelengthShift
                :   incomingDirectionAndLocation
                :   incomingWavelength
                :   RANDOM_GENERATOR
                :   pathDirection
                :   sampledWavelength
                : & attenuation
                :   sampleProbability
                ])
            return NO;
        
        arpdfvalue_d_mul_p(
              SAMPLED_COSINE_WORLDSPACE / MATH_PI, // probability of direction
              sampleProbability
            );
        
        // todo: reverseSampleProbability
        
        ARATTENUATIONSAMPLE_VVV_PD_C_INIT_DEPOLARISING_A(
            & INCOMING_VECTOR_WORLDSPACE,
            & SURFACE_NORMAL_WORLDSPACE,
            & ARDIRECTIONCOSINE_VECTOR(*sampledDirection),
              pathDirection,
            & attenuation,
              attenuationSample
        );
    }
    else
    {
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
            ArReferenceFrame  refframe_entry, refframe_exit;

            arreframe_vvv_pd_init_rf_rf(
                  art_gv,
                & INCOMING_VECTOR_WORLDSPACE,
                & SURFACE_NORMAL_WORLDSPACE,
                & ARDIRECTIONCOSINE_VECTOR(*sampledDirection),
                  pathDirection,
                & refframe_entry,
                & refframe_exit
                );

            [ SUB_COLOUR getDepolarisingAttenuationSample
                :   incomingDirectionAndLocation
                : & refframe_entry
                : & refframe_exit
                :   incomingWavelength
                :   attenuationSample
                ];
        }
        else
        {
            [ SUB_COLOUR getAttenuationSample
                :   incomingDirectionAndLocation
                :   incomingWavelength
                :   attenuationSample
                ];
        }
    }

    arattenuationsample_d_mul_a(
          art_gv,
          MATH_1_DIV_PI,
          attenuationSample
        );
    
    return YES;
}

@end

// ===========================================================================
