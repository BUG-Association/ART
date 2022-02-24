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

#ifndef _ART_FOUNDATION_LIGHTANDATTENUATION_ARSPECTRAL_SAMPLE_H_
#define _ART_FOUNDATION_LIGHTANDATTENUATION_ARSPECTRAL_SAMPLE_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArSpectralSample)


/* ---------------------------------------------------------------------------
    NOTE: this file defines one of the internal spectral representations
          (ISRs) that ART can use for all its internal calculations which
          involve light, colour and reflectance values.
------------------------------------------------------------------------aw- */

#include "ART_Foundation_Math.h"
#include "FoundationAssertionMacros.h"
#include "ArSpectrum.h"
#include "ArWavelength.h"


typedef struct ArSpectralSample
{
    Crd4  c;
    ISR_ASSERTION_DATA
}
ArSpectralSample;

#define SPS(__sv)           ((ArSpectralSample){CRD4((__sv),0.0,0.0,0.0)})
#define SPS4(__sv)          ((ArSpectralSample){CRD4((__sv),(__sv),(__sv),(__sv))})
#define SPS_C(__sv)         (__sv).c
#define SPS_CI(__sv,__i)    C4_CI((__sv).c,(__i))

#include "SpectralDatatype_InterfaceMacros.h"

CANONICAL_INTERFACE_FOR_ISR_AND_SAMPLES( ArSpectralSample, sps, s );

void sps_sw_init_s(
        const ART_GV            * art_gv,
        const ArSpectrum        * s0,
        const ArWavelength      * w0,
              ArSpectralSample  * sr
        );

void sps_s500w_init_s(
        const ART_GV            * art_gv,
        const ArSpectrum500     * s0,
        const ArWavelength      * w0,
              ArSpectralSample  * sr
        );

ArSpectralSample const * sps_one(
        const ART_GV  * art_gv
        );

ArSpectralSample const * sps_zero(
        const ART_GV  * art_gv
        );

ArSpectralSample * sps_alloc(
        const ART_GV  * art_gv
        );

ArSpectralSample * sps_d_alloc_init(
        const ART_GV  * art_gv,
        const double    d0
        );

ArSpectralSample * sps_s_alloc_init(
        const ART_GV            * art_gv,
        const ArSpectralSample  * h0
        );

unsigned int arspectralsample_s_valid(
        const ART_GV            * art_gv,
        const ArSpectralSample  * s0
        );

#define SS_ZERO     sps_zero(art_gv)
#define SS_ONE      sps_one(art_gv)

#define sps_s_valid  arspectralsample_s_valid
#define sps_c_valid  arspectralsample_s_valid

//  The following data is needed during the generation of every HWSS spectral
//  wavelength set. So it pays to have it around pre-computed in a single
//  struct.

//  This number is switchable to enable running of ART in monochrome mode,
//  which can be very useful when searching for bugs in the HWSS logic.

#define HERO_SAMPLES_TO_SPLAT       art_hero_samples_to_splat(art_gv)

unsigned int art_hero_samples_to_splat(
        const ART_GV  * art_gv
        );

void art_set_hero_samples_to_splat(
              ART_GV        * art_gv,
        const unsigned int    new_hero_samples_to_splat
        );

/*
    Currently, the spectral sample splatting logic is wired such that
    it performs bi-linear splatting of samples iff all receiving sample
    bins are of equal width (that is why it only needs
    factor0 and factor1, and such). For target spectra with unequal width
    bins, samples are just dropped into the nearest bin.
 
    If anything fancier is needed, it ought to be added here, and in the
    sample splatting functions of plain & SV light.
*/

typedef struct ArSpectralSampleSplattingData
{
    double  numChannels;            // intentionally a double, not an int
    int     lastChannelIndex;
    double  spectralRangeStart;
    double  spectralRangeEnd;
    double  channelWidth;
    
    //   Never mind that it is called 's500', and as such, could be
    //   expected to have certain properties. We still gather these
    //   when filling this struct with data.
    
    double  s500_range_start;
    double  s500_range_end;

    int     splatChannel0[500];
    int     splatChannel1[500];
    
    float   splatFactor0[500];   // the second one is one minus the first
    float   splatFactor1[500];   // but why not precompute it...
}
ArSpectralSampleSplattingData;

#define  INVALID_SPLAT_CHANNEL      -1

//   Based on the current ISR, determine the sampling data.

void sps_splatting_data_from_current_ISR_s(
        const ART_GV                         * art_gv,
              ArSpectralSampleSplattingData  * splattingData
        );

#endif /* _ART_FOUNDATION_LIGHTANDATTENUATION_ARSPECTRAL_SAMPLE_H_ */
/* ======================================================================== */
