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

#define ART_MODULE_NAME ArnFluorescentAwareTrackingVolumeIntegrator

#import "ArnFluorescentAwareTrackingVolumeIntegrator.h"

ART_MODULE_INITIALISATION_FUNCTION
(
     (void) art_gv;
     [ ART_MODULE_NAME registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

@implementation ArnFluorescentAwareTrackingVolumeIntegrator

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnFluorescentAwareTrackingVolumeIntegrator)

- (id) copy
{
    ArnFluorescentAwareTrackingVolumeIntegrator  * copiedInstance = [ super copy ];

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnFluorescentAwareTrackingVolumeIntegrator  * copiedInstance =
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
    BOOL earlyEnd = NO;
    double t = [ randomGenerator valueFromNewSequence ];
    
    ArSpectralSample crossSection;
    
    // From light direction, as this serves for transmittance computation
    [ volume crossSection
        : & RAY3D_P(*ray)
        :   wavelength
        :   arpathdirection_from_light
        : & crossSection
        ];

    // Compute the transmittance based on sampled distance
    // transmittanceSample = exp(-distance * crossSection)
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
   
    if(distanceProbability)
    {
        ArSpectralSample scattering;
        
        [ volume scatteringCoefficient
            : & RAY3D_P(*ray)
            :   wavelength
            :   pathDirection
            : & scattering
            ];
        
        // sample the distance based on the hero channel
        double step;
        
        if ( SPS_CI(scattering, 0) == 0.0 )
        {
            step = *distance;
        }
        else if ( SPS_CI(crossSection, 0) == 0.0 )
        {
            step = t * (*distance + 1 / SPS_CI(scattering, 0));
        }
        else
        {
            step =
                -log(1 - t *
                    (1 + (SPS_CI(crossSection, 0) / SPS_CI(scattering, 0) - 1) * SPS_CI(*transmittanceSample, 0))
                    ) / SPS_CI(crossSection, 0);
        }
        
        // it is important to make this exact comparison, as distance may evaluate to NaN (which should fall into the maxDistance branch)
        earlyEnd = step < *distance;
        
        ArSpectralSample term;
        // term = scattering * (1 - totalTransmission) / crossSection
        // (limit term for crossSection approaching zero = maxDistance)
        sps_sd_sub_s(
              art_gv,
              transmittanceSample,
              1.0,
            & term
            );
        // uses cc_safediv_c, produces MATH_HUGE_DOUBLE for division by zero
        sps_s_div_s(
              art_gv,
            & crossSection,
            & term);
        // it is never negative & never above maxDistance for positive crossSection, and this clamps MATH_HUGE_DOUBLE to the proper limit
        sps_dd_clamp_s(
              art_gv,
              0.0,
            * distance,
            & term
            );
        
        sps_s_mul_s(
              art_gv,
            & scattering,
            & term
            );
        
        ArSpectralSample denominator;
        
        sps_ss_add_s(
              art_gv,
              transmittanceSample,
            & term,
            & denominator
            );
        
        *distance = MIN(*distance, step);

        if ( earlyEnd )
        {
            ArSpectralSample distanceTerm;
            // distanceTerm = scattering * exp(-distance * crossSection)
            sps_ds_mul_s(
                  art_gv,
                * distance,
                & crossSection,
                & distanceTerm
                );
            sps_negexp_s(
                  art_gv,
                & distanceTerm
                );
            sps_s_mul_s(
                  art_gv,
                & scattering,
                & distanceTerm
                );
            
            // pdf = distanceTerm / (term + totalTransmittance)
            // directly using c4, so that we use div and not safediv and we do not produce negative values
            c4_cc_div_c(
                & SPS_C(denominator),
                & SPS_C(distanceTerm),
                & ARPDFVAL_C(*distanceProbability)
                );
        }
        else
        {
            // pdf = 1 - term / (term + totalTransmittance);
            // directly using c4, so that we use div and not safediv and we do not produce negative values
            c4_cc_div_c(
                & SPS_C(denominator),
                & SPS_C(term),
                & ARPDFVAL_C(*distanceProbability)
                );
            
            // term / (term + totalTransmittance) is certainly not larger than 1
            c4_cd_sub_c(
                & ARPDFVAL_C(*distanceProbability),
                  1.0,
                & ARPDFVAL_C(*distanceProbability)
                );
        }
    }
    
    if ( volumeProbability )
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
            :   wavelength
            :   pathDirection
            : & scattering
            ];
        
        ArSpectralSample term;
        // term = scattering * (1 - totalTransmission) / crossSection
        // (limit term for crossSection approaching zero = maxDistance)
        sps_sd_sub_s(
              art_gv,
              transmittanceSample,
              1.0,
            & term
            );
        sps_s_div_s( // uses cc_safediv_c, produces MATH_HUGE_DOUBLE for division by zero
              art_gv,
            & crossSection,
            & term
            );
        
        sps_dd_clamp_s(
              art_gv,
              0.0, // it is never negative
            * distance, // never above maxDistance for positive crossSection, and this clamps MATH_HUGE_DOUBLE to the proper limit
            & term
            );
        sps_s_mul_s(
              art_gv,
            & scattering,
            & term
            );
        
        ArSpectralSample denominator;
        sps_ss_add_s(
              art_gv,
              transmittanceSample,
            & term,
            & denominator
            );

        // pdf = 1 - term / (term + totalTransmittance);
        c4_cc_div_c( // directly using c4, so that we use div and not safediv and we do not produce negative values
            & SPS_C(denominator),
            & SPS_C(term),
            & ARPDFVAL_C(*distanceProbability)
            );
        c4_cd_sub_c( // term / (term + totalTransmittance) is certainly not larger than 1
            & ARPDFVAL_C(*distanceProbability),
              1.0,
            & ARPDFVAL_C(*distanceProbability)
            );
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

- (void) maxScatteringBidir
        : (      ArNode <ArpVolumeMaterial> *) volume
        : (const Ray3D                      *) ray
        : (const ArWavelength               *) wavelength
        : (      ArPathDirection             ) pathDirection
        : (      ArSpectralSample           *) maxScattering
{
    ArSpectralSample scatteringRev;
    
    [ volume maxScatteringCoefficientForRay
        :   ray
        :   wavelength
        :   pathDirection
        :   maxScattering
        ];
    
    if (pathDirection == arpathdirection_from_eye)
    {
        [ volume maxScatteringCoefficientForRay
            :   ray
            :   wavelength
            :   arpathdirection_opposite(pathDirection)
            : & scatteringRev
            ];
    
        sps_ss_max_s(
              art_gv,
              maxScattering,
            & scatteringRev,
              maxScattering
            );
    }
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
    
    ArSpectralSample
        maxCrossSection,
        maxExtinction,
        inverseMeanFreePath,
        minMeanFreePath,
        maxAbsorption;

    [ self startParameters
        :   volume
        :   ray
        :   wavelength
        :   pathDirection
        : & maxCrossSection
        : & inverseMeanFreePath
        : & minMeanFreePath
        ];

    [ self maxScatteringBidir
        :   volume
        :   ray
        :   wavelength
        :   pathDirection
        : & maxExtinction
        ];
    
    [ volume maxAbsorptionCoefficientForRay
        :   ray
        :   wavelength
        : & maxAbsorption
        ];
    
    sps_ss_add_s(
          art_gv,
        & maxAbsorption,
        & maxExtinction,
        & maxExtinction
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
    
    ArSpectralSample totalTransmittance;
    // totalTransmittance = exp(-maxDistance * crossSection)
    sps_ds_mul_s(art_gv, (maxt - mint), &maxCrossSection, &totalTransmittance);
    sps_negexp_s(art_gv, &totalTransmittance);
    
    ArSequenceID  sequence_id = [ randomGenerator currentSequenceID ];

    while ( mint < maxt )
    {
        // sample distance based on the hero wavelength
        [ randomGenerator setCurrentSequenceID: sequence_id ];
        const double rd = [ randomGenerator valueFromNewSequence ];
        
        double step;
        
        if(SPS_CI(maxExtinction, 0) == 0.0)
        {
            // Adding EPSILON for imprecisions in the case of t == maxt
            step = (maxt - mint) + FLT_EPSILON;
        }
        else if(SPS_CI(maxCrossSection, 0) == 0.0)
        {
            step = rd * ((maxt - mint) + 1 / SPS_CI(maxExtinction, 0));
        }
        else
        {
            step = -log(1 - rd *
                        (1 + (SPS_CI(maxCrossSection, 0) / SPS_CI(maxExtinction, 0) - 1) * SPS_CI(totalTransmittance, 0))
                        ) / SPS_CI(maxCrossSection, 0);
        }
        
        const double minStep = MIN(step, maxt - t);
        
        t += step;
        
        ArSpectralSample stepTransmittance;
        ArSpectralSample stepProbability;
        
        // stepTransmittance = exp(-step * mu_t_max)
        sps_ds_mul_s(art_gv, minStep, &maxCrossSection, &stepTransmittance);
        sps_negexp_s(art_gv, &stepTransmittance);
        
        // transmittanceSample *= stepTransmittance
        sps_s_mul_s(art_gv, &stepTransmittance, transmittanceSample);
        
        Pnt3D arrival;
        pnt3d_dr_eval_p(t, ray, &arrival);
        
        ArSpectralSample term;
        // term = scattering * (1 - totalTransmission) / crossSection
        // (limit term for crossSection approaching zero = maxDistance)
        sps_sd_sub_s(art_gv, &stepTransmittance, 1.0, &term);
        // uses cc_safediv_c, produces MATH_HUGE_DOUBLE for division by zero
        sps_s_div_s(art_gv, &maxCrossSection, &term);
        // it is never negative & never above maxDistance for positive crossSection, and this clamps MATH_HUGE_DOUBLE to the proper limit
        sps_dd_clamp_s(art_gv, 0.0, (maxt - mint), &term);

        sps_s_mul_s(art_gv, &maxExtinction, &term);

        ArSpectralSample denominator;
        sps_ss_add_s(art_gv, &stepTransmittance, &term, &denominator);
        
        if ( t >= maxt )
        {
            sps_ss_div_s(art_gv, &denominator, &term, &stepProbability);
            sps_sd_sub_s(art_gv, &stepProbability, 1.0, &stepProbability);
            sps_s_mul_s(art_gv, &stepProbability, &pdf);

            break;
        }
        else
        {
            ArSpectralSample distanceTerm;
            // distanceTerm = scattering * exp(-distance * crossSection)
            sps_ds_mul_s(art_gv, step, &maxCrossSection, &distanceTerm);
            sps_negexp_s(art_gv, &distanceTerm);
            sps_s_mul_s(art_gv, &maxExtinction, &distanceTerm);
            
            // pdf = distanceTerm / (term + totalTransmittance)
            sps_ss_div_s(art_gv, &denominator, &distanceTerm, &stepProbability);
            sps_s_mul_s(art_gv, &stepProbability, &pdf);

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
