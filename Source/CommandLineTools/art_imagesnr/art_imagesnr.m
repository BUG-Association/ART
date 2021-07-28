/* ===========================================================================

    Copyright (c) 1996-2021 The ART Development Team
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

int art_imagesnr(
        int        argc,
        char    ** argv,
        ART_GV   * art_gv
        )
{
    ART_APPLICATION_DEFINE_STANDARD_OPTIONS_WITH_FEATURES(
        "ART image snr",
        art_appfeatures_no_threading
        );

    ART_APPLICATION_MAIN_OPTIONS_FOLLOW

    id compareImageOpt =
        [ STRING_OPTION
             :   "compare"
             :   "c"
             :   "<ART image>"
             :   "Set the image to compare with"
             ];
    
    id outputSNROpt =
        [ STRING_OPTION
             :   "output"
             :   "o"
             :   "<text file>"
             :   "Set a text file to write the SNR into"
         ];
    
    ART_SINGLE_INPUT_FILE_APPLICATION_STARTUP(
        "art_imagesnr",
        "ART raw image SNR utility",
        "art_imagesnr <ART image reference> -c <ART image>"
        );

    if ( ! ( [ compareImageOpt hasBeenSpecified ] ) ) {
        ART_ERRORHANDLING_FATAL_ERROR("missing (compare) image for computing SNR");
    }

    // Read the reference image
    const char  * inputFileNameReference = argv[1];

    [ ART_GLOBAL_REPORTER beginTimedAction
         :   "reading ART input image %s"
         ,   inputFileNameReference
         ];
    
    ArnFileImage  * inputFileImageReference =
        [ FILE_IMAGE
            :   inputFileNameReference
            ];
    
    if ( ! (   ( [ inputFileImageReference imageFileIsKindOf: [ ArfRAWRasterImage class ] ] )
            || ( [ inputFileImageReference dataImageClass ] == [ ArfARTCSP class ] )) ) {
        ART_ERRORHANDLING_FATAL_ERROR(
            "reference file is not an internal ART image file - "
            "%s instead of ArfARTRAW/OpenEXR/CSP"
            ,   [ [ inputFileImageReference dataImageClass ] cStringClassName ]
            );
    }
    
    IVec2D sizeReference = [ inputFileImageReference size ];
    
    [ ART_GLOBAL_REPORTER endAction ];
    
    // Read the image to compare to
    const char  * inputFileNameCompare   = [ compareImageOpt cStringValue ];
    
    [ ART_GLOBAL_REPORTER beginTimedAction
         :   "reading ART input image %s"
         ,   inputFileNameCompare
         ];
    
    ArnFileImage  * inputFileImageCompare =
        [ FILE_IMAGE
             :   inputFileNameCompare
             ];
    
    if ( ! (   ( [ inputFileImageCompare imageFileIsKindOf: [ ArfRAWRasterImage class ] ] )
            || ( [ inputFileImageCompare dataImageClass ] == [ ArfARTCSP class ] )) ) {
        ART_ERRORHANDLING_FATAL_ERROR(
            "comparison file is not an internal ART image file - "
            "%s instead of ArfARTRAW/OpenEXR/CSP"
            ,   [ [ inputFileImageCompare dataImageClass ] cStringClassName ]
            );
    }
    
    IVec2D sizeCompare = [ inputFileImageCompare size ];
    
    [ ART_GLOBAL_REPORTER endAction ];
    
    // Check if the image size match
    if ( ! ( XC(sizeReference) == XC(sizeCompare) && YC(sizeReference) == YC(sizeCompare))) {
        ART_ERRORHANDLING_FATAL_ERROR(
            "comparison and reference file size mismatch:\n"
            "\treference  is %dx%d\n"
            "\tcomparison is %dx%d"
            , XC(sizeReference), YC(sizeReference)
            , XC(sizeCompare)  , YC(sizeCompare)
            );
    }
    
    if ([ inputFileImageReference imageFileIsKindOf: [ ArfRAWRasterImage class ] ]  ||
        [ inputFileImageCompare   imageFileIsKindOf: [ ArfRAWRasterImage class ] ]  ) {
        ART_ERRORHANDLING_FATAL_ERROR("this tool currently works only on ARTRAW images");
    }
    
    ArfRAWRasterImage  * rawImageReference = (ArfRAWRasterImage *) inputFileImageReference->imageFile;
    ArfRAWRasterImage  * rawImageCompare   = (ArfRAWRasterImage *) inputFileImageCompare->imageFile;

    ArDataType  rawContentTypeReference = [ rawImageReference fileDataType ];
    ArDataType  rawContentTypeCompare   = [ rawImageCompare   fileDataType ];
    
    if ( rawContentTypeReference != rawContentTypeCompare ) {
        ART_ERRORHANDLING_FATAL_ERROR("comparison and reference images have different spectrums");
    }

    //   Check if the current ISR is already set to match the contents
    //   of the RAW file
    
    if ( rawContentTypeReference != art_isr( art_gv ) )
    {
        //   If they do not match...
        
        [ ART_GLOBAL_REPORTER beginAction
         :   "automatically switching ISR to match ARTRAW contents"
         ];
        
        char  * newInputFileNameReference;
        char  * newInputFileNameCompare;

        arstring_s_copy_s(
            [ inputFileImageReference fileName ],
            & newInputFileNameReference
            );
        
        arstring_s_copy_s(
            [ inputFileImageCompare fileName ],
            & newInputFileNameCompare
            );
        
        [ ART_GLOBAL_REPORTER printf
         :   "Default ISR was : %s\n"
         ,   ardatatype_name( art_isr( art_gv ) )
         ];
        
        [ ART_GLOBAL_REPORTER printf
         :   "ARTRAW content is : %s\n"
         ,   ardatatype_name( rawContentTypeReference )
         ];
        
        [ ART_GLOBAL_REPORTER printf
             :   "ISR will be set as: %s\n"
             ,   ardatatype_name( rawContentTypeReference )
             ];
        
        art_set_isr( art_gv, rawContentTypeReference );
        
        [ ART_GLOBAL_REPORTER printf
             :   "ISR is now set to : %s\n"
             ,   ardatatype_name( art_isr( art_gv ) )
             ];
        
        [ ART_GLOBAL_REPORTER printf
             :   "Re-reading raw images...\n"
             ];
        
        inputFileImageReference =
            [ FILE_IMAGE
                 :   newInputFileNameReference
                 ];
        
        inputFileImageCompare =
            [ FILE_IMAGE
                 :   newInputFileNameCompare
                 ];
        
        FREE_ARRAY( newInputFileNameReference );
        FREE_ARRAY( newInputFileNameCompare );

        [ ART_GLOBAL_REPORTER printf
             :   "Done.\n"
             ];
        
        [ ART_GLOBAL_REPORTER endAction ];
    }
    
    ArnPlainImage  * lightAlphaImageReference =
        [ ALLOC_OBJECT(ArnLightAlphaImage)
             initWithSize
             :   sizeReference
             ];
    
    [ inputFileImageReference getPlainImage
        :   IPNT2D( 0, 0 )
        :   lightAlphaImageReference
        ];
    
    ArnPlainImage  * lightAlphaImageCompare =
        [ ALLOC_OBJECT(ArnLightAlphaImage)
            initWithSize
            :   sizeCompare
            ];
    
    [ inputFileImageCompare getPlainImage
         :   IPNT2D( 0, 0 )
         :   lightAlphaImageCompare
         ];
    
    
    [ ART_GLOBAL_REPORTER beginTimedAction
         :   "Computing SNR"
         ];
    
    ArnLightAlphaImage  * lightAlphaLineReference =
        [ ALLOC_OBJECT(ArnLightAlphaImage)
             initWithSize
             :   IVEC2D(XC(sizeReference),1)
             ];

    ArnLightAlphaImage  * lightAlphaLineCompare =
        [ ALLOC_OBJECT(ArnLightAlphaImage)
             initWithSize
             :   IVEC2D(XC(sizeCompare),1)
             ];

    ArSpectrum  * spectrumReference    = spc_alloc(art_gv);
    ArSpectrum  * spectrumCompare      = spc_alloc(art_gv);
    ArSpectrum  * spectrumReferenceSqr = spc_alloc(art_gv);
    ArSpectrum  * spectrumDiff         = spc_alloc(art_gv);
    ArSpectrum  * spectrumDiffSqr      = spc_alloc(art_gv);

    double sumRefSquared = 0;
    double sumDiffSquared = 0;
    
    double sumRefSquaredRGB = 0;
    double sumDiffSquaredRGB = 0;
    
    for (int y = 0; y < YC(sizeReference); y++)
    {
        [ lightAlphaImageReference getPlainImage
             :   IPNT2D( 0, y )
             :   lightAlphaLineReference
             ];
        
        [ lightAlphaImageCompare getPlainImage
             :   IPNT2D( 0, y )
             :   lightAlphaLineCompare
             ];

        for (int x = 0; x < XC(sizeReference); x++)
        {
            ArLightAlpha  * valueReference = lightAlphaLineReference->data[ x ];
            ArLightAlpha  * valueCompare   = lightAlphaLineCompare->data[ x ];

            arlightalpha_to_spc(art_gv, valueReference, spectrumReference);
            arlightalpha_to_spc(art_gv, valueCompare,   spectrumCompare);

            spc_ds_pow_s(art_gv, 2.0, spectrumReference, spectrumReferenceSqr);
            spc_ss_sub_s(art_gv, spectrumReference, spectrumCompare, spectrumDiff);
            spc_ds_pow_s(art_gv, 2.0, spectrumDiff, spectrumDiffSqr);
            
            sumRefSquared  += spc_s_l1_norm(art_gv, spectrumReferenceSqr);
            sumDiffSquared += spc_s_l1_norm(art_gv, spectrumDiffSqr);

            ArRGB  referenceRGB;
            ArRGB  compareRGB;
            
            spc_to_rgb( art_gv, spectrumReference, & referenceRGB );
            spc_to_rgb( art_gv, spectrumCompare, & compareRGB );
            
            referenceRGB = ARRGB( 0, ARRGB_G(referenceRGB) , 0 );
            compareRGB = ARRGB( 0, ARRGB_G(compareRGB) , 0 );

            ArRGB  referenceRGBSqr;
            ArRGB  diffRGB;
            ArRGB  diffRGBSqr;

            rgb_ds_pow_s(art_gv, 2.0, & referenceRGB, & referenceRGBSqr);
            rgb_ss_sub_s(art_gv, & referenceRGB, & compareRGB, & diffRGB);
            rgb_ds_pow_s(art_gv, 2.0, & diffRGB, & diffRGBSqr);
            
            sumRefSquaredRGB  += rgb_s_l1_norm(art_gv, & referenceRGBSqr);
            sumDiffSquaredRGB += rgb_s_l1_norm(art_gv, & diffRGBSqr);
        }
    }

    spc_free(art_gv, spectrumReference);
    spc_free(art_gv, spectrumCompare);
    spc_free(art_gv, spectrumReferenceSqr);
    spc_free(art_gv, spectrumDiff);
    spc_free(art_gv, spectrumDiffSqr);

    [ ART_GLOBAL_REPORTER endAction ];
    
    double snr = 10.0 * log10(sumRefSquared/sumDiffSquared);
    double snrRGB = 10.0 * log10(sumRefSquaredRGB/sumDiffSquaredRGB);
    
    [ ART_GLOBAL_REPORTER consolePrintf
         :   "Spectral SNR: %f dB\n"
         ,   snr
         ];
    
    [ ART_GLOBAL_REPORTER consolePrintf
         :   "Colour   SNR: %f dB\n"
         ,   snrRGB
         ];

    if ( [ outputSNROpt hasBeenSpecified ] ) {
        FILE * outputFile = fopen( [ outputSNROpt cStringValue ], "w");
        
        if (outputFile) {
            fprintf(outputFile, "%f %f", snr, snrRGB);
            fclose(outputFile);
        } else {
            ART_ERRORHANDLING_WARNING(
                "Could not write to the specified file %s",
                [ outputSNROpt cStringValue ]
                );
        }
    }
    
    return 0;
}

ADVANCED_RENDERING_TOOLKIT_MAIN(art_imagesnr)

// ===========================================================================
