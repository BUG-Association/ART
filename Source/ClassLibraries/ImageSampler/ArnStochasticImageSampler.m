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

#define ART_MODULE_NAME     ArnStochasticImageSampler

#import "ArnStochasticImageSampler.h"
#import "ArnImageSamplerCommonMacros.h"

#import "ArnReconstructionKernel.h"
#import "FoundationAssertionMacros.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    [ ArnStochasticImageSampler registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

/* ===========================================================================
    'ArnStochasticImageSampler'
=========================================================================== */

//   k - number of sample
//   i - number of splatting kernel pixel

#define SAMPLE_SPLATTING_FACTOR( __k, __i ) \
    sampleSplattingFactor[ (__k) * splattingKernelArea + (__i) ]

#define SAMPLE_SPLATTING_FACTOR_UV( __k, __u, __v ) \
    SAMPLE_SPLATTING_FACTOR( __k, (__u) * splattingKernelWidth + (__v) )

@implementation ArnStochasticImageSampler

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnStochasticImageSampler)

- init
        : (ArNode <ArpPathspaceIntegrator> * ) newPathspaceIntegrator
        : (ArNode <ArpReconstructionKernel> *) newReconstructionKernel
        : (unsigned int) newNumberOfSamples
        : (int) newRandomValueGeneration
{
    self =
        [ super init
            :   newPathspaceIntegrator
            :   newReconstructionKernel
            :   newNumberOfSamples
            :   newRandomValueGeneration
            ];

    deterministicWavelengths = NO;
    wavelengthSteps = 1;
    
    return self;
}

- (void) useDeterministicWavelengths
{
    deterministicWavelengths = YES;
    wavelengthSteps = spc_channels(art_gv);
    art_set_hero_samples_to_splat( art_gv, 1 );
}

- (void) prepareForSampling
        : (ArNode <ArpWorld> *) newWorld
        : (ArNode <ArpCamera> *) newCamera
        : (ArNode <ArpImageWriter> **) image
        : (int) numberOfResultImages
{
    [ super prepareForSampling
        :   newWorld
        :   newCamera
        :   image
        :   numberOfResultImages
        ];

    //   splatting kernel properties

    splattingKernelWidth  = [ RECONSTRUCTION_KERNEL supportSize ];
    splattingKernelWidthDiv2 = splattingKernelWidth / 2;
    splattingKernelArea   = M_SQR( splattingKernelWidth );
    splattingKernelOffset = (splattingKernelWidth - 1) / 2;

    //   2D sample coordinates are pre-generated for the entire packet

    //   CAVEAT: they are the same for all packets - but given the huge
    //           packet sizes, this should never really be an issue!
    //           And as long as you cast less than DEFAULT_PACKET_SIZE
    //           rays, everything is as expected anyway - each ray has
    //           unique 2D subpixel coordinates in that case.

    numberOfSubpixelSamples =
        M_MIN( IMAGE_SAMPLER_MAX_SUBPIXEL_SAMPLES, numberOfSamplesPerThread);
    
    sampleCoord = ALLOC_ARRAY( Pnt2D, numberOfSubpixelSamples );

    //   Actual generation of the 2D sample coordinates

    //   Note that we reset the random generator sequence counter for each
    //   2D coordinate - they all should be from the first two sequences the
    //   generator has to offer!

    for ( unsigned int i = 0; i < numberOfSubpixelSamples; i++ )
    {
        [ randomGenerator[0] resetSequenceIDs ];

        [ randomGenerator[0] getValuesFromNewSequences
            : & XC( sampleCoord[i] )
            : & YC( sampleCoord[i] )
            ];
    }

    //   We remember the sequence ID of the generator after it did the last
    //   coordinate - rendering always has to start there!

    startingSequenceID = [ randomGenerator[0] currentSequenceID ];

    //   Sample splatting factors: these are the contribution factors for each
    //   pre-computed 2D sample coordinate. They do not change between pixels,
    //   so pre-computing the splatting kernel influence for each of them
    //   is faster than re-evaluating this every time a sample gets splatted.

    if ( splattingKernelWidth > 1 )
    {
        sampleSplattingFactor =
            ALLOC_ARRAY( double, splattingKernelArea * numberOfSubpixelSamples );

        for ( unsigned int i = 0; i < numberOfSubpixelSamples; i++ )
        {
            for ( unsigned int u = 0; u < splattingKernelWidth; u++ )
            {
                double  dY = 1.0 * u - splattingKernelOffset;

                for ( int v = 0; v < splattingKernelWidth; v++ )
                {
                    double  dX = 1.0 * v - splattingKernelOffset;

                    Pnt2D  localCoord;

                    XC(localCoord) = XC(sampleCoord[i]) - dX - 0.5;
                    YC(localCoord) = YC(sampleCoord[i]) - dY - 0.5;

                    SAMPLE_SPLATTING_FACTOR_UV( i, u, v ) =
                        [ RECONSTRUCTION_KERNEL valueAt
                            : & localCoord
                            ];

                } // splatting kernel v coord for loop
            } // splatting kernel u coord for loop
        } // i - number of samples per packet

        sampleSplattingOffset =
            ALLOC_ARRAY( IPnt2D, splattingKernelArea );

        for ( unsigned int u = 0; u < splattingKernelWidth; u++ )
            for ( int v = 0; v < splattingKernelWidth; v++ )
            {
                XC( sampleSplattingOffset[ u * splattingKernelWidth + v ] )
                    = u - splattingKernelOffset;
                YC( sampleSplattingOffset[ u * splattingKernelWidth + v ] )
                    = v - splattingKernelOffset;
            }
    }
}

- copy
{
    ArnStochasticImageSampler  * copiedInstance = [ super copy ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnStochasticImageSampler  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

//   Purpose of this critter: to pack pixel coordinates and thread ID into
//   one structure, so that crc32 can initialise the thread random generator
//   to a unique and repeatable value that is based both on pixel coordinates
//   and thread ID.

typedef struct ArPixelID
{
    long   globalRandomSeed;
    int    threadIndex;
    int    sampleIndex;
    Pnt2D  pixelCoord;
}
ArPixelID;


- (void) renderProc
        : (ArcInteger *) threadIndex
{
    //   Autorelease pool for this thread to keep Cocoa happy

    NSAutoreleasePool  * threadPool;

    threadPool = [ [ NSAutoreleasePool alloc ] init ];

    //   The values for individual samples, obtained via the gathering estimator
    
    //   We only need an array of pointers, as the actual gathering results
    //   are grabbed from, and returned to, a freelist.

    ArPathspaceResult  ** sampleValue =
        ALLOC_ARRAY( ArPathspaceResult *, numberOfImagesToWrite );
    
    /* ------------------------------------------------------------------
         We render scanlines until there is no unrendered scanline left.
    ------------------------------------------------------------------ */
    
    ArPixelID  px_id;
    
    px_id.threadIndex = THREAD_INDEX;
    px_id.globalRandomSeed = arrandom_global_seed(art_gv);
    
    int   threadStripWidth = YC(imageSize) / numberOfRenderThreads;
    int   threadOffset = threadStripWidth * THREAD_INDEX;

    for ( unsigned int i = 0; i < numberOfSamplesPerThread; i++ )
    {
        int  subpixelIdx = i % numberOfSubpixelSamples;
        
        px_id.sampleIndex = i;

        for ( int ylv = 0; ylv < YC(imageSize); ylv++ )
        {
            int  y = ylv + threadOffset;
            
            if ( y >= YC(imageSize) )
            {
                y = y - YC(imageSize);
            }
            
            YC(px_id.pixelCoord) = y + YC(imageOrigin);

            /* -------------------------------------------------------------
                 We render each pixel of the scanline.
            ------------------------------------------------------------- */

            for ( int x = x_start[y]; x < x_end[y]; x++ )
            {
                if ( renderThreadsShouldTerminate )
                {
                    [ self renderProcHasFinished: threadIndex ];

                    FREE_ARRAY(sampleValue);
                    
                    return;
                }
                
                XC(px_id.pixelCoord) = x + XC(imageOrigin);

                for ( int w = 0; w < wavelengthSteps; w++ )
                {
                    [ THREAD_RANDOM_GENERATOR reInitializeWith
                        :   crc32_of_data( & px_id, sizeof(ArPixelID) )
                        ];

                    /* --------------------------------------------------------------
                        We double-check whether a given sample should be
                        included: first the validity of the ray is checked (rays are
                        always valid for normal cameras, but e.g. fisheye cameras
                        have pixels which lie outside the imaged area) and secondly
                        all rays which do not contain plausible radiance information
                        (all components > 0) are simply not used.

                        The latter check should not be necessary in a perfect world,
                        but in reality malformed mesh data and other gremlins can
                        lead to artefacts if this is not looked after.
                    ------------------------------------------------------------aw- */

                    BOOL  validSample = FALSE;

                    [ THREAD_RANDOM_GENERATOR setCurrentSequenceID
                        :  startingSequenceID
                        ];

                    Ray3D              ray;
                    ArReferenceFrame   referenceFrame;
                    ArWavelength       wavelength;
                    
                    if ( deterministicWavelengths )
                    {
                        arwavelength_i_deterministic_init_w(
                              art_gv,
                              w,
                            & wavelength
                            );
                    }
                    else
                    {
                        arwavelength_sd_init_w(
                              art_gv,
                            & spectralSamplingData,
                              [ THREAD_RANDOM_GENERATOR valueFromNewSequence ],
                            & wavelength
                            );
                    }
                    
                    if ( [ camera getWorldspaceRay
                             : & VEC2D(
                                    XC(px_id.pixelCoord) + XC(sampleCoord[subpixelIdx]),
                                    YC(px_id.pixelCoord) + YC(sampleCoord[subpixelIdx])
                                    )
                             :   THREAD_RANDOM_GENERATOR
                             : & referenceFrame
                             : & ray
                             ] )
                    {
                        [ THREAD_PATHSPACE_INTEGRATOR calculateLightSamples
                            : & ray
                            : & wavelength
                            :   sampleValue
                            ];

                        if ( arlightalphasample_l_valid(
                                art_gv,
                                ARPATHSPACERESULT_LIGHTALPHASAMPLE(*sampleValue[0])
                                ) )
                        {
                            if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE )
                            {
                                for ( unsigned int im = 0; im < numberOfImagesToWrite; im++ )
                                    arlightsample_realign_to_coaxial_refframe_l(
                                          art_gv,
                                        & referenceFrame,
                                          ARPATHSPACERESULT_LIGHTSAMPLE( *sampleValue[im] )
                                        );
                            }
                            
                            validSample = TRUE;
                        }
                    }
                    else
                    {
                        for ( unsigned int im = 0; im < numberOfImagesToWrite; im++ )
                        {
                            sampleValue[im] =
                                (ArPathspaceResult*) arfreelist_pop(
                                    & pathspaceResultFreelist[THREAD_INDEX]
                                    );
                            
                            ARPATHSPACERESULT_NEXT(*sampleValue[im]) = NULL;
                            
                            arlightalphasample_l_init_l(
                                  art_gv,
                                  ARLIGHTALPHASAMPLE_NONE_A0,
                                  ARPATHSPACERESULT_LIGHTALPHASAMPLE(*sampleValue[im])
                                );
                        }

                        validSample = TRUE;
                    }

                    if ( validSample )
                    {
                        if ( splattingKernelWidth == 1 )
                        {
                            for ( int im = 0; im < numberOfImagesToWrite; im++ )
                            {
                                PIXEL_SAMPLE_COUNT( x, y, THREAD_INDEX, im ) += 1.0;
                                
                                arlightalpha_wsd_sloppy_add_l(
                                      art_gv,
                                      ARPATHSPACERESULT_LIGHTALPHASAMPLE(*sampleValue[im]),
                                    & wavelength,
                                    & spectralSplattingData,
                                      3.0 DEGREES,
                                      PIXEL_SAMPLE_VALUE( x, y, THREAD_INDEX, im )
                                    );
                            }
                        }
                        else
                        {
                            for ( unsigned int l = 0; l < splattingKernelArea; l++ )
                            {
                                int  cX = x + XC( sampleSplattingOffset[l] );
                                int  cY = y + YC( sampleSplattingOffset[l] );

                                if (   cX >= 0
                                    && cX < XC(imageSize)
                                    && cY >= 0
                                    && cY < YC(imageSize) )
                                {
                                    for ( unsigned int im = 0; im < numberOfImagesToWrite; im++ )
                                    {
                                        PIXEL_SAMPLE_COUNT( cX, cY, THREAD_INDEX, im ) += SAMPLE_SPLATTING_FACTOR( subpixelIdx, l );

                                        for ( unsigned int im = 0; im < numberOfImagesToWrite; im++ )
                                        {
                                            arlightalpha_dwsd_mul_sloppy_add_l(
                                                  art_gv,
                                                  SAMPLE_SPLATTING_FACTOR( subpixelIdx, l ),
                                                  ARPATHSPACERESULT_LIGHTALPHASAMPLE(*sampleValue[im]),
                                                & wavelength,
                                                & spectralSplattingData,
                                                  5.0 DEGREES,
                                                  THREAD_RESULT_PIXEL( cX, cY, im )
                                                );
                                        }
                                    }
                                }
                            }
                        }
                    }

                    for ( unsigned int im = 0; im < numberOfImagesToWrite; im++ )
                    {
                        arpathspaceresult_free_to_freelist(
                              art_gv,
                            & pathspaceResultFreelist[THREAD_INDEX],
                              sampleValue[im]
                            );
                    }
                }
            } // x, XC(image)
        } // y, YC(image)

        //   We only update the progress counter via the first thread
        
        if ( THREAD_INDEX == 0 )
        {
            [ sampleCounter step
                :   numberOfRenderThreads
                ];
        }
    } // i, sample index
    
    [ self renderProcHasFinished: threadIndex ];

    //   Sample value free
    
    FREE_ARRAY(sampleValue);
}

- (void) dealloc
{
    FREE_ARRAY( sampleCoord );

    if ( splattingKernelWidth > 1 )
    {
        FREE_ARRAY( sampleSplattingFactor );
        FREE_ARRAY( sampleSplattingOffset );
    }

    [ super dealloc ];
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];
    [ coder codeBOOL: & deterministicWavelengths ];
    
    if ([coder isReading])
    {
#warning  this whole thing is fragile and should not be in a release version of ART (aw)
        if ( ! deterministicWavelengths )
            wavelengthSteps = 1;
        else
            wavelengthSteps = spc_channels(art_gv) / 4;
    }
}

@end

// ===========================================================================

