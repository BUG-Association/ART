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

ART_MODULE_INTERFACE(ArfARTGSC)

    /* ------------------------------------------------------------------
        For a detailed description of this file format see the
        comments in the accompanying implementation file (.m)!
    ---------------------------------------------------------------aw- */

#import "ArfRasterImage.h"

#define ARFARTGSC_VERSION           2.1
#define ARFARTGSC_EXTENSION         "artgsc"
#define ARFARTGSC_NO_MIN_VALUE      MATH_HUGE_DOUBLE
#define ARFARTGSC_NO_MAX_VALUE     -MATH_HUGE_DOUBLE

    /* ------------------------------------------------------------------

        Version history
        ---------------

        1.0    first release
        1.1    header omits obsolete AMS_CONFIG information
        1.2    includes DPI information
     
        ---    2.x is INCOMPATIBLE with 1.x
        
        2.0    alpha channel & optional max data value added
        2.1    optional min value added

    ---------------------------------------------------------------aw- */


@interface ArfARTGSC
           : ArfRasterImage
{
    ArGreyAlpha  * scanline;
    double         minDataValue;
    double         maxDataValue;
}

- (double) minDataValue
        ;

- (double) maxDataValue
        ;

- (void) setMinDataValue
        : (double) newMinDataValue
        ;

- (void) setMaxDataValue
        : (double) newMaxDataValue
        ;

@end


// ===========================================================================
