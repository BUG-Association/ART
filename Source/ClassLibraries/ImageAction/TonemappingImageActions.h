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

ART_MODULE_INTERFACE(TonemappingImageActions)

#import "ART_ImageData.h"

#import "ArnSingleImageManipulationAction.h"

void tonemappingimageactions_initialise();


@interface ArnExponentialToneMapper
        : ArnSingleImageManipulationAction
        < ArpCoding, ArpConcreteClass, ArpAction >
{
    double  mappingValue;
}

- (id) mappingValue
        : (double) newMappingValue
        ;

- (id) init
        : (double) newMappingValue
        ;

@end


@interface ArnScaleToUnityToneMapper
        : ArnSingleImageManipulationAction
        < ArpCoding, ArpConcreteClass, ArpAction >
{
}

@end


@interface ArnInteractiveCalibrationToneMapper
        : ArnSingleImageManipulationAction
        < ArpCoding, ArpConcreteClass, ArpAction >
{
    double  aperture;
    double  contrast;
    BOOL    predefined_AB;
    double  a;
    double  b;
}

- (id) aperture
                : (double) newAperture
        contrast: (double) newContrast
        ;

- (id) a
         : (double) newA
        b: (double) newB
        ;

- (id) init
        : (double) newAperture
        : (double) newContrast
        : (BOOL) newPredefined_AB
        : (double) newA
        : (double) newB
        ;

@end


@interface ArnMonochromeInteractiveCalibrationToneMapper
        : ArnInteractiveCalibrationToneMapper
        < ArpCoding, ArpConcreteClass, ArpAction >
{
    double  wavelength;
}

- (id) aperture
                  : (double) newAperture
        wavelength: (double) newWavelength
        ;

@end


#define IAC_TONEMAPPER_DEFAULT_APERTURE       0.0
#define IAC_TONEMAPPER_DEFAULT_CONTRAST     100.0


// ===========================================================================
