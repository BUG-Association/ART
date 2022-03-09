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

#define ART_MODULE_NAME     ArnStochasticImageSamplerRGBA

#import "ArnStochasticImageSamplerRGBA.h"
#import "ArnImageSamplerCommonMacros.h"

#import "FoundationAssertionMacros.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnStochasticImageSamplerRGBA registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

/* ===========================================================================
    'ArnStochasticImageSamplerRGBA'
=========================================================================== */

@implementation ArnStochasticImageSamplerRGBA

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnStochasticImageSamplerRGBA)

- (void) renderProc
        : (ArcUnsignedInteger *) threadIndex
{
    //   Autorelease pool for this thread to keep Cocoa happy

    NSAutoreleasePool  * threadPool;

    threadPool = [ [ NSAutoreleasePool alloc ] init ];
    
    Pnt2D  * sampleCoord = ALLOC_ARRAY( Pnt2D, overallNumberOfSamplesPerPixel );

    //   Actual generation of the 2D sample coordinates

    //   Note that we reset the random generator sequence counter for each
    //   2D coordinate - they all should be from the first two sequences the
    //   generator has to offer!

    for ( unsigned int i = 0; i < overallNumberOfSamplesPerPixel; i++ )
    {
        [ randomGenerator[0] resetSequenceIDs ];

        [ randomGenerator[0] getValuesFromNewSequences
            : & XC( sampleCoord[i] )
            : & YC( sampleCoord[i] )
            ];
    }
    
    Pnt2D  pixelCoord;

    int  threadStripWidth = YC(imageSize) / numberOfRenderThreads;
    int  threadOffset = threadStripWidth * THREAD_INDEX;
    
    int  y_start = threadOffset;
    int  y_end   = threadOffset + threadStripWidth;
    
    unsigned int  centerThread = numberOfRenderThreads / 2;
    
    if ( centerThread >= numberOfRenderThreads )
        centerThread = 0;
    
    for ( int y = y_start; y < y_end; y++ )
    {
        YC(pixelCoord) = y + YC(imageOrigin);

        for ( int x = 0; x < XC(imageSize); x++ )
        {
            XC(pixelCoord) = x + XC(imageOrigin);

            for ( unsigned int i = 0; i < overallNumberOfSamplesPerPixel; i++ )
            {
                ArRGBA             sample;
                Ray3D              ray;
                ArReferenceFrame   referenceFrame;

                if ( [ camera getWorldspaceRay
                         : & VEC2D(
                                XC(pixelCoord) + XC( sampleCoord[i] ),
                                YC(pixelCoord) + YC( sampleCoord[i] )
                                )
                         :   THREAD_RANDOM_GENERATOR
                         : & referenceFrame
                         : & ray ] )
                {
                    [ THREAD_PATHSPACE_INTEGRATOR calculateRGBASample
                        : & ray
                        : & sample
                        ];
                }
                else
                {
                    sample =
                        ARRGBA(0, 0, 0, 0);
                }
                
                ARRGBA_R( RGB_PIXEL_SAMPLE_VALUE(x, y) ) += ARRGBA_R( sample );
                ARRGBA_G( RGB_PIXEL_SAMPLE_VALUE(x, y) ) += ARRGBA_G( sample );
                ARRGBA_B( RGB_PIXEL_SAMPLE_VALUE(x, y) ) += ARRGBA_B( sample );
                ARRGBA_A( RGB_PIXEL_SAMPLE_VALUE(x, y) ) += ARRGBA_A( sample );
            }

            ARRGBA_R( RGB_PIXEL_SAMPLE_VALUE(x, y) ) *= inverseNumberOfOverallSamples;
            ARRGBA_G( RGB_PIXEL_SAMPLE_VALUE(x, y) ) *= inverseNumberOfOverallSamples;
            ARRGBA_B( RGB_PIXEL_SAMPLE_VALUE(x, y) ) *= inverseNumberOfOverallSamples;
            ARRGBA_A( RGB_PIXEL_SAMPLE_VALUE(x, y) ) *= inverseNumberOfOverallSamples;
        }
        
        //   We only update the progress counter via one thread that is
        //   responsible for (roughly) the center of the image.
        
        if ( THREAD_INDEX == centerThread )
        {
            [ sampleCounter step
                :   numberOfRenderThreads
                ];
        }

    } // y, YC(image)

    FREE_ARRAY( sampleCoord );

    [ self renderProcHasFinished: threadIndex ];
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];
}

@end

// ===========================================================================

