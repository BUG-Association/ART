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

#define ART_MODULE_NAME     Arn2DGMMSpectrumDebugTab

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_linalg.h>

#import "Arn2DGMMSpectrumDebugTab.h"
#import "ART_ImageFileFormat.h"
#import "ArnColourStandardImplementation.h"

ART_MODULE_INITIALISATION_FUNCTION
(
        [ Arn2DGMMSpectrumDebugTab registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation Arn2DGMMSpectrumDebugTab

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(Arn2DGMMSpectrumDebugTab)

 - deepSemanticCopy
         : (ArnGraphTraversal *) traversal
 {
     Arn2DGMMSpectrumDebugTab  * copiedInstance =
         [ super deepSemanticCopy
             :   traversal
             ];
     
     copiedInstance =
        [ copiedInstance init
             : self->n_gaussians
             : ardoublearray_array(& self->means)
             : ardoublearray_array(& self->covariances)
             : ardoublearray_array(& self->weights)
             : self->scaling_attenuation
             : ARRSS_SIZE(self->diagonal)
             : ARRSS_START(self->diagonal)
             : ARRSS_STEP(self->diagonal)
             : ARRSS_ARRAY(self->diagonal)
         ];
 }


- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];

    [ coder codeInt:(int *) &n_gaussians];
    [ coder codeDouble: &scaling_attenuation];
    
    arpcoder_ardoublearray(coder, &means);
    arpcoder_ardoublearray(coder, &covariances);
    arpcoder_ardoublearray(coder, &weights);
    arpcoder_arrsspectrum(art_gv, coder, &diagonal);

    if ( [ coder isReading ] )
        [ self _setup ];
}

- init
        : (   int  ) _n_gaussians
        : (double *) _means
        : (double *) _covariances
        : (double *) _weights
        : (double  ) _scaling_attenuation
        : (   int  ) diagonal_size
        : (double  ) diagonal_start
        : (double  ) diagonal_step
        : (double *) diagonal_values
{
    self = [super init];

    if ( self ) {
        n_gaussians = (size_t) _n_gaussians;
        scaling_attenuation = _scaling_attenuation;

        printf("attenuation scale: %lf\n", scaling_attenuation);

        means          = ardoublearray_init(2 * n_gaussians);
        covariances    = ardoublearray_init(3 * n_gaussians);
        weights        = ardoublearray_init(n_gaussians);

        mainDiagonal        = NULL;
        crosstalk           = NULL;
        hiresMainDiagonal   = NULL;
        hiresHorizontalSums = NULL;
        hiresVerticalSums   = NULL;

        memcpy(
            ardoublearray_array(&means),
            _means,
            2 * n_gaussians * sizeof(Double)
        );

        memcpy(
            ardoublearray_array(&covariances),
            _covariances,
            3 * n_gaussians * sizeof(Double)
        );

        memcpy(
            ardoublearray_array(&weights),
            _weights,
            n_gaussians * sizeof(Double)
        );

        ARRSS_SIZE(diagonal)  = (unsigned long) diagonal_size;
        ARRSS_START(diagonal) = diagonal_start;
        ARRSS_STEP(diagonal)  = diagonal_step;
        ARRSS_SCALE(diagonal) = 1.0;
        ARRSS_ARRAY(diagonal) = ALLOC_ARRAY( double, ARRSS_SIZE(diagonal) );

        memcpy(
            ARRSS_ARRAY(diagonal),
            diagonal_values,
            ARRSS_SIZE(diagonal) * sizeof(double)
        );

        [self _setup];
    }

    return self;
}

- (void) dealloc
{
    ardoublearray_free_contents(&means);
    ardoublearray_free_contents(&covariances);
    ardoublearray_free_contents(&weights);
    
    for(int i=0;i<n_gaussians;i++){
        gsl_vector_free(gaussian_params[i].mean);
        gsl_matrix_free(gaussian_params[i].covariance);
    }
    
    if (gaussian_params)
        free(gaussian_params);
    
    FREE_ARRAY(ARRSS_ARRAY(diagonal));
    
    if(mainDiagonal)
        spc_free(art_gv, mainDiagonal);
    if(crosstalk)
        arcrosstalk_free(art_gv, crosstalk);
    if(hiresMainDiagonal)
        s500_free(art_gv, hiresMainDiagonal);
    if(hiresCrosstalk)
        cx500_free(art_gv, hiresCrosstalk);
    if(hiresVerticalSums)
        cx500_free(art_gv, hiresVerticalSums);
    if(hiresHorizontalSums)
        cx500_free(art_gv, hiresHorizontalSums);
    
    [ super dealloc ];
}

- (void) _setup
{
    printf("Setup!\nWARNING: this is the debug implementation!\n\n");
    
    // Organize the GMM params in a more understandable way:
    gaussian_params = ALLOC_ARRAY(GMMParameters2, n_gaussians);
    
    // dimension of the data should always be two.
    for (int i = 0; i < n_gaussians; i++) {
        gaussian_params[i].mean = gsl_vector_alloc(2);
        gaussian_params[i].covariance = gsl_matrix_alloc(2,2);
        
        gsl_vector_set(gaussian_params[i].mean,0,ARARRAY_I(means, 2 * i + 0));
        gsl_vector_set(gaussian_params[i].mean,1,ARARRAY_I(means, 2 * i + 1));
        
        gsl_matrix_set(gaussian_params[i].covariance,0,0,ARARRAY_I(covariances, 3 * i + 0));
        gsl_matrix_set(gaussian_params[i].covariance,0,1,ARARRAY_I(covariances, 3 * i + 1));
        gsl_matrix_set(gaussian_params[i].covariance,1,0,ARARRAY_I(covariances, 3 * i + 1));
        gsl_matrix_set(gaussian_params[i].covariance,1,1,ARARRAY_I(covariances, 3 * i + 2));
        
        gaussian_params[i].weight = ARARRAY_I(weights, i);
    }
    
    // Provide native representation for the main diagonal
    if ( ! mainDiagonal )
        mainDiagonal = spc_alloc( art_gv );
    if ( ! hiresMainDiagonal )
        hiresMainDiagonal = s500_alloc(art_gv);

    rss_to_spc ( art_gv, & diagonal, mainDiagonal );
    rss_to_s500( art_gv, & diagonal, hiresMainDiagonal );
    
    // Just to test:
    // we reconstruct the "nativeValue" as if it was provided in a tabular form
    // The reconstruction is done using the GMM parameters
    nativeValue = ALLOC(ArRSSpectrum2D);
    
    // Hard coded in order to get the same as measurements
    nativeValue->emission_start   = 380 NM;
    nativeValue->excitation_start = 300 NM;

    nativeValue->stride = 49;
    nativeValue->size   = 41*49;
    
    nativeValue->excitation_step = 10 NM;
    nativeValue->emission_step   = 10 NM;
    
    nativeValue->scale = 1.;
    
    nativeValue->array = ALLOC_ARRAY(double, nativeValue->size);

    [self _setupReconstructReradiation];
    
    // Now, create native forms
    // (copy paste from ArnRSSSpectrum2D.m)
    // Remove non fluorescent part
    // rss2d_strip_noncrosstalk_data( art_gv, nativeValue );   
    
    if ( ! crosstalk )
        crosstalk = arcrosstalk_alloc( art_gv );
    if ( ! hiresCrosstalk )
        hiresCrosstalk = cx500_alloc(art_gv);
    
    rss2d_to_cx500( art_gv, nativeValue, hiresCrosstalk );
    cx500_to_crosstalk( art_gv, hiresCrosstalk, crosstalk );
    
    if( ! hiresVerticalSums )
        hiresVerticalSums = cx500_alloc(art_gv);
    if( ! hiresHorizontalSums )
        hiresHorizontalSums = cx500_alloc(art_gv);
    
    cx500_vertical_cx500(
        art_gv,
        hiresCrosstalk,
        hiresVerticalSums
    );
    
    cx500_horizontal_cx500(
        art_gv,
        hiresCrosstalk,
        hiresHorizontalSums
    );

    // Debug : Write the full reradiation matrix to a file.
    // This can be plotted with gnuplot using the command:
    // plot "debug.txt" matrix w image

    FILE* fp = fopen("debug.txt", "w");

    if (fp != NULL) {
        for (int i = 0; i < nativeValue->stride; i++) {
            for (int j = 0; j < nativeValue->size / nativeValue->stride; j++) {
                fprintf(fp, "%lf ",
                    // ARRSSPECTRUM2D_SAMPLE(nativeValue, j, i));
                    nativeValue->array[j * nativeValue->stride + i]);
            }

            fprintf(fp, "\n");
        }
    }

    fclose(fp);
    
}

- (void) _setupReconstructReradiation
{  
    memset(nativeValue->array, 0, nativeValue->size * sizeof(double));
    
    for (int excitation_idx = 0; excitation_idx < nativeValue->stride; excitation_idx++) {
        double lambda_i = nativeValue->excitation_start + nativeValue->excitation_step * (double)excitation_idx;

        for (int emission_idx = 0; emission_idx < nativeValue->size / nativeValue->stride; emission_idx++) {
            double lambda_o = nativeValue->emission_start + nativeValue->emission_step * (double)emission_idx;

            nativeValue->array[emission_idx * nativeValue->stride + excitation_idx] = 
                [self attenuation
                    :  NANO_FROM_UNIT(lambda_i)
                    :  NANO_FROM_UNIT(lambda_o)
                ];
        }
    }    
}

- (void) getSpectrum
        : (ArcPointContext *) locationInfo
        : (ArSpectrum *) outSpectrum
{
    spc_s_init_s(
          art_gv,
          mainDiagonal,
          outSpectrum
        );
}
- (void) getHiresSpectrum
        : (ArcPointContext *) locationInfo
        : (ArSpectrum500 *) outSpectrum
{
    s500_s_init_s(
          art_gv,
          hiresMainDiagonal,
          outSpectrum
        );
}

- (void) getSpectralSample
        : (const ArcPointContext *) locationInfo
        : (const ArWavelength *) wavelength
        : (      ArSpectralSample *) outSpectrum
{
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

- (void) getLightIntensity
        : (ArcPointContext *) locationInfo
        : (ArLightIntensity *) outLightIntensity
{
    arlightintensity_s_init_i(
          art_gv,
          mainDiagonal,
          outLightIntensity
        );
}

- (void) getLight
        : (ArcPointContext *) locationInfo
        : (ArLight *) outLight
{
    arlight_s_init_unpolarised_l(
          art_gv,
          mainDiagonal,
          outLight
        );
}

- (void) getAttenuation
        : (ArcPointContext *) locationInfo
        : (ArAttenuation *) attenuation_r
{
    arattenuation_sx_init_a(
           art_gv,
           mainDiagonal,
           crosstalk,
           attenuation_r
        );
}

- (void) getAttenuationSample
        : (const ArcPointContext *) locationInfo
        : (const ArWavelength *) wavelength
        : (      ArAttenuationSample *) outAttenuation
{
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

- (void) getDepolarisingAttenuation
        : (ArcPointContext *) locationInfo
        : (ArReferenceFrame *) refframeEntry
        : (ArReferenceFrame *) refframeExit
        : (ArAttenuation *) attenuation_r
{
    arattenuation_sxrr_init_depolarising_a(
           art_gv,
           mainDiagonal,
           crosstalk,
           refframeEntry,
           refframeExit,
           attenuation_r
        );
}

- (void) getDepolarisingAttenuationSample
        : (const ArcPointContext *) locationInfo
        : (const ArReferenceFrame *) refframeEntry
        : (const ArReferenceFrame *) refframeExit
        : (const ArWavelength *) wavelength
        : (      ArAttenuationSample *) outAttenuation
{
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

- (void) getNonpolarisingAttenuation
        : (ArcPointContext *) locationInfo
        : (ArReferenceFrame *) refframeEntry
        : (ArReferenceFrame *) refframeExit
        : (ArAttenuation *) attenuation_r
{
    arattenuation_sxrr_init_nonpolarising_a(
           art_gv,
           mainDiagonal,
           crosstalk,
           refframeEntry,
           refframeExit,
           attenuation_r
        );
}

- (void) getNonpolarisingAttenuationSample
        : (const ArcPointContext *) locationInfo
        : (const ArReferenceFrame *) refframeEntry
        : (const ArReferenceFrame *) refframeExit
        : (const ArWavelength *) wavelength
        : (      ArAttenuationSample *) outAttenuation
{
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

- (unsigned long) getSpectrumValues
        : (ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (unsigned long) numberOfValues
        : (ArSpectrum *) outSpectrum
{
    numberOfValues = M_MIN(numberOfValues, ARPVALUES_MAX_VALUES);

    for ( unsigned int i = 0; i < numberOfValues; i++)
        spc_s_init_s(
              art_gv,
              mainDiagonal,
            &(outSpectrum[i])
            );

    return numberOfValues;
}

- (unsigned long) getSpectrumValue
        : (ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (ArSpectrum *) outSpectrum
{
    spc_s_init_s(
          art_gv,
          mainDiagonal,
          outSpectrum
        );

    return 1;
}

- (unsigned int) getHiresSpectrumValue
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (ArSpectrum500 *) outSpectrum
{
    s500_s_init_s(
          art_gv,
          hiresMainDiagonal,
          outSpectrum
        );

    return 1;
}

- (unsigned long) getAttenuationValues
        : (ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (unsigned long) numberOfValues
        : (ArAttenuation *) outAttenuations
{
    numberOfValues = M_MIN(numberOfValues, ARPVALUES_MAX_VALUES);

    for ( unsigned int i = 0; i < numberOfValues; i++)
        arattenuation_sx_init_a(
              art_gv,
              mainDiagonal,
              crosstalk,
            &(outAttenuations[i])
            );

    return numberOfValues;
}

- (unsigned long) getAttenuationValue
        : (ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (ArAttenuation *) outAttenuation
{
    arattenuation_sx_init_a(
          art_gv,
          mainDiagonal,
          crosstalk,
          outAttenuation
        );

    return 1;
}

- (unsigned long) getDepolarisingAttenuationValues
        : (ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (ArReferenceFrame *) refframeEntry
        : (ArReferenceFrame *) refframeExit
        : (unsigned long) numberOfValues
        : (ArAttenuation *) outAttenuations
{
    numberOfValues = M_MIN(numberOfValues, ARPVALUES_MAX_VALUES);

    for ( unsigned int i = 0; i < numberOfValues; i++)
        arattenuation_sxrr_init_depolarising_a(
              art_gv,
              mainDiagonal,
              crosstalk,
              refframeEntry,
              refframeExit,
            &(outAttenuations[i])
            );

    return numberOfValues;
}

- (unsigned long) getDepolarisingAttenuationValue
        : (ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (ArReferenceFrame *) refframeEntry
        : (ArReferenceFrame *) refframeExit
        : (ArAttenuation *) outAttenuation
{
    arattenuation_sxrr_init_depolarising_a(
          art_gv,
          mainDiagonal,
          crosstalk,
          refframeEntry,
          refframeExit,
          outAttenuation
        );

    return 1;
}

- (unsigned long) getNonpolarisingAttenuationValues
        : (ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (ArReferenceFrame *) refframeEntry
        : (ArReferenceFrame *) refframeExit
        : (unsigned long) numberOfValues
        : (ArAttenuation *) outAttenuations
{
    numberOfValues = M_MIN(numberOfValues, ARPVALUES_MAX_VALUES);

    for ( unsigned int i = 0; i < numberOfValues; i++)
        arattenuation_sxrr_init_nonpolarising_a(
              art_gv,
              mainDiagonal,
              crosstalk,
              refframeEntry,
              refframeExit,
            &(outAttenuations[i])
            );

    return numberOfValues;
}

- (unsigned long) getNonpolarisingAttenuationValue
        : (ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (ArReferenceFrame *) refframeEntry
        : (ArReferenceFrame *) refframeExit
        : (ArAttenuation *) outAttenuation
{
    arattenuation_sxrr_init_nonpolarising_a(
          art_gv,
          mainDiagonal,
          crosstalk,
          refframeEntry,
          refframeExit,
          outAttenuation
        );

    return 1;
}

- (void) getReflectanceSpectralValue 
        : (const ArcPointContext *) locationInfo 
        : (const ArWavelength *) wavelength 
        : (      ArPathDirection) pathDirection 
        : (      ArSpectralSample *) reflectance 
{ 
    ArCrosstalk500* crosstalkSums = (pathDirection == arpathdirection_from_eye ? hiresHorizontalSums : hiresVerticalSums); 
    int shift_in_x = (pathDirection == arpathdirection_from_eye ? 1 : 0), shift_in_y = 1 - shift_in_x; 
    
    ArWavelength wavelengthA; 
    ArWavelength wavelengthB; 
    c4_dc_sub_c( /* wavelengthA = wavelength - shift_in_x NM */ 
          shift_in_x NM, 
        & ARWL_C(*wavelength), 
        & ARWL_C(wavelengthA) 
        ); 
    c4_dc_add_c( /* wavelengthB = wavelength + shift_in_y NM */ 
          shift_in_y NM, 
        & ARWL_C(*wavelength), 
        & ARWL_C(wavelengthB) 
        ); 
    
    ArSpectralSample crosstalkSum; 
    cx500_wl_wl_init_s( 
          art_gv, 
          crosstalkSums, 
        & wavelengthA, 
        & wavelengthB, 
        & crosstalkSum 
        ); 
    ArSpectralSample mainReflectance; 
    sps_s500w_init_s( 
          art_gv, 
          hiresMainDiagonal, 
          wavelength, 
        & mainReflectance 
        ); 
    
    sps_ss_add_s( 
        art_gv, 
        & mainReflectance, 
        & crosstalkSum, 
          reflectance 
        ); 
} 


- (BOOL) randomWavelengthShift 
        : (const ArcPointContext *) locationInfo 
        : (const ArWavelength *) inputWavelength 
        : (      id <ArpRandomGenerator>) randomGenerator 
        : (      ArPathDirection) pathDirection 
        : (      ArWavelength *) outputWavelength 
        : (      ArSpectralSample *) attenuation 
        : (      ArPDFValue *) probability 
{ 
    ArCrosstalk500* crosstalkSums = (pathDirection == arpathdirection_from_eye ? hiresHorizontalSums : hiresVerticalSums); 
    int shift_in_x = (pathDirection == arpathdirection_from_eye ? 1 : 0), shift_in_y = 1 - shift_in_x; 
    
    ArWavelength wavelengthA; 
    ArWavelength wavelengthB; 
    c4_dc_sub_c( /* wavelengthA = inputWavelength - shift_in_x NM */ 
          shift_in_x NM, 
        & ARWL_C(*inputWavelength), 
        & ARWL_C(wavelengthA) 
        ); 
    c4_dc_add_c( /* wavelengthB = inputWavelength + shift_in_y NM */ 
          shift_in_y NM, 
        & ARWL_C(*inputWavelength), 
        & ARWL_C(wavelengthB) 
        ); 
    
    ArSpectralSample crosstalkSum; 
    cx500_wl_wl_init_s( 
          art_gv, 
          crosstalkSums, 
        & wavelengthA, 
        & wavelengthB, 
        & crosstalkSum 
        ); 
    ArSpectralSample mainReflectance; 
    sps_s500w_init_s( 
          art_gv, 
          hiresMainDiagonal, 
          inputWavelength, 
        & mainReflectance 
        ); 
    
    ArSpectralSample totalReflectance; 
    sps_ss_add_s( /* totalReflectance = mainReflectance + crosstalkSum */ 
        art_gv, 
        & mainReflectance, 
        & crosstalkSum, 
        & totalReflectance 
        ); 
    if(SPS_CI(totalReflectance, 0) == 0.0) 
        return NO; 
    
    arpdfvalue_d_init_p(1.0, probability); 
    
    ArSpectralSample probabilities; double pdf = 1.0; 
    for( int i = 0; i < HERO_SAMPLES_TO_SPLAT; ++i ) 
    { 
        if(SPS_CI(totalReflectance, i) == 0.0) 
        { 
            ARWL_WI(*outputWavelength, i) = ARWL_WI(*inputWavelength, i); 
            SPS_CI(*attenuation, i) = 0.0; 
            SPS_CI(probabilities, i) = 1.0; 
            ARPDFVAL_PI(*probability, i) = 0.0; /* shortcircuit the probability */ 
            continue; 
        } 
        
        double totalReflectanceInverse = 1 / SPS_CI(totalReflectance, i); 
        double mainProbability = SPS_CI(mainReflectance, i) * totalReflectanceInverse; 
        
        double randomValue = [ randomGenerator valueFromNewSequence ]; 
        if(randomValue < mainProbability) 
        { 
            ARWL_WI(*outputWavelength, i) = ARWL_WI(*inputWavelength, i); 
            SPS_CI(*attenuation, i) = SPS_CI(mainReflectance, i); 
            SPS_CI(probabilities, i) = mainProbability; 
            pdf *= mainProbability; 
        } 
        else 
        { 
            double sumValue = [ randomGenerator valueFromNewSequence ] * SPS_CI(crosstalkSum, i); 
            
            int cidx_i = round(NANO_FROM_UNIT(ARWL_WI(*inputWavelength, i)) - ARCROSSTALK500_LOWER_BOUND); 
            int from, to; 
            from = 0 + shift_in_y * (cidx_i + 1); 
            to = cidx_i * shift_in_x + shift_in_y * ARCROSSTALK500_SPECTRAL_CHANNELS - 1; 
    
            while(from < to) 
            { 
                int center = (from + to) / 2; 
                double centerValue = 
                    CX500_XY( 
                        *crosstalkSums, 
                        center * shift_in_x + shift_in_y * cidx_i, 
                        cidx_i * shift_in_x + shift_in_y * center 
                    ); 
    
                switch (pathDirection) { 
                    case arpathdirection_from_eye: 
                        if(sumValue > centerValue) { 
                            from = center + 1; /* shift right */ 
                        } else { 
                            to = center; 
                        } 
                    break; 
                    case arpathdirection_from_light: 
                    if(sumValue > centerValue) { 
                        to = center; /* shift left */ 
                    } else { 
                        from = center + 1; 
                    } 
                    default: 
                        break; 
                } 
            } 
    
            if(from == to) 
            { 
                double outWL = (from + ARCROSSTALK500_LOWER_BOUND + [ randomGenerator valueFromNewSequence ]) NM; 
                ARWL_WI(*outputWavelength, i) = outWL; 
                SPS_CI(*attenuation, i) = 
                    cx500_dd_value( 
                        art_gv, 
                        hiresCrosstalk, 
                        ARWL_WI(*outputWavelength, i) * shift_in_x + shift_in_y * ARWL_WI(*inputWavelength, i), 
                        ARWL_WI(*inputWavelength, i) * shift_in_x + shift_in_y * ARWL_WI(*outputWavelength, i) 
                    ); 
                double shiftProbability = SPS_CI(*attenuation, i) * totalReflectanceInverse; 
                SPS_CI(probabilities, i) = shiftProbability; 
                pdf *= shiftProbability; 
            } 
            else /* this should never happend */ 
            { 
                ART_ERRORHANDLING_FATAL_ERROR( 
                    "Binary searching a reradiation matrix didn't produce a result although it was expected." 
                    ); 
            } 
        } 
    } 
    
    /* attenuation *= ( pdf / probabilities = ( product(t != k) p_t )_k ) */ 
    sps_inv_s( 
          art_gv, 
        & probabilities 
        ); 
    sps_d_mul_s( 
          art_gv, 
          pdf, 
        & probabilities 
        ); 
    sps_s_mul_s( 
          art_gv, 
        & probabilities, 
          attenuation 
        ); 
    
    arpdfvalue_d_mul_p( 
          pdf, 
          probability 
        ); 
    
    return ( SPS_CI(*attenuation, 0) > 0.0 ); 
} 
 
- (BOOL) attenuationForWavelengthShift 
        : (const ArcPointContext *) locationInfo 
        : (const ArWavelength *) inputWavelength 
        : (const ArWavelength *) outputWavelength 
        : (      ArPathDirection) pathDirection 
        : (      ArSpectralSample *) attenuation 
        : (      ArPDFValue *) probability 
{ 
    ArCrosstalk500* crosstalkSums = (pathDirection == arpathdirection_from_eye ? hiresHorizontalSums : hiresVerticalSums); 
    int shift_in_x = (pathDirection == arpathdirection_from_eye ? 1 : 0), shift_in_y = 1 - shift_in_x; 
    
    ArWavelength wavelengthA; 
    ArWavelength wavelengthB; 
    c4_dc_sub_c( /* wavelengthA = inputWavelength - shift_in_x NM */ 
          shift_in_x NM, 
        & ARWL_C(*inputWavelength), 
        & ARWL_C(wavelengthA) 
        ); 
    c4_dc_add_c( /* wavelengthB = inputWavelength + shift_in_y NM */ 
          shift_in_y NM, 
        & ARWL_C(*inputWavelength), 
        & ARWL_C(wavelengthB) 
        ); 
    
    ArSpectralSample crosstalkSum; 
    cx500_wl_wl_init_s( 
          art_gv, 
          crosstalkSums, 
        & wavelengthA, 
        & wavelengthB, 
        & crosstalkSum 
        ); 
    ArSpectralSample mainReflectance; 
    sps_s500w_init_s( 
          art_gv, 
          hiresMainDiagonal, 
          inputWavelength, 
        & mainReflectance 
        ); 
    
    ArSpectralSample totalReflectance; 
    sps_ss_add_s( /* totalReflectance = mainReflectance + crosstalkSum */ 
        art_gv, 
        & mainReflectance, 
        & crosstalkSum, 
        & totalReflectance 
        ); 
    if(SPS_CI(totalReflectance, 0) == 0.0) 
        return NO; 
    
    if(probability) 
        arpdfvalue_d_init_p(1.0, probability); 
    
    ArSpectralSample probabilities; double pdf = 1.0; 
    for( int i = 0; i < HERO_SAMPLES_TO_SPLAT; ++i ) 
    { 
        if(SPS_CI(totalReflectance, i) == 0.0) 
        { 
            SPS_CI(*attenuation, i) = 0.0; 
            SPS_CI(probabilities, i) = 1.0; 
            if(probability) 
                ARPDFVAL_PI(*probability, i) = 0.0; /* shortcircuit the probability */ 
            continue; 
        } 
        
        double totalReflectanceInverse = 1 / SPS_CI(totalReflectance, i); 
        
        if( ARWL_WI(*inputWavelength,i) ==  ARWL_WI(*outputWavelength,i)) 
        { 
            /* main diagonal */ 
            SPS_CI(*attenuation,i) = SPS_CI(mainReflectance, i); 
            double mainProbability = SPS_CI(mainReflectance, i) * totalReflectanceInverse; 
            SPS_CI(probabilities, i) = mainProbability; 
            pdf *= mainProbability; 
        } 
        else 
        { 
            /* crosstalk */ 
            SPS_CI(*attenuation, i) = 
                cx500_dd_value( 
                    art_gv, 
                    hiresCrosstalk, 
                    ARWL_WI(*outputWavelength, i) * shift_in_x + shift_in_y * ARWL_WI(*inputWavelength, i), 
                    ARWL_WI(*inputWavelength, i) * shift_in_x + shift_in_y * ARWL_WI(*outputWavelength, i) 
                ); 
            double shiftProbability = SPS_CI(*attenuation, i) * totalReflectanceInverse; 
            SPS_CI(probabilities, i) = shiftProbability; 
            pdf *= shiftProbability; 
        } 
    } 
    
    /* attenuation *= ( pdf / probabilities = ( product(t != k) p_t )_k ) */ 
    sps_inv_s( 
          art_gv, 
        & probabilities 
        ); 
    sps_d_mul_s( 
          art_gv, 
          pdf, 
        & probabilities 
        ); 
    sps_s_mul_s( 
          art_gv, 
        & probabilities, 
          attenuation 
        ); 
    
    if(probability) 
        arpdfvalue_d_mul_p( 
              pdf, 
              probability 
            ); 
    
    return ( SPS_CI(*attenuation, 0) > 0.0 ); 
} 

- (BOOL) isFluorescent 
{ 
    return YES; 
} 



ARPVALUES_STANDARD_VALUETYPE_IMPLEMENTATION(
    arvalue_spectrum | arvalue_attenuation
    )

ARPVALUES_NULLARY_EVALENVTYPE_IMPLEMENTATION(arevalenv_none)

- (void) getNewPSSpectrum
        : (ArcPointContext *) locationInfo
        : (ArPSSpectrum *) outPSSpectrum
{
    rss2d_to_pss_new( art_gv, nativeValue, outPSSpectrum );
}

- (double) valueAtWavelength
        : (ArcPointContext *) locationInfo
        : (const double) wavelength
{
    ART_ERRORHANDLING_FATAL_ERROR(
        "valueAtWavelength:: not implemented yet"
        );

    return 0.0;
}

+ (double) pdf_gaussian
        : (const     double  ) x
        : (const     double  ) y
        : (const gsl_vector *) mean
        : (const gsl_matrix *) covariance
{
    int s;
    gsl_matrix* X  = gsl_matrix_alloc(2, 1);
    gsl_matrix* LU = gsl_matrix_alloc(2, 2);
    
    // For storing intermediate results
    gsl_matrix* A = gsl_matrix_alloc(1, 2);
    gsl_matrix* B = gsl_matrix_alloc(1, 1);

    gsl_matrix* inv_covariance = gsl_matrix_alloc(2, 2);
    gsl_permutation * p = gsl_permutation_alloc(2);

    // [x, y] - mean
    gsl_matrix_set(X, 0, 0, x - gsl_vector_get(mean, 0));
    gsl_matrix_set(X, 1, 0, y - gsl_vector_get(mean, 1));

    // inv_covariance = covariance^-1
    gsl_matrix_memcpy(LU, covariance);
    gsl_linalg_LU_decomp(LU, p, &s);    
    gsl_linalg_LU_invert(LU, p, inv_covariance);

    const double det = gsl_linalg_LU_det(LU, 2);
    const double norm = 1./(2.*M_PI*sqrt(det));
    
    // TODO: address this
    if (norm != norm) {
        // fprintf(stderr, "Invalid normalization");
        return 0;
    }

    // fn = X.transpose() * inv_covariance * X;
    gsl_blas_dgemm(CblasTrans, CblasNoTrans, 1.0, X, inv_covariance, 0.0, A); 
    gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, A, X, 0.0, B);
    const double fn = gsl_matrix_get(B, 0, 0);

    gsl_matrix_free(X);
    gsl_matrix_free(LU);
    gsl_matrix_free(inv_covariance);
    gsl_matrix_free(A);
    gsl_matrix_free(B);
    gsl_permutation_free(p);

    return norm * exp(-0.5 * fn);
}

- (double) attenuation
        : (const double          ) lambda_i // in nanometers
        : (const double          ) lambda_o // in nanometers
{
    double res = 0;

    for (int i = 0; i < n_gaussians; i++) {
        res += gaussian_params[i].weight * 
            [Arn2DGMMSpectrumDebugTab pdf_gaussian
                :  lambda_i
                :  lambda_o
                :  gaussian_params[i].mean
                :  gaussian_params[i].covariance
            ];
    }

    return scaling_attenuation * res;
}

@end

// ===========================================================================
