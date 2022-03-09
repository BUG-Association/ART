/* ===========================================================================

    Copyright (c) The ART Development Team
    --------------------------------------

    For a comprehensive list of the members of the development team, and a
    deslription of their respective contributions, see the file
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

#define ART_MODULE_NAME     ArLightSample

#include "ArLightSample.h"

#include "_ArLightSample_GV.h"
#include "FoundationAssertionMacros.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    ARLIGHTSAMPLE_GV = ALLOC(ArLightSample_GV);

    ARLIGHTSAMPLE_LCT_HAS_BEEN_INITIALISED_GV = 0;

    ARLIGHTSAMPLE_UNPOLARISED_UNIT_GV = 0;
    ARLIGHTSAMPLE_NONE_GV = 0;
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    if ( ARLIGHTSAMPLE_UNPOLARISED_UNIT_GV )
        arlightsample_free( art_gv, ARLIGHTSAMPLE_UNPOLARISED_UNIT_GV );
    if ( ARLIGHTSAMPLE_NONE_GV )
        arlightsample_free( art_gv, ARLIGHTSAMPLE_NONE_GV );

    FREE( ARLIGHTSAMPLE_GV );
)

ART_LIGHT_AND_ATTENUATION_MODULE_INITIALISATION_FUNCTION
(
    if ( ARLIGHTSAMPLE_LCT_HAS_BEEN_INITIALISED_GV )
    {
        ARLIGHTSAMPLE_UNPOLARISED_UNIT_GV =
            arlightsample_d_alloc_init_unpolarised(
                art_gv,
                1.0
                );

        ARLIGHTSAMPLE_NONE_GV =
            arlightsample_d_alloc_init_unpolarised(
                art_gv,
                0.0
                );
    }
)

ART_LIGHT_AND_ATTENUATION_MODULE_SHUTDOWN_FUNCTION
(
    if ( ARLIGHTSAMPLE_UNPOLARISED_UNIT_GV )
        arlightsample_free( art_gv, ARLIGHTSAMPLE_UNPOLARISED_UNIT_GV );
    if ( ARLIGHTSAMPLE_NONE_GV )
        arlightsample_free( art_gv, ARLIGHTSAMPLE_NONE_GV );
)

const char * arlightsample_lct_string(
        const ART_GV  * art_gv
        )
{
    return
        art_gv->arlightsample_gv->_alf_lct_string(
            art_gv
            );
}

ArLightSample * arlightsample_alloc(
        const ART_GV  * art_gv
        )
{
    ArLightSample  * newLight = ALLOC( ArLightSample );

    newLight->next = 0;

    newLight->value =
        art_gv->arlightsample_gv->_alf_alloc(
            art_gv
            );

    return newLight;
}

void arlightsample_free(
        const ART_GV            * art_gv,
              ArLightSample * lr
        )
{
    ASSERT_ALLOCATED_LIGHT_SAMPLE( lr )

    art_gv->arlightsample_gv->_alf_free(
        art_gv,
        lr->value
        );

    FREE( lr );
}

ArLightSample * arlightsample_d_alloc_init_unpolarised(
        const ART_GV  * art_gv,
        const double    d0
        )
{
    ASSERT_NONNEGATIVE_DOUBLE( d0 )

    ArLightSample  * newLight = ALLOC( ArLightSample );

    newLight->value =
        art_gv->arlightsample_gv->_alf_d_alloc_init_unpolarised(
            art_gv,
            d0
            );

    ASSERT_VALID_LIGHT_SAMPLE( newLight )

    return newLight;
}

ArLightSample *  arlightsample_l_alloc_init(
        const ART_GV            * art_gv,
        const ArLightSample * l0
        )
{
    ASSERT_VALID_LIGHT_SAMPLE( l0 )

    ArLightSample  * newLight = ALLOC( ArLightSample );

    ASSERT_ALLOCATED_LIGHT_SAMPLE( newLight )

    newLight->value =
        art_gv->arlightsample_gv->_alf_l_alloc_init(
            art_gv,
            l0
            );

    ASSERT_VALID_LIGHT_SAMPLE( newLight )

    return newLight;
}

ArLightSample const * arlightsample_unpolarised_unit(
        const ART_GV  * art_gv
        )
{
    ASSERT_VALID_LIGHT_SAMPLE( ARLIGHTSAMPLE_UNPOLARISED_UNIT_GV )

    return
        ARLIGHTSAMPLE_UNPOLARISED_UNIT_GV;
}

ArLightSample const * arlightsample_none(
        const ART_GV  * art_gv
        )
{
    ASSERT_VALID_LIGHT_SAMPLE( ARLIGHTSAMPLE_NONE_GV )

    return
        ARLIGHTSAMPLE_NONE_GV;
}

void arlightsample_d_init_unpolarised_l(
        const ART_GV            * art_gv,
        const double              d0,
              ArLightSample * lr
        )
{
    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_ALLOCATED_LIGHT_SAMPLE( lr )

    art_gv->arlightsample_gv->_alf_d_init_unpolarised_l(
        art_gv,
        d0,
        lr->value
        );

    ASSERT_VALID_LIGHT_SAMPLE( lr )
}

void arlightsample_s_init_unpolarised_l(
        const ART_GV                * art_gv,
        const ArSpectralSample  * c0,
              ArLightSample     * lr
        )
{
    ASSERT_VALID_SPECTRAL_SAMPLE( c0 )
    ASSERT_ALLOCATED_LIGHT_SAMPLE( lr )

    art_gv->arlightsample_gv->_alf_s_init_unpolarised_l(
        art_gv,
        c0,
        lr->value
        );

    ASSERT_VALID_LIGHT_SAMPLE( lr )
}

void arlightsample_l_init_l(
        const ART_GV             * art_gv,
        const ArLightSample  * l0,
              ArLightSample  * lr
        )
{
    ASSERT_VALID_LIGHT_SAMPLE( l0 )
    ASSERT_ALLOCATED_LIGHT_SAMPLE( lr )

    art_gv->arlightsample_gv->_alf_l_init_l(
        art_gv,
        l0->value,
        lr->value
        );

    ASSERT_VALID_LIGHT_SAMPLE( lr )
    ASSERT_COAXIAL_SAMPLE_REFERENCE_FRAMES_LL( l0, lr, 3.0 DEGREES)
}

unsigned int arlightsample_l_polarised(
        const ART_GV             * art_gv,
        const ArLightSample  * l0
        )
{
    ASSERT_VALID_LIGHT_SAMPLE( l0 )

    return
        art_gv->arlightsample_gv->_alf_l_polarised(
            art_gv,
            l0->value
            );
}

ArReferenceFrame const * arlightsample_l_refframe(
        const ART_GV             * art_gv,
        const ArLightSample  * l0
        )
{
    ASSERT_VALID_LIGHT_SAMPLE( l0 )

    return
        art_gv->arlightsample_gv->_alf_l_refframe(
            art_gv,
            l0->value
            );
}

void arlightsample_s_rf_init_polarised_l(
        const ART_GV              * art_gv,
        const ArStokesVectorSample  * s0,
        const ArReferenceFrame    * r0,
              ArLightSample   * lr
        )
{
    ASSERT_VALID_STOKES_VECTOR_SAMPLE( s0 )
    ASSERT_VALID_REFERENCE_FRAME( r0 )
    ASSERT_ALLOCATED_LIGHT_SAMPLE( lr )

    art_gv->arlightsample_gv->_alf_s_rf_init_polarised_l(
        art_gv,
        s0,
        r0,
        lr->value
        );

    ASSERT_VALID_LIGHT_SAMPLE( lr )
}

void arlightsample_l_init_h(
        const ART_GV                * art_gv,
        const ArLightSample     * l0,
              ArSpectralSample  * cr
        )
{
    ASSERT_VALID_LIGHT_SAMPLE( l0 )
    ASSERT_ALLOCATED_SPECTRUM( cr )

    art_gv->arlightsample_gv->_alf_l_init_h(
        art_gv,
        l0->value,
        cr
        );

    ASSERT_VALID_SPECTRAL_SAMPLE( cr )
}

void arlightsample_l_init_i(
        const ART_GV                * art_gv,
        const ArLightSample     * l0,
              ArLightIntensitySample  * ir
        )
{
    ASSERT_VALID_LIGHT_SAMPLE( l0 )
    ASSERT_ALLOCATED_LIGHT_INTENSITY_SAMPLE( ir )

    art_gv->arlightsample_gv->_alf_l_init_i(
        art_gv,
        l0->value,
        ir
        );

    ASSERT_VALID_LIGHT_INTENSITY_SAMPLE( ir )
}

void arlightsample_l_to_sv(
        const ART_GV              * art_gv,
        const ArLightSample   * l0,
              ArStokesVectorSample  * sr
        )
{
    ASSERT_VALID_LIGHT_SAMPLE( l0 )
    ASSERT_ALLOCATED_STOKES_VECTOR_SAMPLE( sr )

    art_gv->arlightsample_gv->_alf_l_to_sv(
        art_gv,
        l0->value,
        sr
        );

    ASSERT_VALID_STOKES_VECTOR_SAMPLE( sr )
}

void arlightsample_ld_realign_to_coaxial_refframe_sv(
        const ART_GV              * art_gv,
        const ArLightSample   * l0,
        const double                d0,
        const ArReferenceFrame    * r0,
              ArStokesVectorSample  * sr
        )
{
    ASSERT_VALID_LIGHT_SAMPLE( l0 )
    ASSERT_VALID_REFERENCE_FRAME( r0 )
    ASSERT_ALLOCATED_STOKES_VECTOR_SAMPLE( sr )
    ASSERT_COAXIAL_SAMPLE_REFERENCE_FRAMES_RL( r0, l0, 3.0 DEGREES)

    art_gv->arlightsample_gv->_alf_ld_realign_to_coaxial_refframe_sv(
        art_gv,
        l0->value,
        d0,
        r0,
        sr
        );

    ASSERT_VALID_STOKES_VECTOR_SAMPLE( sr )
}

void arlightsample_realign_to_coaxial_refframe_l(
        const ART_GV             * art_gv,
        const ArReferenceFrame   * r0,
              ArLightSample  * lr
        )
{
    ASSERT_VALID_LIGHT_SAMPLE( lr )
    ASSERT_VALID_REFERENCE_FRAME( r0 )
    ASSERT_COAXIAL_SAMPLE_REFERENCE_FRAMES_RL( r0, lr, 3.0 DEGREES)

    art_gv->arlightsample_gv->_alf_realign_to_coaxial_refframe_l(
        art_gv,
        r0,
        lr->value
        );

    ASSERT_VALID_LIGHT_SAMPLE( lr )
    ASSERT_IDENTICAL_SAMPLE_REFERENCE_FRAMES_RL( r0, lr )
}

void arlightsample_l_realign_to_coaxial_refframe_l(
        const ART_GV             * art_gv,
        const ArLightSample  * l0,
        const ArReferenceFrame   * r0,
              ArLightSample  * lr
        )
{
    ASSERT_VALID_LIGHT_SAMPLE( l0 )
    ASSERT_VALID_REFERENCE_FRAME( r0 )
    ASSERT_ALLOCATED_LIGHT_SAMPLE( lr )
    ASSERT_COAXIAL_SAMPLE_REFERENCE_FRAMES_RL( r0, l0, 3.0 DEGREES)

    art_gv->arlightsample_gv->_alf_l_realign_to_coaxial_refframe_l(
        art_gv,
        l0->value,
        r0,
        lr->value
        );

    ASSERT_VALID_LIGHT_SAMPLE( lr )
    ASSERT_IDENTICAL_SAMPLE_REFERENCE_FRAMES_RL( r0, lr )
}

unsigned int arlightsample_ll_equal(
        const ART_GV             * art_gv,
        const ArLightSample  * l0,
        const ArLightSample  * l1
        )
{
    ASSERT_VALID_LIGHT_SAMPLE( l0 )
    ASSERT_VALID_LIGHT_SAMPLE( l1 )

    return
        art_gv->arlightsample_gv->_alf_ll_equal(
            art_gv,
            l0->value,
            l1->value
            );
}

unsigned int arlightsample_lld_equal(
        const ART_GV             * art_gv,
        const ArLightSample  * l0,
        const ArLightSample  * l1,
        const double               max_relative_error
        )
{
    ASSERT_VALID_LIGHT_SAMPLE( l0 )
    ASSERT_VALID_LIGHT_SAMPLE( l1 )

    return
        art_gv->arlightsample_gv->_alf_lld_equal(
            art_gv,
            l0->value,
            l1->value,
            max_relative_error
            );
}

double arlightsample_l_norm(
        const ART_GV  * art_gv,
        const ArLightSample * l0
        )
{
    ASSERT_VALID_LIGHT_SAMPLE( l0 )

    return
        art_gv->arlightsample_gv->_alf_l_norm(
            art_gv,
            l0->value
            );
}

void arlightsample_l_inv_l(
        const ART_GV             * art_gv,
        const ArLightSample  * l0,
              ArLightSample  * lr
        )
{
    ASSERT_VALID_LIGHT_SAMPLE( l0 )
    ASSERT_ALLOCATED_LIGHT_SAMPLE( lr )

    art_gv->arlightsample_gv->_alf_l_inv_l(
        art_gv,
        l0->value,
        lr->value
        );

    ASSERT_VALID_LIGHT_SAMPLE( lr )
    ASSERT_COAXIAL_SAMPLE_REFERENCE_FRAMES_LL( l0, lr, 3.0 DEGREES)
}

double arlightsample_l_max(
        const ART_GV             * art_gv,
        const ArLightSample  * l0
        )
{
    ASSERT_VALID_LIGHT_SAMPLE( l0 )

    return
        art_gv->arlightsample_gv->_alf_l_max(
            art_gv,
            l0->value
            );
}

void arlightsample_ll_max_l(
        const ART_GV             * art_gv,
        const ArLightSample  * l0,
        const ArLightSample  * l1,
              ArLightSample  * lr
        )
{
    ASSERT_VALID_LIGHT_SAMPLE( l0 )
    ASSERT_VALID_LIGHT_SAMPLE( l1 )
    ASSERT_ALLOCATED_LIGHT_SAMPLE( lr )
    ASSERT_COAXIAL_SAMPLE_REFERENCE_FRAMES_LL( l0, l1, 3.0 DEGREES)

    art_gv->arlightsample_gv->_alf_ll_max_l(
        art_gv,
        l0->value,
        l1->value,
        lr->value
        );

    ASSERT_VALID_LIGHT_SAMPLE( lr )
    ASSERT_COAXIAL_SAMPLE_REFERENCE_FRAMES_LL( l0, lr, 3.0 DEGREES)
}

double arlightsample_l_min(
        const ART_GV             * art_gv,
        const ArLightSample  * l0
        )
{
    ASSERT_VALID_LIGHT_SAMPLE( l0 )

    return
        art_gv->arlightsample_gv->_alf_l_min(
            art_gv,
            l0->value
            );
}

void arlightsample_ll_min_l(
        const ART_GV             * art_gv,
        const ArLightSample  * l0,
        const ArLightSample  * l1,
              ArLightSample  * lr
        )
{
    ASSERT_VALID_LIGHT_SAMPLE( l0 )
    ASSERT_VALID_LIGHT_SAMPLE( l1 )
    ASSERT_ALLOCATED_LIGHT_SAMPLE( lr )
    ASSERT_COAXIAL_SAMPLE_REFERENCE_FRAMES_LL( l0, l1, 3.0 DEGREES)

    art_gv->arlightsample_gv->_alf_ll_min_l(
        art_gv,
        l0->value,
        l1->value,
        lr->value
        );

    ASSERT_VALID_LIGHT_SAMPLE( lr )
    ASSERT_COAXIAL_SAMPLE_REFERENCE_FRAMES_LL( l0, lr, 3.0 DEGREES)
}

double arlightsample_ll_maxdiff(
        const ART_GV             * art_gv,
        const ArLightSample  * l0,
        const ArLightSample  * l1
        )
{
    ASSERT_VALID_LIGHT_SAMPLE( l0 )
    ASSERT_VALID_LIGHT_SAMPLE( l1 )
    ASSERT_COAXIAL_SAMPLE_REFERENCE_FRAMES_LL( l0, l1, 3.0 DEGREES)

    return
        art_gv->arlightsample_gv->_alf_ll_maxdiff(
            art_gv,
            l0->value,
            l1->value
            );
}

void arlightsample_i_add_l(
        const ART_GV                * art_gv,
        const ArLightIntensitySample  * i0,
              ArLightSample     * lr
        )
{
    ASSERT_VALID_LIGHT_INTENSITY_SAMPLE( i0 )
    ASSERT_VALID_LIGHT_SAMPLE( lr )

    art_gv->arlightsample_gv->_alf_i_add_l(
        art_gv,
        i0,
        lr->value
        );

    ASSERT_VALID_LIGHT_SAMPLE( lr )
}

void arlightsample_l_add_l(
        const ART_GV             * art_gv,
        const ArLightSample  * l0,
              ArLightSample  * lr
        )
{
    ASSERT_VALID_LIGHT_SAMPLE( l0 )
    ASSERT_VALID_LIGHT_SAMPLE( lr )
    ASSERT_COAXIAL_SAMPLE_REFERENCE_FRAMES_LL( l0, lr, 3.0 DEGREES)

    art_gv->arlightsample_gv->_alf_l_add_l(
        art_gv,
        l0->value,
        lr->value
        );

    ASSERT_VALID_LIGHT_SAMPLE( lr )
    ASSERT_COAXIAL_SAMPLE_REFERENCE_FRAMES_LL( l0, lr, 3.0 DEGREES)
}

void arlightsample_l_atomic_add_l(
        const ART_GV             * art_gv,
        const ArLightSample  * l0,
              ArLightSample  * lr
        )
{
    ASSERT_VALID_LIGHT_SAMPLE( l0 )
    ASSERT_VALID_LIGHT_SAMPLE( lr )
    ASSERT_COAXIAL_SAMPLE_REFERENCE_FRAMES_LL( l0, lr, 3.0 DEGREES)

    art_gv->arlightsample_gv->_alf_l_atomic_add_l(
        art_gv,
        l0->value,
        lr->value
        );

    ASSERT_VALID_LIGHT_SAMPLE( lr )
    ASSERT_COAXIAL_SAMPLE_REFERENCE_FRAMES_LL( l0, lr, 3.0 DEGREES)
}

void arlightsample_ll_add_l(
        const ART_GV             * art_gv,
        const ArLightSample  * l0,
        const ArLightSample  * l1,
              ArLightSample  * lr
        )
{
    ASSERT_VALID_LIGHT_SAMPLE( l0 )
    ASSERT_VALID_LIGHT_SAMPLE( l1 )
    ASSERT_ALLOCATED_LIGHT_SAMPLE( lr )
    ASSERT_COAXIAL_SAMPLE_REFERENCE_FRAMES_LL( l0, l1, 3.0 DEGREES)

    art_gv->arlightsample_gv->_alf_ll_add_l(
        art_gv,
        l0->value,
        l1->value,
        lr->value
        );

    ASSERT_VALID_LIGHT_SAMPLE( lr )
    ASSERT_COAXIAL_SAMPLE_REFERENCE_FRAMES_LL( l0, lr, 3.0 DEGREES)
}

void arlightsample_lld_sloppy_add_l(
        const ART_GV             * art_gv,
        const ArLightSample  * l0,
        const ArLightSample  * l1,
        const double               d0,
              ArLightSample  * lr
        )
{
    ASSERT_VALID_LIGHT_SAMPLE( l0 )
    ASSERT_VALID_LIGHT_SAMPLE( l1 )
    ASSERT_NONNEGATIVE_DOUBLE(d0)
    ASSERT_ALLOCATED_LIGHT_SAMPLE( lr )
    ASSERT_COAXIAL_SAMPLE_REFERENCE_FRAMES_LL( l0, l1, 3.0 DEGREES)

    art_gv->arlightsample_gv->_alf_lld_sloppy_add_l(
        art_gv,
        l0->value,
        l1->value,
        d0,
        lr->value
        );

    ASSERT_VALID_LIGHT_SAMPLE( lr )
    ASSERT_COAXIAL_SAMPLE_REFERENCE_FRAMES_LL( l0, lr, 3.0 DEGREES)
}

void arlightsample_ll_sub_l(
        const ART_GV             * art_gv,
        const ArLightSample  * l0,
        const ArLightSample  * l1,
              ArLightSample  * lr
        )
{
    ASSERT_VALID_LIGHT_SAMPLE( l0 )
    ASSERT_VALID_LIGHT_SAMPLE( l1 )
    ASSERT_ALLOCATED_LIGHT_SAMPLE( lr )
    ASSERT_COAXIAL_SAMPLE_REFERENCE_FRAMES_LL( l0, l1, 3.0 DEGREES)

    art_gv->arlightsample_gv->_alf_ll_sub_l(
        art_gv,
        l0->value,
        l1->value,
        lr->value
        );

    ASSERT_VALID_LIGHT_SAMPLE( lr )
    ASSERT_COAXIAL_SAMPLE_REFERENCE_FRAMES_LL( l0, lr, 3.0 DEGREES)
}

void arlightsample_d_mul_l(
        const ART_GV             * art_gv,
        const double               d0,
              ArLightSample  * lr
        )
{
    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_VALID_LIGHT_SAMPLE( lr )

    art_gv->arlightsample_gv->_alf_d_mul_l(
        art_gv,
        d0,
        lr->value
        );

    ASSERT_VALID_LIGHT_SAMPLE( lr )
}

void arlightsample_dl_mul_l(
        const ART_GV             * art_gv,
        const double               d0,
        const ArLightSample  * l0,
              ArLightSample  * lr
        )
{
    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_VALID_LIGHT_SAMPLE( l0 )
    ASSERT_ALLOCATED_LIGHT_SAMPLE( lr )

    art_gv->arlightsample_gv->_alf_dl_mul_l(
        art_gv,
        d0,
        l0->value,
        lr->value
        );

    ASSERT_VALID_LIGHT_SAMPLE( lr )
    ASSERT_COAXIAL_SAMPLE_REFERENCE_FRAMES_LL( l0, lr, 3.0 DEGREES)
}

void arlightsample_i_mul_l(
        const ART_GV                * art_gv,
        const ArLightIntensitySample  * i0,
              ArLightSample     * lr
        )
{
    ASSERT_VALID_LIGHT_INTENSITY_SAMPLE( i0 )
    ASSERT_VALID_LIGHT_SAMPLE( lr )

    art_gv->arlightsample_gv->_alf_i_mul_l(
        art_gv,
        i0,
        lr->value
        );

    ASSERT_VALID_LIGHT_SAMPLE( lr )
}

void arlightsample_l_mul_l(
        const ART_GV             * art_gv,
        const ArLightSample  * l0,
              ArLightSample  * lr
        )
{
    ASSERT_VALID_LIGHT_SAMPLE( l0 )
    ASSERT_VALID_LIGHT_SAMPLE( lr )
    ASSERT_COAXIAL_SAMPLE_REFERENCE_FRAMES_LL( l0, lr, 3.0 DEGREES)

    art_gv->arlightsample_gv->_alf_l_mul_l(
        art_gv,
        l0->value,
        lr->value
        );

    ASSERT_VALID_LIGHT_SAMPLE( lr )
    ASSERT_COAXIAL_SAMPLE_REFERENCE_FRAMES_LL( l0, lr, 3.0 DEGREES)
}

void arlightsample_ll_mul_l(
        const ART_GV             * art_gv,
        const ArLightSample  * l0,
        const ArLightSample  * l1,
              ArLightSample  * lr
        )
{
    ASSERT_VALID_LIGHT_SAMPLE( l0 )
    ASSERT_VALID_LIGHT_SAMPLE( l1 )
    ASSERT_ALLOCATED_LIGHT_SAMPLE( lr )
    ASSERT_COAXIAL_SAMPLE_REFERENCE_FRAMES_LL( l0, l1, 3.0 DEGREES)

    art_gv->arlightsample_gv->_alf_ll_mul_l(
        art_gv,
        l0->value,
        l1->value,
        lr->value
        );

    ASSERT_VALID_LIGHT_SAMPLE( lr )
    ASSERT_COAXIAL_SAMPLE_REFERENCE_FRAMES_LL( l0, lr, 3.0 DEGREES)
}

void arlightsample_d_div_l(
        const ART_GV             * art_gv,
        const double               d0,
              ArLightSample  * lr
        )
{
    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_VALID_LIGHT_SAMPLE( lr )

    art_gv->arlightsample_gv->_alf_d_div_l(
        art_gv,
        d0,
        lr->value
        );

    ASSERT_VALID_LIGHT_SAMPLE( lr )
}

void arlightsample_dl_div_l(
        const ART_GV             * art_gv,
        const double               d0,
        const ArLightSample  * l0,
              ArLightSample  * lr
        )
{
    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_VALID_LIGHT_SAMPLE( l0 )
    ASSERT_ALLOCATED_LIGHT_SAMPLE( lr )

    art_gv->arlightsample_gv->_alf_dl_div_l(
        art_gv,
        d0,
        l0->value,
        lr->value
        );

    ASSERT_VALID_LIGHT_SAMPLE( lr )
    ASSERT_COAXIAL_SAMPLE_REFERENCE_FRAMES_LL( l0, lr, 3.0 DEGREES)
}

void arlightsample_dl_pow_l(
        const ART_GV             * art_gv,
        const double               d0,
        const ArLightSample  * l0,
              ArLightSample  * lr
        )
{
    ASSERT_VALID_LIGHT_SAMPLE( l0 )
    ASSERT_ALLOCATED_LIGHT_SAMPLE( lr )

    art_gv->arlightsample_gv->_alf_dl_pow_l(
        art_gv,
        d0,
        l0->value,
        lr->value
        );

    ASSERT_VALID_LIGHT_SAMPLE( lr )
    ASSERT_COAXIAL_SAMPLE_REFERENCE_FRAMES_LL( l0, lr, 3.0 DEGREES)
}

void arlightsample_ddl_clamp_l(
        const ART_GV             * art_gv,
        const double               d0,
        const double               d1,
        const ArLightSample  * l0,
              ArLightSample  * lr
        )
{
    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_NONNEGATIVE_DOUBLE( d1 )
    ASSERT_VALID_LIGHT_SAMPLE( l0 )
    ASSERT_ALLOCATED_LIGHT_SAMPLE( lr )

    art_gv->arlightsample_gv->_alf_ddl_clamp_l(
        art_gv,
        d0,
        d1,
        l0->value,
        lr->value
        );

    ASSERT_VALID_LIGHT_SAMPLE( lr )
    ASSERT_COAXIAL_SAMPLE_REFERENCE_FRAMES_LL( l0, lr, 3.0 DEGREES)
}

void arlightsample_dl_mul_add_l(
        const ART_GV             * art_gv,
        const double               d0,
        const ArLightSample  * l0,
              ArLightSample  * lr
        )
{
    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_VALID_LIGHT_SAMPLE( l0 )
    ASSERT_VALID_LIGHT_SAMPLE( lr )
    ASSERT_COAXIAL_SAMPLE_REFERENCE_FRAMES_LL( l0, lr, 3.0 DEGREES)

    art_gv->arlightsample_gv->_alf_dl_mul_add_l(
        art_gv,
        d0,
        l0->value,
        lr->value
        );

    ASSERT_VALID_LIGHT_SAMPLE( lr )
    ASSERT_COAXIAL_SAMPLE_REFERENCE_FRAMES_LL( l0, lr, 3.0 DEGREES)
}

void arlightsample_dll_interpol_l(
        const ART_GV             * art_gv,
        const double               d0,
        const ArLightSample  * l0,
        const ArLightSample  * l1,
              ArLightSample  * lr
        )
{
    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_VALID_LIGHT_SAMPLE( l0 )
    ASSERT_VALID_LIGHT_SAMPLE( l1 )
    ASSERT_ALLOCATED_LIGHT_SAMPLE( lr )
    ASSERT_COAXIAL_SAMPLE_REFERENCE_FRAMES_LL( l0, l1, 3.0 DEGREES)

    art_gv->arlightsample_gv->_alf_dll_interpol_l(
        art_gv,
        d0,
        l0->value,
        l1->value,
        lr->value
        );

    ASSERT_VALID_LIGHT_SAMPLE( lr )
    ASSERT_COAXIAL_SAMPLE_REFERENCE_FRAMES_LL( l0, lr, 3.0 DEGREES)
}

unsigned int arlightsample_l_valid(
        const ART_GV             * art_gv,
        const ArLightSample  * l0
        )
{
    if ( ! l0 )
    {
        ART_ERRORHANDLING_WARNING(
            "unallocated ArLightSample argument 'l0'"
            );
        return 0;
    }

    return
        art_gv->arlightsample_gv->_alf_l_valid(
            art_gv,
            l0->value
            );
}

unsigned int arlightsample_ll_collinear(
        const ART_GV             * art_gv,
        const ArLightSample  * l0,
        const ArLightSample  * l1
        )
{
    return
        art_gv->arlightsample_gv->_alf_ll_collinear(
            art_gv,
            l0->value,
            l1->value
            );
}

void arlightsample_l_debugprintf(
        const ART_GV             * art_gv,
        const ArLightSample  * l0
        )
{
    debugprintf(
        "ArLightSample [%p]: ",
        l0
        );

    art_gv->arlightsample_gv->_alf_l_debugprintf(
        art_gv,
        l0->value
        );
}

// ===========================================================================
