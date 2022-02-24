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

#define ART_MODULE_NAME     ArcImageMetrics

#import "ArcImageMetrics.h"

#import "ArnPlainImage.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArcImageMetrics

- (id) init
        : (ArNode <ArpImageReader> *) newImage
        : (ArcObject <ArpReporter> *) newReporter
{
    self = [ super init ];

    if ( self )
    {
        reporter = newReporter;

        image =
            [ ALLOC_OBJECT(ArnCIEXYZAImage) initAsPartOf
                :   ((ArNode <ArpBasicImage> *)newImage)
                ];
    }
    
    return self;
}

- (id) copy
{
    ArcImageMetrics  * copiedInstance = [ super copy ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- (void) dealloc
{
    RELEASE_OBJECT(image);

    [ super dealloc ];
}

- (double) maximumLuminance
{
    if (!(state & arcimagemetrics_luminance))
        [image calculateLuminanceMetrics :self :reporter];
    return maximumLuminance;
}

- (double) averageLuminance
{
    if (!(state & arcimagemetrics_luminance))
        [image calculateLuminanceMetrics :self :reporter];
    return averageLuminance;
}

- (double) minimumLuminance
{
    if (!(state & arcimagemetrics_luminance))
        [image calculateLuminanceMetrics :self :reporter];
    return minimumLuminance;
}

- (double) adaptationLuminance
{
    if (!(state & arcimagemetrics_adaptation))
        [image calculateAdaptationMetrics :self :reporter];
    return adaptationLuminance;
}

@end

@implementation ArnCIEXYZAImage (ImageMetrics)

- (void) calculateLuminanceMetrics
        : (ArcImageMetrics *) metrics
        : (ArcObject <ArpReporter> *) reporter
{
    double maxLum = -MATH_HUGE_DOUBLE;
    double minLum =  MATH_HUGE_DOUBLE;
    double sumLum =  0.0;
    double sumPix =  0.0;

    BOOL   validEstimate = NO;

    [ reporter beginAction
        :   "calculating luminance metrics"
        ];

    for ( int y = 0; y < YC(size); y++ )
    {
        ArCIEXYZA * line = data + y * stride;
        for ( int x = 0; x < XC(size); x++ )
        {
            ArCIExyY  xyy;
            
            xyz_to_xyy( art_gv, & ARCIEXYZA_C(line[x]), & xyy );
            
            double pixLum = ARCIExyY_Y(xyy);
            double alpha = line[x].alpha;
            if (alpha > 0.0)
            {
                //   The overall estimate only becomes valid if at least
                //   one pixel has an alpha > 0.0

                validEstimate = YES;
                maxLum = M_MAX( maxLum, pixLum );
                minLum = M_MIN( minLum, pixLum );
            }

            sumLum += pixLum * alpha;
            sumPix += alpha;
        }
    }

    [reporter endAction];

    if ( validEstimate )
    {
        metrics->maximumLuminance = maxLum;
        metrics->minimumLuminance = minLum;
        metrics->averageLuminance = sumLum / sumPix;
    }
    else
    {
        metrics->maximumLuminance = 0.0;
        metrics->minimumLuminance = 0.0;
        metrics->averageLuminance = 0.0;
    }

    metrics->state |= arcimagemetrics_luminance;
}

- (void) calculateAdaptationMetrics
        : (ArcImageMetrics *) metrics
        : (ArcObject <ArpReporter> *) reporter
{
    long x, y;
    int nonBlacks = 0;
    double sumLogLum = 0.0;

    [ reporter beginAction
        :   "calculating adaptation luminance"
        ];

    for (y = 0; y < YC(size); y++)
    {
        ArCIEXYZA * line = data + y * stride;
        for (x = 0; x < XC(size); x++)
        {
            double pixLum = ARCIEXYZA_Y(line[x]);
            double alpha = line[x].alpha;
            if (alpha > 0 && pixLum > 0)
            {
                sumLogLum = log(pixLum);
                ++nonBlacks;
            }
        }
    }

    metrics->adaptationLuminance = exp( sumLogLum /(double)nonBlacks + 0.84 );

    [reporter endAction :"[Lwa=%g]", metrics->adaptationLuminance];

    metrics->state |= arcimagemetrics_adaptation;
}

@end

// ===========================================================================
