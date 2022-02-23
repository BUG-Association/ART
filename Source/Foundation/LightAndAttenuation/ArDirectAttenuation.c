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

#define ART_MODULE_NAME     ArDirectAttenuation

#include "ArDirectAttenuation.h"

#include "_ArDirectAttenuation_GV.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    ArDirectAttenuation_GV  * ardirectattenuation_gv = ALLOC(ArDirectAttenuation_GV);

    ardirectattenuation_gv->act_has_been_initialised = 0;

    art_gv->ardirectattenuation_gv = ardirectattenuation_gv;
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    FREE( art_gv->ardirectattenuation_gv );
)

const char * ardirectattenuation_act_string(
        const ART_GV  * art_gv
        )
{
    return
        art_gv->ardirectattenuation_gv->_aaf_act_string(
            art_gv
            );
}

ArDirectAttenuation * ardirectattenuation_alloc(
        const ART_GV  * art_gv
        )
{
    ArDirectAttenuation  * newDirectAttenuation = ALLOC( ArDirectAttenuation );

    newDirectAttenuation->value =
        art_gv->ardirectattenuation_gv->_aaf_alloc(
            art_gv
            );

    return newDirectAttenuation;
}

void ardirectattenuation_free(
        const ART_GV               * art_gv,
              ArDirectAttenuation  * ar
        )
{
    art_gv->ardirectattenuation_gv->_aaf_free(
        art_gv,
        ar->value
        );

    FREE( ar );
}

ArDirectAttenuation * ardirectattenuation_d_alloc_init(
        const ART_GV  * art_gv,
        const double    d0
        )
{
    ArDirectAttenuation  * newDirectAttenuation = ALLOC( ArDirectAttenuation );

    newDirectAttenuation->value =
        art_gv->ardirectattenuation_gv->_aaf_d_alloc_init(
            art_gv,
            d0
            );

    return newDirectAttenuation;
}

ArDirectAttenuation *  ardirectattenuation_a_alloc_init(
        const ART_GV               * art_gv,
        const ArDirectAttenuation  * a0
        )
{
    ArDirectAttenuation  * newDirectAttenuation = ALLOC( ArDirectAttenuation );

    newDirectAttenuation->value =
        art_gv->ardirectattenuation_gv->_aaf_a_alloc_init(
            art_gv,
            a0
            );

    return newDirectAttenuation;
}

ArDirectAttenuation const * ardirectattenuation_total(
        const ART_GV  * art_gv
        )
{
    return
        art_gv->ardirectattenuation_gv->_aaf_total(
            art_gv
            );
}

ArDirectAttenuation const * ardirectattenuation_none(
        const ART_GV  * art_gv
        )
{
    return
        art_gv->ardirectattenuation_gv->_aaf_none(
            art_gv
            );
}

void ardirectattenuation_d_init_a(
        const ART_GV               * art_gv,
        const double                 d0,
              ArDirectAttenuation  * ar
        )
{
    art_gv->ardirectattenuation_gv->_aaf_d_init_a(
        art_gv,
        d0,
        ar->value
        );
}

void ardirectattenuation_drr_init_depolarising_a(
        const ART_GV               * art_gv,
        const double                 d0,
        const ArReferenceFrame     * r0,
        const ArReferenceFrame     * r1,
              ArDirectAttenuation  * ar
        )
{
    art_gv->ardirectattenuation_gv->_aaf_drr_init_depolarising_a(
        art_gv,
        d0,
        r0,
        r1,
        ar->value
        );
}

void ardirectattenuation_drr_init_nonpolarising_a(
        const ART_GV               * art_gv,
        const double                 d0,
        const ArReferenceFrame     * r0,
        const ArReferenceFrame     * r1,
              ArDirectAttenuation  * ar
        )
{
    art_gv->ardirectattenuation_gv->_aaf_drr_init_nonpolarising_a(
        art_gv,
        d0,
        r0,
        r1,
        ar->value
        );
}

void ardirectattenuation_s_init_a(
        const ART_GV               * art_gv,
        const ArSpectrum             * c0,
              ArDirectAttenuation  * ar
        )
{
    art_gv->ardirectattenuation_gv->_aaf_s_init_a(
        art_gv,
        c0,
        ar->value
        );
}

void ardirectattenuation_srr_init_depolarising_a(
        const ART_GV               * art_gv,
        const ArSpectrum             * c0,
        const ArReferenceFrame     * r0,
        const ArReferenceFrame     * r1,
              ArDirectAttenuation  * ar
        )
{
    art_gv->ardirectattenuation_gv->_aaf_srr_init_depolarising_a(
        art_gv,
        c0,
        r0,
        r1,
        ar->value
        );
}

void ardirectattenuation_srr_init_nonpolarising_a(
        const ART_GV               * art_gv,
        const ArSpectrum             * c0,
        const ArReferenceFrame     * r0,
        const ArReferenceFrame     * r1,
              ArDirectAttenuation  * ar
        )
{
    art_gv->ardirectattenuation_gv->_aaf_srr_init_nonpolarising_a(
        art_gv,
        c0,
        r0,
        r1,
        ar->value
        );
}

void ardirectattenuation_mm_rr_init_polarising_a(
        const ART_GV               * art_gv,
        const ArMuellerMatrix      * m0,
        const ArReferenceFrame     * r0,
        const ArReferenceFrame     * r1,
              ArDirectAttenuation  * ar
        )
{
    art_gv->ardirectattenuation_gv->_aaf_mm_rr_init_polarising_a(
        art_gv,
        m0,
        r0,
        r1,
        ar->value
        );
}

void ardirectattenuation_ddrr_init_linear_polariser_a(
        const ART_GV               * art_gv,
        const double                 d0,
        const double                 d1,
        const ArReferenceFrame     * r0,
        const ArReferenceFrame     * r1,
              ArDirectAttenuation  * ar
        )
{
    art_gv->ardirectattenuation_gv->_aaf_ddrr_init_linear_polariser_a(
        art_gv,
        d0,
        d1,
        r0,
        r1,
        ar->value
        );
}

void ardirectattenuation_a_init_s(
        const ART_GV               * art_gv,
        const ArDirectAttenuation  * a0,
              ArSpectrum             * cr
        )
{
    art_gv->ardirectattenuation_gv->_aaf_a_init_s(
        art_gv,
        a0->value,
        cr
        );
}

void ardirectattenuation_a_init_a(
        const ART_GV               * art_gv,
        const ArDirectAttenuation  * a0,
              ArDirectAttenuation  * ar
        )
{
    art_gv->ardirectattenuation_gv->_aaf_a_init_a(
        art_gv,
        a0->value,
        ar->value
        );
}

double ardirectattenuation_ai(
        const ART_GV               * art_gv,
        const ArDirectAttenuation  * a0,
        const unsigned int           i0
        )
{
    return
        art_gv->ardirectattenuation_gv->_aaf_ai(
            art_gv,
            a0->value,
            i0
            );
}

void ardirectattenuation_set_aid(
        const ART_GV               * art_gv,
        const ArDirectAttenuation  * a0,
        const unsigned int           i0,
        const double                 d0
        )
{
    return
        art_gv->ardirectattenuation_gv->_aaf_set_aid(
            art_gv,
            a0->value,
            i0,
            d0
            );
}

unsigned int ardirectattenuation_a_polarising(
        const ART_GV               * art_gv,
        const ArDirectAttenuation  * a0
        )
{
    return
        art_gv->ardirectattenuation_gv->_aaf_a_polarising(
            art_gv,
            a0->value
            );
}

ArReferenceFrame const * ardirectattenuation_a_entry_rf(
        const ART_GV               * art_gv,
        const ArDirectAttenuation  * a0
        )
{
    return
        art_gv->ardirectattenuation_gv->_aaf_a_entry_rf(
            art_gv,
            a0->value
            );
}

ArReferenceFrame const * ardirectattenuation_a_exit_rf(
        const ART_GV               * art_gv,
        const ArDirectAttenuation  * a0
        )
{
    return
        art_gv->ardirectattenuation_gv->_aaf_a_exit_rf(
            art_gv,
            a0->value
            );
}

void ardirectattenuation_a_to_mm(
        const ART_GV               * art_gv,
        const ArDirectAttenuation  * a0,
              ArMuellerMatrix      * mm
        )
{
    art_gv->ardirectattenuation_gv->_aaf_a_to_mm(
        art_gv,
        a0->value,
        mm
        );
}

void ardirectattenuation_a_realign_to_coaxial_exit_rf_mm(
        const ART_GV               * art_gv,
        const ArDirectAttenuation  * a0,
        const ArReferenceFrame     * r0,
              ArMuellerMatrix      * mm
        )
{
    art_gv->ardirectattenuation_gv->_aaf_a_realign_to_coaxial_exit_rf_mm(
        art_gv,
        a0->value,
        r0,
        mm
        );
}

void ardirectattenuation_realign_to_coaxial_exit_rf_a(
        const ART_GV               * art_gv,
        const ArReferenceFrame     * r0,
              ArDirectAttenuation  * ar
        )
{
    art_gv->ardirectattenuation_gv->_aaf_realign_to_coaxial_exit_rf_a(
        art_gv,
        r0,
        ar->value
        );
}

void ardirectattenuation_a_realign_to_coaxial_exit_rf_a(
        const ART_GV               * art_gv,
        const ArDirectAttenuation  * a0,
        const ArReferenceFrame     * r0,
              ArDirectAttenuation  * ar
        )
{
    art_gv->ardirectattenuation_gv->_aaf_a_realign_to_coaxial_exit_rf_a(
        art_gv,
        a0->value,
        r0,
        ar->value
        );
}

double ardirectattenuation_a_avg(
        const ART_GV               * art_gv,
        const ArDirectAttenuation  * a0
        )
{
    return
        art_gv->ardirectattenuation_gv->_aaf_a_avg(
            art_gv,
            a0->value
            );
}

double ardirectattenuation_a_norm(
        const ART_GV               * art_gv,
        const ArDirectAttenuation  * a0
        )
{
    return
        art_gv->ardirectattenuation_gv->_aaf_a_norm(
            art_gv,
            a0->value
            );
}

void ardirectattenuation_inv_a(
        const ART_GV               * art_gv,
              ArDirectAttenuation  * ar
        )
{
    art_gv->ardirectattenuation_gv->_aaf_inv_a(
        art_gv,
        ar->value
        );
}

void ardirectattenuation_a_inv_a(
        const ART_GV               * art_gv,
        const ArDirectAttenuation  * a0,
              ArDirectAttenuation  * ar
        )
{
    art_gv->ardirectattenuation_gv->_aaf_a_inv_a(
        art_gv,
        a0->value,
        ar->value
        );
}

double ardirectattenuation_a_max(
        const ART_GV               * art_gv,
        const ArDirectAttenuation  * a0
        )
{
    return
        art_gv->ardirectattenuation_gv->_aaf_a_max(
            art_gv,
            a0->value
            );
}

void ardirectattenuation_aa_max_a(
        const ART_GV               * art_gv,
        const ArDirectAttenuation  * a0,
        const ArDirectAttenuation  * a1,
              ArDirectAttenuation  * ar
        )
{
    art_gv->ardirectattenuation_gv->_aaf_aa_max_a(
        art_gv,
        a0->value,
        a1->value,
        ar->value
        );
}

double ardirectattenuation_a_min(
        const ART_GV               * art_gv,
        const ArDirectAttenuation  * a0
        )
{
    return
        art_gv->ardirectattenuation_gv->_aaf_a_min(
            art_gv,
            a0->value
            );
}

void ardirectattenuation_aa_min_a(
        const ART_GV               * art_gv,
        const ArDirectAttenuation  * a0,
        const ArDirectAttenuation  * a1,
              ArDirectAttenuation  * ar
        )
{
    art_gv->ardirectattenuation_gv->_aaf_aa_min_a(
        art_gv,
        a0->value,
        a1->value,
        ar->value
        );
}

double ardirectattenuation_aa_maxdiff(
        const ART_GV               * art_gv,
        const ArDirectAttenuation  * a0,
        const ArDirectAttenuation  * a1
        )
{
    return
        art_gv->ardirectattenuation_gv->_aaf_aa_maxdiff(
            art_gv,
            a0->value,
            a1->value
            );
}

void ardirectattenuation_a_add_a(
        const ART_GV               * art_gv,
        const ArDirectAttenuation  * a0,
              ArDirectAttenuation  * ar
        )
{
    art_gv->ardirectattenuation_gv->_aaf_a_add_a(
        art_gv,
        a0->value,
        ar->value
        );
}

void ardirectattenuation_aa_add_a(
        const ART_GV               * art_gv,
        const ArDirectAttenuation  * a0,
        const ArDirectAttenuation  * a1,
              ArDirectAttenuation  * ar
        )
{
    art_gv->ardirectattenuation_gv->_aaf_aa_add_a(
        art_gv,
        a0->value,
        a1->value,
        ar->value
        );
}

void ardirectattenuation_aa_sub_a(
        const ART_GV               * art_gv,
        const ArDirectAttenuation  * a0,
        const ArDirectAttenuation  * a1,
              ArDirectAttenuation  * ar
        )
{
    art_gv->ardirectattenuation_gv->_aaf_aa_sub_a(
        art_gv,
        a0->value,
        a1->value,
        ar->value
        );
}

void ardirectattenuation_d_mul_a(
        const ART_GV               * art_gv,
        const double                 d0,
              ArDirectAttenuation  * ar
        )
{
    art_gv->ardirectattenuation_gv->_aaf_d_mul_a(
        art_gv,
        d0,
        ar->value
        );
}

void ardirectattenuation_da_mul_a(
        const ART_GV               * art_gv,
        const double                 d0,
        const ArDirectAttenuation  * a0,
              ArDirectAttenuation  * ar
        )
{
    art_gv->ardirectattenuation_gv->_aaf_da_mul_a(
        art_gv,
        d0,
        a0->value,
        ar->value
        );
}

void ardirectattenuation_a_mul_a(
        const ART_GV               * art_gv,
        const ArDirectAttenuation  * a0,
              ArDirectAttenuation  * ar
        )
{
    art_gv->ardirectattenuation_gv->_aaf_a_mul_a(
        art_gv,
        a0->value,
        ar->value
        );
}

void ardirectattenuation_aa_mul_a(
        const ART_GV               * art_gv,
        const ArDirectAttenuation  * a0,
        const ArDirectAttenuation  * a1,
              ArDirectAttenuation  * ar
        )
{
    art_gv->ardirectattenuation_gv->_aaf_aa_mul_a(
        art_gv,
        a0->value,
        a1->value,
        ar->value
        );
}

void ardirectattenuation_d_div_a(
        const ART_GV         * art_gv,
        const double           d0,
              ArDirectAttenuation  * ar
        )
{
    art_gv->ardirectattenuation_gv->_aaf_d_div_a(
        art_gv,
        d0,
        ar->value
        );
}

void ardirectattenuation_da_div_a(
        const ART_GV               * art_gv,
        const double                 d0,
        const ArDirectAttenuation  * a0,
              ArDirectAttenuation  * ar
        )
{
    art_gv->ardirectattenuation_gv->_aaf_da_div_a(
        art_gv,
        d0,
        a0->value,
        ar->value
        );
}

void ardirectattenuation_da_pow_a(
        const ART_GV               * art_gv,
        const double                 d0,
        const ArDirectAttenuation  * a0,
              ArDirectAttenuation  * ar
        )
{
    art_gv->ardirectattenuation_gv->_aaf_da_pow_a(
        art_gv,
        d0,
        a0->value,
        ar->value
        );
}

void ardirectattenuation_da_negpow_a(
        const ART_GV               * art_gv,
        const double                 d0,
        const ArDirectAttenuation  * a0,
              ArDirectAttenuation  * ar
        )
{
    art_gv->ardirectattenuation_gv->_aaf_da_negpow_a(
        art_gv,
        d0,
        a0->value,
        ar->value
        );
}

void ardirectattenuation_dda_clamp_a(
        const ART_GV               * art_gv,
        const double                 d0,
        const double                 d1,
        const ArDirectAttenuation  * a0,
              ArDirectAttenuation  * ar
        )
{
    art_gv->ardirectattenuation_gv->_aaf_dda_clamp_a(
        art_gv,
        d0,
        d1,
        a0->value,
        ar->value
        );
}

void ardirectattenuation_da_mul_add_a(
        const ART_GV               * art_gv,
        const double                 d0,
        const ArDirectAttenuation  * a0,
              ArDirectAttenuation  * ar
        )
{
    art_gv->ardirectattenuation_gv->_aaf_da_mul_add_a(
        art_gv,
        d0,
        a0->value,
        ar->value
        );
}

void ardirectattenuation_daa_interpol_a(
        const ART_GV               * art_gv,
        const double                 d0,
        const ArDirectAttenuation  * a0,
        const ArDirectAttenuation  * a1,
              ArDirectAttenuation  * ar
        )
{
    art_gv->ardirectattenuation_gv->_aaf_daa_interpol_a(
        art_gv,
        d0,
        a0->value,
        a1->value,
        ar->value
        );
}

void ardirectattenuation_a_complement_from_one_a(
        const ART_GV               * art_gv,
        const ArDirectAttenuation  * a0,
              ArDirectAttenuation  * ar
        )
{
    art_gv->ardirectattenuation_gv->_aaf_a_complement_from_one_a(
        art_gv,
        a0->value,
        ar->value
        );
}

void ardirectattenuation_ia_singleband_complement_from_one_a(
        const ART_GV               * art_gv,
        const unsigned int           i0,
        const ArDirectAttenuation  * a0,
              ArDirectAttenuation  * ar
        )
{
    art_gv->ardirectattenuation_gv->_aaf_ia_singleband_complement_from_one_a(
        art_gv,
        i0,
        a0->value,
        ar->value
        );
}

void ardirectattenuation_dia_singleband_mul_copy_a(
        const ART_GV               * art_gv,
        const double                 d0,
        const unsigned int           i0,
        const ArDirectAttenuation  * a0,
              ArDirectAttenuation  * ar
        )
{
    art_gv->ardirectattenuation_gv->_aaf_dia_singleband_mul_copy_a(
        art_gv,
        d0,
        i0,
        a0->value,
        ar->value
        );
}

void ardirectattenuation_a_mul_l(
        const ART_GV               * art_gv,
        const ArDirectAttenuation  * a0,
              ArLight              * lr
        )
{
    art_gv->ardirectattenuation_gv->_aaf_a_mul_l(
        art_gv,
        a0->value,
        lr->value
        );
}

void ardirectattenuation_al_mul_l(
        const ART_GV               * art_gv,
        const ArDirectAttenuation  * a0,
        const ArLight              * l0,
              ArLight              * lr
        )
{
    art_gv->ardirectattenuation_gv->_aaf_al_mul_l(
        art_gv,
        a0->value,
        l0->value,
        lr->value
        );
}

void ardirectattenuation_ax_mul_l(
        const ART_GV               * art_gv,
        const ArDirectAttenuation  * a0,
        const ArCrosstalk          * x0,
              ArLight              * lr
        )
{
    art_gv->ardirectattenuation_gv->_aaf_ax_mul_l(
        art_gv,
        a0->value,
        x0,
        lr->value
        );
}

void ardirectattenuation_axl_mul_l(
        const ART_GV         * art_gv,
        const ArDirectAttenuation  * a0,
        const ArCrosstalk    * x0,
        const ArLight        * l0,
              ArLight        * lr
        )
{
    art_gv->ardirectattenuation_gv->_aaf_axl_mul_l(
        art_gv,
        a0->value,
        x0,
        l0->value,
        lr->value
        );
}

void ardirectattenuation_a_mul_i(
        const ART_GV               * art_gv,
        const ArDirectAttenuation  * a0,
              ArLightIntensity     * lr
        )
{
    art_gv->ardirectattenuation_gv->_aaf_a_mul_i(
        art_gv,
        a0->value,
        lr
        );
}

void ardirectattenuation_ai_mul_i(
        const ART_GV               * art_gv,
        const ArDirectAttenuation  * a0,
        const ArLightIntensity     * l0,
              ArLightIntensity     * lr
        )
{
    art_gv->ardirectattenuation_gv->_aaf_ai_mul_i(
        art_gv,
        a0->value,
        l0,
        lr
        );
}

void ardirectattenuation_ax_mul_i(
        const ART_GV               * art_gv,
        const ArDirectAttenuation  * a0,
        const ArCrosstalk          * x0,
              ArLightIntensity     * lr
        )
{
    art_gv->ardirectattenuation_gv->_aaf_ax_mul_i(
        art_gv,
        a0->value,
        x0,
        lr
        );
}

void ardirectattenuation_axi_mul_i(
        const ART_GV               * art_gv,
        const ArDirectAttenuation  * a0,
        const ArCrosstalk          * x0,
        const ArLightIntensity     * l0,
              ArLightIntensity     * lr
        )
{
    art_gv->ardirectattenuation_gv->_aaf_axi_mul_i(
        art_gv,
        a0->value,
        x0,
        l0,
        lr
        );
}

unsigned int ardirectattenuation_a_valid(
        const ART_GV               * art_gv,
        const ArDirectAttenuation  * a0
        )
{
    return
        art_gv->ardirectattenuation_gv->_aaf_a_valid(
            art_gv,
            a0->value
            );
}

void ardirectattenuation_a_debugprintf(
        const ART_GV               * art_gv,
        const ArDirectAttenuation  * a0
        )
{
    printf("ArDirectAttenuation: ");

    art_gv->ardirectattenuation_gv->_aaf_a_debugprintf(
        art_gv,
        a0->value
        );
}

// ===========================================================================
