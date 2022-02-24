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

#define ART_MODULE_NAME     ArnPhongSurfaceMaterial

#import "ArnPhongSurfaceMaterial.h"

#import "SurfaceMaterialMacros.h"
#import "ArpEvaluationEnvironment.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnPhongSurfaceMaterial registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


/* ===========================================================================
    'ArnPhongSurfaceMaterial'
=========================================================================== */

@implementation ArnPhongSurfaceMaterial

#define SUB_REFLECTANCE_NODE    ARNBINARY_SUBNODE_0
#define SUB_SHININESS_NODE      ARNBINARY_SUBNODE_1

#define REFLECTANCE_SUBNODE  ((ArNode <ArpSpectrumValues>*) ARNBINARY_SUBNODE_0)
#define SHININIESS_SUBNODE    ((ArNode <ArpDoubleValues>*) ARNBINARY_SUBNODE_1)

- (void) _setupArnTestPhongReflectorValParam
{
    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
            SUB_REFLECTANCE_NODE,
            ArpSpectrumValues
            );
    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
            SUB_SHININESS_NODE,
            ArpDoubleValues
            );

    AREVALENV_TYPE_CHECK(
            REFLECTANCE_SUBNODE,
            arevalenv_surfacepoint,
            "colour"
            );
    AREVALENV_TYPE_CHECK(
            SHININIESS_SUBNODE,
            arevalenv_surfacepoint,
            "intensity"
            );
}

- (id) init
        : (ArNode <ArpNode>*) newColour
        : (ArNode <ArpNode>*) newShininess
{
    self =
        [ super init
            :   HARD_NODE_REFERENCE(newColour)
            :   HARD_NODE_REFERENCE(newShininess)
            ];

    if ( self )
    {
        [ self _setupArnTestPhongReflectorValParam ];
    }
    
    return self;
}

- (id) copy
{
    ArnPhongSurfaceMaterial  * copiedInstance = [ super copy ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnPhongSurfaceMaterial  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- (void) code
        : (ArcObject <ArpCoder>*) coder
{
    [ super code: coder ];

    [ coder codeUInt: ((unsigned int*)& additionalSurfaceProperties) ];

    if ( [ coder isReading ] )
        [ self _setupArnTestPhongReflectorValParam ];
}

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnPhongSurfaceMaterial)

ARPSURFACEMATERIAL_DEFAULT_NONDIFFUSE_NONEMISSIVE_IMPLEMENTATION
ARPSURFACEMATERIAL_DEFAULT_SURFACETYPE_IMPLEMENTATION(
      ARSURFACETYPE_GENERIC_REFLECTIONS,
      YES
    )

- (BOOL)calculateBidirectionalAttenuationSample
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
    //   If the wavelengths do not match, we don't play - Phong is not
    //   a fluo capable surface
    
    if ( ! arwavelength_ww_equal( art_gv, incomingWavelength, outgoingWavelength ) )
        return NO;
    
    //   If either of the two cosines is < 0 the BRDF is a total blocker

    if ( INCOMING_COSINE_WORLDSPACE <= 0.0 || OUTGOING_COSINE_WORLDSPACE <= 0.0 )
    {
        if(sampleProbability) *sampleProbability = ARPDFVALUE_ZERO;
        if(reverseSampleProbability) *reverseSampleProbability = ARPDFVALUE_ZERO;

        ARATTENUATIONSAMPLE_VVV_PD_INIT_AS_BLOCKER_A(
            & INCOMING_VECTOR_WORLDSPACE,
            & SURFACE_NORMAL_WORLDSPACE,
            & OUTGOING_VECTOR_WORLDSPACE,
              pathDirection,
              attenuationSample
            );

        return NO;
    }
    else
    {
        ArDirectionCosine  reflectedDirection;

        ardirectioncosine_reflect(
              art_gv,
            & INCOMING_DIRECTION_COSINE_WORLDSPACE,
            & SURFACE_NORMAL_WORLDSPACE,
            & reflectedDirection
            );

        double  cosTheta =
            vec3d_vv_dot(
                & ARDC_VECTOR(reflectedDirection),
                & OUTGOING_VECTOR_WORLDSPACE
                );

        if ( cosTheta > 0.0001 )
        {
            double  parameter;
            
            [ SHININIESS_SUBNODE getDoubleValue
                :   incomingDirectionAndLocation
                : & parameter
                ];
            
            // ScaleFactor Ref. Total Compendium P.D. (acw)
            
            double scale = ( parameter + 1.0 ) / MATH_2_MUL_PI;
            double phong = PHONG_FACTOR( cosTheta, parameter, scale );

            ArSpectralSample  attenuationColourSample;

            [ REFLECTANCE_SUBNODE getSpectralSample
                :   incomingDirectionAndLocation
                :   incomingWavelength
                : & attenuationColourSample
                ];

            if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE )
            {
                ART_ERRORHANDLING_FATAL_ERROR_WITH_CODE(
                    ART_ERROR_SURFACE_MATERIAL_NOT_POLARISATION_CAPABLE,
                    "There is no sensible way to use a Phong surface in a "
                    "polarisation renderer. Use Torrance Sparrow, or a layered "
                    "surface instead!"
                    );
            }
            else
            {
                arattenuationsample_s_init_a(
                      art_gv,
                    & attenuationColourSample,
                      attenuationSample
                    );
            }

            arattenuationsample_d_mul_a(
                  art_gv,
                  phong,
                  attenuationSample
                );

            if(sampleProbability)
                 arpdfvalue_dd_init_p(
                      phong,
                      phong,
                      sampleProbability
                    );

            if(reverseSampleProbability)
                 arpdfvalue_dd_init_p(
                      phong,
                      phong,
                      reverseSampleProbability
                    );

            return YES;
        }
        else
        {
            if(sampleProbability) *sampleProbability = ARPDFVALUE_ZERO;
            if(reverseSampleProbability) *reverseSampleProbability = ARPDFVALUE_ZERO;

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
    *sampledWavelength = *incomingWavelength;
    
    ArDirectionCosine  reflectedDirection;
    
    ardirectioncosine_reflect(
          art_gv,
        & INCOMING_DIRECTION_COSINE_WORLDSPACE,
        & SURFACE_NORMAL_WORLDSPACE,
        & reflectedDirection
        );
    
    Trafo3D  local2world;
    
    trafo3d_v_local2world_from_worldspace_normal_t(
        & ARDIRECTIONCOSINE_VECTOR(reflectedDirection),
        & local2world
        );
    
    double  r0, r1;
    
    [ RANDOM_GENERATOR getValuesFromNewSequences: & r0 : & r1 ];
    
    // Repeat the generation of a random exitant direction until
    // we find a valid one; according to Lafortune/Willems the
    // samples should be random numbers between [0,1]. However,
    // the getValuesFromNewSequences-random numbers are between
    // [0,1[. Maybe that's the reason for the 1-r0.
    // Ref: Lafortune/Willems, Using the Modified Phong Reflectance
    // Model for Physically Based Rendering, 1994 (acw)
    
    double  parameter;
    
    [ SHININIESS_SUBNODE getDoubleValue
        :   incomingDirectionAndLocation
        : & parameter
        ];
    
    double  z   = m_dd_pow(1.0 - r0, 2.0 / ( parameter + 1 ) );
    double  phi = MATH_2_MUL_PI * r1;
    double  r = sqrt(1 - z);
    
    Vec3D   localVec = VEC3D(r * cos(phi), r * sin(phi), sqrt(z));
    
    vec3d_v_trafo3d_v(
        & localVec,
        & local2world,
        & ARDIRECTIONCOSINE_VECTOR(*sampledDirection)
        );
    
    // This is only > 0 if the sample is not cast into the material
    // (for grazing angles a part of the lobe is always beneath
    // the surface)
    
    ARDIRECTIONCOSINE_COSINE(*sampledDirection) =
        vec3d_vv_dot(
            & SURFACE_NORMAL_WORLDSPACE,
            & ARDIRECTIONCOSINE_VECTOR(*sampledDirection)
            );
    
    // This is the actual "Phong factor"
    double cosThetaPhong =
        vec3d_vv_dot(
            & ARDIRECTIONCOSINE_VECTOR(reflectedDirection),
            & ARDIRECTIONCOSINE_VECTOR(*sampledDirection)
            );
    
    // Both of these have to be > 0 for the sample to be valid
    
    if (   cosThetaPhong > 0.001
        && ARDIRECTIONCOSINE_COSINE(*sampledDirection) > 0.0 )
    {
        ArSpectralSample  attenuationColourSample;
        
        [ REFLECTANCE_SUBNODE getSpectralSample
            :   incomingDirectionAndLocation
            :   incomingWavelength
            : & attenuationColourSample
            ];
        
        if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE )
        {
                ART_ERRORHANDLING_FATAL_ERROR_WITH_CODE(
                    ART_ERROR_SURFACE_MATERIAL_NOT_POLARISATION_CAPABLE,
                    "There is no sensible way to use a Phong surface in "
                    "a polarisation renderer. Use Torrance Sparrow, or a "
                    "layered surface instead!"
                    );
        }
        else
        {
            arattenuationsample_s_init_a(
                  art_gv,
                & attenuationColourSample,
                  attenuationSample
                );
        }
        
        // ScaleFactor Ref. Total Compendium P.D. (acw)
        
        double  scale = ( parameter + 1.0 ) / MATH_2_MUL_PI;
        double  phong = PHONG_FACTOR( cosThetaPhong, parameter, scale );
        
        // Multiplication of the specular colour with the Phong factor
        
        arattenuationsample_d_mul_a(
              art_gv,
              phong,
              attenuationSample
            );
        
        // Ref. Total Compendium P.D. (acw)
        
        arpdfvalue_dd_init_p(
              phong,
              phong,
              sampleProbability
            );
        
        if(reverseSampleProbability)
            arpdfvalue_dd_init_p(
                  phong,
                  phong,
                  reverseSampleProbability
                );
        
        return YES;
    }
    else
        return NO;
}

@end

// ===========================================================================
