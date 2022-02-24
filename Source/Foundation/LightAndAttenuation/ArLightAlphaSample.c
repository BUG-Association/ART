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

#define ART_MODULE_NAME     ArLightAlphaSample

#include "ArLightAlphaSample.h"

#include "_ArLightSample_GV.h"
#include "FoundationAssertionMacros.h"

typedef struct ArLightAlphaSample_GV
{
    ArLightAlphaSample  * light_unit;
    ArLightAlphaSample  * light_none_a0;
    ArLightAlphaSample  * light_none_a1;
}
ArLightAlphaSample_GV;

#define ARLIGHTAPLHA_GV   art_gv->arlightalphasample_gv

#define ARLIGHTALPHASAMPLE_GV_UNIT      ARLIGHTAPLHA_GV->light_unit
#define ARLIGHTALPHASAMPLE_GV_NONE_A0   ARLIGHTAPLHA_GV->light_none_a0
#define ARLIGHTALPHASAMPLE_GV_NONE_A1   ARLIGHTAPLHA_GV->light_none_a1

ART_MODULE_INITIALISATION_FUNCTION
(
    ARLIGHTAPLHA_GV = ALLOC(ArLightAlphaSample_GV);

    ARLIGHTALPHASAMPLE_GV_UNIT    = 0;
    ARLIGHTALPHASAMPLE_GV_NONE_A0 = 0;
    ARLIGHTALPHASAMPLE_GV_NONE_A1 = 0;
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    if ( ARLIGHTALPHASAMPLE_GV_UNIT )
        arlightalphasample_free( art_gv, ARLIGHTALPHASAMPLE_GV_UNIT );
    if ( ARLIGHTALPHASAMPLE_GV_NONE_A0 )
        arlightalphasample_free( art_gv, ARLIGHTALPHASAMPLE_GV_NONE_A0 );
    if ( ARLIGHTALPHASAMPLE_GV_NONE_A1 )
        arlightalphasample_free( art_gv, ARLIGHTALPHASAMPLE_GV_NONE_A1 );

    FREE( ARLIGHTAPLHA_GV );
)

ART_LIGHT_AND_ATTENUATION_MODULE_INITIALISATION_FUNCTION
(
    ARLIGHTALPHASAMPLE_GV_UNIT =
        arlightalphasample_d_alloc_init_unpolarised(
            art_gv,
            1.0
            );

    ARLIGHTALPHASAMPLE_GV_NONE_A0 =
        arlightalphasample_d_alloc_init_unpolarised(
            art_gv,
            0.0
            );

    ARLIGHTALPHASAMPLE_A( * ARLIGHTALPHASAMPLE_GV_NONE_A0 ) = 0.0;

    ARLIGHTALPHASAMPLE_GV_NONE_A1 =
        arlightalphasample_d_alloc_init_unpolarised(
            art_gv,
            0.0
            );
)

ART_LIGHT_AND_ATTENUATION_MODULE_SHUTDOWN_FUNCTION
(
    if ( ARLIGHTALPHASAMPLE_GV_UNIT )
        arlightalphasample_free( art_gv, ARLIGHTALPHASAMPLE_GV_UNIT );
    if ( ARLIGHTALPHASAMPLE_GV_NONE_A0 )
        arlightalphasample_free( art_gv, ARLIGHTALPHASAMPLE_GV_NONE_A0 );
    if ( ARLIGHTALPHASAMPLE_GV_NONE_A1 )
        arlightalphasample_free( art_gv, ARLIGHTALPHASAMPLE_GV_NONE_A1 );
)


ArLightAlphaSample * arlightalphasample_alloc(
        const ART_GV  * art_gv
        )
{
    ArLightAlphaSample  * newLight = ALLOC( ArLightAlphaSample );

    ARLIGHTALPHASAMPLE_L( *newLight ) = arlightsample_alloc( art_gv );

    ASSERT_ALLOCATED_LIGHTALPHA_SAMPLE( newLight )

    return newLight;
}

void arlightalphasample_free(
        const ART_GV            * art_gv,
              ArLightAlphaSample  * lr
        )
{
    ASSERT_ALLOCATED_LIGHTALPHA_SAMPLE( lr )

    arlightsample_free( art_gv, ARLIGHTALPHASAMPLE_L(*lr) );

    FREE( lr );
}

ArLightAlphaSample * arlightalphasample_d_alloc_init_unpolarised(
        const ART_GV  * art_gv,
        const double    d0
        )
{
    ASSERT_NONNEGATIVE_DOUBLE( d0 )

    ArLightAlphaSample  * newLightAlpha =
        arlightalphasample_alloc( art_gv );

    arlightsample_d_init_unpolarised_l(
        art_gv,
        d0,
        ARLIGHTALPHASAMPLE_L(*newLightAlpha)
        );

    ARLIGHTALPHASAMPLE_A(*newLightAlpha) = 1.0;

    ASSERT_VALID_LIGHTALPHA_SAMPLE( newLightAlpha )

    return newLightAlpha;
}

ArLightAlphaSample *  arlightalphasample_l_alloc_init(
        const ART_GV            * art_gv,
        const ArLightAlphaSample  * l0
        )
{
    ASSERT_VALID_LIGHTALPHA_SAMPLE( l0 )

    ArLightAlphaSample  * newLightAlpha = arlightalphasample_alloc( art_gv );

    arlightsample_l_init_l(
        art_gv,
        ARLIGHTALPHASAMPLE_L(*l0),
        ARLIGHTALPHASAMPLE_L(*newLightAlpha)
        );

    ARLIGHTALPHASAMPLE_A(*newLightAlpha) = ARLIGHTALPHASAMPLE_A(*l0);

    ASSERT_VALID_LIGHTALPHA_SAMPLE( newLightAlpha )

    return newLightAlpha;
}

ArLightAlphaSample const * arlightalphasample_unpolarised_unit(
        const ART_GV  * art_gv
        )
{
    ASSERT_VALID_LIGHTALPHA_SAMPLE( ARLIGHTALPHASAMPLE_GV_UNIT )

    return
        ARLIGHTALPHASAMPLE_GV_UNIT;
}

ArLightAlphaSample const * arlightalphasample_none_a0(
        const ART_GV  * art_gv
        )
{
    ASSERT_VALID_LIGHTALPHA_SAMPLE( ARLIGHTALPHASAMPLE_GV_NONE_A0 )

    return
        ARLIGHTALPHASAMPLE_GV_NONE_A0;
}

ArLightAlphaSample const * arlightalphasample_none(
        const ART_GV  * art_gv
        )
{
    ASSERT_VALID_LIGHTALPHA_SAMPLE( ARLIGHTALPHASAMPLE_GV_NONE_A1 )

    return
        ARLIGHTALPHASAMPLE_GV_NONE_A1;
}

void arlightalphasample_d_init_unpolarised_l(
        const ART_GV            * art_gv,
        const double              d0,
              ArLightAlphaSample  * lr
        )
{
    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_ALLOCATED_LIGHTALPHA_SAMPLE( lr )

    art_gv->arlightsample_gv->_alf_d_init_unpolarised_l(
        art_gv,
        d0,
        lr->light->value
        );

    lr->alpha = 1.0;

    ASSERT_VALID_LIGHTALPHA_SAMPLE( lr )
}

void arlightalphasample_s_init_unpolarised_l(
        const ART_GV            * art_gv,
        const ArSpectralSample    * c0,
              ArLightAlphaSample  * lr
        )
{
    ASSERT_VALID_SPECTRAL_SAMPLE( c0 )
    ASSERT_ALLOCATED_LIGHTALPHA_SAMPLE( lr )

    art_gv->arlightsample_gv->_alf_s_init_unpolarised_l(
        art_gv,
        c0,
        lr->light->value
        );

    lr->alpha = 1.0;

    ASSERT_VALID_LIGHTALPHA_SAMPLE( lr )
}

void arlightalphasample_l_init_l(
        const ART_GV            * art_gv,
        const ArLightAlphaSample  * l0,
              ArLightAlphaSample  * lr
        )
{
    ASSERT_VALID_LIGHTALPHA_SAMPLE( l0 )
    ASSERT_ALLOCATED_LIGHTALPHA_SAMPLE( lr )

    art_gv->arlightsample_gv->_alf_l_init_l(
        art_gv,
        l0->light->value,
        lr->light->value
        );

    lr->alpha = l0->alpha;

    ASSERT_VALID_LIGHTALPHA_SAMPLE( lr )
}

void arlightalphasample_s_rf_init_polarised_l(
        const ART_GV                * art_gv,
        const ArStokesVectorSample    * s0,
        const ArReferenceFrame      * r0,
              ArLightAlphaSample      * lr
        )
{
    ASSERT_VALID_STOKES_VECTOR_SAMPLE( s0 )
    ASSERT_VALID_REFERENCE_FRAME( r0 )
    ASSERT_ALLOCATED_LIGHTALPHA_SAMPLE( lr )

    art_gv->arlightsample_gv->_alf_s_rf_init_polarised_l(
        art_gv,
        s0,
        r0,
        lr->light->value
        );

    lr->alpha = 1.0;

    ASSERT_VALID_LIGHTALPHA_SAMPLE( lr )
}

void arlightalphasample_l_init_h(
        const ART_GV            * art_gv,
        const ArLightAlphaSample  * l0,
              ArSpectralSample    * cr
        )
{
    ASSERT_VALID_LIGHTALPHA_SAMPLE( l0 )

    art_gv->arlightsample_gv->_alf_l_init_h(
        art_gv,
        l0->light->value,
        cr
        );

    ASSERT_VALID_SPECTRAL_SAMPLE( cr )
}

void arlightalphasample_l_init_i(
        const ART_GV                * art_gv,
        const ArLightAlphaSample      * l0,
              ArLightIntensitySample  * ir
        )
{
    ASSERT_VALID_LIGHTALPHA_SAMPLE( l0 )

    art_gv->arlightsample_gv->_alf_l_init_i(
        art_gv,
        l0->light->value,
        ir
        );

    ASSERT_VALID_LIGHT_INTENSITY_SAMPLE( ir )
}

unsigned int  arlightalphasample_l_polarised(
        const ART_GV            * art_gv,
        const ArLightAlphaSample  * l0
        )
{
    ASSERT_VALID_LIGHTALPHA_SAMPLE( l0 )

    return
        art_gv->arlightsample_gv->_alf_l_polarised(
            art_gv,
            l0->light->value
            );
}

void arlightalphasample_l_to_sv(
        const ART_GV              * art_gv,
        const ArLightAlphaSample    * l0,
              ArStokesVectorSample  * sr
        )
{
    ASSERT_VALID_LIGHTALPHA_SAMPLE( l0 )

    art_gv->arlightsample_gv->_alf_l_to_sv(
        art_gv,
        l0->light->value,
        sr
        );

    ASSERT_VALID_STOKES_VECTOR_SAMPLE( sr )
}

void arlightalphasample_ld_realign_to_coaxial_refframe_sv(
        const ART_GV                * art_gv,
        const ArLightAlphaSample      * l0,
        const double                  d0,
        const ArReferenceFrame      * r0,
              ArStokesVectorSample    * sr
        )
{
    ASSERT_VALID_LIGHTALPHA_SAMPLE( l0 )
    ASSERT_VALID_REFERENCE_FRAME( r0 )

    art_gv->arlightsample_gv->_alf_ld_realign_to_coaxial_refframe_sv(
        art_gv,
        l0->light->value,
        d0,
        r0,
        sr
        );

    ASSERT_VALID_STOKES_VECTOR_SAMPLE( sr )
}

void arlightalphasample_realign_to_coaxial_refframe_l(
        const ART_GV                * art_gv,
        const ArReferenceFrame      * r0,
              ArLightAlphaSample      * lr
        )
{
    ASSERT_VALID_REFERENCE_FRAME( r0 )
    ASSERT_VALID_LIGHTALPHA_SAMPLE( lr )

    art_gv->arlightsample_gv->_alf_realign_to_coaxial_refframe_l(
        art_gv,
        r0,
        lr->light->value
        );

    ASSERT_VALID_LIGHTALPHA_SAMPLE( lr )
}

double arlightalphasample_l_norm(
        const ART_GV           * art_gv,
        const ArLightAlphaSample * l0
        )
{
    ASSERT_VALID_LIGHTALPHA_SAMPLE( l0 )

    return
        art_gv->arlightsample_gv->_alf_l_norm(
            art_gv,
            l0->light->value
            );
}

void arlightalphasample_l_inv_l(
        const ART_GV           * art_gv,
        const ArLightAlphaSample * l0,
              ArLightAlphaSample * lr
        )
{
    ASSERT_VALID_LIGHTALPHA_SAMPLE( l0 )
    ASSERT_ALLOCATED_LIGHTALPHA_SAMPLE( lr )

    art_gv->arlightsample_gv->_alf_l_inv_l(
        art_gv,
        l0->light->value,
        lr->light->value
        );

    ASSERT_VALID_LIGHTALPHA_SAMPLE( lr )
}

double arlightalphasample_l_max(
        const ART_GV           * art_gv,
        const ArLightAlphaSample * l0
        )
{
    ASSERT_VALID_LIGHTALPHA_SAMPLE( l0 )

    return
        art_gv->arlightsample_gv->_alf_l_max(
            art_gv,
            l0->light->value
            );
}

void arlightalphasample_ll_max_l(
        const ART_GV           * art_gv,
        const ArLightAlphaSample * l0,
        const ArLightAlphaSample * l1,
              ArLightAlphaSample * lr
        )
{
    ASSERT_VALID_LIGHTALPHA_SAMPLE( l0 )
    ASSERT_VALID_LIGHTALPHA_SAMPLE( l1 )
    ASSERT_ALLOCATED_LIGHTALPHA_SAMPLE( lr )

    art_gv->arlightsample_gv->_alf_ll_max_l(
        art_gv,
        l0->light->value,
        l1->light->value,
        lr->light->value
        );

    lr->alpha = M_MAX( l0->alpha, l1->alpha );

    ASSERT_VALID_LIGHTALPHA_SAMPLE( lr )
}

double arlightalphasample_l_min(
        const ART_GV           * art_gv,
        const ArLightAlphaSample * l0
        )
{
    ASSERT_VALID_LIGHTALPHA_SAMPLE( l0 )

    return
        art_gv->arlightsample_gv->_alf_l_min(
            art_gv,
            l0->light->value
            );
}

void arlightalphasample_ll_min_l(
        const ART_GV           * art_gv,
        const ArLightAlphaSample * l0,
        const ArLightAlphaSample * l1,
              ArLightAlphaSample * lr
        )
{
    ASSERT_VALID_LIGHTALPHA_SAMPLE( l0 )
    ASSERT_VALID_LIGHTALPHA_SAMPLE( l1 )
    ASSERT_ALLOCATED_LIGHTALPHA_SAMPLE( lr )

    art_gv->arlightsample_gv->_alf_ll_min_l(
        art_gv,
        l0->light->value,
        l1->light->value,
        lr->light->value
        );

    lr->alpha = M_MIN( l0->alpha, l1->alpha );

    ASSERT_VALID_LIGHTALPHA_SAMPLE( lr )
}

double arlightalphasample_ll_maxdiff(
        const ART_GV           * art_gv,
        const ArLightAlphaSample * l0,
        const ArLightAlphaSample * l1
        )
{
    ASSERT_VALID_LIGHTALPHA_SAMPLE( l0 )
    ASSERT_VALID_LIGHTALPHA_SAMPLE( l1 )

    return
        art_gv->arlightsample_gv->_alf_ll_maxdiff(
            art_gv,
            l0->light->value,
            l1->light->value
            );
}

void arlightalphasample_i_add_l(
        const ART_GV                * art_gv,
        const ArLightIntensitySample  * i0,
              ArLightAlphaSample      * lr
        )
{
    ASSERT_VALID_LIGHT_INTENSITY_SAMPLE( i0 )
    ASSERT_VALID_LIGHTALPHA_SAMPLE( lr )

    art_gv->arlightsample_gv->_alf_i_add_l(
        art_gv,
        i0,
        lr->light->value
        );

    ASSERT_VALID_LIGHTALPHA_SAMPLE( lr )
}

void arlightalphasample_l_add_l(
        const ART_GV            * art_gv,
        const ArLightAlphaSample  * l0,
              ArLightAlphaSample  * lr
        )
{
    ASSERT_VALID_LIGHTALPHA_SAMPLE( l0 )
    ASSERT_VALID_LIGHTALPHA_SAMPLE( lr )

    art_gv->arlightsample_gv->_alf_l_add_l(
        art_gv,
        l0->light->value,
        lr->light->value
        );

    lr->alpha += l0->alpha;

    ASSERT_VALID_LIGHTALPHA_SAMPLE( lr )
}

void arlightalphasample_dl_sloppy_add_l(
        const ART_GV            * art_gv,
        const double              d0,
        const ArLightAlphaSample  * l0,
              ArLightAlphaSample  * lr
        )
{
    ASSERT_VALID_LIGHTALPHA_SAMPLE( l0 )
    ASSERT_VALID_LIGHTALPHA_SAMPLE( lr )

    art_gv->arlightsample_gv->_alf_dl_sloppy_add_l(
        art_gv,
        d0,
        l0->light->value,
        lr->light->value
        );

    lr->alpha += l0->alpha;

    ASSERT_VALID_LIGHTALPHA_SAMPLE( lr )
}

void arlightalphasample_l_atomic_add_l(
        const ART_GV            * art_gv,
        const ArLightAlphaSample  * l0,
              ArLightAlphaSample  * lr
        )
{
    ASSERT_VALID_LIGHTALPHA_SAMPLE( l0 )
    ASSERT_VALID_LIGHTALPHA_SAMPLE( lr )

    arlightsample_l_atomic_add_l(
        art_gv,
        ARLIGHTALPHASAMPLE_L(*l0),
        ARLIGHTALPHASAMPLE_L(*lr)
        );

    ARLIGHTALPHASAMPLE_A(*lr) =
        m_dd_atomic_add(
              ARLIGHTALPHASAMPLE_A(*l0),
            & ARLIGHTALPHASAMPLE_A(*lr)
            );

    ASSERT_VALID_LIGHTALPHA_SAMPLE( lr )
}

void arlightalphasample_ll_add_l(
        const ART_GV           * art_gv,
        const ArLightAlphaSample * l0,
        const ArLightAlphaSample * l1,
              ArLightAlphaSample * lr
        )
{
    ASSERT_VALID_LIGHTALPHA_SAMPLE( l0 )
    ASSERT_VALID_LIGHTALPHA_SAMPLE( l1 )
    ASSERT_ALLOCATED_LIGHTALPHA_SAMPLE( lr )

    arlightsample_ll_add_l(
        art_gv,
        ARLIGHTALPHASAMPLE_L(*l0),
        ARLIGHTALPHASAMPLE_L(*l1),
        ARLIGHTALPHASAMPLE_L(*lr)
        );

    ARLIGHTALPHASAMPLE_A( *lr ) =
        ARLIGHTALPHASAMPLE_A( *l0 ) + ARLIGHTALPHASAMPLE_A( *l1 );

    ASSERT_VALID_LIGHTALPHA_SAMPLE( lr )
}

void arlightalphasample_lld_sloppy_add_l(
        const ART_GV           * art_gv,
        const ArLightAlphaSample * l0,
        const ArLightAlphaSample * l1,
        const double             d0,
              ArLightAlphaSample * lr
        )
{
    ASSERT_VALID_LIGHTALPHA_SAMPLE( l0 )
    ASSERT_VALID_LIGHTALPHA_SAMPLE( l1 )
    ASSERT_ALLOCATED_LIGHTALPHA_SAMPLE( lr )

    arlightsample_lld_sloppy_add_l(
        art_gv,
        ARLIGHTALPHASAMPLE_L(*l0),
        ARLIGHTALPHASAMPLE_L(*l1),
        d0,
        ARLIGHTALPHASAMPLE_L(*lr)
        );

    ARLIGHTALPHASAMPLE_A( *lr ) =
        ARLIGHTALPHASAMPLE_A( *l0 ) + ARLIGHTALPHASAMPLE_A( *l1 );

    ASSERT_VALID_LIGHTALPHA_SAMPLE( lr )
}

void arlightalphasample_ll_sub_l(
        const ART_GV           * art_gv,
        const ArLightAlphaSample * l0,
        const ArLightAlphaSample * l1,
              ArLightAlphaSample * lr
        )
{
    ASSERT_VALID_LIGHTALPHA_SAMPLE( l0 )
    ASSERT_VALID_LIGHTALPHA_SAMPLE( l1 )
    ASSERT_ALLOCATED_LIGHTALPHA_SAMPLE( lr )

    arlightsample_ll_sub_l(
        art_gv,
        ARLIGHTALPHASAMPLE_L(*l0),
        ARLIGHTALPHASAMPLE_L(*l1),
        ARLIGHTALPHASAMPLE_L(*lr)
        );

    ARLIGHTALPHASAMPLE_A( *lr ) = ARLIGHTALPHASAMPLE_A( *l0 ) - ARLIGHTALPHASAMPLE_A( *l1 );

    ASSERT_VALID_LIGHTALPHA_SAMPLE( lr )
}

void arlightalphasample_d_mul_l(
        const ART_GV           * art_gv,
        const double             d0,
              ArLightAlphaSample * lr
        )
{
    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_VALID_LIGHTALPHA_SAMPLE( lr )

    art_gv->arlightsample_gv->_alf_d_mul_l(
        art_gv,
        d0,
        lr->light->value
        );

    lr->alpha *= d0;

    ASSERT_VALID_LIGHTALPHA_SAMPLE( lr )
}

void arlightalphasample_dl_mul_l(
        const ART_GV           * art_gv,
        const double             d0,
        const ArLightAlphaSample * l0,
              ArLightAlphaSample * lr
        )
{
    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_VALID_LIGHTALPHA_SAMPLE( l0 )
    ASSERT_ALLOCATED_LIGHTALPHA_SAMPLE( lr )

    art_gv->arlightsample_gv->_alf_dl_mul_l(
        art_gv,
        d0,
        l0->light->value,
        lr->light->value
        );

    lr->alpha = d0 * l0->alpha;

    ASSERT_VALID_LIGHTALPHA_SAMPLE( lr )
}

void arlightalphasample_i_mul_l(
        const ART_GV                * art_gv,
        const ArLightIntensitySample  * i0,
              ArLightAlphaSample      * lr
        )
{
    ASSERT_VALID_LIGHT_INTENSITY_SAMPLE( i0 )
    ASSERT_VALID_LIGHTALPHA_SAMPLE( lr )

    art_gv->arlightsample_gv->_alf_i_mul_l(
        art_gv,
        i0,
        lr->light->value
        );

    ASSERT_VALID_LIGHTALPHA_SAMPLE( lr )
}

void arlightalphasample_l_mul_l(
        const ART_GV           * art_gv,
        const ArLightAlphaSample * l0,
              ArLightAlphaSample * lr
        )
{
    ASSERT_VALID_LIGHTALPHA_SAMPLE( l0 )
    ASSERT_VALID_LIGHTALPHA_SAMPLE( lr )

    art_gv->arlightsample_gv->_alf_l_mul_l(
        art_gv,
        l0->light->value,
        lr->light->value
        );

    ASSERT_VALID_LIGHTALPHA_SAMPLE( lr )
}

void arlightalphasample_ll_mul_l(
        const ART_GV           * art_gv,
        const ArLightAlphaSample * l0,
        const ArLightAlphaSample * l1,
              ArLightAlphaSample * lr
        )
{
    ASSERT_VALID_LIGHTALPHA_SAMPLE( l0 )
    ASSERT_VALID_LIGHTALPHA_SAMPLE( l1 )
    ASSERT_ALLOCATED_LIGHTALPHA_SAMPLE( lr )

    art_gv->arlightsample_gv->_alf_ll_mul_l(
        art_gv,
        l0->light->value,
        l1->light->value,
        lr->light->value
        );

    ASSERT_VALID_LIGHTALPHA_SAMPLE( lr )
}

void arlightalphasample_d_div_l(
        const ART_GV           * art_gv,
        const double             d0,
              ArLightAlphaSample * lr
        )
{
    ASSERT_POSITIVE_DOUBLE( d0 )
    ASSERT_VALID_LIGHTALPHA_SAMPLE( lr )

    art_gv->arlightsample_gv->_alf_d_div_l(
        art_gv,
        d0,
        lr->light->value
        );

    ASSERT_VALID_LIGHTALPHA_SAMPLE( lr )
}

void arlightalphasample_dl_div_l(
        const ART_GV           * art_gv,
        const double             d0,
        const ArLightAlphaSample * l0,
              ArLightAlphaSample * lr
        )
{
    ASSERT_POSITIVE_DOUBLE( d0 )
    ASSERT_VALID_LIGHTALPHA_SAMPLE( l0 )
    ASSERT_ALLOCATED_LIGHTALPHA_SAMPLE( lr )

    art_gv->arlightsample_gv->_alf_dl_div_l(
        art_gv,
        d0,
        l0->light->value,
        lr->light->value
        );

    ASSERT_VALID_LIGHTALPHA_SAMPLE( lr )
}

void arlightalphasample_dl_pow_l(
        const ART_GV           * art_gv,
        const double             d0,
        const ArLightAlphaSample * l0,
              ArLightAlphaSample * lr
        )
{
    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_VALID_LIGHTALPHA_SAMPLE( l0 )
    ASSERT_ALLOCATED_LIGHTALPHA_SAMPLE( lr )

    art_gv->arlightsample_gv->_alf_dl_pow_l(
        art_gv,
        d0,
        l0->light->value,
        lr->light->value
        );

    ASSERT_VALID_LIGHTALPHA_SAMPLE( lr )
}

void arlightalphasample_ddl_clamp_l(
        const ART_GV           * art_gv,
        const double             d0,
        const double             d1,
        const ArLightAlphaSample * l0,
              ArLightAlphaSample * lr
        )
{
    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_NONNEGATIVE_DOUBLE( d1 )
    ASSERT_VALID_LIGHTALPHA_SAMPLE( l0 )
    ASSERT_ALLOCATED_LIGHTALPHA_SAMPLE( lr )

    art_gv->arlightsample_gv->_alf_ddl_clamp_l(
        art_gv,
        d0,
        d1,
        l0->light->value,
        lr->light->value
        );

    ASSERT_VALID_LIGHTALPHA_SAMPLE( lr )
}

void arlightalphasample_dl_mul_add_l(
        const ART_GV           * art_gv,
        const double             d0,
        const ArLightAlphaSample * l0,
              ArLightAlphaSample * lr
        )
{
    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_VALID_LIGHTALPHA_SAMPLE( l0 )
    ASSERT_ALLOCATED_LIGHTALPHA_SAMPLE( lr )

    art_gv->arlightsample_gv->_alf_dl_mul_add_l(
        art_gv,
        d0,
        l0->light->value,
        lr->light->value
        );

    ARLIGHTALPHASAMPLE_A(*lr) += d0 * ARLIGHTALPHASAMPLE_A(*l0);

    ASSERT_VALID_LIGHTALPHA_SAMPLE( lr )
}

void arlightalphasample_dld_mul_sloppy_add_l(
        const ART_GV           * art_gv,
        const double             d0,
        const ArLightAlphaSample * l0,
        const double             d1,
              ArLightAlphaSample * lr
        )
{
    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_VALID_LIGHTALPHA_SAMPLE( l0 )
    ASSERT_ALLOCATED_LIGHTALPHA_SAMPLE( lr )

    art_gv->arlightsample_gv->_alf_dld_mul_sloppy_add_l(
        art_gv,
        d0,
        l0->light->value,
        d1,
        lr->light->value
        );

    ARLIGHTALPHASAMPLE_A(*lr) += d0 * ARLIGHTALPHASAMPLE_A(*l0);

    ASSERT_VALID_LIGHTALPHA_SAMPLE( lr )
}

void arlightalphasample_dll_interpol_l(
        const ART_GV           * art_gv,
        const double             d0,
        const ArLightAlphaSample * l0,
        const ArLightAlphaSample * l1,
              ArLightAlphaSample * lr
        )
{
    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_VALID_LIGHTALPHA_SAMPLE( l0 )
    ASSERT_VALID_LIGHTALPHA_SAMPLE( l1 )
    ASSERT_ALLOCATED_LIGHTALPHA_SAMPLE( lr )

    art_gv->arlightsample_gv->_alf_dll_interpol_l(
        art_gv,
        d0,
        l0->light->value,
        l1->light->value,
        lr->light->value
        );

    ASSERT_VALID_LIGHTALPHA_SAMPLE( lr )
}

unsigned int arlightalphasample_l_valid(
        const ART_GV           * art_gv,
        const ArLightAlphaSample * l0
        )
{
    if ( ARLIGHTALPHASAMPLE_A( *l0 ) < 0.0 )
    {
        ART_ERRORHANDLING_WARNING(
            "light alpha is negative (%f)",
            ARLIGHTALPHASAMPLE_A( *l0 )
            );
        return 0;
    }

    return
        art_gv->arlightsample_gv->_alf_l_valid(
            art_gv,
            l0->light->value
            );
}

void arlightalphasample_l_debugprintf(
        const ART_GV           * art_gv,
        const ArLightAlphaSample * l0
        )
{
    printf("ArLightAlphaSample: ");

    art_gv->arlightsample_gv->_alf_l_debugprintf(
        art_gv,
        l0->light->value
        );

    printf("ArLightAlphaSample: Alpha: %7.6f\n",l0->alpha);
}

void sps_to_arlightalphasample(
        const ART_GV            * art_gv,
        const ArSpectralSample    * c0,
              ArLightAlphaSample  * lr
        )
{
    art_gv->arlightsample_gv->_alf_s_init_unpolarised_l(
        art_gv,
        c0,
        lr->light->value
        );

    lr->alpha = 1.0;
}

void arlightalphasample_to_ss(
        const ART_GV            * art_gv,
        const ArLightAlphaSample  * l0,
              ArSpectralSample    * cr
        )
{
    art_gv->arlightsample_gv->_alf_l_init_h(
        art_gv,
        l0->light->value,
        cr
        );
}

void arlightalphasample_to_arlightalphasample(
        const ART_GV            * art_gv,
        const ArLightAlphaSample  * l0,
              ArLightAlphaSample  * lr
        )
{
    art_gv->arlightsample_gv->_alf_l_init_l(
        art_gv,
        l0->light->value,
        lr->light->value
        );

    lr->alpha = l0->alpha;
}


// ===========================================================================
