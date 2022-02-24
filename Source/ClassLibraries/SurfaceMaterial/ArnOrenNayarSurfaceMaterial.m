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

#define ART_MODULE_NAME     ArnOrenNayarSurfaceMaterial

#import "ArnOrenNayarSurfaceMaterial.h"

#import "SurfaceMaterialMacros.h"
#import "ArpEvaluationEnvironment.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
 
    [ ArnOrenNayarSurfaceMaterial registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


/* ===========================================================================
    'ArnOrenNayarSurfaceMaterial'
=========================================================================== */

@implementation ArnOrenNayarSurfaceMaterial

- (id) copy
{
    ArnOrenNayarSurfaceMaterial  * copiedInstance = [ super copy ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnOrenNayarSurfaceMaterial  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

#define SPECTRUM_SUBNODE    ((ArNode <ArpSpectrum,ArpSpectrum2D,ArpSpectrumValues>*) ARNBINARY_SUBNODE_0)
#define SIGMA_SUBNODE       ((ArNode <ArpDoubleValues>*) ARNBINARY_SUBNODE_1)


ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnOrenNayarSurfaceMaterial)
ARPSURFACEMATERIAL_DEFAULT_NONEMISSIVE_IMPLEMENTATION
ARPSURFACEMATERIAL_DEFAULT_WAVELENGTH_SHIFTING_SURFACETYPE_IMPLEMENTATION(
    arsurface_generates_perfectly_diffuse_reflections,
    YES
    )

- (BOOL) sampleWavelengthShift
        : (      ArcIntersection               *) incomingDirectionAndLocation
        : (      ArPathDirection                ) pathDirection
        : (      ArBSDFSampleGenerationContext *) context
        : (const ArWavelength                  *) incomingWavelength
        : (      ArWavelength                  *) sampledWavelength
        : (      ArPDFValue                    *) shiftProbability
{
    if ( [ SPECTRUM_SUBNODE isFluorescent ] )
    {
        ArSpectralSample dummyAttenuation;
        
        return
            [ SPECTRUM_SUBNODE randomWavelengthShift
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
    if ( [ SPECTRUM_SUBNODE isFluorescent ] )
    {
        ArSpectralSample dummyAttenuation;
        return
            [ SPECTRUM_SUBNODE attenuationForWavelengthShift
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

- (void) _computeSigmaData
        :   (double) sigma
{
    sigma2  = M_SQR( sigma );
    c1      = 1.0 - ( sigma2 / ( 2.0 * sigma2 + 0.33 ));
}

- (void) _computeSigmaDataIfNecessary
        :   (ArcObject <ArpEvaluationEnvironment> *) evalEnv
{
    if ( ! isSigmaConstant )
    {
        double sigma;

        [ SIGMA_SUBNODE getDoubleValue
            :    evalEnv
            :  & sigma
            ];

        [ self _computeSigmaData
            :    sigma
            ];
    }
}

- (void) _setupOrenNayarReflector
{
    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
          SPECTRUM_SUBNODE,
          ArpSpectrumValues
        );

    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
          SIGMA_SUBNODE,
          ArpDoubleValues
        );

    AREVALENV_TYPE_CHECK(
          SPECTRUM_SUBNODE,
          arevalenv_surfacepoint,
          "colour"
        );

    AREVALENV_TYPE_CHECK(
          SIGMA_SUBNODE,
          arevalenv_surfacepoint,
          "sigma"
        );
    
    // If the sigma parameter is constant, precompute some data
    
    double  sigma;
    
    unsigned long  constValCount =
        [ SIGMA_SUBNODE getConstDoubleValue
            : & sigma
            ];
    
    isSigmaConstant = (constValCount > 0);

    if (isSigmaConstant)
    {
        [ self _computeSigmaData
            :   sigma
            ];
    }
}

- (id) init
        : (ArNode <ArpNode>*) newColour
        : (ArNode <ArpNode>*) newSigma
{
    self =
        [ super init
            :   HARD_NODE_REFERENCE(newColour)
            :   HARD_NODE_REFERENCE(newSigma)
            ];
    
    if ( self )
    {
        [ self _setupOrenNayarReflector ];
    }
    
    return self;
}

- (void) code
        : (ArcObject <ArpCoder>*) coder
{
    [ super code: coder ];
    
    [ coder codeUInt: ((unsigned int*)& additionalSurfaceProperties) ];
    
    if ( [ coder isReading ] )
        [ self _setupOrenNayarReflector ];
}

/* ---------------------------------------------------------------------------
 Oren-Nayar surface model evaluation functions.
 
 These are modelled according to the notation and conventions used in
 the orginal 1994 SIGGRAPH paper. We use the full form of the model,
 i.e. the one where C1, C2 and C3 are all evaluated (p. 7) and not
 the abbreviated model from p. 8 which is more often cited in secondary
 literature.
 
 The idea is that path tracers - where this model is primarily used -
 are so slow to begin with that we might as well do the job properly.
 ------------------------------------------------------------------------aw- */

double on_factor_c2(
     const double  alpha,
     const double  beta,
     const double  cosDeltaPhi,
     const double  sigma2
     )
{
    if( cosDeltaPhi >= 0.0 )
        return 0.45 * ( sigma2 / ( sigma2 + 0.09) ) * sin(alpha);
    else
        return   0.45
               * ( sigma2 / ( sigma2 + 0.09) )
               * ( sin(alpha) - M_CUBE( ( 2 * beta ) / MATH_PI ) );
}

double on_factor_c3(
     const double  alpha,
     const double  beta,
     const double  sigma2
     )
{

    return
          0.125
        * ( sigma2 / ( sigma2 + 0.09 ) )
        * M_SQR( ( 4 * alpha * beta ) / ( M_SQR( MATH_PI )) );
}

double on_lr1(
     const double  cosDeltaPhi,
     const double  sigma2,
     const double  alpha,
     const double  beta,
     const double  c1
     )
{
    return
          c1
        +   cosDeltaPhi
          * on_factor_c2( alpha, beta, cosDeltaPhi, sigma2 )
          * tan( beta ) +  ( 1.0 - M_ABS( cosDeltaPhi ) )
          * on_factor_c3( alpha, beta, sigma2 ) * tan( ( alpha + beta ) / 2.0 );
}

void orenNayarReflectanceSpectralSample(
           ART_GV            * art_gv,
     const Vec3D             * localI_original,
     const Vec3D             * localR,
     const double              c1,
     const double              sigma2,
           ArSpectralSample  * spectralSample
     )
{
    Vec3D  localI;
    
    vec3d_dv_mul_v( -1.0, localI_original, & localI );

    double  cosThetaI = M_ABS( ZC(localI) );
    double  cosThetaR = M_ABS( ZC(*localR) );

    double  thetaI = acos( cosThetaI );
    double  thetaR = acos( cosThetaR );

    Vec2D  phiI, phiR;

    XC(phiI) = XC(localI);
    YC(phiI) = YC(localI);

    XC(phiR) = XC(*localR);
    YC(phiR) = YC(*localR);

    double  phiLenI = vec2d_v_sqrlen( & phiI );
    double  phiLenR = vec2d_v_sqrlen( & phiR );

    double  cosDeltaPhi;

    if ( phiLenI > 0.0 && phiLenR > 0.0 )
    {
        vec2d_norm_v( & phiI );
        vec2d_norm_v( & phiR );

        cosDeltaPhi = vec2d_vv_dot( & phiI, & phiR );
    }
    else
    {
        cosDeltaPhi = 0.0;
    }

    double  alpha = M_MAX( M_ABS(thetaR), M_ABS(thetaI) );
    double  beta  = M_MIN( M_ABS(thetaR), M_ABS(thetaI) );

    double  d_lr1  = on_lr1( cosDeltaPhi, sigma2, alpha, beta, c1 );

    //   Clamp to zero, to get rid of values just beneath zero for extreme
    //   roughness values.
    
    d_lr1 = M_MAX( d_lr1, 0. );
    
    ArSpectralSample  sps_temp_1;

    sps_d_init_s(
          art_gv,
          d_lr1,
        & sps_temp_1
        );

    double  mulFactor =
          0.17 / MATH_PI
        * ( sigma2 / ( sigma2 + 0.13 ) )
        * ( 1.0 - cosDeltaPhi * M_SQR( 2 * beta / MATH_PI) );

    //   Clamp to zero, to get rid of values just beneath zero for extreme
    //   roughness values.

    mulFactor = M_MAX( mulFactor, 0. );

    ArSpectralSample  sps_temp_2;

    sps_ds_mul_s(
          art_gv,
          mulFactor,
          spectralSample,
        & sps_temp_2
        );
    
    sps_s_add_s(
          art_gv,
        & sps_temp_1,
        & sps_temp_2
        );

    sps_s_mul_s(
          art_gv,
        & sps_temp_2,
          spectralSample
        );
}

- (BOOL) calculateBidirectionalAttenuationSample
        : (      ArcIntersection *) incomingDirectionAndLocation
        : (      ArDirectionCosine *) outgoingDirection
        : (      ArPathDirection) pathDirection
        : (      ArBSDFSampleGenerationContext *) context
        : (const ArWavelength *) incomingWavelength
        : (const ArWavelength *) outgoingWavelength
        : (      ArPDFValue *) sampleProbability
        : (      ArPDFValue *) reverseSampleProbability
        : (      ArAttenuationSample *) attenuationSample
{
    if ( OUTGOING_COSINE_WORLDSPACE > 0.0 )
    {
        ArSpectralSample  spectralSample;

        if ( [ SPECTRUM_SUBNODE isFluorescent ] )
        {
            if(![ SPECTRUM_SUBNODE attenuationForWavelengthShift
                    :   incomingDirectionAndLocation
                    :   incomingWavelength
                    :   outgoingWavelength
                    :   pathDirection
                    : & spectralSample
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

            if ( reverseSampleProbability )
            {
                // todo: reverseSampleProbability
                
                ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
            }
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
            
            [ SPECTRUM_SUBNODE getSpectralSample
                :   incomingDirectionAndLocation
                :   incomingWavelength
                : & spectralSample
                ];
        }

        [ self _computeSigmaDataIfNecessary
            :   incomingDirectionAndLocation
            ];

        Trafo3D  world2local;

        trafo3d_v_world2local_from_worldspace_normal_t(
            & SURFACE_NORMAL_WORLDSPACE,
            & world2local
            );
        
        Vec3D  localI;

        vec3d_v_trafo3d_v(
            & INCOMING_VECTOR_WORLDSPACE,
            & world2local,
            & localI
            );

        Vec3D  localO;

        vec3d_v_trafo3d_v(
            & OUTGOING_VECTOR_WORLDSPACE,
            & world2local,
            & localO
            );

        orenNayarReflectanceSpectralSample(
              art_gv,
            & localI,
            & localO,
              c1,
              sigma2,
            & spectralSample
            );

        ARATTENUATIONSAMPLE_VVV_PD_C_INIT_DEPOLARISING_A(
            & INCOMING_VECTOR_WORLDSPACE,
            & SURFACE_NORMAL_WORLDSPACE,
            & OUTGOING_VECTOR_WORLDSPACE,
              pathDirection,
            & spectralSample,
              attenuationSample
        );

        arattenuationsample_d_mul_a(
            art_gv,
            MATH_1_DIV_PI,
            attenuationSample
            );

        return YES;
    }
    else
    {
        if ( sampleProbability )
            *sampleProbability = ARPDFVALUE_ZERO;
        
        if ( reverseSampleProbability )
            *reverseSampleProbability = ARPDFVALUE_ZERO;

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
        : (      ArcIntersection *) incomingDirectionAndLocation
        : (      ArPathDirection) pathDirection
        : (      ArBSDFSampleGenerationContext *) context
        : (const ArWavelength *) incomingWavelength
        : (      ArWavelength *) sampledWavelength
        : (      ArDirectionCosine *) sampledDirection
        : (      ArPDFValue *) sampleProbability
        : (      ArPDFValue *) reverseSampleProbability
        : (      ArAttenuationSample *) attenuationSample
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

    ArSpectralSample  spectralSample;
    
    if ( [ SPECTRUM_SUBNODE isFluorescent ] )
    {
        if(![ SPECTRUM_SUBNODE randomWavelengthShift
                :   incomingDirectionAndLocation
                :   incomingWavelength
                :   RANDOM_GENERATOR
                :   pathDirection
                :   sampledWavelength
                : & spectralSample
                :   sampleProbability
                ])
            return NO;

        arpdfvalue_d_mul_p(
              SAMPLED_COSINE_WORLDSPACE / MATH_PI, // probability of direction
              sampleProbability
            );
        
        if( reverseSampleProbability )
        {
            // todo: reverseSampleProbability
            ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
        }
    }
    else
    {
        *sampledWavelength = *incomingWavelength;
    
        arpdfvalue_d_init_p(
              SAMPLED_COSINE_WORLDSPACE / MATH_PI,
              sampleProbability
            );
        
        if( reverseSampleProbability )
        {
            arpdfvalue_d_init_p(
                  INCOMING_COSINE_WORLDSPACE / MATH_PI,
                  reverseSampleProbability
                );
        }

        [ SPECTRUM_SUBNODE getSpectralSample
            :   incomingDirectionAndLocation
            :   incomingWavelength
            : & spectralSample
            ];
    }
    
    [ self _computeSigmaDataIfNecessary
        :   incomingDirectionAndLocation
        ];

    Trafo3D  world2local;

    trafo3d_t_transpose_t(
        & local2world,
        & world2local
        );

    Vec3D  localI;

    vec3d_v_trafo3d_v(
        & INCOMING_VECTOR_WORLDSPACE,
        & world2local,
        & localI
        );

    orenNayarReflectanceSpectralSample(
          art_gv,
        & localI,
        & localO,
          c1,
          sigma2,
        & spectralSample
        );

    ARATTENUATIONSAMPLE_VVV_PD_C_INIT_DEPOLARISING_A(
        & INCOMING_VECTOR_WORLDSPACE,
        & SURFACE_NORMAL_WORLDSPACE,
        & ARDIRECTIONCOSINE_VECTOR(*sampledDirection),
          pathDirection,
        & spectralSample,
          attenuationSample
        );

    arattenuationsample_d_mul_a(
          art_gv,
          MATH_1_DIV_PI,
          attenuationSample
        );
    
    return YES;
}

- (BOOL)calculateAlbedoSampleAtWavelength:(ArcSurfacePoint *)emissionLocation :(const ArWavelength *)wavelength :(ArSpectralSample *)albedo {
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
    return NO;
}



@end

// ===========================================================================
