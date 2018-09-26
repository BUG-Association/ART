/* ===========================================================================
 
 Copyright (c) 1996-2018 The ART Development Team
 -------------------------------------------
 
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

#import "AdvancedRenderingToolkit.h"
#define POST_IT

int export_spectrum(
          int        argc,
          char    ** argv,
          ART_GV   * art_gv
          )
{
    ART_APPLICATION_DEFINE_STANDARD_OPTIONS_WITH_FEATURES(
          "Spectrum test program for debuggin ART",
          art_appfeatures_none_beyond_baseline | art_appfeatures_no_threading
        );
    
    ART_APPLICATION_MAIN_OPTIONS_FOLLOW
    
    ART_SINGLE_INPUT_FILE_APPLICATION_STARTUP(
          "export_spectrum",
          "A programm to export Spectrum implementation",
          "export_spectrum\n\n"
        );
    
    const double absorbMain = 500 NM;
    const double reemitMain = 600 NM;
    const int nSamples = 1024;
    
    const int outSpectrumStart    = 300; // nanometers
    const int outSpectrumEnd      = 800; // nanometers
    const int outSpectrumSampling = 10;  // nanometers
    const int outSpectrumSize = (outSpectrumEnd - outSpectrumStart) / outSpectrumSampling;

    ArSpectralSample sample;
    
    FILE * f_light = NULL;
    FILE * f_eye = NULL;
    
    ArcObject <ArpRandomGenerator> *  randomGenerator = ARCRANDOMGENERATOR_NEW(arrandomvaluegeneration_random,
                                                                               30,
                                                                               art_global_reporter(art_gv)
                                                                               );
    
    id spectrum_test =
#ifdef POST_IT
    EXTERNAL_NAMED_COLOUR("SpectralData/FluorescentPostIt.ark", "fluorescent_pink");
#elif defined CONST_FLUO
    CONST_FLUORESCENT_COLOUR(CONST_COLOUR_GRAY(0.0), // black
                             0.025,
                             PNT2D(absorbMain, reemitMain),
                             VEC2D(  40 NM,  40 NM )
                             );
#else
    CONST_COLOUR_GRAY(0.8);
#endif
    
    /*******************************************************
     * Reflectance testing
     *******************************************************/
    f_eye   = fopen("art_spectrum_from_eye.txt", "w");
    f_light = fopen("art_spectrum_from_light.txt", "w");
    
    for (double wavelength = 380 NM; wavelength < 800.0 NM; wavelength += 1.0 NM) {
        ArWavelength wl; wl.c = CRD4(wavelength, wavelength, wavelength, wavelength);
        
        [ spectrum_test getReflectanceSpectralValue
             :   0
             : & wl
             :   arpathdirection_from_eye
             : & sample
             ];
        
        fprintf(f_eye, "%f %f\n", wavelength * 1e9, SPS_CI(sample, 0));
        
        [ spectrum_test getReflectanceSpectralValue
             :   0
             : & wl
             :   arpathdirection_from_light
             : & sample
             ];
        
        fprintf(f_light, "%f %f\n", wavelength * 1e9, SPS_CI(sample, 0));
    }
    
    fclose(f_eye);
    fclose(f_light);
    
    /*******************************************************
     * Wavelength shift testing
     *******************************************************/
    int *samplesEye   = calloc(outSpectrumSize, sizeof(int)); memset(samplesEye,   0, sizeof(int) * outSpectrumSize);
    int *samplesLight = calloc(outSpectrumSize, sizeof(int)); memset(samplesLight, 0, sizeof(int) * outSpectrumSize);

    ArWavelength incomingWavelengthEye, incomingWavelengthLight;
    ArSpectralSample dummyAttenuation;
    
    arwavelength_d_init_w(art_gv, reemitMain , &incomingWavelengthEye);
    arwavelength_d_init_w(art_gv, absorbMain , &incomingWavelengthLight);
    
    for (int i = 0; i < nSamples; i++) {
        ArPDFValue shiftProbability;
        ArWavelength sampledWavelength;
        
        [ spectrum_test randomWavelengthShift
             :   NULL
             : & incomingWavelengthEye
             :   randomGenerator
             :   arpathdirection_from_eye
             : & sampledWavelength
             : & dummyAttenuation
             : & shiftProbability
             ];
        
        int wl_idx = (ARWL_WI(sampledWavelength, 0) * 1e9 - outSpectrumStart)/ outSpectrumSampling;
        if (wl_idx > -1 && wl_idx < outSpectrumSize) {
            samplesEye[wl_idx] += 1;
        } else {
            printf("\t%d, %f\n",wl_idx, ARWL_WI(sampledWavelength, 0) * 1e9);
        }
        
        [ spectrum_test randomWavelengthShift
             :   NULL
             : & incomingWavelengthLight
             :   randomGenerator
             :   arpathdirection_from_light
             : & sampledWavelength
             : & dummyAttenuation
             : & shiftProbability
             ];
        
        wl_idx = (ARWL_WI(sampledWavelength, 0) * 1e9 - outSpectrumStart) / outSpectrumSampling;
        if (wl_idx > -1 && wl_idx < outSpectrumSize) {
            samplesLight[wl_idx] += 1;
        } else {
            printf("\t%d, %f\n", wl_idx, ARWL_WI(sampledWavelength, 0) );
        }
    }
    
    f_eye   = fopen("art_wl_shift_eye.txt", "w");
    f_light = fopen("art_wl_shift_light.txt", "w");
    
    for (int wavelength = outSpectrumStart ; wavelength < outSpectrumEnd; wavelength += outSpectrumSampling) {
        //if (wavelength == (int)(reemitMain*1e9)) continue;
        int wl_idx = (wavelength - outSpectrumStart)/ outSpectrumSampling;
        
        fprintf(f_eye,   "%d %f\n", wavelength, (double)samplesEye[wl_idx] / (double)nSamples);
        fprintf(f_light, "%d %f\n", wavelength, (double)samplesLight[wl_idx] / (double)nSamples);
    }
    
    fclose(f_eye);
    fclose(f_light);
    
    free(samplesLight);
    free(samplesEye);
    
    /*******************************************************
     * Attenuation testing
     *******************************************************/
    f_eye   = fopen("art_attenuation_from_eye.txt", "w");
    f_light = fopen("art_attenuation_from_light.txt", "w");
    
    for (double wavelength = 380 NM; wavelength < 800.0 NM; wavelength += 1.0 NM) {
        ArWavelength wl; wl.c = CRD4(wavelength, wavelength, wavelength, wavelength);
        ArPDFValue pdf;
        
        [ spectrum_test attenuationForWavelengthShift
             :   NULL
             : & incomingWavelengthEye
             : & wl
             :   arpathdirection_from_eye
             : & sample
             : & pdf
             ];
        
        fprintf(f_eye, "%f %f\n", wavelength * 1e9, SPS_CI(sample, 0));
        
        [ spectrum_test attenuationForWavelengthShift
             :   NULL
             : & incomingWavelengthLight
             : & wl
             :   arpathdirection_from_light
             : & sample
             : & pdf
             ];
        
        fprintf(f_light, "%f %f\n", wavelength * 1e9, SPS_CI(sample, 0));
    }
    
    fclose(f_eye);
    fclose(f_light);
    

    return 0;
}

ADVANCED_RENDERING_TOOLKIT_MAIN(export_spectrum)





// ===========================================================================
