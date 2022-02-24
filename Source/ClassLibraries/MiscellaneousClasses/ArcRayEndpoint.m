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

#define ART_MODULE_NAME     ArcRayEndpoint

#import "ArcRayEndpoint.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArcRayEndpoint

- (void) prepareForUse
        : (ArcPhaseInterfaceCache *) phaseInterfaceCache
{
    [ super prepareForUse
        :   phaseInterfaceCache
        ];

    if ( ! ARCRAYENDPOINT_WORLDSPACE_POINT_IS_VALID(self) )
    {
        Pnt3D  temp;

        pnt3d_dr_eval_p(
              ARCRAYENDPOINT_T(self),
            & ARCRAYENDPOINT_OBJECTSPACE_INCOMING_RAY(self),
            & temp
            );
        
        if ( ARCRAYENDPOINT_TRAFO(self) )
        {
            [ ARCRAYENDPOINT_TRAFO(self) transformPnt3D
                : & temp
                : & ARCRAYENDPOINT_WORLDSPACE_POINT(self)
                ];
        }
        else
        {
            ARCRAYENDPOINT_WORLDSPACE_POINT(self) =
                temp;
        }

        ARCRAYENDPOINT_FLAG_WORLD_POINT_AS_VALID(self);
    }

    vec3d_norm_v( &ARCRAYENDPOINT_WORLDSPACE_INCOMING_RAY_VECTOR(self) );

    if ( ! ARCRAYENDPOINT_OBJECTSPACE_POINT_IS_VALID(self) )
    {
        Pnt3D  temp;

        pnt3d_dr_eval_p(
              ARCRAYENDPOINT_T(self),
            & ARCRAYENDPOINT_OBJECTSPACE_INCOMING_RAY(self),
            & temp
            );

        ARCRAYENDPOINT_SET_OBJECTSPACE_POINT(self,temp);
    }
}


- (BOOL) sampleWavelengthShift
        : (      ArPathDirection) pathDirection
        : (      ArBSDFSampleGenerationContext *) context
        : (const ArWavelength *) incomingWavelength
        : (      ArWavelength *) sampledWavelength
        : (      ArPDFValue *) shiftProbability
{
    return
        [ ARCRAYENDPOINT_VOLUME_MATERIAL(self) sampleWavelengthShift
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
        [ ARCRAYENDPOINT_VOLUME_MATERIAL(self) calculateWavelengthShiftProbability
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
    ARDIRECTIONCOSINE_COSINE(*sampledDirection) = 1.0;
    if(material)
        *material = ARCRAYENDPOINT_VOLUME_MATERIAL(self);
    
    return
        [ ARCRAYENDPOINT_VOLUME_MATERIAL(self) calculatePhaseFunctionSample
            :   self
            :   pathDirection
            :   context
            :   incomingWavelength
            :   sampledWavelength
            : & ARDIRECTIONCOSINE_VECTOR(*sampledDirection)
            :   sampleProbability
            :   reverseSampleProbability
            :   attenuationSample
            ];
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
    *outgoingCosine = 1.0;
    if(material)
        *material = ARCRAYENDPOINT_VOLUME_MATERIAL(self);
    
    return
        [ ARCRAYENDPOINT_VOLUME_MATERIAL(self) evaluatePhaseFunction
            :   self
            :   outgoingDirection
            :   pathDirection
            :   context
            :   incomingWavelength
            :   outgoingWavelength
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
    if(material)
        *material = ARCRAYENDPOINT_VOLUME_MATERIAL(self);
    
    return
        [ ARCRAYENDPOINT_VOLUME_MATERIAL(self) evaluatePhaseFunction
            :   self
            : & ARDIRECTIONCOSINE_VECTOR(*outgoingDirection)
            :   pathDirection
            :   context
            :   incomingWavelength
            :   outgoingWavelength
            :   sampleProbability
            :   reverseSampleProbability
            :   attenuationSample
            ];
}

@end

// ===========================================================================
