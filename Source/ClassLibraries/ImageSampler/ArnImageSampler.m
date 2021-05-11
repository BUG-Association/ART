/* ===========================================================================

    Copyright (c) 1996-2020 The ART Development Team
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

#define ART_MODULE_NAME     ArnImageSampler

#import "ArnImageSampler.h"
#import "ArnImageSamplerCommonMacros.h"

#include <stdio.h> 
#include <unistd.h>
#include <signal.h>
#include <termios.h> 

#import "ART_ImageFileFormat.h"
#import "ART_ARM_Interface.h"
#import "ApplicationSupport.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    [ ArnImageSampler registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


/* ===========================================================================
    'ArnImageSampler'
    All derivates of ArnImageSamplerBase share the performOn method
=========================================================================== */

@implementation ArnImageSampler

ARPNODE_DEFAULT_IMPLEMENTATION(ArnImageSampler)
ARPACTION_DEFAULT_IMPLEMENTATION(ArnImageSampler)

- copy
{
    ArnImageSampler  * copiedInstance = [ super copy ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnImageSampler  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- (void) performOn
        : (ArNode <ArpNodeStack> *) nodeStack
{
    [ super performOn: nodeStack ];
}

- (void) prepareForSampling
        : (ArNode <ArpWorld> *) newWorld
        : (ArNode <ArpCamera> *) newCamera
        : (ArNode <ArpImageWriter> **) image
        : (int) numberOfResultImages
{
    if ( overallNumberOfSamplesPerPixel == 0 )
    {
        asprintf(
            & preSamplingMessage,
              "---   interactive mode on, open ended sampling, press t to terminate   ---\n"
            );
    }
    else
    {
        asprintf(
            & preSamplingMessage,
              "---   interactive mode on, goal are %d spp   ---\n",
              overallNumberOfSamplesPerPixel
            );
    }
    
    [ super prepareForSampling
        :   newWorld
        :   newCamera
        :   image
        :   numberOfResultImages
        ];

    pathspaceIntegrator =
        ALLOC_ARRAY(
            ArNode <ArpPathspaceIntegrator> *,
            numberOfRenderThreads
            );

    pathspaceResultFreelist =
        ALLOC_ARRAY(
            ArFreelist,
            numberOfRenderThreads
            );

    for ( unsigned int i = 0; i < numberOfRenderThreads; i++)
    {
        ARFREELIST_INIT_FOR_TYPE(
            pathspaceResultFreelist[i],
            arpathspaceresult,
            128
            );

        pathspaceIntegrator[i] =
            [ GATHERING_ESTIMATOR copy ];

        [ pathspaceIntegrator[i] setGatheringResultFreelist
            : & pathspaceResultFreelist[i]
            ];

        [ pathspaceIntegrator[i] setRandomGenerator
            :   randomGenerator[i]
            ];

        [ pathspaceIntegrator[i] prepareForEstimation
            :   world
            :   lightsources
            :   [ camera eye ]
            :   [ camera near ]
            :   numberOfSamplesPerThread
            :   ART_GLOBAL_REPORTER
            ];
    }

    //   Get sample splatting pre-computed data

    arwavelength_sampling_data_from_current_ISR_s(
          art_gv,
        & spectralSamplingData
        );

    sps_splatting_data_from_current_ISR_s(
          art_gv,
        & spectralSplattingData
        );
    
    //   Mutex for the "tonemap and open intermediate result" thread.
    
    pthread_mutex_init( & tonemapAndOpenThreadMutex, NULL );
    
    //   Creation of the image and sample count buffers for the threads.

    resultImage =
        ALLOC_ARRAY(
            ArnLightAlphaImage *,
            numberOfRenderThreads * numberOfResultImages
            );

    samplesPerPixel =
        ALLOC_ARRAY(
            double,
              numberOfRenderThreads * numberOfResultImages
            * XC(imageSize) * YC(imageSize)
            );

    for ( unsigned int i = 0;
          i < numberOfRenderThreads * numberOfResultImages;
          i++ )
    {
        resultImage[i] =
            [ ALLOC_OBJECT(ArnLightAlphaImage)
                initWithSize
                :   imageSize
                ];
    }

    //   For all threads, and all result images in each thread, we
    //   zero out the buffers.
    
    for ( unsigned int threadIdx = 0;
          threadIdx < numberOfRenderThreads;
          threadIdx++ )
    {
        for ( unsigned int imgIdx = 0;
              imgIdx < numberOfResultImages;
              imgIdx++ )
        {
            for ( unsigned int y = 0; y < YC(imageSize); y++ )
            {
                for ( unsigned int x = 0; x < XC(imageSize); x++ )
                {
                    arlightalpha_l_init_l(
                          art_gv,
                          ARLIGHTALPHA_NONE_A0,
                          PIXEL_SAMPLE_VALUE( x, y, threadIdx, imgIdx )
                        );
                    
                    PIXEL_SAMPLE_COUNT( x, y, threadIdx, imgIdx ) = 0.0;
                }
            }
        }
    }
}

- (void) sampleImage
        : (ArNode <ArpWorld> *) newWorld
        : (ArNode <ArpCamera > *) newCamera
        : (ArNode <ArpImageWriter> **) image
        : (int) numberOfResultImages
{
    pthread_mutex_lock( & writeThreadMutex );

    //   This function sets the stage for the rendering processes to do their
    //   work, starts them, and then sleeps until they are done.
    
    //   Detach n render threads.

    [ sampleCounter start ];

    for ( unsigned int i = 0; i < numberOfRenderThreads; i++ )
    {
        ArcInteger  * index = [ ALLOC_INIT_OBJECT(ArcInteger) : i ];

        if ( ! art_thread_detach(@selector(renderProc:), self,  index))
            ART_ERRORHANDLING_FATAL_ERROR(
                "could not detach rendering thread %d",
                i
                );
    }

    if ( ! art_thread_detach(@selector(termIOProc:), self,  0))
        ART_ERRORHANDLING_FATAL_ERROR(
            "could not detach terminal I/O thread"
            );

    struct sigaction sa;

    image_sampler_reset_received_signals();

    sa.sa_handler = image_sampler_sigusr1_handler;
    sa.sa_flags = 0;
    sigemptyset( & sa.sa_mask );

    if ( sigaction( SIGUSR1, & sa, NULL ) == -1 )
    {
        ART_ERRORHANDLING_FATAL_ERROR(
            "could not install handler for SIGUSR1"
            );
    }

    sa.sa_handler = image_sampler_sigusr2_handler;

    if ( sigaction( SIGUSR2, & sa, NULL ) == -1 )
    {
        ART_ERRORHANDLING_FATAL_ERROR(
            "could not install handler for SIGUSR2"
            );
    }

    sa.sa_handler = image_sampler_sigint_handler;

    if ( sigaction( SIGINT, & sa, NULL ) == -1 )
    {
        ART_ERRORHANDLING_FATAL_ERROR(
            "could not install handler for SIGINT"
            );
    }

    if( pipe( read_thread_pipe ) == -1 )
    {
        fprintf(stderr, "Error creating pipe to I/O thread\n");
    }

    //   Wait for the results to come in, and assemble them into a final
    //   result image.
    
    ArnLightAlphaImage  * compositeScanline =
        [ ALLOC_OBJECT(ArnLightAlphaImage)
            initWithSize
            :   IVEC2D(XC(imageSize), 1)
            ];

    /* ------------------------------------------------------------------
        IMPORTANT: unlike all other timings done in ART the duration of
        the main image writing thread (from which the duration of the
        whole ray-based rendering pass is computed) is based on elapsed
        time, and not on system time used.
        The reason for this is that the multi-threaded nature of the
        pixel sampler makes most other options pretty useless. The
        user time of the write thread is far less than the true rendering
        time - it spends most of its time sleeping and waiting for the
        rendering thread(s) - while the user time of the rendering
        thread(s) might differ by a considerable amount. If you have
        too much time on your hands you can try computing the average
        of the rendering thread user times and returning _that_. :-)
    ---------------------------------------------------------------aw- */

    ArTime  beginTime, endTime;

    artime_now( & beginTime );

    renderingThreadsAreDone = NO;
    renderThreadsShouldTerminate = NO;
    
    //   The basic structure of the threaded renderer is such that it
    //   is able to write partially-done images even before all threads
    //   terminate. Writing of such intermediate images is triggered via
    //   user signals, which have to be reset after use.
    
    do
    {
        pthread_cond_wait( & writeThreadCond, & writeThreadMutex );
        
        //   we splice all the thread images, and write them to disk
        
        unsigned int  overallNumberOfPixels = YC(imageSize) * XC(imageSize);
        
        for ( unsigned int imgIdx = 0; imgIdx < numberOfImagesToWrite; imgIdx++ )
        {
            //   first, we figure out the average number of samples per pixel
            //   this goes into the image statistics that are saved
            //   along with the command line
            
            unsigned int  maxSamples = 0;
            unsigned int  minSamples = 0xffffffff;

            unsigned long int  overallSampleCount = 0;
            unsigned long int  nonzeroPixels = 0;
            
            for ( unsigned int y = 0; y < YC(imageSize); y++ )
            {
                for ( int x = 0; x < XC(imageSize); x++ )
                {
                    int  pixelSampleCount = 0;
                    
                    for ( unsigned int threadIdx = 0;
                          threadIdx < numberOfRenderThreads;
                          threadIdx++ )
                    {
                        pixelSampleCount +=
                            PIXEL_SAMPLE_COUNT(x,y,threadIdx,imgIdx);
                    }
                    
                    if ( pixelSampleCount > 0 )
                    {
                        nonzeroPixels++;
                    
                        if ( pixelSampleCount < minSamples )
                            minSamples = pixelSampleCount;
                        
                        overallSampleCount += pixelSampleCount;
                    }
                    
                    if ( pixelSampleCount > maxSamples )
                        maxSamples = pixelSampleCount;
                }
            }

            unsigned int  avgSamples = 0;
            
            if ( nonzeroPixels > 0 )
            {
                avgSamples = (unsigned int)
                    ( (double) overallSampleCount / (double) nonzeroPixels );
            }
            
            char  * samplecountString = NULL;
            
            double  percentageOfZeroPixels =
                ( 1.0 - ( 1.0 * nonzeroPixels / overallNumberOfPixels )) * 100.0;

            if ( percentageOfZeroPixels > 1.0 )
            {
                if ( nonzeroPixels > 0 )
                {
                    asprintf(
                        & samplecountString,
                          "%.0f%% pixels with zero samples,"
                          " rest: %d/%d/%d min/avg/max spp",
                          percentageOfZeroPixels,
                          minSamples,
                          avgSamples,
                          maxSamples
                        );
                }
                else
                {
                    asprintf(
                        & samplecountString,
                          "0 spp"
                        );
                }
            }
            else
            {
                asprintf(
                    & samplecountString,
                      "%d/%d/%d min/avg/max spp",
                      minSamples,
                      avgSamples,
                      maxSamples
                    );
            }

            [ outputImage[imgIdx] setSamplecountString
                :   samplecountString
                ];
            
            FREE( samplecountString );

            artime_now( & endTime );

            writeThreadWallClockDuration =
                  artime_seconds( & endTime )
                - artime_seconds( & beginTime);

            char  * rendertimeString = NULL;
            
            asprintf(
                & rendertimeString,
                  "%.0f seconds",
                  writeThreadWallClockDuration
                );

            [ outputImage[imgIdx] setRendertimeString
                :   rendertimeString
                ];
            
            FREE( rendertimeString );

            for ( unsigned int y = 0; y < YC(imageSize); y++ )
            {
                for ( int x = 0; x < XC(imageSize); x++ )
                {
                    arlightalpha_l_init_l(
                          art_gv,
                          ARLIGHTALPHA_NONE_A0,
                          compositeScanline->data[x]
                        );
                    
                    double  pixelSampleCount = 0.0;

                    for ( unsigned int threadIdx = 0;
                          threadIdx < numberOfRenderThreads;
                          threadIdx++ )
                    {
                        if ( PIXEL_SAMPLE_COUNT(x,y,threadIdx,imgIdx) > 0.0 )
                        {
                            ASSERT_VALID_LIGHTALPHA(
                                PIXEL_SAMPLE_VALUE(x,y,threadIdx,imgIdx)
                                );
                            
                            pixelSampleCount += PIXEL_SAMPLE_COUNT(x,y,threadIdx,imgIdx);
                            
                            arlightalpha_l_add_l(
                                  art_gv,
                                  PIXEL_SAMPLE_VALUE(x,y,threadIdx,imgIdx),
                                  compositeScanline->data[x]
                                );
                        }
                    }

                    if ( pixelSampleCount > 0.0 )
                    {
                        arlightalpha_d_mul_l(
                              art_gv,
                              1.0 / pixelSampleCount,
                              compositeScanline->data[x]
                            );
                    }
                }

                [ outputImage[imgIdx] setPlainImage
                    :   IPNT2D(0,y)
                    :   compositeScanline
                    ];
            }
        }
        
        //   We might have been woken by a SIGUSR sent by 'impresario'.
        
        //   USR1 is "write a partially completed image, and go to sleep"
        //   So we just reset the flag, if we were woken by this (work already
        //   done by the code above).
        
        int  receivedSignal =
            image_sampler_received_signal();
        
        if ( receivedSignal == 1 )
        {
            image_sampler_reset_received_signals();
        }
        
        //   USR2 writes the current result image, but also tonemaps and
        //   opens it.
        
        if ( receivedSignal == 2 )
        {
            image_sampler_reset_received_signals();
            
            //   The mutex basically only ensures that at most, there is only
            //   one tone mapping thread going on at any time.
            //   (so that one cannot shoot down 'artist' by bombarding it with
            //   SIGUSR2 signals)
            
            //   The mutex is released by the tone mapping thread, once it is
            //   done.
            
            pthread_mutex_lock( & tonemapAndOpenThreadMutex );

            //   The tone mapping thread, if there is one, has index
            //   "renderthreads plus one". It doesn't use the index anyway, but
            //   still needs one.

            ArcInteger  * index =
                [ ALLOC_INIT_OBJECT(ArcInteger)
                    :   numberOfRenderThreads
                    ];

            if ( ! art_thread_detach(@selector(tonemapAndOpenProc:), self,  index))
                ART_ERRORHANDLING_FATAL_ERROR(
                    "could not detach intermediate result tone mapping "
                    "& display thread"
                    );
        }

        if ( receivedSignal == 3 )
        {
            image_sampler_reset_received_signals();
            signal(SIGINT, SIG_DFL);
            renderThreadsShouldTerminate = YES;
        }
    }
    while( ! renderingThreadsAreDone );

    //   This shuts down the I/O watching thread for interactive mode
    
    write( read_thread_pipe[1], "q", 1 );
    
    artime_now( & endTime );

    writeThreadWallClockDuration =
          artime_seconds( & endTime )
        - artime_seconds( & beginTime);

    [ sampleCounter stop
        :  writeThreadWallClockDuration
        ];

    RELEASE_OBJECT(compositeScanline);
    
    pthread_mutex_unlock( & writeThreadMutex );
}

- (const char *) preSamplingMessage
{
    return preSamplingMessage;
}

- (const char *) postSamplingMessage
{
    return "---   interactive mode off   ---\n";
}


- (void) tonemapAndOpenProc
        : (ArcInteger *) threadIndex
{
    //   autorelease pool for this thread to keep Cocoa happy

    NSAutoreleasePool  * threadPool;

    threadPool = [ [ NSAutoreleasePool alloc ] init ];

    //   minimalist tone mapping action sequence
    
    ArNode <ArpAction>  * actionSequence =
        ACTION_SEQUENCE(
            [ IMAGECONVERSION_ARTRAW_TO_ARTCSP
                removeSource : NO 
            ], 

            STANDARD_GLOBAL_TONEMAPPING_OPERATOR, 

            STANDARD_LUMINANCE_CLIPPING, 

            [ IMAGECONVERSION_ARTCSP_TO_TIFF 
                removeSource : YES 
                bitsPerChannel : 8 
            ], 
            
            OPEN_RESULT_IMAGE_IN_EXTERNAL_VIEWER_ACTION,

            ACTION_SEQUENCE_END
            );
    
    ArNode <ArpNodeStack>  *  localNodestack =
        ARNNODESTACK( ART_APPLICATION_MAIN_FILENAME );

    //   place input image on node stack, and start stack machine
    //   loop over all output images

    for ( unsigned int imgIdx = 0; imgIdx < numberOfImagesToWrite; imgIdx++ )
    {
        //   we effectively create copies of the actual output images
        //   handing over the originals should work as well, though
        
        ArnFileImage  * image =
            [ FILE_IMAGE
                :   [ (ArnFileImage <ArpImage> *)outputImage[imgIdx] fileName ]
                ];

        [ localNodestack push
            :   HARD_NODE_REFERENCE(image)
            ];
    }

    //   stack machine start
    
    [ actionSequence performOn
        :   localNodestack
        ];

    //   release all things that can be auto-released
    //   (most objects further up were created with autorelease set)
    
    [ threadPool release ];

    //   Release the mutex
    
    pthread_mutex_unlock( & tonemapAndOpenThreadMutex );
}

- (void) renderProcHasFinished
        : (ArcInteger *) threadIndex
{
    threadStatus[ THREAD_INDEX ] = FINISHED;

    pthread_mutex_lock( & writeThreadMutex );
    
    if ( ! renderingThreadsAreDone )
    {
        renderingThreadsAreDone = YES;
        
        for ( unsigned int i = 0; i < numberOfRenderThreads; i++)
        {
            if ( threadStatus[i] == INCOMPLETE )
            {
                renderingThreadsAreDone = NO;
                break;
            }
        }
        
        if ( renderingThreadsAreDone )
        {
            pthread_cond_signal( & writeThreadCond );
        }
    }

    pthread_mutex_unlock( & writeThreadMutex );
}

- (void) termIOProc
        : (ArcInteger *) threadIndex
{
    if ( art_interactive_mode_permitted( art_gv ) )
    {
        setvbuf(stdout,NULL,_IONBF,0);

        struct termios old_termios, new_termios;
        
        tcgetattr( STDIN_FILENO, & old_termios );

        new_termios = old_termios;

        // disable canonical mode and echo
        new_termios.c_lflag &= (~ICANON & ~ECHO);

        // at least one character must be written before read returns
        new_termios.c_cc[VMIN] = 1;

        // no timeout
        new_termios.c_cc[VTIME] = 0;
        
        tcsetattr( STDIN_FILENO, TCSANOW, & new_termios );

        char  lineA[256]; ssize_t  lenA;
        char  lineB[256]; ssize_t  lenB;
        char * line; ssize_t  len;
        do{
            int r, nfds = 0;
            fd_set rd, wr, er;

            FD_ZERO(&rd);
            FD_ZERO(&wr);
            FD_ZERO(&er);
            FD_SET(STDIN_FILENO, &rd);
            nfds = M_MAX(nfds, STDIN_FILENO);
            FD_SET(read_thread_pipe[0], &rd);
            nfds = M_MAX(nfds, read_thread_pipe[0]);

            r = select(nfds + 1, &rd, &wr, &er, NULL);
            
            lenA = 0;
            lenB = 0;
            
            if ( FD_ISSET(STDIN_FILENO, &rd) )
            {
                lenA=read(STDIN_FILENO,lineA,256); lineA[lenA]='\0';
            }
            
            if ( FD_ISSET(read_thread_pipe[0], &rd) )
            {
                lenB=read(read_thread_pipe[0],lineB,256); lineB[lenB]='\0';
            }
            
            if (lenA > 0 )
            {
                line = lineA;
                len = lenA;
            }
            else
            {
                line = lineB;
                len = lenB;
            }

            if( len > 0 )
            {
                if ( line[0] == 'w' )
                {
                    image_sampler_sigusr1_handler( SIGUSR1 );
                }

                if ( line[0] == 'd' )
                {
                    image_sampler_sigusr2_handler( SIGUSR2 );
                }

                if ( line[0] == 't' )
                {
                    image_sampler_sigint_handler( SIGINT );
                }
            }
        }
        //   we only terminate if we got a 'q' via the pipe, not via the
        //   keyboard
        while( !( line[0] == 'q' && lenB > 0 ) );

        tcsetattr( 0, TCSANOW, & old_termios );
    }
}

- (void) cleanupAfterImageSampling
        : (ArNode <ArpWorld> *) world4cleanup
        : (ArNode <ArpCamera > *) camera4cleanup
        : (ArNode <ArpImageWriter> **) image4cleanup
        : (int) numberOfResultImages4cleanup
{
    [ super cleanupAfterImageSampling
        :   world4cleanup
        :   camera4cleanup
        :   image4cleanup
        :   numberOfResultImages4cleanup
        ];


#if defined(ENABLE_EMBREE_SUPPORT)
    // if embree is enabled, clean up embree too
    if([ArnEmbree embreeEnabled]) {
        [ArnEmbree cleanUp];
    }
#endif

    for ( unsigned int i = 0; i < numberOfRenderThreads; i++ )
    {
        [ pathspaceIntegrator[i] cleanupAfterEstimation: ART_GLOBAL_REPORTER ];

        RELEASE_OBJECT( pathspaceIntegrator[i] );
    }
    
    FREE_ARRAY( pathspaceIntegrator );
}

@end


// ===========================================================================

