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

#define ART_MODULE_NAME     ArnSingleImageManipulationAction

#import "ArnSingleImageManipulationAction.h"
#import "ART_ColourAndSpectra.h"
#import "FoundationAssertionMacros.h"
#import "ArnImageManipulationMacros.h"

#import "ARM_Action.h"

//   Uncomment the following #define to see how pathnames are derived

//#define PATHNAME_DEBUGPRINTF

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnSingleImageManipulationAction registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArnSingleImageManipulationAction

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnSingleImageManipulationAction)

- (id) removeSource
        : (BOOL) newRemoveSource
{
    return
        [ self init
            :   newRemoveSource
            ];
}

- (id) init
{
    return
        [ self init
            :   NO
            :   0
            :   0
            ];
}

- (id) init
        : (BOOL) newRemoveSource
{
    return
        [ self init
            :   newRemoveSource
            :   0
            :   0
            ];
}

- (id) init
        : (BOOL) newRemoveSource
        : (ArNode *) newSubnode0
        : (ArNode *) newSubnode1
{
    numberOfDestinationsPerSource = 1;
    useActionSequenceMasterFilenameForDestination = NO;

    self =
        [ super init
            :   HARD_NODE_REFERENCE(newSubnode0)
            :   HARD_NODE_REFERENCE(newSubnode1)
            ];
    
    if ( self )
    {
        deleteSourceImageAfterUse = newRemoveSource;
    }
    
    return self;
}

#define DESTINATION_COLOURSPACE \
((ArnColourSpace *) ARNODEREF_POINTER(subnodeRefArray[0]))

- (void) setRemoveSource
        : (BOOL) newRemoveSource
{
    deleteSourceImageAfterUse = newRemoveSource;
}

- (void) prepareImageTags
{
}

- (void) prepareForImageManipulation
        : (ArNode <ArpNodeStack> *) nodeStack
        : (Class) sourceImageClass
        : (Class) destinationImageClass
{
    [ self prepareForImageManipulation
        :   nodeStack
        :   sourceImageClass
        :   destinationImageClass
        :   1
        ];
}

- (void) prepareForImageManipulation
        : (ArNode <ArpNodeStack> *) nodeStack
        : (Class) sourceImageClass
        : (Class) destinationImageClass
        : (unsigned int) scalingFactor
{
    ArList     sourceImageRefList = ARLIST_EMPTY;
    ArList     otherImageRefList  = ARLIST_EMPTY;
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
            ArnFileImage  * fileImageToAdaptTo = (ArnFileImage <ArpImage> *) ARNODEREF_POINTER(refFromStack);
            if ( [ fileImageToAdaptTo imageFileIsKindOf: sourceImageClass ] )
            {
                arlist_add_noderef_at_tail( & sourceImageRefList, refFromStack );
            }
            else
            {
                arlist_add_noderef_at_tail( & otherImageRefList, refFromStack );
            }
        }
    }
    
    numberOfSourceImages = arlist_length( & sourceImageRefList );

    if ( numberOfSourceImages == 0 )
        ART_ERRORHANDLING_FATAL_ERROR(
            "no images to process found on stack"
            );

    numberOfDestinationImages =
        numberOfSourceImages * numberOfDestinationsPerSource;

    sourceImage =
        ALLOC_ARRAY( ArnFileImage <ArpImage> *, numberOfSourceImages );

    for ( unsigned int i = 0; i < numberOfSourceImages; i++ )
    {
        ArNodeRef  imageRef = ARNODEREF_NONE;
        
        arlist_pop_noderef( & sourceImageRefList, & imageRef );

        sourceImage[i] = (ArnFileImage <ArpImage>*)ARNODEREF_POINTER(imageRef);
    }

    numberOfOtherImages  = arlist_length( & otherImageRefList );

    otherImage =
        ALLOC_ARRAY( ArnFileImage <ArpImage> *, numberOfOtherImages );

    for ( unsigned int i = 0; i < numberOfOtherImages; i++ )
    {
        ArNodeRef  imageRef = ARNODEREF_NONE;
        
        arlist_pop_noderef( & otherImageRefList, & imageRef );

        otherImage[i] = (ArnFileImage <ArpImage>*)ARNODEREF_POINTER(imageRef);
    }

    /* ------------------------------------------------------------------
         We then create the name for the new destination file image.
         This is the same as for the source file, just with the old
         extension stripped off and replaced by the new one.

         Unless source and destination image class are the same (which
         is sometimes the case). Then we add an additional tag to the
         destination path in order to avoid overwriting the source.
    ------------------------------------------------------------------ */

    #ifdef PATHNAME_DEBUGPRINTF
    debugprintf(
        "\nMaster filename         : %s \n"
        ,   [ nodeStack masterOutputFilename ]
        );
    #endif
    
    [ self prepareImageTags ];

    destinationFilename = ALLOC_ARRAY( char *, numberOfDestinationImages );

    for ( unsigned int i = 0; i < numberOfSourceImages; i++ )
    {
        char  * clippedSourceFilename = 0;
        
        //   Using the "fallback to a user specified name" feature
        //   only makes sense for single input images
        
        if (     numberOfSourceImages == 1
             &&  useActionSequenceMasterFilenameForDestination )
        {
            #ifdef PATHNAME_DEBUGPRINTF
            debugprintf(
                "Using application-supplied filename\n"
                );
            #endif

            arstring_s_copy_s(
                  [ nodeStack masterOutputFilename ],
                & clippedSourceFilename
                );
        }
        else
        {
            #ifdef PATHNAME_DEBUGPRINTF
            debugprintf(
                "Using source filename\n"
                );
            #endif
            arstring_p_copy_without_extension_p(
                  [ sourceImage[i] fileName ],
                & clippedSourceFilename
                );
        }
    
        #ifdef PATHNAME_DEBUGPRINTF
        debugprintf(
            "Input filename %03d      : %s \n"
            "Clipped input filename  : %s \n"
            ,   i
            ,   [ sourceImage[i] fileName ]
            ,   clippedSourceFilename
            );
        #endif

        if ( sourceImageClass != destinationImageClass )
        {
            //   If we find that the source filename has a copy tag attached,
            //   and if source and destination filetype are not the same, we
            //   remove the copy tag, and revert to the original name.
            
            ArString  extensionOfClippedSourceFilename = 0;
            
            arstring_sc_copy_rightmost_component_s(
                  clippedSourceFilename,
                  '.',
                & extensionOfClippedSourceFilename
                );

            #ifdef PATHNAME_DEBUGPRINTF
            debugprintf(
                "Extension of clipped input filename: %s \n",
                extensionOfClippedSourceFilename
                );
            debugprintf(
                "ART image operation copy tag       : %s \n",
                ART_IMAGEFILE_COPY_TAG
                );
            #endif
            
            BOOL  inputImageHasCopyTag =
                ! strcmp( extensionOfClippedSourceFilename,
                          ART_IMAGEFILE_COPY_TAG );

            FREE(extensionOfClippedSourceFilename);

            if ( inputImageHasCopyTag )
            {
                #ifdef PATHNAME_DEBUGPRINTF
                debugprintf(
                    "Changing clipped input file name to untagged version\n"
                    );
                #endif
                
                ArString  temp;
                
                arstring_s_copy_s( clippedSourceFilename, & temp );
                
                FREE(clippedSourceFilename);
                
                arstring_p_copy_without_extension_p(
                      temp,
                    & clippedSourceFilename
                    );

                #ifdef PATHNAME_DEBUGPRINTF
                debugprintf(
                    "New clipped input filename: %s \n",
                    clippedSourceFilename
                    );
                #endif
            }
        }

        //   Case one: we have multiple destinations per source
        
        //   Here, we do not worry about overriding any source images,
        //   as all destinations get tagged anyway
        
        if ( numberOfDestinationsPerSource > 1 )
        {
            for ( unsigned int j = 0; j < numberOfDestinationsPerSource; j++ )
            {
                arstring_pe_copy_add_extension_p(
                      clippedSourceFilename,
                      destinationFilenameTag[j],
                    & destinationFilename[ i * numberOfDestinationsPerSource + j ]
                    );
                
                arstring_e_add_extension_p(
                      [ destinationImageClass fileExtension ],
                    & destinationFilename[ i * numberOfDestinationsPerSource + j ]
                    );

                #ifdef PATHNAME_DEBUGPRINTF
                debugprintf(
                    "Output file %03d/%03d (%d)     : %s\n"
                    ,   i
                    ,   j
                    ,   i * numberOfDestinationsPerSource + j
                    ,   destinationFilename[ i * numberOfDestinationsPerSource + j ]
                    );
                #endif
            }
        }
        else
        {
            //   Case two: one to one relationship source <-> destination
            //   Here, we have to be careful: if the file formats match,
            //   we have to add a temporary tag to the name, to avoid
            //   overwriting the source *while we are working*.
            
            if ( sourceImageClass == destinationImageClass )
            {
                /* ------------------------------------------------------------
                    Bad variable naming: 'clippedSourceFilename' should
                    probably be called something different, as it can be
                    either derived from the actual source filename, or from
                    the master filename specified via the -o option.
                    
                    As we need a check against the clipped version of the 
                    actual source file name here, we have to do this again.
                    Not pretty, but works.
                ------------------------------------------------------------ */
                
                ArString  clippedInputFilename;
                
                arstring_p_copy_without_extension_p(
                      [ sourceImage[i] fileName ],
                    & clippedInputFilename
                    );

                //   Logic: we only tag the image if the two filenames actually
                //   match - otherwise, there is no danger of overwriting
                //   anything.

                if (    useActionSequenceMasterFilenameForDestination
                     && strcmp(clippedSourceFilename, clippedInputFilename) )
                {
                    tagWasAddedToDestinationFilename = NO;

                    arstring_pe_copy_add_extension_p(
                          clippedSourceFilename,
                          [ destinationImageClass fileExtension ],
                        & destinationFilename[i]
                        );
                }
                else
                {
                    tagWasAddedToDestinationFilename = YES;

                    arstring_pe_copy_add_extension_p(
                          clippedSourceFilename,
                          ART_IMAGEFILE_COPY_TAG,
                        & destinationFilename[i]
                        );

                    arstring_e_add_extension_p(
                          [ destinationImageClass fileExtension ],
                        & destinationFilename[i]
                        );
                }
            }
            else
            {
                tagWasAddedToDestinationFilename = NO;
                
                if ( destinationFilenameTag )
                {
                    arstring_pe_copy_add_extension_p(
                          clippedSourceFilename,
                          destinationFilenameTag[0],
                        & destinationFilename[i]
                        );

                    arstring_e_add_extension_p(
                          [ destinationImageClass fileExtension ],
                        & destinationFilename[i]
                        );
                }
                else
                {
                    arstring_pe_copy_add_extension_p(
                          clippedSourceFilename,
                          [ destinationImageClass fileExtension ],
                        & destinationFilename[i]
                        );
                }
            }
            
            #ifdef PATHNAME_DEBUGPRINTF
            debugprintf(
                "Output file %03d         : %s\n"
                ,   i
                ,   destinationFilename[ i ]
                );
            #endif
        }
        
        FREE_ARRAY(clippedSourceFilename);
    }

    /* ------------------------------------------------------------------
         Next we create and fill an image buffer of the specified source
         type (which is NOT exactly the same type as of the file image we
         took from the stack) that holds the entire source image, plus a
         smaller buffer of the same type which can hold a single
         scanline.
    ---------------------------------------------------------------aw- */

    //   Assumption in all this: all source images are the same size,
    //   and of the same type.
    
    sourceImageSize = [ sourceImage[0] size ];

    if ( scalingFactor > 1 )
    {
        if ( scalingFactor > 1000 )
        {
            ART_ERRORHANDLING_WARNING(
                "extremely large image downscale factor %d",
                scalingFactor
                );
        }
        
        ivec2d_dv_mul_v(
              1.0 / scalingFactor,
            & sourceImageSize,
            & destinationImageSize
            );
    }
    else
    {
        if ( scalingFactor < 1 )
        {
            ART_ERRORHANDLING_WARNING(
                "invalid downscale factor %d",
                scalingFactor
                );
        }

        destinationImageSize = sourceImageSize;
    }

    if ( extensionXC > 0 )
        XC(destinationImageSize) = XC(sourceImageSize) + extensionXC;

    Class  sourceImageBufferClass =
        [ sourceImage[0] nativeContentClass ];

#ifdef PATHNAME_DEBUGPRINTF
    debugprintf(
        "Buffer class: '%s' \n"
        ,   [ [sourceImage[0] nativeContentClass ] cStringClassName ]
        );
#endif

    sourceImageBuffer = ALLOC_ARRAY( ArNode *, numberOfSourceImages );
    
    for ( unsigned int i = 0; i < numberOfSourceImages; i++ )
    {
        sourceImageBuffer[i] =
            (ArNode *)
            [ ALLOC_OBJECT_BY_CLASS(
                sourceImageBufferClass,
                ArpPlainImageSimpleMemory
                )
                initWithSize
                :   sourceImageSize
                ];

        //   If we cast the pointer, an assertion is in order afterwards.

        ASSERT_CLASS_OR_SUBCLASS_MEMBERSHIP(
            sourceImageBuffer[i],
            ArNode
            );

        [ sourceImage[i] getPlainImage
            :   IPNT2D( 0, 0 )
            :   ((ArnPlainImage *)sourceImageBuffer[i])
            ];
    }

#ifdef PATHNAME_DEBUGPRINTF
    debugprintf(
        "Allocating source scanline buffer length %d \n"
        ,   XC(sourceImageSize)
        );
#endif

    numberOfSourceScanlineBuffers = scalingFactor;

    sourceScanlineBuffer =
        ALLOC_ARRAY( ArNode *, numberOfSourceScanlineBuffers );
    
    for ( unsigned int i = 0; i < numberOfSourceScanlineBuffers; i++ )
    {
        sourceScanlineBuffer[i] =
            (ArNode *)
            [ ALLOC_OBJECT_BY_CLASS(
                sourceImageBufferClass,
                ArpPlainImageSimpleMemory
                )
                initWithSize
                :   IVEC2D(XC(sourceImageSize), 1)
                ];
            
        ASSERT_CLASS_OR_SUBCLASS_MEMBERSHIP(
            sourceScanlineBuffer[i],
            ArNode
            );
    }
    
    /* ------------------------------------------------------------------
         Finally, we create the output file image (based on the
         specifications taken from the source), and also a matching
         single scanline buffer of suitable type.
    ------------------------------------------------------------------ */

    destinationImageInfo =
        [ ALLOC_INIT_OBJECT(ArnImageInfo)
            :   destinationImageSize
            :   destinationImageDataType
            :   destinationFileDataType
            :   [ sourceImage[0] resolution ]
            :   DESTINATION_COLOURSPACE
            ];

    destinationImage =
        ALLOC_ARRAY(ArnFileImage <ArpImage> *, numberOfDestinationImages);

    #ifdef PATHNAME_DEBUGPRINTF
        debugprintf(
            "Creating %d destination images\n"
            ,   numberOfDestinationImages
            );
    #endif
    
    for ( unsigned int i = 0; i < numberOfDestinationImages; i++ )
    {
        #ifdef PATHNAME_DEBUGPRINTF
        debugprintf(
            "Creating destination image %d of %d: '%s'"
            ,   i
            ,   numberOfDestinationImages
            ,   destinationFilename[i]
            );
        #endif
        destinationImage[i] =
            [ ALLOC_INIT_OBJECT(ArnFileImage)
                :   destinationFilename[i]
                :   destinationImageClass
                :   destinationImageInfo
                ];

        #ifdef PATHNAME_DEBUGPRINTF
            debugprintf( ".\n" );
        #endif
    }

    Class  destinationImageBufferClass =
        [ destinationImage[0] nativeContentClass ];

#ifdef PATHNAME_DEBUGPRINTF
    debugprintf(
        "Allocating destination scanline buffer length %d \n"
        ,   XC(destinationImageSize)
        );
#endif
    destinationScanlineBuffer =
        (ArNode *)
        [ ALLOC_OBJECT_BY_CLASS(
            destinationImageBufferClass,
            ArpPlainImageSimpleMemory
            )
            initWithSize
            :   IVEC2D(XC(destinationImageSize), 1)
            ];

    ASSERT_CLASS_OR_SUBCLASS_MEMBERSHIP(
        destinationScanlineBuffer,
        ArNode
        );

#ifdef PATHNAME_DEBUGPRINTF
    debugprintf(
        "Destination buffer class: '%s' \n"
        ,   [ [destinationScanlineBuffer class ] cStringClassName ]
        );
#endif
}

- (unsigned int) loadSourceScanlineBuffer
        : (unsigned int) imageNumber
        : (unsigned int) scanline
{
    unsigned int  validScanlines = 0;
    
    for ( unsigned int i = 0; i < numberOfSourceScanlineBuffers; i++ )
    {
        const int  thisScanline = scanline + i;
        
        if ( thisScanline < YC(sourceImageSize) )
        {
            [ ((id <ArpGetPlainImage>)sourceImageBuffer[imageNumber])
                getPlainImage
                :   IPNT2D( 0, thisScanline )
                :   ((ArnPlainImage *)(sourceScanlineBuffer[i]))
                ];
            
            validScanlines++;
        }
    }
    
    return validScanlines;
}

- (void) writeDestinationScanlineBuffer
        : (unsigned int) imageNumber
        : (unsigned int) scanline
{
    [ destinationImage[imageNumber] setPlainImage
        :   IPNT2D( 0, scanline )
        :   ((ArnPlainImage *)destinationScanlineBuffer)
        ];
}

- (void) finishImageManipulation
        : (ArNode <ArpNodeStack> *) nodeStack
{
    if ( deleteSourceImageAfterUse )
    {
        //   Delete the source image file

        for ( unsigned int i = 0; i < numberOfSourceImages; i++ )
        {
            #ifdef PATHNAME_DEBUGPRINTF
            debugprintf(
                "Removing file %s\n"
                ,   [ sourceImage[i] fileName ]
                );
            #endif
            remove( [ sourceImage[i] fileName ] );
        }

        //   If both the source and destination image classes were the
        //   same we added a tag to the destination path to make it
        //   different from the source. Now that the source is gone we
        //   can rename the destination back to the original name.

        if ( tagWasAddedToDestinationFilename )
        {
            for ( unsigned int i = 0; i < numberOfSourceImages; i++ )
            {
                #ifdef PATHNAME_DEBUGPRINTF
                debugprintf(
                    "Changing name of image #0 back to untagged version\n"
                    );
                debugprintf("Orig : %s \n",[ sourceImage[i] fileName ]);
                debugprintf("Prev : %s \n",[ destinationImage[i] fileName ]);
                #endif
                [ destinationImage[i] changeFileNameTo
                    :   [ sourceImage[i] fileName ]
                    ];

                #ifdef PATHNAME_DEBUGPRINTF
                debugprintf("After: %s \n",[ destinationImage[i] fileName ]);
                #endif
            }
        }
    }
    #ifdef PATHNAME_DEBUGPRINTF
    else
    {
        for ( int i = 0; i < numberOfSourceImages; i++ )
            debugprintf(
                "Keeping source image %s\n"
                ,   [ sourceImage[i] fileName ]
                );
    }
    #endif

    //   Put the destination image on the stack

    for ( unsigned int i = 0; i < numberOfDestinationImages; i++ )
    {
        #ifdef PATHNAME_DEBUGPRINTF
        debugprintf(
            "Pushing destination image %s\n"
            ,   [ destinationImage[i] fileName ]
            );
        #endif
        [ nodeStack push
            :   HARD_NODE_REFERENCE(destinationImage[i])
            ];
    }
    
    if ( returnSourceImagesToStack )
    {
        [ nodeStack push
            :   WEAK_NODE_REFERENCE(NOP_ACTION_SINGLETON)
            ];

        for ( unsigned int i = 0; i < numberOfSourceImages; i++ )
        {
            #ifdef PATHNAME_DEBUGPRINTF
            debugprintf(
                "Pushing source image %s\n"
                ,   [ sourceImage[i] fileName ]
                );
            #endif
            [ nodeStack push
                :   HARD_NODE_REFERENCE(sourceImage[i])
                ];
        }
    }

    for ( unsigned int i = 0; i < numberOfOtherImages; i++ )
    {
        #ifdef PATHNAME_DEBUGPRINTF
        debugprintf(
            "Pushing other image %s\n"
            ,   [ otherImage[i] fileName ]
            );
        #endif
        [ nodeStack push
            :   HARD_NODE_REFERENCE(otherImage[i])
            ];
    }

    #ifdef PATHNAME_DEBUGPRINTF
    printf("Finished with 'finishImageManipulation'\n");
    #endif
}

- (void)dealloc
{
    //   Free temporary data structures

    #ifdef PATHNAME_DEBUGPRINTF
    printf("Releasing/freeing images, buffers and strings.\n");
    #endif

    RELEASE_OBJECT(destinationImageInfo);

    for ( unsigned int i = 0; i < numberOfSourceScanlineBuffers; i++ )
    {
        RELEASE_OBJECT(sourceScanlineBuffer[i]);
    }
    
    FREE_ARRAY(sourceScanlineBuffer);

    RELEASE_OBJECT(destinationScanlineBuffer);

    for ( unsigned int i = 0; i < numberOfDestinationImages; i++ )
        FREE_ARRAY(destinationFilename[i]);

    FREE_ARRAY(destinationFilename);

    if ( destinationFilenameTag )
    {
        for ( unsigned int i = 0; i < numberOfDestinationsPerSource; i++ )
            FREE_ARRAY(destinationFilenameTag[i]);

        FREE_ARRAY(destinationFilenameTag);
    }

    for ( unsigned int i = 0; i < numberOfSourceImages; i++ )
    {
        RELEASE_OBJECT(sourceImage[i]);
        RELEASE_OBJECT(sourceImageBuffer[i]);
    }
    
    FREE_ARRAY(sourceImage);
    FREE_ARRAY(sourceImageBuffer);
    FREE_ARRAY(otherImage);

    for ( unsigned int i = 0; i < numberOfDestinationImages; i++ )
        RELEASE_OBJECT(destinationImage[i]);
    
    FREE_ARRAY(destinationImage);

    [ super dealloc ];
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];

    [ coder codeUInt: & numberOfDestinationsPerSource ];
    [ coder codeBOOL: & deleteSourceImageAfterUse ];
    [ coder codeBOOL: & useActionSequenceMasterFilenameForDestination ];
}

@end


// ===========================================================================



