/* ===========================================================================

    Copyright (c) 1996-2019 The ART Development Team
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

#define ART_MODULE_NAME     SmoothingFilterImageActions

#import "SmoothingFilterImageActions.h"
#import "ART_ImageData.h"
#import "ART_ImageFileFormat.h"
#import "ArnImageManipulationMacros.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    [ ArnBilateralFilterSmoothingAction     registerWithRuntime ];
    [ ArnGaussianFilterSmoothingAction      registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#define REPORTER    ART_GLOBAL_REPORTER

#define GEXP(_x,_sigma) \
    (exp(-((_x)*(_x)/(2*(_sigma)*(_sigma)))))

#define GEXP2(_x,_y,_sigma) \
    (exp(-(((_x)*(_x) + (_y)*(_y))/(2*(_sigma)*(_sigma)))))

#define INDEX(_x,_y,_size) \
    (((_y)+(_size))*(_size)+(_x)+(_size))


@implementation ArnBilateralFilterSmoothingAction

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnBilateralFilterSmoothingAction)
ARPACTION_DEFAULT_SINGLE_IMAGE_ACTION_IMPLEMENTATION(ArnBilateralFilterSmoothingAction)

- sigmas
        : (double) newSigmaD
        : (double) newSigmaC
{
    return
        [ self init
            : newSigmaD
            : newSigmaC
            ];
}

- init
        : (double) newSigmaD
        : (double) newSigmaC
{
    self =
        [ super init
            :   YES
            ];
    
    if ( self )
    {
        sigma_d = newSigmaD;
        sigma_c = newSigmaC;
    }
    
    return self;
}

- init
{
    return
        [ self init
            :   1.0
            :   1.0
            ];
}

- (void) performOn
        : (ArNode <ArpNodeStack> *) nodeStack
{
    [ REPORTER beginTimedAction
        :   "applying bilateral filter smoothing operator"
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
         we wish to create (in our case, two instances of ArfARTCSP).
    ---------------------------------------------------------------aw- */

    [ self prepareForImageManipulation
        :   nodeStack
        :   [ ArfARTCSP class ]
        :   [ ArfARTCSP class ]
        ];

    const double SIGMA_D = sigma_d;
    const double SIGMA_C = sigma_c;

    const int KERNEL_RADIUS     = 5;
    const int KERNEL_SIZE       = 2*KERNEL_RADIUS + 1;
    const int KERNEL_ARRAY_SIZE = KERNEL_SIZE * KERNEL_SIZE;

    double weights[KERNEL_ARRAY_SIZE];
    
    for ( int j = 0; j < KERNEL_SIZE; j++ )
    for ( int i = 0; i < KERNEL_SIZE; i++ )
        weights[j*KERNEL_SIZE + i] = GEXP2( i * 2.0 / (2*KERNEL_RADIUS) - 1.0,
                                            j * 2.0 / (2*KERNEL_RADIUS) - 1.0,
                                            SIGMA_D
                                            );
    
    // Debug print
    //printf("\n");
    //printf("SigmaD: %f, SigmaC: %f\n", sigma_d, sigma_c);
    //for ( int j = 0; j < KERNEL_SIZE; j++ )
    //{
    //    for ( int i = 0; i < KERNEL_SIZE; i++ )
    //        printf("%f, ", weights[j*KERNEL_SIZE + i]);
    //    printf("\n");
    //}
    //fflush(stdout);
    /* ------------------------------------------------------------------
         Process all pixels in the image.
    ---------------------------------------------------------------aw- */

    for ( int i = 0; i < numberOfSourceImages; i++ )
    {
        for ( long y = 0; y < YC(destinationImageSize); y++ )
        {
            [ self loadSourceScanlineBuffer: i : y ];

            for ( long x = 0; x < XC(destinationImageSize); x++ )
            {
                
                double sumX = 0.0;
                double sumY = 0.0;
                double sumZ = 0.0;

                double norm = 0.0;
                    
                ArCIELab lab;
                xyz_to_lab(
                    art_gv,
                    & XYZA_SOURCE_BUFFER_XYZ(x),
                    & lab );
                
                for ( int hy = -KERNEL_RADIUS; hy <= KERNEL_RADIUS; hy++ )
                if ( y + hy >= 0 && y + hy < YC(destinationImageSize) )
                {
                    [ self loadSourceScanlineBuffer: i : y + hy ];
                    
                    for ( int hx = -KERNEL_RADIUS; hx <= KERNEL_RADIUS; hx++ )
                    {
                        if ( x + hx >= 0 && x + hx < XC(destinationImageSize) )
                        {
                            ArCIELab lab_hx;
                            xyz_to_lab(
                                  art_gv,
                                & XYZA_SOURCE_BUFFER_XYZ(x + hx),
                                & lab_hx );

                            const double delta = lab_delta_E(&lab, &lab_hx);
                            const double weight_color = GEXP( delta, SIGMA_C );
                            const double weight_dist = weights[INDEX(hx, hy, KERNEL_RADIUS)]; 

                            sumX += weight_dist * weight_color * ARCIEXYZ_X(XYZA_SOURCE_BUFFER_XYZ(x + hx));
                            sumY += weight_dist * weight_color * ARCIEXYZ_Y(XYZA_SOURCE_BUFFER_XYZ(x + hx));
                            sumZ += weight_dist * weight_color * ARCIEXYZ_Z(XYZA_SOURCE_BUFFER_XYZ(x + hx));

                            norm += weight_dist * weight_color; 
                        }
                    }
                }
                
                [ self loadSourceScanlineBuffer: i : y ];

                ARCIEXYZ_X(XYZA_DESTINATION_BUFFER_XYZ(x)) = sumX / norm;
                ARCIEXYZ_Y(XYZA_DESTINATION_BUFFER_XYZ(x)) = sumY / norm;
                ARCIEXYZ_Z(XYZA_DESTINATION_BUFFER_XYZ(x)) = sumZ / norm;

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
        :   nodeStack
        ];

    [ REPORTER endAction ];
}

@end

@implementation ArnGaussianFilterSmoothingAction

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnGaussianFilterSmoothingAction)
ARPACTION_DEFAULT_SINGLE_IMAGE_ACTION_IMPLEMENTATION(ArnGaussianFilterSmoothingAction)

- sigma
        : (double) newSigma
{
    return
        [ self init
            : newSigma
            ];
}

- init
        : (double) newSigma
{
    self =
        [ super init
            :   YES
            ];
    
    if ( self )
    {
        sigma = newSigma;
    }
    
    return self;
}

- init
{
    return
        [ self init
            :   1.0
            ];
}

- (void) performOn
        : (ArNode <ArpNodeStack> *) nodeStack
{
    [ REPORTER beginTimedAction
        :   "applying gaussian filter smoothing operator"
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
         we wish to create (in our case, two instances of ArfARTCSP).
    ---------------------------------------------------------------aw- */

    [ self prepareForImageManipulation
        :   nodeStack
        :   [ ArfARTCSP class ]
        :   [ ArfARTCSP class ]
        ];

    const double SIGMA = sigma;

    const int KERNEL_RADIUS     = 5;
    const int KERNEL_SIZE       = 2*KERNEL_RADIUS + 1;
    const int KERNEL_ARRAY_SIZE = KERNEL_SIZE * KERNEL_SIZE;

    double weights[KERNEL_ARRAY_SIZE];
    
    for ( int j = 0; j < KERNEL_SIZE; j++ )
    for ( int i = 0; i < KERNEL_SIZE; i++ )
        weights[j*KERNEL_SIZE + i] = GEXP2( i * 2.0 * SIGMA / (2*KERNEL_RADIUS) - 1.0, 
                                            j * 2.0 * SIGMA / (2*KERNEL_RADIUS) - 1.0,
                                            SIGMA
                                            );
    
    // Debug print
    //printf("\n");
    //for ( int j = 0; j < KERNEL_SIZE; j++ )
    //{
    //    for ( int i = 0; i < KERNEL_SIZE; i++ )
    //        printf("%f, ", weights[j*KERNEL_SIZE + i]);
    //    printf("\n");
    //}
    //fflush(stdout);
    
    /* ------------------------------------------------------------------
         Process all pixels in the image.
    ---------------------------------------------------------------aw- */

    for ( int i = 0; i < numberOfSourceImages; i++ )
    {
        for ( long y = 0; y < YC(destinationImageSize); y++ )
        {
            for ( long x = 0; x < XC(destinationImageSize); x++ )
            {
                double sumX = 0.0;
                double sumY = 0.0;
                double sumZ = 0.0;

                double norm = 0.0;
                
                for ( int hy = -KERNEL_RADIUS; hy <= KERNEL_RADIUS; hy++ )
                if ( y + hy >= 0 && y + hy < YC(destinationImageSize) )
                {
                    [ self loadSourceScanlineBuffer: i : y + hy ];
                    
                    for ( int hx = -KERNEL_RADIUS; hx <= KERNEL_RADIUS; hx++ )
                    {
                        if ( x + hx >= 0 && x + hx < XC(destinationImageSize) )
                        {
                            const double weight = weights[INDEX(hx, hy, KERNEL_RADIUS)]; 

                            sumX += weight * ARCIEXYZ_X(XYZA_SOURCE_BUFFER_XYZ(x + hx));
                            sumY += weight * ARCIEXYZ_Y(XYZA_SOURCE_BUFFER_XYZ(x + hx));
                            sumZ += weight * ARCIEXYZ_Z(XYZA_SOURCE_BUFFER_XYZ(x + hx));

                            norm += weight;
                        }
                    }
                }
                
                [ self loadSourceScanlineBuffer: i : y ];

                ARCIEXYZ_X(XYZA_DESTINATION_BUFFER_XYZ(x)) = sumX / norm;
                ARCIEXYZ_Y(XYZA_DESTINATION_BUFFER_XYZ(x)) = sumY / norm;
                ARCIEXYZ_Z(XYZA_DESTINATION_BUFFER_XYZ(x)) = sumZ / norm;

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
        :   nodeStack
        ];

    [ REPORTER endAction ];
}

@end

// ===========================================================================
