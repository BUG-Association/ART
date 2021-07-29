/* ===========================================================================

    Copyright (c) 1996-2021 The ART Development Team
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

#define ART_MODULE_NAME     ArnLightsourceCollectionSampling

#import "ArpLightsourceSampling_Categories.h"
#import "LightsourceSamplingCommonMacros.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

#define RANDOM_GENERATOR samplingContext->randomGenerator
#define LIGHTSOURCE(__a,__i) \
    ((id <ArpLightsourceSampling>)LSC_LIGHT((__a),(__i)).source)

@implementation ArnLightsourceCollection ( RaySampling )

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
    double  percentileThreshold = [ RANDOM_GENERATOR valueFromNewSequence ];

    unsigned int i;
    // int  i = -1;
    int  a = 0;

    if ( complexSkydomePresent )
    {
        i = 0;
    }
    else
    {
        ArSpectralSample percentile;

        for (i = 0; i < numberOfLights; i++) {
            // the same method as is used later in pdf multiplication
            // (as spc_sd_value_at_wavelength behaves differently)
            sps_s500w_init_s(
                art_gv,
                LSC_LIGHT(a,i).overallSpectralPowerPercentile,
                wavelength,
                & percentile
                );

            // decide based on the hero wavelength only
            if (SPS_CI(percentile, 0) >= percentileThreshold) {
                break;
            }
        }

        if (i == numberOfLights) {
            return NO;
        }
    }
        
    BOOL result =
        [ LIGHTSOURCE(a,i) sampleLightsource
            : illuminatedPoint
            : samplingContext
            : wavelength
            : samplingRegion
            : sampledPoint
            : sampledDirection
            : illuminationProbability
            : emissionProbability
            : lightSample
            ];
    
    if ( result )
    {
        if ( illuminationProbability || emissionProbability )
        {
            ArSpectralSample pdf;
            
            sps_s500w_init_s(
                  art_gv,
                  LSC_LIGHT(a,i).percentOfOverallSpectralPower,
                  wavelength,
                & pdf
                );
            
            if(illuminationProbability)
                arpdfvalue_d_mul_p(
                    ARPDFVALUE_MAIN(pdf),
                    illuminationProbability
                  );
            
            if(emissionProbability)
                arpdfvalue_d_mul_p(
                    ARPDFVALUE_MAIN(pdf),
                    emissionProbability
                  );
        }
    }
    
    return result;
}

- (BOOL) sampleProbability
        : (      ArNode *)                       emissiveObject
        : (      ArcPointContext *)              illuminatedPoint
        : (      ArcSurfacePoint *)              lightSamplePoint
        : (const ArDirectionCosine *)            lightSampleDirection
        : (const ArWavelength *)                 wavelength
        : (const ArSamplingRegion *)             samplingRegion
        : (      ArPDFValue *)                   illuminationProbability /* optional */
        : (      ArPDFValue *)                   emissionProbability /* optional */
{
    unsigned int  i = 0;
    int  a = 0;

    while ( i < numberOfLights
           && (id)[ LSC_LIGHT(a,i).source shape ] != (id)emissiveObject )
        i++;

    if(i >= numberOfLights)
        return NO;

    [ LIGHTSOURCE(a,i) sampleProbability
        : illuminatedPoint
        : lightSamplePoint
        : lightSampleDirection
        : wavelength
        : samplingRegion
        : illuminationProbability
        : emissionProbability
        ];
    
    if ( illuminationProbability || emissionProbability )
    {
        ArSpectralSample  pdf;
        
        sps_s500w_init_s(
              art_gv,
              LSC_LIGHT(a,i).percentOfOverallSpectralPower,
              wavelength,
            & pdf
            );
        
        if(illuminationProbability)
            arpdfvalue_d_mul_p(
                ARPDFVALUE_MAIN(pdf),
                illuminationProbability
              );
        
        if(emissionProbability)
            arpdfvalue_d_mul_p(
                ARPDFVALUE_MAIN(pdf),
                emissionProbability
              );
    }
    
    return YES;
}


// first attempt, not used
- (id <ArpLightsourceSampling>) selectLightsource
        : (const ArcPointContext *)              illuminatedPoint
        : (      ArLightsourceSamplingContext *) samplingContext
        : (      ArPDFValue *)                   selectionProbability
{
    double  powerThreshold = [ RANDOM_GENERATOR valueFromNewSequence ];
    int  i = 0;
    int  a = 0;
    while( LSC_LIGHT(a,i).overallRadiantPowerPercentile < powerThreshold )
        i++;

    arpdfvalue_dd_init_p(
        LSC_LIGHT(a,i).percentOfOverallRadiantPower,
        LSC_LIGHT(a,i).percentOfOverallRadiantPower,
        selectionProbability
      );
    
    return LIGHTSOURCE(a,i);
}

- (id <ArpLightsourceSampling>) getLightsource
        : (const ArNode *)                       emissiveObject
        : (const ArcPointContext *)              illuminatedPoint
        : (      ArPDFValue *)                   selectionProbability
{
    ASSERT_POSITIVE_INTEGER(numberOfLights);

    unsigned int  i = 0;
    int  a = 0;

    while ( ( i + 1 < numberOfLights )
           && (id)[ LSC_LIGHT(a,i).source shape ] != (id)emissiveObject )
        i++;

    if(selectionProbability)
        arpdfvalue_dd_init_p(
            LSC_LIGHT(a,i).percentOfOverallRadiantPower,
            LSC_LIGHT(a,i).percentOfOverallRadiantPower,
            selectionProbability
          );

    return LIGHTSOURCE(a,i);
}

@end

// ===========================================================================
