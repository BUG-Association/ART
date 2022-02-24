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

#ifndef _ART_FOUNDATION_LIGHTANDATTENUATION_ARWAVELENGTH_H_
#define _ART_FOUNDATION_LIGHTANDATTENUATION_ARWAVELENGTH_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArWavelength)

#include "ART_Foundation_Math.h"
#include "FoundationAssertionMacros.h"

typedef struct ArWavelength
{
    Crd4  c;
}
ArWavelength;

#define ARWL_C(__w)                 (__w).c
#define ARWL_WI(__w,__i)            C4_CI(ARWL_C(__w),(__i))

//  The following data is needed during the generation of every HWSS spectral
//  wavelength set. So it pays to have it around pre-computed in a single
//  struct.

typedef struct ArWavelengthSamplingData
{
    double  spectralRangeStart;
    double  spectralRangeEnd;
    double  spectralRange;
    double  spectralRangeDiv4;
}
ArWavelengthSamplingData;

//   Based on the current ISR, determine the sampling data.

void arwavelength_sampling_data_from_current_ISR_s(
        const ART_GV                    * art_gv,
              ArWavelengthSamplingData  * samplingData
        );

//   Given one wavelength, return a properly
//   initialised wavelength struct for the current ISR.

void arwavelength_d_init_w(
        const ART_GV                    * art_gv,
        const double                      wavelength_0,
              ArWavelength              * wavelength_r
        );

void arwavelength_c_init_w(
        const ART_GV                    * art_gv,
        const Crd4                      * wavelengths,
              ArWavelength              * wavelength_r
        );

//   Given one random value from the range [0,1), return a properly
//   initialised wavelength struct for the current ISR.

void arwavelength_sd_init_w(
        const ART_GV                    * art_gv,
        const ArWavelengthSamplingData  * samplingData,
        const double                      unitRandomValue,
              ArWavelength              * wavelength_r
        );
        
void arwavelength_i_deterministic_init_w(
        const ART_GV        * art_gv,
        const int             wavelengthStartIndex,
              ArWavelength  * wavelength_r
        );
        
unsigned int arwavelength_w_valid(
        const ART_GV        * art_gv,
        const ArWavelength  * w0
        );

int arwavelength_ww_equal(
        const ART_GV        * art_gv,
        const ArWavelength  * w0,
        const ArWavelength  * w1
        );

//   Debug output of the wavelength struct in human readable form.

void arwavelength_w_debugprintf(
        const ART_GV        * art_gv,
        const ArWavelength  * w0
        );

//   Shorthand versions of the function names
//   Canonical names are all good and fine, but sometimes
//   you can have too much of a good thing...

#define w_sampling_data_from_current_ISR_s \
    arwavelength_sampling_data_from_current_ISR_s



#endif /* _ART_FOUNDATION_LIGHTANDATTENUATION_ARWAVELENGTH_H_ */
/* ======================================================================== */
