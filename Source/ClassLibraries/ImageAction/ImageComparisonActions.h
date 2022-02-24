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

ART_MODULE_INTERFACE(ImageComparisonActions)

#import "ART_Scenegraph.h"

#import "ArnImageOperators.h"
#import "ArnDualImageManipulationAction.h"


/* ===========================================================================
    'Arn2xRAW_Add_RAW'
=========================================================================== */
    
@interface Arn2xRAW_Add_RAW
        : ArnDualImageManipulationAction
        < ArpCoding, ArpConcreteClass, ArpAction >
{
}
    
- (id) outputName
        : (const char *) newOutputName
        ;


- (id) init
        : (const char *) newOutputName
        ;
@end

/* ===========================================================================
    'Arn2xARTCSP_Add_ARTCSP'
=========================================================================== */

@interface Arn2xARTCSP_Add_ARTCSP
        : ArnDualImageManipulationAction
        < ArpCoding, ArpConcreteClass, ArpAction >
{
}

- (id) outputTag
        : (const char *) newOutputTag
        ;

@end

/* ===========================================================================
    'Arn2xARTCSP_AddMul_ARTCSP'
=========================================================================== */

@interface Arn2xARTCSP_AddMul_ARTCSP
        : ArnDualImageManipulationAction
        < ArpCoding, ArpConcreteClass, ArpAction >
{
}

- (id) outputTag
        : (const char *) newOutputName
        ;

@end

/* ===========================================================================
    'Arn2xARTCSP_To_ARTGSC_DifferenceImage'
=========================================================================== */

/* ---------------------------------------------------------------------------

    'ArDifferenceImageFeatures'
 
    'ardifferenceimagefeatures_default'
 
    Standard behaviour is a delta E difference image in CIEDE 2000 metric.
 
    'ardifferenceimagefeatures_luminance'
    'ardifferenceimagefeatures_chroma'
    'ardifferenceimagefeatures_hue'
 
    Luminance, chroma and hue differences according to the
    CIE 1976 colour difference formulas.
 
    'ardifferenceimagefeatures_deltaE1976'
 
    Delta E image in CIE 1976 colour difference space.
 
--------------------------------------------------------------------------- */

typedef enum ArDifferenceImageFeatures
{
    ardifferenceimagefeatures_default        = 0x00,
    ardifferenceimagefeatures_luminance      = 0x01,
    ardifferenceimagefeatures_chroma         = 0x02,
    ardifferenceimagefeatures_hue            = 0x04,
    ardifferenceimagefeatures_deltaE1976     = 0x08
}
ArDifferenceImageFeatures;

@interface Arn2xARTCSP_To_ARTGSC_DifferenceImage
        : ArnDualImageManipulationAction
        < ArpCoding, ArpConcreteClass, ArpAction >
{
    ArDifferenceImageFeatures  features;
}

- (id) outputName    : (const char *) newOutputName
             features: (ArDifferenceImageFeatures) newFeatures
        ;

- (id) init
        : (const char *) newOutputName
        : (ArDifferenceImageFeatures) newFeatures
        ;

@end

/* ===========================================================================
    'Arn2xRAW_To_ARTGSC_DifferenceImage'
=========================================================================== */

@interface Arn2xRAW_To_ARTGSC_DifferenceImage
        : ArnDualImageManipulationAction
        < ArpCoding, ArpConcreteClass, ArpAction >
{
    double  wavelength;
}

- (id) outputName      : (const char *) newOutputName
             wavelength: (double) newWavelength
        ;

- (id) init
        : (const char *) newOutputName
        : (double) newWavelength
        ;

@end

/* ===========================================================================
    'Arn2xRAW_SNR'
=========================================================================== */

@interface Arn2xRAW_SNR
        : ArnDualImageManipulationAction
        < ArpCoding, ArpConcreteClass, ArpAction >
{
    ArSymbol  resultFilename;
}

- (id) outputName
        : (const char *) newFilename
        ;

- (id) init
        : (const char *) newFilename
        ;

@end

/* ===========================================================================
    'Arn2xARTCSP_avg_diff'
=========================================================================== */

@interface Arn2xARTCSP_avg_diff
        : ArnDualImageManipulationAction
        < ArpCoding, ArpConcreteClass, ArpAction >
{
    ArSymbol  resultFilename;
}

- (id) outputName
        : (const char *) newFilename
        ;

- (id) init
        : (const char *) newFilename
        ;

@end

// ===========================================================================
