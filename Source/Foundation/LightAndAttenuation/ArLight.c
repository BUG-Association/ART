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

#define ART_MODULE_NAME     ArLight

#include "ArLight.h"

#include "_ArLight_GV.h"
#include "FoundationAssertionMacros.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    ARLIGHT_GV = ALLOC(ArLight_GV);

    ARLIGHT_LCT_HAS_BEEN_INITIALISED_GV = 0;

    ARLIGHT_UNPOLARISED_UNIT_GV = 0;
    ARLIGHT_NONE_GV = 0;
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    if ( ARLIGHT_UNPOLARISED_UNIT_GV )
        arlight_free( art_gv, ARLIGHT_UNPOLARISED_UNIT_GV );
    if ( ARLIGHT_NONE_GV )
        arlight_free( art_gv, ARLIGHT_NONE_GV );

    FREE( ARLIGHT_GV );
)

ART_LIGHT_AND_ATTENUATION_MODULE_INITIALISATION_FUNCTION
(
    if ( ARLIGHT_LCT_HAS_BEEN_INITIALISED_GV )
    {
        ARLIGHT_UNPOLARISED_UNIT_GV =
            arlight_d_alloc_init_unpolarised(
                art_gv,
                1.0
                );

        ARLIGHT_NONE_GV =
            arlight_d_alloc_init_unpolarised(
                art_gv,
                0.0
                );
    }
)

ART_LIGHT_AND_ATTENUATION_MODULE_SHUTDOWN_FUNCTION
(
    if ( ARLIGHT_UNPOLARISED_UNIT_GV )
        arlight_free( art_gv, ARLIGHT_UNPOLARISED_UNIT_GV );
    if ( ARLIGHT_NONE_GV )
        arlight_free( art_gv, ARLIGHT_NONE_GV );
)

const char * arlight_lct_string(
        const ART_GV  * art_gv
        )
{
    return
        art_gv->arlight_gv->_alf_lct_string(
            art_gv
            );
}

ArLight * arlight_alloc(
        const ART_GV  * art_gv
        )
{
    ArLight  * newLight = ALLOC( ArLight );

    newLight->next = 0;

    newLight->value =
        art_gv->arlight_gv->_alf_alloc(
            art_gv
            );

    return newLight;
}

void arlight_free(
        const ART_GV  * art_gv,
              ArLight * lr
        )
{
    ASSERT_ALLOCATED_LIGHT( lr )

    art_gv->arlight_gv->_alf_free(
        art_gv,
        lr->value
        );

    FREE( lr );
}

ArLight * arlight_d_alloc_init_unpolarised(
        const ART_GV  * art_gv,
        const double    d0
        )
{
    ASSERT_NONNEGATIVE_DOUBLE( d0 )

    ArLight  * newLight = ALLOC( ArLight );

    newLight->value =
        art_gv->arlight_gv->_alf_d_alloc_init_unpolarised(
            art_gv,
            d0
            );

    ASSERT_VALID_LIGHT( newLight )

    return newLight;
}

ArLight *  arlight_l_alloc_init(
        const ART_GV  * art_gv,
        const ArLight * l0
        )
{
    ASSERT_VALID_LIGHT( l0 )

    ArLight  * newLight = ALLOC( ArLight );

    ASSERT_ALLOCATED_LIGHT( newLight )

    newLight->value =
        art_gv->arlight_gv->_alf_l_alloc_init(
            art_gv,
            l0
            );

    ASSERT_VALID_LIGHT( newLight )

    return newLight;
}

ArLight const * arlight_unpolarised_unit(
        const ART_GV  * art_gv
        )
{
    ASSERT_VALID_LIGHT( ARLIGHT_UNPOLARISED_UNIT_GV )

    return
        ARLIGHT_UNPOLARISED_UNIT_GV;
}

ArLight const * arlight_none(
        const ART_GV  * art_gv
        )
{
    ASSERT_VALID_LIGHT( ARLIGHT_NONE_GV )

    return
        ARLIGHT_NONE_GV;
}

void arlight_d_init_unpolarised_l(
        const ART_GV  * art_gv,
        const double    d0,
              ArLight * lr
        )
{
    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_ALLOCATED_LIGHT( lr )

    art_gv->arlight_gv->_alf_d_init_unpolarised_l(
        art_gv,
        d0,
        lr->value
        );

    ASSERT_VALID_LIGHT( lr )
}

void arlight_s_init_unpolarised_l(
        const ART_GV    * art_gv,
        const ArSpectrum  * c0,
              ArLight   * lr
        )
{
    ASSERT_VALID_SPECTRUM( c0 )
    ASSERT_ALLOCATED_LIGHT( lr )

    art_gv->arlight_gv->_alf_s_init_unpolarised_l(
        art_gv,
        c0,
        lr->value
        );

    ASSERT_VALID_LIGHT( lr )
}

void arlight_l_init_l(
        const ART_GV   * art_gv,
        const ArLight  * l0,
              ArLight  * lr
        )
{
    ASSERT_VALID_LIGHT( l0 )
    ASSERT_ALLOCATED_LIGHT( lr )

    art_gv->arlight_gv->_alf_l_init_l(
        art_gv,
        l0->value,
        lr->value
        );

    ASSERT_VALID_LIGHT( lr )
    ASSERT_COAXIAL_REFERENCE_FRAMES_LL( l0, lr, 3.0 DEGREES)
}

unsigned int arlight_l_polarised(
        const ART_GV   * art_gv,
        const ArLight  * l0
        )
{
    ASSERT_VALID_LIGHT( l0 )

    return
        art_gv->arlight_gv->_alf_l_polarised(
            art_gv,
            l0->value
            );
}

ArReferenceFrame const * arlight_l_refframe(
        const ART_GV   * art_gv,
        const ArLight  * l0
        )
{
    ASSERT_VALID_LIGHT( l0 )

    return
        art_gv->arlight_gv->_alf_l_refframe(
            art_gv,
            l0->value
            );
}

void arlight_s_rf_init_polarised_l(
        const ART_GV            * art_gv,
        const ArStokesVector    * s0,
        const ArReferenceFrame  * r0,
              ArLight           * lr
        )
{
    ASSERT_VALID_STOKES_VECTOR( s0 )
    ASSERT_VALID_REFERENCE_FRAME( r0 )
    ASSERT_ALLOCATED_LIGHT( lr )

    art_gv->arlight_gv->_alf_s_rf_init_polarised_l(
        art_gv,
        s0,
        r0,
        lr->value
        );

    ASSERT_VALID_LIGHT( lr )
}

void arlight_l_init_s(
        const ART_GV    * art_gv,
        const ArLight   * l0,
              ArSpectrum  * cr
        )
{
    ASSERT_VALID_LIGHT( l0 )
    ASSERT_ALLOCATED_SPECTRUM( cr )

    art_gv->arlight_gv->_alf_l_init_s(
        art_gv,
        l0->value,
        cr
        );

    ASSERT_VALID_SPECTRUM( cr )
}

void arlight_l_init_i(
        const ART_GV            * art_gv,
        const ArLight           * l0,
              ArLightIntensity  * ir
        )
{
    ASSERT_VALID_LIGHT( l0 )
    ASSERT_ALLOCATED_LIGHT_INTENSITY( ir )

    art_gv->arlight_gv->_alf_l_init_i(
        art_gv,
        l0->value,
        ir
        );

    ASSERT_VALID_LIGHT_INTENSITY( ir )
}

void arlight_l_to_sv(
        const ART_GV          * art_gv,
        const ArLight         * l0,
              ArStokesVector  * sr
        )
{
    ASSERT_VALID_LIGHT( l0 )
    ASSERT_ALLOCATED_STOKES_VECTOR( sr )

    art_gv->arlight_gv->_alf_l_to_sv(
        art_gv,
        l0->value,
        sr
        );

    ASSERT_VALID_STOKES_VECTOR( sr )
}

void arlight_ld_realign_to_coaxial_refframe_sv(
        const ART_GV            * art_gv,
        const ArLight           * l0,
        const double              d0,
        const ArReferenceFrame  * r0,
              ArStokesVector    * sr
        )
{
    ASSERT_VALID_LIGHT( l0 )
    ASSERT_VALID_REFERENCE_FRAME( r0 )
    ASSERT_ALLOCATED_STOKES_VECTOR( sr )
    ASSERT_COAXIAL_REFERENCE_FRAMES_RL( r0, l0, 3.0 DEGREES)

    art_gv->arlight_gv->_alf_ld_realign_to_coaxial_refframe_sv(
        art_gv,
        l0->value,
        d0,
        r0,
        sr
        );

    ASSERT_VALID_STOKES_VECTOR( sr )
}

void arlight_realign_to_coaxial_refframe_l(
        const ART_GV            * art_gv,
        const ArReferenceFrame  * r0,
              ArLight           * lr
        )
{
    ASSERT_VALID_LIGHT( lr )
    ASSERT_VALID_REFERENCE_FRAME( r0 )
    ASSERT_COAXIAL_REFERENCE_FRAMES_RL( r0, lr, 3.0 DEGREES)

    art_gv->arlight_gv->_alf_realign_to_coaxial_refframe_l(
        art_gv,
        r0,
        lr->value
        );

    ASSERT_VALID_LIGHT( lr )
    ASSERT_IDENTICAL_REFERENCE_FRAMES_RL( r0, lr )
}

void arlight_l_realign_to_coaxial_refframe_l(
        const ART_GV            * art_gv,
        const ArLight           * l0,
        const ArReferenceFrame  * r0,
              ArLight           * lr
        )
{
    ASSERT_VALID_LIGHT( l0 )
    ASSERT_VALID_REFERENCE_FRAME( r0 )
    ASSERT_ALLOCATED_LIGHT( lr )
    ASSERT_COAXIAL_REFERENCE_FRAMES_RL( r0, l0, 3.0 DEGREES)

    art_gv->arlight_gv->_alf_l_realign_to_coaxial_refframe_l(
        art_gv,
        l0->value,
        r0,
        lr->value
        );

    ASSERT_VALID_LIGHT( lr )
    ASSERT_IDENTICAL_REFERENCE_FRAMES_RL( r0, lr )
}

unsigned int arlight_ll_equal(
        const ART_GV   * art_gv,
        const ArLight  * l0,
        const ArLight  * l1
        )
{
    ASSERT_VALID_LIGHT( l0 )
    ASSERT_VALID_LIGHT( l1 )

    return
        art_gv->arlight_gv->_alf_ll_equal(
            art_gv,
            l0->value,
            l1->value
            );
}

unsigned int arlight_lld_equal(
        const ART_GV   * art_gv,
        const ArLight  * l0,
        const ArLight  * l1,
        const double     max_relative_error
        )
{
    ASSERT_VALID_LIGHT( l0 )
    ASSERT_VALID_LIGHT( l1 )

    return
        art_gv->arlight_gv->_alf_lld_equal(
            art_gv,
            l0->value,
            l1->value,
            max_relative_error
            );
}

double arlight_l_norm(
        const ART_GV  * art_gv,
        const ArLight * l0
        )
{
    ASSERT_VALID_LIGHT( l0 )

    return
        art_gv->arlight_gv->_alf_l_norm(
            art_gv,
            l0->value
            );
}

double arlight_l_norm_visible_range(
        const ART_GV  * art_gv,
        const ArLight * l0
        )
{
    ASSERT_VALID_LIGHT( l0 )

    return
        art_gv->arlight_gv->_alf_l_norm_visible_range(
            art_gv,
            l0->value
            );
}

void arlight_l_inv_l(
        const ART_GV  * art_gv,
        const ArLight * l0,
              ArLight * lr
        )
{
    ASSERT_VALID_LIGHT( l0 )
    ASSERT_ALLOCATED_LIGHT( lr )

    art_gv->arlight_gv->_alf_l_inv_l(
        art_gv,
        l0->value,
        lr->value
        );

    ASSERT_VALID_LIGHT( lr )
    ASSERT_COAXIAL_REFERENCE_FRAMES_LL( l0, lr, 3.0 DEGREES)
}

double arlight_l_max(
        const ART_GV  * art_gv,
        const ArLight * l0
        )
{
    ASSERT_VALID_LIGHT( l0 )

    return
        art_gv->arlight_gv->_alf_l_max(
            art_gv,
            l0->value
            );
}

void arlight_ll_max_l(
        const ART_GV  * art_gv,
        const ArLight * l0,
        const ArLight * l1,
              ArLight * lr
        )
{
    ASSERT_VALID_LIGHT( l0 )
    ASSERT_VALID_LIGHT( l1 )
    ASSERT_ALLOCATED_LIGHT( lr )
    ASSERT_COAXIAL_REFERENCE_FRAMES_LL( l0, l1, 3.0 DEGREES)

    art_gv->arlight_gv->_alf_ll_max_l(
        art_gv,
        l0->value,
        l1->value,
        lr->value
        );

    ASSERT_VALID_LIGHT( lr )
    ASSERT_COAXIAL_REFERENCE_FRAMES_LL( l0, lr, 3.0 DEGREES)
}

double arlight_l_min(
        const ART_GV  * art_gv,
        const ArLight * l0
        )
{
    ASSERT_VALID_LIGHT( l0 )

    return
        art_gv->arlight_gv->_alf_l_min(
            art_gv,
            l0->value
            );
}

void arlight_ll_min_l(
        const ART_GV  * art_gv,
        const ArLight * l0,
        const ArLight * l1,
              ArLight * lr
        )
{
    ASSERT_VALID_LIGHT( l0 )
    ASSERT_VALID_LIGHT( l1 )
    ASSERT_ALLOCATED_LIGHT( lr )
    ASSERT_COAXIAL_REFERENCE_FRAMES_LL( l0, l1, 3.0 DEGREES)

    art_gv->arlight_gv->_alf_ll_min_l(
        art_gv,
        l0->value,
        l1->value,
        lr->value
        );

    ASSERT_VALID_LIGHT( lr )
    ASSERT_COAXIAL_REFERENCE_FRAMES_LL( l0, lr, 3.0 DEGREES)
}

double arlight_ll_maxdiff(
        const ART_GV  * art_gv,
        const ArLight * l0,
        const ArLight * l1
        )
{
    ASSERT_VALID_LIGHT( l0 )
    ASSERT_VALID_LIGHT( l1 )
    ASSERT_COAXIAL_REFERENCE_FRAMES_LL( l0, l1, 3.0 DEGREES)

    return
        art_gv->arlight_gv->_alf_ll_maxdiff(
            art_gv,
            l0->value,
            l1->value
            );
}

void arlight_i_add_l(
        const ART_GV            * art_gv,
        const ArLightIntensity  * i0,
              ArLight           * lr
        )
{
    ASSERT_VALID_LIGHT_INTENSITY( i0 )
    ASSERT_VALID_LIGHT( lr )

    art_gv->arlight_gv->_alf_i_add_l(
        art_gv,
        i0,
        lr->value
        );

    ASSERT_VALID_LIGHT( lr )
}

void arlight_l_add_l(
        const ART_GV  * art_gv,
        const ArLight * l0,
              ArLight * lr
        )
{
    ASSERT_VALID_LIGHT( l0 )
    ASSERT_VALID_LIGHT( lr )
    ASSERT_COAXIAL_REFERENCE_FRAMES_LL( l0, lr, 3.0 DEGREES)

    art_gv->arlight_gv->_alf_l_add_l(
        art_gv,
        l0->value,
        lr->value
        );

    ASSERT_VALID_LIGHT( lr )
    ASSERT_COAXIAL_REFERENCE_FRAMES_LL( l0, lr, 3.0 DEGREES)
}

void arlight_l_atomic_add_l(
        const ART_GV  * art_gv,
        const ArLight * l0,
              ArLight * lr
        )
{
    ASSERT_VALID_LIGHT( l0 )
    ASSERT_VALID_LIGHT( lr )
    ASSERT_COAXIAL_REFERENCE_FRAMES_LL( l0, lr, 3.0 DEGREES)

    art_gv->arlight_gv->_alf_l_atomic_add_l(
        art_gv,
        l0->value,
        lr->value
        );

    ASSERT_VALID_LIGHT( lr )
    ASSERT_COAXIAL_REFERENCE_FRAMES_LL( l0, lr, 3.0 DEGREES)
}

void arlight_ll_add_l(
        const ART_GV  * art_gv,
        const ArLight * l0,
        const ArLight * l1,
              ArLight * lr
        )
{
    ASSERT_VALID_LIGHT( l0 )
    ASSERT_VALID_LIGHT( l1 )
    ASSERT_ALLOCATED_LIGHT( lr )
    ASSERT_COAXIAL_REFERENCE_FRAMES_LL( l0, l1, 3.0 DEGREES)

    art_gv->arlight_gv->_alf_ll_add_l(
        art_gv,
        l0->value,
        l1->value,
        lr->value
        );

    ASSERT_VALID_LIGHT( lr )
    ASSERT_COAXIAL_REFERENCE_FRAMES_LL( l0, lr, 3.0 DEGREES)
}

void arlight_dl_sloppy_add_l(
        const ART_GV  * art_gv,
        const double    d0,
        const ArLight * l0,
              ArLight * lr
        )
{
    ASSERT_VALID_LIGHT( l0 )
    ASSERT_NONNEGATIVE_DOUBLE(d0)
    ASSERT_ALLOCATED_LIGHT( lr )
    ASSERT_COAXIAL_REFERENCE_FRAMES_LL( l0, lr, d0 DEGREES)

    art_gv->arlight_gv->_alf_dl_sloppy_add_l(
        art_gv,
        d0,
        l0->value,
        lr->value
        );

    ASSERT_VALID_LIGHT( lr )
    ASSERT_COAXIAL_REFERENCE_FRAMES_LL( l0, lr, d0 DEGREES)
}

void arlight_lld_sloppy_add_l(
        const ART_GV  * art_gv,
        const ArLight * l0,
        const ArLight * l1,
        const double    d0,
              ArLight * lr
        )
{
    ASSERT_VALID_LIGHT( l0 )
    ASSERT_VALID_LIGHT( l1 )
    ASSERT_NONNEGATIVE_DOUBLE(d0)
    ASSERT_ALLOCATED_LIGHT( lr )
    ASSERT_COAXIAL_REFERENCE_FRAMES_LL( l0, l1, d0 DEGREES)

    art_gv->arlight_gv->_alf_lld_sloppy_add_l(
        art_gv,
        l0->value,
        l1->value,
        d0,
        lr->value
        );

    ASSERT_VALID_LIGHT( lr )
    ASSERT_COAXIAL_REFERENCE_FRAMES_LL( l0, lr, d0 DEGREES)
}

void arlight_ll_sub_l(
        const ART_GV  * art_gv,
        const ArLight * l0,
        const ArLight * l1,
              ArLight * lr
        )
{
    ASSERT_VALID_LIGHT( l0 )
    ASSERT_VALID_LIGHT( l1 )
    ASSERT_ALLOCATED_LIGHT( lr )
    ASSERT_COAXIAL_REFERENCE_FRAMES_LL( l0, l1, 3.0 DEGREES)

    art_gv->arlight_gv->_alf_ll_sub_l(
        art_gv,
        l0->value,
        l1->value,
        lr->value
        );

    ASSERT_VALID_LIGHT( lr )
    ASSERT_COAXIAL_REFERENCE_FRAMES_LL( l0, lr, 3.0 DEGREES)
}

void arlight_d_mul_l(
        const ART_GV  * art_gv,
        const double    d0,
              ArLight * lr
        )
{
    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_VALID_LIGHT( lr )

    art_gv->arlight_gv->_alf_d_mul_l(
        art_gv,
        d0,
        lr->value
        );

    ASSERT_VALID_LIGHT( lr )
}

void arlight_dl_mul_l(
        const ART_GV  * art_gv,
        const double    d0,
        const ArLight * l0,
              ArLight * lr
        )
{
    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_VALID_LIGHT( l0 )
    ASSERT_ALLOCATED_LIGHT( lr )

    art_gv->arlight_gv->_alf_dl_mul_l(
        art_gv,
        d0,
        l0->value,
        lr->value
        );

    ASSERT_VALID_LIGHT( lr )
    ASSERT_COAXIAL_REFERENCE_FRAMES_LL( l0, lr, 3.0 DEGREES)
}

void arlight_i_mul_l(
        const ART_GV            * art_gv,
        const ArLightIntensity  * i0,
              ArLight           * lr
        )
{
    ASSERT_VALID_LIGHT_INTENSITY( i0 )
    ASSERT_VALID_LIGHT( lr )

    art_gv->arlight_gv->_alf_i_mul_l(
        art_gv,
        i0,
        lr->value
        );

    ASSERT_VALID_LIGHT( lr )
}

void arlight_l_mul_l(
        const ART_GV  * art_gv,
        const ArLight* l0,
              ArLight* lr
        )
{
    ASSERT_VALID_LIGHT( l0 )
    ASSERT_VALID_LIGHT( lr )
    ASSERT_COAXIAL_REFERENCE_FRAMES_LL( l0, lr, 3.0 DEGREES)

    art_gv->arlight_gv->_alf_l_mul_l(
        art_gv,
        l0->value,
        lr->value
        );

    ASSERT_VALID_LIGHT( lr )
    ASSERT_COAXIAL_REFERENCE_FRAMES_LL( l0, lr, 3.0 DEGREES)
}

void arlight_ll_mul_l(
        const ART_GV  * art_gv,
        const ArLight * l0,
        const ArLight * l1,
              ArLight * lr
        )
{
    ASSERT_VALID_LIGHT( l0 )
    ASSERT_VALID_LIGHT( l1 )
    ASSERT_ALLOCATED_LIGHT( lr )
    ASSERT_COAXIAL_REFERENCE_FRAMES_LL( l0, l1, 3.0 DEGREES)

    art_gv->arlight_gv->_alf_ll_mul_l(
        art_gv,
        l0->value,
        l1->value,
        lr->value
        );

    ASSERT_VALID_LIGHT( lr )
    ASSERT_COAXIAL_REFERENCE_FRAMES_LL( l0, lr, 3.0 DEGREES)
}

void arlight_d_div_l(
        const ART_GV  * art_gv,
        const double    d0,
              ArLight * lr
        )
{
    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_VALID_LIGHT( lr )

    art_gv->arlight_gv->_alf_d_div_l(
        art_gv,
        d0,
        lr->value
        );

    ASSERT_VALID_LIGHT( lr )
}

void arlight_dl_div_l(
        const ART_GV  * art_gv,
        const double    d0,
        const ArLight * l0,
              ArLight * lr
        )
{
    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_VALID_LIGHT( l0 )
    ASSERT_ALLOCATED_LIGHT( lr )

    art_gv->arlight_gv->_alf_dl_div_l(
        art_gv,
        d0,
        l0->value,
        lr->value
        );

    ASSERT_VALID_LIGHT( lr )
    ASSERT_COAXIAL_REFERENCE_FRAMES_LL( l0, lr, 3.0 DEGREES)
}

void arlight_dl_pow_l(
        const ART_GV  * art_gv,
        const double    d0,
        const ArLight * l0,
              ArLight * lr
        )
{
    ASSERT_VALID_LIGHT( l0 )
    ASSERT_ALLOCATED_LIGHT( lr )

    art_gv->arlight_gv->_alf_dl_pow_l(
        art_gv,
        d0,
        l0->value,
        lr->value
        );

    ASSERT_VALID_LIGHT( lr )
    ASSERT_COAXIAL_REFERENCE_FRAMES_LL( l0, lr, 3.0 DEGREES)
}

void arlight_ddl_clamp_l(
        const ART_GV  * art_gv,
        const double    d0,
        const double    d1,
        const ArLight * l0,
              ArLight * lr
        )
{
    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_NONNEGATIVE_DOUBLE( d1 )
    ASSERT_VALID_LIGHT( l0 )
    ASSERT_ALLOCATED_LIGHT( lr )

    art_gv->arlight_gv->_alf_ddl_clamp_l(
        art_gv,
        d0,
        d1,
        l0->value,
        lr->value
        );

    ASSERT_VALID_LIGHT( lr )
    ASSERT_COAXIAL_REFERENCE_FRAMES_LL( l0, lr, 3.0 DEGREES)
}

void arlight_dl_mul_add_l(
        const ART_GV  * art_gv,
        const double    d0,
        const ArLight * l0,
              ArLight * lr
        )
{
    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_VALID_LIGHT( l0 )
    ASSERT_VALID_LIGHT( lr )
    ASSERT_COAXIAL_REFERENCE_FRAMES_LL( l0, lr, 3.0 DEGREES)

    art_gv->arlight_gv->_alf_dl_mul_add_l(
        art_gv,
        d0,
        l0->value,
        lr->value
        );

    ASSERT_VALID_LIGHT( lr )
    ASSERT_COAXIAL_REFERENCE_FRAMES_LL( l0, lr, 3.0 DEGREES)
}

void arlight_dll_interpol_l(
        const ART_GV  * art_gv,
        const double    d0,
        const ArLight * l0,
        const ArLight * l1,
              ArLight * lr
        )
{
    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_VALID_LIGHT( l0 )
    ASSERT_VALID_LIGHT( l1 )
    ASSERT_ALLOCATED_LIGHT( lr )
    ASSERT_COAXIAL_REFERENCE_FRAMES_LL( l0, l1, 3.0 DEGREES)

    art_gv->arlight_gv->_alf_dll_interpol_l(
        art_gv,
        d0,
        l0->value,
        l1->value,
        lr->value
        );

    ASSERT_VALID_LIGHT( lr )
    ASSERT_COAXIAL_REFERENCE_FRAMES_LL( l0, lr, 3.0 DEGREES)
}

unsigned int arlight_l_valid(
        const ART_GV  * art_gv,
        const ArLight * l0
        )
{
    if ( ! l0 )
    {
        ART_ERRORHANDLING_WARNING(
            "unallocated ArLight argument 'l0'"
            );
        return 0;
    }

    return
        art_gv->arlight_gv->_alf_l_valid(
            art_gv,
            l0->value
            );
}

unsigned int arlight_ll_collinear(
        const ART_GV  * art_gv,
        const ArLight * l0,
        const ArLight * l1
        )
{
    return
        art_gv->arlight_gv->_alf_ll_collinear(
            art_gv,
            l0->value,
            l1->value
            );
}

void arlight_l_debugprintf(
        const ART_GV  * art_gv,
        const ArLight * l0
        )
{
    debugprintf(
        "ArLight [%p]: ",
        l0
        );

    art_gv->arlight_gv->_alf_l_debugprintf(
        art_gv,
        l0->value
        );
}

// ===========================================================================
