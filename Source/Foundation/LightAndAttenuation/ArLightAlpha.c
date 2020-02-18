/* ===========================================================================

    Copyright (c) 1996-2020 The ART Development Team
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

#define ART_MODULE_NAME     ArLightAlpha

#include "ArLightAlpha.h"

#include "_ArLight_GV.h"
#include "FoundationAssertionMacros.h"

typedef struct ArLightAlpha_GV
{
    ArLightAlpha  * light_unit;
    ArLightAlpha  * light_none_a0;
    ArLightAlpha  * light_none_a1;
}
ArLightAlpha_GV;

#define ARLIGHTAPLHA_GV   art_gv->arlightalpha_gv

#define ARLIGHTALPHA_GV_UNIT      ARLIGHTAPLHA_GV->light_unit
#define ARLIGHTALPHA_GV_NONE_A0   ARLIGHTAPLHA_GV->light_none_a0
#define ARLIGHTALPHA_GV_NONE_A1   ARLIGHTAPLHA_GV->light_none_a1

ART_MODULE_INITIALISATION_FUNCTION
(
    ARLIGHTAPLHA_GV = ALLOC(ArLightAlpha_GV);

    ARLIGHTALPHA_GV_UNIT    = 0;
    ARLIGHTALPHA_GV_NONE_A0 = 0;
    ARLIGHTALPHA_GV_NONE_A1 = 0;
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    if ( ARLIGHTALPHA_GV_UNIT )
        arlightalpha_free( art_gv, ARLIGHTALPHA_GV_UNIT );
    if ( ARLIGHTALPHA_GV_NONE_A0 )
        arlightalpha_free( art_gv, ARLIGHTALPHA_GV_NONE_A0 );
    if ( ARLIGHTALPHA_GV_NONE_A1 )
        arlightalpha_free( art_gv, ARLIGHTALPHA_GV_NONE_A1 );

    FREE( ARLIGHTAPLHA_GV );
)

ART_LIGHT_AND_ATTENUATION_MODULE_INITIALISATION_FUNCTION
(
    ARLIGHTALPHA_GV_UNIT =
        arlightalpha_d_alloc_init_unpolarised(
            art_gv,
            1.0
            );

    ARLIGHTALPHA_GV_NONE_A0 =
        arlightalpha_d_alloc_init_unpolarised(
            art_gv,
            0.0
            );

    ARLIGHTALPHA_A( * ARLIGHTALPHA_GV_NONE_A0 ) = 0.0;

    ARLIGHTALPHA_GV_NONE_A1 =
        arlightalpha_d_alloc_init_unpolarised(
            art_gv,
            0.0
            );
)

ART_LIGHT_AND_ATTENUATION_MODULE_SHUTDOWN_FUNCTION
(
    if ( ARLIGHTALPHA_GV_UNIT )
        arlightalpha_free( art_gv, ARLIGHTALPHA_GV_UNIT );
    if ( ARLIGHTALPHA_GV_NONE_A0 )
        arlightalpha_free( art_gv, ARLIGHTALPHA_GV_NONE_A0 );
    if ( ARLIGHTALPHA_GV_NONE_A1 )
        arlightalpha_free( art_gv, ARLIGHTALPHA_GV_NONE_A1 );
)


ArLightAlpha * arlightalpha_alloc(
        const ART_GV  * art_gv
        )
{
    ArLightAlpha  * newLight = ALLOC( ArLightAlpha );

    ARLIGHTALPHA_L( *newLight ) = arlight_alloc( art_gv );

    ASSERT_ALLOCATED_LIGHTALPHA( newLight )

    return newLight;
}

void arlightalpha_free(
        const ART_GV        * art_gv,
              ArLightAlpha  * lr
        )
{
    ASSERT_ALLOCATED_LIGHTALPHA( lr )

    arlight_free( art_gv, ARLIGHTALPHA_L(*lr) );

    FREE( lr );
}

ArLightAlpha * arlightalpha_d_alloc_init_unpolarised(
        const ART_GV  * art_gv,
        const double    d0
        )
{
    ASSERT_NONNEGATIVE_DOUBLE( d0 )

    ArLightAlpha  * newLightAlpha =
        arlightalpha_alloc( art_gv );

    arlight_d_init_unpolarised_l(
        art_gv,
        d0,
        ARLIGHTALPHA_L(*newLightAlpha)
        );

    ARLIGHTALPHA_A(*newLightAlpha) = 1.0;

    ASSERT_VALID_LIGHTALPHA( newLightAlpha )

    return newLightAlpha;
}

ArLightAlpha *  arlightalpha_l_alloc_init(
        const ART_GV        * art_gv,
        const ArLightAlpha  * l0
        )
{
    ASSERT_VALID_LIGHTALPHA( l0 )

    ArLightAlpha  * newLightAlpha = arlightalpha_alloc( art_gv );

    arlight_l_init_l(
        art_gv,
        ARLIGHTALPHA_L(*l0),
        ARLIGHTALPHA_L(*newLightAlpha)
        );

    ARLIGHTALPHA_A(*newLightAlpha) = ARLIGHTALPHA_A(*l0);

    ASSERT_VALID_LIGHTALPHA( newLightAlpha )

    return newLightAlpha;
}

ArLightAlpha const * arlightalpha_unpolarised_unit(
        const ART_GV  * art_gv
        )
{
    ASSERT_VALID_LIGHTALPHA( ARLIGHTALPHA_GV_UNIT )

    return
        ARLIGHTALPHA_GV_UNIT;
}

ArLightAlpha const * arlightalpha_none_a0(
        const ART_GV  * art_gv
        )
{
    ASSERT_VALID_LIGHTALPHA( ARLIGHTALPHA_GV_NONE_A0 )

    return
        ARLIGHTALPHA_GV_NONE_A0;
}

ArLightAlpha const * arlightalpha_none(
        const ART_GV  * art_gv
        )
{
    ASSERT_VALID_LIGHTALPHA( ARLIGHTALPHA_GV_NONE_A1 )

    return
        ARLIGHTALPHA_GV_NONE_A1;
}

void arlightalpha_d_init_unpolarised_l(
        const ART_GV        * art_gv,
        const double          d0,
              ArLightAlpha  * lr
        )
{
    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_ALLOCATED_LIGHTALPHA( lr )

    art_gv->arlight_gv->_alf_d_init_unpolarised_l(
        art_gv,
        d0,
        lr->light->value
        );

    lr->alpha = 1.0;

    ASSERT_VALID_LIGHTALPHA( lr )
}

void arlightalpha_s_init_unpolarised_l(
        const ART_GV        * art_gv,
        const ArSpectrum      * c0,
              ArLightAlpha  * lr
        )
{
    ASSERT_VALID_SPECTRUM( c0 )
    ASSERT_ALLOCATED_LIGHTALPHA( lr )

    art_gv->arlight_gv->_alf_s_init_unpolarised_l(
        art_gv,
        c0,
        lr->light->value
        );

    lr->alpha = 1.0;

    ASSERT_VALID_LIGHTALPHA( lr )
}

void arlightalpha_l_init_l(
        const ART_GV        * art_gv,
        const ArLightAlpha  * l0,
              ArLightAlpha  * lr
        )
{
    ASSERT_VALID_LIGHTALPHA( l0 )
    ASSERT_ALLOCATED_LIGHTALPHA( lr )

    art_gv->arlight_gv->_alf_l_init_l(
        art_gv,
        l0->light->value,
        lr->light->value
        );

    lr->alpha = l0->alpha;

    ASSERT_VALID_LIGHTALPHA( lr )
}

void arlightalpha_s_rf_init_polarised_l(
        const ART_GV            * art_gv,
        const ArStokesVector    * s0,
        const ArReferenceFrame  * r0,
              ArLightAlpha      * lr
        )
{
    ASSERT_VALID_STOKES_VECTOR( s0 )
    ASSERT_VALID_REFERENCE_FRAME( r0 )
    ASSERT_ALLOCATED_LIGHTALPHA( lr )

    art_gv->arlight_gv->_alf_s_rf_init_polarised_l(
        art_gv,
        s0,
        r0,
        lr->light->value
        );

    lr->alpha = 1.0;

    ASSERT_VALID_LIGHTALPHA( lr )
}

void arlightalpha_l_init_s(
        const ART_GV        * art_gv,
        const ArLightAlpha  * l0,
              ArSpectrum      * cr
        )
{
    ASSERT_VALID_LIGHTALPHA( l0 )

    art_gv->arlight_gv->_alf_l_init_s(
        art_gv,
        l0->light->value,
        cr
        );

    ASSERT_VALID_SPECTRUM( cr )
}

void arlightalpha_l_init_i(
        const ART_GV            * art_gv,
        const ArLightAlpha      * l0,
              ArLightIntensity  * ir
        )
{
    ASSERT_VALID_LIGHTALPHA( l0 )

    art_gv->arlight_gv->_alf_l_init_i(
        art_gv,
        l0->light->value,
        ir
        );

    ASSERT_VALID_LIGHT_INTENSITY( ir )
}

unsigned int  arlightalpha_l_polarised(
        const ART_GV        * art_gv,
        const ArLightAlpha  * l0
        )
{
    ASSERT_VALID_LIGHTALPHA( l0 )

    return
        art_gv->arlight_gv->_alf_l_polarised(
            art_gv,
            l0->light->value
            );
}

void arlightalpha_l_to_sv(
        const ART_GV          * art_gv,
        const ArLightAlpha    * l0,
              ArStokesVector  * sr
        )
{
    ASSERT_VALID_LIGHTALPHA( l0 )

    art_gv->arlight_gv->_alf_l_to_sv(
        art_gv,
        l0->light->value,
        sr
        );

    ASSERT_VALID_STOKES_VECTOR( sr )
}

void arlightalpha_ld_realign_to_coaxial_refframe_sv(
        const ART_GV            * art_gv,
        const ArLightAlpha      * l0,
        const double              d0,
        const ArReferenceFrame  * r0,
              ArStokesVector    * sr
        )
{
    ASSERT_VALID_LIGHTALPHA( l0 )
    ASSERT_VALID_REFERENCE_FRAME( r0 )

    art_gv->arlight_gv->_alf_ld_realign_to_coaxial_refframe_sv(
        art_gv,
        l0->light->value,
        d0,
        r0,
        sr
        );

    ASSERT_VALID_STOKES_VECTOR( sr )
}

void arlightalpha_realign_to_coaxial_refframe_l(
        const ART_GV            * art_gv,
        const ArReferenceFrame  * r0,
              ArLightAlpha      * lr
        )
{
    ASSERT_VALID_REFERENCE_FRAME( r0 )
    ASSERT_VALID_LIGHTALPHA( lr )

    art_gv->arlight_gv->_alf_realign_to_coaxial_refframe_l(
        art_gv,
        r0,
        lr->light->value
        );

    ASSERT_VALID_LIGHTALPHA( lr )
}

double arlightalpha_l_norm(
        const ART_GV       * art_gv,
        const ArLightAlpha * l0
        )
{
    ASSERT_VALID_LIGHTALPHA( l0 )

    return
        art_gv->arlight_gv->_alf_l_norm(
            art_gv,
            l0->light->value
            );
}

void arlightalpha_l_inv_l(
        const ART_GV       * art_gv,
        const ArLightAlpha * l0,
              ArLightAlpha * lr
        )
{
    ASSERT_VALID_LIGHTALPHA( l0 )
    ASSERT_ALLOCATED_LIGHTALPHA( lr )

    art_gv->arlight_gv->_alf_l_inv_l(
        art_gv,
        l0->light->value,
        lr->light->value
        );

    ASSERT_VALID_LIGHTALPHA( lr )
}

double arlightalpha_l_max(
        const ART_GV       * art_gv,
        const ArLightAlpha * l0
        )
{
    ASSERT_VALID_LIGHTALPHA( l0 )

    return
        art_gv->arlight_gv->_alf_l_max(
            art_gv,
            l0->light->value
            );
}

void arlightalpha_ll_max_l(
        const ART_GV       * art_gv,
        const ArLightAlpha * l0,
        const ArLightAlpha * l1,
              ArLightAlpha * lr
        )
{
    ASSERT_VALID_LIGHTALPHA( l0 )
    ASSERT_VALID_LIGHTALPHA( l1 )
    ASSERT_ALLOCATED_LIGHTALPHA( lr )

    art_gv->arlight_gv->_alf_ll_max_l(
        art_gv,
        l0->light->value,
        l1->light->value,
        lr->light->value
        );

    lr->alpha = M_MAX( l0->alpha, l1->alpha );

    ASSERT_VALID_LIGHTALPHA( lr )
}

double arlightalpha_l_min(
        const ART_GV       * art_gv,
        const ArLightAlpha * l0
        )
{
    ASSERT_VALID_LIGHTALPHA( l0 )

    return
        art_gv->arlight_gv->_alf_l_min(
            art_gv,
            l0->light->value
            );
}

void arlightalpha_ll_min_l(
        const ART_GV       * art_gv,
        const ArLightAlpha * l0,
        const ArLightAlpha * l1,
              ArLightAlpha * lr
        )
{
    ASSERT_VALID_LIGHTALPHA( l0 )
    ASSERT_VALID_LIGHTALPHA( l1 )
    ASSERT_ALLOCATED_LIGHTALPHA( lr )

    art_gv->arlight_gv->_alf_ll_min_l(
        art_gv,
        l0->light->value,
        l1->light->value,
        lr->light->value
        );

    lr->alpha = M_MIN( l0->alpha, l1->alpha );

    ASSERT_VALID_LIGHTALPHA( lr )
}

double arlightalpha_ll_maxdiff(
        const ART_GV       * art_gv,
        const ArLightAlpha * l0,
        const ArLightAlpha * l1
        )
{
    ASSERT_VALID_LIGHTALPHA( l0 )
    ASSERT_VALID_LIGHTALPHA( l1 )

    return
        art_gv->arlight_gv->_alf_ll_maxdiff(
            art_gv,
            l0->light->value,
            l1->light->value
            );
}

void arlightalpha_i_add_l(
        const ART_GV            * art_gv,
        const ArLightIntensity  * i0,
              ArLightAlpha      * lr
        )
{
    ASSERT_VALID_LIGHT_INTENSITY( i0 )
    ASSERT_VALID_LIGHTALPHA( lr )

    art_gv->arlight_gv->_alf_i_add_l(
        art_gv,
        i0,
        lr->light->value
        );

    ASSERT_VALID_LIGHTALPHA( lr )
}

void arlightalpha_l_add_l(
        const ART_GV        * art_gv,
        const ArLightAlpha  * l0,
              ArLightAlpha  * lr
        )
{
    ASSERT_VALID_LIGHTALPHA( l0 )
    ASSERT_VALID_LIGHTALPHA( lr )

    art_gv->arlight_gv->_alf_l_add_l(
        art_gv,
        l0->light->value,
        lr->light->value
        );

    lr->alpha += l0->alpha;

    ASSERT_VALID_LIGHTALPHA( lr )
}

void arlightalpha_dl_sloppy_add_l(
        const ART_GV        * art_gv,
        const double          d0,
        const ArLightAlpha  * l0,
              ArLightAlpha  * lr
        )
{
    ASSERT_VALID_LIGHTALPHA( l0 )
    ASSERT_VALID_LIGHTALPHA( lr )

    art_gv->arlight_gv->_alf_dl_sloppy_add_l(
        art_gv,
        d0,
        l0->light->value,
        lr->light->value
        );

    lr->alpha += l0->alpha;

    ASSERT_VALID_LIGHTALPHA( lr )
}

void arlightalpha_l_atomic_add_l(
        const ART_GV        * art_gv,
        const ArLightAlpha  * l0,
              ArLightAlpha  * lr
        )
{
    ASSERT_VALID_LIGHTALPHA( l0 )
    ASSERT_VALID_LIGHTALPHA( lr )

    arlight_l_atomic_add_l(
        art_gv,
        ARLIGHTALPHA_L(*l0),
        ARLIGHTALPHA_L(*lr)
        );

    ARLIGHTALPHA_A(*lr) =
        m_dd_atomic_add(
              ARLIGHTALPHA_A(*l0),
            & ARLIGHTALPHA_A(*lr)
            );

    ASSERT_VALID_LIGHTALPHA( lr )
}

void arlightalpha_ll_add_l(
        const ART_GV       * art_gv,
        const ArLightAlpha * l0,
        const ArLightAlpha * l1,
              ArLightAlpha * lr
        )
{
    ASSERT_VALID_LIGHTALPHA( l0 )
    ASSERT_VALID_LIGHTALPHA( l1 )
    ASSERT_ALLOCATED_LIGHTALPHA( lr )

    arlight_ll_add_l(
        art_gv,
        ARLIGHTALPHA_L(*l0),
        ARLIGHTALPHA_L(*l1),
        ARLIGHTALPHA_L(*lr)
        );

    ARLIGHTALPHA_A( *lr ) = ARLIGHTALPHA_A( *l0 ) + ARLIGHTALPHA_A( *l1 );

    ASSERT_VALID_LIGHTALPHA( lr )
}

void arlightalpha_lld_sloppy_add_l(
        const ART_GV       * art_gv,
        const ArLightAlpha * l0,
        const ArLightAlpha * l1,
        const double         d0,
              ArLightAlpha * lr
        )
{
    ASSERT_VALID_LIGHTALPHA( l0 )
    ASSERT_VALID_LIGHTALPHA( l1 )
    ASSERT_ALLOCATED_LIGHTALPHA( lr )

    arlight_lld_sloppy_add_l(
        art_gv,
        ARLIGHTALPHA_L(*l0),
        ARLIGHTALPHA_L(*l1),
        d0,
        ARLIGHTALPHA_L(*lr)
        );

    ARLIGHTALPHA_A( *lr ) = ARLIGHTALPHA_A( *l0 ) + ARLIGHTALPHA_A( *l1 );

    ASSERT_VALID_LIGHTALPHA( lr )
}

void arlightalpha_ll_sub_l(
        const ART_GV       * art_gv,
        const ArLightAlpha * l0,
        const ArLightAlpha * l1,
              ArLightAlpha * lr
        )
{
    ASSERT_VALID_LIGHTALPHA( l0 )
    ASSERT_VALID_LIGHTALPHA( l1 )
    ASSERT_ALLOCATED_LIGHTALPHA( lr )

    arlight_ll_sub_l(
        art_gv,
        ARLIGHTALPHA_L(*l0),
        ARLIGHTALPHA_L(*l1),
        ARLIGHTALPHA_L(*lr)
        );

    ARLIGHTALPHA_A( *lr ) = ARLIGHTALPHA_A( *l0 ) - ARLIGHTALPHA_A( *l1 );

    ASSERT_VALID_LIGHTALPHA( lr )
}

void arlightalpha_d_mul_l(
        const ART_GV       * art_gv,
        const double         d0,
              ArLightAlpha * lr
        )
{
    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_VALID_LIGHTALPHA( lr )

    art_gv->arlight_gv->_alf_d_mul_l(
        art_gv,
        d0,
        lr->light->value
        );

    lr->alpha *= d0;

    ASSERT_VALID_LIGHTALPHA( lr )
}

void arlightalpha_dl_mul_l(
        const ART_GV       * art_gv,
        const double         d0,
        const ArLightAlpha * l0,
              ArLightAlpha * lr
        )
{
    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_VALID_LIGHTALPHA( l0 )
    ASSERT_ALLOCATED_LIGHTALPHA( lr )

    art_gv->arlight_gv->_alf_dl_mul_l(
        art_gv,
        d0,
        l0->light->value,
        lr->light->value
        );

    lr->alpha = d0 * l0->alpha;

    ASSERT_VALID_LIGHTALPHA( lr )
}

void arlightalpha_i_mul_l(
        const ART_GV            * art_gv,
        const ArLightIntensity  * i0,
              ArLightAlpha      * lr
        )
{
    ASSERT_VALID_LIGHT_INTENSITY( i0 )
    ASSERT_VALID_LIGHTALPHA( lr )

    art_gv->arlight_gv->_alf_i_mul_l(
        art_gv,
        i0,
        lr->light->value
        );

    ASSERT_VALID_LIGHTALPHA( lr )
}

void arlightalpha_l_mul_l(
        const ART_GV       * art_gv,
        const ArLightAlpha * l0,
              ArLightAlpha * lr
        )
{
    ASSERT_VALID_LIGHTALPHA( l0 )
    ASSERT_VALID_LIGHTALPHA( lr )

    art_gv->arlight_gv->_alf_l_mul_l(
        art_gv,
        l0->light->value,
        lr->light->value
        );

    ASSERT_VALID_LIGHTALPHA( lr )
}

void arlightalpha_ll_mul_l(
        const ART_GV       * art_gv,
        const ArLightAlpha * l0,
        const ArLightAlpha * l1,
              ArLightAlpha * lr
        )
{
    ASSERT_VALID_LIGHTALPHA( l0 )
    ASSERT_VALID_LIGHTALPHA( l1 )
    ASSERT_ALLOCATED_LIGHTALPHA( lr )

    art_gv->arlight_gv->_alf_ll_mul_l(
        art_gv,
        l0->light->value,
        l1->light->value,
        lr->light->value
        );

    ASSERT_VALID_LIGHTALPHA( lr )
}

void arlightalpha_d_div_l(
        const ART_GV       * art_gv,
        const double         d0,
              ArLightAlpha * lr
        )
{
    ASSERT_POSITIVE_DOUBLE( d0 )
    ASSERT_VALID_LIGHTALPHA( lr )

    art_gv->arlight_gv->_alf_d_div_l(
        art_gv,
        d0,
        lr->light->value
        );

    ASSERT_VALID_LIGHTALPHA( lr )
}

void arlightalpha_dl_div_l(
        const ART_GV       * art_gv,
        const double         d0,
        const ArLightAlpha * l0,
              ArLightAlpha * lr
        )
{
    ASSERT_POSITIVE_DOUBLE( d0 )
    ASSERT_VALID_LIGHTALPHA( l0 )
    ASSERT_ALLOCATED_LIGHTALPHA( lr )

    art_gv->arlight_gv->_alf_dl_div_l(
        art_gv,
        d0,
        l0->light->value,
        lr->light->value
        );

    ASSERT_VALID_LIGHTALPHA( lr )
}

void arlightalpha_dl_pow_l(
        const ART_GV       * art_gv,
        const double         d0,
        const ArLightAlpha * l0,
              ArLightAlpha * lr
        )
{
    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_VALID_LIGHTALPHA( l0 )
    ASSERT_ALLOCATED_LIGHTALPHA( lr )

    art_gv->arlight_gv->_alf_dl_pow_l(
        art_gv,
        d0,
        l0->light->value,
        lr->light->value
        );

    ASSERT_VALID_LIGHTALPHA( lr )
}

void arlightalpha_ddl_clamp_l(
        const ART_GV       * art_gv,
        const double         d0,
        const double         d1,
        const ArLightAlpha * l0,
              ArLightAlpha * lr
        )
{
    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_NONNEGATIVE_DOUBLE( d1 )
    ASSERT_VALID_LIGHTALPHA( l0 )
    ASSERT_ALLOCATED_LIGHTALPHA( lr )

    art_gv->arlight_gv->_alf_ddl_clamp_l(
        art_gv,
        d0,
        d1,
        l0->light->value,
        lr->light->value
        );

    ASSERT_VALID_LIGHTALPHA( lr )
}

void arlightalpha_dl_mul_add_l(
        const ART_GV       * art_gv,
        const double         d0,
        const ArLightAlpha * l0,
              ArLightAlpha * lr
        )
{
    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_VALID_LIGHTALPHA( l0 )
    ASSERT_ALLOCATED_LIGHTALPHA( lr )

    art_gv->arlight_gv->_alf_dl_mul_add_l(
        art_gv,
        d0,
        l0->light->value,
        lr->light->value
        );

    ARLIGHTALPHA_A(*lr) += d0 * ARLIGHTALPHA_A(*l0);

    ASSERT_VALID_LIGHTALPHA( lr )
}

void arlightalpha_dld_mul_sloppy_add_l(
        const ART_GV       * art_gv,
        const double         d0,
        const ArLightAlpha * l0,
        const double         d1,
              ArLightAlpha * lr
        )
{
    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_VALID_LIGHTALPHA( l0 )
    ASSERT_ALLOCATED_LIGHTALPHA( lr )

    art_gv->arlight_gv->_alf_dld_mul_sloppy_add_l(
        art_gv,
        d0,
        l0->light->value,
        d1,
        lr->light->value
        );

    ARLIGHTALPHA_A(*lr) += d0 * ARLIGHTALPHA_A(*l0);

    ASSERT_VALID_LIGHTALPHA( lr )
}

void arlightalpha_dll_interpol_l(
        const ART_GV       * art_gv,
        const double         d0,
        const ArLightAlpha * l0,
        const ArLightAlpha * l1,
              ArLightAlpha * lr
        )
{
    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_VALID_LIGHTALPHA( l0 )
    ASSERT_VALID_LIGHTALPHA( l1 )
    ASSERT_ALLOCATED_LIGHTALPHA( lr )

    art_gv->arlight_gv->_alf_dll_interpol_l(
        art_gv,
        d0,
        l0->light->value,
        l1->light->value,
        lr->light->value
        );

    ASSERT_VALID_LIGHTALPHA( lr )
}

unsigned int arlightalpha_l_valid(
        const ART_GV       * art_gv,
        const ArLightAlpha * l0
        )
{
    if ( ARLIGHTALPHA_A( *l0 ) < 0.0 )
    {
        ART_ERRORHANDLING_WARNING(
            "light alpha is negative (%f)",
            ARLIGHTALPHA_A( *l0 )
            );
        return 0;
    }

    return
        art_gv->arlight_gv->_alf_l_valid(
            art_gv,
            l0->light->value
            );
}

void arlightalpha_l_debugprintf(
        const ART_GV       * art_gv,
        const ArLightAlpha * l0
        )
{
    printf("ArLightAlpha: ");

    art_gv->arlight_gv->_alf_l_debugprintf(
        art_gv,
        l0->light->value
        );

    printf("ArLightAlpha: Alpha: %7.6f\n",l0->alpha);
}

void spc_to_arlightalpha(
        const ART_GV        * art_gv,
        const ArSpectrum      * c0,
              ArLightAlpha  * lr
        )
{
    art_gv->arlight_gv->_alf_s_init_unpolarised_l(
        art_gv,
        c0,
        lr->light->value
        );

    lr->alpha = 1.0;
}

void arlightalpha_to_spc(
        const ART_GV        * art_gv,
        const ArLightAlpha  * l0,
              ArSpectrum      * cr
        )
{
    art_gv->arlight_gv->_alf_l_init_s(
        art_gv,
        l0->light->value,
        cr
        );
}

void arlightalpha_wsd_sloppy_add_l(
        const ART_GV                         * art_gv,
        const ArLightAlphaSample             * l0,
        const ArWavelength                   * w0,
        const ArSpectralSampleSplattingData  * s0,
        const double                           d0,
              ArLightAlpha                   * lr
        )
{
    ASSERT_VALID_LIGHTALPHA_SAMPLE( l0 )
    ASSERT_VALID_WAVELENGTH( w0 )
    ASSERT_VALID_LIGHTALPHA( lr )

    art_gv->arlight_gv->_alf_wsd_sloppy_add_l(
        art_gv,
        l0->light->value,
        w0,
        s0,
        d0,
        lr->light->value
        );

    ARLIGHTALPHA_A(*lr) += ARLIGHTALPHA_A(*l0);

    ASSERT_VALID_LIGHTALPHA( lr )
}

void arlightalpha_dwsd_mul_sloppy_add_l(
        const ART_GV                         * art_gv,
        const double                           d0,
        const ArLightAlphaSample             * l0,
        const ArWavelength                   * w0,
        const ArSpectralSampleSplattingData  * s0,
        const double                           d1,
              ArLightAlpha                   * lr
        )
{
    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_VALID_LIGHTALPHA_SAMPLE( l0 )
    ASSERT_VALID_WAVELENGTH( w0 )
    ASSERT_ALLOCATED_LIGHTALPHA( lr )

    art_gv->arlight_gv->_alf_dwsd_mul_sloppy_add_l(
        art_gv,
        d0,
        l0->light->value,
        w0,
        s0,
        d1,
        lr->light->value
        );

    ARLIGHTALPHA_A(*lr) += d0 * ARLIGHTALPHA_A(*l0);

    ASSERT_VALID_LIGHTALPHA( lr )
}

void arlightalpha_to_arlightalpha(
        const ART_GV        * art_gv,
        const ArLightAlpha  * l0,
              ArLightAlpha  * lr
        )
{
    art_gv->arlight_gv->_alf_l_init_l(
        art_gv,
        l0->light->value,
        lr->light->value
        );

    lr->alpha = l0->alpha;
}


// ===========================================================================
