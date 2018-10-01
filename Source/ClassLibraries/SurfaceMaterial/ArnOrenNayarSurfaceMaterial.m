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

#define ART_MODULE_NAME     ArnOrenNayarSurfaceMaterial

#import "ArnOrenNayarSurfaceMaterial.h"

#import "SurfaceMaterialMacros.h"
#import "ArpEvaluationEnvironment.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    [ ArnOrenNayarSurfaceMaterial registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


/* ===========================================================================
    'ArnOrenNayarSurfaceMaterial'
=========================================================================== */

@implementation ArnOrenNayarSurfaceMaterial

- copy
{
    ArnOrenNayarSurfaceMaterial  * copiedInstance = [ super copy ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnOrenNayarSurfaceMaterial  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

#define SUB_COLOUR_NODE         ARNBINARY_SUBNODE_0
#define SUB_SIGMA_NODE          ARNBINARY_SUBNODE_1

#define SUB_COLOUR_VALUES       ((ArNode <ArpSpectrumValues>*) SUB_COLOUR_NODE)
#define SUB_SIGMA_VALUES        ((ArNode <ArpDoubleValues>*) SUB_SIGMA_NODE)

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnOrenNayarSurfaceMaterial)

ARPSURFACEMATERIAL_DEFAULT_NONDIFFUSE_NONEMISSIVE_IMPLEMENTATION
ARPSURFACEMATERIAL_DEFAULT_SURFACETYPE_IMPLEMENTATION(
      arsurface_generates_perfectly_diffuse_reflections,
      YES
    )


- (void) _computeSigmaData
        :   (double) sigma
{
    sigma2  = M_SQR( sigma );
    c1      = 1.0 - ( sigma2 / ( 2.0 * sigma2 + 0.33 ));
}

- (void) _computeSigmaDataIfNecessary
        :   (ArcObject <ArpEvaluationEnvironment> *) evalEnv
{
    if (!isSigmaConstant)
    {
        double sigma;
        [ SUB_SIGMA_VALUES getDoubleValue
         :     evalEnv
         :   & sigma
         ];
        [ self _computeSigmaData
         :     sigma
         ];
    }
}

- (void) _setupOrenNayarReflector
{
    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
          SUB_COLOUR_NODE,
          ArpSpectrumValues
        );

    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
          SUB_SIGMA_NODE,
          ArpDoubleValues
        );

    AREVALENV_TYPE_CHECK(
          SUB_COLOUR_VALUES,
          arevalenv_surfacepoint,
          "colour"
        );

    AREVALENV_TYPE_CHECK(
          SUB_SIGMA_VALUES,
          arevalenv_surfacepoint,
          "sigma"
        );
    
    // If the sigma parameter is constant, precompute some data
    
    double  sigma;
    
    unsigned long  constValCount =
        [ SUB_SIGMA_VALUES getConstDoubleValue
            : & sigma
            ];
    
    isSigmaConstant = (constValCount > 0);

    if (isSigmaConstant)
        [ self _computeSigmaData
         :     sigma
         ];
}

- init
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

void orenNayarReflectanceSample(
     const Vec3D                * localI_original,
     const Vec3D                * localR,
     const double                 c1,
     const double                 sigma2,
           ArAttenuationSample  * attenuationSample
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
        cosDeltaPhi = 0.0;

    double  alpha = M_MAX( M_ABS(thetaR), M_ABS(thetaI) );
    double  beta  = M_MIN( M_ABS(thetaR), M_ABS(thetaI) );

    double  d_lr1  = on_lr1( cosDeltaPhi, sigma2, alpha, beta, c1 );

    d_lr1 *= MATH_1_DIV_PI;
#ifdef NEVERMORE
    ArAttenuation  *lr1 = arattenuation_alloc( art_gv );
    ArAttenuation  *lr2 = arattenuation_alloc( art_gv );


    double  mulFactor =
          0.17 / MATH_PI
        * ( sigma2 / ( sigma2 + 0.13 ) )
        * ( 1.0 - cosDeltaPhi * M_SQR( 2 * beta / MATH_PI) );
    
    arattenuation_da_mul_a(
        art_gv,
        mulFactor,
        attenuationSample,
                  lr2
                                    );

    if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE )
    {
        ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
    }
    else
    {
        arattenuation_d_init_a(
            art_gv,
            d_lr1,
            lr1
            );
    }

    arattenuation_a_add_a(art_gv, lr1, lr2 );

    arattenuation_a_mul_a( art_gv, lr1, surfaceReflectancy);

    arattenuation_free(art_gv, lr1);
    arattenuation_free(art_gv, lr2);
#endif

}

void orenNayarProbability(
     double      * cosine,
     ArPDFValue  * bsdfsampleProbability
     )
{
    arpdfvalue_dd_init_p(
          *cosine / MATH_PI,
          *cosine / MATH_PI,
          bsdfsampleProbability
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
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
    return NO;
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
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
    return NO;
}


@end

// ===========================================================================
