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

#ifndef _ART_FOUNDATION_LIGHTANDATTENUATION_ARSTOKESVECTOR_SAMPLE_H_
#define _ART_FOUNDATION_LIGHTANDATTENUATION_ARSTOKESVECTOR_SAMPLE_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArStokesVectorSample)

#include "ArSpectralSample.h"

#include "ArMuellerMatrixSample.h"

/* ---------------------------------------------------------------------------

    'ArStokesVectorSample' struct

    The basic data structure used to describe polarised light. The first
    component encodes the absolute radiance, while the other three contain
    relative values which describe the degreee of linear and circular
    polarisation in the given sample.

    Note that this struct is by itself insufficient to fully characterise
    light in a rendering setting, since it still the lacks geometric
    orientation provided by an ArReferenceFrame. If the renderer is switched
    to polarisation-aware mode both are used as parts of the main ArLightSample
    struct (see "ArPolarisableLight.h" for details).


    'ARSVS_I' accessor macro

    Used to cleanly access the individual components of a given Stokes vector.


    'arstokesvectorsample_sh_debugprintf'

    Outputs the components of the Stokes vector to stdout. Used for printf
    debugging purposes only.

------------------------------------------------------------------------aw- */


typedef struct ArStokesVectorSample
{
    ArSpectralSample  c[4];
}
ArStokesVectorSample;

#define ARSTOKESVECTORSAMPLE_SV_I(__sc,__i)     ((__sc).c[(__i)])

#define ARSVS_I                                 ARSTOKESVECTORSAMPLE_SV_I

ArStokesVectorSample * arstokesvectorsample_alloc(
        const ART_GV  * art_gv
        );

void arstokesvectorsample_free(
        const ART_GV                * art_gv,
              ArStokesVectorSample  * sr
        );

void arstokesvectorsample_s_init_depolarised_sv(
        const ART_GV                * art_gv,
        const ArSpectralSample      * c0,
              ArStokesVectorSample  * sr
        );

void arstokesvectorsample_ssss_init_sv(
        const ART_GV                * art_gv,
        const ArSpectralSample      * c0,
        const ArSpectralSample      * c1,
        const ArSpectralSample      * c2,
        const ArSpectralSample      * c3,
              ArStokesVectorSample  * sr
        );

void arstokesvectorsample_dddd_init_sv(
        const ART_GV                * art_gv,
              double                  d0,
              double                  d1,
              double                  d2,
              double                  d3,
              ArStokesVectorSample  * svr
        );

void arstokesvectorsample_sv_init_sv(
        const ART_GV                * art_gv,
        const ArStokesVectorSample  * s0,
              ArStokesVectorSample  * sr
        );

void arstokesvectorsample_d_mul_sv(
        const ART_GV                 * art_gv,
        const double                   d0,
              ArStokesVectorSample   * sr
        );

void arstokesvectorsample_sv_add_sv(
        const ART_GV                 * art_gv,
        const ArStokesVectorSample   * s0,
              ArStokesVectorSample   * sr
        );

void arstokesvectorsample_sv_mm_mul_sv(
        const ART_GV                 * art_gv,
        const ArStokesVectorSample   * s0,
        const ArMuellerMatrixSample  * mm0,
              ArStokesVectorSample   * sr
        );

unsigned int arstokesvectorsample_sv_sv_equal(
        const ART_GV                * art_gv,
        const ArStokesVectorSample  * s0,
        const ArStokesVectorSample  * s1
        );

unsigned int arstokesvectorsample_sv_sv_d_equal(
        const ART_GV                * art_gv,
        const ArStokesVectorSample  * s0,
        const ArStokesVectorSample  * s1,
        const double                  max_relative_error
        );

unsigned int arstokesvectorsample_sv_valid(
        const ART_GV                * art_gv,
        const ArStokesVectorSample  * s0
        );

void arstokesvectorsample_sv_debugprintf(
        const ART_GV                * art_gv,
        const ArStokesVectorSample  * s0
        );


#endif /* _ART_FOUNDATION_LIGHTANDATTENUATION_ARSTOKESVECTOR_SAMPLE_H_ */
// ===========================================================================
