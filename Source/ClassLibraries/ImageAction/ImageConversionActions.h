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

ART_MODULE_INTERFACE(ImageConversionActions)

#import "ART_ColourAndSpectra.h"
#import "ART_ImageData.h"
#import "ART_ImageFileFormat.h"

#import "ArnSingleImageManipulationAction.h"

/* ===========================================================================
    'ArnImageConverter_RAW_To_ARTCSP'
=========================================================================== */

@interface ArnImageConverter_RAW_To_ARTCSP
        : ArnSingleImageManipulationAction
        < ArpConcreteClass, ArpAction >
{
}

@end

#define RAW_CONVERSION_WAVELENGTH_SAMPLE_TAG     "monochrome"

/* ===========================================================================
    'ArnImageConverter_RAW_To_Monochrome_ARTCSP'
=========================================================================== */

@interface ArnImageConverter_RAW_To_Monochrome_ARTCSP
        : ArnSingleImageManipulationAction
        < ArpConcreteClass, ArpAction, ArpCoding >
{
    double  wavelength;
}


- (id) wavelength
        : (double) newWavelength
        ;

@end


/* ===========================================================================
    'ArnImageConverter_TIFF_To_ARTCSP'
=========================================================================== */

@interface ArnImageConverter_TIFF_To_ARTCSP
        : ArnSingleImageManipulationAction
        < ArpCoding, ArpConcreteClass, ArpAction >
{
}

- (id) removeSource
        : (BOOL) newRemoveOption
        ;

- (id) init
        : (BOOL) newRemoveOption
        ;

@end

/* ===========================================================================
    'ArnImageConverter_ARTCSP_To_TIFF'
=========================================================================== */

@interface ArnImageConverter_ARTCSP_To_TIFF
        : ArnSingleImageManipulationAction
        < ArpCoding, ArpConcreteClass, ArpAction >
{
    unsigned int  destinationBitsPerChannel;
}

- (id) removeSource
                        : (BOOL) newRemoveOption
        bitsPerChannel  : (unsigned int) newNumberOfBits
        ;

- (id) removeSource
                        : (BOOL) newRemoveOption
        colourSpace     : (ArNode <ArpColourSpace> *) newColourSpace
        bitsPerChannel  : (unsigned int) newNumberOfBits
        ;

- (id) init
        : (BOOL) newRemoveOption
        : (ArNode <ArpColourSpace> *) newColourSpace
        : (ArnColourTransform *) newColourTransform
        : (unsigned int) newNumberOfBits
        ;

@end

/* ===========================================================================
    'ArnImageConverter_RAW_To_Singlechannel_ARTGSC'
=========================================================================== */

@interface ArnImageConverter_RAW_To_Singlechannel_ARTGSC
        : ArnSingleImageManipulationAction
        < ArpCoding, ArpConcreteClass, ArpAction >
{
    BOOL    normalise;
    double  wavelength;
}

- (id) removeSource
                  : (BOOL) newRemoveOption
        normalise : (BOOL) newNormalise
        wavelength: (double) newWavelength
        ;

- (id) init
        : (BOOL) newRemoveOption
        : (BOOL) newNormalise
        : (double) newWavelength
        ;

@end

/* ===========================================================================
    'ArnImageConverter_RAW_To_Singlechannel_ARTGSCs'
=========================================================================== */

@interface ArnImageConverter_RAW_To_Singlechannel_ARTGSCs
        : ArnSingleImageManipulationAction
        < ArpCoding, ArpConcreteClass, ArpAction >
{
    unsigned int  stokesComponentsToOutput;
    unsigned int  componentsPerInputImage;
    unsigned int  stokesComponentID[4];
}

- (id) removeSource
                                : (BOOL) newRemoveOption
        stokesComponentsToOutput: (int) newStokesComponentsToOutput
        ;

- (id) init
        : (BOOL) newRemoveOption
        : (int) newStokesComponentsToOutput
        ;

@end

/* ===========================================================================
    'ArnImageConverter_ARTGSC_To_TIFF'
=========================================================================== */

#define ARTGSC_COMPUTE_MAX_VALUE            -1.0
#define ARTGSC_NO_REFERENCE_SCALE            0
#define ARTGSC_NO_TICKMARKS                  0

@interface ArnImageConverter_ARTGSC_To_TIFF
        : ArnSingleImageManipulationAction
        < ArpCoding, ArpConcreteClass, ArpAction >
{
    unsigned int  destinationBitsPerChannel;
    BOOL          outputFalsecolour;
    BOOL          plusMinus;
    double        maxValue;
    unsigned int  scaleWidth;
    unsigned int  scaleTickmarks;
}

- (id) removeSource
                        : (BOOL) newDeleteSourceImageAfterUse
        bitsPerChannel  : (unsigned int) newDestinationBitsPerChannel
        falsecolour     : (BOOL) newFalsecolourOption
        plusMinus       : (BOOL) newPlusMinusOption
        maxValue        : (double) newMaxValue
        scaleWidth      : (unsigned int) newScaleWidth
        scaleTickmarks  : (unsigned int) newScaleTickmarks
        ;

- (id) init
        : (BOOL) newDeleteSourceImageAfterUse
        : (unsigned int) newDestinationBitsPerChannel
        : (BOOL) newFalsecolourOption
        : (BOOL) newPlusMinusOption
        : (double) newMaxValue
        : (unsigned int) newScaleWidth
        : (unsigned int) newScaleTickmarks
        ;

@end

/* ===========================================================================
    'ArnImageConverter_ARTGSC_To_GreyCSV'
=========================================================================== */

@interface ArnImageConverter_ARTGSC_To_GreyCSV
        : ArnSingleImageManipulationAction
        < ArpCoding, ArpConcreteClass, ArpAction >
{
}

- (id) removeSource
        : (BOOL) newRemoveOption
        ;

- (id) init
        : (BOOL) newRemoveOption
        ;

@end


// ===========================================================================


#ifdef ART_WITH_OPENEXR

/* ===========================================================================
    'ArnImageConverter_RAW_To_SPECTRAL_EXR'
=========================================================================== */

@interface ArnImageConverter_RAW_To_Spectral_EXR
        : ArnSingleImageManipulationAction
        < ArpConcreteClass, ArpAction >
{
}

@end

/* ===========================================================================
    'ArnImageConverter_ARTCSP_To_EXR'
=========================================================================== */

@interface ArnImageConverter_ARTCSP_To_EXR
        : ArnSingleImageManipulationAction
        < ArpCoding, ArpConcreteClass, ArpAction >
{
}

- (id) removeSource
        : (BOOL) newRemoveOption
        ;

- (id) removeSource
                        : (BOOL) newRemoveOption
        colourSpace     : (ArNode <ArpColourSpace> *) newColourSpace
        ;

- (id) init
        : (BOOL) newRemoveOption
        : (ArNode <ArpColourSpace> *) newColourSpace
        : (ArnColourTransform *) newColourTransform
        ;

@end

/* ===========================================================================
    'ArnImageConverter_EXR_To_ARTCSP'
=========================================================================== */

@interface ArnImageConverter_EXR_To_ARTCSP
        : ArnSingleImageManipulationAction
        < ArpCoding, ArpConcreteClass, ArpAction >
{
}

- (id) removeSource
        : (BOOL) newRemoveOption
        ;

- (id) init
        : (BOOL) newRemoveOption
        ;

@end

/* ===========================================================================
    'ArnImageConverter_ARTGSC_To_EXR'
=========================================================================== */

@interface ArnImageConverter_ARTGSC_To_EXR
        : ArnSingleImageManipulationAction
        < ArpCoding, ArpConcreteClass, ArpAction >
{
}

- (id) removeSource
        : (BOOL) newRemoveOption
        ;

- (id) init
        : (BOOL) newRemoveOption
        ;

@end

#endif // ART_WITH_OPENEXR

