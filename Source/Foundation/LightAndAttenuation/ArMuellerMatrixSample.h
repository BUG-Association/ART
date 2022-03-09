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

#ifndef _ART_FOUNDATION_LIGHTANDATTENUATION_ARMUELLERMATRIX_SAMPLE_H_
#define _ART_FOUNDATION_LIGHTANDATTENUATION_ARMUELLERMATRIX_SAMPLE_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArMuellerMatrixSample)

#include "ArSpectralSample.h"

/* ---------------------------------------------------------------------------

    'ArMuellerMatrixSample' struct

    The basic data structure used to describe attenuation operations on
    polarised light. The (0,0) entry component encodes the non-polarising
    attenuation, while the other 15 describe the energy transfer between the
    various components of the Stokes vector that is being attenuated.

    Note that this struct is by itself insufficient to fully characterise
    attenuation in a rendering setting, since it still the lacks geometric
    orientation provided by an ArReferenceFrame.


    'ARMUELLER_S_M_I' accessor macro

    Used to cleanly access the individual components of a given Mueller
    matrix.


    'armuellermatrixsample_mii_debugprintf' and
    'armuellermatrixsample_mi_debugprintf'

    Outputs the chosen component or the center component respectively of
    the Mueller matrix to stdout. Used for printf debugging purposes only.

---------------------------------------------------------------------aw-ip- */


typedef struct ArMuellerMatrixSample
{
    ArSpectralSample  h[16];
}
ArMuellerMatrixSample;


#define ARMUELLER_S_M_I(__mm,__i)              (&(__mm).h[(__i)])
#define ARMUELLER_S_M_II(__mm,__row,__col)     (&(__mm).h[4*(__row)+(__col)])

#define MMS_I(__mm,__i)                        ((__mm).h[(__i)])
#define MMS_II(__mm,__row,__col)               ((__mm).h[4*(__row)+(__col)])

ArMuellerMatrixSample * armuellermatrixsample_alloc(
        const ART_GV  * art_gv
        );

void armuellermatrixsample_free(
        const ART_GV               * art_gv,
              ArMuellerMatrixSample  * mmr
        );

void armuellermatrixsample_m_init_m(
        const ART_GV               * art_gv,
        const ArMuellerMatrixSample  * mm0,
              ArMuellerMatrixSample  * mmr
        );

void armuellermatrixsample_d_init_depolarising_m(
        const ART_GV               * art_gv,
        const double                 d0,
              ArMuellerMatrixSample  * mm
        );

void armuellermatrixsample_dd_init_depolarising_m(
        const ART_GV               * art_gv,
        const double                 d0,
        const double                 d1,
              ArMuellerMatrixSample  * mm
        );

void armuellermatrixsample_d_init_nonpolarising_m(
        const ART_GV               * art_gv,
        const double                 d0,
              ArMuellerMatrixSample  * mm
        );

void armuellermatrixsample_h_init_depolarising_m(
        const ART_GV               * art_gv,
        const ArSpectralSample       * c0,
              ArMuellerMatrixSample  * mm
        );

void armuellermatrixsample_h_init_nonpolarising_m(
        const ART_GV               * art_gv,
        const ArSpectralSample       * c0,
              ArMuellerMatrixSample  * mm
        );

void armuellermatrixsample_dd_init_linear_polariser_m(
        const ART_GV               * art_gv,
        const double                 angle,
        const double                 strength,
              ArMuellerMatrixSample  * mm
        );

void armuellermatrixsample_d_init_phase_shifter_m(
        const ART_GV               * art_gv,
        const double                 angle,
              ArMuellerMatrixSample  * mm
        );

void armuellermatrixsample_d_init_rotator_m(
        const ART_GV               * art_gv,
        const double                 angle,
              ArMuellerMatrixSample  * mm
        );

void armuellermatrixsample_mm_add_m(
        const ART_GV               * art_gv,
        const ArMuellerMatrixSample  * mm0,
        const ArMuellerMatrixSample  * mm1,
              ArMuellerMatrixSample  * mmr
        );

void armuellermatrixsample_m_add_m(
        const ART_GV               * art_gv,
        const ArMuellerMatrixSample  * mm0,
              ArMuellerMatrixSample  * mmr
        );

void armuellermatrixsample_mm_mul_m(
        const ART_GV               * art_gv,
        const ArMuellerMatrixSample  * m0,
        const ArMuellerMatrixSample  * m1,
              ArMuellerMatrixSample  * m_r
        );

void armuellermatrixsample_md_mul_m(
        const ART_GV               * art_gv,
        const ArMuellerMatrixSample  * mm0,
        const double                 d0,
              ArMuellerMatrixSample  * mmr
        );

void armuellermatrixsample_mdd_rotate_input_m(
        const ART_GV               * art_gv,
        const ArMuellerMatrixSample  * m0,
        const double                 rot_cos,
        const double                 rot_sin,
              ArMuellerMatrixSample  * mr
        );

void armuellermatrixsample_mdd_rotate_output_m(
        const ART_GV               * art_gv,
        const ArMuellerMatrixSample  * m0,
        const double                 rot_cos,
        const double                 rot_sin,
              ArMuellerMatrixSample  * mr
        );

void armuellermatrixsample_mdddd_rotate_input_and_output_m(
        const ART_GV               * art_gv,
        const ArMuellerMatrixSample  * m0,
        const double                 entry_rot_cos,
        const double                 entry_rot_sin,
        const double                 exit_rot_cos,
        const double                 exit_rot_sin,
              ArMuellerMatrixSample  * mr
        );

unsigned int armuellermatrixsample_mmd_equal(
        const ART_GV               * art_gv,
        const ArMuellerMatrixSample  * mm0,
        const ArMuellerMatrixSample  * mm1,
        const double                 max_relative_error
        );

unsigned int armuellermatrixsample_m_valid(
        const ART_GV               * art_gv,
        const ArMuellerMatrixSample  * mm0,
              unsigned int           allow_attenuation_greater_than_one,
              unsigned int           debug_print_only
        );

void armuellermatrixsample_d_randomly_damage_m(
        const ART_GV               * art_gv,
        const double                 damage,
              ArMuellerMatrixSample  * mm
        );

void armuellermatrixsample_mi_debugprintf(
        const ART_GV               * art_gv,
        const ArMuellerMatrixSample  * mm0,
              unsigned int           precision
        );

void armuellermatrixsample_mii_debugprintf(
        const ART_GV               * art_gv,
        const ArMuellerMatrixSample  * mm0,
              unsigned int           channel,
              unsigned int           precision
        );

/* ---------------------------------------------------------------------------
    Routines for testing purposes
------------------------------------------------------------------------ip- */

void armuellermatrixsample_test_synthetic_MMs_validity(
        const ART_GV               * art_gv
        );

void armuellermatrixsample_test_damaged_MMs_validity(
        const ART_GV               * art_gv
        );

void armuellermatrixsample_test_blended_MMs_validity(
        const ART_GV               * art_gv
        );

void armuellermatrixsample_test_MM_rotation_operations(
        const ART_GV               * art_gv
        );

#endif /* _ART_FOUNDATION_LIGHTANDATTENUATION_ARMUELLERMATRIX_SAMPLE_H_ */
// ===========================================================================
