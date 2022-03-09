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

#define ART_MODULE_NAME     ArStokesVector

#include "ArStokesVector.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

ArStokesVector * arstokesvector_alloc(
        const ART_GV  * art_gv
        )
{
    ArStokesVector  * newSV = ALLOC( ArStokesVector );

    for ( unsigned int i = 0; i < 4; i++ )
        ARSV_I( *newSV, i )=
            spc_alloc(
                art_gv
                );

    return newSV;
}

void arstokesvector_free(
        const ART_GV          * art_gv,
              ArStokesVector  * svr
        )
{
    for ( unsigned int i = 0; i < 4; i++ )
        spc_free(
            art_gv,
            ARSV_I( *svr, i )
            );

    FREE(svr);
}

void arstokesvector_s_init_unpolarised_sv(
        const ART_GV          * art_gv,
        const ArSpectrum        * c0,
              ArStokesVector  * sr
        )
{
    ASSERT_ALLOCATED_STOKES_VECTOR( sr )

    spc_s_init_s(
        art_gv,
        c0,
        ARSV_I( *sr, 0 )
        );

    for ( unsigned int i = 1; i < 4; i++ )
        spc_d_init_s(
            art_gv,
            0.0,
            ARSV_I( *sr, i )
            );

    ASSERT_VALID_STOKES_VECTOR( sr )
}

void arstokesvector_cccc_init_sv(
        const ART_GV          * art_gv,
        const ArSpectrum        * c0,
        const ArSpectrum        * c1,
        const ArSpectrum        * c2,
        const ArSpectrum        * c3,
              ArStokesVector  * sr
        )
{
    ASSERT_ALLOCATED_STOKES_VECTOR( sr )

    spc_s_init_s(
        art_gv,
        c0,
        ARSV_I( *sr, 0 )
        );

    spc_s_init_s(
        art_gv,
        c1,
        ARSV_I( *sr, 1 )
        );

    spc_s_init_s(
        art_gv,
        c2,
        ARSV_I( *sr, 2 )
        );

    spc_s_init_s(
        art_gv,
        c3,
        ARSV_I( *sr, 3 )
        );

    ASSERT_VALID_STOKES_VECTOR( sr )
}

void arstokesvector_dddd_init_sv(
        const ART_GV          * art_gv,
              double            d0,
              double            d1,
              double            d2,
              double            d3,
              ArStokesVector  * sr
        )
{
    ASSERT_ALLOCATED_STOKES_VECTOR( sr )

    spc_d_init_s(
        art_gv,
        d0,
        ARSV_I( *sr, 0 )
        );

    spc_d_init_s(
        art_gv,
        d1,
        ARSV_I( *sr, 1 )
        );

    spc_d_init_s(
        art_gv,
        d2,
        ARSV_I( *sr, 2 )
        );

    spc_d_init_s(
        art_gv,
        d3,
        ARSV_I( *sr, 3 )
        );

    ASSERT_VALID_STOKES_VECTOR( sr )
}

void arstokesvector_sv_init_sv(
        const ART_GV          * art_gv,
        const ArStokesVector  * s0,
              ArStokesVector  * sr
        )
{
    ASSERT_VALID_STOKES_VECTOR( s0 )
    ASSERT_ALLOCATED_STOKES_VECTOR( sr )

    for ( unsigned int i = 0; i < 4; i++ )
        spc_s_init_s(
            art_gv,
            ARSV_I( *s0, i ),
            ARSV_I( *sr, i )
            );

    ASSERT_VALID_STOKES_VECTOR( sr )
}

/* ---------------------------------------------------------------------------

    'arstokescomponents_sm_mul_s'

        Multiplies a Stokes vector with a Mueller matrix. This is the
        basic mathematical operation for the simulation of all interactions
        between light and matter. (sounds nice, doesn't it? :-)

------------------------------------------------------------------------aw- */

void arstokesvector_sv_mm_mul_sv(
        const ART_GV           * art_gv,
        const ArStokesVector   * sv0,
        const ArMuellerMatrix  * mm0,
              ArStokesVector   * svr
        )
{
    ASSERT_VALID_STOKES_VECTOR( sv0 );
    ASSERT_VALID_MUELLER_MATRIX( mm0 );

    ArSpectrum  * lc0 = spc_alloc( art_gv );
    ArSpectrum  * lc1 = spc_alloc( art_gv );
    ArSpectrum  * lc2 = spc_alloc( art_gv );

    for ( unsigned int i = 0; i < 4; i++ )
    {
        spc_ss_mul_s(
            art_gv,
            ARSV_I( *sv0, 0 ),
            ARMUELLER_M_I( *mm0, 4*i ),
            lc0
            );

        spc_ss_mul_s(
            art_gv,
            ARSV_I(  *sv0, 1 ),
            ARMUELLER_M_I( *mm0, 4*i + 1 ),
            lc1
            );

        spc_ss_mul_s(
            art_gv,
            ARSV_I(  *sv0, 2 ),
            ARMUELLER_M_I( *mm0, 4*i + 2 ),
            lc2
            );

        spc_ss_mul_s(
            art_gv,
            ARSV_I(  *sv0, 3 ),
            ARMUELLER_M_I( *mm0, 4*i + 3 ),
            ARSV_I(  *svr, i )
            );

        spc_s_add_s(
            art_gv,
            lc0,
            ARSV_I( *svr, i )
            );

        spc_s_add_s(
            art_gv,
            lc1,
            ARSV_I( *svr, i )
            );

        spc_s_add_s(
            art_gv,
            lc2,
            ARSV_I( *svr, i )
            );
    }

    spc_free( art_gv, lc0 );
    spc_free( art_gv, lc1 );
    spc_free( art_gv, lc2 );

    ASSERT_VALID_STOKES_VECTOR( svr );
}

unsigned int arstokesvector_sv_sv_equal(
        const ART_GV          * art_gv,
        const ArStokesVector  * s0,
        const ArStokesVector  * s1
        )
{
    ASSERT_VALID_STOKES_VECTOR( s0 );
    ASSERT_VALID_STOKES_VECTOR( s1 );

    unsigned int  result = 1;

    for ( unsigned int i = 0; i < 4; i++ )
    {
        if ( ! spc_ss_equal(
                 art_gv,
                 ARSV_I( *s0, i ),
                 ARSV_I( *s1, i )
                 ))
        {
            result = 0;
            break;
        }
    }

    return result;
}

unsigned int arstokesvector_sv_sv_d_equal(
        const ART_GV          * art_gv,
        const ArStokesVector  * s0,
        const ArStokesVector  * s1,
        const double            max_relative_error
        )
{
    ASSERT_VALID_STOKES_VECTOR( s0 );
    ASSERT_VALID_STOKES_VECTOR( s1 );

    unsigned int  result = 1;

    for ( unsigned int i = 0; i < 4; i++ )
    {
        if ( ! spc_ssd_equal(
                 art_gv,
                 ARSV_I( *s0, i ),
                 ARSV_I( *s1, i ),
                 max_relative_error
                 ))
        {
            result = 0;
            break;
        }
    }

    return result;
}

void arstokesvector_sv_sample_at_wavelength_svs(
        const ART_GV                * art_gv,
        const ArStokesVector        * s0,
        const ArWavelength            w0,
              ArStokesVectorSample  * sr
        )
{
    ASSERT_VALID_STOKES_VECTOR( s0 );
    ASSERT_VALID_WAVELENGTH( & w0 );
    ASSERT_ALLOCATED_STOKES_VECTOR_SAMPLE( sr );
    
    ARSVS_I( *sr, 0 ) =
        SPS(spc_sd_value_at_wavelength( art_gv, ARSV_I( *s0, 0 ), ARWL_WI(w0,0) ));
    ARSVS_I( *sr, 1 ) =
        SPS(spc_sd_value_at_wavelength( art_gv, ARSV_I( *s0, 1 ), ARWL_WI(w0,0) ));
    ARSVS_I( *sr, 2 ) =
        SPS(spc_sd_value_at_wavelength( art_gv, ARSV_I( *s0, 2 ), ARWL_WI(w0,0) ));
    ARSVS_I( *sr, 3 ) =
        SPS(spc_sd_value_at_wavelength( art_gv, ARSV_I( *s0, 3 ), ARWL_WI(w0,0) ));
    
    ASSERT_VALID_STOKES_VECTOR_SAMPLE( sr );
}

unsigned int arstokesvector_sv_valid(
        const ART_GV          * art_gv,
        const ArStokesVector  * sv0
        )
{
    unsigned int result = 1;

    /* -----------------------------------------------------------------------
        Basic colour validity check
    --------------------------------------------------------------------ip- */

    for (unsigned int i = 0; i < 4; i++)
    {
        if ( ! spc_s_valid( art_gv, ARSV_I( *sv0, i ) ) )
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

    ArSpectrum  * temp_0 = spc_alloc( art_gv );
    ArSpectrum  * temp_1 = spc_alloc( art_gv );
    ArSpectrum  * temp_2 = spc_alloc( art_gv );

    spc_ss_mul_s(
        art_gv,
        ARSV_I( *sv0, 1 ),
        ARSV_I( *sv0, 1 ),
        temp_0
        );

    spc_ss_mul_s(
        art_gv,
        ARSV_I( *sv0, 2 ),
        ARSV_I( *sv0, 2 ),
        temp_1
        );

    spc_ss_add_s(
        art_gv,
        temp_0,
        temp_1,
        temp_2
        );

    spc_ss_mul_s(
        art_gv,
        ARSV_I( *sv0, 3 ),
        ARSV_I( *sv0, 3 ),
        temp_1
        );

    spc_ss_add_s(
        art_gv,
        temp_2,
        temp_1,
        temp_0
        );

    spc_sqrt_s(
        art_gv,
        temp_0
        );

    spc_ss_div_s(
        art_gv,
        ARSV_I( *sv0, 0 ),
        temp_0,
        temp_2
        );

    const unsigned int channels_count = spc_channels( art_gv );
    for ( unsigned int channel = 0; channel < channels_count; channel++ )
    {
        double divisor  = spc_si( art_gv, ARSV_I( *sv0, 0 ), channel );
        double dividend = spc_si( art_gv, temp_0, channel );
        double dop      = spc_si( art_gv, temp_2, channel );

        //  We check whether the Degree of polarisation is larger than 1,
        //  but only for non-zero Stokes vectors                           (ip)

        if (    ! ( m_d_is_tiny( dividend ) && m_d_is_tiny( divisor ) )
             &&   ( dop > ( 1.0 + 0.0001 ) ) )
        {
            ART_ERRORHANDLING_WARNING(
                "the Stokes vector is overpolarised (DoP %.4f) in channel %d, "
                "skipping the test for the remaining channels...",
                dop,
                channel
                );
            result = 0;
            break;
        }
    }

    spc_free( art_gv, temp_0 );
    spc_free( art_gv, temp_1 );
    spc_free( art_gv, temp_2 );

    return result;
}

void arstokesvector_sv_debugprintf(
        const ART_GV          * art_gv,
        const ArStokesVector  * sv0
        )
{
    printf( "Stokes colour:\n" );

    for ( unsigned int i = 0; i < 4; i++ )
    {
        printf("SC %d: ",i);

        spc_s_debugprintf(
              art_gv,
              ARSV_I( *sv0, i )
            );
    }
}

// ===========================================================================
