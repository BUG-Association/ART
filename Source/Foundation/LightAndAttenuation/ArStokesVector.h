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

#ifndef _ART_FOUNDATION_LIGHTANDATTENUATION_ARSTOKESVECTOR_H_
#define _ART_FOUNDATION_LIGHTANDATTENUATION_ARSTOKESVECTOR_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArStokesVector)

#include "ART_Foundation_ColourAndSpectra.h"

#include "ArStokesVectorSample.h"
#include "ArMuellerMatrix.h"

/* ---------------------------------------------------------------------------

    'ArStokesVector' struct

    The basic data structure used to describe polarised light. The first
    component encodes the absolute radiance, while the other three contain
    relative values which describe the degreee of linear and circular
    polarisation in the given sample.

    Note that this struct is by itself insufficient to fully characterise
    light in a rendering setting, since it still the lacks geometric
    orientation provided by an ArReferenceFrame. If the renderer is switched
    to polarisation-aware mode both are used as parts of the main ArLight
    struct (see "ArPolarisableLight.h" for details).


    'ARSV_I' accessor macro

    Used to cleanly access the individual components of a given Stokes vector.


    'arstokesvector_sc_debugprintf'

    Outputs the components of the Stokes vector to stdout. Used for printf
    debugging purposes only.

------------------------------------------------------------------------aw- */


typedef struct ArStokesVector
{
    ArSpectrum  * c[4];
}
ArStokesVector;

#define ARSTOKESVECTOR_SV_I(__sc,__i)       (__sc).c[(__i)]

#define ARSV_I                              ARSTOKESVECTOR_SV_I

ArStokesVector * arstokesvector_alloc(
        const ART_GV  * art_gv
        );

void arstokesvector_free(
        const ART_GV          * art_gv,
              ArStokesVector  * sr
        );

void arstokesvector_s_init_unpolarised_sv(
        const ART_GV          * art_gv,
        const ArSpectrum        * c0,
              ArStokesVector  * sr
        );

void arstokesvector_cccc_init_sv(
        const ART_GV          * art_gv,
        const ArSpectrum        * c0,
        const ArSpectrum        * c1,
        const ArSpectrum        * c2,
        const ArSpectrum        * c3,
              ArStokesVector  * sr
        );

void arstokesvector_dddd_init_sv(
        const ART_GV          * art_gv,
              double            d0,
              double            d1,
              double            d2,
              double            d3,
              ArStokesVector  * sr
        );

void arstokesvector_sv_init_sv(
        const ART_GV          * art_gv,
        const ArStokesVector  * s0,
              ArStokesVector  * sr
        );

void arstokesvector_sv_mm_mul_sv(
        const ART_GV           * art_gv,
        const ArStokesVector   * s0,
        const ArMuellerMatrix  * mm0,
              ArStokesVector   * sr
        );

unsigned int arstokesvector_sv_sv_equal(
        const ART_GV          * art_gv,
        const ArStokesVector  * s0,
        const ArStokesVector  * s1
        );

unsigned int arstokesvector_sv_sv_d_equal(
        const ART_GV          * art_gv,
        const ArStokesVector  * s0,
        const ArStokesVector  * s1,
        const double            max_relative_error
        );

void arstokesvector_sv_sample_at_wavelength_svs(
        const ART_GV                * art_gv,
        const ArStokesVector        * s0,
        const ArWavelength            w0,
              ArStokesVectorSample  * sr
        );

unsigned int arstokesvector_sv_valid(
        const ART_GV          * art_gv,
        const ArStokesVector  * s0
        );

void arstokesvector_sv_debugprintf(
        const ART_GV          * art_gv,
        const ArStokesVector  * s0
        );


#endif /* _ART_FOUNDATION_LIGHTANDATTENUATION_ARSTOKESVECTOR_H_ */
// ===========================================================================
