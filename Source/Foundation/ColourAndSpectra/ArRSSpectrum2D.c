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

#include <stdarg.h>

#include "ArRSSpectrum.h"
#include "ArRSSpectrum2D.h"

#include "ART_Foundation_Math.h"
#include "FoundationAssertionMacros.h"

#include "ArSpectrum.h"

#define RSS2D_SAMPLE    ARRSS2D_SAMPLE


ArRSSpectrum2D rss2d_v(
        const double   excitation_start,
        const double   excitation_step,
        const double   emission_start,
        const double   emission_step,
        const double   maximum,
              va_list  argPtr
        )
{
    ArRSSpectrum2D   s;

    ARRSS2D_EX_START(s) = excitation_start;
    ARRSS2D_EX_STEP(s)  = excitation_step;
    ARRSS2D_EM_START(s) = emission_start;
    ARRSS2D_EM_STEP(s)  = emission_step;
    ARRSS2D_SCALE(s)    = 1.0 / maximum;

    ArList  sampleList = ARLIST_EMPTY;

    double  sample = RSS2D_LINE_END;

    int stride = 0;
    ARRSS2D_STRIDE(s) = -1; // mark as undefined
    while ( sample != RSS2D_END )
    {
        sample = va_arg( argPtr, double );

        if ( sample == RSS2D_LINE_END || sample == RSS2D_END )
        {
            if( ARRSS2D_STRIDE(s) == -1 )
                ARRSS2D_STRIDE(s) = stride;
            else if( ARRSS2D_STRIDE(s) != stride && ( stride != 0 || sample == RSS2D_LINE_END ) )
                ART_ERRORHANDLING_FATAL_ERROR(
                    "ArRSSpectrum2D: incompatible stride -- %u at first, %u then",
                    ARRSSPECTRUM2D_STRIDE(s),
                    stride
                );
            stride = 0;
        }
        else
        {
            arlist_add_d_at_tail( & sampleList, sample );
            ++stride;
        }
    }

    ARRSS2D_SIZE(s)  = arlist_length( & sampleList );
    ARRSS2D_ARRAY(s) = ALLOC_ARRAY( double, ARRSS2D_SIZE(s) );

    for ( unsigned int i = 0; i < ARRSS2D_SIZE(s); i++ )
    {
        arlist_pop_d_from_head(
            & sampleList,
            & ARRSS2D_ARRAY_I(s,i)
            );
    }

    return s;
}

ArRSSpectrum2D rss2d(
        const double  excitation_start,
        const double  excitation_step,
        const double  emission_start,
        const double  emission_step,
        const double  maximum,
        ...
        )
{
    ArRSSpectrum2D s;
    va_list        argPtr;

    va_start( argPtr, maximum );

    s = rss2d_v( excitation_start, excitation_step, emission_start, emission_step, maximum, argPtr );

    va_end( argPtr );

    return s;
}


typedef struct ArIntegrationCell2D
{
    Vec2D   size;
    double  value[4];  /*counter-clockwise, starting at the lower left corner */
}
ArIntegrationCell2D;

//   uncomment this to have rss2d_integrate talk to you about what it is doing

//#define RSS2d_INTEGRATE_DIAGNOSTIC_OUTPUT

//   Determines the integer array coordinate for a given double
//   coordinate in wavelength space, e.g. 430nm -> array index 5

unsigned long rss_coord(
        double  wavelength,
        double  rss_start,
        double  rss_step
        )
{
    const double  d_from_start = wavelength - rss_start;

    ASSERT_NONNEGATIVE_FINITE_DOUBLE(d_from_start);

    return  (int) ( d_from_start / rss_step );
}

double getHeight(
        const ArRSSpectrum2D  * rss2d,
        const double            x,
        const double            y
        )
{
    double lowerEmission = 0.0;
    double excitationFactor = 0.0;
    double emissionFactor = 0.0;
    double factor = 0.0;
    unsigned int i,j;
    double a, b;
    double height = 0.0;
    double eps = UNIT_FROM_NANO(0.001);
    double xtmp = UNIT_FROM_NANO(299.0);
    double ytmp = UNIT_FROM_NANO(376.0);
    xtmp = x;
    ytmp = y;


    i = rss_coord(xtmp,
                rss2d->excitation_start,
                 rss2d->excitation_step);
    excitationFactor = xtmp - (rss2d->excitation_start +
                               i * rss2d->excitation_step);
    excitationFactor = NANO_FROM_UNIT(excitationFactor) / 10.0;

    j = rss_coord(ytmp,
                 rss2d->emission_start,
                 rss2d->emission_step);
    emissionFactor = ytmp - (rss2d->emission_start +
                             j * rss2d->emission_step);
    emissionFactor = NANO_FROM_UNIT(emissionFactor) / 10.0;

    lowerEmission = rss2d->emission_start + j * rss2d->emission_step;

    //Upper right triangle
    if ((ytmp - lowerEmission) <=
        excitationFactor * rss2d->emission_step + eps)
    {
        a = M_INTERPOL(RSS2D_SAMPLE(rss2d, i, j),
                       RSS2D_SAMPLE(rss2d, i+1, j),
                       excitationFactor);
        b = M_INTERPOL(RSS2D_SAMPLE(rss2d, i, j),
                       RSS2D_SAMPLE(rss2d, i+1, j+1),
                       excitationFactor);
        if (fabs(excitationFactor) <= eps) factor = emissionFactor;
        else factor = emissionFactor/excitationFactor;
        height = M_INTERPOL(a, b,  factor);
    }
    //Lower left triangle
    else
    {
        a = M_INTERPOL(RSS2D_SAMPLE(rss2d, i, j+1),
                       RSS2D_SAMPLE(rss2d, i+1, j+1),
                       excitationFactor);
        b = M_INTERPOL(RSS2D_SAMPLE(rss2d, i, j),
                       RSS2D_SAMPLE(rss2d, i+1, j+1),
                       excitationFactor);
        if (fabs(excitationFactor) <= eps) factor = emissionFactor;
        else factor = (1.0 - emissionFactor)/excitationFactor;
        height = M_INTERPOL(a, b, factor);
    }

    height = M_MAX(height,0.0);

    ASSERT_NONNEGATIVE_FINITE_DOUBLE( height )

    return height;
}

double arintegrationcell2d_volume(
        ArIntegrationCell2D  integrationCell
        )
{
    return
        ( integrationCell.value[0] + integrationCell.value[1] +
          integrationCell.value[2] + integrationCell.value[3] )
        / 4.0;
}

/* ---------------------------------------------------------------------------
    'rss2d_integrate'
        Integrates a regularly sampled spectrum in the bounds given by
        ymin and xmax.
--------------------------------------------------------------------------- */

double rss2d_integrate(
        const ART_GV          * art_gv,
        const ArRSSpectrum2D  * rss2d,
        const double            xmin,
        const double            xmax,
        const double            ymin,
        const double            ymax
        )
{
    (void) art_gv;
    
    ASSERT_NONNEGATIVE_FINITE_DOUBLE( xmin )
    ASSERT_NONNEGATIVE_FINITE_DOUBLE( xmax )
    ASSERT_NONNEGATIVE_FINITE_DOUBLE( ymin )
    ASSERT_NONNEGATIVE_FINITE_DOUBLE( ymax )

    ASSERT_VALID_RSS2D( rss2d )

#ifdef RSS2d_INTEGRATE_DIAGNOSTIC_OUTPUT
    debugprintf("\nIntegration Range:\n");
    debugprintf("X: %4.1fnm - %4.1fnm\n",NANO_FROM_UNIT(xmin),NANO_FROM_UNIT(xmax));
    debugprintf("Y: %4.1fnm - %4.1fnm\n",NANO_FROM_UNIT(ymin),NANO_FROM_UNIT(ymax));
#endif

    double  volume = 0.0;

    int count = 0;

    double  x1 = xmin;
    double  x2 =
        rss2d->excitation_start + rss2d->excitation_step *
                (rss_coord(xmin,
                          rss2d->excitation_start,
                          rss2d->excitation_step) + 1);

    int max_excitation = rss_coord(xmax,
            rss2d->excitation_start,
            rss2d->excitation_step);

    int max_emission = rss_coord(ymax,
            rss2d->emission_start,
            rss2d->emission_step);

#ifdef RSS2d_INTEGRATE_DIAGNOSTIC_OUTPUT
    debugprintf("\nIteration Range:\n");
    debugprintf("EX: 0 - %d\n",max_excitation);
    debugprintf("EM: 0 - %d\n",max_emission);
#endif

    for ( int i = 0; i < max_excitation; i++ )
    {
        double  y1 = ymin;
        double  y2 = rss2d->emission_start + rss2d->emission_step *
                (rss_coord(ymin,
                       rss2d->emission_start,
                       rss2d->emission_step) + 1);

#ifdef RSS2d_INTEGRATE_DIAGNOSTIC_OUTPUT
    debugprintf("\nCell Range:\n");
    debugprintf("x: %4.1fnm - %4.1fnm\n",NANO_FROM_UNIT(x1),NANO_FROM_UNIT(x2));
    debugprintf("y: %4.1fnm - %4.1fnm\n",NANO_FROM_UNIT(y1),NANO_FROM_UNIT(y2));
#endif

        double  upperLeft = getHeight(rss2d, x1, y1);
        double  upperRight = getHeight(rss2d, x2, y1);

        for ( int j = 0; j < max_emission; j++ )
        {
            ArIntegrationCell2D  integrationCell;

            integrationCell.value[0] = getHeight(rss2d, x1, y2);
            integrationCell.value[1] = getHeight(rss2d, x2, y2);
            integrationCell.value[2] = upperRight;
            integrationCell.value[3] = upperLeft;

            XC(integrationCell.size) = x2 - x1;
            YC(integrationCell.size) = y2 - y1;

            volume += arintegrationcell2d_volume(integrationCell);

            count++;

            upperLeft = integrationCell.value[0];
            upperRight = integrationCell.value[1];

            y1 = y2;

            if ((y2 + rss2d->emission_step > ymax) && (y2 != ymax))
                y2 = ymax;
            else
                y2 += rss2d->emission_step;
        }

        x1 = x2;

        if ((x2 + rss2d->emission_step > xmax) && (x2 != xmax))
            x2 = xmax;
        else
            x2 += rss2d->emission_step;
    }

    double  result;

    if ( count > 0 )
        result = volume/(double)count;
    else
        result = 0.0;

    ASSERT_NONNEGATIVE_FINITE_DOUBLE( result )

    return result;
}

ArRSSpectrum2D  * rss2d_s_alloc_init_denoised_version_free_original(
        ArRSSpectrum2D  * rss2d,
        double            maxToleratedValue,
        double            maxDifferenceToNeighbours
        )
{
    ArRSSpectrum2D  * denoised_rss2d = ALLOC( ArRSSpectrum2D );

    *denoised_rss2d = *rss2d;

    ARRSS2D_ARRAY(*denoised_rss2d) =
        ALLOC_ARRAY(
            double,
            ARRSS2D_SIZE(*rss2d)
            );

    int  offset =
          ARRSS2D_STRIDE(*rss2d)
        - ( ARRSS2D_SIZE(*rss2d) / ARRSS2D_STRIDE(*rss2d) );

    //   Step 1 - the original array will be discarded anyway, so we clip
    //   the sample values there (to a maximum value, and zero).

    for ( unsigned long i = 0; i < ARRSS2D_SIZE(*rss2d); i++ )
    {
        int  x = i % ARRSS2D_STRIDE(*rss2d);
        int  y = i / ARRSS2D_STRIDE(*rss2d);

        //   The max clipping only affects crosstalk elements, i.e. everything
        //   except the main diagonal.

        if (   ARRSS2D_ARRAY_I(*rss2d,i) > maxToleratedValue
            && x - offset != y )
            ARRSS2D_ARRAY_I(*rss2d,i) = maxToleratedValue;

        //   Zero clipping is done for all samples.

        if ( ARRSS2D_ARRAY_I(*rss2d,i) < 0.0 )
            ARRSS2D_ARRAY_I(*rss2d,i) = 0.0;
    }

    //   Step 2 - outliers, judged by percent difference to the average of
    //   their surroundings, are clipped to that average as well.

    for ( unsigned long i = 0; i < ARRSS2D_SIZE(*rss2d); i++ )
    {
        int  x = i % ARRSS2D_STRIDE(*rss2d);
        int  y = i / ARRSS2D_STRIDE(*rss2d);

        ARRSS2D_ARRAY_I(*denoised_rss2d,i) =
            ARRSS2D_SAMPLE(rss2d,x,y);

        if (   ARRSS2D_SAMPLE(rss2d,x,y) > 0.0
            && x - offset != y )
        {
            double  surroundAverage =
                  ARRSS2D_SAMPLE( rss2d, x + 1, y + 1 )
                + ARRSS2D_SAMPLE( rss2d, x + 1, y     )
                + ARRSS2D_SAMPLE( rss2d, x + 1, y - 1 )
                + ARRSS2D_SAMPLE( rss2d, x - 1, y + 1 )
                + ARRSS2D_SAMPLE( rss2d, x - 1, y     )
                + ARRSS2D_SAMPLE( rss2d, x - 1, y - 1 )
                + ARRSS2D_SAMPLE( rss2d, x    , y + 1 )
                + ARRSS2D_SAMPLE( rss2d, x    , y - 1 );

            surroundAverage /= 8.0;

            double difference =
                M_ABS( 1.0 - surroundAverage / ARRSS2D_SAMPLE(rss2d,x,y) );

            if ( difference > maxDifferenceToNeighbours )
                ARRSS2D_ARRAY_I(*denoised_rss2d,i) = surroundAverage;
        }
    }

    FREE_ARRAY( ARRSS2D_ARRAY(*rss2d) );
    FREE( rss2d );

    return denoised_rss2d;
}

void rss2d_to_rss(
        const ArRSSpectrum2D  * rss2d,
              ArRSSpectrum    * rss
        )
{
    int  offset;

    rss->start = rss2d->emission_start;
    rss->step  = rss2d->emission_step;
    rss->scale = rss2d->scale;
    rss->size  = rss2d->size / rss2d->stride;
    rss->array = ALLOC_ARRAY( double, rss->size );

    offset = rss2d->stride - rss->size;

    for ( unsigned int i = 0; i < rss->size; i++ )
    {
        rss->array[i] = ARRSSPECTRUM2D_SAMPLE(rss2d,i+offset,i);
    }
}

unsigned int rss2d_s_valid(
        const ART_GV          * art_gv,
        const ArRSSpectrum2D  * s0
        )
{
    //   null pointer

    if ( !s0 )
        return 0;

    //   minimum size for a sensible (but in that case, degenerate) 2D RSS

    if ( s0->size < spc_channels( art_gv ) )
        return 0;

    //   size not a multiple of stride

    if ( ( s0->size % s0->stride ) != 0 )
        return 0;

    //   stride larger than size

    if ( s0->stride > (int) s0->size )
        return 0;

    //   zero or negative double parameters

    if ( s0->excitation_start <= 0.0 )
        return 0;
    if ( s0->excitation_step <= 0.0 )
        return 0;
    if ( s0->emission_start <= 0.0 )
        return 0;
    if ( s0->emission_step <= 0.0 )
        return 0;
    if ( s0->scale <= 0.0 )
        return 0;

    //   finally, a check for degenerate spectrum entries

    for ( unsigned long i = 0; i < s0->size; i++ )
        if (   s0->array[i] < 0.0
            || m_d_isInf( s0->array[i] )
            || m_d_isNaN( s0->array[i] ) )
            return 0;

    return 1;
}

// ===========================================================================
