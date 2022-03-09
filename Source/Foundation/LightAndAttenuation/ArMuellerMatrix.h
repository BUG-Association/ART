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

#ifndef _ART_FOUNDATION_LIGHTANDATTENUATION_ARMUELLERMATRIX_H_
#define _ART_FOUNDATION_LIGHTANDATTENUATION_ARMUELLERMATRIX_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArMuellerMatrix)

#include "ART_Foundation_ColourAndSpectra.h"

/* ---------------------------------------------------------------------------

    'ArMuellerMatrix' struct

    The basic data structure used to describe attenuation operations on
    polarised light. The (0,0) entry component encodes the non-polarising
    attenuation, while the other 15 describe the energy transfer between the
    various components of the Stokes vector that is being attenuated.

    Note that this struct is by itself insufficient to fully characterise
    attenuation in a rendering setting, since it still the lacks geometric
    orientation provided by an ArReferenceFrame.


    'ARMUELLER_M_I' accessor macro

    Used to cleanly access the individual components of a given Mueller
    matrix.


    'armuellermatrix_mii_debugprintf' and
    'armuellermatrix_mi_debugprintf'

    Outputs the chosen component or the center component respectively of
    the Mueller matrix to stdout. Used for printf debugging purposes only.

---------------------------------------------------------------------aw-ip- */


typedef struct ArMuellerMatrix
{
    ArSpectrum  * c[16];
}
ArMuellerMatrix;


#define ARMUELLER_M_I(__mm,__i)             (__mm).c[(__i)]
#define ARMUELLER_M_II(__mm,__row,__col)    (__mm).c[4*(__row)+(__col)]

ArMuellerMatrix * armuellermatrix_alloc(
        const ART_GV  * art_gv
        );

void armuellermatrix_free(
        const ART_GV           * art_gv,
              ArMuellerMatrix  * mmr
        );

void armuellermatrix_m_init_m(
        const ART_GV           * art_gv,
        const ArMuellerMatrix  * mm0,
              ArMuellerMatrix  * mmr
        );

void armuellermatrix_d_init_depolarising_m(
        const ART_GV           * art_gv,
        const double             d0,
              ArMuellerMatrix  * mm
        );

void armuellermatrix_dd_init_depolarising_m(
        const ART_GV           * art_gv,
        const double             d0,
        const double             d1,
              ArMuellerMatrix  * mm
        );

void armuellermatrix_d_init_nonpolarising_m(
        const ART_GV           * art_gv,
        const double             d0,
              ArMuellerMatrix  * mm
        );

void armuellermatrix_c_init_depolarising_m(
        const ART_GV           * art_gv,
        const ArSpectrum         * c0,
              ArMuellerMatrix  * mm
        );

void armuellermatrix_c_init_nonpolarising_m(
        const ART_GV           * art_gv,
        const ArSpectrum         * c0,
              ArMuellerMatrix  * mm
        );

void armuellermatrix_dd_init_linear_polariser_m(
        const ART_GV           * art_gv,
        const double             angle,
        const double             strength,
              ArMuellerMatrix  * mm
        );

void armuellermatrix_d_init_phase_shifter_m(
        const ART_GV           * art_gv,
        const double             angle,
              ArMuellerMatrix  * mm
        );

void armuellermatrix_d_init_rotator_m(
        const ART_GV           * art_gv,
        const double             angle,
              ArMuellerMatrix  * mm
        );

void armuellermatrix_mm_add_m(
        const ART_GV           * art_gv,
        const ArMuellerMatrix  * mm0,
        const ArMuellerMatrix  * mm1,
        ArMuellerMatrix        * mmr
        );

void armuellermatrix_m_add_m(
        const ART_GV           * art_gv,
        const ArMuellerMatrix  * mm0,
        ArMuellerMatrix        * mmr
        );

void armuellermatrix_mm_mul_m(
        const ART_GV           * art_gv,
        const ArMuellerMatrix  * m0,
        const ArMuellerMatrix  * m1,
              ArMuellerMatrix  * m_r
        );

void armuellermatrix_md_mul_m(
        const ART_GV           * art_gv,
        const ArMuellerMatrix  * mm0,
        const double             d0,
        ArMuellerMatrix        * mmr
        );

void armuellermatrix_mdd_rotate_input_m(
        const ART_GV                 * art_gv,
        const ArMuellerMatrix        * m0,
        const double                   rot_cos,
        const double                   rot_sin,
              ArMuellerMatrix        * mr
        );

void armuellermatrix_mdd_rotate_output_m(
        const ART_GV                 * art_gv,
        const ArMuellerMatrix        * m0,
        const double                   rot_cos,
        const double                   rot_sin,
              ArMuellerMatrix        * mr
        );

void armuellermatrix_mdddd_rotate_input_and_output_m(
        const ART_GV                 * art_gv,
        const ArMuellerMatrix        * m0,
        const double                   entry_rot_cos,
        const double                   entry_rot_sin,
        const double                   exit_rot_cos,
        const double                   exit_rot_sin,
              ArMuellerMatrix        * mr
        );

unsigned int armuellermatrix_mmd_equal(
        const ART_GV           * art_gv,
        const ArMuellerMatrix  * mm0,
        const ArMuellerMatrix  * mm1,
        const double             max_relative_error
        );

unsigned int armuellermatrix_m_valid(
        const ART_GV           * art_gv,
        const ArMuellerMatrix  * mm0,
        unsigned int             allow_attenuation_greater_than_one,
        unsigned int             debug_print_only
        );

void armuellermatrix_d_randomly_damage_m(
        const ART_GV           * art_gv,
        const double             damage,
              ArMuellerMatrix  * mm
        );

void armuellermatrix_mi_debugprintf(
        const ART_GV           * art_gv,
        const ArMuellerMatrix  * mm0,
        unsigned int             precision
        );

void armuellermatrix_mii_debugprintf(
        const ART_GV           * art_gv,
        const ArMuellerMatrix  * mm0,
        unsigned int             channel,
        unsigned int             precision
        );

/* ---------------------------------------------------------------------------
    Routines for testing purposes
------------------------------------------------------------------------ip- */

void armuellermatrix_test_synthetic_MMs_validity(
        const ART_GV           * art_gv
        );

void armuellermatrix_test_damaged_MMs_validity(
        const ART_GV           * art_gv
        );

void armuellermatrix_test_blended_MMs_validity(
        const ART_GV           * art_gv
        );

void armuellermatrix_test_MM_rotation_operations(
        const ART_GV           * art_gv
        );

#endif /* _ART_FOUNDATION_LIGHTANDATTENUATION_ARMUELLERMATRIX_H_ */
// ===========================================================================
