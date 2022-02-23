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

#define ART_MODULE_NAME ArnMaxExponentialTrackingVolumeIntegrator

#import "ArnMaxExponentialTrackingVolumeIntegrator.h"

ART_MODULE_INITIALISATION_FUNCTION
(
     (void) art_gv;
     [ ART_MODULE_NAME registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

@implementation ArnMaxExponentialTrackingVolumeIntegrator

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnMaxExponentialTrackingVolumeIntegrator)

- (id) copy
{
    ArnMaxExponentialTrackingVolumeIntegrator  * copiedInstance = [ super copy ];

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnMaxExponentialTrackingVolumeIntegrator  * copiedInstance =
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
    ArSpectralSample crossSection, scattering;
    
    [ volume crossSection
        : & RAY3D_P(*ray)
        :   wavelength
        :   arpathdirection_from_light
        : & crossSection
        ];
    
    [ volume scatteringCoefficient
        : & RAY3D_P(*ray)
        : wavelength
        : pathDirection
        : & scattering
        ];
    
    ArSpectralSample maxInverseMeanFreePath;
    
    sps_ss_max_s(
          art_gv,
        & crossSection,
        & scattering,
        & maxInverseMeanFreePath
        );
    
    // Sample the distance
    const double t = [ randomGenerator valueFromNewSequence ];
    const double step = -log(1 - t) / SPS_CI(maxInverseMeanFreePath, 0);
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
            & maxInverseMeanFreePath,
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
        ArSpectralSample scattering;
        
        [ volume scatteringCoefficient
            : & RAY3D_P(*ray)
            : wavelength
            : pathDirection
            : & scattering
            ];
    
        ArSpectralSample maxInverseMeanFreePath;
        
        sps_ss_max_s(
              art_gv,
            & crossSection,
            & scattering,
            & maxInverseMeanFreePath
            );
        
        ArSpectralSample probabilitySample;
        
        // probabilitySample = exp(-distance * inverseMeanFreePath)
        sps_ds_mul_s(
              art_gv,
            * distance,
            & maxInverseMeanFreePath,
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

- (void) startParameters
        : (      ArNode <ArpVolumeMaterial> *) volume
        : (const Ray3D                      *) ray
        : (const ArWavelength               *) wavelength
        : (      ArPathDirection             ) pathDirection
        : (      ArSpectralSample           *) maxCrossSection
        : (      ArSpectralSample           *) maxInverseMeanFreePath
        : (      ArSpectralSample           *) minMeanFreePath
{
    // Determine the mean free path
    ArSpectralSample maxScatteringCoefficient;
    
    [ volume maxCrossSectionForRay
        :   ray
        :   wavelength
        :   arpathdirection_from_light
        :   maxCrossSection
        ];
    
    [ volume maxScatteringCoefficientForRay
        :   ray
        :   wavelength
        :   pathDirection
        : & maxScatteringCoefficient
        ];
    
    // inverseMeanFreePath = max(mu_t_max, mu_s_max)
    sps_ss_max_s(
          art_gv,
          maxCrossSection,
        & maxScatteringCoefficient,
          maxInverseMeanFreePath
        );
    
    sps_s_inv_s(
        art_gv,
        maxInverseMeanFreePath,
        minMeanFreePath
        );
}

- (void) meanFreePath
        : (      ArNode <ArpVolumeMaterial> *) volume
        : (const Pnt3D                      *) point
        : (const ArWavelength               *) wavelength
        : (      ArPathDirection             ) pathDirection
        : (      ArSpectralSample           *) crossSection
        : (      ArSpectralSample           *) meanFreePath
{
    // Determine the mean free path
    ArSpectralSample scatteringCoefficient;
    
    [ volume crossSection
        :   point
        :   wavelength
        :   arpathdirection_from_light
        :   crossSection
        ];
    
    [ volume scatteringCoefficient
        :   point
        :   wavelength
        :   pathDirection
        : & scatteringCoefficient
        ];
    
    // inverseMeanFreePath = max(mu_t_max, mu_s_max)
    sps_ss_max_s(
          art_gv,
          crossSection,
        & scatteringCoefficient,
          meanFreePath
        );
    
    sps_s_inv_s(
        art_gv,
        meanFreePath,
        meanFreePath
        );
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
    
    //#warning from Michal
    // TODO: this causes a wrong probability to be used for the MISing in the case of fluorescence
    // hardcoded direction from light, as we do not need to scatter, just estimate transmittance in the best way
    //if ( ! allowTermination ) pathDirection = arpathdirection_from_light;
    
    BOOL earlyEnd = NO;
    
    // Determine the mean free path
    ArSpectralSample
        maxCrossSection,
        inverseMeanFreePath,
        minMeanFreePath;
    
    [ self startParameters
        :   volume
        :   ray
        :   wavelength
        :   pathDirection
        : & maxCrossSection
        : & inverseMeanFreePath
        : & minMeanFreePath
        ];
    
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
        const double step = -log(1.0 - rd) / SPS_CI(inverseMeanFreePath, 0);
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
        
        if(t >= maxt) {
            break;
        } else {
            // pdf *= inverseMeanFreePath
            sps_s_mul_s(art_gv, &inverseMeanFreePath, &pdf);
            
            Pnt3D arrival;
            pnt3d_dr_eval_p(t, ray, &arrival);
            
            ArSpectralSample
                    arrivalCrossSection,
                    arrivalMeanFreePath;
            
            [ self meanFreePath
                :   volume
                : & arrival
                :   wavelength
                :   pathDirection
                : & arrivalCrossSection
                : & arrivalMeanFreePath
                ];
            
            if ( allowTermination || distanceProbability )
            {
                [ randomGenerator setCurrentSequenceID: sequence_id ];
                const double rejectionSample = [ randomGenerator valueFromNewSequence ];
                
                ArSpectralSample ratio;
                
                sps_ss_div_s(
                      art_gv,
                    & arrivalMeanFreePath,
                    & minMeanFreePath,
                    & ratio
                    );

                if ( rejectionSample < SPS_CI(ratio, 0) )
                {
                    *distance = t;
                    arpdfvalue_s_mul_p(&ratio, distanceProbability);
                    earlyEnd = YES;
                    if (allowTermination) break;
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
