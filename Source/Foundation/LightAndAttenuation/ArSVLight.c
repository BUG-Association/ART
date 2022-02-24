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

#define ART_MODULE_NAME     ArSVLight

#include "ArSVLight.h"
#include "ArSVLightSample.h"
#include "_ArLight_GV.h"
#include <stdlib.h>
#include "LightImplementationMacros.h"
#include "ArLightAndAttenuationSubsystemManagement.h"
#include "FoundationAssertionMacros.h"

typedef struct ArSVLight_GV
{
    ArSVLight  * light_none;
    ArSVLight  * light_unit;
}
ArSVLight_GV;

ART_MODULE_INITIALISATION_FUNCTION
(
    ArSVLight_GV  * arsvlight_gv =
        ALLOC(ArSVLight_GV);

    arsvlight_gv->light_none = 0;
    arsvlight_gv->light_unit = 0;

    art_gv->arsvlight_gv = arsvlight_gv;
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    if ( art_gv->arsvlight_gv->light_none )
        arsvlight_free( art_gv, art_gv->arsvlight_gv->light_none );
    if ( art_gv->arsvlight_gv->light_unit )
        arsvlight_free( art_gv, art_gv->arsvlight_gv->light_unit );

    FREE( art_gv->arsvlight_gv );
)

ART_LIGHT_AND_ATTENUATION_MODULE_INITIALISATION_FUNCTION
(
    art_gv->arsvlight_gv->light_none =
        arsvlight_d_alloc_init_unpolarised(
            art_gv,
            0.0
            );

    art_gv->arsvlight_gv->light_unit =
        arsvlight_d_alloc_init_unpolarised(
            art_gv,
            1.0
            );
)

ART_LIGHT_AND_ATTENUATION_MODULE_SHUTDOWN_FUNCTION
(
    if ( art_gv->arsvlight_gv->light_none )
        arsvlight_free( art_gv, art_gv->arsvlight_gv->light_none );
    if ( art_gv->arsvlight_gv->light_unit )
        arsvlight_free( art_gv, art_gv->arsvlight_gv->light_unit );
)

SWITCHING_IMPLEMENTATION_FOR_LCT( arsvlight )


/* ---------------------------------------------------------------------------

    Debug printf messages for this module
    =====================================

    In order to keep the source as comprehensible as possible all internal
    debug printing / trace functionality has been moved to a separate file.
    By default the following #include imports a file which does exactly
    nothing except #define empty versions of all the debug macros used in
    the code. If you want to enable any of the debug printf messages you
    should read the comments in that file and edit the definitions found
    there to suit your purpose.

------------------------------------------------------------------------aw- */

#include "ArSVLightDebugOutput.c"


#define  YES  1
#define  NO   0


void arsvlight_li_sv(
        const ART_GV          * art_gv,
        const ArSVLight       * l0,
        const int               i0,
              ArStokesVectorSample  * sr
        )
{
    ARSVS_I( *sr, 0 ) = SPS(spc_si( art_gv, ARSVLIGHT_SV_I( *l0, 0 ), i0 ));
    
    if ( ARSVLIGHT_POLARISED( *l0 ) )
    {
        ARSVS_I( *sr, 1 ) = SPS(spc_si( art_gv, ARSVLIGHT_SV_I( *l0, 1 ), i0 ));
        ARSVS_I( *sr, 2 ) = SPS(spc_si( art_gv, ARSVLIGHT_SV_I( *l0, 2 ), i0 ));
        ARSVS_I( *sr, 3 ) = SPS(spc_si( art_gv, ARSVLIGHT_SV_I( *l0, 3 ), i0 ));
    }
    else
    {
        ARSVS_I( *sr, 1 ) = SPS(0.0);
        ARSVS_I( *sr, 2 ) = SPS(0.0);
        ARSVS_I( *sr, 3 ) = SPS(0.0);
    }
}

void arsvlight_set_li_sv(
        const ART_GV                * art_gv,
              ArSVLight             * l0,
        const int                     i0,
        const ArStokesVectorSample  * sr
        )
{
    ASSERT_VALID_SV_LIGHT(l0);
    ASSERT_VALID_STOKES_VECTOR_SAMPLE(sr);
    ASSERT_INTEGER_WITHIN_ISR_CHANNEL_RANGE(i0);
    
    spc_set_sid( art_gv, ARSVLIGHT_SV_I( *l0, 0 ), i0, SPS_CI(ARSVS_I( *sr, 0 ),0) );
    spc_set_sid( art_gv, ARSVLIGHT_SV_I( *l0, 1 ), i0, SPS_CI(ARSVS_I( *sr, 1 ),0) );
    spc_set_sid( art_gv, ARSVLIGHT_SV_I( *l0, 2 ), i0, SPS_CI(ARSVS_I( *sr, 2 ),0) );
    spc_set_sid( art_gv, ARSVLIGHT_SV_I( *l0, 3 ), i0, SPS_CI(ARSVS_I( *sr, 3 ),0) );

    ASSERT_VALID_SV_LIGHT(l0);
}

const char * arsvlight_lct_string(
        const ART_GV  * art_gv
        )
{
    (void) art_gv;
    
    return "Stokes vector light";
}

ArSVLight * arsvlight_alloc(
        const ART_GV  * art_gv
        )
{
    ArSVLight  * newSVLight = ALLOC( ArSVLight );

    ARSVLIGHT_SV( *newSVLight ) =
        arstokesvector_alloc( art_gv );

    return newSVLight;
}

void arsvlight_free(
        const ART_GV     * art_gv,
              ArSVLight  * lr
        )
{
    arstokesvector_free(
        art_gv,
        ARSVLIGHT_SV( *lr )
        );

    FREE(lr);
}

ArSVLight * arsvlight_d_alloc_init_unpolarised(
        const ART_GV  * art_gv,
        const double    d0
        )
{
    ArSVLight  * lr;

    lr = arsvlight_alloc( art_gv );

    arsvlight_d_init_unpolarised_l(
        art_gv,
        d0,
        lr
        );

    return lr;
}

ArSVLight * arsvlight_l_alloc_init(
        const ART_GV     * art_gv,
        const ArSVLight  * l0
        )
{
    ArSVLight  * lr;

    lr = arsvlight_alloc( art_gv );

    arsvlight_l_init_l(
        art_gv,
        l0,
        lr
        );

    return lr;
}

ArSVLight const * arsvlight_unpolarised_unit(
        const ART_GV  * art_gv
        )
{
    return art_gv->arsvlight_gv->light_unit;
}

ArSVLight const * arsvlight_none(
        const ART_GV  * art_gv
        )
{
    return art_gv->arsvlight_gv->light_none;
}

void arsvlight_d_init_unpolarised_l(
        const ART_GV     * art_gv,
        const double       d0,
              ArSVLight  * lr
        )
{
    spc_d_init_s(
        art_gv,
        d0,
        ARSVLIGHT_INTENSITY( *lr )
        );

    for ( unsigned int i = 1; i < 4; i++ )
        spc_d_init_s(
            art_gv,
            0.0,
            ARSVLIGHT_SV_I( *lr, i )
            );

    ARSVLIGHT_POLARISED( *lr ) = NO;
}

void arsvlight_s_init_unpolarised_l(
        const ART_GV     * art_gv,
        const ArSpectrum   * c0,
              ArSVLight  * lr
        )
{
    spc_s_init_s(
        art_gv,
        c0,
        ARSVLIGHT_INTENSITY( *lr )
        );

    for ( unsigned int i = 1; i < 4; i++ )
        spc_d_init_s(
            art_gv,
            0.0,
            ARSVLIGHT_SV_I( *lr, i )
            );

    ARSVLIGHT_POLARISED( *lr ) = NO;
}

void arsvlight_s_rf_init_polarised_l(
        const ART_GV            * art_gv,
        const ArStokesVector    * s0,
        const ArReferenceFrame  * r0,
              ArSVLight         * lr
        )
{
    for ( unsigned int i = 0; i < 4; i++ )
        spc_s_init_s(
            art_gv,
            ARSV_I( *s0, i ),
            ARSVLIGHT_SV_I( *lr, i )
            );

    ARSVLIGHT_REFFRAME( *lr ) = *r0;

    ARSVLIGHT_POLARISED( *lr ) = YES;
}

void arsvlight_l_init_s(
        const ART_GV     * art_gv,
        const ArSVLight  * l0,
              ArSpectrum   * cr
        )
{
    spc_s_init_s(
        art_gv,
        ARSVLIGHT_SV_I( *l0, 0 ),
        cr
        );
}

void  arsvlight_l_init_i(
        const ART_GV            * art_gv,
        const ArSVLight         * l0,
              ArLightIntensity  * ir
        )
{
    spc_s_init_s(
        art_gv,
        ARSVLIGHT_SV_I( *l0, 0 ),
        ir
        );
}

void  arsvlight_l_init_l(
        const ART_GV     * art_gv,
        const ArSVLight  * l0,
              ArSVLight  * lr
        )
{
    for ( unsigned int i = 0; i < 4; i++ )
        spc_s_init_s(
            art_gv,
            ARSVLIGHT_SV_I( *l0, i ),
            ARSVLIGHT_SV_I( *lr, i )
            );

    ARSVLIGHT_REFFRAME( *lr ) = ARSVLIGHT_REFFRAME( *l0 );

    ARSVLIGHT_POLARISED( *lr ) = ARSVLIGHT_POLARISED( *l0 );
}

unsigned int  arsvlight_l_polarised(
        const ART_GV     * art_gv,
        const ArSVLight  * l0
        )
{
    (void) art_gv;
    
    return ARSVLIGHT_POLARISED( *l0 );
}

ArReferenceFrame const * arsvlight_l_refframe(
        const ART_GV     * art_gv,
        const ArSVLight  * l0
        )
{
    (void) art_gv;
    
    if ( ARSVLIGHT_POLARISED( *l0 ) )
        return & ARSVLIGHT_REFFRAME( *l0 );
    else
        return 0;
}

void arsvlight_l_to_sv(
        const ART_GV          * art_gv,
        const ArSVLight       * l0,
              ArStokesVector  * sr
        )
{
    if ( ARSVLIGHT_POLARISED( *l0 ) )
        arstokesvector_sv_init_sv(
            art_gv,
            ARSVLIGHT_SV( *l0 ),
            sr
            );
    else
        arstokesvector_s_init_unpolarised_sv(
            art_gv,
            ARSVLIGHT_SV_I( *l0, 0 ),
            sr
            );
}

/* ---------------------------------------------------------------------------

    'arsvlight_ld_realign_to_coaxial_refframe_sv'

    This function has a limited but highly important function: it modifies
    the Stokes Vector of a light ray to fit a different coaxial reference
    coordinate system, i.e. one that is just rotated from the original along
    the axis of propagation. It leaves the original data intact, and returns
    the modified Stokes Vector as a separate result. If the input light value
    is not polarised, the function generates an "empty" Stokes Vector.

    IMPORTANT NOTE:
    ---------------

    This procedure returns complete garbage if it is used to compute the
    transformation between non-coaxial reference systems. Which is the way
    it should be, since that kind of transformation does not make sense from
    a physical standpoint. In normal operation, the procedure does not
    perform any sanity checks in this direction for performance reasons,
    so be careful when using it.

------------------------------------------------------------------------aw- */

void arsvlight_ld_realign_to_coaxial_refframe_sv(
        const ART_GV            * art_gv,
        const ArSVLight         * l0,
        const double              d0,
        const ArReferenceFrame  * r0,
              ArStokesVector    * sr
        )
{
    ASSERT_VALID_SV_LIGHT(l0)
    ASSERT_VALID_REFERENCE_FRAME(r0)
    ASSERT_ALLOCATED_STOKES_VECTOR(sr)
    ASSERT_COAXIAL_REFERENCE_FRAMES_R_SVL( r0, l0, d0 )
    (void) d0;  // d0 is only used if assertions are on
    
    if ( ARSVLIGHT_POLARISED( *l0 ) )
    {
        double  cos_phi =
            vec3d_vv_dot(
                & ARSVLIGHT_REFFRAME_I( *l0, 0 ),
                & ARREFFRAME_RF_I( *r0, 0 )
                );

        double  sin_phi =
            vec3d_vv_dot(
                & ARSVLIGHT_REFFRAME_I( *l0, 1 ),
                & ARREFFRAME_RF_I( *r0, 0 )
                );

        double  cos_2phi = M_SQR( cos_phi ) - M_SQR( sin_phi );
        double  sin_2phi = 2 * cos_phi * sin_phi;

        spc_s_init_s(
            art_gv,
            ARSVLIGHT_SV_I( *l0, 0 ),
            ARSV_I( *sr, 0 )
            );

        spc_s_init_s(
            art_gv,
            ARSVLIGHT_SV_I( *l0, 3 ),
            ARSV_I( *sr, 3 )
            );

        spc_ds_mul_ds_mul_add_s(
            art_gv,
            cos_2phi,
            ARSVLIGHT_SV_I( *l0, 1 ),
            sin_2phi,
            ARSVLIGHT_SV_I( *l0, 2 ),
            ARSV_I( *sr, 1 )
            );

        spc_ds_mul_ds_mul_add_s(
            art_gv,
           -sin_2phi,
            ARSVLIGHT_SV_I( *l0, 1 ),
            cos_2phi,
            ARSVLIGHT_SV_I( *l0, 2 ),
            ARSV_I( *sr, 2 )
            );
    }
    else
    {
        spc_s_init_s(
            art_gv,
            ARSVLIGHT_SV_I( *l0, 0 ),
            ARSV_I( *sr, 0 )
            );

        for ( unsigned int i = 1; i < 4; i++ )
            spc_d_init_s(
                art_gv,
                0.0,
                ARSV_I( *sr, i )
                );
    }

    ASSERT_VALID_SV_LIGHT(l0)
    ASSERT_VALID_STOKES_VECTOR(sr)
}

/* ---------------------------------------------------------------------------

    'arsvlight_realign_to_coaxial_refframe_l'

    This procedure modifies a ray of polarised light to fit a different
    coaxial reference coordinate system. It is basically a wrapper around
    'arsvlight_ld_realign_to_coaxial_refframe_sv' - see the description of this
    function for more information and the restrictions on its usage.

------------------------------------------------------------------------aw- */

void arsvlight_realign_to_coaxial_refframe_l(
        const ART_GV            * art_gv,
        const ArReferenceFrame  * r0,
              ArSVLight         * lr
        )
{
    if ( ARSVLIGHT_POLARISED( *lr ) )
    {
        ArStokesVector * newSV =
            arstokesvector_alloc( art_gv );

        arsvlight_ld_realign_to_coaxial_refframe_sv(
            art_gv,
            lr,
            3.0 DEGREES,
            r0,
            newSV
            );

        arstokesvector_free(
            art_gv,
            ARSVLIGHT_SV(*lr)
            );

        ARSVLIGHT_REFFRAME(*lr) = *r0;
        ARSVLIGHT_SV(*lr) = newSV;
    }
}

void arsvlight_l_realign_to_coaxial_refframe_l(
        const ART_GV            * art_gv,
        const ArSVLight         * l0,
        const ArReferenceFrame  * r0,
              ArSVLight         * lr
        )
{
    if ( ARSVLIGHT_POLARISED( *l0 ) )
    {
        ArStokesVector * newSV =
            arstokesvector_alloc( art_gv );

        arsvlight_ld_realign_to_coaxial_refframe_sv(
            art_gv,
            l0,
            3.0 DEGREES,
            r0,
            newSV
            );

        arstokesvector_free(
            art_gv,
            ARSVLIGHT_SV(*lr)
            );

        ARSVLIGHT_REFFRAME(*lr) = *r0;
        ARSVLIGHT_SV(*lr) = newSV;
        ARSVLIGHT_POLARISED(*lr) = YES;
    }
    else
        arsvlight_l_init_l(
            art_gv,
            l0,
            lr
            );
}

unsigned int arsvlight_ll_equal(
        const ART_GV     * art_gv,
        const ArSVLight  * l0,
        const ArSVLight  * l1
        )
{
    int  p0 = ARSVLIGHT_POLARISED( *l0 );
    int  p1 = ARSVLIGHT_POLARISED( *l1 );

    if ( p0 != p1 )
        return 0;

    if ( p0 )
    {
        if ( ! arrefframe_rf_rf_equal(
                     art_gv,
                   & ARSVLIGHT_REFFRAME( *l0 ),
                   & ARSVLIGHT_REFFRAME( *l1 )
                   ) )
            return 0;

        if ( ! arstokesvector_sv_sv_equal(
                     art_gv,
                     ARSVLIGHT_SV( *l0 ),
                     ARSVLIGHT_SV( *l1 )
                   ) )
            return 0;
    }
    else
    {
        if ( ! spc_ss_equal(
                     art_gv,
                     ARSVLIGHT_SV_I( *l0, 0 ),
                     ARSVLIGHT_SV_I( *l1, 0 )
                   ) )
            return 0;
    }

    return 1;
}

unsigned int arsvlight_lld_equal(
        const ART_GV     * art_gv,
        const ArSVLight  * l0,
        const ArSVLight  * l1,
        const double       max_relative_error
        )
{
    int  p0 = ARSVLIGHT_POLARISED( *l0 );
    int  p1 = ARSVLIGHT_POLARISED( *l1 );

    if ( p0 != p1 )
        return 0;

    if ( p0 )
    {
        if ( ! arrefframe_rf_rf_d_equal(
                     art_gv,
                   & ARSVLIGHT_REFFRAME( *l0 ),
                   & ARSVLIGHT_REFFRAME( *l1 ),
                     max_relative_error
                   ) )
            return 0;

        if ( ! arstokesvector_sv_sv_d_equal(
                     art_gv,
                     ARSVLIGHT_SV( *l0 ),
                     ARSVLIGHT_SV( *l1 ),
                     max_relative_error
                   ) )
            return 0;
    }
    else
    {
        if ( ! spc_ssd_equal(
                     art_gv,
                     ARSVLIGHT_SV_I( *l0, 0 ),
                     ARSVLIGHT_SV_I( *l1, 0 ),
                     max_relative_error
                   ) )
            return 0;
    }

    return 1;
}

/* ---------------------------------------------------------------------------

    'arsvlight_l_norm', 'arsvlight_l_norm_visible_range'

        Computes the L1 norm of the light in question. This only affects the
        first component of the light regardless of polarisation state.


------------------------------------------------------------------------aw- */

double arsvlight_l_norm(
        const ART_GV     * art_gv,
        const ArSVLight  * l0
        )
{
    return
        spc_s_l1_norm(
            art_gv,
            ARSVLIGHT_INTENSITY( *l0 )
            );
}

double arsvlight_l_norm_visible_range(
        const ART_GV     * art_gv,
        const ArSVLight  * l0
        )
{
    return
        spc_s_l1_norm_visible_range(
            art_gv,
            ARSVLIGHT_INTENSITY( *l0 )
            );
}

/* ---------------------------------------------------------------------------

    'arsvlight_ll_align_and_return_ss'
    'arsvlight_lll_align_and_return_ss'

    These at first glance rather odd functions fulfil a similar purpose.
    The only difference between them is that the first caters for the two-
    operand case, while the second is intended for three operands. Both
    require that at least one of the input light values is polarised.

    They are only intended to be used inside a function which manipulates
    light as input and output values (e.g. arsvlight_l_add_l for adding
    radiances). Tasks which are common to all of these - and which are
    performed by these two functions - are:

    - to bring both of the light structs to the same reference system by
      rotating one of them. Note that as always this operation only makes
      sense if the two are collinear light rays; see the reference for
      'arsvlight_ld_realign_to_coaxial_refframe_sv' for details

    - if necessary, assign this new reference system to the result

    - if necessary, set the polarisation flag for the result

    - and to return the Stokes Vectors for the aligned light values

    Returning the plain Stokes Vectors is necessary since most of the
    calling functions perform their manipulations on these directly.

------------------------------------------------------------------------aw- */


void arsvlight_lld_align_and_return_ss(
        const ART_GV          * art_gv,
        const ArSVLight       * l0,
              ArSVLight       * lr,
        const double            d0,
              ArStokesVector  * svr0,
              ArStokesVector  * svr1
        )
{
    ASSERT_VALID_SV_LIGHT(l0)
    ASSERT_VALID_SV_LIGHT(lr)

    /* -----------------------------------------------------------------------
        Two-operand version. Assumption: at least one of the two operands
        is already polarised, or this function would not have been called.
    --------------------------------------------------------------------aw- */

    if ( ! ARSVLIGHT_POLARISED( *lr ) )
    {
        /* ------------------------------------------------------------------
            Case 1: the result is not polarised yet, so we just copy the
                    reference system from the first operand and use
                    'arsvlight_ld_realign_to_coaxial_refframe_sv' to extract
                    the "empty" Stokes components for the result.
        ---------------------------------------------------------------aw- */

        if ( ARSVLIGHT_POLARISED( *l0 ) )
        {
            arstokesvector_sv_init_sv(
                  art_gv,
                  ARSVLIGHT_SV( *l0 ),
                  svr0
                );

            ASSERT_VALID_REFERENCE_FRAME( & ARSVLIGHT_REFFRAME( *l0 ) )

            ASSERT_VALID_SV_LIGHT(lr)

            arsvlight_ld_realign_to_coaxial_refframe_sv(
                  art_gv,
                  lr,
                  d0,
                & ARSVLIGHT_REFFRAME( *l0 ),
                  svr1
                );

            ARSVLIGHT_REFFRAME( *lr ) = ARSVLIGHT_REFFRAME( *l0 );
            ARSVLIGHT_POLARISED( *lr ) = YES;

            ASSERT_VALID_SV_LIGHT(lr)
            ASSERT_VALID_STOKES_VECTOR(svr0)
            ASSERT_VALID_STOKES_VECTOR(svr1)
        }
        else
        {
            arstokesvector_sv_init_sv(
                art_gv,
                ARSVLIGHT_SV( *l0 ),
                svr0
                );

            arstokesvector_sv_init_sv(
                art_gv,
                ARSVLIGHT_SV( *lr ),
                svr1
                );

            ARSVLIGHT_POLARISED( *lr ) = NO;

            ASSERT_VALID_SV_LIGHT(lr)
            ASSERT_VALID_STOKES_VECTOR(svr0)
            ASSERT_VALID_STOKES_VECTOR(svr1)
        }
    }
    else
    {
        /* ------------------------------------------------------------------
            Case 2: the result is already polarised, so we align the first
                    operand with the reference system of the result.
        ---------------------------------------------------------------aw- */

        arsvlight_ld_realign_to_coaxial_refframe_sv(
              art_gv,
              l0,
              d0,
            & ARSVLIGHT_REFFRAME( *lr ),
              svr0
            );

        arstokesvector_sv_init_sv(
            art_gv,
            ARSVLIGHT_SV( *lr ),
            svr1
            );

        ASSERT_VALID_SV_LIGHT(lr)
        ASSERT_VALID_STOKES_VECTOR(svr0)
        ASSERT_VALID_STOKES_VECTOR(svr1)
    }
}

void arsvlight_lldl_align_and_return_ss(
        const ART_GV          * art_gv,
        const ArSVLight       * l0,
        const ArSVLight       * l1,
        const double            d0,
              ArSVLight       * lr,
              ArStokesVector  * svr0,
              ArStokesVector  * svr1
        )
{
    ASSERT_VALID_SV_LIGHT(l0)
    ASSERT_VALID_SV_LIGHT(l1)

    ASSERT_ALLOCATED_SV_LIGHT(lr)

    /* -----------------------------------------------------------------------
        Three-operand version. Assumption: at least one of the two input
        operands 'l0' or 'l1' is already polarised, or this
        function would not have been called.
    --------------------------------------------------------------------aw- */

    if ( ! ARSVLIGHT_POLARISED( *l1 ) )
    {
        /* ------------------------------------------------------------------
            Case 1: 'l0' is the only polarised one, so we rotate the
                    other one to its reference system and also assign its
                    reference system to the result.
        ---------------------------------------------------------------aw- */

        arstokesvector_sv_init_sv(
            art_gv,
            ARSVLIGHT_SV( *l0 ),
            svr0
            );

        arsvlight_ld_realign_to_coaxial_refframe_sv(
            art_gv,
              l1,
              d0,
            & ARSVLIGHT_REFFRAME( *l0 ),
              svr1
            );

        ARSVLIGHT_REFFRAME( *lr ) = ARSVLIGHT_REFFRAME( *l0 );
    }
    else
    {
        /* ------------------------------------------------------------------
            Case 2: 'l1' and possibly also 'l0' are polarised, so
                    we use 'l1' as the reference and assign its
                    reference system to the result.
        ---------------------------------------------------------------aw- */

        arsvlight_ld_realign_to_coaxial_refframe_sv(
              art_gv,
              l0,
              d0,
            & ARSVLIGHT_REFFRAME( *l1 ),
              svr0
            );

        arstokesvector_sv_init_sv(
            art_gv,
            ARSVLIGHT_SV( *l1 ),
            svr1
            );

        ARSVLIGHT_REFFRAME( *lr ) = ARSVLIGHT_REFFRAME( *l1 );
    }

    ARSVLIGHT_POLARISED( *lr ) = YES;

    ASSERT_VALID_SV_LIGHT(lr)
    ASSERT_VALID_STOKES_VECTOR(svr0)
    ASSERT_VALID_STOKES_VECTOR(svr1)
}

void arsvlight_l_inv_l(
        const ART_GV     * art_gv,
        const ArSVLight  * l0,
              ArSVLight  * lr
        )
{
    (void) art_gv;
    (void) l0;
    (void) lr;
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

/* ---------------------------------------------------------------------------

    'arsvlight_l_min'
    'arsvlight_l_max'

    Computes the minimum and maximum for a given light struct. Since the
    meaning of "min" and "max" in a rendering context is based on absolute
    radiometric intensity, only the first of the Stokes components is used
    for this computation.

------------------------------------------------------------------------aw- */

double arsvlight_l_max(
        const ART_GV     * art_gv,
        const ArSVLight  * l0
        )
{
    return
        spc_s_max(
            art_gv,
            ARSVLIGHT_INTENSITY( *l0 )
            );
}

double arsvlight_l_min(
        const ART_GV     * art_gv,
        const ArSVLight  * l0
        )
{
    return
        spc_s_min(
            art_gv,
            ARSVLIGHT_INTENSITY( *l0 )
            );
}

/* ---------------------------------------------------------------------------

    'arsvlight_ll_max_l'
    'arsvlight_ll_min_l'

------------------------------------------------------------------------aw- */

void arsvlight_ll_max_l(
        const ART_GV     * art_gv,
        const ArSVLight  * l0,
        const ArSVLight  * l1,
              ArSVLight  * lr
        )
{
    spc_ss_max_s(
        art_gv,
        ARSVLIGHT_SV_I( * l0, 0 ),
        ARSVLIGHT_SV_I( * l1, 0 ),
        ARSVLIGHT_SV_I( * lr, 0 )
        );

    ARSVLIGHT_POLARISED( *lr ) = NO;
}

void arsvlight_ll_min_l(
        const ART_GV     * art_gv,
        const ArSVLight  * l0,
        const ArSVLight  * l1,
              ArSVLight  * lr
        )
{
    spc_ss_min_s(
        art_gv,
        ARSVLIGHT_SV_I( * l0, 0 ),
        ARSVLIGHT_SV_I( * l1, 0 ),
        ARSVLIGHT_SV_I( * lr, 0 )
        );

    ARSVLIGHT_POLARISED( *lr ) = NO;
}

double arsvlight_ll_maxdiff(
        const ART_GV     * art_gv,
        const ArSVLight  * l0,
        const ArSVLight  * l1
        )
{
    return
        spc_ss_maxdiff(
            art_gv,
            ARSVLIGHT_SV_I( * l0, 0 ),
            ARSVLIGHT_SV_I( * l1, 0 )
            );
}

/* ---------------------------------------------------------------------------

    'arsvlight_i_add_l'

    Adds an unpolarised radiant intensity to a - possibly polarised - light
    ray. This only affects the first component.

------------------------------------------------------------------------aw- */

void arsvlight_i_add_l(
        const ART_GV            * art_gv,
        const ArLightIntensity  * i0,
              ArSVLight         * lr
        )
{
    spc_s_add_s(
        art_gv,
        i0,
        ARSVLIGHT_SV_I( *lr, 0 )
        );
}

/* ---------------------------------------------------------------------------

    'arsvlight_l_add_l'
    'arsvlight_ll_add_l'
    'arsvlight_ll_sub_l'

    Adds or subtracts the radiant intensities and polarisation states of two
    collinear light rays. The first Stokes components can be normally combined,
    but the other three have to be aligned to a common reference system before
    they can be added or subtracted. See the reference for
    'arsvlight_ll_align_and_return_ss' for details of this alignment operation.

------------------------------------------------------------------------aw- */

void arsvlight_l_add_l(
        const ART_GV     * art_gv,
        const ArSVLight  * l0,
              ArSVLight  * lr
        )
{
    spc_s_add_s(
        art_gv,
        ARSVLIGHT_SV_I( *l0, 0 ),
        ARSVLIGHT_SV_I( *lr, 0 )
        );

    if ( ARSVLIGHT_POLARISED( *l0 ) || ARSVLIGHT_POLARISED( *lr ) )
    {
        ArStokesVector  * s0 = arstokesvector_alloc( art_gv );
        ArStokesVector  * s1 = arstokesvector_alloc( art_gv );

        arsvlight_lld_align_and_return_ss(
              art_gv,
              l0,
              lr,
              3.0 DEGREES,
              s0,
              s1
            );

        for ( unsigned int i = 1; i < 4; i++ )
            spc_ss_add_s(
                art_gv,
                ARSV_I( *s0, i ),
                ARSV_I( *s1, i ),
                ARSVLIGHT_SV_I( *lr, i )
                );

        ARSVLIGHT_POLARISED(*lr) = YES;

        arstokesvector_free( art_gv, s0 );
        arstokesvector_free( art_gv, s1 );
    }
}

void arsvlight_dl_sloppy_add_l(
        const ART_GV     * art_gv,
        const double       d0,
        const ArSVLight  * l0,
              ArSVLight  * lr
        )
{
    spc_s_add_s(
        art_gv,
        ARSVLIGHT_SV_I( *l0, 0 ),
        ARSVLIGHT_SV_I( *lr, 0 )
        );

    if ( ARSVLIGHT_POLARISED( *l0 ) || ARSVLIGHT_POLARISED( *lr ) )
    {
        ArStokesVector  * s0 = arstokesvector_alloc( art_gv );
        ArStokesVector  * s1 = arstokesvector_alloc( art_gv );

        arsvlight_lld_align_and_return_ss(
              art_gv,
              l0,
              lr,
              d0,
              s0,
              s1
            );

        for ( unsigned int i = 1; i < 4; i++ )
            spc_ss_add_s(
                art_gv,
                ARSV_I( *s0, i ),
                ARSV_I( *s1, i ),
                ARSVLIGHT_SV_I( *lr, i )
                );

        ARSVLIGHT_POLARISED(*lr) = YES;

        arstokesvector_free( art_gv, s0 );
        arstokesvector_free( art_gv, s1 );
    }
}

void arsvlight_ws_add_l(
        const ART_GV                         * art_gv,
        const ArSVLightSample                * s0,
        const ArWavelength                   * w0,
        const ArSpectralSampleSplattingData  * sd,
              ArSVLight                      * lr
        )
{
    ASSERT_VALID_WAVELENGTH( w0 )
    ASSERT_VALID_SV_LIGHT_SAMPLE( s0 )
    ASSERT_VALID_SV_LIGHT( lr )

    ArSVLightSample  t0 = *s0;

    if ( ARSVLIGHTSAMPLE_POLARISED( *s0 ) )
    {
        if ( ARSVLIGHT_POLARISED(*lr) )
        {
            arsvlightsample_realign_to_coaxial_refframe_l(
                  art_gv,
                & ARSVLIGHT_REFFRAME(*lr),
                & t0
                );
        }
        else
        {
            ARSVLIGHT_POLARISED(*lr) = YES;
            ARSVLIGHT_REFFRAME(*lr) = ARSVLIGHTSAMPLE_REFFRAME(*s0);
        }
    }

    for ( unsigned int i = 0; i < HERO_SAMPLES_TO_SPLAT; i++ )
    {
        ASSERT_DOUBLE_LARGER_THAN(
            NANO_FROM_UNIT(ARWL_WI(*w0,i)),
            sd->s500_range_start
            )
        
        ASSERT_DOUBLE_LESS_THAN(
            NANO_FROM_UNIT(ARWL_WI(*w0,i)),
            sd->s500_range_end
            )

        unsigned int  s500_index =
            (unsigned int)(   NANO_FROM_UNIT(ARWL_WI(*w0,i))
                            - sd->s500_range_start );

        if ( art_foundation_isr(art_gv) == ardt_xyz )
        {
            for ( int j = 0; j < 3; j++ )
            {
                ArStokesVectorSample  prior_value;
                
                arsvlight_li_sv(
                      art_gv,
                      lr,
                      j,
                    & prior_value
                    );

                double  cmf_value =
                    s500_si(
                          art_gv,
                        & ARCMF_CURVE_500( *DEFAULT_CMF, j ),
                          s500_index
                        );
                
                ArStokesVectorSample  sample;

                arsvlightsample_li_sv(
                      art_gv,
                    & t0,
                      i,
                    & sample
                    );

                arstokesvectorsample_d_mul_sv(
                      art_gv,
                      cmf_value,
                    & sample
                    );

                arstokesvectorsample_sv_add_sv(
                      art_gv,
                    & sample,
                    & prior_value
                    );
                
                arsvlight_set_li_sv(
                      art_gv,
                      lr,
                      j,
                    & prior_value
                    );
            }
        }
        else
        {
            if ( sd->splatChannel0[s500_index] != INVALID_SPLAT_CHANNEL)
            {
                ArStokesVectorSample  temp0;

                arsvlightsample_li_sv(
                      art_gv,
                    & t0,
                      i,
                    & temp0
                    );

                arstokesvectorsample_d_mul_sv(
                      art_gv,
                      sd->splatFactor0[s500_index],
                    & temp0
                    );

                ArStokesVectorSample  temp1;
                
                arsvlight_li_sv(
                      art_gv,
                      lr,
                      sd->splatChannel0[s500_index],
                    & temp1
                    );

                arstokesvectorsample_sv_add_sv(
                      art_gv,
                    & temp0,
                    & temp1
                    );
                
                arsvlight_set_li_sv(
                      art_gv,
                      lr,
                      sd->splatChannel0[s500_index],
                    & temp1
                    );
            }

            if ( sd->splatChannel1[s500_index] != INVALID_SPLAT_CHANNEL)
            {
                ArStokesVectorSample  temp0;

                arsvlightsample_li_sv(
                      art_gv,
                    & t0,
                      i,
                    & temp0
                    );

                arstokesvectorsample_d_mul_sv(
                      art_gv,
                      sd->splatFactor1[s500_index],
                    & temp0
                    );

                ArStokesVectorSample  temp1;
                
                arsvlight_li_sv(
                      art_gv,
                      lr,
                      sd->splatChannel1[s500_index],
                    & temp1
                    );

                arstokesvectorsample_sv_add_sv(
                      art_gv,
                    & temp0,
                    & temp1
                    );
                
                arsvlight_set_li_sv(
                      art_gv,
                      lr,
                      sd->splatChannel1[s500_index],
                    & temp1
                    );
            }
        }
    }
}

void arsvlight_wsd_sloppy_add_l(
        const ART_GV                         * art_gv,
        const ArSVLightSample                * h0,
        const ArWavelength                   * w0,
        const ArSpectralSampleSplattingData  * s0,
        const double                           d0,
              ArSVLight                      * lr
        )
{
#ifdef FOUNDATION_ASSERTIONS
    if (   ARSVLIGHTSAMPLE_POLARISED(*h0)
        && ARSVLIGHT_POLARISED(*lr) )
    {
        ASSERT_COAXIAL_REFERENCE_FRAMES_RR(
            & ARSVLIGHTSAMPLE_REFFRAME(*h0),
            & ARSVLIGHT_REFFRAME(*lr),
              d0
            );
    }
#endif
    (void) d0;  // d0 is only used if assertions are on

    arsvlight_ws_add_l(
          art_gv,
          h0,
          w0,
          s0,
          lr
        );
}

void arsvlight_dws_mul_add_l(
        const ART_GV                       * art_gv,
        const double                         d0,
        const ArSVLightSample                * h0,
        const ArWavelength            * w0,
        const ArSpectralSampleSplattingData  * s0,
              ArSVLight                    * lr
        )
{
    ArSVLightSample  temp;
    
    arsvlightsample_dl_mul_l(
          art_gv,
          d0,
          h0,
        & temp
        );
    
    arsvlight_ws_add_l(
          art_gv,
        & temp,
          w0,
          s0,
          lr
        );
}

void arsvlight_dwsd_mul_sloppy_add_l(
        const ART_GV                         * art_gv,
        const double                           d0,
        const ArSVLightSample                * h0,
        const ArWavelength                   * w0,
        const ArSpectralSampleSplattingData  * s0,
        const double                           d1,
              ArSVLight                      * lr
        )
{
    ASSERT_VALID_SV_LIGHT_SAMPLE(h0)
    ASSERT_VALID_WAVELENGTH(w0)
    ASSERT_NONNEGATIVE_DOUBLE(d1)
    ASSERT_VALID_SV_LIGHT(lr)
    (void) d1;

    arsvlight_dws_mul_add_l(
          art_gv,
          d0,
          h0,
          w0,
          s0,
          lr
        );

    ASSERT_VALID_SV_LIGHT(lr)
}

void arsvlight_l_atomic_add_l(
        const ART_GV     * art_gv,
        const ArSVLight  * l0,
              ArSVLight  * lr
        )
{
    spc_s_atomic_add_s(
        art_gv,
        ARSVLIGHT_SV_I( *l0, 0 ),
        ARSVLIGHT_SV_I( *lr, 0 )
        );

    if ( ARSVLIGHT_POLARISED( *l0 ) || ARSVLIGHT_POLARISED( *lr ) )
    {
        ArStokesVector  * s0 = arstokesvector_alloc( art_gv );
        ArStokesVector  * s1 = arstokesvector_alloc( art_gv );

        arsvlight_lld_align_and_return_ss(
              art_gv,
              l0,
              lr,
              3.0 DEGREES,
              s0,
              s1
            );

        for ( unsigned int i = 1; i < 4; i++ )
        {
            //   If we are allowed to use c_add_c for Stokes component #0,
            //   we can also use it for the other ones.

            spc_s_atomic_add_s(
                art_gv,
                ARSV_I( *s0, i ),
                ARSVLIGHT_SV_I( *lr, i )
                );

            spc_s_atomic_add_s(
                art_gv,
                ARSV_I( *s1, i ),
                ARSVLIGHT_SV_I( *lr, i )
                );
        }

        //   CAVEAT: assignments are not atomic. However, since concurrent
        //           'add' operations can only alter an all-zero-bit 'NO' state
        //           to 'YES' (light can not become unpolarised by addition),
        //           this should work regardless. I.e. it might fail, but in
        //           that case it should fail in a way that does not really
        //           matter.

        ARSVLIGHT_POLARISED(*lr) = YES;

        arstokesvector_free( art_gv, s0 );
        arstokesvector_free( art_gv, s1 );
    }
}

void arsvlight_ll_add_l(
        const ART_GV     * art_gv,
        const ArSVLight  * l0,
        const ArSVLight  * l1,
              ArSVLight  * lr
        )
{
    spc_ss_add_s(
        art_gv,
        ARSVLIGHT_SV_I( *l0, 0 ),
        ARSVLIGHT_SV_I( *l1, 0 ),
        ARSVLIGHT_SV_I( *lr, 0 )
        );

    if ( ARSVLIGHT_POLARISED( *l0 ) || ARSVLIGHT_POLARISED( *l1 ) )
    {
        ArStokesVector  * s0 = arstokesvector_alloc( art_gv );
        ArStokesVector  * s1 = arstokesvector_alloc( art_gv );

        arsvlight_lldl_align_and_return_ss(
            art_gv,
            l0,
            l1,
            3.0 DEGREES,
            lr,
            s0,
            s1
            );

        for ( unsigned int i = 1; i < 4; i++ )
            spc_ss_add_s(
                art_gv,
                ARSV_I( *s0, i ),
                ARSV_I( *s1, i ),
                ARSVLIGHT_SV_I( *lr, i )
                );

        ARSVLIGHT_POLARISED(*lr) = YES;

        arstokesvector_free( art_gv, s0 );
        arstokesvector_free( art_gv, s1 );
    }
    else
        ARSVLIGHT_POLARISED(*lr) = NO;
}

void arsvlight_lld_sloppy_add_l(
        const ART_GV     * art_gv,
        const ArSVLight  * l0,
        const ArSVLight  * l1,
        const double       d0,
              ArSVLight  * lr
        )
{
    spc_ss_add_s(
        art_gv,
        ARSVLIGHT_SV_I( *l0, 0 ),
        ARSVLIGHT_SV_I( *l1, 0 ),
        ARSVLIGHT_SV_I( *lr, 0 )
        );

    if ( ARSVLIGHT_POLARISED( *l0 ) || ARSVLIGHT_POLARISED( *l1 ) )
    {
        ArStokesVector  * s0 = arstokesvector_alloc( art_gv );
        ArStokesVector  * s1 = arstokesvector_alloc( art_gv );

        arsvlight_lldl_align_and_return_ss(
              art_gv,
              l0,
              l1,
              d0,
              lr,
              s0,
              s1
            );

        for ( unsigned int i = 1; i < 4; i++ )
            spc_ss_add_s(
                art_gv,
                ARSV_I( *s0, i ),
                ARSV_I( *s1, i ),
                ARSVLIGHT_SV_I( *lr, i )
                );

        ARSVLIGHT_POLARISED(*lr) = YES;

        arstokesvector_free( art_gv, s0 );
        arstokesvector_free( art_gv, s1 );
    }
    else
        ARSVLIGHT_POLARISED(*lr) = NO;
}

void arsvlight_ll_sub_l(
        const ART_GV     * art_gv,
        const ArSVLight  * l0,
        const ArSVLight  * l1,
              ArSVLight  * lr
        )
{
    spc_ss_sub_s(
        art_gv,
        ARSVLIGHT_SV_I( *l0, 0 ),
        ARSVLIGHT_SV_I( *l1, 0 ),
        ARSVLIGHT_SV_I( *lr, 0 )
        );

    if ( ARSVLIGHT_POLARISED( *l0 ) || ARSVLIGHT_POLARISED( *l1 ) )
    {
        ArStokesVector  * s0 = arstokesvector_alloc( art_gv );
        ArStokesVector  * s1 = arstokesvector_alloc( art_gv );

        arsvlight_lldl_align_and_return_ss(
            art_gv,
            l0,
            l1,
            3.0 DEGREES,
            lr,
            s0,
            s1
            );

        for ( unsigned int i = 1; i < 4; i++ )
            spc_ss_sub_s(
                art_gv,
                ARSV_I( *s0, i ),
                ARSV_I( *s1, i ),
                ARSVLIGHT_SV_I( *lr, i )
                );

        ARSVLIGHT_POLARISED(*lr) = YES;

        arstokesvector_free( art_gv, s0 );
        arstokesvector_free( art_gv, s1 );
    }
    else
        ARSVLIGHT_POLARISED(*lr) = NO;
}

/* ---------------------------------------------------------------------------

    'arsvlight_d_mul_l'
    'arsvlight_ld_mul_l'

    Multiplies a given light value by a scalar.

------------------------------------------------------------------------aw- */

void arsvlight_d_mul_l(
        const ART_GV     * art_gv,
        const double       d0,
              ArSVLight  * lr
        )
{
    spc_d_mul_s(
        art_gv,
        d0,
        ARSVLIGHT_SV_I( *lr, 0 )
        );

    if ( ARSVLIGHT_POLARISED( *lr ) )
    {
        for ( unsigned int i = 1; i < 4; i++ )
            spc_d_mul_s(
                art_gv,
                d0,
                ARSVLIGHT_SV_I( *lr, i )
                );
    }
}

void arsvlight_dl_mul_l(
        const ART_GV     * art_gv,
        const double       d0,
        const ArSVLight  * l0,
              ArSVLight  * lr
        )
{
    spc_ds_mul_s(
        art_gv,
        d0,
        ARSVLIGHT_SV_I( *l0, 0 ),
        ARSVLIGHT_SV_I( *lr, 0 )
        );

    if ( ARSVLIGHT_POLARISED( *l0 ) )
    {
        for ( unsigned int i = 1; i < 4; i++ )
            spc_ds_mul_s(
                art_gv,
                d0,
                ARSVLIGHT_SV_I( *l0, i ),
                ARSVLIGHT_SV_I( *lr, i )
                );

        ARSVLIGHT_REFFRAME( *lr ) = ARSVLIGHT_REFFRAME( * l0 );
        ARSVLIGHT_POLARISED( *lr ) = YES;
    }
    else
        ARSVLIGHT_POLARISED( *lr ) = NO;
}

/* ---------------------------------------------------------------------------

    'arsvlight_i_mul_l'

    Scale the radiant intensity of a light ray by a wavelength-dependent
    scalar which is (not entirely unreasonably, but not 100% intuitively
    either) encoded as a light intensity.

------------------------------------------------------------------------aw- */

void arsvlight_i_mul_l(
        const ART_GV            * art_gv,
        const ArLightIntensity  * i0,
              ArSVLight         * lr
        )
{
    if ( ARSVLIGHT_POLARISED( *lr ) )
    {
        for ( unsigned int i = 0; i < 4; i++ )
            spc_s_mul_s(
                art_gv,
                i0,
                ARSVLIGHT_SV_I( *lr, i )
                );
    }
    else
        spc_s_mul_s(
            art_gv,
            i0,
            ARSVLIGHT_SV_I( *lr, 0 )
            );
}

/* ---------------------------------------------------------------------------

    'arsvlight_l_mul_l'
    'arsvlight_ll_mul_l'

    Multiplies the intensity of two light structs with each other.

    CAUTION: this operation should NEVER be used in a physically-based
    simulation since it has no obvious correspondence with any physical
    interaction or operation. If you were thinking of using it, you probably
    meant 'arattenuation_a_mul_l'.

------------------------------------------------------------------------aw- */

void arsvlight_l_mul_l(
        const ART_GV     * art_gv,
        const ArSVLight  * l0,
              ArSVLight  * lr
        )
{
    ART_ERRORHANDLING_WARNING(
        "arsvlight_l_mul_l is not a physically meaningful operation on two "
        "light operands"
        );

    spc_s_mul_s(
        art_gv,
        ARSVLIGHT_SV_I( *l0, 0 ),
        ARSVLIGHT_SV_I( *lr, 0 )
        );

    ARSVLIGHT_POLARISED( *lr ) = NO;
}

void arsvlight_ll_mul_l(
        const ART_GV     * art_gv,
        const ArSVLight  * l0,
        const ArSVLight  * l1,
              ArSVLight  * lr
        )
{
    ART_ERRORHANDLING_WARNING(
        "arsvlight_ll_mul_l is not a physically meaningful operation on two "
        "light operands"
        );

    spc_ss_mul_s(
        art_gv,
        ARSVLIGHT_SV_I( *l0, 0 ),
        ARSVLIGHT_SV_I( *l1, 0 ),
        ARSVLIGHT_SV_I( *lr, 0 )
        );

    ARSVLIGHT_POLARISED( *lr ) = NO;
}

/* ---------------------------------------------------------------------------

    'arsvlight_d_div_l'
    'arsvlight_dl_div_l'

    Multiplies a given light value by a scalar.

------------------------------------------------------------------------aw- */

void arsvlight_d_div_l(
        const ART_GV     * art_gv,
        const double       d0,
              ArSVLight  * lr
        )
{
    spc_d_mul_s(
        art_gv,
        d0,
        ARSVLIGHT_SV_I( *lr, 0 )
        );

    if ( ARSVLIGHT_POLARISED( *lr ) )
    {
        for ( unsigned int i = 1; i < 4; i++ )
            spc_d_mul_s(
                art_gv,
                d0,
                ARSVLIGHT_SV_I( *lr, i )
                );
    }
}

void arsvlight_dl_div_l(
        const ART_GV     * art_gv,
        const double       d0,
        const ArSVLight  * l0,
              ArSVLight  * lr
        )
{
    spc_ds_div_s(
        art_gv,
        d0,
        ARSVLIGHT_SV_I( *l0, 0 ),
        ARSVLIGHT_SV_I( *lr, 0 )
        );

    if ( ARSVLIGHT_POLARISED( *l0 ) )
    {
        for ( unsigned int i = 1; i < 4; i++ )
            spc_ds_div_s(
                art_gv,
                d0,
                ARSVLIGHT_SV_I( *l0, i ),
                ARSVLIGHT_SV_I( *lr, i )
                );

        ARSVLIGHT_REFFRAME( *lr ) = ARSVLIGHT_REFFRAME( * l0 );
        ARSVLIGHT_POLARISED( *lr ) = YES;
    }
    else
        ARSVLIGHT_POLARISED( *lr ) = NO;
}

void arsvlight_dl_pow_l(
        const ART_GV     * art_gv,
        const double       d0,
        const ArSVLight  * l0,
              ArSVLight  * lr
        )
{
    spc_ds_pow_s(
        art_gv,
        d0,
        ARSVLIGHT_SV_I( * l0, 0 ),
        ARSVLIGHT_SV_I( * lr, 0 )
        );

    if ( ARSVLIGHT_POLARISED( *l0 ) )
    {
        for ( int i = 1; i < 4; i++ )
            spc_ds_pow_s(
                art_gv,
                d0,
                ARSVLIGHT_SV_I( * l0, i ),
                ARSVLIGHT_SV_I( * lr, i )
                );

        ARSVLIGHT_POLARISED( *lr ) = YES;
    }
    else
        ARSVLIGHT_POLARISED( *lr ) = NO;
}

void arsvlight_ddl_clamp_l(
        const ART_GV     * art_gv,
        const double       d0,
        const double       d1,
        const ArSVLight  * l0,
              ArSVLight  * lr
        )
{
    spc_dds_clamp_s(
        art_gv,
        d0,
        d1,
        ARSVLIGHT_SV_I( * l0, 0 ),
        ARSVLIGHT_SV_I( * lr, 0 )
        );

    if ( ARSVLIGHT_POLARISED( *l0 ) )
    {
        for ( int i = 1; i < 4; i++ )
            spc_dds_clamp_s(
                art_gv,
                d0,
                d1,
                ARSVLIGHT_SV_I( * l0, i ),
                ARSVLIGHT_SV_I( * lr, i )
                );

        ARSVLIGHT_POLARISED( *lr ) = YES;
    }
    else
        ARSVLIGHT_POLARISED( *lr ) = NO;
}

void arsvlight_dl_mul_add_l(
        const ART_GV     * art_gv,
        const double       d0,
        const ArSVLight  * l0,
              ArSVLight  * lr
        )
{
    spc_ds_mul_add_s(
        art_gv,
        d0,
        ARSVLIGHT_SV_I( * l0, 0 ),
        ARSVLIGHT_SV_I( * lr, 0 )
        );

    if ( ARSVLIGHT_POLARISED( *l0 ) || ARSVLIGHT_POLARISED( *lr ) )
    {
        ArStokesVector  * s0 = arstokesvector_alloc( art_gv );
        ArStokesVector  * s1 = arstokesvector_alloc( art_gv );

        arsvlight_lld_align_and_return_ss(
              art_gv,
              l0,
              lr,
              3.0 DEGREES,
              s0,
              s1
            );

        for ( unsigned int i = 1; i < 4; i++ )
        {
            spc_d_mul_s(
                art_gv,
                d0,
                ARSV_I( *s0, i )
                );

            spc_ss_add_s(
                art_gv,
                ARSV_I( *s0, i ),
                ARSV_I( *s1, i ),
                ARSVLIGHT_SV_I( * lr, i )
                );
        }

        arstokesvector_free( art_gv, s0 );
        arstokesvector_free( art_gv, s1 );

        ARSVLIGHT_POLARISED(*lr) = YES;
    }
    else
        ARSVLIGHT_POLARISED(*lr) = NO;
}

void arsvlight_dld_mul_sloppy_add_l(
        const ART_GV     * art_gv,
        const double       d0,
        const ArSVLight  * l0,
        const double       d1,
              ArSVLight  * lr
        )
{
    spc_ds_mul_add_s(
        art_gv,
        d0,
        ARSVLIGHT_SV_I( * l0, 0 ),
        ARSVLIGHT_SV_I( * lr, 0 )
        );

    if ( ARSVLIGHT_POLARISED( *l0 ) || ARSVLIGHT_POLARISED( *lr ) )
    {
        ArStokesVector  * s0 = arstokesvector_alloc( art_gv );
        ArStokesVector  * s1 = arstokesvector_alloc( art_gv );

        arsvlight_lld_align_and_return_ss(
              art_gv,
              l0,
              lr,
              d1,
              s0,
              s1
            );

        for ( unsigned int i = 1; i < 4; i++ )
        {
            spc_d_mul_s(
                art_gv,
                d0,
                ARSV_I( *s0, i )
                );

            spc_ss_add_s(
                art_gv,
                ARSV_I( *s0, i ),
                ARSV_I( *s1, i ),
                ARSVLIGHT_SV_I( * lr, i )
                );
        }

        arstokesvector_free( art_gv, s0 );
        arstokesvector_free( art_gv, s1 );

        ARSVLIGHT_POLARISED(*lr) = YES;
    }
    else
        ARSVLIGHT_POLARISED(*lr) = NO;
}

void arsvlight_dll_interpol_l(
        const ART_GV     * art_gv,
        const double       d0,
        const ArSVLight  * l0,
        const ArSVLight  * l1,
              ArSVLight  * lr
        )
{
    spc_dss_interpol_s(
        art_gv,
        d0,
        ARSVLIGHT_SV_I( * l0, 0 ),
        ARSVLIGHT_SV_I( * l1, 0 ),
        ARSVLIGHT_SV_I( * lr, 0 )
        );

    if ( ARSVLIGHT_POLARISED( *l0 ) )
    {
        for ( int i = 1; i < 4; i++ )
            spc_dss_interpol_s(
                art_gv,
                d0,
                ARSVLIGHT_SV_I( * l0, i ),
                ARSVLIGHT_SV_I( * l1, i ),
                ARSVLIGHT_SV_I( * lr, i )
                );

        ARSVLIGHT_POLARISED( *lr ) = YES;
    }
    else
        ARSVLIGHT_POLARISED( *lr ) = NO;
}

/* ---------------------------------------------------------------------------

    'arsvlight_valid_l'

        Performs a sanity check on the given light struct. It first does a
        colour sanity check on the first Stokes component, and if this is
        passed, tests whether

        i_0 <= sqrt( i_1^2 + i_2^2 + i_3^2 )

        holds.


------------------------------------------------------------------------aw- */

unsigned int arsvlight_l_valid(
        const ART_GV     * art_gv,
        const ArSVLight  * l0
        )
{
    int light_is_valid =
        spc_s_valid(
            art_gv,
            ARSVLIGHT_SV_I( *l0, 0 )
            );

    if ( ! light_is_valid )
    {
        ART_ERRORHANDLING_WARNING(
            "colour data of first Stokes vector component are invalid"
            );
        return 0;
    }

    if ( ARSVLIGHT_POLARISED( *l0 ) )
    {
        int  sc1_valid =
            spc_s_valid(
                art_gv,
                ARSVLIGHT_SV_I( *l0, 1 )
                );

        int  sc2_valid =
            spc_s_valid(
                art_gv,
                ARSVLIGHT_SV_I( *l0, 2 )
                );

        int  sc3_valid =
            spc_s_valid(
                art_gv,
                ARSVLIGHT_SV_I( *l0, 3 )
                );

        if ( ! ( sc1_valid && sc2_valid && sc3_valid ) )
        {
            ART_ERRORHANDLING_WARNING(
                "colour data of second (%d), third (%d) and/or fourth (%d) Stokes vector component are invalid",
                sc1_valid,
                sc2_valid,
                sc3_valid
                );
            return 0;
        }

        if ( ! arrefframe_rf_valid( art_gv, & ARSVLIGHT_REFFRAME(*l0) ) )
        {
            ART_ERRORHANDLING_WARNING(
                "reference frame is invalid"
                );
            return 0;
        }

        ArSpectrum  * squaredColour[3];

        for ( int i = 0; i < 3; i++ )
        {
            squaredColour[i] = spc_alloc( art_gv );

            spc_ss_mul_s(
                art_gv,
                ARSVLIGHT_SV_I( *l0, i + 1 ),
                ARSVLIGHT_SV_I( *l0, i + 1 ),
                squaredColour[i]
                );
        }

        spc_s_add_s(
            art_gv,
            squaredColour[2],
            squaredColour[0]
            );

        spc_s_add_s(
            art_gv,
            squaredColour[1],
            squaredColour[0]
            );

        int  numberOfChannels = spc_channels( art_gv );

        for ( int i = 0; i < numberOfChannels; i++ )
        {
            double  ci = spc_si( art_gv, squaredColour[0], i );

            ci = sqrt( ci );

            if ( ci  > ( spc_si( art_gv, ARSVLIGHT_INTENSITY( *l0 ), i ) + 0.00001 ) )
                light_is_valid = NO;
        }

        for ( int i = 0; i < 3; i++ )
            spc_free( art_gv, squaredColour[i] );

        if ( ! light_is_valid)
        {
            ART_ERRORHANDLING_WARNING(
                "Stokes vector is ovepolarized"
                );
            return 0;
        }

    }

    return 1;
}

unsigned int arsvlight_ll_collinear(
        const ART_GV     * art_gv,
        const ArSVLight  * l0,
        const ArSVLight  * l1
        )
{
    (void) art_gv;
    (void) l0;
    (void) l1;
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
    return 1;
}

void arsvlight_l_debugprintf(
        const ART_GV     * art_gv,
        const ArSVLight  * l0
        )
{
    if ( ARSVLIGHT_POLARISED( *l0 ) )
    {
        printf( "ArSVLight (polarised):\n" );
        arrefframe_rf_debugprintf(
              art_gv,
            & ARSVLIGHT_REFFRAME( *l0 )
            );

        arstokesvector_sv_debugprintf(
              art_gv,
              ARSVLIGHT_SV( *l0 )
            );

        printf( "\n" );
    }
    else
    {
        printf( "ArSVLight (unpolarised): " );
        spc_s_debugprintf(
              art_gv,
              ARSVLIGHT_INTENSITY(*l0)
            );
    }
}

// ===========================================================================
