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

#define ARNSPECTRUM_STANDARD_METHOD_IMPLEMENTATIONS \
\
- (void) getSpectrum \
        : (ArcPointContext *) locationInfo \
        : (ArSpectrum *) outSpectrum \
{ \
    (void) locationInfo; \
\
    spc_s_init_s( \
        art_gv, \
        spectrum, \
        outSpectrum \
        ); \
} \
\
- (void) getHiresSpectrum \
        : (ArcPointContext *) locationInfo \
        : (ArSpectrum500 *) outSpectrum \
{ \
    (void) locationInfo; \
    \
    s500_s_init_s( \
        art_gv, \
        internalHiresSpectrum, \
        outSpectrum \
        ); \
} \
\
- (void) getSpectralSample \
        : (const ArcPointContext *) locationInfo \
        : (const ArWavelength *) wavelength \
        : (      ArSpectralSample *) outSpectralSample \
{ \
    (void) locationInfo; \
    \
    sps_s500w_init_s( \
          art_gv, \
          internalHiresSpectrum, \
          wavelength, \
          outSpectralSample \
        ); \
} \
\
- (void) getAttenuation \
        : (ArcPointContext *) locationInfo \
        : (ArAttenuation *) outAttenuation \
{ \
    (void) locationInfo; \
    \
    arattenuation_s_init_a( \
        art_gv, \
        spectrum, \
        outAttenuation \
        ); \
} \
\
- (void) getAttenuationSample \
        : (const ArcPointContext *) locationInfo \
        : (const ArWavelength *) wavelength \
        : (      ArAttenuationSample *) outAttenuationSample \
{ \
    (void) locationInfo; \
    \
    ArSpectralSample  temp_ss; \
\
    sps_s500w_init_s( \
          art_gv, \
          internalHiresSpectrum, \
          wavelength, \
        & temp_ss \
        ); \
\
    arattenuationsample_s_init_a( \
          art_gv, \
        & temp_ss, \
          outAttenuationSample \
        ); \
} \
\
- (void) getDepolarisingAttenuation \
        : (ArcPointContext *) locationInfo \
        : (ArReferenceFrame *) refframeEntry \
        : (ArReferenceFrame *) refframeExit \
        : (ArAttenuation *) outAttenuation \
{ \
    (void) locationInfo; \
    \
    arattenuation_srr_init_depolarising_a( \
        art_gv, \
        spectrum, \
        refframeEntry, \
        refframeExit, \
        outAttenuation \
        ); \
} \
\
- (void) getDepolarisingAttenuationSample \
        : (const ArcPointContext *) locationInfo \
        : (const ArReferenceFrame *) refframeEntry \
        : (const ArReferenceFrame *) refframeExit \
        : (const ArWavelength *) wavelength \
        : (      ArAttenuationSample *) outAttenuationSample \
{ \
    (void) locationInfo; \
    \
    ArSpectralSample  temp_sps; \
\
    sps_s500w_init_s( \
          art_gv, \
          internalHiresSpectrum, \
          wavelength, \
        & temp_sps \
        ); \
\
    arattenuationsample_srr_init_depolarising_a( \
          art_gv, \
        & temp_sps, \
          refframeEntry, \
          refframeExit, \
          outAttenuationSample \
        ); \
} \
\
- (void) getNonpolarisingAttenuation \
        : (ArcPointContext *) locationInfo \
        : (ArReferenceFrame *) refframeEntry \
        : (ArReferenceFrame *) refframeExit \
        : (ArAttenuation *) outAttenuation \
{ \
    (void) locationInfo; \
    \
    arattenuation_srr_init_nonpolarising_a( \
        art_gv, \
        spectrum, \
        refframeEntry, \
        refframeExit, \
        outAttenuation \
        ); \
} \
\
- (void) getNonpolarisingAttenuationSample \
        : (const ArcPointContext *) locationInfo \
        : (const ArReferenceFrame *) refframeEntry \
        : (const ArReferenceFrame *) refframeExit \
        : (const ArWavelength *) wavelength \
        : (      ArAttenuationSample *) outAttenuationSample \
{ \
    (void) locationInfo; \
    \
    ArSpectralSample  temp_sps; \
\
    sps_s500w_init_s( \
          art_gv, \
          internalHiresSpectrum, \
          wavelength, \
        & temp_sps \
        ); \
\
    arattenuationsample_srr_init_nonpolarising_a( \
          art_gv, \
        & temp_sps, \
          refframeEntry, \
          refframeExit, \
          outAttenuationSample \
        ); \
} \
\
- (void) prepareForISRChange \
{ \
    [ super prepareForISRChange ]; \
\
    if ( spectrum ) \
    { \
        spc_free( \
            art_gv, \
            spectrum \
            ); \
\
        spectrum = 0; \
    } \
} \
\
- (void) reinitialiseAfterISRChange \
{ \
    [ super reinitialiseAfterISRChange ]; \
    [ self _setup ]; \
} \
\
- (unsigned int) getSpectrumValues \
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv \
        : (const unsigned int) numberOfValues \
        : (      ArSpectrum *) outColours \
{ \
    (void) evalEnv; \
    \
    unsigned int actualNumberOfValues = M_MIN(numberOfValues, ARPVALUES_MAX_VALUES); \
    for (unsigned int i = 0; i < actualNumberOfValues; i++) \
        spc_s_init_s( \
            art_gv, \
            spectrum, \
            &(outColours[i]) \
            ); \
    return actualNumberOfValues; \
} \
\
- (unsigned int) getSpectrumValue \
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv \
        : (      ArSpectrum *) outSpectrum \
{ \
    (void) evalEnv; \
    \
    spc_s_init_s( \
        art_gv, \
        spectrum, \
        outSpectrum \
        ); \
    return 1; \
} \
\
- (unsigned int) getHiresSpectrumValue \
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv \
        : (      ArSpectrum500 *) outSpectrum \
{ \
    (void) evalEnv; \
    \
    s500_s_init_s( \
        art_gv, \
        internalHiresSpectrum, \
        outSpectrum \
        ); \
    return 1; \
} \
\
- (unsigned int) getAttenuationValues \
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv \
        : (const unsigned int) numberOfValues \
        : (      ArAttenuation *) outAttenuations \
{ \
    (void) evalEnv; \
    \
    unsigned int actualNumberOfValues = M_MIN(numberOfValues, ARPVALUES_MAX_VALUES); \
    for (unsigned int i = 0; i < actualNumberOfValues; i++) \
        arattenuation_s_init_a( \
              art_gv, \
              spectrum, \
            & (outAttenuations[i]) \
            ); \
    return actualNumberOfValues; \
} \
\
- (unsigned int) getAttenuationValue \
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv \
        : (      ArAttenuation *) outAttenuation \
{ \
    (void) evalEnv; \
    \
    arattenuation_s_init_a( \
        art_gv, \
        spectrum, \
        outAttenuation \
        ); \
    return 1; \
} \
\
- (unsigned int) getDepolarisingAttenuationValues \
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv \
        : (const ArReferenceFrame *) refframeEntry \
        : (const ArReferenceFrame *) refframeExit \
        : (const unsigned int) numberOfValues \
        : (      ArAttenuation *) outAttenuations \
{ \
    (void) evalEnv; \
    \
    const unsigned int actualNumberOfValues = M_MIN(numberOfValues, ARPVALUES_MAX_VALUES); \
    for (unsigned int i = 0; i < actualNumberOfValues; i++) \
        arattenuation_srr_init_depolarising_a( \
            art_gv, \
            spectrum, \
            refframeEntry, \
            refframeExit, \
            &(outAttenuations[i]) \
            ); \
    return actualNumberOfValues; \
} \
\
- (unsigned int) getDepolarisingAttenuationValue \
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv \
        : (const ArReferenceFrame *) refframeEntry \
        : (const ArReferenceFrame *) refframeExit \
        : (      ArAttenuation *) outAttenuation \
{ \
    (void) evalEnv; \
    \
    arattenuation_srr_init_depolarising_a( \
        art_gv, \
        spectrum, \
        refframeEntry, \
        refframeExit, \
        outAttenuation \
        ); \
    return 1; \
} \
\
- (unsigned int) getNonpolarisingAttenuationValues \
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv \
        : (const ArReferenceFrame *) refframeEntry \
        : (const ArReferenceFrame *) refframeExit \
        : (const unsigned int) numberOfValues \
        : (      ArAttenuation *) outAttenuations \
{ \
    (void) evalEnv; \
    \
    unsigned int actualNumberOfValues = M_MIN(numberOfValues, ARPVALUES_MAX_VALUES); \
    for (unsigned int i = 0; i < actualNumberOfValues; i++) \
        arattenuation_srr_init_nonpolarising_a( \
            art_gv, \
            spectrum, \
            refframeEntry, \
            refframeExit, \
            &(outAttenuations[i]) \
            ); \
    return actualNumberOfValues; \
} \
\
- (unsigned int) getNonpolarisingAttenuationValue \
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv \
        : (const ArReferenceFrame *) refframeEntry \
        : (const ArReferenceFrame *) refframeExit \
        : (      ArAttenuation *) outAttenuation \
{ \
    (void) evalEnv; \
\
    arattenuation_srr_init_nonpolarising_a( \
        art_gv, \
        spectrum, \
        refframeEntry, \
        refframeExit, \
        outAttenuation \
        ); \
    return 1; \
} \
\
- (BOOL) isFluorescent \
{ \
    return NO; \
} \
\
ARPVALUES_STANDARD_VALUETYPE_IMPLEMENTATION(arvalue_spectrum|arvalue_attenuation) \
ARPVALUES_NULLARY_EVALENVTYPE_IMPLEMENTATION(arevalenv_none)


#define ARNVAL_CONST_COLOUR_IMPL(_Type,_type,_classtype) \
\
@implementation ArnVal_const_##_classtype \
 \
ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnVal_const_##_classtype) \
\
- (void) _setup \
{ \
    spectrum = spc_alloc( art_gv ); \
\
    _type##_to_spc( \
          art_gv, \
        & nativeValue, \
          spectrum \
        ); \
\
    internalHiresSpectrum = s500_alloc( art_gv ); \
\
    _type##_to_s500( \
          art_gv, \
        & nativeValue, \
          internalHiresSpectrum \
        ); \
} \
\
ARNSPECTRUM_STANDARD_METHOD_IMPLEMENTATIONS \
\
- (id) init \
    : (Ar##_Type) newNativeValue \
{ \
    self = [ super init ]; \
 \
    if ( self ) \
    { \
        nativeValue = newNativeValue; \
        \
        [ self _setup ]; \
    } \
 \
    return self; \
} \
\
- (void) getNewPSSpectrum \
    : (ArcPointContext *) locationInfo \
    : (ArPSSpectrum *) outPSSpectrum \
{ \
    (void) locationInfo; \
\
    _type##_to_pss_new( art_gv, & nativeValue, outPSSpectrum ); \
} \
\
- (double) valueAtWavelength \
    : (ArcPointContext *) locationInfo \
    : (const double) wavelength \
{ \
    (void) locationInfo; \
\
    return s500_sd_value_at_wavelength( art_gv, internalHiresSpectrum, wavelength ); \
} \
\
- (void) code \
    : (ArcObject <ArpCoder> *) coder \
{ \
    [ super code: coder ]; \
 \
    [ coder codeAr##_Type : & nativeValue ]; \
 \
    if ( [ coder isReading ] ) [ self _setup ]; \
} \
\
@end

#define ARNSPECTRUM2D_STANDARD_METHOD_IMPLEMENTATIONS \
\
- (void) getReflectanceSpectralValue \
        : (const ArcPointContext *) locationInfo \
        : (const ArWavelength *) wavelength \
        : (      ArPathDirection) pathDirection \
        : (      ArSpectralSample *) reflectance \
{ \
    (void) locationInfo; \
    \
    ArCrosstalk500* crosstalkSums = (pathDirection == arpathdirection_from_eye ? hiresHorizontalSums : hiresVerticalSums); \
    int shift_in_x = (pathDirection == arpathdirection_from_eye ? 1 : 0), shift_in_y = 1 - shift_in_x; \
    \
    ArWavelength wavelengthA; \
    ArWavelength wavelengthB; \
    c4_dc_sub_c( /* wavelengthA = wavelength - shift_in_x NM */ \
          shift_in_x NM, \
        & ARWL_C(*wavelength), \
        & ARWL_C(wavelengthA) \
        ); \
    c4_dc_add_c( /* wavelengthB = wavelength + shift_in_y NM */ \
          shift_in_y NM, \
        & ARWL_C(*wavelength), \
        & ARWL_C(wavelengthB) \
        ); \
    \
    ArSpectralSample crosstalkSum; \
    cx500_wl_wl_init_s( \
          art_gv, \
          crosstalkSums, \
        & wavelengthA, \
        & wavelengthB, \
        & crosstalkSum \
        ); \
    ArSpectralSample mainReflectance; \
    sps_s500w_init_s( \
          art_gv, \
          hiresMainDiagonal, \
          wavelength, \
        & mainReflectance \
        ); \
    \
    sps_ss_add_s( \
        art_gv, \
        & mainReflectance, \
        & crosstalkSum, \
          reflectance \
        ); \
} \
\
\
- (BOOL) randomWavelengthShift \
        : (const ArcPointContext *) locationInfo \
        : (const ArWavelength *) inputWavelength \
        : (      id <ArpRandomGenerator>) randomGenerator \
        : (      ArPathDirection) pathDirection \
        : (      ArWavelength *) outputWavelength \
        : (      ArSpectralSample *) attenuation \
        : (      ArPDFValue *) probability \
{ \
    (void) locationInfo; \
    \
    ArCrosstalk500* crosstalkSums = (pathDirection == arpathdirection_from_eye ? hiresHorizontalSums : hiresVerticalSums); \
    int shift_in_x = (pathDirection == arpathdirection_from_eye ? 1 : 0), shift_in_y = 1 - shift_in_x; \
    \
    ArWavelength wavelengthA; \
    ArWavelength wavelengthB; \
    c4_dc_sub_c( /* wavelengthA = inputWavelength - shift_in_x NM */ \
          shift_in_x NM, \
        & ARWL_C(*inputWavelength), \
        & ARWL_C(wavelengthA) \
        ); \
    c4_dc_add_c( /* wavelengthB = inputWavelength + shift_in_y NM */ \
          shift_in_y NM, \
        & ARWL_C(*inputWavelength), \
        & ARWL_C(wavelengthB) \
        ); \
    \
    ArSpectralSample crosstalkSum; \
    cx500_wl_wl_init_s( \
          art_gv, \
          crosstalkSums, \
        & wavelengthA, \
        & wavelengthB, \
        & crosstalkSum \
        ); \
    ArSpectralSample mainReflectance; \
    sps_s500w_init_s( \
          art_gv, \
          hiresMainDiagonal, \
          inputWavelength, \
        & mainReflectance \
        ); \
    \
    ArSpectralSample totalReflectance; \
    sps_ss_add_s( /* totalReflectance = mainReflectance + crosstalkSum */ \
        art_gv, \
        & mainReflectance, \
        & crosstalkSum, \
        & totalReflectance \
        ); \
    if(SPS_CI(totalReflectance, 0) == 0.0) \
        return NO; \
    \
    arpdfvalue_d_init_p(1.0, probability); \
    \
    ArSpectralSample probabilities; double pdf = 1.0; \
    for( unsigned int i = 0; i < HERO_SAMPLES_TO_SPLAT; ++i ) \
    { \
        if(SPS_CI(totalReflectance, i) == 0.0) \
        { \
            ARWL_WI(*outputWavelength, i) = ARWL_WI(*inputWavelength, i); \
            SPS_CI(*attenuation, i) = 0.0; \
            SPS_CI(probabilities, i) = 1.0; \
            ARPDFVAL_PI(*probability, i) = 0.0; /* shortcircuit the probability */ \
            continue; \
        } \
        \
        double totalReflectanceInverse = 1 / SPS_CI(totalReflectance, i); \
        double mainProbability = SPS_CI(mainReflectance, i) * totalReflectanceInverse; \
        \
        double randomValue = [ randomGenerator valueFromNewSequence ]; \
        if(randomValue < mainProbability) \
        { \
            ARWL_WI(*outputWavelength, i) = ARWL_WI(*inputWavelength, i); \
            SPS_CI(*attenuation, i) = SPS_CI(mainReflectance, i); \
            SPS_CI(probabilities, i) = mainProbability; \
            pdf *= mainProbability; \
        } \
        else \
        { \
            double sumValue = [ randomGenerator valueFromNewSequence ] * SPS_CI(crosstalkSum, i); \
            \
            int cidx_i = round(NANO_FROM_UNIT(ARWL_WI(*inputWavelength, i)) - ARCROSSTALK500_LOWER_BOUND); \
            int from, to; \
            from = 0 + shift_in_y * (cidx_i + 1); \
            to = cidx_i * shift_in_x + shift_in_y * ARCROSSTALK500_SPECTRAL_CHANNELS - 1; \
    \
            while(from < to) \
            { \
                int center = (from + to) / 2; \
                double centerValue = \
                    CX500_XY( \
                        *crosstalkSums, \
                        center * shift_in_x + shift_in_y * cidx_i, \
                        cidx_i * shift_in_x + shift_in_y * center \
                    ); \
    \
                switch (pathDirection) { \
                    case arpathdirection_from_eye: \
                        if(sumValue > centerValue) { \
                            from = center + 1; /* shift right */ \
                        } else { \
                            to = center; \
                        } \
                    break; \
                    case arpathdirection_from_light: \
                    if(sumValue > centerValue) { \
                        to = center; /* shift left */ \
                    } else { \
                        from = center + 1; \
                    } \
                    default: \
                        break; \
                } \
            } \
    \
            if(from == to) \
            { \
                double outWL = (from + ARCROSSTALK500_LOWER_BOUND + [ randomGenerator valueFromNewSequence ]) NM; \
                ARWL_WI(*outputWavelength, i) = outWL; \
                SPS_CI(*attenuation, i) = \
                    cx500_dd_value( \
                        art_gv, \
                        hiresCrosstalk, \
                        ARWL_WI(*outputWavelength, i) * shift_in_x + shift_in_y * ARWL_WI(*inputWavelength, i), \
                        ARWL_WI(*inputWavelength, i) * shift_in_x + shift_in_y * ARWL_WI(*outputWavelength, i) \
                    ); \
                double shiftProbability = SPS_CI(*attenuation, i) * totalReflectanceInverse; \
                SPS_CI(probabilities, i) = shiftProbability; \
                pdf *= shiftProbability; \
            } \
            else /* this should never happend */ \
            { \
                ART_ERRORHANDLING_FATAL_ERROR( \
                    "Binary searching a reradiation matrix didn't produce a result although it was expected." \
                    ); \
            } \
        } \
    } \
    \
    /* attenuation *= ( pdf / probabilities = ( product(t != k) p_t )_k ) */ \
    sps_inv_s( \
          art_gv, \
        & probabilities \
        ); \
    sps_d_mul_s( \
          art_gv, \
          pdf, \
        & probabilities \
        ); \
    sps_s_mul_s( \
          art_gv, \
        & probabilities, \
          attenuation \
        ); \
    \
    arpdfvalue_d_mul_p( \
          pdf, \
          probability \
        ); \
    \
    return ( SPS_CI(*attenuation, 0) > 0.0 ); \
} \
 \
- (BOOL) attenuationForWavelengthShift \
        : (const ArcPointContext *) locationInfo \
        : (const ArWavelength *) inputWavelength \
        : (const ArWavelength *) outputWavelength \
        : (      ArPathDirection) pathDirection \
        : (      ArSpectralSample *) attenuation \
        : (      ArPDFValue *) probability \
{ \
    (void) locationInfo; \
    \
    ArCrosstalk500* crosstalkSums = (pathDirection == arpathdirection_from_eye ? hiresHorizontalSums : hiresVerticalSums); \
    int shift_in_x = (pathDirection == arpathdirection_from_eye ? 1 : 0), shift_in_y = 1 - shift_in_x; \
    \
    ArWavelength wavelengthA; \
    ArWavelength wavelengthB; \
    c4_dc_sub_c( /* wavelengthA = inputWavelength - shift_in_x NM */ \
          shift_in_x NM, \
        & ARWL_C(*inputWavelength), \
        & ARWL_C(wavelengthA) \
        ); \
    c4_dc_add_c( /* wavelengthB = inputWavelength + shift_in_y NM */ \
          shift_in_y NM, \
        & ARWL_C(*inputWavelength), \
        & ARWL_C(wavelengthB) \
        ); \
    \
    ArSpectralSample crosstalkSum; \
    cx500_wl_wl_init_s( \
          art_gv, \
          crosstalkSums, \
        & wavelengthA, \
        & wavelengthB, \
        & crosstalkSum \
        ); \
    ArSpectralSample mainReflectance; \
    sps_s500w_init_s( \
          art_gv, \
          hiresMainDiagonal, \
          inputWavelength, \
        & mainReflectance \
        ); \
    \
    ArSpectralSample totalReflectance; \
    sps_ss_add_s( /* totalReflectance = mainReflectance + crosstalkSum */ \
        art_gv, \
        & mainReflectance, \
        & crosstalkSum, \
        & totalReflectance \
        ); \
    if(SPS_CI(totalReflectance, 0) == 0.0) \
        return NO; \
    \
    if(probability) \
        arpdfvalue_d_init_p(1.0, probability); \
    \
    ArSpectralSample probabilities; double pdf = 1.0; \
    for( unsigned int i = 0; i < HERO_SAMPLES_TO_SPLAT; ++i ) \
    { \
        if(SPS_CI(totalReflectance, i) == 0.0) \
        { \
            SPS_CI(*attenuation, i) = 0.0; \
            SPS_CI(probabilities, i) = 1.0; \
            if(probability) \
                ARPDFVAL_PI(*probability, i) = 0.0; /* shortcircuit the probability */ \
            continue; \
        } \
        \
        double totalReflectanceInverse = 1 / SPS_CI(totalReflectance, i); \
        \
        if( ARWL_WI(*inputWavelength,i) ==  ARWL_WI(*outputWavelength,i)) \
        { \
            /* main diagonal */ \
            SPS_CI(*attenuation,i) = SPS_CI(mainReflectance, i); \
            double mainProbability = SPS_CI(mainReflectance, i) * totalReflectanceInverse; \
            SPS_CI(probabilities, i) = mainProbability; \
            pdf *= mainProbability; \
        } \
        else \
        { \
            /* crosstalk */ \
            SPS_CI(*attenuation, i) = \
                cx500_dd_value( \
                    art_gv, \
                    hiresCrosstalk, \
                    ARWL_WI(*outputWavelength, i) * shift_in_x + shift_in_y * ARWL_WI(*inputWavelength, i), \
                    ARWL_WI(*inputWavelength, i) * shift_in_x + shift_in_y * ARWL_WI(*outputWavelength, i) \
                ); \
            double shiftProbability = SPS_CI(*attenuation, i) * totalReflectanceInverse; \
            SPS_CI(probabilities, i) = shiftProbability; \
            pdf *= shiftProbability; \
        } \
    } \
    \
    /* attenuation *= ( pdf / probabilities = ( product(t != k) p_t )_k ) */ \
    sps_inv_s( \
          art_gv, \
        & probabilities \
        ); \
    sps_d_mul_s( \
          art_gv, \
          pdf, \
        & probabilities \
        ); \
    sps_s_mul_s( \
          art_gv, \
        & probabilities, \
          attenuation \
        ); \
    \
    if(probability) \
        arpdfvalue_d_mul_p( \
              pdf, \
              probability \
            ); \
    \
    return ( SPS_CI(*attenuation, 0) > 0.0 ); \
} \
\
- (BOOL) isFluorescent \
{ \
    return YES; \
} \

// ===========================================================================
