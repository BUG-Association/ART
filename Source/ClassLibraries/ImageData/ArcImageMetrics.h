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

#include "ART_Foundation.h"

ART_MODULE_INTERFACE(ArcImageMetrics)

#import "ART_Scenegraph.h"

#import "ArnPlainImage.h"

typedef enum ArcImageMetricsState
{
    arcimagemetrics_invalid    = 0,
    arcimagemetrics_luminance  = 1,
    arcimagemetrics_adaptation = 2
}
ArcImageMetricsState;

/* ===========================================================================
    'ArcImageMetrics'
        An image metrics objects stores statistics about image data.
=========================================================================== */
@interface ArcImageMetrics
        : ArcObject
{
    ArnCIEXYZAImage          * image;
    ArcObject <ArpReporter>  * reporter;
@public
    unsigned int state;
    double maximumLuminance;
    double averageLuminance;
    double minimumLuminance;
    double adaptationLuminance;
}

- (id) init
        : (ArNode <ArpImageReader> *) newImage
        : (ArcObject <ArpReporter> *) newReporter
        ;

- (double) maximumLuminance
        ;

- (double) averageLuminance
        ;

- (double) minimumLuminance
        ;

- (double) adaptationLuminance
        ;

@end

/* ===========================================================================
    'ArnCIEXYZImage (ImageMetrics)'
        Special image metrics for CIE XYZ images.
=========================================================================== */
@interface ArnCIEXYZAImage (ImageMetrics)

- (void) calculateLuminanceMetrics
        : (ArcImageMetrics *) metrics
        : (ArcObject <ArpReporter> *) reporter
        ;

- (void) calculateAdaptationMetrics
        : (ArcImageMetrics *) metrics
        : (ArcObject <ArpReporter> *) reporter
        ;

@end

// ===========================================================================
