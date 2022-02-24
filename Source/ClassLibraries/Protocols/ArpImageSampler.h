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

ART_MODULE_INTERFACE(ArpImageSampler)

@protocol ArpNode;
@protocol ArpWorld;
@protocol ArpCamera;
@protocol ArpImageWriter;

@class ArNode;

@protocol ArpImageSampler

- (void) prepareForSampling
        : (ArNode <ArpWorld> *) world
        : (ArNode <ArpCamera> *) camera
        : (ArNode <ArpImageWriter> **) image
        : (int) numberOfResultImages
        ;

- (void) sampleImage
        : (ArNode <ArpWorld> *) world
        : (ArNode <ArpCamera> *) camera
        : (ArNode <ArpImageWriter> **) image
        : (int) numberOfResultImages
        ;

- (void) cleanupAfterImageSampling
        : (ArNode <ArpWorld> *) world
        : (ArNode <ArpCamera> *) camera
        : (ArNode <ArpImageWriter> **) image
        : (int) numberOfResultImages
        ;

@end

ART_MODULE_INTERFACE(ArpImageSamplerMessenger)

@protocol ArpImageSamplerMessenger

- (const char *) preSamplingMessage
        ;

- (const char *) postSamplingMessage
        ;

@end

// ===========================================================================
