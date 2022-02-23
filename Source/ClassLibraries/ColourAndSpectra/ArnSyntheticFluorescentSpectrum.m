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

#define ART_MODULE_NAME     ArnSyntheticFluorescentSpectrum

#import "ArnSyntheticFluorescentSpectrum.h"
#import "ArnColourStandardImplementation.h"
#import "ColourAndLightSubsystem.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnSyntheticFluorescentSpectrum registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#define COLOUR_SUBNODE ((ArNode <ArpSpectrum> *)ARNUNARY_SUBNODE)

@implementation ArnSyntheticFluorescentSpectrum

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnSyntheticFluorescentSpectrum)

- (void) _setupSFC
{
    if ( ! mainDiagonalColour )
        mainDiagonalColour = spc_alloc( art_gv );

    [ COLOUR_SUBNODE getSpectrum
        :   0
        :   mainDiagonalColour
        ];
    
    if ( ! hiresMainDiagonal )
        hiresMainDiagonal = s500_alloc(art_gv);
    int s500_channelCount = s500_channels(art_gv);
    for( int i = 0; i < s500_channelCount; ++i )
    {
        ArWavelength wavelength;
        
        arwavelength_d_init_w(art_gv,s500_channel_center(art_gv, i),&wavelength);
        ArSpectralSample spectralSample;
        [ COLOUR_SUBNODE getSpectralSample
            : 0
            : & wavelength
            : & spectralSample
            ];
    
        s500_set_sid(
            art_gv,
            hiresMainDiagonal,
            i,
            SPS_CI(spectralSample, 0)
        );
    }
    
    if ( ! hiresCrosstalk )
        hiresCrosstalk = cx500_alloc(art_gv);

    cx500_dpv_init_x(
          art_gv,
          crosstalkMaximum,
        & crosstalkCenter,
        & crosstalkExtent,
          hiresCrosstalk
        );
    
    if ( ! crosstalk )
        crosstalk = arcrosstalk_alloc( art_gv );

    cx500_to_crosstalk(
          art_gv,
          hiresCrosstalk,
          crosstalk
        );
    
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
        : (ArNodeRef) newMainDiagonal
        : (double) newCrosstalkMaximum
        : (Pnt2D) newCrosstalkCenter
        : (Vec2D) newCrosstalkExtent
{
    self = [ super init: newMainDiagonal ];
    
    if ( self )
    {
        crosstalkMaximum = newCrosstalkMaximum;
        crosstalkCenter  = newCrosstalkCenter;
        crosstalkExtent  = newCrosstalkExtent;

        [ self _setupSFC ];
    }
    
    return self;
}

- (void) dealloc
{
    if(mainDiagonalColour)
        spc_free(art_gv, mainDiagonalColour);
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
          mainDiagonalColour,
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
          mainDiagonalColour,
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
          mainDiagonalColour,
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
          mainDiagonalColour,
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
          mainDiagonalColour,
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
          mainDiagonalColour,
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

- (void) getNewPSSpectrum
        : (ArcPointContext *) locationInfo
        : (ArPSSpectrum *) outPSSpectrum
{
    (void) locationInfo;
    (void) outPSSpectrum;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
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

- (unsigned int) getSpectrumValues
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (const unsigned int) numberOfValues
        : (ArSpectrum *) outSpectrum
{
    (void) evalEnv;
    
    unsigned int actualNumberOfValues = M_MIN(numberOfValues, ARPVALUES_MAX_VALUES);

    for ( unsigned int i = 0; i < actualNumberOfValues; i++)
        spc_s_init_s(
              art_gv,
              mainDiagonalColour,
            &(outSpectrum[i])
            );

    return actualNumberOfValues;
}

- (unsigned int) getSpectrumValue
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (ArSpectrum *) outSpectrum
{
    (void) evalEnv;
    
    spc_s_init_s(
          art_gv,
          mainDiagonalColour,
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

- (unsigned int) getAttenuationValues
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (const unsigned int) numberOfValues
        : (      ArAttenuation *) outAttenuations
{
    (void) evalEnv;
    
    unsigned int actualNumberOfValues = M_MIN(numberOfValues, ARPVALUES_MAX_VALUES);

    for ( unsigned int i = 0; i < actualNumberOfValues; i++)
        arattenuation_sx_init_a(
              art_gv,
              mainDiagonalColour,
              crosstalk,
            &(outAttenuations[i])
            );

    return actualNumberOfValues;
}

- (unsigned int) getAttenuationValue
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (      ArAttenuation *) outAttenuation
{
    (void) evalEnv;
    
    arattenuation_sx_init_a(
          art_gv,
          mainDiagonalColour,
          crosstalk,
          outAttenuation
        );

    return 1;
}

- (unsigned int) getDepolarisingAttenuationValues
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (const ArReferenceFrame *) refframeEntry
        : (const ArReferenceFrame *) refframeExit
        : (const unsigned int) numberOfValues
        : (      ArAttenuation *) outAttenuations
{
    (void) evalEnv;
    
    unsigned int actualNumberOfValues = M_MIN(numberOfValues, ARPVALUES_MAX_VALUES);

    for ( unsigned int i = 0; i < actualNumberOfValues; i++)
        arattenuation_sxrr_init_depolarising_a(
              art_gv,
              mainDiagonalColour,
              crosstalk,
              refframeEntry,
              refframeExit,
            &(outAttenuations[i])
            );

    return actualNumberOfValues;
}

- (unsigned int) getDepolarisingAttenuationValue
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (const ArReferenceFrame *) refframeEntry
        : (const ArReferenceFrame *) refframeExit
        : (      ArAttenuation *) outAttenuation
{
    (void) evalEnv;
    
    arattenuation_sxrr_init_depolarising_a(
          art_gv,
          mainDiagonalColour,
          crosstalk,
          refframeEntry,
          refframeExit,
          outAttenuation
        );

    return 1;
}

- (unsigned int) getNonpolarisingAttenuationValues
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (const ArReferenceFrame *) refframeEntry
        : (const ArReferenceFrame *) refframeExit
        : (const unsigned int) numberOfValues
        : (      ArAttenuation *) outAttenuations
{
    (void) evalEnv;
    
    unsigned int actualNumberOfValues = M_MIN(numberOfValues, ARPVALUES_MAX_VALUES);

    for ( unsigned int i = 0; i < actualNumberOfValues; i++)
        arattenuation_sxrr_init_nonpolarising_a(
              art_gv,
              mainDiagonalColour,
              crosstalk,
              refframeEntry,
              refframeExit,
            &(outAttenuations[i])
            );

    return actualNumberOfValues;
}

- (unsigned int) getNonpolarisingAttenuationValue
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (const ArReferenceFrame *) refframeEntry
        : (const ArReferenceFrame *) refframeExit
        : (      ArAttenuation *) outAttenuation
{
    (void) evalEnv;
    
    arattenuation_sxrr_init_nonpolarising_a(
          art_gv,
          mainDiagonalColour,
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

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];

    [ coder codeDouble: & crosstalkMaximum ];
    [ coder codePnt2D: & crosstalkCenter ];
    [ coder codeVec2D: & crosstalkExtent ];

    if ( [ coder isReading ] )
        [ self _setupSFC ];
}

@end


// ===========================================================================
