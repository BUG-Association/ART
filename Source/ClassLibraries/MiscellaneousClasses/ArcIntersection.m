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

#define ART_MODULE_NAME     ArcIntersection

#import "ArcIntersection.h"
#import "ArcPhaseInterfaceCache.h"
#import "ArnInfSphere.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArcIntersection

- init
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

    if ( ! ARCINTERSECTION_OBJECTSPACE_NORMAL_IS_VALID(self) )
        [ (ArNode <ArpShapeRayCasting> *)ARCINTERSECTION_SHAPE(self)
            calculateLocalNormalForIntersection
            :   self
            ];

    if ( ! ARCINTERSECTION_WORLDSPACE_POINT_IS_VALID(self) )
    {
        Pnt3D  temp;

        pnt3d_dr_eval_p(
              ARCINTERSECTION_T(self),
            & ARCINTERSECTION_OBJECTSPACE_INCOMING_RAY(self),
            & temp
            );
        
        if ( ARCINTERSECTION_TRAFO(self) )
        {
            [ ARCINTERSECTION_TRAFO(self) transformPnt3D
                : & temp
                : & ARCINTERSECTION_WORLDSPACE_POINT(self)
                ];
        }
        else
        {
            ARCINTERSECTION_WORLDSPACE_POINT(self) =
                temp;
        }

        ARCINTERSECTION_FLAG_WORLD_POINT_AS_VALID(self);
    }

    if ( ! ARCINTERSECTION_WORLDSPACE_NORMAL_IS_VALID(self) )
    {
        if ( ARCINTERSECTION_TRAFO(self) )
        {
            [ ARCINTERSECTION_TRAFO(self) transformNormalVec3D
                : & ARCINTERSECTION_OBJECTSPACE_NORMAL(self)
                : & ARCINTERSECTION_WORLDSPACE_NORMAL(self)
                ];
        }
        else
        {
            ARCINTERSECTION_WORLDSPACE_NORMAL(self) =
                ARCINTERSECTION_OBJECTSPACE_NORMAL(self);
        }

        vec3d_norm_v( & ARCINTERSECTION_WORLDSPACE_NORMAL(self) );

        ARCINTERSECTION_FLAG_WORLD_NORMAL_AS_VALID(self);
    }

    vec3d_norm_v( &ARCINTERSECTION_WORLDSPACE_INCOMING_RAY_VECTOR(self) );
    vec3d_norm_v( &ARCINTERSECTION_WORLDSPACE_NORMAL(self) );

    if ( ! ARCINTERSECTION_OBJECTSPACE_POINT_IS_VALID(self) )
    {
        Pnt3D  temp;

        pnt3d_dr_eval_p(
              ARCINTERSECTION_T(self),
            & ARCINTERSECTION_OBJECTSPACE_INCOMING_RAY(self),
            & temp
            );

        ARCINTERSECTION_SET_OBJECTSPACE_POINT(self,temp);
    }


    ARCINTERSECTION_WORLDSPACE_COSINE(self) =
        vec3d_vv_dot(
            & ARCINTERSECTION_WORLDSPACE_INCOMING_RAY_VECTOR(self),
            & ARCINTERSECTION_WORLDSPACE_NORMAL(self)
            );

    // The following code segment is needed for refractive objects;
    // surface normal and incoming direction have to be flipped if the
    // ray hits the inside of the object.
    
    // Note the slight brain twister involved here: because we compute
    // the cosine based on surface normal and ray direction (which points
    // towards the surface), we invert the cosine if it is negative
    // (this is the case where all is good), and invert the directions
    // otherwise.

    if ( ARCINTERSECTION_WORLDSPACE_COSINE(self) < 0.0 )
    {
        ARCINTERSECTION_WORLDSPACE_COSINE(self) =
            - ARCINTERSECTION_WORLDSPACE_COSINE(self);
    }
    else
    {
        vec3d_negate_v( & ARCINTERSECTION_OBJECTSPACE_NORMAL(self) );
        vec3d_negate_v( & ARCINTERSECTION_WORLDSPACE_NORMAL(self) );
    }
}

- (void) activate
{
    [ super activate ];
}

- (void) deactivate
{
    [ super deactivate ];
}

- (void) dealloc
{
    [ super dealloc ];
}



- (BOOL) evaluateEmission
        : (      ArcPointContext *) illuminatedPoint
        // path direction assumed to be arpathdirection_from_eye
        : (const ArWavelength *) wavelength
        : (      ArSamplingRegion *) samplingRegion
        : (      ArLightSample *) lightSample
{
    if ( [ ARCINTERSECTION_SHAPE(self)
              isMemberOfClass
              :   [ ArnInfSphere class ] ] )
    {
        [ ARCINTERSECTION_ENVIRONMENT_MATERIAL(self) lightSampleEmittedTowardsPointFromDirection
             :   illuminatedPoint
             : & ARCINTERSECTION_WORLDSPACE_INCOMING_RAY_VECTOR(self)
             :   wavelength
             :   samplingRegion
             :   lightSample ];
        
        return YES;
    }
    else if ( ARCINTERSECTION_SURFACE_IS_EMITTER(self) )
    {
        ArDirectionCosine lightDirection;
        // inverting the direction of the ray to be pointing in the direction of light
        vec3d_v_negate_v(
            & ARCINTERSECTION_WORLDSPACE_INCOMING_RAY_VECTOR(self),
            & ARDIRECTIONCOSINE_VECTOR(lightDirection)
            );
        // cosine stays the same, as it is already computed for ray going away from the surface
        ARDIRECTIONCOSINE_COSINE(lightDirection) =
            ARCINTERSECTION_WORLDSPACE_COSINE(self);
        
        [ ARCSURFACEPOINT_SURFACE_MATERIAL(self) calculateEmittedLightSample
            :   self
            : & lightDirection
            :   wavelength
            :   samplingRegion
            :   lightSample
            ];
        
        return YES;
    }
    else
        return NO;
}


- (BOOL) sampleWavelengthShift
        : (      ArPathDirection) pathDirection
        : (      ArBSDFSampleGenerationContext *) context
        : (const ArWavelength *) incomingWavelength
        : (      ArWavelength *) sampledWavelength
        : (      ArPDFValue *) shiftProbability
{
    return
        [ ARCINTERSECTION_SURFACE_MATERIAL(self) sampleWavelengthShift
            : self
            : pathDirection
            : context
            : incomingWavelength
            : sampledWavelength
            : shiftProbability
            ];
}

- (BOOL) calculateWavelengthShiftProbability
        : (      ArPathDirection) pathDirection
        : (      ArBSDFSampleGenerationContext *) context
        : (const ArWavelength *) incomingWavelength
        : (const ArWavelength *) outgoingWavelength
        : (      ArPDFValue *) shiftProbability
{
    return
        [ ARCINTERSECTION_SURFACE_MATERIAL(self) calculateWavelengthShiftProbability
            : self
            : pathDirection
            : context
            : incomingWavelength
            : outgoingWavelength
            : shiftProbability
            ];
}

- (BOOL) sampleScattering
        : (      ArPathDirection) pathDirection
        : (      ArBSDFSampleGenerationContext *) context
        : (const ArWavelength *) incomingWavelength
        : (      ArWavelength *) sampledWavelength
        : (      ArDirectionCosine *) sampledDirection
        : (      ArNode <ArpVolumeMaterial> **) material
        : (      ArPDFValue *) sampleProbability
        : (      ArPDFValue *) reverseSampleProbability
        : (      ArAttenuationSample *) attenuationSample
{
    BOOL sampleValid =
        [ ARCINTERSECTION_SURFACE_MATERIAL(self) calculateSingleBSDFSample
            : self
            : pathDirection
            : context
            : incomingWavelength
            : sampledWavelength
            : sampledDirection
            : sampleProbability
            : reverseSampleProbability
            : attenuationSample
            ];
            
    if(material && sampleValid)
        *material =
            ARDIRECTIONCOSINE_COSINE(*sampledDirection) < 0.0 ?
                ARCINTERSECTION_VOLUME_MATERIAL_INTO(self) :
                ARCINTERSECTION_VOLUME_MATERIAL_FROM(self);
        
    return sampleValid;
}

- (BOOL) evaluateScattering
        : (const Vec3D *) outgoingDirection
        : (      ArPathDirection) pathDirection
        : (      ArBSDFSampleGenerationContext *) context
        : (const ArWavelength *) incomingWavelength
        : (const ArWavelength *) outgoingWavelength
        : (      double *) outgoingCosine
        : (      ArNode <ArpVolumeMaterial> **) material
        : (      ArPDFValue *) sampleProbability
        : (      ArPDFValue *) reverseSampleProbability
        : (      ArAttenuationSample *) attenuationSample
{
    ArDirectionCosine outgoingDirectionCosine;
    ARDIRECTIONCOSINE_VECTOR(outgoingDirectionCosine) =
        *outgoingDirection;
    ARDIRECTIONCOSINE_COSINE(outgoingDirectionCosine) = *outgoingCosine =
        vec3d_vv_dot(
             & ARCINTERSECTION_WORLDSPACE_NORMAL(self),
               outgoingDirection
         );
    
    return
        [ self evaluateScattering
            : & outgoingDirectionCosine
            :   pathDirection
            :   context
            :   incomingWavelength
            :   outgoingWavelength
            :   material
            :   sampleProbability
            :   reverseSampleProbability
            :   attenuationSample
        ];
}

- (BOOL) evaluateScattering
        : (      ArDirectionCosine *) outgoingDirection
        : (      ArPathDirection) pathDirection
        : (      ArBSDFSampleGenerationContext *) context
        : (const ArWavelength *) incomingWavelength
        : (const ArWavelength *) outgoingWavelength
        : (      ArNode <ArpVolumeMaterial> **) material
        : (      ArPDFValue *) sampleProbability
        : (      ArPDFValue *) reverseSampleProbability
        : (      ArAttenuationSample *) attenuationSample
{
    BOOL sampleValid =
        [ ARCINTERSECTION_SURFACE_MATERIAL(self) calculateBidirectionalAttenuationSample
            : self
            : outgoingDirection
            : pathDirection
            : context
            : incomingWavelength
            : outgoingWavelength
            : sampleProbability
            : reverseSampleProbability
            : attenuationSample
            ];
    
    if(material && sampleValid)
        *material =
            ARDIRECTIONCOSINE_COSINE(*outgoingDirection) < 0.0 ?
                ARCINTERSECTION_VOLUME_MATERIAL_INTO(self) :
                ARCINTERSECTION_VOLUME_MATERIAL_FROM(self);
        
    return sampleValid;
}

@end

// ===========================================================================
