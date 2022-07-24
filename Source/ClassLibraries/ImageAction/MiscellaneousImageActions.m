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

#define ART_MODULE_NAME     MiscellaneousImageActions

#ifndef __APPLE__
#import <sys/wait.h>
#endif

#include <unistd.h>

#import "MiscellaneousImageActions.h"
#import "ART_ImageData.h"
#import "ART_ImageFileFormat.h"
#import "ArnImageManipulationMacros.h"

#import "ColourAndLightSubsystem.h"

#import "ART_ARM_Interface.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnImageFractionDitherer registerWithRuntime ];
    [ ArnARTCSPLuminanceClipping registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

/* ===========================================================================
    'ArnImageFractionDitherer'
=========================================================================== */

@implementation ArnImageFractionDitherer

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnImageFractionDitherer)
ARPACTION_DEFAULT_IMPLEMENTATION(ArnImageFractionDitherer)

- (id) init
        : (const char *) newDitherFileName
{
    self = [ super init ];

    if ( self )
    {
        ditherFileName = arsymbol(art_gv,newDitherFileName);
    }
    
    return self;
}

- (void) performOn
        : (ArNode <ArpNodeStack> *) nodeStack
{
    ArNodeRef  ciexyzImage_Ref = [ nodeStack pop ];

    ArnCIEXYZAImage  * ciexyzImage =
        (ArnCIEXYZAImage*)ARNODEREF_POINTER(ciexyzImage_Ref);

    [ ART_GLOBAL_REPORTER beginAction
        :   "fractional dither"
        ];

    if ( [ ciexyzImage isMemberOfClass: [ ArnCIEXYZAImage class ] ] )
    {
        ArnFileImage * ditherFile = [ ALLOC_INIT_OBJECT(ArnFileImage): ditherFileName ];

        ArnRGBA32Image * ditherImage =
        [ ALLOC_OBJECT(ArnRGBA32Image) initAsCopyOf: ditherFile ];

        ArnRGBA32Image * rgba32Image =
            [ ciexyzImage fractionDitheredRGBA32Image
                :   ditherImage
                :   ART_GLOBAL_REPORTER
                ];

        [ nodeStack push
            :   HARD_NODE_REFERENCE(rgba32Image)
            ];

        RELEASE_OBJECT(ditherImage);
        RELEASE_OBJECT(ditherFile);
        RELEASE_OBJECT(ciexyzImage);
        RELEASE_OBJECT(rgba32Image);
    }

    RELEASE_NODE_REF( ciexyzImage_Ref );

    [ ART_GLOBAL_REPORTER endAction ];
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [super code :coder];
    [coder codeSymbol :&ditherFileName];
}

@end


@implementation ArnARTCSPLuminanceClipping

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnARTCSPLuminanceClipping)
ARPACTION_DEFAULT_SINGLE_IMAGE_ACTION_IMPLEMENTATION(ArnARTCSPLuminanceClipping)

- (id) removeSource
                        : (BOOL) newRemoveOption
        whiteLuminance  : (double) newWhiteLuminance
{
    [ self init
        :   newRemoveOption
        :   newWhiteLuminance ];

    return self;
}

- (id) init
        : (BOOL) newRemoveOption
        : (double) newWhiteLuminance
{
    self = [ super init: newRemoveOption ];

    if ( self )
    {
        whiteLuminance = newWhiteLuminance;

        if ( whiteLuminance <= 100.0 )
            ART_ERRORHANDLING_FATAL_ERROR(
                "white luminance value <= 100.0"
                );
    }
    
    return self;
}

- (void) performOn
        : (ArNode <ArpNodeStack> *) nodeStack
{
    [ ART_GLOBAL_REPORTER beginTimedAction
        :   "luminance and chroma reduction for L > 100.0"
        ];


    /* ------------------------------------------------------------------
         Before calling the function that sets up the framework for
         image manipulation we have to specify what colour type the
         result image will have.

         imageDataType = what we are going to feed it
         fileDataType  = what we want it to write to disk for us
    ---------------------------------------------------------------aw- */

    destinationImageDataType = ardt_xyz;
    destinationFileDataType  = ardt_xyz;


    /* ------------------------------------------------------------------
         Activation of the framework common to all image manipulation
         actions. This takes the source image from the stack, and creates
         the destination image along with all needed scanline buffers.

         In order to do this properly it has to be informed of what
         kind of source image to expect, and what kind of result image
         we wish to create (in our case, ArfARTCSP and ArfARTCSP).
    ---------------------------------------------------------------aw- */

    [ self prepareForImageManipulation
        :   nodeStack
        :   [ ArfARTCSP class ]
        :   [ ArfARTCSP class ] ];

    double  mappingRange = whiteLuminance - 100.0;


    /* ------------------------------------------------------------------
         Process all pixels in all the source images.
    ---------------------------------------------------------------aw- */

    for ( unsigned int i = 0; i < numberOfSourceImages; i++ )
    {
        for ( long y = 0; y < YC(destinationImageSize); y++ )
        {
            [ self loadSourceScanlineBuffer: i : y ];

            for ( long x = 0; x < XC(destinationImageSize); x++ )
            {
                //   Convert the pixel to L*a*b* colour space

                ArCIELab  labValue;

                //  missing: add wp information!

                xyz_to_lab(
                      art_gv,
                    & XYZA_SOURCE_BUFFER_XYZ(x),
                    & labValue );

                //   Values above the "white luminance" are directly mapped
                //   to white @ L = 100.0

                if ( ARCIELab_L(labValue) > whiteLuminance )
                {
                    labValue = ARCIELab( 100.0, 0.0, 0.0 );
                }
                else
                {
                    //   Values above 100.0 are moved to the gamut, all
                    //   others are left alone

                    if ( ARCIELab_L(labValue) > 100.0 )
                    {
                        double  focusLuminance;

                        focusLuminance =
                            ( ( ARCIELab_L(labValue) - 100.0 ) / mappingRange ) * 100.0;

                        lab_move_luminance_below_100(
                              art_gv,
                              focusLuminance,
                            & labValue
                            );
                    }
                }

                //   back to XYZ

                lab_to_xyz(
                      art_gv,
                    & labValue,
                    & XYZA_DESTINATION_BUFFER_XYZ(x) );

                XYZA_DESTINATION_BUFFER_ALPHA(x) = XYZA_SOURCE_BUFFER_ALPHA(x);
            }

            [ self writeDestinationScanlineBuffer: i : y ];
        }
    }


    /* ------------------------------------------------------------------
         Free the image manipulation infrastructure and end the action;
         this also places the destination image on the stack.
    ---------------------------------------------------------------aw- */

    [ self finishImageManipulation
        :   nodeStack ];

    [ ART_GLOBAL_REPORTER endAction ];
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];

    [ coder codeDouble: & whiteLuminance ];

    if ( whiteLuminance <= 100.0 )
        ART_ERRORHANDLING_FATAL_ERROR(
            "white luminance value <= 100.0"
            );
}

@end


/* ===========================================================================
    'ArnOpenImageInExternalViewer'
=========================================================================== */


@implementation ArnOpenImageInExternalViewer

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnOpenImageInExternalViewer)
ARPACTION_DEFAULT_IMPLEMENTATION(ArnOpenImageInExternalViewer)

- (void) performOn
        : (ArNode <ArpNodeStack> *) nodeStack
{
    ArList     imageRefList = ARLIST_EMPTY;
    ArNodeRef  refFromStack;

    //   We pop all the things from the stack we can find, and assign them to
    //   the pointers that need filling.

    while ( ARNODEREF_POINTER( refFromStack = [ nodeStack pop ] ) )
    {
        if ( [ ARNODEREF_POINTER(refFromStack)
                 conformsToArProtocol
                 :   ARPROTOCOL(ArpBasicImage)
                 ] )
        {
            arlist_add_noderef_at_tail( & imageRefList, refFromStack );
        }
    }
    
    int  numberOfImages = arlist_length( & imageRefList );

    if ( numberOfImages == 0 )
        ART_ERRORHANDLING_FATAL_ERROR(
            "no result images found on stack"
            );

    id  * image = ALLOC_ARRAY( id, numberOfImages );

    for ( int i = 0; i < numberOfImages; i++ )
    {
        ArNodeRef  imageRef = ARNODEREF_NONE;
        
        arlist_pop_noderef( & imageRefList, & imageRef );

        image[i] = ARNODEREF_POINTER(imageRef);

        [ nodeStack push
            :   imageRef
            ];
    }

    if ( numberOfImages > 1 )
    {
        [ ART_GLOBAL_REPORTER beginAction
            :   "opening %d result images in external viewer"
            ,   numberOfImages
            ];
    }
    else
    {
        [ ART_GLOBAL_REPORTER beginAction
            :   "opening result image in external viewer"
            ];
    }

    for ( int i = 0; i < numberOfImages; i++ )
    {
        int   imageOpeningSubprocessResult;
        pid_t imageOpeningSubprocessPID = fork();

        if ( imageOpeningSubprocessPID == 0 ) // child process
        {
            //   Close stderr in case someone uses a KDE app as external
            //   viewer. These things talk *way* too much once they are
            //   started.

            close(STDERR_FILENO);

            imageOpeningSubprocessResult =
                execlp(
                    ART_VIEWER,
                    ART_VIEWER,
                    [ image[i] fileName ],
                    (char *)NULL
                    );

            if ( imageOpeningSubprocessResult == -1 )
                ART_ERRORHANDLING_WARNING(
                    "viewer call failed with error code %d '%s'"
                    ,   errno
                    ,   strerror(errno)
                    );
        }
        else
            if ( imageOpeningSubprocessPID == -1 )
                ART_ERRORHANDLING_FATAL_ERROR(
                    "fork() failed with error code %d '%s'"
                    ,   errno
                    ,   strerror(errno)
                    );
    }
    
    FREE_ARRAY( image );

    [ ART_GLOBAL_REPORTER endAction ];
}

@end

/* ===========================================================================
    'ArnSetColourSubsystemWhitepoint'
=========================================================================== */


@implementation ArnSetColourSubsystemWhitepoint

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnSetColourSubsystemWhitepoint)
ARPACTION_DEFAULT_IMPLEMENTATION(ArnSetColourSubsystemWhitepoint)

- (id) init
        : (char *) newWP_Desc
        : (ArNode <ArpSpectrum> *) newIlluminantSpectrum
{
    self =
        [ super init
            :   HARD_NODE_REFERENCE(newIlluminantSpectrum)
            ];

    if ( self )
    {
        wp_desc = arsymbol(art_gv, newWP_Desc);
    }
    
    return self;
}

- (id) init
{
    return
        [ self init
            :   "D50"
            :   0
            ];
}

#define  WHITEPOINT_SUBNODE ((ArNode <ArpSpectrum>*)ARNUNARY_SUBNODE)

- (void) performOn
        : (ArNode <ArpNodeStack> *) nodeStack
{
    (void) nodeStack;
    
    if ( art_system_white_point_has_been_manually_set(art_gv) )
    {
        [ ART_GLOBAL_REPORTER beginAction
            :   "white point setting of %s overridden by the user to %s"
            ,   wp_desc
            ,   art_system_white_point_symbol(art_gv)
            ];

        [ ART_GLOBAL_REPORTER endAction ];
    }
    else
    {
        [ ART_GLOBAL_REPORTER beginAction
            :   "setting the white point of the colour subsystem to %s"
            ,   wp_desc
            ];

        if ( WHITEPOINT_SUBNODE )
        {
            ArSpectrum500  wp500;
            
            [ WHITEPOINT_SUBNODE getHiresSpectrum
                :   0
                : & wp500
                ];
            
            ArCIEXYZ  wpXYZ;
            
            s500_to_xyz(
                  art_gv,
                & wp500,
                & wpXYZ
                );
            
            ArCIExyY  wpxyY;
            
            xyz_to_xyy(
                  art_gv,
                & wpXYZ,
                & wpxyY
                );
            
            ArCIExy  wpxy;
            
            ARCIExy_x(wpxy) = ARCIExyY_x(wpxyY);
            ARCIExy_y(wpxy) = ARCIExyY_y(wpxyY);

            art_set_system_white_point(
                  art_gv,
                  wp_desc,
                & wpxy
                );
        }
        else
        {
            art_set_system_white_point_by_desc( art_gv, wp_desc );
        }

        [ ART_GLOBAL_REPORTER endAction ];
    }
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code
        :   coder
        ];

    [ coder codeSymbol
        : & wp_desc
        ];
}

@end


/* ===========================================================================
    'ArnChangeISR_to_Match_RAW_Contents'
=========================================================================== */


@implementation ArnChangeISR_to_Match_RAW_Contents

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnChangeISR_to_Match_RAW_Contents)
ARPACTION_DEFAULT_IMPLEMENTATION(ArnChangeISR_to_Match_RAW_Contents)

- (id) init
        : (double) newWavelengthToCheckAgainstISRBounds
        : (BOOL) newRequirePolarisedRAW
{
    self = [ super init ];

    if ( self )
    {
        wavelengthToCheckAgainstISRBounds = newWavelengthToCheckAgainstISRBounds;
        requirePolarisedRAW               = newRequirePolarisedRAW;
    }
    
    return self;
}

- (id) init
{
    return
        [ self init
            :   ISR_CHANGE_PERFORM_NO_WAVELENGTH_CHECK
            :   NO
            ];
}

- (void) performOn
        : (ArNode <ArpNodeStack> *) nodeStack
{
    ArNodeRef  nodeFromStack_Ref = [ nodeStack pop ];

    id  nodeFromStack = ARNODEREF_POINTER(nodeFromStack_Ref);

    ArnFileImage  * fileImageToAdaptTo = NULL;

    if ( [ nodeFromStack isKindOfClass: [ ArnFileImage class ] ] )
        fileImageToAdaptTo = (ArnFileImage *) nodeFromStack;
    else
        ART_ERRORHANDLING_FATAL_ERROR(
            "wrong type of object found on stack - "
            "%s instead of a subclass of ArnFileImage"
            ,   [ [ nodeFromStack class ] cStringClassName ]
            );

    ArfRAWRasterImage  * rawImage = NULL;

    if ( ! [ fileImageToAdaptTo->imageFile isKindOfClass:[ ArfRAWRasterImage class ] ] ) {
        ART_ERRORHANDLING_FATAL_ERROR(
            "wrong type of image found on stack - "
            "%s instead of a RAW image"
            ,   [ [ fileImageToAdaptTo dataImageClass ] cStringClassName ]
            );
    } else {
        rawImage = (ArfRAWRasterImage *) fileImageToAdaptTo->imageFile;
    }

    ArDataType  rawContentType = [ rawImage fileDataType ];

    if (    requirePolarisedRAW
         && ! ( rawContentType & ardt_polarisable ) )
        ART_ERRORHANDLING_FATAL_ERROR(
            "non-polarised source image"
            );

    //   Check if the current ISR is already set to match the contents
    //   of the RAW file

    if ( rawContentType == art_isr( art_gv ) )
    {
        //   If they match, we just get out of here - there is nothing for
        //   us to do.

        [ nodeStack push
            :   nodeFromStack_Ref
            ];

        RELEASE_NODE_REF(nodeFromStack_Ref);
        
    }
    else
    {
        //   If they do not match...

        [ ART_GLOBAL_REPORTER beginAction
            :   "automatically switching ISR to match RAW contents"
            ];

        char  * inputFileName;

        arstring_s_copy_s(
              [ fileImageToAdaptTo fileName ],
            & inputFileName
            );
        
        //   This releases the original image we took off the stack.

        RELEASE_NODE_REF(nodeFromStack_Ref);

        [ ART_GLOBAL_REPORTER printf
            :   "Default ISR was   : %s\n"
            ,   ardatatype_name( art_isr( art_gv ) )
            ];

        [ ART_GLOBAL_REPORTER printf
            :   "RAW content is : %s\n"
            ,   ardatatype_name( rawContentType )
            ];

        [ ART_GLOBAL_REPORTER printf
            :   "ISR will be set as: %s\n"
            ,   ardatatype_name( rawContentType )
            ];

        art_set_isr( art_gv, rawContentType );

        [ ART_GLOBAL_REPORTER printf
            :   "ISR is now set to : %s\n"
            ,   ardatatype_name( art_isr( art_gv ) )
            ];

        [ ART_GLOBAL_REPORTER printf
            :   "Re-reading raw image...\n"
            ];

        fileImageToAdaptTo =
            [ FILE_IMAGE
                :   inputFileName
                ];

        FREE_ARRAY( inputFileName );

        [ ART_GLOBAL_REPORTER printf
            :   "Done.\n"
            ];

        [ ART_GLOBAL_REPORTER endAction ];

        [ nodeStack push
            :   HARD_NODE_REFERENCE(fileImageToAdaptTo)
            ];

        RELEASE_OBJECT(fileImageToAdaptTo);
    }
    
    //   Now that we can be sure to be in a ISR that matches RAW
    //   contents, we can check sample WL validity - if this was
    //   requested
    
    if (    wavelengthToCheckAgainstISRBounds
         != ISR_CHANGE_PERFORM_NO_WAVELENGTH_CHECK )
    {
        double  raw_lower_bound =
            spc_channel_lower_bound( art_gv, 0 );

        double  raw_upper_bound =
              spc_channel_lower_bound( art_gv, spc_channels(art_gv) - 1 )
            + spc_channel_width(art_gv, spc_channels(art_gv) - 1 );
        
        if (   ( wavelengthToCheckAgainstISRBounds < raw_lower_bound )
            || ( wavelengthToCheckAgainstISRBounds > raw_upper_bound ) )
            ART_ERRORHANDLING_FATAL_ERROR(
                "RAW contains no data for selected wavelength: "
                "spectral range is [%3.0f-%3.0f] nm, selected WL is %3.0f nm"
                ,   NANO_FROM_UNIT(raw_lower_bound)
                ,   NANO_FROM_UNIT(raw_upper_bound)
                ,   NANO_FROM_UNIT(wavelengthToCheckAgainstISRBounds)
                );
    }
}

@end


/* ===========================================================================
    'ArnOutputCurrentISR'
=========================================================================== */

@implementation ArnOutputCurrentISR

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnOutputCurrentISR)
ARPACTION_DEFAULT_IMPLEMENTATION(ArnOutputCurrentISR)

- (void) performOn
        : (ArNode <ArpNodeStack> *) nodeStack
{
    (void) nodeStack;
    
    [ ART_GLOBAL_REPORTER beginAction
        :   "performing computations using %s%s as ISR"
        ,   ardatatype_polarisable_string( art_isr( art_gv ) )
        ,   ardatatype_long_name_string( art_gv, art_isr( art_gv ) )
        ];

    [ ART_GLOBAL_REPORTER endAction ];
}

@end

/* ===========================================================================
    'ArnRAW_Double_Mul_RAW'
=========================================================================== */

@implementation ArnRAW_Double_Mul_RAW

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnRAW_Double_Mul_RAW)
ARPACTION_DEFAULT_SINGLE_IMAGE_ACTION_IMPLEMENTATION(ArnRAW_Double_Mul_RAW)

- (id) _init
        : (double) newFactor
        : (BOOL) newCloneOption
{
    self =
        [ super init
            :   NO
            ];
    
    if ( self )
    {
        factor      = newFactor;
        cloneOption = newCloneOption;
    }
    
    return self;
}

- (id) _init
        : (double) newFactor
{
    self =
        [ self _init
            :   newFactor
            :   YES
            ];

    return self;
}

- (id) multiplyBy
                   : (double) newFactor
        cloneSource: (BOOL) newCloneOption
{
    return
        [ self _init
            :   newFactor
            :   newCloneOption
            ];
}

- (id) multiplyBy
                   : (double) newFactor
{
    return
        [ self _init
            :   newFactor
            ];
}

- (void) performOn
        : (ArNode <ArpNodeStack> *) nodeStack
{
    [ ART_GLOBAL_REPORTER beginTimedAction
        :   "multiplying RAW pixels by %f"
        ,   factor
        ];

    /* ------------------------------------------------------------------
         Before calling the function that sets up the framework for
         image manipulation we have to specify what colour type the
         result image will have.

         imageDataType = what we are going to feed it
         fileDataType  = what we want it to write to disk for us
    ---------------------------------------------------------------aw- */


    //   In the case of a renderer directly writing its output to file, the
    //   image and file colour data types are the same.

    destinationImageDataType = art_isr( art_gv );
    destinationFileDataType = destinationImageDataType;


    /* ------------------------------------------------------------------
         Activation of the framework common to all image manipulation
         actions. This takes the source image from the stack, and creates
         the destination image along with all needed scanline buffers.

         In order to do this properly it has to be informed of what
         kind of source image to expect, and what kind of result image
         we wish to create (in our case, ArfRAWRasterImage).
    ---------------------------------------------------------------aw- */

#ifdef ART_WITH_OPENEXR
    if (ART_RAW_WORKFLOW_FORMAT_IS_NATIVE) {
        [ self prepareForImageManipulation
            :   nodeStack
            :   [ ArfRAWRasterImage class ]
            :   [ ArfARTRAW class ]
            ];
    } else {
        [ self prepareForImageManipulation
            :   nodeStack
            :   [ ArfRAWRasterImage class ]
            :   [ ArfOpenEXRSpectral class ]
            ];
    }
#else
    [ self prepareForImageManipulation
	:   nodeStack
	:   [ ArfRAWRasterImage class ]
	:   [ ArfARTRAW class ]
        ];
#endif // ART_WITH_OPENEXR

    /* ------------------------------------------------------------------
         Process all pixels in the image.
    ---------------------------------------------------------------aw- */

    ArStokesVector  * sv0 = arstokesvector_alloc(art_gv);

    for ( unsigned int i = 0; i < numberOfSourceImages; i++ )
    {
        for ( long y = 0; y < YC(destinationImageSize); y++ )
        {
            [ self loadSourceScanlineBuffer: i : y ];

            for ( long x = 0; x < XC(destinationImageSize); x++ )
            {
                arlight_dl_mul_l(
                      art_gv,
                      factor,
                      LIGHTALPHA_SOURCE_BUFFER_LIGHT(x),
                      LIGHTALPHA_DESTINATION_BUFFER_LIGHT(x)
                    );
                
                LIGHTALPHA_DESTINATION_BUFFER_ALPHA(x) =
                    LIGHTALPHA_SOURCE_BUFFER_ALPHA(x);
            }

            [ self writeDestinationScanlineBuffer: i : y ];
        }
    }

    arstokesvector_free( art_gv, sv0 );


    /* ------------------------------------------------------------------
         Free the image manipulation infrastructure and end the action;
         this also places the destination image on the stack.
    ---------------------------------------------------------------aw- */

    [ self finishImageManipulation
        :   nodeStack
        ];

    [ ART_GLOBAL_REPORTER endAction ];
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];

    [ coder codeDouble: & factor ];
    [ coder codeBOOL  : & cloneOption ];
}

@end

/* ===========================================================================
    'ArnFilterTinyRAWValues'
=========================================================================== */

@implementation ArnFilterTinyRAWValues

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnFilterTinyRAWValues)
ARPACTION_DEFAULT_SINGLE_IMAGE_ACTION_IMPLEMENTATION(ArnFilterTinyRAWValues)

- (id) _init
        : (double) newThresholdValue
        : (BOOL) newCloneOption
{
    self =
        [ super init
            :   NO
            ];
    
    if ( self )
    {
        thresholdValue = newThresholdValue;
        cloneOption    = newCloneOption;
    }
    
    return self;
}

- (id) _init
        : (double) newThresholdValue
{
    self =
        [ self _init
            :   newThresholdValue
            :   YES
            ];

    return self;
}

- (id) thresholdValue
                   : (double) newThresholdValue
        cloneSource: (BOOL) newCloneOption
{
    return
        [ self _init
            :   newThresholdValue
            :   newCloneOption
            ];
}

- (id) thresholdValue
                   : (double) newThresholdValue
{
    return
        [ self _init
            :   newThresholdValue
            ];
}

- (void) performOn
        : (ArNode <ArpNodeStack> *) nodeStack
{
    [ ART_GLOBAL_REPORTER beginTimedAction
        :   "filtering RAW pixels with S0 < %f"
        ,   thresholdValue
        ];

    /* ------------------------------------------------------------------
         Before calling the function that sets up the framework for
         image manipulation we have to specify what colour type the
         result image will have.

         imageDataType = what we are going to feed it
         fileDataType  = what we want it to write to disk for us
    ---------------------------------------------------------------aw- */


    //   In the case of a renderer directly writing its output to file, the
    //   image and file colour data types are the same.

    destinationImageDataType = art_isr( art_gv );

    destinationFileDataType = destinationImageDataType;


    /* ------------------------------------------------------------------
         Activation of the framework common to all image manipulation
         actions. This takes the source image from the stack, and creates
         the destination image along with all needed scanline buffers.

         In order to do this properly it has to be informed of what
         kind of source image to expect, and what kind of result image
         we wish to create (in our case, ArfRAWRasterImage).
    ---------------------------------------------------------------aw- */

#ifdef ART_WITH_OPENEXR
    if (ART_RAW_WORKFLOW_FORMAT_IS_NATIVE) {
        [ self prepareForImageManipulation
            :   nodeStack
            :   [ ArfRAWRasterImage class ]
            :   [ ArfARTRAW class ]
            ];
    } else {
        [ self prepareForImageManipulation
            :   nodeStack
            :   [ ArfRAWRasterImage class ]
            :   [ ArfOpenEXRSpectral class ]
            ];
    }
#else
    [ self prepareForImageManipulation
	:   nodeStack
	:   [ ArfRAWRasterImage class ]
	:   [ ArfARTRAW class ]
        ];
#endif // ART_WITH_OPENEXR

    /* ------------------------------------------------------------------
         Process all pixels in the image.
    ---------------------------------------------------------------aw- */

    ArStokesVector  * sv0 = arstokesvector_alloc(art_gv);

    for ( unsigned int i = 0; i < numberOfSourceImages; i++ )
    {
        for ( long y = 0; y < YC(destinationImageSize); y++ )
        {
            [ self loadSourceScanlineBuffer: i : y ];

            for ( long x = 0; x < XC(destinationImageSize); x++ )
            {
                arlight_l_to_sv(
                      art_gv,
                      LIGHTALPHA_SOURCE_BUFFER_LIGHT(x),
                      sv0
                    );
                
                double  maxS0 =
                    spc_s_max(art_gv, ARSV_I( *sv0, 0 ) );
                
                if ( maxS0 < thresholdValue )
                {
                    arlight_d_init_unpolarised_l(
                          art_gv,
                          0.0,
                          LIGHTALPHA_DESTINATION_BUFFER_LIGHT(x)
                        );
                }
                else
                {
                    arlight_l_init_l(
                          art_gv,
                          LIGHTALPHA_SOURCE_BUFFER_LIGHT(x),
                          LIGHTALPHA_DESTINATION_BUFFER_LIGHT(x)
                        );
                }

                LIGHTALPHA_DESTINATION_BUFFER_ALPHA(x) =
                    LIGHTALPHA_SOURCE_BUFFER_ALPHA(x);
            }

            [ self writeDestinationScanlineBuffer: i : y ];
        }
    }

    arstokesvector_free( art_gv, sv0 );


    /* ------------------------------------------------------------------
         Free the image manipulation infrastructure and end the action;
         this also places the destination image on the stack.
    ---------------------------------------------------------------aw- */

    [ self finishImageManipulation
        :   nodeStack
        ];

    [ ART_GLOBAL_REPORTER endAction ];
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];

    [ coder codeDouble: & thresholdValue ];
    [ coder codeBOOL  : & cloneOption ];
}

@end

/* ===========================================================================
    'ArnDownscaleRAW'
=========================================================================== */

@implementation ArnDownscaleRAW

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnDownscaleRAW)
ARPACTION_DEFAULT_SINGLE_IMAGE_ACTION_IMPLEMENTATION(ArnDownscaleRAW)

- (id) downscaleFactor
                   : (unsigned int) newDownscaleFactor
{
    [ super init
        :   NO
        ];

    downscaleFactor = newDownscaleFactor;
    
    return self;
}

- (void) performOn
        : (ArNode <ArpNodeStack> *) nodeStack
{
    [ ART_GLOBAL_REPORTER beginTimedAction
        :   "down-scaling RAW image size by a factor of %d"
        ,   downscaleFactor
        ];

    /* ------------------------------------------------------------------
         Before calling the function that sets up the framework for
         image manipulation we have to specify what colour type the
         result image will have.

         imageDataType = what we are going to feed it
         fileDataType  = what we want it to write to disk for us
    ---------------------------------------------------------------aw- */

    //   In the case of a renderer directly writing its output to file, the
    //   image and file colour data types are the same.

    destinationImageDataType = art_isr( art_gv );
    destinationFileDataType = destinationImageDataType;


    /* ------------------------------------------------------------------
         Activation of the framework common to all image manipulation
         actions. This takes the source image from the stack, and creates
         the destination image along with all needed scanline buffers.

         In order to do this properly it has to be informed of what
         kind of source image to expect, and what kind of result image
         we wish to create (in our case, ArfRAW and ArfARTCSP).
    ---------------------------------------------------------------aw- */

#ifdef ART_WITH_OPENEXR
    if (ART_RAW_WORKFLOW_FORMAT_IS_NATIVE) {
        [ self prepareForImageManipulation
            :   nodeStack
            :   [ ArfRAWRasterImage class ]
            :   [ ArfARTRAW class ]
            :   downscaleFactor
            ];
    } else {
        [ self prepareForImageManipulation
            :   nodeStack
            :   [ ArfRAWRasterImage class ]
            :   [ ArfOpenEXRSpectral class ]
            :   downscaleFactor
            ];
    }
#else
    [ self prepareForImageManipulation
	:   nodeStack
	:   [ ArfRAWRasterImage class ]
	:   [ ArfARTRAW class ]
	:   downscaleFactor
        ];
#endif // ART_WITH_OPENEXR

    /* ------------------------------------------------------------------
         Process all pixels in the image.
    ---------------------------------------------------------------aw- */

    for ( unsigned int i = 0; i < numberOfSourceImages; i++ )
    {
        for ( long y = 0; y < YC(destinationImageSize); y++ )
        {
            [ self loadSourceScanlineBuffer: i : y * downscaleFactor ];

            for ( long x = 0; x < XC(destinationImageSize); x++ )
            {
                unsigned int validSVs = 0;
                
                arlight_d_init_unpolarised_l(
                      art_gv,
                      0.0,
                      LIGHTALPHA_DESTINATION_BUFFER_LIGHT(x)
                    );
                
                LIGHTALPHA_DESTINATION_BUFFER_ALPHA(x) = 0.0;

                for ( unsigned int xx = 0; xx < downscaleFactor; xx++ )
                {
                    for ( unsigned int yy = 0; yy < downscaleFactor; yy++ )
                    {
                        long  actual_x = ( x * downscaleFactor ) + xx;
                        long  actual_y = ( y * downscaleFactor ) + yy;
                        
                        if (   actual_x < XC(sourceImageSize)
                            && actual_y < YC(sourceImageSize))
                        {
                            validSVs++;

                            arlight_dl_sloppy_add_l(
                                  art_gv,
                                  5 DEGREES,
                                  LIGHTALPHA_SOURCE_BUFFER_LIGHT_I(actual_x,yy),
                                  LIGHTALPHA_DESTINATION_BUFFER_LIGHT(x)
                                );
                            
                            LIGHTALPHA_DESTINATION_BUFFER_ALPHA(x) +=
                                LIGHTALPHA_SOURCE_BUFFER_ALPHA_I(actual_x,yy);
                        }
                    }
                }
                
                if ( validSVs == 0 )
                {
                    ART_ERRORHANDLING_FATAL_ERROR(
                        "downscale consistency error"
                        );
                }

                arlight_d_mul_l(
                      art_gv,
                      1.0 / (double) validSVs,
                      LIGHTALPHA_DESTINATION_BUFFER_LIGHT(x)
                    );

                LIGHTALPHA_DESTINATION_BUFFER_ALPHA(x) =
                    LIGHTALPHA_DESTINATION_BUFFER_ALPHA(x) / validSVs;
            }

            [ self writeDestinationScanlineBuffer: i : y ];
        }
    }

    /* ------------------------------------------------------------------
         Free the image manipulation infrastructure and end the action;
         this also places the destination image on the stack.
    ---------------------------------------------------------------aw- */

    [ self finishImageManipulation
        :   nodeStack
        ];

    [ ART_GLOBAL_REPORTER endAction ];
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];

    [ coder codeUInt: & downscaleFactor ];
}

@end

/* ===========================================================================
    'ArnFilterHighDopRAWValues'
=========================================================================== */

@implementation ArnFilterHighDopRAWValues

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnFilterHighDopRAWValues)
ARPACTION_DEFAULT_SINGLE_IMAGE_ACTION_IMPLEMENTATION(ArnFilterHighDopRAWValues)

- (id) _init
        : (double) newThresholdValue
        : (BOOL) newCloneOption
        : (BOOL) newRemoveOption
{
    self =
        [ super init
            :   newRemoveOption
            ];
    
    if ( self )
    {
        thresholdValue = newThresholdValue;
        cloneOption    = newCloneOption;
    }
    
    return self;
}

- (id) _init
        : (double) newThresholdValue
{
    self =
        [ self _init
            :   newThresholdValue
            :   YES
            :   NO
            ];

    return self;
}

- (id) thresholdValue
                   : (double) newThresholdValue
        cloneSource: (BOOL) newCloneOption
{
    return
        [ self _init
            :   newThresholdValue
            :   newCloneOption
            :   NO
            ];
}

- (id) thresholdValue
                    : (double) newThresholdValue
        removeSource: (BOOL) newRemoveOption
{
    return
        [ self _init
            :   newThresholdValue
            :   NO
            :   newRemoveOption
            ];
}

- (id) thresholdValue
                   : (double) newThresholdValue
{
    return
        [ self _init
            :   newThresholdValue
            ];
}

- (void) performOn
        : (ArNode <ArpNodeStack> *) nodeStack
{
    [ ART_GLOBAL_REPORTER beginTimedAction
        :   "filtering ARTRAW pixels with DOP > %f"
        ,   thresholdValue
        ];

    /* ------------------------------------------------------------------
         Before calling the function that sets up the framework for
         image manipulation we have to specify what colour type the
         result image will have.

         imageDataType = what we are going to feed it
         fileDataType  = what we want it to write to disk for us
    ---------------------------------------------------------------aw- */

    //   In the case of a renderer directly writing its output to file, the
    //   image and file colour data types are the same.

    destinationImageDataType = art_isr( art_gv );
    destinationFileDataType = destinationImageDataType;


    /* ------------------------------------------------------------------
         Activation of the framework common to all image manipulation
         actions. This takes the source image from the stack, and creates
         the destination image along with all needed scanline buffers.

         In order to do this properly it has to be informed of what
         kind of source image to expect, and what kind of result image
         we wish to create (in our case, ArfRAWRasterImage).
    ---------------------------------------------------------------aw- */
#ifdef ART_WITH_OPENEXR
    if (ART_RAW_WORKFLOW_FORMAT_IS_NATIVE) {
        [ self prepareForImageManipulation
            :   nodeStack
            :   [ ArfRAWRasterImage class ]
            :   [ ArfARTRAW class ]
            ];
    } else {
        [ self prepareForImageManipulation
            :   nodeStack
            :   [ ArfRAWRasterImage class ]
            :   [ ArfOpenEXRSpectral class ]
            ];
    }
#else
    [ self prepareForImageManipulation
        :   nodeStack
        :   [ ArfRAWRasterImage class ]
	:   [ ArfARTRAW class ]
        ];
#endif // ART_WITH_OPENEXR

    /* ------------------------------------------------------------------
         Process all pixels in the image.
    ---------------------------------------------------------------aw- */

    ArStokesVector  * sv0 = arstokesvector_alloc(art_gv);
    ArSpectrum  * dop = spc_alloc(art_gv);
    
    double  num_channels = spc_channels(art_gv);

    for ( unsigned int i = 0; i < numberOfSourceImages; i++ )
    {
        for ( long y = 0; y < YC(destinationImageSize); y++ )
        {
            [ self loadSourceScanlineBuffer: i : y ];

            for ( long x = 0; x < XC(destinationImageSize); x++ )
            {
                arlight_l_to_sv(
                      art_gv,
                      LIGHTALPHA_SOURCE_BUFFER_LIGHT(x),
                      sv0
                    );
                
                for ( int j = 0; j < num_channels; j++ )
                {
                    double  c_dop = 0.0;

                    if ( spc_si(art_gv, ARSV_I( *sv0, 0 ),j) > 0. )
                    {
                        c_dop =
                            sqrt(
                              M_SQR( spc_si(art_gv, ARSV_I( *sv0, 1 ), j) )
                            + M_SQR( spc_si(art_gv, ARSV_I( *sv0, 2 ), j) )
                            + M_SQR( spc_si(art_gv, ARSV_I( *sv0, 3 ), j) ));
                        
                        c_dop /= spc_si(art_gv, ARSV_I( *sv0, 0 ),j);
                    }
                    
                    spc_set_sid(
                          art_gv,
                          dop,
                          j,
                          c_dop
                        );
                }
                
                double  maxDOP =
                    spc_s_max( art_gv, dop );
                
                if ( maxDOP > thresholdValue )
                {
                    arlight_d_init_unpolarised_l(
                          art_gv,
                          0.0,
                          LIGHTALPHA_DESTINATION_BUFFER_LIGHT(x)
                        );
                }
                else
                {
                    arlight_l_init_l(
                          art_gv,
                          LIGHTALPHA_SOURCE_BUFFER_LIGHT(x),
                          LIGHTALPHA_DESTINATION_BUFFER_LIGHT(x)
                        );
                }

                LIGHTALPHA_DESTINATION_BUFFER_ALPHA(x) =
                    LIGHTALPHA_SOURCE_BUFFER_ALPHA(x);
            }

            [ self writeDestinationScanlineBuffer: i : y ];
        }
    }

    arstokesvector_free( art_gv, sv0 );
    spc_free( art_gv, dop );


    /* ------------------------------------------------------------------
         Free the image manipulation infrastructure and end the action;
         this also places the destination image on the stack.
    ---------------------------------------------------------------aw- */

    [ self finishImageManipulation
        :   nodeStack
        ];

    [ ART_GLOBAL_REPORTER endAction ];
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];

    [ coder codeDouble: & thresholdValue ];
    [ coder codeBOOL  : & cloneOption ];
}

@end


// ===========================================================================



