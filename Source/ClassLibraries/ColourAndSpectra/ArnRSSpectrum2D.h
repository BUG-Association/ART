/* ===========================================================================

    Copyright (c) 1996-2018 The ART Development Team
    ------------------------------------------------

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

ART_MODULE_INTERFACE(ArnRSSpectrum2D)

#import "ART_Scenegraph.h"

#import "ArnConstSpectrum.h"

@interface ArnRSSpectrum2D
        : ArnValConstSpectrum < ArpConcreteClass, ArpSpectrum2D, ArpSpectrum >
{
    ArSpectrum      * mainDiagonal;
    ArCrosstalk     * crosstalk;
    ArRSSpectrum2D  * nativeValue;
    
    ArSpectrum500  * hiresMainDiagonal;
    ArCrosstalk500 * hiresCrosstalk;
    ArCrosstalk500 * hiresHorizontalSums;
    ArCrosstalk500 * hiresVerticalSums;
}

- init
        : (ArRSSpectrum2D) newNativeValue
        ;

@end

void rss2d_to_attenuation(
        const ArRSSpectrum2D  * rss,
              ArAttenuation   * attenuation
        );


ArnRSSpectrum2D * arnconstrsspectrum2Dvalue(
              ART_GV  * art_gv,
        const double    excitation_start,
        const double    excitation_step,
        const double    emission_start,
        const double    emission_step,
        const double    unit,
        ...
        );

// ===========================================================================
