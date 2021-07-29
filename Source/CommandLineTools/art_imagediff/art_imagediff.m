
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

int art_imagediff(
        int        argc,
        char    ** argv,
        ART_GV   * art_gv
        )
{
    ART_APPLICATION_DEFINE_STANDARD_OPTIONS_WITH_FEATURES(
        "ART image diff",
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
             :   "Set a text file to write the diff value into"
         ];
    
    ART_SINGLE_INPUT_FILE_APPLICATION_STARTUP(
        "art_imagediff",
        "ART image difference utility",
        "art_imagediff <ART image reference> -c <ART image>"
        );

    if ( ! ( [ compareImageOpt hasBeenSpecified ] ) ) {
        ART_ERRORHANDLING_FATAL_ERROR("missing (compare) image for computing diff");
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
    
    // TODO
    if ( ! (   ( [ inputFileImageReference dataImageClass ] == [ ArfARTCSP class ] )) ) {
        ART_ERRORHANDLING_FATAL_ERROR(
            "currently, supporting only ARTCSP - "
            "%s instead of ArfARTRAW/OpenEXR"
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
    
    /*
    if ([ inputFileImageReference dataImageClass ] != [ ArfARTRAW class ]  ||
        [ inputFileImageCompare   dataImageClass ] != [ ArfARTRAW class ]  ) {
        ART_ERRORHANDLING_FATAL_ERROR("this tool currently works only on ARTRAW images");
    }
    */
    
    ArnPlainImage  * imageReference =
        [ ALLOC_OBJECT(ArnCIEXYZAImage)
             initWithSize
             :   sizeReference
             ];
    
    [ inputFileImageReference getPlainImage
        :   IPNT2D( 0, 0 )
        :   imageReference
        ];
    
    ArnPlainImage  * imageCompare =
        [ ALLOC_OBJECT(ArnCIEXYZAImage)
            initWithSize
            :   sizeCompare
            ];
    
    [ inputFileImageCompare getPlainImage
         :   IPNT2D( 0, 0 )
         :   imageCompare
         ];
    
    
    [ ART_GLOBAL_REPORTER beginTimedAction
         :   "Computing difference"
         ];
    
    ArnCIEXYZAImage  * imageReferenceScanline =
        [ ALLOC_OBJECT(ArnCIEXYZAImage)
             initWithSize
             :   IVEC2D(XC(sizeReference),1)
             ];

    ArnCIEXYZAImage  * imageCompareScanline =
        [ ALLOC_OBJECT(ArnCIEXYZAImage)
             initWithSize
             :   IVEC2D(XC(sizeCompare),1)
             ];

    double delta = 0;
    
    for (int y = 0; y < YC(sizeReference); y++)
    {
        [ imageReference getPlainImage
             :   IPNT2D( 0, y )
             :   imageReferenceScanline
             ];
        
        [ imageCompare getPlainImage
             :   IPNT2D( 0, y )
             :   imageCompareScanline
             ];

        for (int x = 0; x < XC(sizeReference); x++)
        {
            ArCIEXYZ valueReference = (((ArnCIEXYZAImage*)imageReferenceScanline)->data[ x ]).xyz;
            ArCIEXYZ valueCompare   = (((ArnCIEXYZAImage*)imageCompareScanline)->data[ x ]).xyz;

            
            double xA = ARCIEXYZ_X(valueReference);
            double yA = ARCIEXYZ_Y(valueReference);
            double zA = ARCIEXYZ_Z(valueReference);
            
            double xB = ARCIEXYZ_X(valueCompare);
            double yB = ARCIEXYZ_Y(valueCompare);
            double zB = ARCIEXYZ_Z(valueCompare);
            
            delta += M_ABS(xA - xB) + M_ABS(yA - yB) + M_ABS(zA - zB);

        }
    }

    delta /= (double)(YC(sizeReference) * XC(sizeReference) * 3);
    
    [ ART_GLOBAL_REPORTER endAction ];
    

    [ ART_GLOBAL_REPORTER consolePrintf
         :   "diff: %f\n"
         ,   delta
         ];

    if ( [ outputSNROpt hasBeenSpecified ] ) {
        FILE * outputFile = fopen( [ outputSNROpt cStringValue ], "w");
        
        if (outputFile) {
            fprintf(outputFile, "%f", delta);
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

ADVANCED_RENDERING_TOOLKIT_MAIN(art_imagediff)

// ===========================================================================
