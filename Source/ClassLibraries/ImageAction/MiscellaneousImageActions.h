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

ART_MODULE_INTERFACE(MiscellaneousImageActions)

#import "ART_Scenegraph.h"

#import "ArnImageOperators.h"
#import "ArnSingleImageManipulationAction.h"

#define ARNARTCSP_LUMINANCE_CLIPPING_DEFAULT_WHITE_LUMINANCE     150.0

/* ---------------------------------------------------------------------------

        'ArnARTCSPLuminanceClipping' image manipulation node

        Takes an ARTCSP image, and performs luminance and chroma reduction for
        all values with L > 100.0 on a per-pixel basis; in ICC parlance this
        basically amounts to gamut mapping with a colorimetric rendering
        intent.

        This action is meant to be used after a tone mapping operator has
        been applied to the image, but before it is converted to some
        output RGB space by using ICC profiles. Its main capability is to
        generate (resp. maintain) highlights in those areas of the image which
        still are above L = 100.0 - i.e. the maximal luminance of the output
        space - after the tonemapping step.

        Its only input parameter is the so-called "whiteLuminance" value,
        which can be set to any value above 100.0. Setting it to exactly
        100.0 (or very close to this) is not recommended, since this may
        yield somewhat strange results.

        The actual luminance and chroma reduction algorithm (which is
        performed in L*a*b* space, where L = 100.0 is the maximal
        brightness) works as follows:

        - Colour values with L below 100.0 are left untouched.

        - Colour values with L between 100.0 and 'whiteLuminance' are moved
          to the gamut (i.e. to the area with L <= 100.0) on a linear
          trajectory that depends on the actual luminance value; for
          L = 100.0 the focus point towards which the colour is being moved is
          the black point ArCIELab( 0, 0, 0 ), while for L = 'whiteLuminance'
          the focus is ArCIELab( 100.0, 0, 0 ). Within this range a linear
          interpolation between these two is used.

          This approach ensures C1 continuity in colour gradients at the border
          between the displayable gamut and areas of higher luminance, while
          still creating the expected white-out in bright areas of an image.

        - Colour values with L > whiteLuminance are directly mapped to
          ArCIELab( 100.0, 0, 0 ).

------------------------------------------------------------------------aw- */

@interface ArnARTCSPLuminanceClipping
        : ArnSingleImageManipulationAction
        < ArpCoding, ArpConcreteClass, ArpAction >
{
    double  whiteLuminance;
}

- (id) removeSource
                        : (BOOL) newRemoveOption
        whiteLuminance  : (double) newWhiteLuminance
        ;

- (id) init
        : (BOOL) newRemoveOption
        : (double) newWhiteLuminance
        ;

@end

/* ===========================================================================
    'ArnImageFractionDitherer'
=========================================================================== */

@interface ArnImageFractionDitherer
        : ArNode < ArpCoding, ArpConcreteClass, ArpAction >
{
    ArSymbol ditherFileName;
}

- (id) init
        : (const char *) newDitherFileName
        ;

@end


/* ===========================================================================
    'ArnOpenImageInExternalViewer'
=========================================================================== */

@interface ArnOpenImageInExternalViewer
        : ArNode < ArpConcreteClass, ArpAction >
{
}

@end

/* ===========================================================================
    'ArnSetColourSubsystemWhitepoint'
=========================================================================== */

@interface ArnSetColourSubsystemWhitepoint
        : ArnUnary < ArpConcreteClass, ArpAction >
{
    ArSymbol  wp_desc;
}

- (id) init
        : (char *) newWP_Desc
        : (ArNode <ArpSpectrum> *) newIlluminantSpectrum
        ;

@end

/* ===========================================================================
    'ArnChangeISR_to_Match_RAW_Contents'
=========================================================================== */

#define ISR_CHANGE_PERFORM_NO_WAVELENGTH_CHECK      -1.0

@interface ArnChangeISR_to_Match_RAW_Contents
        : ArNode < ArpConcreteClass, ArpAction >
{
    double  wavelengthToCheckAgainstISRBounds;
    BOOL    requirePolarisedRAW;
}

- (id) init
        : (double) newWavelengthToCheckAgainstISRBounds
        : (BOOL) newRequirePolarisedRAW
        ;

@end

/* ===========================================================================
    'ArnOutputCurrentISR'
=========================================================================== */

@interface ArnOutputCurrentISR
        : ArNode < ArpConcreteClass, ArpAction >
{
}

@end

/* ===========================================================================
    'ArnRAW_Double_Mul_RAW'
=========================================================================== */

@interface ArnRAW_Double_Mul_RAW
        : ArnSingleImageManipulationAction
        < ArpCoding, ArpConcreteClass, ArpAction >
{
    double  factor;
    BOOL    cloneOption;
}

- (id) multiplyBy
                   : (double) newFactor
        cloneSource: (BOOL) newCloneOption
        ;

- (id) multiplyBy
                   : (double) newFactor
        ;

@end

/* ===========================================================================
    'ArnDownscaleRAW'
=========================================================================== */

@interface ArnDownscaleRAW
        : ArnSingleImageManipulationAction
        < ArpCoding, ArpConcreteClass, ArpAction >
{
    unsigned int  downscaleFactor;
}

- (id) downscaleFactor
                   : (unsigned int) newDownscaleFactor
        ;

@end

/* ===========================================================================
    'ArnFilterTinyRAWValues'
=========================================================================== */

@interface ArnFilterTinyRAWValues
        : ArnSingleImageManipulationAction
        < ArpCoding, ArpConcreteClass, ArpAction >
{
    double  thresholdValue;
    BOOL    cloneOption;
}

- (id) thresholdValue
                   : (double) newThresholdValue
        cloneSource: (BOOL) newCloneOption
        ;

- (id) thresholdValue
                   : (double) newThresholdValue
        ;

@end

/* ===========================================================================
    'ArnFilterHighDopRAWValues'
=========================================================================== */

@interface ArnFilterHighDopRAWValues
        : ArnSingleImageManipulationAction
        < ArpCoding, ArpConcreteClass, ArpAction >
{
    double  thresholdValue;
    BOOL    cloneOption;
}

- (id) thresholdValue
                   : (double) newThresholdValue
        cloneSource: (BOOL) newCloneOption
        ;

- (id) thresholdValue
                    : (double) newThresholdValue
        removeSource: (BOOL) newRemoveOption
        ;

- (id) thresholdValue
                   : (double) newThresholdValue
        ;

@end


// ===========================================================================
