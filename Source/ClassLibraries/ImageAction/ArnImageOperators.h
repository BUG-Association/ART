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

ART_MODULE_INTERFACE(ArnImageOperators)

#import "ART_ImageData.h"

@interface ArnPlainImage (Dithering)

- (ArnRGBA32Image *) fractionDitheredRGBA32Image
        : (ArnRGBA32Image *) ditherImage
        : (ArcObject <ArpReporter> *) reporter
        ;

@end

@interface ArnCIEXYZAImage (Tonemapping)

- (void) gammaTransform
        : (double) gamma
        : (ArcObject <ArpReporter> *) reporter
        ;

- (void) exponentialMapping
        : (double) mappingValue
        : (ArcImageMetrics *) metrics
        : (ArcObject <ArpReporter> *) reporter
        ;

- (void) meanValueMapping
        : (double) mappingValue
        : (ArcImageMetrics *) metrics
        : (ArcObject <ArpReporter> *) reporter
        ;

- (void) interactiveCalibration
        : (double) aperture
        : (double) contrast
        : (ArcImageMetrics *) metrics
        : (ArcObject <ArpReporter> *) reporter
        ;

- (void) schlicksMapping
        : (double) p_in
        : (ArcImageMetrics *) metrics
        : (ArcObject <ArpReporter> *) reporter
        ;

- (void) wardsMapping
        : (double) lwa_in
        : (double) ldmax
        : (ArcImageMetrics *) metrics
        : (ArcObject <ArpReporter> *) reporter
        ;

- (void) tumblinRushmeierMapping
        : (double) lwa_in
        : (double) ldmax
        : (double) cmax
        : (ArcImageMetrics *) metrics
        : (ArcObject <ArpReporter> *) reporter
        ;

@end

// ===========================================================================
