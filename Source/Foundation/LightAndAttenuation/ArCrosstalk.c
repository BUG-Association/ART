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

#define ART_MODULE_NAME     ArCrosstalk

#include "ArCrosstalk.h"
#include "FoundationAssertionMacros.h"
#include "ColourAndLightSubsystem.h"

#include "_ArSpectrum_GV.h"

typedef struct ArCrosstalk_GV
{
    unsigned int  crosstalk_channels;
}
ArCrosstalk_GV;

ART_MODULE_INITIALISATION_FUNCTION
(
    ArCrosstalk_GV  * arcrosstalk_gv;

    arcrosstalk_gv = ALLOC(ArCrosstalk_GV);

    //   We intentionally do not initialise anything in the
    //   arcrosstalk_gv struct here; this is done in the
    //   light and attenuation init routine later on anyway, and
    //   having the same code in two places is always a bad idea.

    art_gv->arcrosstalk_gv = arcrosstalk_gv;

    INITIALISE_LIGHT_AND_ATTENUATION_MODULE( ArCrosstalk );
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    FREE( art_gv->arcrosstalk_gv );
)

#define SPC_CHANNELS        art_gv->arspectrum_gv->number_of_channels
#define CROSSTALK_CHANNELS  art_gv->arcrosstalk_gv->crosstalk_channels

ART_LIGHT_AND_ATTENUATION_MODULE_INITIALISATION_FUNCTION
(
    CROSSTALK_CHANNELS = (( SPC_CHANNELS * ( SPC_CHANNELS - 1 )) / 2 );
)

ART_LIGHT_AND_ATTENUATION_MODULE_NO_SHUTDOWN_FUNCTION_NECESSARY

ArCrosstalk  * arcrosstalk_alloc(
        const ART_GV  * art_gv
        )
{
    ArCrosstalk  * x0 = ALLOC( ArCrosstalk );

    x0->c = ALLOC_ARRAY( double, CROSSTALK_CHANNELS );

    ASSERT_ALLOCATED_CROSSTALK( x0 )

    return x0;
}

ArCrosstalk  * arcrosstalk_d_alloc_init(
        const ART_GV  * art_gv,
        const double    d0
        )
{
    ArCrosstalk  * x0;

    x0 = arcrosstalk_alloc( art_gv );

    for ( unsigned int i = 0; i < CROSSTALK_CHANNELS; i++ )
        ARCROSSTALK_I( *x0, i ) = d0;

    ASSERT_VALID_CROSSTALK( x0 )

    return x0;
}

void arcrosstalk_free(
        const ART_GV       * art_gv,
              ArCrosstalk  * xr
        )
{
    (void) art_gv;
    
    FREE(xr->c);
    FREE(xr);
}

ArCrosstalk const * arcrosstalk_none(
        const ART_GV  * art_gv
        )
{
    (void) art_gv;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return 0;
}

void arcrosstalk_d_init_x(
        const ART_GV       * art_gv,
        const double         d0,
              ArCrosstalk  * xr
        )
{
    ASSERT_ALLOCATED_CROSSTALK( xr )
    ASSERT_NONNEGATIVE_DOUBLE( d0 )

    for ( unsigned int i = 0; i < CROSSTALK_CHANNELS; i++ )
        ARCROSSTALK_I( *xr, i) = d0;

    ASSERT_VALID_CROSSTALK( xr )
}

void arcrosstalk_x_init_x(
        const ART_GV       * art_gv,
        const ArCrosstalk  * x0,
              ArCrosstalk  * xr
        )
{
    ASSERT_VALID_CROSSTALK( x0 )
    ASSERT_ALLOCATED_CROSSTALK( xr )

    for ( unsigned int i = 0; i < CROSSTALK_CHANNELS; i++ )
        ARCROSSTALK_I( *xr, i) = ARCROSSTALK_I( *x0, i);

    ASSERT_VALID_CROSSTALK( xr )
}

double arcrosstalk_x_min(
        const ART_GV       * art_gv,
        const ArCrosstalk  * x0
        )
{
    ASSERT_VALID_CROSSTALK( x0 )

    double  min = MATH_HUGE_DOUBLE;

    for ( unsigned int i = 0; i < CROSSTALK_CHANNELS; i++ )
        if ( min > ARCROSSTALK_I( *x0, i) )
            min = ARCROSSTALK_I( *x0, i);

    ASSERT_NONNEGATIVE_DOUBLE( min )

    return min;
}

double arcrosstalk_x_max(
        const ART_GV       * art_gv,
        const ArCrosstalk  * x0
        )
{
    ASSERT_VALID_CROSSTALK( x0 )

    double  max = -MATH_HUGE_DOUBLE;

    for ( unsigned int i = 0; i < CROSSTALK_CHANNELS; i++ )
        if ( max < ARCROSSTALK_I( *x0, i) )
            max = ARCROSSTALK_I( *x0, i);

    ASSERT_NONNEGATIVE_DOUBLE( max )

    return max;
}

double arcrosstalk_x_avg(
        const ART_GV       * art_gv,
        const ArCrosstalk  * x0
        )
{
    ASSERT_VALID_CROSSTALK( x0 )

    double  sum = 0.0;

    for ( unsigned int i = 0; i < CROSSTALK_CHANNELS; i++ )
        sum += ARCROSSTALK_I( *x0, i);

    sum /= CROSSTALK_CHANNELS;

    ASSERT_NONNEGATIVE_DOUBLE( sum )

    return sum;
}

double arcrosstalk_x_norm(
        const ART_GV       * art_gv,
        const ArCrosstalk  * x0
        )
{
    (void) art_gv;
    (void) x0;
//    ASSERT_VALID_CROSSTALK( x0 )

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return 0.0;
}

void arcrosstalk_x_add_x(
        const ART_GV       * art_gv,
        const ArCrosstalk  * x0,
              ArCrosstalk  * xr
        )
{
    ASSERT_VALID_CROSSTALK( x0 )
    ASSERT_VALID_CROSSTALK( xr )

    for ( unsigned int i = 0; i < CROSSTALK_CHANNELS; i++ )
        ARCROSSTALK_I(*xr, i) += ARCROSSTALK_I(*x0, i);

    ASSERT_VALID_CROSSTALK( xr )
}

void arcrosstalk_d_mul_x(
        const ART_GV       * art_gv,
        const double         d0,
              ArCrosstalk  * xr
        )
{
    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_VALID_CROSSTALK( xr )

    for ( unsigned int i = 0; i < CROSSTALK_CHANNELS; i++ )
        ARCROSSTALK_I(*xr, i) *= d0;

    ASSERT_VALID_CROSSTALK( xr )
}

void arcrosstalk_dx_mul_x(
        const ART_GV       * art_gv,
        const double         d0,
        const ArCrosstalk  * x0,
              ArCrosstalk  * xr
        )
{
    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_VALID_CROSSTALK( x0 )
    ASSERT_VALID_CROSSTALK( xr )

    for ( unsigned int i = 0; i < CROSSTALK_CHANNELS; i++ )
        ARCROSSTALK_I(*xr, i) =
            ARCROSSTALK_I(*x0, i) * d0;

    ASSERT_VALID_CROSSTALK( xr )
}

void arcrosstalk_dx_mul_add_x(
        const ART_GV       * art_gv,
        const double         d0,
        const ArCrosstalk  * x0,
              ArCrosstalk  * xr
        )
{
    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_VALID_CROSSTALK( x0 )
    ASSERT_VALID_CROSSTALK( xr )

    for ( unsigned int i = 0; i < CROSSTALK_CHANNELS; i++ )
        ARCROSSTALK_I(*xr, i) +=
            ARCROSSTALK_I(*x0, i) * d0;

    ASSERT_VALID_CROSSTALK( xr )
}

void arcrosstalk_xd_pow_x(
        const ART_GV       * art_gv,
        const ArCrosstalk  * x0,
        const double         d0,
              ArCrosstalk  * xr
        )
{
    ASSERT_VALID_CROSSTALK( x0 )
    ASSERT_VALID_CROSSTALK( xr )

    for ( unsigned int i = 0; i < CROSSTALK_CHANNELS; i++ )
        ARCROSSTALK_I(*xr, i) =
            m_dd_pow( d0, ARCROSSTALK_I(*x0, i) );

    ASSERT_VALID_CROSSTALK( xr )
}

void arcrosstalk_xd_negpow_x(
        const ART_GV       * art_gv,
        const ArCrosstalk  * x0,
        const double         d0,
              ArCrosstalk  * xr
        )
{
    ASSERT_VALID_CROSSTALK( x0 )
    ASSERT_VALID_CROSSTALK( xr )

    for ( unsigned int i = 0; i < CROSSTALK_CHANNELS; i++ )
        ARCROSSTALK_I(*xr, i) =
            m_dd_pow( d0, - ARCROSSTALK_I(*x0, i) );

    ASSERT_VALID_CROSSTALK( xr )
}

void arcrosstalk_dx_pow_x(
        const ART_GV       * art_gv,
        const double         d0,
        const ArCrosstalk  * x0,
              ArCrosstalk  * xr
        )
{
    ASSERT_VALID_CROSSTALK( x0 )
    ASSERT_VALID_CROSSTALK( xr )

    for ( unsigned int i = 0; i < CROSSTALK_CHANNELS; i++ )
        ARCROSSTALK_I(*xr, i) =
            m_dd_pow( ARCROSSTALK_I(*x0, i), d0 );

    ASSERT_VALID_CROSSTALK( xr )
}

void arcrosstalk_dx_negpow_x(
        const ART_GV       * art_gv,
        const double         d0,
        const ArCrosstalk  * x0,
              ArCrosstalk  * xr
        )
{
    ASSERT_VALID_CROSSTALK( x0 )
    ASSERT_VALID_CROSSTALK( xr )

    for ( unsigned int i = 0; i < CROSSTALK_CHANNELS; i++ )
        ARCROSSTALK_I(*xr, i) =
            m_dd_pow( ARCROSSTALK_I(*x0, i), -d0 );

    ASSERT_VALID_CROSSTALK( xr )
}

void arcrosstalk_xx_add_x(
        const ART_GV       * art_gv,
        const ArCrosstalk  * x0,
        const ArCrosstalk  * x1,
              ArCrosstalk  * xr
        )
{
    ASSERT_VALID_CROSSTALK( x0 )
    ASSERT_VALID_CROSSTALK( x1 )
    ASSERT_ALLOCATED_CROSSTALK( xr )

    for ( unsigned int i = 0; i < CROSSTALK_CHANNELS; i++ )
        ARCROSSTALK_I(*xr, i) =
            ARCROSSTALK_I(*x0, i) + ARCROSSTALK_I(*x1, i);

    ASSERT_VALID_CROSSTALK( xr )
}

void arcrosstalk_xx_sub_x(
        const ART_GV       * art_gv,
        const ArCrosstalk  * x0,
        const ArCrosstalk  * x1,
              ArCrosstalk  * xr
        )
{
    ASSERT_VALID_CROSSTALK( x0 )
    ASSERT_VALID_CROSSTALK( x1 )
    ASSERT_ALLOCATED_CROSSTALK( xr )

    for ( unsigned int i = 0; i < CROSSTALK_CHANNELS; i++ )
        ARCROSSTALK_I(*xr, i) =
            ARCROSSTALK_I(*x0, i) - ARCROSSTALK_I(*x1, i);

    ASSERT_VALID_CROSSTALK( xr )
}

void arcrosstalk_dxx_interpol_x(
        const ART_GV       * art_gv,
        const double         d0,
        const ArCrosstalk  * x0,
        const ArCrosstalk  * x1,
              ArCrosstalk  * xr
        )
{
    ASSERT_UNIT_RANGE_DOUBLE( d0 )
    ASSERT_VALID_CROSSTALK( x0 )
    ASSERT_VALID_CROSSTALK( x1 )
    ASSERT_ALLOCATED_CROSSTALK( xr )

    for ( unsigned int i = 0; i < CROSSTALK_CHANNELS; i++ )
        ARCROSSTALK_I(*xr, i) =
             M_INTERPOL(
                ARCROSSTALK_I(*x0, i),
                ARCROSSTALK_I(*x1, i),
                d0
                );

    ASSERT_VALID_CROSSTALK( xr )
}

void arcrosstalk_ddx_clamp_x(
        const ART_GV       * art_gv,
        const double         d0,
        const double         d1,
        const ArCrosstalk  * x0,
              ArCrosstalk  * xr
        )
{
    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_NONNEGATIVE_DOUBLE( d1 )
    ASSERT_VALID_CROSSTALK( x0 )
    ASSERT_ALLOCATED_CROSSTALK( xr )

    for ( unsigned int i = 0; i < CROSSTALK_CHANNELS; i++ )
         m_ddd_clamp_d(
              d0,
              d1,
              ARCROSSTALK_I(*x0, i),
            & ARCROSSTALK_I(*xr, i)
            );

    ASSERT_VALID_CROSSTALK( xr )
}

void arcrosstalk_x_complement_from_one_x(
        const ART_GV       * art_gv,
        const ArCrosstalk  * x0,
              ArCrosstalk  * xr
        )
{
    ASSERT_VALID_CROSSTALK( x0 )
    ASSERT_ALLOCATED_CROSSTALK( xr )

    for ( unsigned int i = 0; i < CROSSTALK_CHANNELS; i++ )
        ARCROSSTALK_I(*xr, i) =
              1.0
            - ARCROSSTALK_I(*x0, i);

    ASSERT_VALID_CROSSTALK( xr )
}

void arcrosstalk_ix_singleband_complement_from_one_x(
        const ART_GV       * art_gv,
        const unsigned int   i0,
        const ArCrosstalk  * x0,
              ArCrosstalk  * xr
        )
{
    ASSERT_INTEGER_WITHIN_RANGE( i0, 0, CROSSTALK_CHANNELS )
    ASSERT_VALID_CROSSTALK( x0 )
    ASSERT_ALLOCATED_CROSSTALK( xr )

    unsigned int  s = 0;

    for ( unsigned int i = 0; i < SPC_CHANNELS; i++ )
    {
        if ( i == i0 )
            for ( unsigned int k = 0; k < (i + 1); k++ )
                ARCROSSTALK_I(*xr, s + k) =
                      1.0
                    - ARCROSSTALK_I(*x0, s + k);
        else
            for ( unsigned int k = 0; k < (i + 1); k++ )
                ARCROSSTALK_I(*xr, s + k) = 0.0;

        s += i + 1;
    }

    ASSERT_VALID_CROSSTALK( xr )
}

void arcrosstalk_d_cwdiv_x(
        const ART_GV       * art_gv,
        const double         d0,
              ArCrosstalk  * xr
        )
{
    ASSERT_NONZERO_DOUBLE( d0 )
    ASSERT_VALID_CROSSTALK( xr )

    unsigned int s = 0;

    for ( unsigned int i = 0; i < SPC_CHANNELS; i++ )
    {
        for ( unsigned int k = 0; k < (i + 1); k++ )
            ARCROSSTALK_I(*xr, s + k) /= d0;
        s += i + 1;
    }

    ASSERT_VALID_CROSSTALK( xr )
}

void arcrosstalk_dix_singleband_mul_copy_x(
        const ART_GV       * art_gv,
        const double         d0,
        const unsigned int   i0,
        const ArCrosstalk  * x0,
              ArCrosstalk  * xr
        )
{
    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_INTEGER_WITHIN_RANGE( i0, 0, CROSSTALK_CHANNELS )
    ASSERT_VALID_CROSSTALK( x0 )
    ASSERT_ALLOCATED_CROSSTALK( xr )

    unsigned int  s = 0;

    for ( unsigned int i = 0; i < SPC_CHANNELS; i++ )
    {
        if ( i == i0 )
            for ( unsigned int k = 0; k < (i + 1); k++ )
                ARCROSSTALK_I(*xr, s + k) =
                    d0 * ARCROSSTALK_I(*x0, s + k);
        s += i + 1;
    }

    ASSERT_VALID_CROSSTALK( xr )
}

void arcrosstalk_inv_x(
        const ART_GV       * art_gv,
              ArCrosstalk  * xr
        )
{
    ASSERT_VALID_CROSSTALK( xr )

    for ( unsigned int i = 0; i < CROSSTALK_CHANNELS; i++ )
    {
        if ( ARCROSSTALK_I(*xr, i) != 0.0 )
            ARCROSSTALK_I(*xr, i) =
                1.0 / ARCROSSTALK_I(*xr, i);
        else
            ARCROSSTALK_I(*xr, i) = MATH_HUGE_DOUBLE;
    }

    ASSERT_VALID_CROSSTALK( xr )
}

void arcrosstalk_x_inv_x(
        const ART_GV       * art_gv,
        const ArCrosstalk  * x0,
              ArCrosstalk  * xr
        )
{
    ASSERT_VALID_CROSSTALK( x0 )
    ASSERT_ALLOCATED_CROSSTALK( xr )

    for ( unsigned int i = 0; i < CROSSTALK_CHANNELS; i++ )
    {
        if ( ARCROSSTALK_I(*x0, i) != 0.0 )
            ARCROSSTALK_I(*xr, i) =
                1.0 / ARCROSSTALK_I(*x0, i);
        else
            ARCROSSTALK_I(*xr, i) = MATH_HUGE_DOUBLE;
    }

    ASSERT_VALID_CROSSTALK( xr )
}

double arcrosstalk_wl_wl_value(
        const ART_GV       * art_gv,
        const ArWavelength * wi,
        const ArWavelength * wo
        )
{
    const ArSpectralSampleSplattingData  * s0 =
        ART_SAMPLE_SPLATTING_DATA;
    
    int  cidx_i = -1;
    int  cidx_o = -1;
    
    double  wil = ARWL_WI(*wi,0);
    double  wol = ARWL_WI(*wo,0);
    
    if (   ( wil > s0->spectralRangeStart )
        && ( wil < s0->spectralRangeEnd ) )
    {
        cidx_i =
            ( wil - s0->spectralRangeStart ) / s0->channelWidth;
    }
    
    if (   ( wol > s0->spectralRangeStart )
        && ( wol < s0->spectralRangeEnd ) )
    {
        cidx_o =
            ( wol - s0->spectralRangeStart ) / s0->channelWidth;
    }

    double  result = 0.0;
    
    return result;
}

unsigned int arcrosstalk_x_valid(
        const ART_GV       * art_gv,
        const ArCrosstalk  * x0
        )
{
    if ( ! x0 )
    {
        ART_ERRORHANDLING_WARNING( "ArCrosstalk parameter is not allocated" );
        return 0;
    }

    for ( unsigned int i = 0; i < CROSSTALK_CHANNELS; i++ )
    {
        if (    ARCROSSTALK_I(*x0, i) < 0.0
             || m_d_isInf( ARCROSSTALK_I(*x0, i) )
             || m_d_isNaN( ARCROSSTALK_I(*x0, i) ) )
        {
            ART_ERRORHANDLING_WARNING( "at least one component of ArCrosstalk data is invalid (negative, NaN or Inf)" );
            return 0;
        }
    }

    return 1;
}

double cx500_integrate_to_spc_channels(
        const ART_GV  * art_gv,
        const ArCrosstalk500 * cx,
        const int       x_ci,
        const int       y_ci
        )
{
    int x0 = (int) NANO_FROM_UNIT(spc_channel_lower_bound( art_gv, x_ci     ));
    int x1 = (int) NANO_FROM_UNIT(spc_channel_lower_bound( art_gv, x_ci + 1 ));
    int y0 = (int) NANO_FROM_UNIT(spc_channel_lower_bound( art_gv, y_ci     ));
    int y1 = (int) NANO_FROM_UNIT(spc_channel_lower_bound( art_gv, y_ci + 1 ));

    x0 -= ARCROSSTALK500_LOWER_BOUND;
    x1 -= ARCROSSTALK500_LOWER_BOUND;
    y0 -= ARCROSSTALK500_LOWER_BOUND;
    y1 -= ARCROSSTALK500_LOWER_BOUND;

    ASSERT_NONNEGATIVE_INTEGER(x0);
    ASSERT_NONNEGATIVE_INTEGER(x1);
    ASSERT_NONNEGATIVE_INTEGER(y0);
    ASSERT_NONNEGATIVE_INTEGER(y1);

    double  result = 0.0;
    double  integrationHeight = x1 -x0;

    for ( int x = x0; x < x1; x++ )
    {
        double  lineResult = 0.0;

        for ( int y = y0; y < x; y++ )
        {
            lineResult += ARCROSSTALK500_XY( *cx, x, y );
        }

        result += lineResult;
    }

    result /= integrationHeight;

    return result;
}

#define RSS2D_SIZE_X(__s)       (__s).stride
#define RSS2D_SIZE_Y(__s)       ( (__s).size / (__s).stride )

/*

The following function is neither pretty nor efficient. All data from the
RSS2D is written to a 1nm-per-cell canvas, from which we then integrate the
values into the ISR-specific ArCrosstalk data structure.

*/

void rss2d_to_crosstalk(
        const ART_GV          * art_gv,
        const ArRSSpectrum2D  * rss2d,
              ArCrosstalk     * ctx
        )
{
    ASSERT_VALID_RSS2D( rss2d )
    ASSERT_ALLOCATED_CROSSTALK( ctx )
    
    ArCrosstalk500* cx500 = cx500_alloc(art_gv);
    rss2d_to_cx500(art_gv, rss2d, cx500);
    
    cx500_to_crosstalk(art_gv, cx500, ctx);
    
    cx500_free(art_gv, cx500);
}

void cx500_to_crosstalk(
        const ART_GV          * art_gv,
        const ArCrosstalk500  * cx500,
              ArCrosstalk     * ctx
        )
{
    unsigned int  cc = SPC_CHANNELS;

    for ( unsigned int x_ci = 0; x_ci < cc; x_ci++ )
        for ( unsigned int y_ci = 0; y_ci < cc; y_ci++ )
            if ( y_ci > x_ci )
            {
                double  rss2d_sample =
                    cx500_integrate_to_spc_channels(
                          art_gv,
                          cx500,
                          x_ci,
                          y_ci
                        );

                ASSERT_NONNEGATIVE_FINITE_DOUBLE( rss2d_sample )

                ARCROSSTALK_SET_XYD( *ctx, x_ci, y_ci, rss2d_sample );
            }
}

void arcrosstalk_x_mathematicaprintf(
        const ART_GV       * art_gv,
        const ArCrosstalk  * x0
        )
{
    const unsigned int  channels = spc_channels( art_gv );

    printf( "ArCrosstalk %dx%d { \n",channels,channels);

    for ( unsigned int y_ci = 0; y_ci < channels; y_ci++ )
    {
        printf( "{ ");

        for ( unsigned int x_ci = 0; x_ci < channels; x_ci++ )
        {
            printf( "%6.4f", ARCROSSTALK_XY( *x0, x_ci, y_ci ) );

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

// ===========================================================================
