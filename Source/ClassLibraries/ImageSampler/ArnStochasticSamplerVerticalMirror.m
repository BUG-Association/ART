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

#define ART_MODULE_NAME     ArnStochasticSamplerVerticalMirror

#import "ArnStochasticSamplerVerticalMirror.h"

#import "ArnReconstructionKernel.h"
#import "FoundationAssertionMacros.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnStochasticSamplerVerticalMirror registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#define  RAYSAMPLER              \
(ArNode* <ArpPathspaceIntegrator>)ARNODEREF_POINTER(subnodeArray[0])
#define  RECONSTRUCTION_KERNEL   \
((id <ArpReconstructionKernel>) ARNODEREF_POINTER(subnodeRefArray[1]))

#define REPORTER    ART_GLOBAL_REPORTER

/* ===========================================================================
    'ArnStochasticSamplerVerticalMirror'
=========================================================================== */

#define DEFAULT_PACKET_SIZE    10000

//   k - number of sample
//   i - number of splatting kernel pixel

#define SAMPLE_SPLATTING_FACTOR( __k, __i ) \
    sampleSplattingFactor[ (__k) * splattingKernelArea + (__i) ]

#define SAMPLE_SPLATTING_FACTOR_UV( __k, __u, __v ) \
    SAMPLE_SPLATTING_FACTOR( __k, (__u) * splattingKernelWidth + (__v) )

@implementation ArnStochasticSamplerVerticalMirror

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnStochasticSamplerVerticalMirror)

- (void) prepareForSampling
        : (ArNode <ArpWorld> *) world
        : (ArNode <ArpCamera> *) newCamera
        : (ArNode <ArpImageWriter> **) image
        : (int) numberOfResultImages
{
    [ super prepareForSampling
        :   world
        :   newCamera
        :   image
        :   numberOfResultImages
        ];

    inverseNumberOfSamples = 1.0 / (double)numberOfSamples;

    //   number of sample packets

    numberOfSamplePackets = ( numberOfSamples / DEFAULT_PACKET_SIZE ) + 1;

    //   samplePacketSize - size of each of the n sample packets. This is
    //   obviously always DEFAULT_PACKET_SIZE, except for the last one.

    packetSize = ALLOC_ARRAY( unsigned int, numberOfSamplePackets );

    for ( unsigned int i = 0; i < ( numberOfSamplePackets - 1 ); i++ )
        packetSize[i] = DEFAULT_PACKET_SIZE;

    //   the last packet has fewer samples

    int  excessSamplesInLastPacket =
          ( DEFAULT_PACKET_SIZE * numberOfSamplePackets )
        - numberOfSamples;

    packetSize[ numberOfSamplePackets - 1 ] =
        DEFAULT_PACKET_SIZE - excessSamplesInLastPacket;

    //   most renders never use sample numbers large enough to fill even one
    //   sample packet - and in these cases, we do not need a large array, either

    if ( numberOfSamplePackets == 1 )
        packetArraySize = numberOfSamples;
    else
        packetArraySize = DEFAULT_PACKET_SIZE;

    //   2D sample coordinates are pre-generated for the entire packet

    //   CAVEAT: they are the same for all packets - but given the huge
    //           packet sizes, this should never really be an issue!
    //           And as long as you cast less than DEFAULT_PACKET_SIZE
    //           rays, everything is as expected anyway - each ray has
    //           unique 2D subpixel coordinates in that case.

    sampleCoord = ALLOC_ARRAY( Pnt2D, packetArraySize );

    //   Actual generation of the 2D sample coordinates

    //   Note that we reset the random generator sequence counter for each
    //   2D coordinate - they all should be from the first two sequences the
    //   generator has to offer!

    for ( unsigned int i = 0; i < packetArraySize; i++ )
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
}

- (id) init
        : (ArNode <ArpPathspaceIntegrator> *) newRaySampler
        : (ArNode <ArpReconstructionKernel> *) newReconstructionKernel
        : (unsigned int) newNumberOfSamples
        : (int) newRandomValueGeneration
{
    self =
        [ super init
            :   newRaySampler
            :   newReconstructionKernel
            ];
    
    if ( self )
    {
        numberOfSamples       = newNumberOfSamples;
        randomValueGeneration = newRandomValueGeneration;
    }
    
    return self;
}

- (id) copy
{
    ArnStochasticSamplerVerticalMirror  * copiedInstance = [ super copy ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnStochasticSamplerVerticalMirror  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

#define FINISHED                 0
#define THREAD_INDEX             threadIndex->value
#define THREAD_MUTEX             threadMutex[ THREAD_INDEX ]
#define YC_CURRENT_SCANLINE      threadPosition[ THREAD_INDEX ]
#define THREAD_RANDOM_GENERATOR  randomGenerator[ THREAD_INDEX ]
#define THREAD_RAYSAMPLER        pathspaceIntegrator[ THREAD_INDEX ]
#define THREAD_RAYCOUNTER        pathCounter[ THREAD_INDEX ]
#define CURRENT_SCANLINE_MUTEX   scanlineMutex[ YC_CURRENT_SCANLINE ]
#define SCANLINE_COUNTER_MUTEX(__i) \
    scanlineCounterMutex[ (__i) ]
#define CACHEPOSITION(__y, __im) \
    (__y) + (THREAD_INDEX * YC(imageSize)) + ((__im) * numberOfRenderThreads * YC(imageSize))
#define CACHEPIXEL(__x,__y,__im) \
    scanlineCache[ CACHEPOSITION(__y,__im) ]->data[(__x)]

typedef struct ArStochasticSample
{
    int             index;
    ArLightAlpha  * light;
}
ArStochasticSample;


- (void) renderProc
        : (ArcUnsignedInteger *) threadIndex
{
    //   Autorelease pool for this thread to keep Cocoa happy

    NSAutoreleasePool  * threadPool;

    threadPool = [ [ NSAutoreleasePool alloc ] init ];

    //   The value for individual samples

    ArPathspaceResult  ** sampleValue =
        ALLOC_ARRAY(ArPathspaceResult*,numberOfImagesToWrite);
    
    pthread_mutex_lock( & THREAD_MUTEX );

    /* ------------------------------------------------------------------
         We render scanlines until there is no unrendered scanline left.
    ------------------------------------------------------------------ */

    while ( [ self stepRenderThread: THREAD_INDEX ] )
    {
        Pnt2D  pixelCoord;

        /* ------------------------------------------------------------------
            First we prepare the temporary storage for this scanline. For
            a reconstruction kernel of a certain width as many scanlines
            have to be allocated centered around the current one. While
            each thread allocates its own temporary scanlines it re-uses
            them between invocations where possible. Once their content
            has been written these temp scanlines are freed by the writing
            thread.
            Note that no locking is necessary for this operation since each
            thread has its own area for temp scanline allocation in the
            cache.
        ---------------------------------------------------------------aw- */

        int  current_scanline_yc = YC_CURRENT_SCANLINE;

        for ( int im = 0; im < numberOfImagesToWrite; im++ )
        {
            if ( ! scanlineCache[ CACHEPOSITION(current_scanline_yc, im) ] )
            {
                scanlineCache[ CACHEPOSITION(current_scanline_yc, im) ] =
                    [ ALLOC_OBJECT(ArnLightAlphaImage)
                        initWithSize
                        :   IVEC2D(XC(imageSize), 1)
                        ];

                for ( int v = 0; v < XC(imageSize); v++ )
                    arlightalpha_l_init_l(
                          art_gv,
                          ARLIGHTALPHA_NONE_A0,
                          CACHEPIXEL(v, current_scanline_yc, im)
                        );
            }
        }

        /* -------------------------------------------------------------
            We lock the scanline_mutex so that the write thread has to
            wait.
        ------------------------------------------------------------- */

        pthread_mutex_lock( & CURRENT_SCANLINE_MUTEX );

        YC(pixelCoord) = current_scanline_yc + YC(imageOrigin);

        /* -------------------------------------------------------------
             We render each pixel of the scanline.
        ------------------------------------------------------------- */
        
        int  xsize = XC(imageSize);
        int  xsize_div_2 = xsize / 2;
        
        int  xsize_mod = xsize % 2;
        int  xsize_render = xsize_div_2 + xsize_mod;

        for ( int i = 0; i < xsize_render; i++ )
        {
            int  ii = xsize - i - 1;

            XC(pixelCoord) = i + XC(imageOrigin);

            [ THREAD_RANDOM_GENERATOR reInitializeWith
                :   crc32_of_data( & pixelCoord, sizeof(Pnt2D) )
                ];

            /* --------------------------------------------------------------
                We first produce n samples per pixel and store them in a
                temporary data structure. Note that for the
                polarisation-aware renderer a reference coordinate system
                has to be maintained for each sample.

                We also double-check whether a given sample should be
                included: first the validity of the ray is checked (rays are
                always valid for normal cameras, but e.g. fisheye cameras
                have pixels which lie outside the imaged area) and secondly
                all rays which do not contain plausible radiance information
                (all components > 0) are simply not used.

                The latter check should not be necessary in a perfect world,
                but in reality malformed mesh data and other gremlins can
                lead to artefacts if this is not looked after.
            ------------------------------------------------------------aw- */

            unsigned int  numberOfValidSamples = 0;

            for ( unsigned int j = 0; j < numberOfSamplePackets; j++ )
            {
                for ( unsigned int k = 0; k < packetSize[j]; k++ )
                {
                    BOOL  validSample = FALSE;

                    [ THREAD_RANDOM_GENERATOR setCurrentSequenceID
                        :  startingSequenceID
                        ];

                    Ray3D              ray;
                    ArReferenceFrame   referenceFrame;
                    ArWavelength       wavelength;
                    
                    arwavelength_sd_init_w(
                          art_gv,
                        & hwssSamplingData,
                          [ THREAD_RANDOM_GENERATOR valueFromNewSequence ],
                        & wavelength
                        );
                    
                    if ( [ camera getWorldspaceRay
                             : & VEC2D(
                                    XC(pixelCoord) + XC(sampleCoord[k]),
                                    YC(pixelCoord) + YC(sampleCoord[k])
                                    )
                             :   THREAD_RANDOM_GENERATOR
                             : & referenceFrame
                             : & ray
                             ] )
                    {
                        [ THREAD_RAYSAMPLER calculateLightSamples
                            : & ray
                            : & wavelength
                            :   sampleValue
                            ];

                        if ( arlightalphasample_l_valid( art_gv, ARPATHSPACERESULT_LIGHTALPHASAMPLE(*sampleValue[0]) ) )
                        {
                            for ( int im = 0; im < numberOfImagesToWrite; im++ )
                                arlightsample_realign_to_coaxial_refframe_l(
                                      art_gv,
                                    & referenceFrame,
                                      ARPATHSPACERESULT_LIGHTSAMPLE( *sampleValue[im] )
                                    );

                            validSample = TRUE;
                            numberOfValidSamples++;
                        }
                    }
                    else
                    {
                        for ( int im = 0; im < numberOfImagesToWrite; im++ )
                            arlightalphasample_l_init_l(
                                  art_gv,
                                  ARLIGHTALPHASAMPLE_NONE_A0,
                                  ARPATHSPACERESULT_LIGHTALPHASAMPLE(*sampleValue[im])
                                );

                        validSample = TRUE;
                        numberOfValidSamples++;
                    }

                    if ( validSample )
                    {
                        for ( int im = 0; im < numberOfImagesToWrite; im++ )
                        {
                            arlightalpha_wsd_sloppy_add_l(
                                  art_gv,
                                  ARPATHSPACERESULT_LIGHTALPHASAMPLE(*sampleValue[im]),
                                & wavelength,
                                & hwssSplattingData,
                                  3.0 DEGREES,
                                  CACHEPIXEL(i, current_scanline_yc,im)
                                );
                            
                            if ( i < xsize_div_2 )
                                arlightalpha_wsd_sloppy_add_l(
                                      art_gv,
                                      ARPATHSPACERESULT_LIGHTALPHASAMPLE(*sampleValue[im]),
                                    & wavelength,
                                    & hwssSplattingData,
                                      3.0 DEGREES,
                                      CACHEPIXEL(ii, current_scanline_yc,im)
                                    );
                        }
                    }
                }
            } // j, # of packets

            for ( int im = 0; im < numberOfImagesToWrite; im++ )
                arpathspaceresult_free_to_freelist(
                      art_gv,
                    & pathspaceResultFreelist[THREAD_INDEX],
                      sampleValue[im]
                    );

            /* ---------------------------------------------------------------
                The inverse of the number of valid samples is the scaling
                factor for the individual contributions.
            ------------------------------------------------------------aw- */

            if ( numberOfValidSamples > 0 )
            {
                double  inverseNumberOfValidSamples = 0.0;

                inverseNumberOfValidSamples =
                    1.0 / numberOfValidSamples;

                ASSERT_VALID_FINITE_DOUBLE(inverseNumberOfValidSamples)
                for ( int im = 0; im < numberOfImagesToWrite; im++ )
                {
                    ASSERT_VALID_LIGHTALPHA(CACHEPIXEL(i, current_scanline_yc,im))
                    arlightalpha_d_mul_l(
                          art_gv,
                          inverseNumberOfValidSamples,
                          CACHEPIXEL(i, current_scanline_yc, im )
                        );

                    if ( i < xsize_div_2 )
                        arlightalpha_d_mul_l(
                              art_gv,
                              inverseNumberOfValidSamples,
                              CACHEPIXEL(ii, current_scanline_yc, im )
                            );
                }
            }
        } // i, XC(image)


        /* -------------------------------------------------------------------
            Finally, we decrease the "to do - counter" for each of the
            scanlines we were just adding samples to - only when these reach
            zero a scanline is considered to be truly finished and ready for
            output by the writing thread.
            We again have to cover the entire width of the reconstruction
            kernel and check for image bounds.
        ----------------------------------------------------------------aw- */

        int  scratchmonkey = 1;

        m_i_xadd_i(
            & scratchmonkey,
            & scanlineStatus[ current_scanline_yc ]
            );

        pthread_mutex_unlock( & CURRENT_SCANLINE_MUTEX );

        [ THREAD_RAYCOUNTER step
            :   YC(imageSize) - current_scanline_yc - 1
            ];
    }

    threadStatus[ THREAD_INDEX ] = FINISHED;
    pthread_mutex_unlock( & THREAD_MUTEX );

    //   Sample value free
        
    FREE_ARRAY(sampleValue);

    //   Remove the splatting temp array, if we had one
}

- (void) dealloc
{
    FREE_ARRAY( packetSize );
    FREE_ARRAY( sampleCoord );

    [ super dealloc ];
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];

    [ coder codeInt:  & numberOfSamples ];
    [ coder codeInt:  & randomValueGeneration ];
}

@end

// ===========================================================================

