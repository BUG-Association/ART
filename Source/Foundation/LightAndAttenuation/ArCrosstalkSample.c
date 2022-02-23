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

#define ART_MODULE_NAME     ArCrosstalkSample

#include "ArCrosstalkSample.h"
#include "FoundationAssertionMacros.h"

//#include "_ArSpectrum_GV.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

ART_LIGHT_AND_ATTENUATION_MODULE_INITIALISATION_FUNCTION( (void) art_gv; )

ART_LIGHT_AND_ATTENUATION_MODULE_NO_SHUTDOWN_FUNCTION_NECESSARY

ArCrosstalkSample  * arcrosstalksample_alloc(
        const ART_GV  * art_gv
        )
{
    (void) art_gv;
    
    ArCrosstalkSample  * x0 = ALLOC( ArCrosstalkSample );

    ASSERT_ALLOCATED_CROSSTALK_SAMPLE( x0 )

    return x0;
}

ArCrosstalkSample  * arcrosstalksample_d_alloc_init(
        const ART_GV  * art_gv,
        const double    d0
        )
{
    ArCrosstalkSample  * x0;

    x0 = arcrosstalksample_alloc( art_gv );

    for ( unsigned int i = 0; i < CROSSTALK_SAMPLE_CHANNELS; ++i )
        ARCROSSTALKSAMPLE_I( *x0, i ) = d0;

    ASSERT_VALID_CROSSTALK_SAMPLE( x0 )

    return x0;
}

void arcrosstalksample_free(
        const ART_GV           * art_gv,
              ArCrosstalkSample  * xr
        )
{
    (void) art_gv;
    
    FREE( xr );
}

ArCrosstalkSample const * arcrosstalksample_none(
        const ART_GV  * art_gv
        )
{
    (void) art_gv;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return 0;
}

void arcrosstalksample_d_init_x(
        const ART_GV           * art_gv,
        const double             d0,
              ArCrosstalkSample  * xr
        )
{
    (void) art_gv;
    
    ASSERT_ALLOCATED_CROSSTALK_SAMPLE( xr )
    ASSERT_NONNEGATIVE_DOUBLE( d0 )

    for ( unsigned int i = 0; i < CROSSTALK_SAMPLE_CHANNELS; ++i )
        ARCROSSTALKSAMPLE_I( *xr, i ) = d0;

    ASSERT_VALID_CROSSTALK_SAMPLE( xr )
}

void arcrosstalksample_x_init_x(
        const ART_GV           * art_gv,
        const ArCrosstalkSample  * x0,
              ArCrosstalkSample  * xr
        )
{
    (void) art_gv;
    
    ASSERT_VALID_CROSSTALK_SAMPLE( x0 )
    ASSERT_ALLOCATED_CROSSTALK_SAMPLE( xr )

    for ( unsigned int i = 0; i < CROSSTALK_SAMPLE_CHANNELS; ++i )
        ARCROSSTALKSAMPLE_I( *xr, i ) = ARCROSSTALKSAMPLE_I( *x0, i );

    ASSERT_VALID_CROSSTALK_SAMPLE( xr )
}

double arcrosstalksample_x_min(
        const ART_GV           * art_gv,
        const ArCrosstalkSample  * x0
        )
{
    (void) art_gv;
    
    ASSERT_VALID_CROSSTALK_SAMPLE( x0 )

    double  min = MATH_HUGE_DOUBLE;

    for ( unsigned int i = 0; i < CROSSTALK_SAMPLE_CHANNELS; ++i )
        if ( min > ARCROSSTALKSAMPLE_I( *x0, i ) )
            min = ARCROSSTALKSAMPLE_I( *x0, i );

    ASSERT_NONNEGATIVE_DOUBLE( min )

    return min;
}

double arcrosstalksample_x_max(
        const ART_GV           * art_gv,
        const ArCrosstalkSample  * x0
        )
{
    (void) art_gv;
    
    ASSERT_VALID_CROSSTALK_SAMPLE( x0 )

    double  max = -MATH_HUGE_DOUBLE;

    for ( unsigned int i = 0; i < CROSSTALK_SAMPLE_CHANNELS; ++i )
        if ( max < ARCROSSTALKSAMPLE_I( *x0, i ) )
            max = ARCROSSTALKSAMPLE_I( *x0, i );

    ASSERT_NONNEGATIVE_DOUBLE( max )

    return max;
}

double arcrosstalksample_x_avg(
        const ART_GV           * art_gv,
        const ArCrosstalkSample  * x0
        )
{
    (void) art_gv;
    
    ASSERT_VALID_CROSSTALK_SAMPLE( x0 )

    double  sum = 0.0;

    for ( unsigned int i = 0; i < CROSSTALK_SAMPLE_CHANNELS; ++i )
        sum += ARCROSSTALKSAMPLE_I( *x0, i );

    sum /= CROSSTALK_SAMPLE_CHANNELS;

    ASSERT_NONNEGATIVE_DOUBLE( sum )

    return sum;
}

double arcrosstalksample_x_norm(
        const ART_GV           * art_gv,
        const ArCrosstalkSample  * x0
        )
{
    (void) art_gv;
    (void) x0;
    
    ASSERT_VALID_CROSSTALK_SAMPLE( x0 )

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return 0.0;
}

void arcrosstalksample_x_add_x(
        const ART_GV           * art_gv,
        const ArCrosstalkSample  * x0,
              ArCrosstalkSample  * xr
        )
{
    (void) art_gv;
    
    ASSERT_VALID_CROSSTALK_SAMPLE( x0 )
    ASSERT_VALID_CROSSTALK_SAMPLE( xr )

    for ( unsigned int i = 0; i < CROSSTALK_SAMPLE_CHANNELS; ++i )
        ARCROSSTALKSAMPLE_I( *xr, i ) += ARCROSSTALKSAMPLE_I( *x0, i );

    ASSERT_VALID_CROSSTALK_SAMPLE( xr )
}

void arcrosstalksample_d_mul_x(
        const ART_GV           * art_gv,
        const double             d0,
              ArCrosstalkSample  * xr
        )
{
    (void) art_gv;
    
    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_VALID_CROSSTALK_SAMPLE( xr )

    for ( unsigned int i = 0; i < CROSSTALK_SAMPLE_CHANNELS; ++i )
        ARCROSSTALKSAMPLE_I( *xr, i ) *= d0;

    ASSERT_VALID_CROSSTALK_SAMPLE( xr )
}

void arcrosstalksample_dx_mul_x(
        const ART_GV           * art_gv,
        const double             d0,
        const ArCrosstalkSample  * x0,
              ArCrosstalkSample  * xr
        )
{
    (void) art_gv;
    
    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_VALID_CROSSTALK_SAMPLE( x0 )
    ASSERT_VALID_CROSSTALK_SAMPLE( xr )

    for ( unsigned int i = 0; i < CROSSTALK_SAMPLE_CHANNELS; ++i )
        ARCROSSTALKSAMPLE_I( *xr, i ) =
            ARCROSSTALKSAMPLE_I( *x0, i ) * d0;

    ASSERT_VALID_CROSSTALK_SAMPLE( xr )
}

void arcrosstalksample_dx_mul_add_x(
        const ART_GV           * art_gv,
        const double             d0,
        const ArCrosstalkSample  * x0,
              ArCrosstalkSample  * xr
        )
{
    (void) art_gv;
    
    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_VALID_CROSSTALK_SAMPLE( x0 )
    ASSERT_VALID_CROSSTALK_SAMPLE( xr )

    for ( unsigned int i = 0; i < CROSSTALK_SAMPLE_CHANNELS; ++i )
        ARCROSSTALKSAMPLE_I( *xr, i ) +=
            ARCROSSTALKSAMPLE_I( *x0, i ) * d0;

    ASSERT_VALID_CROSSTALK_SAMPLE( xr )
}

void arcrosstalksample_xd_pow_x(
        const ART_GV           * art_gv,
        const ArCrosstalkSample  * x0,
        const double             d0,
              ArCrosstalkSample  * xr
        )
{
    (void) art_gv;
    
    ASSERT_VALID_CROSSTALK_SAMPLE( x0 )
    ASSERT_VALID_CROSSTALK_SAMPLE( xr )

    for ( unsigned int i = 0; i < CROSSTALK_SAMPLE_CHANNELS; ++i )
        ARCROSSTALKSAMPLE_I( *xr, i ) =
            m_dd_pow( d0, ARCROSSTALKSAMPLE_I( *x0, i ) );

    ASSERT_VALID_CROSSTALK_SAMPLE( xr )
}

void arcrosstalksample_xd_negpow_x(
        const ART_GV           * art_gv,
        const ArCrosstalkSample  * x0,
        const double             d0,
              ArCrosstalkSample  * xr
        )
{
    (void) art_gv;
    
    ASSERT_VALID_CROSSTALK_SAMPLE( x0 )
    ASSERT_VALID_CROSSTALK_SAMPLE( xr )

    for ( unsigned int i = 0; i < CROSSTALK_SAMPLE_CHANNELS; ++i )
        ARCROSSTALKSAMPLE_I( *xr, i ) =
            m_dd_pow( d0, - ARCROSSTALKSAMPLE_I( *x0, i ) );

    ASSERT_VALID_CROSSTALK_SAMPLE( xr )
}

void arcrosstalksample_dx_pow_x(
        const ART_GV           * art_gv,
        const double             d0,
        const ArCrosstalkSample  * x0,
              ArCrosstalkSample  * xr
        )
{
    (void) art_gv;
    
    ASSERT_VALID_CROSSTALK_SAMPLE( x0 )
    ASSERT_VALID_CROSSTALK_SAMPLE( xr )

    for ( unsigned int i = 0; i < CROSSTALK_SAMPLE_CHANNELS; ++i )
        ARCROSSTALKSAMPLE_I( *xr, i ) =
            m_dd_pow( ARCROSSTALKSAMPLE_I( *x0, i ), d0 );

    ASSERT_VALID_CROSSTALK_SAMPLE( xr )
}

void arcrosstalksample_dx_negpow_x(
        const ART_GV           * art_gv,
        const double             d0,
        const ArCrosstalkSample  * x0,
              ArCrosstalkSample  * xr
        )
{
    (void) art_gv;
    
    ASSERT_VALID_CROSSTALK_SAMPLE( x0 )
    ASSERT_VALID_CROSSTALK_SAMPLE( xr )

    for ( unsigned int i = 0; i < CROSSTALK_SAMPLE_CHANNELS; ++i )
        ARCROSSTALKSAMPLE_I(*xr, i) =
            m_dd_pow( ARCROSSTALKSAMPLE_I( *x0, i ), -d0 );

    ASSERT_VALID_CROSSTALK_SAMPLE( xr )
}

void arcrosstalksample_xx_add_x(
        const ART_GV           * art_gv,
        const ArCrosstalkSample  * x0,
        const ArCrosstalkSample  * x1,
              ArCrosstalkSample  * xr
        )
{
    (void) art_gv;
    
    ASSERT_VALID_CROSSTALK_SAMPLE( x0 )
    ASSERT_VALID_CROSSTALK_SAMPLE( x1 )
    ASSERT_ALLOCATED_CROSSTALK_SAMPLE( xr )

    for ( unsigned int i = 0; i < CROSSTALK_SAMPLE_CHANNELS; ++i )
        ARCROSSTALKSAMPLE_I( *xr, i ) =
            ARCROSSTALKSAMPLE_I( *x0, i ) + ARCROSSTALKSAMPLE_I( *x1, i );

    ASSERT_VALID_CROSSTALK_SAMPLE( xr )
}

void arcrosstalksample_xx_sub_x(
        const ART_GV           * art_gv,
        const ArCrosstalkSample  * x0,
        const ArCrosstalkSample  * x1,
              ArCrosstalkSample  * xr
        )
{
    (void) art_gv;
    
    ASSERT_VALID_CROSSTALK_SAMPLE( x0 )
    ASSERT_VALID_CROSSTALK_SAMPLE( x1 )
    ASSERT_ALLOCATED_CROSSTALK_SAMPLE( xr )

    for ( unsigned int i = 0; i < CROSSTALK_SAMPLE_CHANNELS; ++i )
        ARCROSSTALKSAMPLE_I( *xr, i ) =
            ARCROSSTALKSAMPLE_I( *x0, i ) - ARCROSSTALKSAMPLE_I( *x1, i );

    ASSERT_VALID_CROSSTALK_SAMPLE( xr )
}

void arcrosstalksample_dxx_interpol_x(
        const ART_GV           * art_gv,
        const double             d0,
        const ArCrosstalkSample  * x0,
        const ArCrosstalkSample  * x1,
              ArCrosstalkSample  * xr
        )
{
    (void) art_gv;
    
    ASSERT_UNIT_RANGE_DOUBLE( d0 )
    ASSERT_VALID_CROSSTALK_SAMPLE( x0 )
    ASSERT_VALID_CROSSTALK_SAMPLE( x1 )
    ASSERT_ALLOCATED_CROSSTALK_SAMPLE( xr )

    for ( unsigned int i = 0; i < CROSSTALK_SAMPLE_CHANNELS; ++i )
        ARCROSSTALKSAMPLE_I( *xr, i ) =
             M_INTERPOL(
                ARCROSSTALKSAMPLE_I( *x0, i ),
                ARCROSSTALKSAMPLE_I( *x1, i ),
                d0
                );

    ASSERT_VALID_CROSSTALK_SAMPLE( xr )
}

void arcrosstalksample_ddx_clamp_x(
        const ART_GV           * art_gv,
        const double             d0,
        const double             d1,
        const ArCrosstalkSample  * x0,
              ArCrosstalkSample  * xr
        )
{
    (void) art_gv;
    
    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_NONNEGATIVE_DOUBLE( d1 )
    ASSERT_VALID_CROSSTALK_SAMPLE( x0 )
    ASSERT_ALLOCATED_CROSSTALK_SAMPLE( xr )

    for ( unsigned int i = 0; i < CROSSTALK_SAMPLE_CHANNELS; ++i )
         m_ddd_clamp_d(
              d0,
              d1,
              ARCROSSTALKSAMPLE_I( *x0, i ),
            & ARCROSSTALKSAMPLE_I( *xr, i )
            );

    ASSERT_VALID_CROSSTALK_SAMPLE( xr )
}

void arcrosstalksample_x_complement_from_one_x(
        const ART_GV           * art_gv,
        const ArCrosstalkSample  * x0,
              ArCrosstalkSample  * xr
        )
{
    (void) art_gv;
    
    ASSERT_VALID_CROSSTALK_SAMPLE( x0 )
    ASSERT_ALLOCATED_CROSSTALK_SAMPLE( xr )

    for ( unsigned int i = 0; i < CROSSTALK_SAMPLE_CHANNELS; ++i )
        ARCROSSTALKSAMPLE_I( *xr, i ) =
              1.0
            - ARCROSSTALKSAMPLE_I( *x0, i );

    ASSERT_VALID_CROSSTALK_SAMPLE( xr )
}

void arcrosstalksample_ix_singleband_complement_from_one_x(
        const ART_GV           * art_gv,
        const unsigned int       i0,
        const ArCrosstalkSample  * x0,
              ArCrosstalkSample  * xr
        )
{
    (void) art_gv;
    
    ASSERT_INTEGER_WITHIN_RANGE( i0, 0, CROSSTALK_SAMPLE_CHANNELS )
    ASSERT_VALID_CROSSTALK_SAMPLE( x0 )
    ASSERT_ALLOCATED_CROSSTALK_SAMPLE( xr )

    unsigned int  s = 0;

    for ( unsigned int i = 0; i < CROSSTALK_SAMPLE_CHANNELS; ++i )
    {
        if ( i == i0 )
            for ( unsigned int k = 0; k < (i + 1); ++k )
                ARCROSSTALKSAMPLE_I( *xr, s + k ) =
                      1.0
                    - ARCROSSTALKSAMPLE_I( *x0, s + k );
        else
            for ( unsigned int k = 0; k < (i + 1); ++k )
                ARCROSSTALKSAMPLE_I( *xr, s + k ) = 0.0;

        s += i + 1;
    }

    ASSERT_VALID_CROSSTALK_SAMPLE( xr )
}

void arcrosstalksample_d_cwdiv_x(
        const ART_GV           * art_gv,
        const double             d0,
              ArCrosstalkSample  * xr
        )
{
    (void) art_gv;
    
    ASSERT_NONZERO_DOUBLE( d0 )
    ASSERT_VALID_CROSSTALK_SAMPLE( xr )

    unsigned int s = 0;

    for ( unsigned int i = 0; i < CROSSTALK_SAMPLE_CHANNELS; ++i )
    {
        for ( unsigned int k = 0; k < (i + 1); ++k )
            ARCROSSTALKSAMPLE_I( *xr, s + k ) /= d0;
        s += i + 1;
    }

    ASSERT_VALID_CROSSTALK_SAMPLE( xr )
}

void arcrosstalksample_dix_singleband_mul_copy_x(
        const ART_GV           * art_gv,
        const double             d0,
        const unsigned int       i0,
        const ArCrosstalkSample  * x0,
              ArCrosstalkSample  * xr
        )
{
    (void) art_gv;
    
    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_INTEGER_WITHIN_RANGE( i0, 0, CROSSTALK_SAMPLE_CHANNELS )
    ASSERT_VALID_CROSSTALK_SAMPLE( x0 )
    ASSERT_ALLOCATED_CROSSTALK_SAMPLE( xr )

    unsigned int  s = 0;

    for ( unsigned int i = 0; i < CROSSTALK_SAMPLE_CHANNELS; ++i )
    {
        if ( i == i0 )
            for ( unsigned int k = 0; k < (i + 1); ++k )
                ARCROSSTALKSAMPLE_I( *xr, s + k ) =
                    d0 * ARCROSSTALKSAMPLE_I( *x0, s + k );
        s += i + 1;
    }

    ASSERT_VALID_CROSSTALK_SAMPLE( xr )
}

void arcrosstalksample_inv_x(
        const ART_GV           * art_gv,
              ArCrosstalkSample  * xr
        )
{
    (void) art_gv;
    
    ASSERT_VALID_CROSSTALK_SAMPLE( xr )

    for ( unsigned int i = 0; i < CROSSTALK_SAMPLE_CHANNELS; ++i )
    {
        if ( ARCROSSTALKSAMPLE_I( *xr, i ) != 0.0 )
            ARCROSSTALKSAMPLE_I( *xr, i ) =
                1.0 / ARCROSSTALKSAMPLE_I( *xr, i );
        else
            ARCROSSTALKSAMPLE_I( *xr, i ) = MATH_HUGE_DOUBLE;
    }

    ASSERT_VALID_CROSSTALK_SAMPLE( xr )
}

void arcrosstalksample_x_inv_x(
        const ART_GV           * art_gv,
        const ArCrosstalkSample  * x0,
              ArCrosstalkSample  * xr
        )
{
    (void) art_gv;
    
    ASSERT_VALID_CROSSTALK_SAMPLE( x0 )
    ASSERT_ALLOCATED_CROSSTALK_SAMPLE( xr )

    for ( unsigned int i = 0; i < CROSSTALK_SAMPLE_CHANNELS; ++i )
    {
        if ( ARCROSSTALKSAMPLE_I( *x0, i ) != 0.0 )
            ARCROSSTALKSAMPLE_I( *xr, i ) =
                1.0 / ARCROSSTALKSAMPLE_I( *x0, i );
        else
            ARCROSSTALKSAMPLE_I( *xr, i ) = MATH_HUGE_DOUBLE;
    }

    ASSERT_VALID_CROSSTALK_SAMPLE( xr )
}

unsigned int arcrosstalksample_x_valid(
        const ART_GV           * art_gv,
        const ArCrosstalkSample  * x0
        )
{
    (void) art_gv;
    
    if ( ! x0 )
    {
        ART_ERRORHANDLING_WARNING( "ArCrosstalkSample parameter is not allocated" );
        return 0;
    }

    for ( unsigned int i = 0; i < CROSSTALK_SAMPLE_CHANNELS; ++i )
    {
        if (    ARCROSSTALKSAMPLE_I(*x0, i) < 0.0
             || m_d_isInf( ARCROSSTALKSAMPLE_I( *x0, i ) )
             || m_d_isNaN( ARCROSSTALKSAMPLE_I( *x0, i ) ) )
        {
            ART_ERRORHANDLING_WARNING( "at least one component of ArCrosstalkSample data is invalid (negative, NaN or Inf)" );
            return 0;
        }
    }

    return 1;
}

void arcrosstalksample_x_mathematicaprintf(
        const ART_GV           * art_gv,
        const ArCrosstalkSample  * x0
        )
{
    (void) art_gv;
    
    const unsigned int  channels = CROSSTALK_SAMPLE_CHANNELS;

    printf( "ArCrosstalkSample %dx%d { \n", channels, channels );

    for ( unsigned int y_ci = 0; y_ci < channels; ++y_ci )
    {
        printf( "{ ");

        for ( unsigned int x_ci = 0; x_ci < channels; ++x_ci )
        {
            printf( "%6.4f", ARCROSSTALKSAMPLE_XY( *x0, x_ci, y_ci ) );

            if ( x_ci < channels - 1 )
                printf(",");
        }

        if ( y_ci < channels - 1 )
            printf(" },\n");
        else
            printf(" }");
    }

    printf(" }\n");

    fflush(stdout);
}

void arcrosstalksample_s_mul_x(
        const ART_GV             * art_gv,
        const ArSpectralSample   * v0,
              ArCrosstalkSample  * xr
        )
{
    (void) art_gv;
    (void) v0;
    (void) xr;
    
    ART__CODE_IS_NOT_TESTED__EXIT_WITH_ERROR

}

void crosstalk_to_crosstalksample(
        const ART_GV              * art_gv,
        const ArCrosstalk         * x0,
              ArCrosstalkSample     * hxr
        )
{
    (void) art_gv;
    (void) x0;
    (void) hxr;
    
    ASSERT_VALID_CROSSTALK( x0 )
    ASSERT_ALLOCATED_CROSSTALK_SAMPLE( hxr )
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    ASSERT_VALID_CROSSTALK_SAMPLE( hxr )
}

// ===========================================================================
