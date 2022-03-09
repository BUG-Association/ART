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

#define ART_MODULE_NAME     ArSVLightSample

#include "ArSVLightSample.h"
#include "_ArLightSample_GV.h"
#include <stdlib.h>
#include "LightSampleImplementationMacros.h"
#include "ArLightAndAttenuationSubsystemManagement.h"
#include "FoundationAssertionMacros.h"

typedef struct ArSVLightSample_GV
{
    ArSVLightSample  * light_none;
    ArSVLightSample  * light_unit;
}
ArSVLightSample_GV;

ART_MODULE_INITIALISATION_FUNCTION
(
    ArSVLightSample_GV  * arsvlightsample_gv =
        ALLOC(ArSVLightSample_GV);

    arsvlightsample_gv->light_none = 0;
    arsvlightsample_gv->light_unit = 0;

    art_gv->arsvlightsample_gv = arsvlightsample_gv;
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    if ( art_gv->arsvlightsample_gv->light_none )
        arsvlightsample_free( art_gv, art_gv->arsvlightsample_gv->light_none );
    if ( art_gv->arsvlightsample_gv->light_unit )
        arsvlightsample_free( art_gv, art_gv->arsvlightsample_gv->light_unit );

    FREE( art_gv->arsvlightsample_gv );
)

ART_LIGHT_AND_ATTENUATION_MODULE_INITIALISATION_FUNCTION
(
    art_gv->arsvlightsample_gv->light_none =
        arsvlightsample_d_alloc_init_unpolarised(
            art_gv,
            0.0
            );

    art_gv->arsvlightsample_gv->light_unit =
        arsvlightsample_d_alloc_init_unpolarised(
            art_gv,
            1.0
            );
)

ART_LIGHT_AND_ATTENUATION_MODULE_SHUTDOWN_FUNCTION
(
    if ( art_gv->arsvlightsample_gv->light_none )
        arsvlightsample_free( art_gv, art_gv->arsvlightsample_gv->light_none );
    if ( art_gv->arsvlightsample_gv->light_unit )
        arsvlightsample_free( art_gv, art_gv->arsvlightsample_gv->light_unit );
)

SWITCHING_IMPLEMENTATION_FOR_SAMPLE_LCT( arsvlightsample )


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

#include "ArSVLightSampleDebugOutput.c"


#define  YES  1
#define  NO   0


void arsvlightsample_li_sv(
        const ART_GV                * art_gv,
        const ArSVLightSample       * l0,
        const int                     i0,
              ArStokesVectorSample  * sr
        )
{
    (void) art_gv;
    
    ASSERT_VALID_SV_LIGHT_SAMPLE(l0);
    ASSERT_ALLOCATED_STOKES_VECTOR(sr);

    ARSVS_I( *sr, 0 ) = SPS(SPS_CI(ARSVLIGHTSAMPLE_SV_I( *l0, 0 ), i0));
    
    if ( ARSVLIGHTSAMPLE_POLARISED(*l0) )
    {
        ARSVS_I( *sr, 1 ) = SPS(SPS_CI(ARSVLIGHTSAMPLE_SV_I( *l0, 1 ), i0));
        ARSVS_I( *sr, 2 ) = SPS(SPS_CI(ARSVLIGHTSAMPLE_SV_I( *l0, 2 ), i0));
        ARSVS_I( *sr, 3 ) = SPS(SPS_CI(ARSVLIGHTSAMPLE_SV_I( *l0, 3 ), i0));
    }
    else
    {
        ARSVS_I( *sr, 1 ) = SPS(0.0);
        ARSVS_I( *sr, 2 ) = SPS(0.0);
        ARSVS_I( *sr, 3 ) = SPS(0.0);
    }

    ASSERT_VALID_STOKES_VECTOR_SAMPLE(sr);
}

void arsvlightsample_svi_set_l(
        const ART_GV                * art_gv,
        const ArStokesVectorSample  * sr,
        const int                     i0,
              ArSVLightSample       * l0
        )
{
    (void) art_gv;
    (void) sr;
    (void) i0;
    (void) l0;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

const char * arsvlightsample_lct_string(
        const ART_GV  * art_gv
        )
{
    (void) art_gv;
    
    return "Stokes vector light";
}

ArSVLightSample * arsvlightsample_alloc(
        const ART_GV  * art_gv
        )
{
    (void) art_gv;
    
    ArSVLightSample  * newSVLight = ALLOC( ArSVLightSample );

    return newSVLight;
}

void arsvlightsample_free(
        const ART_GV         * art_gv,
              ArSVLightSample  * lr
        )
{
    (void) art_gv;
    
    FREE(lr);
}

ArSVLightSample * arsvlightsample_d_alloc_init_unpolarised(
        const ART_GV  * art_gv,
        const double    d0
        )
{
    ArSVLightSample  * lr;

    lr = arsvlightsample_alloc( art_gv );

    arsvlightsample_d_init_unpolarised_l(
        art_gv,
        d0,
        lr
        );

    return lr;
}

ArSVLightSample * arsvlightsample_l_alloc_init(
        const ART_GV         * art_gv,
        const ArSVLightSample  * l0
        )
{
    ArSVLightSample  * lr;

    lr = arsvlightsample_alloc( art_gv );

    arsvlightsample_l_init_l(
        art_gv,
        l0,
        lr
        );

    return lr;
}

ArSVLightSample const * arsvlightsample_unpolarised_unit(
        const ART_GV  * art_gv
        )
{
    return art_gv->arsvlightsample_gv->light_unit;
}

ArSVLightSample const * arsvlightsample_none(
        const ART_GV  * art_gv
        )
{
    return art_gv->arsvlightsample_gv->light_none;
}

void arsvlightsample_d_init_unpolarised_l(
        const ART_GV         * art_gv,
        const double           d0,
              ArSVLightSample  * lr
        )
{
    sps_d_init_s(
        art_gv,
        d0,
        & ARSVLIGHTSAMPLE_INTENSITY( *lr )
        );

    for ( unsigned int i = 1; i < 4; i++ )
        sps_d_init_s(
            art_gv,
            0.0,
            & ARSVLIGHTSAMPLE_SV_I( *lr, i )
            );

    ARSVLIGHTSAMPLE_POLARISED( *lr ) = NO;
}

void arsvlightsample_s_init_unpolarised_l(
        const ART_GV         * art_gv,
        const ArSpectralSample * c0,
              ArSVLightSample  * lr
        )
{
    sps_s_init_s(
        art_gv,
        c0,
        & ARSVLIGHTSAMPLE_INTENSITY( *lr )
        );

    for ( unsigned int i = 1; i < 4; i++ )
        sps_d_init_s(
            art_gv,
            0.0,
            & ARSVLIGHTSAMPLE_SV_I( *lr, i )
            );

    ARSVLIGHTSAMPLE_POLARISED( *lr ) = NO;
}

void arsvlightsample_s_rf_init_polarised_l(
        const ART_GV                * art_gv,
        const ArStokesVectorSample    * s0,
        const ArReferenceFrame      * r0,
              ArSVLightSample         * lr
        )
{
    for ( unsigned int i = 0; i < 4; i++ )
        sps_s_init_s(
            art_gv,
            & ARSVS_I( *s0, i ),
            & ARSVLIGHTSAMPLE_SV_I( *lr, i )
            );

    ARSVLIGHTSAMPLE_REFFRAME( *lr ) = *r0;

    ARSVLIGHTSAMPLE_POLARISED( *lr ) = YES;
}

void arsvlightsample_l_init_s(
        const ART_GV         * art_gv,
        const ArSVLightSample  * l0,
              ArSpectralSample * cr
        )
{
    sps_s_init_s(
        art_gv,
        & ARSVLIGHTSAMPLE_SV_I( *l0, 0 ),
        cr
        );
}

void  arsvlightsample_l_init_i(
        const ART_GV                * art_gv,
        const ArSVLightSample         * l0,
              ArLightIntensitySample  * ir
        )
{
    sps_s_init_s(
        art_gv,
        & ARSVLIGHTSAMPLE_SV_I( *l0, 0 ),
        ir
        );
}

void  arsvlightsample_l_init_l(
        const ART_GV         * art_gv,
        const ArSVLightSample  * l0,
              ArSVLightSample  * lr
        )
{
    for ( unsigned int i = 0; i < 4; i++ )
        sps_s_init_s(
            art_gv,
            & ARSVLIGHTSAMPLE_SV_I( *l0, i ),
            & ARSVLIGHTSAMPLE_SV_I( *lr, i )
            );

    ARSVLIGHTSAMPLE_REFFRAME( *lr ) = ARSVLIGHTSAMPLE_REFFRAME( *l0 );

    ARSVLIGHTSAMPLE_POLARISED( *lr ) = ARSVLIGHTSAMPLE_POLARISED( *l0 );
}

unsigned int  arsvlightsample_l_polarised(
        const ART_GV         * art_gv,
        const ArSVLightSample  * l0
        )
{
    (void) art_gv;
    
    return ARSVLIGHTSAMPLE_POLARISED( *l0 );
}

ArReferenceFrame const * arsvlightsample_l_refframe(
        const ART_GV         * art_gv,
        const ArSVLightSample  * l0
        )
{
    (void) art_gv;
    
    if ( ARSVLIGHTSAMPLE_POLARISED( *l0 ) )
        return & ARSVLIGHTSAMPLE_REFFRAME( *l0 );
    else
        return 0;
}

void arsvlightsample_l_to_sv(
        const ART_GV              * art_gv,
        const ArSVLightSample       * l0,
              ArStokesVectorSample  * sr
        )
{
    if ( ARSVLIGHTSAMPLE_POLARISED( *l0 ) )
        arstokesvectorsample_sv_init_sv(
              art_gv,
            & ARSVLIGHTSAMPLE_SV( *l0 ),
              sr
            );
    else
        arstokesvectorsample_s_init_depolarised_sv(
              art_gv,
              & ARSVLIGHTSAMPLE_SV_I( *l0, 0 ),
              sr
            );
}

/* ---------------------------------------------------------------------------

    'arsvlightsample_ld_realign_to_coaxial_refframe_sv'

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

void arsvlightsample_ld_realign_to_coaxial_refframe_sv(
        const ART_GV                * art_gv,
        const ArSVLightSample         * l0,
        const double                  d0,
        const ArReferenceFrame      * r0,
              ArStokesVectorSample    * sr
        )
{
    ASSERT_VALID_SV_LIGHT_SAMPLE(l0)
    ASSERT_VALID_REFERENCE_FRAME(r0)
    ASSERT_ALLOCATED_STOKES_VECTOR(sr)
    ASSERT_COAXIAL_SAMPLE_REFERENCE_FRAMES_R_SVL( r0, l0, d0 )
    (void) d0; // only used when assertions are on
    
    if ( ARSVLIGHTSAMPLE_POLARISED( *l0 ) )
    {
        double  cos_phi =
            vec3d_vv_dot(
                & ARSVLIGHTSAMPLE_REFFRAME_I( *l0, 0 ),
                & ARREFFRAME_RF_I( *r0, 0 )
                );

        double  sin_phi =
            vec3d_vv_dot(
                & ARSVLIGHTSAMPLE_REFFRAME_I( *l0, 1 ),
                & ARREFFRAME_RF_I( *r0, 0 )
                );

        double  cos_2phi = M_SQR( cos_phi ) - M_SQR( sin_phi );
        double  sin_2phi = 2 * cos_phi * sin_phi;

        sps_s_init_s(
              art_gv,
            & ARSVLIGHTSAMPLE_SV_I( *l0, 0 ),
            & ARSVS_I( *sr, 0 )
            );

        sps_s_init_s(
              art_gv,
            & ARSVLIGHTSAMPLE_SV_I( *l0, 3 ),
            & ARSVS_I( *sr, 3 )
            );

        sps_ds_mul_ds_mul_add_s(
              art_gv,
              cos_2phi,
            & ARSVLIGHTSAMPLE_SV_I( *l0, 1 ),
              sin_2phi,
            & ARSVLIGHTSAMPLE_SV_I( *l0, 2 ),
            & ARSVS_I( *sr, 1 )
            );

        sps_ds_mul_ds_mul_add_s(
              art_gv,
             -sin_2phi,
            & ARSVLIGHTSAMPLE_SV_I( *l0, 1 ),
              cos_2phi,
            & ARSVLIGHTSAMPLE_SV_I( *l0, 2 ),
            & ARSVS_I( *sr, 2 )
            );
    }
    else
    {
        sps_s_init_s(
            art_gv,
            & ARSVLIGHTSAMPLE_SV_I( *l0, 0 ),
            & ARSVS_I( *sr, 0 )
            );

        for ( unsigned int i = 1; i < 4; i++ )
            sps_d_init_s(
                  art_gv,
                  0.0,
                & ARSVS_I( *sr, i )
                );
    }

    ASSERT_VALID_SV_LIGHT_SAMPLE(l0)
    ASSERT_VALID_STOKES_VECTOR_SAMPLE(sr)
}

/* ---------------------------------------------------------------------------

    'arsvlightsample_realign_to_coaxial_refframe_l'

    This procedure modifies a ray of polarised light to fit a different
    coaxial reference coordinate system. It is basically a wrapper around
    'arsvlightsample_ld_realign_to_coaxial_refframe_sv' - see the description of this
    function for more information and the restrictions on its usage.

------------------------------------------------------------------------aw- */

void arsvlightsample_realign_to_coaxial_refframe_l(
        const ART_GV                * art_gv,
        const ArReferenceFrame      * r0,
              ArSVLightSample         * lr
        )
{
    if ( ARSVLIGHTSAMPLE_POLARISED( *lr ) )
    {
        ArStokesVectorSample  temp;

        arsvlightsample_ld_realign_to_coaxial_refframe_sv(
              art_gv,
              lr,
              3.0 DEGREES,
              r0,
            & temp
            );

        ARSVLIGHTSAMPLE_REFFRAME(*lr) = *r0;
        ARSVLIGHTSAMPLE_SV(*lr) = temp;
    }
}

void arsvlightsample_l_realign_to_coaxial_refframe_l(
        const ART_GV                * art_gv,
        const ArSVLightSample         * l0,
        const ArReferenceFrame      * r0,
              ArSVLightSample         * lr
        )
{
    if ( ARSVLIGHTSAMPLE_POLARISED( *l0 ) )
    {
        ArStokesVectorSample  temp;

        arsvlightsample_ld_realign_to_coaxial_refframe_sv(
              art_gv,
              l0,
              3.0 DEGREES,
              r0,
            & temp
            );

        ARSVLIGHTSAMPLE_REFFRAME(*lr) = *r0;
        ARSVLIGHTSAMPLE_SV(*lr) = temp;
        ARSVLIGHTSAMPLE_POLARISED(*lr) = YES;
    }
    else
        arsvlightsample_l_init_l(
            art_gv,
            l0,
            lr
            );
}

unsigned int arsvlightsample_ll_equal(
        const ART_GV         * art_gv,
        const ArSVLightSample  * l0,
        const ArSVLightSample  * l1
        )
{
    int  p0 = ARSVLIGHTSAMPLE_POLARISED( *l0 );
    int  p1 = ARSVLIGHTSAMPLE_POLARISED( *l1 );

    if ( p0 != p1 )
        return 0;

    if ( p0 )
    {
        if ( ! arrefframe_rf_rf_equal(
                     art_gv,
                   & ARSVLIGHTSAMPLE_REFFRAME( *l0 ),
                   & ARSVLIGHTSAMPLE_REFFRAME( *l1 )
                   ) )
            return 0;

        if ( ! arstokesvectorsample_sv_sv_equal(
                       art_gv,
                     & ARSVLIGHTSAMPLE_SV( *l0 ),
                     & ARSVLIGHTSAMPLE_SV( *l1 )
                   ) )
            return 0;
    }
    else
    {
        if ( ! sps_ss_equal(
                     art_gv,
                     & ARSVLIGHTSAMPLE_SV_I( *l0, 0 ),
                     & ARSVLIGHTSAMPLE_SV_I( *l1, 0 )
                   ) )
            return 0;
    }

    return 1;
}

unsigned int arsvlightsample_lld_equal(
        const ART_GV         * art_gv,
        const ArSVLightSample  * l0,
        const ArSVLightSample  * l1,
        const double       max_relative_error
        )
{
    int  p0 = ARSVLIGHTSAMPLE_POLARISED( *l0 );
    int  p1 = ARSVLIGHTSAMPLE_POLARISED( *l1 );

    if ( p0 != p1 )
        return 0;

    if ( p0 )
    {
        if ( ! arrefframe_rf_rf_d_equal(
                     art_gv,
                   & ARSVLIGHTSAMPLE_REFFRAME( *l0 ),
                   & ARSVLIGHTSAMPLE_REFFRAME( *l1 ),
                     max_relative_error
                   ) )
            return 0;

        if ( ! arstokesvectorsample_sv_sv_d_equal(
                       art_gv,
                     & ARSVLIGHTSAMPLE_SV( *l0 ),
                     & ARSVLIGHTSAMPLE_SV( *l1 ),
                       max_relative_error
                   ) )
            return 0;
    }
    else
    {
        if ( ! sps_ssd_equal(
                     art_gv,
                     & ARSVLIGHTSAMPLE_SV_I( *l0, 0 ),
                     & ARSVLIGHTSAMPLE_SV_I( *l1, 0 ),
                     max_relative_error
                   ) )
            return 0;
    }

    return 1;
}

/* ---------------------------------------------------------------------------

    'arsvlightsample_l_norm', 'arsvlightsample_l_norm_visible_range'

        Computes the L1 norm of the light in question. This only affects the
        first component of the light regardless of polarisation state.


------------------------------------------------------------------------aw- */

double arsvlightsample_l_norm(
        const ART_GV         * art_gv,
        const ArSVLightSample  * l0
        )
{
    return
        sps_s_l1_norm(
            art_gv,
            & ARSVLIGHTSAMPLE_INTENSITY( *l0 )
            );
}

/* ---------------------------------------------------------------------------

    'arsvlightsample_ll_align_and_return_ss'
    'arsvlightsample_lll_align_and_return_ss'

    These at first glance rather odd functions fulfil a similar purpose.
    The only difference between them is that the first caters for the two-
    operand case, while the second is intended for three operands. Both
    require that at least one of the input light values is polarised.

    They are only intended to be used inside a function which manipulates
    light as input and output values (e.g. arsvlightsample_l_add_l for adding
    radiances). Tasks which are common to all of these - and which are
    performed by these two functions - are:

    - to bring both of the light structs to the same reference system by
      rotating one of them. Note that as always this operation only makes
      sense if the two are collinear light rays; see the reference for
      'arsvlightsample_ld_realign_to_coaxial_refframe_sv' for details

    - if necessary, assign this new reference system to the result

    - if necessary, set the polarisation flag for the result

    - and to return the Stokes Vectors for the aligned light values

    Returning the plain Stokes Vectors is necessary since most of the
    calling functions perform their manipulations on these directly.

------------------------------------------------------------------------aw- */


void arsvlightsample_lld_align_and_return_ss(
        const ART_GV              * art_gv,
        const ArSVLightSample       * l0,
              ArSVLightSample       * lr,
        const double                d0,
              ArStokesVectorSample  * svr0,
              ArStokesVectorSample  * svr1
        )
{
    ASSERT_VALID_SV_LIGHT_SAMPLE(l0)
    ASSERT_VALID_SV_LIGHT_SAMPLE(lr)

    /* -----------------------------------------------------------------------
        Two-operand version. Assumption: at least one of the two operands
        is already polarised, or this function would not have been called.
    --------------------------------------------------------------------aw- */

    if ( ! ARSVLIGHTSAMPLE_POLARISED( *lr ) )
    {
        /* ------------------------------------------------------------------
            Case 1: the result is not polarised yet, so we just copy the
                    reference system from the first operand and use
                    'arsvlightsample_ld_realign_to_coaxial_refframe_sv' to extract
                    the "empty" Stokes components for the result.
        ---------------------------------------------------------------aw- */

        if ( ARSVLIGHTSAMPLE_POLARISED( *l0 ) )
        {
            arstokesvectorsample_sv_init_sv(
                  art_gv,
                & ARSVLIGHTSAMPLE_SV( *l0 ),
                  svr0
                );

            ASSERT_VALID_REFERENCE_FRAME( & ARSVLIGHTSAMPLE_REFFRAME( *l0 ) )

            ASSERT_VALID_SV_LIGHT_SAMPLE(lr)

            arsvlightsample_ld_realign_to_coaxial_refframe_sv(
                  art_gv,
                  lr,
                  d0,
                & ARSVLIGHTSAMPLE_REFFRAME( *l0 ),
                  svr1
                );

            ARSVLIGHTSAMPLE_REFFRAME( *lr ) = ARSVLIGHTSAMPLE_REFFRAME( *l0 );
            ARSVLIGHTSAMPLE_POLARISED( *lr ) = YES;

            ASSERT_VALID_SV_LIGHT_SAMPLE(lr)
            ASSERT_VALID_STOKES_VECTOR_SAMPLE(svr0)
            ASSERT_VALID_STOKES_VECTOR_SAMPLE(svr1)
        }
        else
        {
            arstokesvectorsample_sv_init_sv(
                  art_gv,
                & ARSVLIGHTSAMPLE_SV( *l0 ),
                  svr0
                );

            arstokesvectorsample_sv_init_sv(
                  art_gv,
                & ARSVLIGHTSAMPLE_SV( *lr ),
                  svr1
                );

            ARSVLIGHTSAMPLE_POLARISED( *lr ) = NO;

            ASSERT_VALID_SV_LIGHT_SAMPLE(lr)
            ASSERT_VALID_STOKES_VECTOR_SAMPLE(svr0)
            ASSERT_VALID_STOKES_VECTOR_SAMPLE(svr1)
        }
    }
    else
    {
        /* ------------------------------------------------------------------
            Case 2: the result is already polarised, so we align the first
                    operand with the reference system of the result.
        ---------------------------------------------------------------aw- */

        arsvlightsample_ld_realign_to_coaxial_refframe_sv(
              art_gv,
              l0,
              d0,
            & ARSVLIGHTSAMPLE_REFFRAME( *lr ),
              svr0
            );

        arstokesvectorsample_sv_init_sv(
              art_gv,
            & ARSVLIGHTSAMPLE_SV( *lr ),
              svr1
            );

        ASSERT_VALID_SV_LIGHT_SAMPLE(lr)
        ASSERT_VALID_STOKES_VECTOR_SAMPLE(svr0)
        ASSERT_VALID_STOKES_VECTOR_SAMPLE(svr1)
    }
}

void arsvlightsample_lldl_align_and_return_ss(
        const ART_GV              * art_gv,
        const ArSVLightSample       * l0,
        const ArSVLightSample       * l1,
        const double                d0,
              ArSVLightSample       * lr,
              ArStokesVectorSample  * svr0,
              ArStokesVectorSample  * svr1
        )
{
    ASSERT_VALID_SV_LIGHT_SAMPLE(l0)
    ASSERT_VALID_SV_LIGHT_SAMPLE(l1)

    ASSERT_ALLOCATED_SV_LIGHT(lr)

    /* -----------------------------------------------------------------------
        Three-operand version. Assumption: at least one of the two input
        operands 'l0' or 'l1' is already polarised, or this
        function would not have been called.
    --------------------------------------------------------------------aw- */

    if ( ! ARSVLIGHTSAMPLE_POLARISED( *l1 ) )
    {
        /* ------------------------------------------------------------------
            Case 1: 'l0' is the only polarised one, so we rotate the
                    other one to its reference system and also assign its
                    reference system to the result.
        ---------------------------------------------------------------aw- */

        arstokesvectorsample_sv_init_sv(
              art_gv,
            & ARSVLIGHTSAMPLE_SV( *l0 ),
              svr0
            );

        arsvlightsample_ld_realign_to_coaxial_refframe_sv(
            art_gv,
              l1,
              d0,
            & ARSVLIGHTSAMPLE_REFFRAME( *l0 ),
              svr1
            );

        ARSVLIGHTSAMPLE_REFFRAME( *lr ) = ARSVLIGHTSAMPLE_REFFRAME( *l0 );
    }
    else
    {
        /* ------------------------------------------------------------------
            Case 2: 'l1' and possibly also 'l0' are polarised, so
                    we use 'l1' as the reference and assign its
                    reference system to the result.
        ---------------------------------------------------------------aw- */

        arsvlightsample_ld_realign_to_coaxial_refframe_sv(
              art_gv,
              l0,
              d0,
            & ARSVLIGHTSAMPLE_REFFRAME( *l1 ),
              svr0
            );

        arstokesvectorsample_sv_init_sv(
              art_gv,
            & ARSVLIGHTSAMPLE_SV( *l1 ),
              svr1
            );

        ARSVLIGHTSAMPLE_REFFRAME( *lr ) = ARSVLIGHTSAMPLE_REFFRAME( *l1 );
    }

    ARSVLIGHTSAMPLE_POLARISED( *lr ) = YES;

    ASSERT_VALID_SV_LIGHT_SAMPLE(lr)
    ASSERT_VALID_STOKES_VECTOR_SAMPLE(svr0)
    ASSERT_VALID_STOKES_VECTOR_SAMPLE(svr1)
}

void arsvlightsample_l_inv_l(
        const ART_GV         * art_gv,
        const ArSVLightSample  * l0,
              ArSVLightSample  * lr
        )
{
    (void) art_gv;
    (void) l0;
    (void) lr;
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

/* ---------------------------------------------------------------------------

    'arsvlightsample_l_min'
    'arsvlightsample_l_max'

    Computes the minimum and maximum for a given light struct. Since the
    meaning of "min" and "max" in a rendering context is based on absolute
    radiometric intensity, only the first of the Stokes components is used
    for this computation.

------------------------------------------------------------------------aw- */

double arsvlightsample_l_max(
        const ART_GV         * art_gv,
        const ArSVLightSample  * l0
        )
{
    return
        sps_s_max(
            art_gv,
            & ARSVLIGHTSAMPLE_INTENSITY( *l0 )
            );
}

double arsvlightsample_l_min(
        const ART_GV         * art_gv,
        const ArSVLightSample  * l0
        )
{
    return
        sps_s_min(
            art_gv,
            & ARSVLIGHTSAMPLE_INTENSITY( *l0 )
            );
}

/* ---------------------------------------------------------------------------

    'arsvlightsample_ll_max_l'
    'arsvlightsample_ll_min_l'

------------------------------------------------------------------------aw- */

void arsvlightsample_ll_max_l(
        const ART_GV         * art_gv,
        const ArSVLightSample  * l0,
        const ArSVLightSample  * l1,
              ArSVLightSample  * lr
        )
{
    sps_ss_max_s(
        art_gv,
        & ARSVLIGHTSAMPLE_SV_I( * l0, 0 ),
        & ARSVLIGHTSAMPLE_SV_I( * l1, 0 ),
        & ARSVLIGHTSAMPLE_SV_I( * lr, 0 )
        );

    ARSVLIGHTSAMPLE_POLARISED( *lr ) = NO;
}

void arsvlightsample_ll_min_l(
        const ART_GV         * art_gv,
        const ArSVLightSample  * l0,
        const ArSVLightSample  * l1,
              ArSVLightSample  * lr
        )
{
    sps_ss_min_s(
        art_gv,
        & ARSVLIGHTSAMPLE_SV_I( * l0, 0 ),
        & ARSVLIGHTSAMPLE_SV_I( * l1, 0 ),
        & ARSVLIGHTSAMPLE_SV_I( * lr, 0 )
        );

    ARSVLIGHTSAMPLE_POLARISED( *lr ) = NO;
}

double arsvlightsample_ll_maxdiff(
        const ART_GV         * art_gv,
        const ArSVLightSample  * l0,
        const ArSVLightSample  * l1
        )
{
    return
        sps_ss_maxdiff(
            art_gv,
            & ARSVLIGHTSAMPLE_SV_I( * l0, 0 ),
            & ARSVLIGHTSAMPLE_SV_I( * l1, 0 )
            );
}

/* ---------------------------------------------------------------------------

    'arsvlightsample_i_add_l'

    Adds an unpolarised radiant intensity to a - possibly polarised - light
    ray. This only affects the first component.

------------------------------------------------------------------------aw- */

void arsvlightsample_i_add_l(
        const ART_GV                * art_gv,
        const ArLightIntensitySample  * i0,
              ArSVLightSample         * lr
        )
{
    sps_s_add_s(
        art_gv,
        i0,
        & ARSVLIGHTSAMPLE_SV_I( *lr, 0 )
        );
}

/* ---------------------------------------------------------------------------

    'arsvlightsample_l_add_l'
    'arsvlightsample_ll_add_l'
    'arsvlightsample_ll_sub_l'

    Adds or subtracts the radiant intensities and polarisation states of two
    collinear light rays. The first Stokes components can be normally combined,
    but the other three have to be aligned to a common reference system before
    they can be added or subtracted. See the reference for
    'arsvlightsample_ll_align_and_return_ss' for details of this alignment operation.

------------------------------------------------------------------------aw- */

void arsvlightsample_l_add_l(
        const ART_GV         * art_gv,
        const ArSVLightSample  * l0,
              ArSVLightSample  * lr
        )
{
    sps_s_add_s(
        art_gv,
        & ARSVLIGHTSAMPLE_SV_I( *l0, 0 ),
        & ARSVLIGHTSAMPLE_SV_I( *lr, 0 )
        );

    if ( ARSVLIGHTSAMPLE_POLARISED( *l0 ) || ARSVLIGHTSAMPLE_POLARISED( *lr ) )
    {
        ArStokesVectorSample  * s0 = arstokesvectorsample_alloc( art_gv );
        ArStokesVectorSample  * s1 = arstokesvectorsample_alloc( art_gv );

        arsvlightsample_lld_align_and_return_ss(
              art_gv,
              l0,
              lr,
              3.0 DEGREES,
              s0,
              s1
            );

        for ( unsigned int i = 1; i < 4; i++ )
            sps_ss_add_s(
                art_gv,
                & ARSVS_I( *s0, i ),
                & ARSVS_I( *s1, i ),
                & ARSVLIGHTSAMPLE_SV_I( *lr, i )
                );

        ARSVLIGHTSAMPLE_POLARISED(*lr) = YES;

        arstokesvectorsample_free( art_gv, s0 );
        arstokesvectorsample_free( art_gv, s1 );
    }
}

void arsvlightsample_dl_sloppy_add_l(
        const ART_GV         * art_gv,
        const double           d0,
        const ArSVLightSample  * l0,
              ArSVLightSample  * lr
        )
{
    sps_s_add_s(
        art_gv,
        & ARSVLIGHTSAMPLE_SV_I( *l0, 0 ),
        & ARSVLIGHTSAMPLE_SV_I( *lr, 0 )
        );

    if ( ARSVLIGHTSAMPLE_POLARISED( *l0 ) || ARSVLIGHTSAMPLE_POLARISED( *lr ) )
    {
        ArStokesVectorSample  * s0 = arstokesvectorsample_alloc( art_gv );
        ArStokesVectorSample  * s1 = arstokesvectorsample_alloc( art_gv );

        arsvlightsample_lld_align_and_return_ss(
              art_gv,
              l0,
              lr,
              d0,
              s0,
              s1
            );

        for ( unsigned int i = 1; i < 4; i++ )
            sps_ss_add_s(
                art_gv,
                & ARSVS_I( *s0, i ),
                & ARSVS_I( *s1, i ),
                & ARSVLIGHTSAMPLE_SV_I( *lr, i )
                );

        ARSVLIGHTSAMPLE_POLARISED(*lr) = YES;

        arstokesvectorsample_free( art_gv, s0 );
        arstokesvectorsample_free( art_gv, s1 );
    }
}

void arsvlightsample_l_atomic_add_l(
        const ART_GV         * art_gv,
        const ArSVLightSample  * l0,
              ArSVLightSample  * lr
        )
{
    sps_s_atomic_add_s(
        art_gv,
        & ARSVLIGHTSAMPLE_SV_I( *l0, 0 ),
        & ARSVLIGHTSAMPLE_SV_I( *lr, 0 )
        );

    if ( ARSVLIGHTSAMPLE_POLARISED( *l0 ) || ARSVLIGHTSAMPLE_POLARISED( *lr ) )
    {
        ArStokesVectorSample  * s0 = arstokesvectorsample_alloc( art_gv );
        ArStokesVectorSample  * s1 = arstokesvectorsample_alloc( art_gv );

        arsvlightsample_lld_align_and_return_ss(
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

            sps_s_atomic_add_s(
                art_gv,
                & ARSVS_I( *s0, i ),
                & ARSVLIGHTSAMPLE_SV_I( *lr, i )
                );

            sps_s_atomic_add_s(
                art_gv,
                & ARSVS_I( *s1, i ),
                & ARSVLIGHTSAMPLE_SV_I( *lr, i )
                );
        }

        //   CAVEAT: assignments are not atomic. However, since concurrent
        //           'add' operations can only alter an all-zero-bit 'NO' state
        //           to 'YES' (light can not become unpolarised by addition),
        //           this should work regardless. I.e. it might fail, but in
        //           that case it should fail in a way that does not really
        //           matter.

        ARSVLIGHTSAMPLE_POLARISED(*lr) = YES;

        arstokesvectorsample_free( art_gv, s0 );
        arstokesvectorsample_free( art_gv, s1 );
    }
}

void arsvlightsample_ll_add_l(
        const ART_GV         * art_gv,
        const ArSVLightSample  * l0,
        const ArSVLightSample  * l1,
              ArSVLightSample  * lr
        )
{
    sps_ss_add_s(
        art_gv,
        & ARSVLIGHTSAMPLE_SV_I( *l0, 0 ),
        & ARSVLIGHTSAMPLE_SV_I( *l1, 0 ),
        & ARSVLIGHTSAMPLE_SV_I( *lr, 0 )
        );

    if ( ARSVLIGHTSAMPLE_POLARISED( *l0 ) || ARSVLIGHTSAMPLE_POLARISED( *l1 ) )
    {
        ArStokesVectorSample  * s0 = arstokesvectorsample_alloc( art_gv );
        ArStokesVectorSample  * s1 = arstokesvectorsample_alloc( art_gv );

        arsvlightsample_lldl_align_and_return_ss(
            art_gv,
            l0,
            l1,
            3.0 DEGREES,
            lr,
            s0,
            s1
            );

        for ( unsigned int i = 1; i < 4; i++ )
            sps_ss_add_s(
                art_gv,
                & ARSVS_I( *s0, i ),
                & ARSVS_I( *s1, i ),
                & ARSVLIGHTSAMPLE_SV_I( *lr, i )
                );

        ARSVLIGHTSAMPLE_POLARISED(*lr) = YES;

        arstokesvectorsample_free( art_gv, s0 );
        arstokesvectorsample_free( art_gv, s1 );
    }
    else
        ARSVLIGHTSAMPLE_POLARISED(*lr) = NO;
}

void arsvlightsample_lld_sloppy_add_l(
        const ART_GV         * art_gv,
        const ArSVLightSample  * l0,
        const ArSVLightSample  * l1,
        const double           d0,
              ArSVLightSample  * lr
        )
{
    sps_ss_add_s(
        art_gv,
        & ARSVLIGHTSAMPLE_SV_I( *l0, 0 ),
        & ARSVLIGHTSAMPLE_SV_I( *l1, 0 ),
        & ARSVLIGHTSAMPLE_SV_I( *lr, 0 )
        );

    if ( ARSVLIGHTSAMPLE_POLARISED( *l0 ) || ARSVLIGHTSAMPLE_POLARISED( *l1 ) )
    {
        ArStokesVectorSample  * s0 = arstokesvectorsample_alloc( art_gv );
        ArStokesVectorSample  * s1 = arstokesvectorsample_alloc( art_gv );

        arsvlightsample_lldl_align_and_return_ss(
              art_gv,
              l0,
              l1,
              d0,
              lr,
              s0,
              s1
            );

        for ( unsigned int i = 1; i < 4; i++ )
            sps_ss_add_s(
                art_gv,
                & ARSVS_I( *s0, i ),
                & ARSVS_I( *s1, i ),
                & ARSVLIGHTSAMPLE_SV_I( *lr, i )
                );

        ARSVLIGHTSAMPLE_POLARISED(*lr) = YES;

        arstokesvectorsample_free( art_gv, s0 );
        arstokesvectorsample_free( art_gv, s1 );
    }
    else
        ARSVLIGHTSAMPLE_POLARISED(*lr) = NO;
}

void arsvlightsample_ll_sub_l(
        const ART_GV         * art_gv,
        const ArSVLightSample  * l0,
        const ArSVLightSample  * l1,
              ArSVLightSample  * lr
        )
{
    sps_ss_sub_s(
        art_gv,
        & ARSVLIGHTSAMPLE_SV_I( *l0, 0 ),
        & ARSVLIGHTSAMPLE_SV_I( *l1, 0 ),
        & ARSVLIGHTSAMPLE_SV_I( *lr, 0 )
        );

    if ( ARSVLIGHTSAMPLE_POLARISED( *l0 ) || ARSVLIGHTSAMPLE_POLARISED( *l1 ) )
    {
        ArStokesVectorSample  * s0 = arstokesvectorsample_alloc( art_gv );
        ArStokesVectorSample  * s1 = arstokesvectorsample_alloc( art_gv );

        arsvlightsample_lldl_align_and_return_ss(
            art_gv,
            l0,
            l1,
            3.0 DEGREES,
            lr,
            s0,
            s1
            );

        for ( unsigned int i = 1; i < 4; i++ )
            sps_ss_sub_s(
                art_gv,
                & ARSVS_I( *s0, i ),
                & ARSVS_I( *s1, i ),
                & ARSVLIGHTSAMPLE_SV_I( *lr, i )
                );

        ARSVLIGHTSAMPLE_POLARISED(*lr) = YES;

        arstokesvectorsample_free( art_gv, s0 );
        arstokesvectorsample_free( art_gv, s1 );
    }
    else
        ARSVLIGHTSAMPLE_POLARISED(*lr) = NO;
}

/* ---------------------------------------------------------------------------

    'arsvlightsample_d_mul_l'
    'arsvlightsample_ld_mul_l'

    Multiplies a given light value by a scalar.

------------------------------------------------------------------------aw- */

void arsvlightsample_d_mul_l(
        const ART_GV         * art_gv,
        const double           d0,
              ArSVLightSample  * lr
        )
{
    sps_d_mul_s(
        art_gv,
        d0,
        & ARSVLIGHTSAMPLE_SV_I( *lr, 0 )
        );

    if ( ARSVLIGHTSAMPLE_POLARISED( *lr ) )
    {
        for ( unsigned int i = 1; i < 4; i++ )
            sps_d_mul_s(
                art_gv,
                d0,
                & ARSVLIGHTSAMPLE_SV_I( *lr, i )
                );
    }
}

void arsvlightsample_dl_mul_l(
        const ART_GV         * art_gv,
        const double           d0,
        const ArSVLightSample  * l0,
              ArSVLightSample  * lr
        )
{
    ASSERT_VALID_SV_LIGHT_SAMPLE(l0)
    ASSERT_ALLOCATED_SV_LIGHT_SAMPLE(lr)

    sps_ds_mul_s(
        art_gv,
        d0,
        & ARSVLIGHTSAMPLE_SV_I( *l0, 0 ),
        & ARSVLIGHTSAMPLE_SV_I( *lr, 0 )
        );

    if ( ARSVLIGHTSAMPLE_POLARISED( *l0 ) )
    {
        for ( unsigned int i = 1; i < 4; i++ )
            sps_ds_mul_s(
                art_gv,
                d0,
                & ARSVLIGHTSAMPLE_SV_I( *l0, i ),
                & ARSVLIGHTSAMPLE_SV_I( *lr, i )
                );

        ARSVLIGHTSAMPLE_REFFRAME( *lr ) = ARSVLIGHTSAMPLE_REFFRAME( * l0 );
        ARSVLIGHTSAMPLE_POLARISED( *lr ) = YES;
    }
    else
        ARSVLIGHTSAMPLE_POLARISED( *lr ) = NO;

    ASSERT_VALID_SV_LIGHT_SAMPLE(lr)
}

/* ---------------------------------------------------------------------------

    'arsvlightsample_i_mul_l'

    Scale the radiant intensity of a light ray by a wavelength-dependent
    scalar which is (not entirely unreasonably, but not 100% intuitively
    either) encoded as a light intensity.

------------------------------------------------------------------------aw- */

void arsvlightsample_i_mul_l(
        const ART_GV                * art_gv,
        const ArLightIntensitySample  * i0,
              ArSVLightSample         * lr
        )
{
    if ( ARSVLIGHTSAMPLE_POLARISED( *lr ) )
    {
        for ( unsigned int i = 0; i < 4; i++ )
            sps_s_mul_s(
                art_gv,
                i0,
                & ARSVLIGHTSAMPLE_SV_I( *lr, i )
                );
    }
    else
        sps_s_mul_s(
            art_gv,
            i0,
            & ARSVLIGHTSAMPLE_SV_I( *lr, 0 )
            );
}

/* ---------------------------------------------------------------------------

    'arsvlightsample_l_mul_l'
    'arsvlightsample_ll_mul_l'

    Multiplies the intensity of two light structs with each other.

    CAUTION: this operation should NEVER be used in a physically-based
    simulation since it has no obvious correspondence with any physical
    interaction or operation. If you were thinking of using it, you probably
    meant 'arattenuationsample_a_mul_l'.

------------------------------------------------------------------------aw- */

void arsvlightsample_l_mul_l(
        const ART_GV         * art_gv,
        const ArSVLightSample  * l0,
              ArSVLightSample  * lr
        )
{
    ART_ERRORHANDLING_WARNING(
        "arsvlightsample_l_mul_l is not a physically meaningful operation on two "
        "light operands"
        );

    sps_s_mul_s(
        art_gv,
        & ARSVLIGHTSAMPLE_SV_I( *l0, 0 ),
        & ARSVLIGHTSAMPLE_SV_I( *lr, 0 )
        );

    ARSVLIGHTSAMPLE_POLARISED( *lr ) = NO;
}

void arsvlightsample_ll_mul_l(
        const ART_GV         * art_gv,
        const ArSVLightSample  * l0,
        const ArSVLightSample  * l1,
              ArSVLightSample  * lr
        )
{
    ART_ERRORHANDLING_WARNING(
        "arsvlightsample_ll_mul_l is not a physically meaningful operation on two "
        "light operands"
        );

    sps_ss_mul_s(
        art_gv,
        & ARSVLIGHTSAMPLE_SV_I( *l0, 0 ),
        & ARSVLIGHTSAMPLE_SV_I( *l1, 0 ),
        & ARSVLIGHTSAMPLE_SV_I( *lr, 0 )
        );

    ARSVLIGHTSAMPLE_POLARISED( *lr ) = NO;
}

/* ---------------------------------------------------------------------------

    'arsvlightsample_d_div_l'
    'arsvlightsample_dl_div_l'

    Multiplies a given light value by a scalar.

------------------------------------------------------------------------aw- */

void arsvlightsample_d_div_l(
        const ART_GV         * art_gv,
        const double           d0,
              ArSVLightSample  * lr
        )
{
    sps_d_mul_s(
        art_gv,
        d0,
        & ARSVLIGHTSAMPLE_SV_I( *lr, 0 )
        );

    if ( ARSVLIGHTSAMPLE_POLARISED( *lr ) )
    {
        for ( unsigned int i = 1; i < 4; i++ )
            sps_d_mul_s(
                art_gv,
                d0,
                & ARSVLIGHTSAMPLE_SV_I( *lr, i )
                );
    }
}

void arsvlightsample_dl_div_l(
        const ART_GV         * art_gv,
        const double           d0,
        const ArSVLightSample  * l0,
              ArSVLightSample  * lr
        )
{
    sps_ds_div_s(
        art_gv,
        d0,
        & ARSVLIGHTSAMPLE_SV_I( *l0, 0 ),
        & ARSVLIGHTSAMPLE_SV_I( *lr, 0 )
        );

    if ( ARSVLIGHTSAMPLE_POLARISED( *l0 ) )
    {
        for ( unsigned int i = 1; i < 4; i++ )
            sps_ds_div_s(
                art_gv,
                d0,
                & ARSVLIGHTSAMPLE_SV_I( *l0, i ),
                & ARSVLIGHTSAMPLE_SV_I( *lr, i )
                );

        ARSVLIGHTSAMPLE_REFFRAME( *lr ) = ARSVLIGHTSAMPLE_REFFRAME( * l0 );
        ARSVLIGHTSAMPLE_POLARISED( *lr ) = YES;
    }
    else
        ARSVLIGHTSAMPLE_POLARISED( *lr ) = NO;
}

void arsvlightsample_dl_pow_l(
        const ART_GV         * art_gv,
        const double           d0,
        const ArSVLightSample  * l0,
              ArSVLightSample  * lr
        )
{
    sps_ds_pow_s(
        art_gv,
        d0,
        & ARSVLIGHTSAMPLE_SV_I( * l0, 0 ),
        & ARSVLIGHTSAMPLE_SV_I( * lr, 0 )
        );

    if ( ARSVLIGHTSAMPLE_POLARISED( *l0 ) )
    {
        for ( int i = 1; i < 4; i++ )
            sps_ds_pow_s(
                art_gv,
                d0,
                & ARSVLIGHTSAMPLE_SV_I( * l0, i ),
                & ARSVLIGHTSAMPLE_SV_I( * lr, i )
                );

        ARSVLIGHTSAMPLE_POLARISED( *lr ) = YES;
    }
    else
        ARSVLIGHTSAMPLE_POLARISED( *lr ) = NO;
}

void arsvlightsample_ddl_clamp_l(
        const ART_GV         * art_gv,
        const double           d0,
        const double           d1,
        const ArSVLightSample  * l0,
              ArSVLightSample  * lr
        )
{
    sps_dds_clamp_s(
        art_gv,
        d0,
        d1,
        & ARSVLIGHTSAMPLE_SV_I( * l0, 0 ),
        & ARSVLIGHTSAMPLE_SV_I( * lr, 0 )
        );

    if ( ARSVLIGHTSAMPLE_POLARISED( *l0 ) )
    {
        for ( int i = 1; i < 4; i++ )
            sps_dds_clamp_s(
                art_gv,
                d0,
                d1,
                & ARSVLIGHTSAMPLE_SV_I( * l0, i ),
                & ARSVLIGHTSAMPLE_SV_I( * lr, i )
                );

        ARSVLIGHTSAMPLE_POLARISED( *lr ) = YES;
    }
    else
        ARSVLIGHTSAMPLE_POLARISED( *lr ) = NO;
}

void arsvlightsample_dl_mul_add_l(
        const ART_GV         * art_gv,
        const double           d0,
        const ArSVLightSample  * l0,
              ArSVLightSample  * lr
        )
{
    sps_ds_mul_add_s(
        art_gv,
        d0,
        & ARSVLIGHTSAMPLE_SV_I( * l0, 0 ),
        & ARSVLIGHTSAMPLE_SV_I( * lr, 0 )
        );

    if ( ARSVLIGHTSAMPLE_POLARISED( *l0 ) || ARSVLIGHTSAMPLE_POLARISED( *lr ) )
    {
        ArStokesVectorSample  * s0 = arstokesvectorsample_alloc( art_gv );
        ArStokesVectorSample  * s1 = arstokesvectorsample_alloc( art_gv );

        arsvlightsample_lld_align_and_return_ss(
              art_gv,
              l0,
              lr,
              3.0 DEGREES,
              s0,
              s1
            );

        for ( unsigned int i = 1; i < 4; i++ )
        {
            sps_d_mul_s(
                art_gv,
                d0,
                & ARSVS_I( *s0, i )
                );

            sps_ss_add_s(
                art_gv,
                & ARSVS_I( *s0, i ),
                & ARSVS_I( *s1, i ),
                & ARSVLIGHTSAMPLE_SV_I( * lr, i )
                );
        }

        arstokesvectorsample_free( art_gv, s0 );
        arstokesvectorsample_free( art_gv, s1 );

        ARSVLIGHTSAMPLE_POLARISED(*lr) = YES;
    }
    else
        ARSVLIGHTSAMPLE_POLARISED(*lr) = NO;
}

void arsvlightsample_dld_mul_sloppy_add_l(
        const ART_GV         * art_gv,
        const double           d0,
        const ArSVLightSample  * l0,
        const double           d1,
              ArSVLightSample  * lr
        )
{
    sps_ds_mul_add_s(
        art_gv,
        d0,
        & ARSVLIGHTSAMPLE_SV_I( * l0, 0 ),
        & ARSVLIGHTSAMPLE_SV_I( * lr, 0 )
        );

    if ( ARSVLIGHTSAMPLE_POLARISED( *l0 ) || ARSVLIGHTSAMPLE_POLARISED( *lr ) )
    {
        ArStokesVectorSample  * s0 = arstokesvectorsample_alloc( art_gv );
        ArStokesVectorSample  * s1 = arstokesvectorsample_alloc( art_gv );

        arsvlightsample_lld_align_and_return_ss(
              art_gv,
              l0,
              lr,
              d1,
              s0,
              s1
            );

        for ( unsigned int i = 1; i < 4; i++ )
        {
            sps_d_mul_s(
                art_gv,
                d0,
                & ARSVS_I( *s0, i )
                );

            sps_ss_add_s(
                art_gv,
                & ARSVS_I( *s0, i ),
                & ARSVS_I( *s1, i ),
                & ARSVLIGHTSAMPLE_SV_I( * lr, i )
                );
        }

        arstokesvectorsample_free( art_gv, s0 );
        arstokesvectorsample_free( art_gv, s1 );

        ARSVLIGHTSAMPLE_POLARISED(*lr) = YES;
    }
    else
        ARSVLIGHTSAMPLE_POLARISED(*lr) = NO;
}

void arsvlightsample_dll_interpol_l(
        const ART_GV         * art_gv,
        const double           d0,
        const ArSVLightSample  * l0,
        const ArSVLightSample  * l1,
              ArSVLightSample  * lr
        )
{
    sps_dss_interpol_s(
        art_gv,
        d0,
        & ARSVLIGHTSAMPLE_SV_I( * l0, 0 ),
        & ARSVLIGHTSAMPLE_SV_I( * l1, 0 ),
        & ARSVLIGHTSAMPLE_SV_I( * lr, 0 )
        );

    if ( ARSVLIGHTSAMPLE_POLARISED( *l0 ) )
    {
        for ( int i = 1; i < 4; i++ )
            sps_dss_interpol_s(
                art_gv,
                d0,
                & ARSVLIGHTSAMPLE_SV_I( * l0, i ),
                & ARSVLIGHTSAMPLE_SV_I( * l1, i ),
                & ARSVLIGHTSAMPLE_SV_I( * lr, i )
                );

        ARSVLIGHTSAMPLE_POLARISED( *lr ) = YES;
    }
    else
        ARSVLIGHTSAMPLE_POLARISED( *lr ) = NO;
}

/* ---------------------------------------------------------------------------

    'arsvlightsample_valid_l'

        Performs a sanity check on the given light struct. It first does a
        colour sanity check on the first Stokes component, and if this is
        passed, tests whether

        i_0 <= sqrt( i_1^2 + i_2^2 + i_3^2 )

        holds.


------------------------------------------------------------------------aw- */

unsigned int arsvlightsample_l_valid(
        const ART_GV         * art_gv,
        const ArSVLightSample  * l0
        )
{
    int light_is_valid =
        sps_s_valid(
            art_gv,
            & ARSVLIGHTSAMPLE_SV_I( *l0, 0 )
            );

    if ( ! light_is_valid )
    {
        ART_ERRORHANDLING_WARNING(
            "value/intensity data of first Stokes vector component are invalid"
            );
        return 0;
    }

    if ( ARSVLIGHTSAMPLE_POLARISED( *l0 ) )
    {
        int  sc1_valid =
            sps_s_valid(
                art_gv,
                & ARSVLIGHTSAMPLE_SV_I( *l0, 1 )
                );

        int  sc2_valid =
            sps_s_valid(
                art_gv,
                & ARSVLIGHTSAMPLE_SV_I( *l0, 2 )
                );

        int  sc3_valid =
            sps_s_valid(
                art_gv,
                & ARSVLIGHTSAMPLE_SV_I( *l0, 3 )
                );

        if ( ! ( sc1_valid && sc2_valid && sc3_valid ) )
        {
            ART_ERRORHANDLING_WARNING(
                "value/intensity data of second (%d), third (%d) and/or fourth (%d) Stokes vector component are invalid",
                sc1_valid,
                sc2_valid,
                sc3_valid
                );
            return 0;
        }

        if ( ! arrefframe_rf_valid( art_gv, & ARSVLIGHTSAMPLE_REFFRAME(*l0) ) )
        {
            ART_ERRORHANDLING_WARNING(
                "reference frame is invalid"
                );
            return 0;
        }

        ArSpectralSample  squaredColour[3];

        for ( int i = 0; i < 3; i++ )
        {
            sps_ss_mul_s(
                  art_gv,
                  & ARSVLIGHTSAMPLE_SV_I( *l0, i + 1 ),
                  & ARSVLIGHTSAMPLE_SV_I( *l0, i + 1 ),
                & squaredColour[i]
                );
        }

        sps_s_add_s(
              art_gv,
            & squaredColour[2],
            & squaredColour[0]
            );

        sps_s_add_s(
              art_gv,
            & squaredColour[1],
            & squaredColour[0]
            );
        
        double  ci = sps_si( art_gv, & squaredColour[0], 0 );

        ci = sqrt( ci );

        if ( ci  > ( sps_si( art_gv, & ARSVLIGHTSAMPLE_INTENSITY( *l0 ), 0 ) + 0.00001 ) )
            light_is_valid = NO;

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

unsigned int arsvlightsample_ll_collinear(
        const ART_GV         * art_gv,
        const ArSVLightSample  * l0,
        const ArSVLightSample  * l1
        )
{
    (void) art_gv;
    (void) l0;
    (void) l1;
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
    return 1;
}

void arsvlightsample_l_debugprintf(
        const ART_GV         * art_gv,
        const ArSVLightSample  * l0
        )
{
    if ( ARSVLIGHTSAMPLE_POLARISED( *l0 ) )
    {
        printf( "ArSVLightSample (polarised):\n" );
        arrefframe_rf_debugprintf(
              art_gv,
            & ARSVLIGHTSAMPLE_REFFRAME( *l0 )
            );

        arstokesvectorsample_sv_debugprintf(
              art_gv,
            & ARSVLIGHTSAMPLE_SV( *l0 )
            );

        printf( "\n" );
    }
    else
    {
        printf( "ArSVLightSample (unpolarised): " );
        sps_s_debugprintf(
              art_gv,
              & ARSVLIGHTSAMPLE_INTENSITY(*l0)
            );
    }
}

// ===========================================================================
