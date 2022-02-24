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

#define ART_MODULE_NAME     ArCrosstalk500

#include "ArCrosstalk500.h"
#include "FoundationAssertionMacros.h"
#include "ColourAndLightSubsystem.h"

#define SPECTRAL_CHANNELS   ARCROSSTALK500_SPECTRAL_CHANNELS
#define CROSSTALK_CHANNELS  ARCROSSTALK500_SIZE

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

ART_LIGHT_AND_ATTENUATION_MODULE_INITIALISATION_FUNCTION( (void) art_gv; )

ART_LIGHT_AND_ATTENUATION_MODULE_NO_SHUTDOWN_FUNCTION_NECESSARY

ArCrosstalk500  * cx500_alloc(
        const ART_GV  * art_gv
        )
{
    (void) art_gv;
    
    ArCrosstalk500  * x0 = ALLOC( ArCrosstalk500 );

    ASSERT_ALLOCATED_CROSSTALK500( x0 )

    return x0;
}

ArCrosstalk500  * cx500_d_alloc_init(
        const ART_GV  * art_gv,
        const double    d0
        )
{
    ArCrosstalk500  * x0;

    x0 = cx500_alloc( art_gv );

    for ( unsigned int i = 0; i < CROSSTALK_CHANNELS; i++ )
        ARCROSSTALK500_I( *x0, i ) = d0;

    ASSERT_VALID_CROSSTALK500( x0 )

    return x0;
}

void cx500_free(
        const ART_GV          * art_gv,
              ArCrosstalk500  * xr
        )
{
    (void) art_gv;
    
    FREE(xr);
}

ArCrosstalk500 const * cx500_none(
        const ART_GV  * art_gv
        )
{
    (void) art_gv;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return 0;
}

void cx500_d_init_x(
        const ART_GV          * art_gv,
        const double            d0,
              ArCrosstalk500  * xr
        )
{
    (void) art_gv;
    
    ASSERT_ALLOCATED_CROSSTALK500( xr )
    ASSERT_NONNEGATIVE_DOUBLE( d0 )

    for ( unsigned int i = 0; i < CROSSTALK_CHANNELS; i++ )
        ARCROSSTALK500_I( *xr, i) = d0;

    ASSERT_VALID_CROSSTALK500( xr )
}

void cx500_x_init_x(
        const ART_GV          * art_gv,
        const ArCrosstalk500  * x0,
              ArCrosstalk500  * xr
        )
{
    (void) art_gv;
    
    ASSERT_VALID_CROSSTALK500( x0 )
    ASSERT_ALLOCATED_CROSSTALK500( xr )

    for ( unsigned int i = 0; i < CROSSTALK_CHANNELS; i++ )
        ARCROSSTALK500_I( *xr, i ) = ARCROSSTALK500_I( *x0, i);

    ASSERT_VALID_CROSSTALK500( xr )
}

void cx500_horizontal_cx500(
        const ART_GV          * art_gv,
        const ArCrosstalk500  * x0,
              ArCrosstalk500  * xr
        )
{
    (void) art_gv;
    
    ASSERT_VALID_CROSSTALK500( x0 );
    ASSERT_ALLOCATED_CROSSTALK500( xr );

    for(unsigned int i = 0; i < SPECTRAL_CHANNELS; ++i)
    {
        double sum = 0.0;
        // In case of conversion to full matrix version
        // Currently, more optimzed like so.
        // for(unsigned int i = 0; i < SPECTRAL_CHANNELS; ++i)
        for(unsigned int j = 0; j < i; ++j)
        {
            sum += ARCROSSTALK500_XY( *x0, j, i );
            ARCROSSTALK500_SET_XYD( *xr, j, i, sum);
        }
    }

    ASSERT_VALID_CROSSTALK500( x0 );
}

void cx500_vertical_cx500(
        const ART_GV          * art_gv,
        const ArCrosstalk500  * x0,
              ArCrosstalk500  * xr
        )
{
    (void) art_gv;
    
    ASSERT_VALID_CROSSTALK500( x0 );
    ASSERT_ALLOCATED_CROSSTALK500( xr );

    for(unsigned int i = 0; i < SPECTRAL_CHANNELS; ++i)
    {
        double sum = 0.0;
        // In case of conversion to full matrix version
        // Currently, more optimzed like so.
        //for(unsigned int j = SPECTRAL_CHANNELS - 1; j >= 0; --j)
        for(unsigned int j = SPECTRAL_CHANNELS - 1; j > i; --j)
        {
            sum += ARCROSSTALK500_XY( *x0, i, j);
            ARCROSSTALK500_SET_XYD( *xr, i, j, sum);
        }
    }

    ASSERT_VALID_CROSSTALK500( x0 );
}

void cx500_horizontal_s500(
        const ART_GV          * art_gv,
        const ArCrosstalk500  * x0,
              ArSpectrum500   * sr
        )
{
    ASSERT_VALID_CROSSTALK500( x0 );
    ASSERT_ALLOCATED_SPECTRUMTYPE( s500, ArSpectrum500, sr );

    for(unsigned int i = 0; i < SPECTRAL_CHANNELS; ++i)
    {
        double sum = 0.0;
        // In case of conversion to full matrix version
        // Currently, more optimzed like so.
        //for(unsigned int j = 0; j < i; ++j) {
        for(unsigned int j = 0; j < SPECTRAL_CHANNELS; ++j)
        {
            sum += ARCROSSTALK500_XY( *x0, j, i );
        }
        
        s500_set_sid(art_gv, sr, i, sum);
    }

    ASSERT_VALID_SPECTRUMTYPE( s500, ArSpectrum500, sr , s )
}

void cx500_vertical_s500(
        const ART_GV          * art_gv,
        const ArCrosstalk500  * x0,
              ArSpectrum500   * sr
        )
{
    ASSERT_VALID_CROSSTALK500( x0 );
    ASSERT_ALLOCATED_SPECTRUMTYPE( s500, ArSpectrum500, sr );

    for(unsigned int i = 0; i < SPECTRAL_CHANNELS; ++i)
    {
        double sum = 0.0;
        // In case of conversion to full matrix version
        // Currently, more optimzed like so.
        //for(unsigned int j = SPECTRAL_CHANNELS - 1; j >= 0; --j)
        for(unsigned int j = SPECTRAL_CHANNELS - 1; j > i; --j)
        {
            sum += ARCROSSTALK500_XY( *x0, i, j );
        }
        
        s500_set_sid(art_gv, sr, i, sum);
    }

    ASSERT_VALID_SPECTRUMTYPE( s500, ArSpectrum500, sr , s )
}

double cx500_dd_value(
        const ART_GV          * art_gv,
        const ArCrosstalk500  * x0,
              double            wi,
              double            wo
        )
{
    (void) art_gv;
    
    const int cidx_i = round(NANO_FROM_UNIT(wi) - ARCROSSTALK500_LOWER_BOUND);
    const int cidx_o = round(NANO_FROM_UNIT(wo) - ARCROSSTALK500_LOWER_BOUND);
    
    if(
        cidx_i < 0 || cidx_i >= SPECTRAL_CHANNELS ||
        cidx_o < 0 || cidx_o >= SPECTRAL_CHANNELS
        )
        return 0.0;
    else
    {
        return ARCROSSTALK500_XY(*x0, cidx_i, cidx_o);
    }
}

void cx500_wl_wl_init_s(
        const ART_GV           * art_gv,
        const ArCrosstalk500   * x0,
        const ArWavelength     * wi,
        const ArWavelength     * wo,
              ArSpectralSample * sr
        )
{
    for ( int i = 0; i < 4; i++ )
    {
        double wiValue = ARWL_WI( *wi,i );
        double woValue = ARWL_WI( *wo,i );
        SPS_CI(*sr, i) = cx500_dd_value(
              art_gv,
              x0,
              wiValue,
              woValue
            );
    }
}

unsigned int cx500_x_valid(
        const ART_GV          * art_gv,
        const ArCrosstalk500  * x0
        )
{
    (void) art_gv;
    
    if ( ! x0 )
    {
        ART_ERRORHANDLING_WARNING( "ArCrosstalk500 parameter is not allocated" );
        return 0;
    }

    for ( unsigned int i = 0; i < CROSSTALK_CHANNELS; i++ )
    {
        if (    ARCROSSTALK500_I(*x0, i) < 0.0
             || m_d_isInf( ARCROSSTALK500_I(*x0, i) )
             || m_d_isNaN( ARCROSSTALK500_I(*x0, i) ) )
        {
            ART_ERRORHANDLING_WARNING( "at least one component of ArCrosstalk500 data is invalid (negative, NaN or Inf)" );
            return 0;
        }
    }

    return 1;
}

void rss2d_strip_noncrosstalk_data(
        const ART_GV          * art_gv,
              ArRSSpectrum2D  * rss2d
        )
{
    (void) art_gv;
    
    int offset = rss2d->stride - rss2d->size / rss2d->stride;

    for ( unsigned int i = 0; i < rss2d->size; i++ )
    {
        unsigned int lineadd = i / rss2d->stride;

        if (    rss2d->array[i] < 0.0
             || (    i - lineadd * rss2d->stride
                  >= offset + lineadd ) )
            rss2d->array[i] = 0.0;
    }
}

#define RSS2D_TO_X_START    ARCROSSTALK500_LOWER_BOUND
#define RSS2D_TO_X_SIZE     SPECTRAL_CHANNELS
#define RSS2D_TO_X_END      (RSS2D_TO_X_START + RSS2D_TO_X_SIZE)

#define SCRATCHPAD_XY(__s,__x,__y)  (__s)[ (__y)*RSS2D_TO_X_SIZE + (__x) ]


#define RSS2D_SIZE_X(__s)       (__s).stride
#define RSS2D_SIZE_Y(__s)       ( (__s).size / (__s).stride )

/*

The following function is neither pretty nor efficient. All data from the
RSS2D is written to a 1nm-per-cell canvas, from which we then integrate the
values into the ISR-specific ArCrosstalk data structure.

*/

void rss2d_to_cx500(
        const ART_GV          * art_gv,
        const ArRSSpectrum2D  * rss2d,
              ArCrosstalk500  * xr
        )
{
    ASSERT_VALID_RSS2D( rss2d )
    ASSERT_ALLOCATED_CROSSTALK500( xr )

    cx500_d_init_x(art_gv, 0.0, xr);

    unsigned int  rss_s_x = RSS2D_SIZE_X(*rss2d);
    unsigned int  rss_s_y = RSS2D_SIZE_Y(*rss2d);

    double  sampleSize =
          NANO_FROM_UNIT(ARRSS2D_EX_STEP(*rss2d));

    double  ex_step_div_2 = ARRSS2D_EX_STEP(*rss2d) / 2.0;
    double  em_step_div_2 = ARRSS2D_EM_STEP(*rss2d) / 2.0;

    for ( unsigned int x = 0; x < rss_s_x; x++ )
        for ( unsigned int y = 0; y < rss_s_y; y++ )
        {
            int x0 = (int) NANO_FROM_UNIT(ARRSS2D_EX_START(*rss2d) - ex_step_div_2 + ARRSS2D_EX_STEP(*rss2d) * x);
            int x1 = (int) NANO_FROM_UNIT(ARRSS2D_EX_START(*rss2d) + ex_step_div_2 + ARRSS2D_EX_STEP(*rss2d) * x );
            int y0 = (int) NANO_FROM_UNIT(ARRSS2D_EM_START(*rss2d) - em_step_div_2 + ARRSS2D_EM_STEP(*rss2d) * y );
            int y1 = (int) NANO_FROM_UNIT(ARRSS2D_EM_START(*rss2d) + em_step_div_2 + ARRSS2D_EM_STEP(*rss2d) * y );

            x0 -= RSS2D_TO_X_START;
            x1 -= RSS2D_TO_X_START;
            y0 -= RSS2D_TO_X_START;
            y1 -= RSS2D_TO_X_START;

            ASSERT_NONNEGATIVE_INTEGER(x0);
            ASSERT_NONNEGATIVE_INTEGER(x1);
            ASSERT_NONNEGATIVE_INTEGER(y0);
            ASSERT_NONNEGATIVE_INTEGER(y1);

            double sampleValue = ARRSSPECTRUM2D_SAMPLE(rss2d,x,y);

            sampleValue /= sampleSize;

            for ( int xx = x0; xx < x1; xx++ )
                for ( int yy = y0; yy < y1; yy++ )
                    ARCROSSTALK500_SET_XYD( *xr, xx, yy, sampleValue );

        }
}

void cx500_dpv_init_x(
        const ART_GV          * art_gv,
        const double            d0,
        const Pnt2D           * p0,
        const Vec2D           * v0,
              ArCrosstalk500  * xr
        )
{
    (void) art_gv;
    
    ASSERT_ALLOCATED_CROSSTALK( xr )

    for ( unsigned int x = 0; x < RSS2D_TO_X_SIZE; x++ )
        for ( unsigned int y = 0; y < RSS2D_TO_X_SIZE; y++ )
        {
            double  xx = RSS2D_TO_X_START + (double) x;
            double  yy = RSS2D_TO_X_START + (double) y;

            double  dx = M_ABS( NANO_FROM_UNIT(XC(*p0)) - xx);
            double  dy = M_ABS( NANO_FROM_UNIT(YC(*p0)) - yy);

            double sampleValue;

            if ( dx > NANO_FROM_UNIT(XC(*v0)) || dy > NANO_FROM_UNIT(YC(*v0)) )
                sampleValue = 0.0;
            else
            {
                double  fx =
                    ( NANO_FROM_UNIT(XC(*v0)) - dx ) / NANO_FROM_UNIT(XC(*v0));
                double  fy =
                    ( NANO_FROM_UNIT(YC(*v0)) - dy ) / NANO_FROM_UNIT(YC(*v0));

                sampleValue = M_MIN( fx * d0, fy * d0 );
            }

            ARCROSSTALK500_SET_XYD( *xr, x, y, sampleValue );

        }
}

void cx500_x_mathematicaprintf(
        const ART_GV          * art_gv,
        const ArCrosstalk500  * x0
        )
{
    (void) art_gv;
    
    const unsigned int  channels = SPECTRAL_CHANNELS;

    printf( "ArCrosstalk500 %dx%d { \n",channels,channels);

    for ( unsigned int y_ci = 0; y_ci < channels; y_ci++ )
    {
        printf( "{ ");

        for ( unsigned int x_ci = 0; x_ci < channels; x_ci++ )
        {
            printf( "%6.4f", ARCROSSTALK500_XY( *x0, x_ci, y_ci ) );

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
