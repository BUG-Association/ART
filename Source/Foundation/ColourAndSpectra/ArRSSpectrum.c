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

#define ART_MODULE_NAME     ArRSSpectrum

#include "ArRSSpectrum.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

#include "ART_Foundation_Math.h"
#include "FoundationAssertionMacros.h"


/* ---------------------------------------------------------------------------
    'rsspectrum_v' function

    The circus with the dynamic list is due to the fact that there is one
    thing a function that just gets a va_list argument (as opposed to being
    a real vararg function) cannot do: perform a second va_start, i.e. parse
    the vararg list twice.

    So we have to make do with a single pass, and that means dynamic
    allocation of the samples we find in the arg list. It ain't pretty, but
    it gets the work done.
------------------------------------------------------------------------aw- */

ArRSSpectrum rsspectrum_v(
        const double   start,
        const double   step,
        const double   maximum,
              va_list  argPtr
        )
{
    ArRSSpectrum   s;

    ARRSS_START(s) = start;
    ARRSS_STEP(s)  = step;
    ARRSS_SCALE(s) = 1.0 / maximum;

    ArList  sampleList = ARLIST_EMPTY;

    double sample = RSS_END;
    
    do
    {
        sample = va_arg( argPtr, double );

        if ( sample != RSS_END )
        {
            arlist_add_d_at_tail( & sampleList, sample );
        }
    } while ( sample != RSS_END );

    ARRSS_SIZE(s)  = arlist_length( & sampleList );
    ARRSS_ARRAY(s) = ALLOC_ARRAY( double, ARRSS_SIZE(s) );

    for ( unsigned int i = 0; i < ARRSS_SIZE(s); i++ )
    {
        arlist_pop_d_from_head(
            & sampleList,
            & ARRSS_ARRAY_I(s,i)
            );
    }

    return s;
}

ArRSSpectrum rsspectrum(
        const double  start,
        const double  step,
        const double  maximum,
        ...
        )
{
    ArRSSpectrum  s;
    va_list       argPtr;

    va_start( argPtr, maximum );

    s = rsspectrum_v( start, step, maximum, argPtr );

    va_end( argPtr );

    return s;
}

void rsspectrum_macadam(
        const unsigned int    centralWavelength,
        const unsigned int    widthDiv2,
              ArRSSpectrum  * rss
        )
{
    ARRSS_START(*rss) = 380 NM;
    ARRSS_STEP(*rss)  = 1 NM;
    ARRSS_SCALE(*rss) = 1.0;
    
    if ( ARRSS_SIZE(*rss) != 360 )
    {
        ARRSS_SIZE(*rss)  = 360;
        
        if ( ARRSS_ARRAY(*rss) )
        {
            FREE_ARRAY(ARRSS_ARRAY(*rss));
            
        }
        
        ARRSS_ARRAY(*rss) = ALLOC_ARRAY( double, ARRSS_SIZE(*rss) );
    }

    int  dist380cwl = M_ABS( 380 - (int)centralWavelength );
    int  dist740cwl = M_ABS( 740 - (int)centralWavelength );

    for ( int i = 0; i < (int)ARRSS_SIZE(*rss); i++ )
    {
        ARRSS_ARRAY_I(*rss,i) = 0.;

        int  wl = i + 380;
        int  dist = M_ABS( (int)centralWavelength - wl );
        
        if ( dist < (int)widthDiv2 )
        {
            ARRSS_ARRAY_I(*rss,i) = 1.;
        }
        else
        {
            int  dist380 = M_ABS( 380 - wl );

            if ( dist380 < (int)widthDiv2 )
            {
                if (    dist740cwl < (int)widthDiv2
                     && dist380 < (int)widthDiv2 - dist740cwl )
                {
                    ARRSS_ARRAY_I(*rss,i) = 1.;
                }
            }
            else
            {
                int  dist740 = M_ABS( 740 - wl );
                
                if ( dist740 < (int)widthDiv2 )
                {
                    if (    dist380cwl < (int)widthDiv2
                         && dist740 < (int)widthDiv2 - dist380cwl )
                    {
                        ARRSS_ARRAY_I(*rss,i) = 1.;
                    }
                }
            }
        }
    }
}

void rss_free_contents(
        const ART_GV        * art_gv,
              ArRSSpectrum  * rss
        )
{
    (void) art_gv;
    
    FREE_ARRAY( ARRSS_ARRAY(*rss) );
}

void rss_free(
        const ART_GV        * art_gv,
              ArRSSpectrum  * rss
        )
{
    rss_free_contents( art_gv, rss );
    FREE( rss );
}

/* ---------------------------------------------------------------------------
    'rss_integrate'
        Integrates a regularly sampled spectrum in the bounds given by
        xmin and xmax.
--------------------------------------------------------------------------- */

double rss_integrate(
        const ART_GV        * art_gv,
        const ArRSSpectrum  * rss,
        const double          xmin,
        const double          xmax
        )
{
    (void) art_gv;
    
    double fraction = (xmin - rss->start) / rss->step;
    ASSERT_NONNEGATIVE_DOUBLE(fraction);

    unsigned int index = (unsigned int)floor(fraction);
    double x = rss->start + rss->step * (double)index;
    double sum;

    fraction -= (double)index;


    sum = (0.5 - fraction) * ARRSS_SAMPLE(*rss,index)
            + 0.5 * fraction * fraction * (ARRSS_SAMPLE(*rss,index+1)
                                - ARRSS_SAMPLE(*rss,index));

    while (x < xmax)
    {
        sum += ARRSS_SAMPLE(*rss,index); index++;
        x += rss->step;
    }

    fraction = (x-xmax)/rss->step;

    sum += (0.5 - fraction) * ARRSS_SAMPLE(*rss,index)
            - 0.5 * fraction * fraction * (ARRSS_SAMPLE(*rss,index)
                                            - ARRSS_SAMPLE(*rss,index-1));


    return sum * rss->step;
}

double rss_sd_value_at_wavelength(
        const ART_GV        * art_gv,
        const ArRSSpectrum  * s0,
        const double          d0
        )
{
    (void) art_gv;
    
    ASSERT_VALID_RSS(s0)
    
    //   Early exit - is the requested wavelength within the spectral bounds?
    
    double  upperWL = ARRSS_START(*s0) + ARRSS_SIZE(*s0) * ARRSS_STEP(*s0);
    
    if (    d0 <  ARRSS_START(*s0)
         || d0 >= upperWL )
        return 0.0;
    
    int  index = ( d0 - ARRSS_START(*s0) ) / ARRSS_STEP(*s0);
    
    ASSERT_INTEGER_WITHIN_RANGE( index, 0, (int)ARRSS_SIZE(*s0) - 1 );
    
    ASSERT_NONNEGATIVE_FINITE_DOUBLE( ARRSS_SAMPLE(*s0,index) );
    
    return ARRSS_SAMPLE(*s0, (unsigned int)index);
}

unsigned int rss_s_valid(
        const ART_GV        * art_gv,
        const ArRSSpectrum  * rss
        )
{
    (void) art_gv;
    
    //   Null pointer? Direct return in that case.
    
    if ( ! rss )
        return 0;
    
    unsigned int result = 1;
    
    //   Sanity checks on various features
    
    //   Nonzero content array?
    
    if ( ARRSS_SIZE(*rss) <= 0 )
    {
        ART_ERRORHANDLING_WARNING(
            "invalid RSS size (%d entries)"
            ,   ARRSS_SIZE(*rss)
            );

        result = 0;
    }
    
    //   Spectrum start?
    
    if (   ARRSS_START(*rss) < 0
        || m_d_isInf( ARRSS_START(*rss) )
        || m_d_isNaN( ARRSS_START(*rss) ) )
    {
        ART_ERRORHANDLING_WARNING(
            "invalid RSS start size %f [nm]"
            ,   NANO_FROM_UNIT(ARRSS_START(*rss))
            );

        result = 0;
    }
    
    //   Spectrum step size?
    
    if (   ARRSS_STEP(*rss) <= 0
        || m_d_isInf( ARRSS_STEP(*rss) )
        || m_d_isNaN( ARRSS_STEP(*rss) ) )
    {
        ART_ERRORHANDLING_WARNING(
            "invalid RSS step size %f [nm]"
            ,   NANO_FROM_UNIT(ARRSS_STEP(*rss))
            );

        result = 0;
    }
    
    //   Spectrum scale?
    
    if (   ARRSS_SCALE(*rss) <= 0
        || m_d_isInf( ARRSS_SCALE(*rss) )
        || m_d_isNaN( ARRSS_SCALE(*rss) ) )
    {
        ART_ERRORHANDLING_WARNING(
            "invalid RSS scale %f"
            ,   ARRSS_SCALE(*rss)
            );

        result = 0;
    }
    
    for ( unsigned int i = 0; i < ARRSS_SIZE(*rss); i++ )
    {
        if (   ARRSS_SAMPLE( *rss, i ) < 0.0
            || m_d_isInf( ARRSS_SAMPLE( *rss, i ) )
            || m_d_isNaN( ARRSS_SAMPLE( *rss, i ) ) )
        {
            ART_ERRORHANDLING_WARNING(
                "RSS sample %d is invalid (%f)",
                i,
                ARRSS_SAMPLE( *rss, i )
                );

            result = 0;
        }
    }

    return result;
}

void rss_s_debugprintf(
        const ART_GV        * art_gv,
        const ArRSSpectrum  * rss
        )
{
    (void) art_gv;
    
    printf("rsspectrum( %d, %f nm, %f nm, %f",
    ARRSS_SIZE(*rss),NANO_FROM_UNIT(ARRSS_START(*rss)),
    NANO_FROM_UNIT(ARRSS_STEP(*rss)),ARRSS_SCALE(*rss));
    for ( unsigned long i = 0; i < ARRSS_SIZE(*rss); i++ )
       printf(", %f",ARRSS_SAMPLE(*rss,i));
    printf(")\n");
    fflush(stdout);
}

void rss_s_mathematicaprintf(
        const ART_GV        * art_gv,
        const ArRSSpectrum  * rss
        )
{
    (void) art_gv;
    
    printf("rsspectrum{ ");

    if ( ARRSS_SIZE(*rss) > 0 )
       printf("{%4.1f,%f}",
           NANO_FROM_UNIT(ARRSS_START(*rss)),
           ARRSS_SAMPLE(*rss,0));

    for ( unsigned long i = 1; i < ARRSS_SIZE(*rss); i++ )
       printf(", {%4.1f,%f}",
           NANO_FROM_UNIT(ARRSS_START(*rss)+i*ARRSS_STEP(*rss)),
           ARRSS_SAMPLE(*rss,i));

    printf(" }\n");
    fflush(stdout);
}


// ===========================================================================
