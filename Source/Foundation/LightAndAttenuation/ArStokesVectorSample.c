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

#define ART_MODULE_NAME     ArStokesVectorSample

#include "ArSpectralSample.h"
#include "ArStokesVectorSample.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

ArStokesVectorSample * arstokesvectorsample_alloc(
        const ART_GV  * art_gv
        )
{
    (void) art_gv;
    
    ArStokesVectorSample  * newSV = ALLOC( ArStokesVectorSample );

    return newSV;
}

void arstokesvectorsample_free(
        const ART_GV              * art_gv,
              ArStokesVectorSample  * svr
        )
{
    (void) art_gv;
    
    FREE( svr );
}

void arstokesvectorsample_s_init_depolarised_sv(
        const ART_GV              * art_gv,
        const ArSpectralSample      * c0,
              ArStokesVectorSample  * sr
        )
{
    ASSERT_ALLOCATED_STOKES_VECTOR_SAMPLE( sr )

    sps_s_init_s(
        art_gv,
        c0,
        &ARSVS_I( *sr, 0 )
        );

    for ( unsigned int i = 1; i < 4; i++ )
        sps_d_init_s(
            art_gv,
            0.0,
            &ARSVS_I( *sr, i )
            );

    ASSERT_VALID_STOKES_VECTOR_SAMPLE( sr )
}

void arstokesvectorsample_ssss_init_sv(
        const ART_GV              * art_gv,
        const ArSpectralSample      * c0,
        const ArSpectralSample      * c1,
        const ArSpectralSample      * c2,
        const ArSpectralSample      * c3,
              ArStokesVectorSample  * sr
        )
{
    ASSERT_ALLOCATED_STOKES_VECTOR_SAMPLE( sr )

    sps_s_init_s(
        art_gv,
        c0,
        &ARSVS_I( *sr, 0 )
        );

    sps_s_init_s(
        art_gv,
        c1,
        &ARSVS_I( *sr, 1 )
        );

    sps_s_init_s(
        art_gv,
        c2,
        &ARSVS_I( *sr, 2 )
        );

    sps_s_init_s(
        art_gv,
        c3,
        &ARSVS_I( *sr, 3 )
        );

    ASSERT_VALID_STOKES_VECTOR_SAMPLE( sr )
}

void arstokesvectorsample_dddd_init_sv(
        const ART_GV              * art_gv,
              double                d0,
              double                d1,
              double                d2,
              double                d3,
              ArStokesVectorSample  * sr
        )
{
    ASSERT_ALLOCATED_STOKES_VECTOR_SAMPLE( sr )

    sps_d_init_s(
        art_gv,
        d0,
        &ARSVS_I( *sr, 0 )
        );

    sps_d_init_s(
        art_gv,
        d1,
        &ARSVS_I( *sr, 1 )
        );

    sps_d_init_s(
        art_gv,
        d2,
        &ARSVS_I( *sr, 2 )
        );

    sps_d_init_s(
        art_gv,
        d3,
        &ARSVS_I( *sr, 3 )
        );

    ASSERT_VALID_STOKES_VECTOR_SAMPLE( sr )
}

void arstokesvectorsample_sv_init_sv(
        const ART_GV              * art_gv,
        const ArStokesVectorSample  * s0,
              ArStokesVectorSample  * sr
        )
{
    ASSERT_VALID_STOKES_VECTOR_SAMPLE( s0 )
    ASSERT_ALLOCATED_STOKES_VECTOR_SAMPLE( sr )

    for ( unsigned int i = 0; i < 4; i++ )
        sps_s_init_s(
            art_gv,
            &ARSVS_I( *s0, i ),
            &ARSVS_I( *sr, i )
            );

    ASSERT_VALID_STOKES_VECTOR_SAMPLE( sr )
}

void arstokesvectorsample_d_mul_sv(
        const ART_GV                 * art_gv,
        const double                   d0,
              ArStokesVectorSample   * sr
        )
{
    ASSERT_VALID_FINITE_DOUBLE( d0 );
    ASSERT_VALID_STOKES_VECTOR_SAMPLE( sr );

    for ( unsigned int i = 0; i < 4; i++ )
    {
        sps_d_mul_s(
              art_gv,
              d0,
            & ARSVS_I( *sr, i )
            );
    }

    ASSERT_VALID_STOKES_VECTOR_SAMPLE( sr );
}

void arstokesvectorsample_sv_add_sv(
        const ART_GV                 * art_gv,
        const ArStokesVectorSample   * s0,
              ArStokesVectorSample   * sr
        )
{
    ASSERT_VALID_STOKES_VECTOR_SAMPLE( s0 );
    ASSERT_VALID_STOKES_VECTOR_SAMPLE( sr );

    for ( unsigned int i = 0; i < 4; i++ )
    {
        sps_s_add_s(
              art_gv,
            & ARSVS_I( *s0, i ),
            & ARSVS_I( *sr, i )
            );
    }

    ASSERT_VALID_STOKES_VECTOR_SAMPLE( sr );
}

/* ---------------------------------------------------------------------------

    'arstokescomponents_sm_mul_s'

        Multiplies a Stokes vector with a Mueller matrix. This is the
        basic mathematical operation for the simulation of all interactions
        between light and matter. (sounds nice, doesn't it? :-)

------------------------------------------------------------------------aw- */

void arstokesvectorsample_sv_mm_mul_sv(
        const ART_GV               * art_gv,
        const ArStokesVectorSample   * sv0,
        const ArMuellerMatrixSample  * mm0,
              ArStokesVectorSample   * svr
        )
{
    ASSERT_VALID_STOKES_VECTOR_SAMPLE( sv0 );
    ASSERT_VALID_MUELLER_MATRIX_SAMPLE( mm0 );

    ArSpectralSample  lc0;
    ArSpectralSample  lc1;
    ArSpectralSample  lc2;

    for ( unsigned int i = 0; i < 4; i++ )
    {
        sps_ss_mul_s(
              art_gv,
              &ARSVS_I( *sv0, 0 ),
              ARMUELLER_S_M_I( *mm0, 4*i ),
            & lc0
            );

        sps_ss_mul_s(
              art_gv,
              &ARSVS_I(  *sv0, 1 ),
              ARMUELLER_S_M_I( *mm0, 4*i + 1 ),
            & lc1
            );

        sps_ss_mul_s(
              art_gv,
              &ARSVS_I(  *sv0, 2 ),
              ARMUELLER_S_M_I( *mm0, 4*i + 2 ),
            & lc2
            );

        sps_ss_mul_s(
              art_gv,
              &ARSVS_I(  *sv0, 3 ),
              ARMUELLER_S_M_I( *mm0, 4*i + 3 ),
              &ARSVS_I(  *svr, i )
            );

        sps_s_add_s(
              art_gv,
            & lc0,
              &ARSVS_I( *svr, i )
            );

        sps_s_add_s(
              art_gv,
            & lc1,
              &ARSVS_I( *svr, i )
            );

        sps_s_add_s(
              art_gv,
            & lc2,
              &ARSVS_I( *svr, i )
            );
    }

    ASSERT_VALID_STOKES_VECTOR_SAMPLE( svr );
}

unsigned int arstokesvectorsample_sv_sv_equal(
        const ART_GV              * art_gv,
        const ArStokesVectorSample  * s0,
        const ArStokesVectorSample  * s1
        )
{
    ASSERT_VALID_STOKES_VECTOR_SAMPLE( s0 );
    ASSERT_VALID_STOKES_VECTOR_SAMPLE( s1 );

    unsigned int  result = 1;

    for ( unsigned int i = 0; i < 4; i++ )
    {
        if ( ! sps_ss_equal(
                 art_gv,
                 &ARSVS_I( *s0, i ),
                 &ARSVS_I( *s1, i )
                 ))
        {
            result = 0;
            break;
        }
    }

    return result;
}

unsigned int arstokesvectorsample_sv_sv_d_equal(
        const ART_GV              * art_gv,
        const ArStokesVectorSample  * s0,
        const ArStokesVectorSample  * s1,
        const double                max_relative_error
        )
{
    ASSERT_VALID_STOKES_VECTOR_SAMPLE( s0 );
    ASSERT_VALID_STOKES_VECTOR_SAMPLE( s1 );

    unsigned int  result = 1;

    for ( unsigned int i = 0; i < 4; i++ )
    {
        if ( ! sps_ssd_equal(
                 art_gv,
                 &ARSVS_I( *s0, i ),
                 &ARSVS_I( *s1, i ),
                 max_relative_error
                 ))
        {
            result = 0;
            break;
        }
    }

    return result;
}

unsigned int arstokesvectorsample_sv_valid(
        const ART_GV              * art_gv,
        const ArStokesVectorSample  * sv0
        )
{
    unsigned int result = 1;

    /* -----------------------------------------------------------------------
        Basic colour validity check
    --------------------------------------------------------------------ip- */

    for (unsigned int i = 0; i < 4; i++)
    {
        if ( ! sps_s_valid( art_gv, &ARSVS_I( *sv0, i ) ) )
        {
            ART_ERRORHANDLING_WARNING(
                "the Stokes vector contains invalid colour data in component %d",
                i
                );
            result = 0;
        }
    }

    if ( ! result )
        return 0;

    /* -----------------------------------------------------------------------
        Degree of polarisation must be less or equal to 1.0:

            sqrt(s1^2 + s2^2 + s3^2) / s0 <= 1.0

    --------------------------------------------------------------------ip- */

    ArSpectralSample  temp_0;
    ArSpectralSample  temp_1;
    ArSpectralSample  temp_2;

    sps_ss_mul_s(
          art_gv,
        & ARSVS_I( *sv0, 1 ),
        & ARSVS_I( *sv0, 1 ),
        & temp_0
        );

    sps_ss_mul_s(
          art_gv,
        & ARSVS_I( *sv0, 2 ),
        & ARSVS_I( *sv0, 2 ),
        & temp_1
        );

    sps_ss_add_s(
          art_gv,
        & temp_0,
        & temp_1,
        & temp_2
        );

    sps_ss_mul_s(
          art_gv,
        & ARSVS_I( *sv0, 3 ),
        & ARSVS_I( *sv0, 3 ),
        & temp_1
        );

    sps_ss_add_s(
          art_gv,
        & temp_2,
        & temp_1,
        & temp_0
        );

    sps_sqrt_s(
          art_gv,
        & temp_0
        );

    sps_ss_div_s(
          art_gv,
        & ARSVS_I( *sv0, 0 ),
        & temp_0,
        & temp_2
        );

    double divisor  = sps_si( art_gv, &ARSVS_I( *sv0, 0 ), 0 );
    double dividend = sps_si( art_gv, & temp_0, 0 );
    double dop      = sps_si( art_gv, & temp_2, 0 );

    //  We check whether the Degree of polarisation is larger than 1,
    //  but only for non-zero Stokes vectors                           (ip)

    if (    ! ( m_d_is_tiny( dividend ) && m_d_is_tiny( divisor ) )
         &&   ( dop > ( 1.0 + 0.0001 ) ) )
    {
        ART_ERRORHANDLING_WARNING(
            "the Stokes vector is overpolarised (DoP %.4f) in channel %d, "
            "skipping the test for the remaining channels...",
            dop,
            0
            );
        result = 0;
    }

    return result;
}

void arstokesvectorsample_sv_debugprintf(
        const ART_GV              * art_gv,
        const ArStokesVectorSample  * sv0
        )
{
    printf( "Stokes vector:\n" );

    for ( unsigned int i = 0; i < 4; i++ )
    {
        printf("SV %d: ",i);

        sps_s_debugprintf(
              art_gv,
              &ARSVS_I( *sv0, i )
            );
    }
}

// ===========================================================================
