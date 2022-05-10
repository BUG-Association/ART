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

#define ART_MODULE_NAME     Arn2DGMMSpectrum

#import "Arn2DGMMSpectrum.h"
#import "ART_ImageFileFormat.h"
#import "ArnColourStandardImplementation.h"

double eval_gaussian_2d(
    double covariance_det,
    const Mat2* inv_covariance,
    const Pnt2D* mean,
    double x, double y)
{
    const double xx = x - XC(*mean);
    const double yy = y - YC(*mean);

    const double v = 
        xx * (inv_covariance->x[0][0] * xx + inv_covariance->x[1][0] * yy) +
        yy * (inv_covariance->x[0][1] * xx + inv_covariance->x[1][1] * yy);

    return exp(-.5 * v) / (2.*M_PI*sqrt(covariance_det));
}


double eval_gaussian_1d(
    double stdev, 
    double mean, 
    double x)
{
    const float v = (x - mean) / stdev;

    return exp(-.5 * v * v) / (sqrt(2.f * M_PI) * stdev);
}


// Conditional gaussian mixture parameters
typedef struct {
    double mu;
    double sigma;
    double weight;
} cGMMParameters;


// ============================================================================
// Class implementation
// ============================================================================

ART_MODULE_INITIALISATION_FUNCTION
(
    [Arn2DGMMSpectrum registerWithRuntime];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

@implementation Arn2DGMMSpectrum

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(Arn2DGMMSpectrum)


// ----------------------------------------------------------------------------
// Constructors / Destructors
// ----------------------------------------------------------------------------

- deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    Arn2DGMMSpectrum *copiedInstance =
        [super deepSemanticCopy
            : traversal
        ];

    copiedInstance =
        [copiedInstance init
            : _n_gaussians
            : ardoublearray_array(&_means)
            : ardoublearray_array(&_covariances)
            : ardoublearray_array(&_weights)
            : _scaling_attenuation
            : ARRSS_SIZE (_diagonal)
            : ARRSS_START(_diagonal)
            : ARRSS_STEP (_diagonal)
            : ARRSS_ARRAY(_diagonal)
        ];

    return copiedInstance;
}


- init
        : (int)      n_gaussians
        : (double *) means
        : (double *) covariances
        : (double *) weights
        : (double  ) scaling_attenuation
        : (int)      diagonal_size
        : (double)   diagonal_start
        : (double)   diagonal_step
        : (double *) diagonal_values
{
    self = [super init];

    if (self) {
        // Init the GMM scaling for reradiation
        _scaling_attenuation = scaling_attenuation;

        // Init the Gaussian parameters
        _n_gaussians = n_gaussians;
        _weights     = ardoublearray_init(_n_gaussians);
        _means       = ardoublearray_init(2 * _n_gaussians);
        _covariances = ardoublearray_init(3 * _n_gaussians);

        memcpy(
            ardoublearray_array(&_weights),
            weights,
            n_gaussians * sizeof(double));
        memcpy(
            ardoublearray_array(&_means), 
            means, 
            2 * n_gaussians * sizeof(double));
        memcpy(
            ardoublearray_array(&_covariances),
            covariances,
            3 * n_gaussians * sizeof(double));

        // Init the diagonal
        ARRSS_SIZE (_diagonal) = (unsigned long) diagonal_size;
        ARRSS_START(_diagonal) = diagonal_start;
        ARRSS_STEP (_diagonal) = diagonal_step;
        ARRSS_SCALE(_diagonal) = 1.0;
        ARRSS_ARRAY(_diagonal) = ALLOC_ARRAY(double, ARRSS_SIZE(_diagonal));

        memcpy(
            ARRSS_ARRAY(_diagonal),
            diagonal_values,
            ARRSS_SIZE(_diagonal) * sizeof(double));

        // Init remaining fields
        _mainDiagonal             = NULL;
        _hiresMainDiagonal        = NULL;
        _hiresCrosstalksums_eye   = NULL;
        _hiresCrosstalksums_light = NULL;

        _gaussian_params = NULL;
    }

    return self;
}


- (void) dealloc 
{
    ardoublearray_free_contents(&_weights);
    ardoublearray_free_contents(&_means);
    ardoublearray_free_contents(&_covariances);

    FREE_ARRAY(ARRSS_ARRAY(_diagonal));

    if (_gaussian_params) { FREE_ARRAY(_gaussian_params); }

    if (_mainDiagonal)             { spc_free(art_gv, _mainDiagonal); }
    if (_hiresMainDiagonal)        { s500_free(art_gv, _hiresMainDiagonal); }
    if (_hiresCrosstalksums_eye)   { s500_free(art_gv, _hiresCrosstalksums_eye); }
    if (_hiresCrosstalksums_light) { s500_free(art_gv, _hiresCrosstalksums_light); }

    [super dealloc];
}


- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code:coder ];

    [ coder codeDouble: &_scaling_attenuation ];

    [ coder codeInt: &_n_gaussians ];

    arpcoder_ardoublearray(coder, &_means);
    arpcoder_ardoublearray(coder, &_covariances);
    arpcoder_ardoublearray(coder, &_weights);

    arpcoder_arrsspectrum(art_gv, coder, &_diagonal);

    if ([ coder isReading ]) {
        [ self _setup ];
    }
}


- (void) _setup 
{
    // Allocate memory
    if (!_gaussian_params) { _gaussian_params = ALLOC_ARRAY(GMMParameters, _n_gaussians); }

    if (!_mainDiagonal)             { _mainDiagonal             = spc_alloc(art_gv); }
    if (!_hiresMainDiagonal)        { _hiresMainDiagonal        = s500_alloc(art_gv); }
    if (!_hiresCrosstalksums_light) { _hiresCrosstalksums_light = s500_alloc(art_gv); }
    if (!_hiresCrosstalksums_eye)   { _hiresCrosstalksums_eye   = s500_alloc(art_gv); }

    // ------------------------------------------------------------------------
    // Organize the GMM params in a more understandable way
    // ------------------------------------------------------------------------

    // Ensure weight sum equals 1
    double totalWeight = 0;
    
    for (int i = 0; i < _n_gaussians; i++) {
        totalWeight += ARARRAY_I(_weights, i);
    }

    // Setup the Gaussian parameters
    for (int i = 0; i < _n_gaussians; i++) {
        _gaussian_params[i].mean = PNT2D(
            ARARRAY_I(_means, 2 * i + 0),
            ARARRAY_I(_means, 2 * i + 1)
        );

        // Ensures weight sum equals 1
        _gaussian_params[i].weight = ARARRAY_I(_weights, i) / totalWeight;

        // Covariance
        _gaussian_params[i].covariance = MAT2(
            ARARRAY_I(_covariances, 3 * i + 0), ARARRAY_I(_covariances, 3 * i + 1),
            ARARRAY_I(_covariances, 3 * i + 1), ARARRAY_I(_covariances, 3 * i + 2)
        );
        
        // Inverse Covariance
        double det = c2_m_det(&_gaussian_params[i].covariance);

        _gaussian_params[i].inverted_covariance = MAT2(
            ARARRAY_I(_covariances, 3 * i + 2) / det, -ARARRAY_I(_covariances, 3 * i + 1) / det,
            -ARARRAY_I(_covariances, 3 * i + 1) / det, ARARRAY_I(_covariances, 3 * i + 0) / det
        );

        // TODO: Store normalization factor instead
        _gaussian_params[i].det = det;
    }

    _scaling_attenuation /= totalWeight;

    // ------------------------------------------------------------------------
    // Local cache
    // ------------------------------------------------------------------------

    // Provide native representation for the main diagonal
    rss_to_spc(art_gv, &_diagonal, _mainDiagonal);
    rss_to_s500(art_gv, &_diagonal, _hiresMainDiagonal);

    // Precomputation of the PDF sums on lines / columns
    // Note this is rather the reradiation slice sums but this does not matter
    // on this implementation: the reradiation matches the PDF and the scaling
    // factor is canceld when computing the final PDF in this specific code
    // (see randomWavelengthShift / attenuationForWavelengthShift PDF 
    // computation when sampling the reradiation)

    double wl_start    = ARCROSSTALK500_LOWER_BOUND NM;
    double wl_end      = ARCROSSTALK500_UPPER_BOUND NM;
    double wl_step     = 1 NM;
    int n_wl           = (wl_end - wl_start) / wl_step + 1;

    // light -> eyes
    ArRSSpectrum rsscrosstalksums_light;
    ARRSS_SIZE (rsscrosstalksums_light) = n_wl;
    ARRSS_START(rsscrosstalksums_light) = wl_start;
    ARRSS_STEP (rsscrosstalksums_light) = wl_step;
    ARRSS_SCALE(rsscrosstalksums_light) = 1.0;
    ARRSS_ARRAY(rsscrosstalksums_light) = ALLOC_ARRAY( double, ARRSS_SIZE(rsscrosstalksums_light) );

    for (int i = 0; i < n_wl; i++) {
        const double lambda_i = wl_start + i * wl_step;

        double sum = 0;

        for (double lambda_o = lambda_i + wl_step; lambda_o <= wl_end; lambda_o += wl_step) {
            assert(lambda_i < lambda_o);
            sum += 
                [self reradiation
                    : lambda_i
                    : lambda_o
                ];
        }

        // TODO FIXME: This is an ugly thing!
        ARRSS_ARRAY_I(rsscrosstalksums_light, i) = sum / 10.;
    }

    rss_to_s500(art_gv, &rsscrosstalksums_light, _hiresCrosstalksums_light);
    FREE_ARRAY(ARRSS_ARRAY(rsscrosstalksums_light));

    // eyes -> light
    ArRSSpectrum rsscrosstalksums_eye;
    ARRSS_SIZE (rsscrosstalksums_eye) = n_wl;
    ARRSS_START(rsscrosstalksums_eye) = wl_start;
    ARRSS_STEP (rsscrosstalksums_eye) = wl_step;
    ARRSS_SCALE(rsscrosstalksums_eye) = 1.0;
    ARRSS_ARRAY(rsscrosstalksums_eye) = ALLOC_ARRAY( double, ARRSS_SIZE(rsscrosstalksums_light) );

    for (int i = 0; i < n_wl; i++) {
        const double lambda_o = wl_start + i * wl_step;

        double sum = 0;

        for (double lambda_i = wl_start; lambda_i < lambda_o; lambda_i += wl_step) {
            assert(lambda_i < lambda_o);
            sum += 
                [self reradiation
                    : lambda_i
                    : lambda_o
                ];
        }

        // TODO FIXME: This is an ugly thing!
        ARRSS_ARRAY_I(rsscrosstalksums_eye, i) = sum / 10.;
    }

    rss_to_s500(art_gv, &rsscrosstalksums_eye, _hiresCrosstalksums_eye);
    FREE_ARRAY(ARRSS_ARRAY(rsscrosstalksums_eye));
}


// ----------------------------------------------------------------------------
// Sampling functions
// ----------------------------------------------------------------------------

+ (double) sample_gaussian_1d
        : (double) stdev
        : (double) mean
        : (id <ArpRandomGenerator>) randomGenerator
{
    double r2 = 0.;
    double x1, x2;

    do {
        double r0 = [ randomGenerator valueFromNewSequence ];
        double r1 = [ randomGenerator valueFromNewSequence ];

        x1 = 2. * r0 - 1.;
        x2 = 2. * r1 - 1.;
        r2 = x1 * x1 + x2 * x2;
    } while (r2 >= 1. || r2 == 0.);

    double v = x2 * sqrt(-2.f * log(r2) / r2);

    return stdev * v + mean;
}


+ (double) sample_gmm_1d
        : (int) n_gaussians
        : (const cGMMParameters*) p
        : (id <ArpRandomGenerator>) randomGenerator
{
    // Pick the Gaussian to sample
    double r0 = [ randomGenerator valueFromNewSequence ];
    int g_idx = 0;
    double cum_weights = 0.;

    for (g_idx = 0; g_idx < n_gaussians; g_idx++) {
        cum_weights += p[g_idx].weight;
        
        if (r0 < cum_weights) {
            break;
        }
    }

    // Sample the picked Gaussian
    return 
        [ Arn2DGMMSpectrum sample_gaussian_1d
            : p[g_idx].sigma
            : p[g_idx].mu
            : randomGenerator
        ];
}


/**
 * Generates conditional Gaussian Mixture pdf for a given lambda_o slice
 */
- (void) vertical_sampling_pdf_cgmm
        : (double) lambda_o
        : (cGMMParameters*) cond_gaussian_params
{
    double *cond_weigths = (double*)calloc(_n_gaussians, sizeof(double));
    double sum_cond_weights = 0.0;

    lambda_o = NANO_FROM_UNIT(lambda_o);

    // Calcualte new weights
    for (int i = 0; i < _n_gaussians; i++) {
        double mean_a = XC(_gaussian_params[i].mean);
        double cov_aa = _gaussian_params[i].covariance.x[0][0];

        double sigma = sqrt(cov_aa);

        cond_weigths[i] = _gaussian_params[i].weight * eval_gaussian_1d(sigma, mean_a, lambda_o);

        sum_cond_weights += cond_weigths[i];
    }

    // Calculate conditional means and sigmas
    for (int i = 0; i < _n_gaussians; i++) {
        double mean_a = XC(_gaussian_params[i].mean);
        double mean_b = YC(_gaussian_params[i].mean);

        double cov_inv_bb = _gaussian_params[i].inverted_covariance.x[1][1];
        double cov_inv_ba = _gaussian_params[i].inverted_covariance.x[1][0];

        double mean_b_a = mean_b - 1.0 / cov_inv_bb * cov_inv_ba * (lambda_o - mean_a);

        cond_gaussian_params[i].mu = mean_b_a;
        cond_gaussian_params[i].sigma = sqrt(1.0 / cov_inv_bb);
        cond_gaussian_params[i].weight = cond_weigths[i] / sum_cond_weights;
    }

    free(cond_weigths);
}


/**
 * Generates conditional Gaussian Mixture pdf for a given lambda_i slice
 */
- (void) horizontal_sampling_pdf_cgmm
        : (double) lambda_i
        : (cGMMParameters*) cond_gaussian_params
{
    double *cond_weigths = (double*)calloc(_n_gaussians, sizeof(double));
    double sum_cond_weights = 0.0;

    lambda_i = NANO_FROM_UNIT(lambda_i);

    // Calcualte new weights
    for (int i = 0; i < _n_gaussians; i++) {
        double mean_b = YC(_gaussian_params[i].mean);
        double cov_bb = _gaussian_params[i].covariance.x[1][1];

        double sigma = sqrt(cov_bb);

        cond_weigths[i] = _gaussian_params[i].weight * eval_gaussian_1d(sigma, mean_b, lambda_i);

        sum_cond_weights += cond_weigths[i];
    }

    // Calculate conditional means and sigmas
    for (int i = 0; i < _n_gaussians; i++) {
        double mean_a = XC(_gaussian_params[i].mean);
        double mean_b = YC(_gaussian_params[i].mean);

        double cov_inv_aa = _gaussian_params[i].inverted_covariance.x[0][0];
        double cov_inv_ab = _gaussian_params[i].inverted_covariance.x[0][1];

        double mean_a_b = mean_a - 1.0 / cov_inv_aa * cov_inv_ab * (lambda_i - mean_b);

        cond_gaussian_params[i].mu = mean_a_b;
        cond_gaussian_params[i].sigma = sqrt(1.0 / cov_inv_aa);
        cond_gaussian_params[i].weight = cond_weigths[i] / sum_cond_weights;
    }

    free(cond_weigths);
}


/**
 * Generates conditional Gaussian Mixture pdf for a given slice
 */
- (void) construct_params_cgmm
        : (ArPathDirection) pathDirection
        : (double         ) wavelength
        : (cGMMParameters*) cond_gaussian_params
{
    // Gererate pdf and conditional pdfs that are conditional on
    // input or output wavelength.
    if (pathDirection == arpathdirection_from_light) {
        [ self vertical_sampling_pdf_cgmm 
            : wavelength
            : cond_gaussian_params
        ];

    } else {
        [ self horizontal_sampling_pdf_cgmm 
            : wavelength
            : cond_gaussian_params
        ];
    }
}


// ----------------------------------------------------------------------------
// Sampling and evaluation
// ----------------------------------------------------------------------------

- (BOOL) randomWavelengthShift
        : (const ArcPointContext * ) locationInfo
        : (const ArWavelength *    ) inputWavelength
        : (      id <ArpRandomGenerator>) randomGenerator
        : (      ArPathDirection   ) pathDirection
        : (      ArWavelength *    ) outputWavelength
        : (      ArSpectralSample *) attenuation
        : (      ArPDFValue *      ) probability
{
    cGMMParameters* cond_gaussian_params = ALLOC_ARRAY(cGMMParameters, _n_gaussians);

    ArSpectralSample mainReflectance;

    sps_s500w_init_s(
            art_gv,
            _hiresMainDiagonal,
            inputWavelength,
            & mainReflectance
    );

    ArSpectralSample crosstalkSum;

    if (pathDirection == arpathdirection_from_eye) {
        sps_s500w_init_s(
                art_gv,
                _hiresCrosstalksums_eye,
                inputWavelength,
                &crosstalkSum
        );
    } else {
        sps_s500w_init_s(
                art_gv,
                _hiresCrosstalksums_light,
                inputWavelength,
                &crosstalkSum
        );
    }

    arpdfvalue_d_init_p(1.0, probability);

    ArSpectralSample probabilities;
    double pdf = 1.0;
    
    for (unsigned int i = 0; i < HERO_SAMPLES_TO_SPLAT; ++i) {
        const double fluoReflectance  = SPS_CI(crosstalkSum, i);
        const double diagReflectance  = SPS_CI(mainReflectance, i);
        const double totalReflectance = diagReflectance + fluoReflectance;
    
        // No reflectance at all
        if (totalReflectance == 0.0) {
            ARWL_WI(*outputWavelength, i) = ARWL_WI(*inputWavelength, i);
            SPS_CI(*attenuation, i) = 0.0;
            SPS_CI(probabilities, i) = 1.0;
            continue;
        }

        const double mainProbability = diagReflectance / totalReflectance;
        const double randomValue = [ randomGenerator valueFromNewSequence ];

        if (randomValue < mainProbability) {
            // We sampled the diagonal
            ARWL_WI(*outputWavelength, i) = ARWL_WI(*inputWavelength, i);
            SPS_CI(*attenuation, i) = SPS_CI(mainReflectance, i);
            SPS_CI(probabilities, i) = mainProbability;
        } else {
            // We are sampling the reradiation
            
            // Construct conditional parameters
            [ self construct_params_cgmm
                : pathDirection 
                : ARWL_WI(*inputWavelength, i)
                : cond_gaussian_params
            ];

            const double in_wl_nm = NANO_FROM_UNIT(ARWL_WI(*inputWavelength, i));
            double sampled_wl_nm = 0.;

            // Rejection sampling
            do {
                sampled_wl_nm = 
                    [ Arn2DGMMSpectrum sample_gmm_1d
                        : _n_gaussians
                        : cond_gaussian_params
                        : randomGenerator
                    ];
            } while (
                   (pathDirection == arpathdirection_from_eye   && (sampled_wl_nm >= in_wl_nm))
                || (pathDirection == arpathdirection_from_light && (sampled_wl_nm <= in_wl_nm))
                || sampled_wl_nm < ARCROSSTALK500_LOWER_BOUND || sampled_wl_nm > ARCROSSTALK500_UPPER_BOUND);

            ARWL_WI(*outputWavelength, i) = sampled_wl_nm NM;

            // Reradiation value, parameter order depends on the ray origin
            if (pathDirection == arpathdirection_from_eye) {
                SPS_CI(*attenuation, i) = 
                    [ self reradiation
                        : ARWL_WI(*outputWavelength, i)
                        : ARWL_WI(*inputWavelength, i)
                    ];
            } else {
                SPS_CI(*attenuation, i) = 
                    [ self reradiation
                        : ARWL_WI(*inputWavelength, i)
                        : ARWL_WI(*outputWavelength, i)
                    ];
            }

            SPS_CI(probabilities, i) = 
                (1. - mainProbability) 
                * (SPS_CI(*attenuation, i)/fluoReflectance);
        }

        pdf *= SPS_CI(probabilities, i);
    }

    // HWSS:
    // attenuation *= ( pdf / probabilities = ( product(t != k) p_t )_k )
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

    FREE_ARRAY(cond_gaussian_params);

    return ( SPS_CI(*attenuation, 0) > 0.0 );
}


- (BOOL) attenuationForWavelengthShift
        : (const ArcPointContext * ) locationInfo
        : (const ArWavelength *    ) inputWavelength
        : (const ArWavelength *    ) outputWavelength
        : (      ArPathDirection   ) pathDirection
        : (      ArSpectralSample *) attenuation
        : (      ArPDFValue *      ) probability
{
    ArSpectralSample mainReflectance;

    sps_s500w_init_s(
            art_gv,
            _hiresMainDiagonal,
            inputWavelength,
            & mainReflectance
    );

    ArSpectralSample crosstalkSum;

    if (pathDirection == arpathdirection_from_eye) {
        sps_s500w_init_s(
                art_gv,
                _hiresCrosstalksums_eye,
                inputWavelength,
                &crosstalkSum
        );
    } else {
        sps_s500w_init_s(
                art_gv,
                _hiresCrosstalksums_light,
                inputWavelength,
                &crosstalkSum
        );
    }

    if (probability) {
        arpdfvalue_d_init_p(1.0, probability);
    }

    ArSpectralSample probabilities; 
    double pdf = 1.0;
    
    for (unsigned int i = 0; i < HERO_SAMPLES_TO_SPLAT; ++i)
    {
        const double fluoReflectance  = SPS_CI(crosstalkSum, i);
        const double diagReflectance  = SPS_CI(mainReflectance, i);
        const double totalReflectance = diagReflectance + fluoReflectance;

        // No reflectance at all
        if (totalReflectance == 0.0) {
            SPS_CI(*attenuation, i) = 0.0;
            SPS_CI(probabilities, i) = 1.0;
            continue;
        }

        const double mainProbability = diagReflectance / totalReflectance;

        if (ARWL_WI(*inputWavelength,i) == ARWL_WI(*outputWavelength,i)) {
            // We sampled the diagonal
            SPS_CI(*attenuation, i) = SPS_CI(mainReflectance, i);
            SPS_CI(probabilities, i) = mainProbability;
        } else {
            // We sampled the reradiation

            // Reradiation value, parameter order depends on the ray origin
            if (pathDirection == arpathdirection_from_eye) {
                SPS_CI(*attenuation, i) = 
                    [ self reradiation
                        : ARWL_WI(*outputWavelength, i)
                        : ARWL_WI(*inputWavelength, i)
                    ];
            } else {
                SPS_CI(*attenuation, i) = 
                    [ self reradiation
                        : ARWL_WI(*inputWavelength, i)
                        : ARWL_WI(*outputWavelength, i)
                    ];
            }

            SPS_CI(probabilities, i) = 
                (1. - mainProbability) 
                * (SPS_CI(*attenuation, i)/fluoReflectance);
        }

        pdf *= SPS_CI(probabilities, i);
    }
    
    // HWSS:
    // attenuation *= ( pdf / probabilities = ( product(t != k) p_t )_k )
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

    if (probability)
        arpdfvalue_d_mul_p(
                pdf,
                probability
        );

    return ( SPS_CI(*attenuation, 0) > 0.0 );
}


- (double) reradiation
        : (const double) lambda_i
        : (const double) lambda_o
{
    assert(lambda_i < lambda_o);

    double res = 0;

    for (int i = 0; i < _n_gaussians; i++) {
        res += 
            _gaussian_params[i].weight * eval_gaussian_2d(
                _gaussian_params[i].det,
                &(_gaussian_params[i].inverted_covariance),
                &(_gaussian_params[i].mean),
                NANO_FROM_UNIT(lambda_i),
                NANO_FROM_UNIT(lambda_o)
            );
    }

    ASSERT_NONNEGATIVE_DOUBLE(res)

    return _scaling_attenuation * res;
}


// ----------------------------------------------------------------------------
// Additional and unimplemented functions
// ----------------------------------------------------------------------------

ARPVALUES_STANDARD_VALUETYPE_IMPLEMENTATION(
        arvalue_spectrum | arvalue_attenuation
)

ARPVALUES_NULLARY_EVALENVTYPE_IMPLEMENTATION(arevalenv_none)


- (void) getReflectanceSpectralValue
        : (const ArcPointContext *) locationInfo
        : (const ArWavelength *)    wavelength
        : (ArPathDirection)         pathDirection
        : (ArSpectralSample *)      reflectance
{
    ArSpectralSample crosstalkSum;

    if (pathDirection == arpathdirection_from_eye) {
        sps_s500w_init_s(
            art_gv,
            _hiresCrosstalksums_eye,
            wavelength,
            &crosstalkSum
        );
    } else {
        sps_s500w_init_s(
            art_gv,
            _hiresCrosstalksums_light,
            wavelength,
            &crosstalkSum
        );
    }

    ArSpectralSample mainReflectance;

    sps_s500w_init_s(
        art_gv,
        _hiresMainDiagonal,
        wavelength,
        &mainReflectance
    );

    sps_ss_add_s(
        art_gv,
        &mainReflectance,
        &crosstalkSum,
        reflectance
    );
}


- (void) getNewPSSpectrum
        : (ArcPointContext *) locationInfo
        : (ArPSSpectrum *   ) outPSSpectrum 
{
    ART__VIRTUAL_METHOD__EXIT_WITH_ERROR
}


- (double) valueAtWavelength
        : (      ArcPointContext *) locationInfo
        : (const double           ) wavelength
{
    ART_ERRORHANDLING_FATAL_ERROR(
            "valueAtWavelength:: not implemented yet"
    );

    return 0.0;
}


- (BOOL) isFluorescent 
{
    return YES;
}


- (void) getSpectrum
        : (ArcPointContext *) locationInfo
        : (ArSpectrum *)      outSpectrum
{
    spc_s_init_s(
        art_gv,
        _mainDiagonal,
        outSpectrum
    );
}


- (void) getHiresSpectrum
        : (ArcPointContext *) locationInfo
        : (ArSpectrum500 *)   outSpectrum
{
    s500_s_init_s(
        art_gv,
        _hiresMainDiagonal,
        outSpectrum
    );
}


- (void) getSpectralSample
        : (const ArcPointContext *) locationInfo
        : (const ArWavelength *)    wavelength
        : (ArSpectralSample *)      outSpectrum 
{
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}


- (void) getLightIntensity
        : (ArcPointContext *)  locationInfo
        : (ArLightIntensity *) outLightIntensity
{
    arlightintensity_s_init_i(
        art_gv,
        _mainDiagonal,
        outLightIntensity
    );
}


- (void) getLight
        : (ArcPointContext *) locationInfo
        : (ArLight *)         outLight
{
    arlight_s_init_unpolarised_l(
        art_gv,
        _mainDiagonal,
        outLight
    );
}


- (void) getAttenuation
        : (ArcPointContext *) locationInfo
        : (ArAttenuation *)   attenuation_r 
{
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}


- (void) getAttenuationSample
        : (const ArcPointContext *) locationInfo
        : (const ArWavelength *)    wavelength
        : (ArAttenuationSample *)   outAttenuation
{
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}


- (void) getDepolarisingAttenuation
        : (ArcPointContext *)   locationInfo
        : (ArReferenceFrame *)  refframeEntry
        : (ArReferenceFrame *)  refframeExit
        : (ArAttenuation *)     attenuation_r 
{
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}


- (void) getDepolarisingAttenuationSample
        : (const ArcPointContext *)  locationInfo
        : (const ArReferenceFrame *) refframeEntry
        : (const ArReferenceFrame *) refframeExit
        : (const ArWavelength *)     wavelength
        : (ArAttenuationSample *)    outAttenuation 
{
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}


- (void) getNonpolarisingAttenuation
        : (ArcPointContext *)   locationInfo
        : (ArReferenceFrame *)  refframeEntry
        : (ArReferenceFrame *)  refframeExit
        : (ArAttenuation *)     attenuation_r
{
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}


- (void) getNonpolarisingAttenuationSample
        : (const ArcPointContext *)  locationInfo
        : (const ArReferenceFrame *) refframeEntry
        : (const ArReferenceFrame *) refframeExit
        : (const ArWavelength *)     wavelength
        : (ArAttenuationSample *)    outAttenuation 
{
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}


- (unsigned long) getSpectrumValues
        : (ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (unsigned long)  numberOfValues
        : (ArSpectrum *)   outSpectrum 
{
    numberOfValues = M_MIN(numberOfValues, ARPVALUES_MAX_VALUES);

    for (unsigned int i = 0; i < numberOfValues; i++)
        spc_s_init_s(
            art_gv,
            _mainDiagonal,
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
        _mainDiagonal,
        outSpectrum
    );

    return 1;
}


- (unsigned int) getHiresSpectrumValue
            :(const ArcObject <ArpEvaluationEnvironment> *) evalEnv
            :(ArSpectrum500 *) outSpectrum 
{
    s500_s_init_s(
        art_gv,
        _hiresMainDiagonal,
        outSpectrum
    );

    return 1;
}


- (unsigned long) getAttenuationValues
        : (ArcObject <ArpEvaluationEnvironment> *)  evalEnv
        : (unsigned long)   numberOfValues
        : (ArAttenuation *) outAttenuations
{
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return 0;
}


- (unsigned long) getAttenuationValue
        : (ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (ArAttenuation *) outAttenuation 
{
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return 1;
}

- (unsigned long) getDepolarisingAttenuationValues
        : (ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (ArReferenceFrame *) refframeEntry
        : (ArReferenceFrame *) refframeExit
        : (unsigned long)      numberOfValues
        : (ArAttenuation *)    outAttenuations
{
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return 0;
}


- (unsigned long) getDepolarisingAttenuationValue
        : (ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (ArReferenceFrame *) refframeEntry
        : (ArReferenceFrame *) refframeExit
        : (ArAttenuation *)    outAttenuation 
{
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return 0;
}


- (unsigned long) getNonpolarisingAttenuationValues
        : (ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (ArReferenceFrame *)  refframeEntry
        : (ArReferenceFrame *)  refframeExit
        : (unsigned long)       numberOfValues
        : (ArAttenuation *)     outAttenuations
{
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return 0;
}


- (unsigned long) getNonpolarisingAttenuationValue
        : (ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (ArReferenceFrame *)   refframeEntry
        : (ArReferenceFrame *)   refframeExit
        : (ArAttenuation *)      outAttenuation
{
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return 1;
}

@end

// ===========================================================================
