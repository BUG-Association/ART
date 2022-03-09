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

ART_MODULE_INTERFACE(ArnImageSamplerRGBA)

#import "ART_Scenegraph.h"
#import "ART_ImageData.h"
#import "ArnImageSamplerBase.h"

/* ===========================================================================
    'ArnImageSampler'
=========================================================================== */

@interface ArnImageSamplerRGBA
        : ArnImageSamplerBase
        < ArpImageSampler, ArpAction >
{
    //   Resouces of which there has to be one per rendering thread
    
    ArNode <ArpPathspaceIntegratorRGBA>  ** pathspaceIntegrator;
    ArnRGBAImage                         * resultImage;
    
    //   Internal variables that are used during rendering
}

- (id) init
        : (ArNode <ArpPathspaceIntegratorRGBA> * ) newPathspaceIntegrator
        ;

- (id) init
        : (ArNode <ArpPathspaceIntegratorRGBA> * ) newPathspaceIntegrator
        : (unsigned int) newNumberOfSamplesPerPixel
        ;

@end


// ===========================================================================
