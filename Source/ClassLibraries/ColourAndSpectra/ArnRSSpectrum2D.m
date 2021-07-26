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

#define ART_MODULE_NAME     ArnRSSpectrum2D

#import "ArnRSSpectrum2D.h"
#import "ArnColourStandardImplementation.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnRSSpectrum2D registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArnRSSpectrum2D

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnRSSpectrum2D)

- (void) _setup
{
    ArRSSpectrum  rss_temp;

    rss2d_to_rss( nativeValue, & rss_temp );

//    rss_c_mathematicaprintf( art_gv, & rss_temp );

    if ( ! mainDiagonal )
        mainDiagonal = spc_alloc( art_gv );
    if ( ! hiresMainDiagonal )
        hiresMainDiagonal = s500_alloc(art_gv);

    rss_to_spc( art_gv, & rss_temp, mainDiagonal );
    rss_to_s500( art_gv, & rss_temp, hiresMainDiagonal );

//    spc_c_mathematicaprintf( art_gv, mainDiagonal );

    rss_free_contents( art_gv, & rss_temp );

    rss2d_strip_noncrosstalk_data( art_gv, nativeValue );

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
    
//    arcrosstalk_x_mathematicaprintf( art_gv, crosstalk );
}

- (id) init
        : (ArRSSpectrum2D) newSpectrum
{
    self = [ super init ];

    if ( self )
    {
        nativeValue = ALLOC(ArRSSpectrum2D);

        *nativeValue = newSpectrum;

        [ self _setup ];
    }
    
    return self;
}

- (void) dealloc
{
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

- (void) getSpectrum
        : (ArcPointContext *) locationInfo
        : (ArSpectrum *) outSpectrum
{
    (void) locationInfo;
    
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
    (void) locationInfo;
    
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
    (void) locationInfo;
    (void) wavelength;
    (void) outSpectrum;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

- (void) getLightIntensity
        : (ArcPointContext *) locationInfo
        : (ArLightIntensity *) outLightIntensity
{
    (void) locationInfo;
    
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
    (void) locationInfo;
    
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
    (void) locationInfo;
    
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
    (void) locationInfo;
    (void) wavelength;
    (void) outAttenuation;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

- (void) getDepolarisingAttenuation
        : (ArcPointContext *) locationInfo
        : (ArReferenceFrame *) refframeEntry
        : (ArReferenceFrame *) refframeExit
        : (ArAttenuation *) attenuation_r
{
    (void) locationInfo;
    
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
    (void) locationInfo;
    (void) refframeEntry;
    (void) refframeExit;
    (void) wavelength;
    (void) outAttenuation;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

- (void) getNonpolarisingAttenuation
        : (ArcPointContext *) locationInfo
        : (ArReferenceFrame *) refframeEntry
        : (ArReferenceFrame *) refframeExit
        : (ArAttenuation *) attenuation_r
{
    (void) locationInfo;
    
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
    (void) locationInfo;
    (void) refframeEntry;
    (void) refframeExit;
    (void) wavelength;
    (void) outAttenuation;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

- (unsigned long) getSpectrumValues
        : (ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (unsigned long) numberOfValues
        : (ArSpectrum *) outSpectrum
{
    (void) evalEnv;
    
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
    (void) evalEnv;
    
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
    (void) evalEnv;
    
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
    (void) evalEnv;
    
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
    (void) evalEnv;
    
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
    (void) evalEnv;
    
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
    (void) evalEnv;
    
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
    (void) evalEnv;
    
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
    (void) evalEnv;
    
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

ARNSPECTRUM2D_STANDARD_METHOD_IMPLEMENTATIONS



ARPVALUES_STANDARD_VALUETYPE_IMPLEMENTATION(
    arvalue_spectrum | arvalue_attenuation
    )

ARPVALUES_NULLARY_EVALENVTYPE_IMPLEMENTATION(arevalenv_none)

- (void) getNewPSSpectrum
        : (ArcPointContext *) locationInfo
        : (ArPSSpectrum *) outPSSpectrum
{
    (void) locationInfo;
    
    rss2d_to_pss_new( art_gv, nativeValue, outPSSpectrum );
}

- (double) valueAtWavelength
        : (ArcPointContext *) locationInfo
        : (const double) wavelength
{
    (void) locationInfo;
    (void) wavelength;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return 0.0;
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];

    if ( [ coder isReading ] )
    {
        if ( ! nativeValue )
            nativeValue = ALLOC(ArRSSpectrum2D);
    }

    arpcoder_arrsspectrum2d( art_gv, coder, nativeValue );

    if ( [ coder isReading ] )
        [ self _setup ];
}

@end


ArnRSSpectrum2D * arnconstrsspectrum2Dvalue(
              ART_GV  * art_gv,
        const double    excitation_start,
        const double    excitation_step,
        const double    emission_start,
        const double    emission_step,
        const double    unit,
        ...
        )
{
    ArnRSSpectrum2D           * s;
    va_list                     argPtr;

    va_start( argPtr, unit );

    ArRSSpectrum2D rss2dSpectrum =
        rss2d_v( excitation_start, excitation_step, emission_start, emission_step, unit, argPtr );

    s =
        [ ALLOC_INIT_OBJECT(ArnRSSpectrum2D)
            :   rss2dSpectrum
            ];

    //   In this case, we do not free the created rss spectrum contents,
    //   as the allocated space is taken over (and later freed) by the
    //   ArnRSSpectrum2D node.

    va_end( argPtr );

    return s;
}

// ===========================================================================
