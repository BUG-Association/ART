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

#define ART_MODULE_NAME ArnExponentialTrackingVolumeIntegrator

#import "ArnExponentialTrackingVolumeIntegrator.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
     [ ART_MODULE_NAME registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

@implementation ArnExponentialTrackingVolumeIntegrator

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnExponentialTrackingVolumeIntegrator)

- (id) copy
{
    ArnExponentialTrackingVolumeIntegrator  * copiedInstance = [ super copy ];

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnExponentialTrackingVolumeIntegrator  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    return copiedInstance;
}

- (void) code
        : (ArcObject<ArpCoder> *) coder
{
    [ super code: coder ];
}

/*****************************************************************************
 *  Homogeneous methods
 ****************************************************************************/

- (BOOL) homogeneousSupport
{
    return YES;
}

- (BOOL) _sampleDistanceHomogeneous
        : (      ArNode <ArpVolumeMaterial>     *) volume
        : (const Ray3D                          *) ray
        : (const ArWavelength                   *) wavelength
        : (      ArPathDirection                 ) pathDirection
        : (      double                         *) distance
        : (      ArSpectralSample               *) transmittanceSample
        : (      ArPDFValue                     *) volumeProbability
        : (      ArPDFValue                     *) distanceProbability
        : (      ArcObject <ArpRandomGenerator> *) randomGenerator
{
    ArSpectralSample crossSection;
    
    [ volume crossSection
        : & RAY3D_P(*ray)
        :   wavelength
        :   arpathdirection_from_light
        : & crossSection
        ];
    
    // Sample the distance
    const double t = [ randomGenerator valueFromNewSequence ];
    const double step = -log(1 - t) / SPS_CI(crossSection, 0);
    const BOOL earlyEnd = step < *distance;
    
    *distance = MIN(*distance, step);
    
    // exp(-distance * crossSection)
    sps_ds_mul_s(
          art_gv,
        * distance,
        & crossSection,
          transmittanceSample
        );
    sps_negexp_s(
          art_gv,
          transmittanceSample
        );
    
    if ( distanceProbability )
    {
        ArSpectralSample probabilitySample;
        
        // probabilitySample = exp(-distance * inverseMeanFreePath)
        sps_ds_mul_s(
              art_gv,
            * distance,
            & crossSection,
            & probabilitySample
            );
        
        sps_negexp_s(
              art_gv,
            & probabilitySample
            );

        arpdfvalue_s_init_p(probabilitySample, distanceProbability);
    }
    
    if(volumeProbability)
    {
        *volumeProbability = ARPDFVALUE_UNIT_DIRAC;
    }
    
    return earlyEnd;
}

- (void) _estimateTransmittanceHomogeneous
        : (      ArNode <ArpVolumeMaterial> *) volume
        : (const Ray3D                      *) ray
        : (const ArWavelength               *) wavelength
        : (      ArPathDirection             ) pathDirection
        : (      double                     *) distance
        : (      ArSpectralSample           *) transmittanceSample
        : (      ArPDFValue                 *) volumeProbability
        : (      ArPDFValue                 *) distanceProbability
{
    ArSpectralSample crossSection;
    
    [ volume crossSection
        : & RAY3D_P(*ray)
        :   wavelength
        :   arpathdirection_from_light
        : & crossSection
        ];
    
    // exp(-distance * crossSection)
    sps_ds_mul_s(
          art_gv,
        * distance,
        & crossSection,
          transmittanceSample
        );
    sps_negexp_s(
          art_gv,
          transmittanceSample
        );

    if ( distanceProbability )
    {
        ArSpectralSample probabilitySample;
        
        // probabilitySample = exp(-distance * inverseMeanFreePath)
        sps_ds_mul_s(
              art_gv,
            * distance,
            & crossSection,
            & probabilitySample
            );
        
        sps_negexp_s(
              art_gv,
            & probabilitySample
            );

        arpdfvalue_s_init_p(probabilitySample, distanceProbability);
    }
    
    if ( volumeProbability )
    {
        *volumeProbability = ARPDFVALUE_UNIT_DIRAC;
    }
}

/*****************************************************************************
 *  Heterogeneous methods
 ****************************************************************************/

- (BOOL) heterogeneousSupport
{
    return YES;
}

- (BOOL) _sampleDistanceTransmittanceHeterogeneous
        : (      ArNode <ArpVolumeMaterial>     *) volume
        : (const Ray3D                          *) ray
        : (const ArWavelength                   *) wavelength
        : (      ArPathDirection                 ) pathDirection
        : (      BOOL                            ) allowTermination
        : (      double                         *) distance
        : (      ArSpectralSample               *) transmittanceSample
        : (      ArPDFValue                     *) volumeProbability
        : (      ArPDFValue                     *) distanceProbability
        : (      ArcObject <ArpRandomGenerator> *) randomGenerator
{
    ArSpectralSample pdf = SPS4(1.0);
    *transmittanceSample = SPS4(1.0);
    if ( distanceProbability ) *distanceProbability = ARPDFVALUE_ONE;
    
    BOOL earlyEnd = NO;
    
    ArSpectralSample
        maxCrossSection,
        inverseMeanFreePath,
        minMeanFreePath;
    
    [ volume maxCrossSectionForRay
        :   ray
        :   wavelength
        :   arpathdirection_from_light
        : & maxCrossSection
        ];
    
    // inverseMeanFreePath = mu_t_max
    sps_s_init_s(
          art_gv,
        & maxCrossSection,
        & inverseMeanFreePath
        );
    
    sps_s_inv_s(
          art_gv,
        & inverseMeanFreePath,
        & minMeanFreePath
        );
    
    // Determine the boundaries of the volume
    double mint = 0, maxt = *distance;

    [ volume rayIntersect
        :   ray
        : & mint
        : & maxt
        ];
    
    mint = MAX(mint, 0);
    maxt = MIN(maxt, *distance);
    
    double t = mint;
    ArSequenceID  sequence_id = [ randomGenerator currentSequenceID ];
    
    while ( mint < maxt )
    {
        // sample distance based on the hero wavelength
        [ randomGenerator setCurrentSequenceID: sequence_id ];
        const double rd = [ randomGenerator valueFromNewSequence ];
        const double step = -log(1.0 - rd) * SPS_CI(minMeanFreePath, 0);
        const double minStep = MIN(step, maxt - t);
        
        t += step;
        
        ArSpectralSample stepTransmittance, stepProbability;
        
        // stepTransmittance = exp(-step * mu_t_max)
        sps_ds_mul_s(art_gv, minStep, &maxCrossSection, &stepTransmittance);
        sps_negexp_s(art_gv, &stepTransmittance);
        
        // transmittanceSample *= stepTransmittance
        sps_s_mul_s(art_gv, &stepTransmittance, transmittanceSample);
        
        // pdf *= exp(-step * mu_t_max)
        sps_ds_mul_s(art_gv, minStep, &inverseMeanFreePath, &stepProbability);
        sps_negexp_s(art_gv, &stepProbability);
        sps_s_mul_s(art_gv, &stepProbability, &pdf);
        
        if( t >= maxt ) {
            break;
        } else {
            // pdf *= inverseMeanFreePath
            sps_s_mul_s(art_gv, &inverseMeanFreePath, &pdf);
            
            Pnt3D arrival;
            pnt3d_dr_eval_p(t, ray, &arrival);
            
            ArSpectralSample arrivalCrossSection;
            
            // From light direction, as this serves for transmittance computation
            [ volume crossSection
                : & arrival
                :   wavelength
                :   arpathdirection_from_light
                : & arrivalCrossSection
                ];
            
            if ( allowTermination || distanceProbability )
            {
                [ randomGenerator setCurrentSequenceID: sequence_id ];
                const double rejectionSample = [ randomGenerator valueFromNewSequence ];
                
                ArSpectralSample arrivalMeanFreePath, ratio;
                
                sps_s_inv_s(
                      art_gv,
                    & arrivalCrossSection,
                    & arrivalMeanFreePath
                    );
                
                sps_ss_div_s(
                      art_gv,
                    & arrivalMeanFreePath,
                    & minMeanFreePath,
                    & ratio
                    );

                if( rejectionSample < SPS_CI(ratio, 0) )
                {
                    *distance = t;
                    arpdfvalue_s_mul_p(&ratio, distanceProbability);
                    earlyEnd = YES;
                    
                    if ( allowTermination ) break;
                }
                else
                {
                    // *terminationProbability *= 1 - ratio
                    sps_sd_sub_s(art_gv, &ratio, 1.0, & ratio);
                    arpdfvalue_s_mul_p(&ratio, distanceProbability);
                }
            }

            // transmittanceSample *= mu_t_max - mu_t;
            sps_ss_sub_s(art_gv, &arrivalCrossSection, &maxCrossSection, &stepTransmittance);
            sps_s_mul_s(art_gv, &stepTransmittance, transmittanceSample);
        }
    }
    
    if ( volumeProbability )
    {
        arpdfvalue_s_init_p(pdf, volumeProbability);
    }
    
    if ( distanceProbability )
    {
        arpdfvalue_s_mul_p(&pdf, distanceProbability);
    }
    
    if ( ! allowTermination )
    {
        // compute the transmittance divided by the probability of sampling it using the hero wavelength
        sps_d_div_s(
            art_gv,
            ARPDFVALUE_MAIN(*volumeProbability), // certainly non-zero
            transmittanceSample
            );
    }
    
    return earlyEnd;
}

/*****************************************************************************
 *  Protocol Implementation
 ****************************************************************************/

- (BOOL) sampleDistance
        : (      ArNode <ArpVolumeMaterial>     *) volume
        : (const Ray3D                          *) ray
        : (const ArWavelength                   *) wavelength
        : (      ArPathDirection                 ) pathDirection
        : (      double                         *) distance
        : (      ArSpectralSample               *) transmittanceSample
        : (      ArPDFValue                     *) volumeProbability
        : (      ArPDFValue                     *) distanceProbability
        : (      ArcObject <ArpRandomGenerator> *) randomGenerator
{
    if ( [volume isHomogeneous ] ) {
        return
            [ self _sampleDistanceHomogeneous
                :   volume
                :   ray
                :   wavelength
                :   pathDirection
                :   distance
                :   transmittanceSample
                :   volumeProbability
                :   distanceProbability
                :   randomGenerator
                ];
    } else {
        return
            [ self _sampleDistanceTransmittanceHeterogeneous
                :   volume
                :   ray
                :   wavelength
                :   pathDirection
                :   YES
                :   distance
                :   transmittanceSample
                :   volumeProbability
                :   distanceProbability
                :   randomGenerator
                ];
    }
}

- (void) estimateTransmittance
        : (      ArNode <ArpVolumeMaterial>     *) volume
        : (const Ray3D                          *) ray
        : (const ArWavelength                   *) wavelength
        : (      ArPathDirection                 ) pathDirection
        : (      double                         *) distance
        : (      ArSpectralSample               *) transmittanceSample
        : (      ArPDFValue                     *) volumeProbability
        : (      ArPDFValue                     *) distanceProbability
        : (      ArcObject <ArpRandomGenerator> *) randomGenerator
{
    if ( [volume isHomogeneous ] ) {
        [ self _estimateTransmittanceHomogeneous
            :   volume
            :   ray
            :   wavelength
            :   pathDirection
            :   distance
            :   transmittanceSample
            :   volumeProbability
            :   distanceProbability
            ];
    } else {
        [ self _sampleDistanceTransmittanceHeterogeneous
            :   volume
            :   ray
            :   wavelength
            :   pathDirection
            :   NO
            :   distance
            :   transmittanceSample
            :   volumeProbability
            :   distanceProbability
            :   randomGenerator
            ];
    }
}


@end

// ===========================================================================
