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

#define ART_MODULE_NAME     ArnDualImageManipulationAction

#import "ArnDualImageManipulationAction.h"
#import "ART_ColourAndSpectra.h"
#import "FoundationAssertionMacros.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnDualImageManipulationAction registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


//#define PATHNAME_DEBUGPRINTF

@implementation ArnDualImageManipulationAction

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnDualImageManipulationAction)

- (id) init
{
    return
        [ self init
            :   0
            :   0
            :   0
            :   0
            ];
}

- (id) init
        : (const char *) newOutputName
        : (const char *) newOutputTag
{
    return
        [ self init
            :   newOutputName
            :   newOutputTag
            :   0
            :   0
            ];
}

- (id) init
        : (const char *) newOutputName
        : (const char *) newOutputTag
        : (ArNode *) newSubnode0
        : (ArNode *) newSubnode1
{
    self =
        [ super init
            :   HARD_NODE_REFERENCE(newSubnode0)
            :   HARD_NODE_REFERENCE(newSubnode1)
            ];
    
    if ( self )
    {
        outputFilename = newOutputName;
        outputTag      = newOutputTag;
    }
    
    return self;
}

#define DESTINATION_COLOURSPACE \
((ArnColourSpace *) ARNODEREF_POINTER(subnodeRefArray[0]))

- (void) prepareForImageManipulation
        : (ArNode <ArpNodeStack> *) nodeStack
        : (Class) sourceImageClass
{
    [ self prepareForImageManipulation
        :   nodeStack
        :   sourceImageClass
        :   0
        ];
}

- (void) prepareForImageManipulation
        : (ArNode <ArpNodeStack> *) nodeStack
        : (Class) sourceImageClass
        : (Class) destinationImageClass
{
    /* ------------------------------------------------------------------
         First, we take the original file image from the stack, and
         check whether it is what we expect (i.e. an instance of the
         class we were told to expect).
    ---------------------------------------------------------------aw- */

    sourceImageA_Ref = [ nodeStack pop ];

    ASSERT_EXACT_CLASS_MEMBERSHIP(
        ARNODEREF_POINTER(sourceImageA_Ref),
        ArnFileImage
        );

    ASSERT_PROTOCOL_SUPPORT(
        ARNODEREF_POINTER(sourceImageA_Ref),
        ArpImage
        );

    sourceImageA =
        (ArnFileImage <ArpImage>*)ARNODEREF_POINTER(sourceImageA_Ref);


    if ( ! [ sourceImageA imageFileIsKindOf: sourceImageClass ] )
        ART_ERRORHANDLING_FATAL_ERROR(
            "wrong type of object found on stack - "
            "%s instead of %s"
            ,   [ [ sourceImageA dataImageClass ] cStringClassName ]
            ,   [ sourceImageClass cStringClassName ]
            );

    sourceImageB_Ref = [ nodeStack pop ];

    ASSERT_EXACT_CLASS_MEMBERSHIP(
        ARNODEREF_POINTER(sourceImageB_Ref),
        ArnFileImage
        );

    ASSERT_PROTOCOL_SUPPORT(
        ARNODEREF_POINTER(sourceImageB_Ref),
        ArpImage
        );

    sourceImageB =
        (ArnFileImage <ArpImage>*)ARNODEREF_POINTER(sourceImageB_Ref);

    if ( ! [ sourceImageB imageFileIsKindOf: sourceImageClass ] )
        ART_ERRORHANDLING_FATAL_ERROR(
            "wrong type of object found on stack - "
            "%s instead of %s"
            ,   [ [ sourceImageB dataImageClass ] cStringClassName ]
            ,   [ sourceImageClass cStringClassName ]
            );

    /* ------------------------------------------------------------------
         We then create the name for the new destination file image.
         This is the same as for the source file, just with the old
         extension stripped off and replaced by the new one.

         Unless source and destination image class are the same (which
         is sometimes the case). Then we add an additional tag to the
         destination path in order to avoid overwriting the source.
    ------------------------------------------------------------------ */

    transientTagWasAddedToDestinationFilename = NO;
    hasDestinationImage = NO;

    destinationFilename = 0;
    
    if (destinationImageClass)
    {
        hasDestinationImage = YES;
        if ( outputFilename )
        {
            arstring_pe_copy_add_extension_p(
                  outputFilename,
                  [ destinationImageClass fileExtension ],
                & destinationFilename
                );
        }
        else
        {
            if ( outputTag )
            {
                arstring_psss_copy_add_tag_swap_extension_p(
                      [ sourceImageB fileName ],
                      [ sourceImageClass fileExtension ],
                      outputTag,
                      [ destinationImageClass fileExtension ],
                    & destinationFilename
                    );
            }
            else
            {
                transientTagWasAddedToDestinationFilename = YES;
                
                if ( sourceImageClass == destinationImageClass )
                {
                    arstring_psss_copy_add_tag_swap_extension_p(
                          [ sourceImageA fileName ],
                          [ sourceImageClass fileExtension ],
                          "copy.diff",
                          [ destinationImageClass fileExtension ],
                        & destinationFilename
                        );
                }
                else
                {
                    arstring_psss_copy_add_tag_swap_extension_p(
                          [ sourceImageA fileName ],
                          [ sourceImageClass fileExtension ],
                          "diff",
                          [ destinationImageClass fileExtension ],
                        & destinationFilename
                        );
                }
            }
        }
    }

    #ifdef PATHNAME_DEBUGPRINTF
    if ( transientTagWasAddedToDestinationFilename )
    {
        printf("\nNew filename with tag\n");
        printf("In : %s \n",[ sourceImageA fileName ]);fflush(stdout);
        printf("In : %s \n",[ sourceImageB fileName ]);fflush(stdout);
        printf("Out: %s \n",destinationFilename);fflush(stdout);
    }
    else
    {
        printf("\nNew filename without tag\n");
        printf("In : %s \n",[ sourceImageA fileName ]);fflush(stdout);
        printf("In : %s \n",[ sourceImageB fileName ]);fflush(stdout);
        printf("Out: %s \n",destinationFilename);fflush(stdout);
    }
    #endif

    /* ------------------------------------------------------------------
         Next we create and fill an image buffer of the specified source
         type (which is NOT exactly the same type as of the file image we
         took from the stack) that holds the entire source image, plus a
         smaller buffer of the same type which can hold a single
         scanline.
    ---------------------------------------------------------------aw- */

    originalImageSize = [ sourceImageA size ];

    imageSize = originalImageSize;

    if ( extensionXC > 0 )
        XC(imageSize) = XC(originalImageSize) + extensionXC;

    Class  sourceImageBufferClass =
        [ sourceImageA nativeContentClass ];

#ifdef PATHNAME_DEBUGPRINTF
            printf("Buffer class: '%s' \n",[  [sourceImageA nativeContentClass ] cStringClassName ]);fflush(stdout);
            printf("Buffer class: '%s' \n",[  [sourceImageB nativeContentClass ] cStringClassName ]);fflush(stdout);
#endif

    sourceImageBufferA =
        (ArNode *)
        [ ALLOC_OBJECT_BY_CLASS(sourceImageBufferClass,ArpPlainImageSimpleMemory)
            initWithSize
            :   originalImageSize
            ];

    //   If we cast the pointer, an assertion is in order afterwards.

    ASSERT_CLASS_OR_SUBCLASS_MEMBERSHIP(
        sourceImageBufferA,
        ArNode
        );

    [ sourceImageA getPlainImage
        :   IPNT2D( 0, 0 )
        :   ((ArnPlainImage *)sourceImageBufferA)
        ];

    sourceScanlineBufferA =
        (ArNode *)
        [ ALLOC_OBJECT_BY_CLASS(
            sourceImageBufferClass,
            ArpPlainImageSimpleMemory
            )
            initWithSize
            :   IVEC2D(XC(originalImageSize), 1)
            ];

    ASSERT_CLASS_OR_SUBCLASS_MEMBERSHIP(
        sourceScanlineBufferA,
        ArNode
        );

    sourceImageBufferB =
        (ArNode *)
        [ ALLOC_OBJECT_BY_CLASS(
            sourceImageBufferClass,
            ArpPlainImageSimpleMemory
            )
            initWithSize
            :   originalImageSize
            ];

    [ sourceImageB getPlainImage
        :   IPNT2D( 0, 0 )
        :   ((ArnPlainImage *)sourceImageBufferB)
        ];

    sourceScanlineBufferB =
        (ArNode *)
        [ ALLOC_OBJECT_BY_CLASS(
            sourceImageBufferClass,
            ArpPlainImageSimpleMemory
            )
            initWithSize
            :   IVEC2D(XC(originalImageSize), 1)
            ];

    ASSERT_CLASS_OR_SUBCLASS_MEMBERSHIP(
        sourceScanlineBufferB,
        ArNode
        );

    if (hasDestinationImage)
    {
        /* ------------------------------------------------------------------
             Finally, we create the output file image (based on the
             specifications taken from the source), and also a matching
             single scanline buffer of suitable type.
        ------------------------------------------------------------------ */

        destinationImageInfo =
            [ ALLOC_INIT_OBJECT(ArnImageInfo)
                :   imageSize
                :   destinationImageDataType
                :   destinationFileDataType
                :   [ sourceImageA resolution ]
                :   DESTINATION_COLOURSPACE
                ];

    #ifdef PATHNAME_DEBUGPRINTF
        debugprintf(
            "Creating destination image '%s', colourtype %s, filetype %s \n",
            destinationFilename,
            ardatatype_name(destinationImageDataType),
            ardatatype_name(destinationFileDataType)
            );
    #endif
        destinationImage =
            [ ALLOC_INIT_OBJECT(ArnFileImage)
                :   destinationFilename
                :   destinationImageClass
                :   destinationImageInfo
                ];

    #ifdef PATHNAME_DEBUGPRINTF
        debugprintf(
            "Destination image class: '%s' \n",
            [  [destinationImage class ] cStringClassName ]
            );
        debugprintf("Done \n");
    #endif

        Class  destinationImageBufferClass =
            [ destinationImage nativeContentClass ];

        destinationScanlineBuffer =
            (ArNode *)
            [ ALLOC_OBJECT_BY_CLASS(
                destinationImageBufferClass,
                ArpPlainImageSimpleMemory
                )
                initWithSize
                :   IVEC2D(XC(imageSize), 1)
                ];

    #ifdef PATHNAME_DEBUGPRINTF
                printf("Destination buffer class: '%s' \n",[  [destinationScanlineBuffer class ] cStringClassName ]);fflush(stdout);
    #endif
    }
}

- (void) loadSourceScanlineBuffers
        : (unsigned int) scanline
{
    [ ((ArNode <ArpGetPlainImage> *)sourceImageBufferA) getPlainImage
        :   IPNT2D( 0, scanline )
        :   ((ArnPlainImage *)sourceScanlineBufferA) ];
    [ ((ArNode <ArpGetPlainImage> *)sourceImageBufferB) getPlainImage
        :   IPNT2D( 0, scanline )
        :   ((ArnPlainImage *)sourceScanlineBufferB) ];
}

- (void) writeDestinationScanlineBuffer
        : (unsigned int) scanline
{
    [ destinationImage setPlainImage
        :   IPNT2D( 0, scanline )
        :   ((ArnPlainImage *)destinationScanlineBuffer) ];
}

- (void) freeActionDatastructures
{
    #ifdef PATHNAME_DEBUGPRINTF
    printf("Releasing/freeing images, buffers and strings.\n");
    #endif

    //   Free temporary data structures
    RELEASE_OBJECT( sourceImageBufferA );
    RELEASE_OBJECT( sourceScanlineBufferA );
    RELEASE_OBJECT( sourceImageBufferB );
    RELEASE_OBJECT( sourceScanlineBufferB );

    if (hasDestinationImage)
    {
        RELEASE_OBJECT( destinationScanlineBuffer );
        FREE_ARRAY( destinationFilename );
    }
}

- (void) finishImageManipulation
        : (ArNode <ArpNodeStack> *) nodeStack
{
    //   Put the destination image on the stack

    [ nodeStack push
        :   HARD_NODE_REFERENCE(destinationImage)
        ];

    //   If we added a tag to the result, this is currently (!) one of the
    //   cases where we can delete the source images afterwards.
    
    if ( outputTag )
    {
        //   Delete the source image file

            #ifdef PATHNAME_DEBUGPRINTF
            debugprintf(
                "Removing source file A %s\n"
                ,   [ sourceImageA fileName ]
                );
            debugprintf(
                "Removing source file B %s\n"
                ,   [ sourceImageB fileName ]
                );
            #endif
//            remove( [ sourceImageA fileName ] );
//            remove( [ sourceImageB fileName ] );
    }

    [ self freeActionDatastructures ];

#ifdef PATHNAME_DEBUGPRINTF
    printf("\nFinished with 'finishImageManipulation'\n");
#endif
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];
}

@end


// ===========================================================================



