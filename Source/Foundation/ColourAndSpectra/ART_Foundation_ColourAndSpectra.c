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

#define ART_LIBRARY_NAME     ART_Foundation_ColourAndSpectra

#include "ART_Foundation_ColourAndSpectra.h"


/* ---------------------------------------------------------------------------

    IMPORTANT
    =========

    The order in which the various "MODULE_INITIALISATIONs" are performed is
    *NOT* arbitrary. Putting these e.g. in alphabetical order would break the
    system, so just don't do that please...

------------------------------------------------------------------------aw- */


ART_LIBRARY_INITIALISATION_FUNCTION
(
    ART_PERFORM_MODULE_INITIALISATION( ArColourSpace )

    ART_PERFORM_MODULE_INITIALISATION( ArSpectrum )
    ART_PERFORM_MODULE_INITIALISATION( ArSpectralSample )
 
    ART_PERFORM_MODULE_INITIALISATION( ArCIExy )
    ART_PERFORM_MODULE_INITIALISATION( ArCIExyY )
    ART_PERFORM_MODULE_INITIALISATION( ArCIEXYZ )
    ART_PERFORM_MODULE_INITIALISATION( ArCIELab)
    ART_PERFORM_MODULE_INITIALISATION( ArCIELch)
    ART_PERFORM_MODULE_INITIALISATION( ArCIELuv )
    ART_PERFORM_MODULE_INITIALISATION( ArGrey )
    ART_PERFORM_MODULE_INITIALISATION( ArRGB )
    ART_PERFORM_MODULE_INITIALISATION( ArHSV )
    ART_PERFORM_MODULE_INITIALISATION( ArRGBA )
    ART_PERFORM_MODULE_INITIALISATION( ArRGBUInt )
    ART_PERFORM_MODULE_INITIALISATION( ArSpectrum8 )
    ART_PERFORM_MODULE_INITIALISATION( ArSpectrum11 )
    ART_PERFORM_MODULE_INITIALISATION( ArSpectrum18 )
    ART_PERFORM_MODULE_INITIALISATION( ArSpectrum46 )
    ART_PERFORM_MODULE_INITIALISATION( ArSpectrum500 )
    ART_PERFORM_MODULE_INITIALISATION( ArRSSpectrum )
    ART_PERFORM_MODULE_INITIALISATION( ArPSSpectrum )

    ART_PERFORM_MODULE_INITIALISATION( ArCMF )
    ART_PERFORM_MODULE_INITIALISATION( ArColourTransform )
    ART_PERFORM_MODULE_INITIALISATION( ArCIEColourConversions )
    ART_PERFORM_MODULE_INITIALISATION( ColourAndSpectralDataConversion )
    ART_PERFORM_MODULE_INITIALISATION( SystemWhitepoint )
    ART_PERFORM_MODULE_INITIALISATION( UpliftCoefficientCube )
)

ART_AUTOMATIC_LIBRARY_SHUTDOWN_FUNCTION


/* ======================================================================== */
