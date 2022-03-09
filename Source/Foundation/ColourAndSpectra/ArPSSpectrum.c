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

#define ART_MODULE_NAME     ArPSSpectrum

#include "ArPSSpectrum.h"

#include "ART_Foundation_Math.h"

#include "Box.h"

ArPSSpectrum  _pss_visible_range;

ART_MODULE_INITIALISATION_FUNCTION_EXEC_ONLY_ONCE
(
    (void) art_gv;
    ARPSS_SIZE(_pss_visible_range)       = 2;
    ARPSS_SCALE(_pss_visible_range)      = 1.0;
    ARPSS_ARRAY(_pss_visible_range)      = ALLOC_ARRAY(Pnt2D,2);
    ARPSS_ARRAY_I(_pss_visible_range, 0) = PNT2D(380.0 NM,1.0);
    ARPSS_ARRAY_I(_pss_visible_range, 1) = PNT2D(720.0 NM,1.0);
,
    // module has no code that gets executed on every startup
)

ART_MODULE_SHUTDOWN_FUNCTION_EXEC_ONLY_ONCE
(
    pss_freearray_s( art_gv, & _pss_visible_range );
,
    // module has no code that gets executed during every shutdown
)

ArPSSpectrum const * pss_visible_range(
        )
{
    return & _pss_visible_range;
}


/* ---------------------------------------------------------------------------
    'psspectrum_v' function

    The circus with the dynamic list is due to the fact that there is one
    thing a function that just gets a va_list argument (as opposed to being
    a real vararg function) cannot do: perform a second va_start, i.e. parse
    the vararg list twice.

    So we have to make do with a single pass, and that means dynamic
    allocation of the samples we find in the arg list. It ain't pretty, but
    it gets the work done.
------------------------------------------------------------------------aw- */

ArPSSpectrum psspectrum_v(
        const double   maximum,
              va_list  argPtr
        )
{
    ArPSSpectrum   s;

    ARPSS_SCALE(s)  = 1.0 / maximum;

    ArList  pointList = ARLIST_EMPTY;

    Pnt2D   point;

    do
    {
        point = va_arg( argPtr, Pnt2D );

        if ( XC(point) != XC(PSS_END) )
        {
            arlist_add_p2d_at_tail( & pointList, point );
        }
    } while ( XC(point) != XC(PSS_END) );

    ARPSS_SIZE(s)  = arlist_length( & pointList );
    ARPSS_ARRAY(s) = ALLOC_ARRAY( Pnt2D, ARPSS_SIZE(s) );

    for ( unsigned int i = 0; i < ARPSS_SIZE(s); i++ )
    {
        arlist_pop_p2d_from_head(
            & pointList,
            & ARPSS_ARRAY_I(s,i)
            );
    }

    return s;
}

ArPSSpectrum psspectrum(
        const double maximum,
        ...
        )
{
    ArPSSpectrum  s;
    va_list       argPtr;

    va_start( argPtr, maximum );

    s = psspectrum_v( maximum, argPtr );

    va_end( argPtr );

    return s;
}

void pss_s_copy_new_s(
        const ART_GV        * art_gv,
        const ArPSSpectrum  * s0,
              ArPSSpectrum  * sr
        )
{
    (void) art_gv;
    
    ARPSS_SIZE(*sr)  = ARPSS_SIZE(*s0);
    ARPSS_SCALE(*sr) = ARPSS_SCALE(*s0);
    ARPSS_ARRAY(*sr) = ALLOC_ARRAY( Pnt2D, ARPSS_SIZE(*sr) );

    for ( unsigned long i = 0; i < ARPSS_SIZE(*sr); i++ )
        ARPSS_ARRAY_I(*sr,i) = ARPSS_ARRAY_I(*s0,i);
}

void pss_freearray_s(
        const ART_GV        * art_gv,
              ArPSSpectrum  * sr
        )
{
    (void) art_gv;
    
    if ( ARPSS_ARRAY(*sr) )
        FREE_ARRAY( ARPSS_ARRAY(*sr) );
}

void pss_d_mul_s(
        const ART_GV        * art_gv,
        const double          d0,
              ArPSSpectrum  * sr
        )
{
    (void) art_gv;
    
    for ( unsigned long i = 0; i < ARPSS_SIZE(*sr); i++ )
        YC( ARPSS_ARRAY_I(*sr,i) ) *= d0;
}

void pss_ss_add_new_s(
        const ART_GV        * art_gv,
        const ArPSSpectrum  * s0,
        const ArPSSpectrum  * s1,
              ArPSSpectrum  * sr
        )
{
    (void) art_gv;
    (void) s0;
    (void) s1;
    (void) sr;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

void pss_s_add_s(
        const ART_GV        * art_gv,
        const ArPSSpectrum  * s0,
              ArPSSpectrum  * sr
        )
{
    (void) art_gv;
    (void) s0;
    (void) sr;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

void rss2d_to_pss_new(
        const ART_GV          * art_gv,
        const ArRSSpectrum2D  * s0,
              ArPSSpectrum    * sr
        )
{
    (void) art_gv;
    (void) s0;
    (void) sr;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

static __inline__ double integral_evaluate(
        double a0, double b0,
        double a1, double b1,
        double x
        )
    { return ((( a0*a1 * x)/3.0 + (a0*b1+a1*b0)/2.0)*x + b0*b1) * x; }


#define STEP_0          1
#define STEP_1          2
#define STEP_BOTH       3

/* ---------------------------------------------------------------------------
    'pss_inner_product'
--------------------------------------------------------------------------- */

double pss_inner_product(
        const ART_GV        * art_gv,
        const ArPSSpectrum  * s0,
        const ArPSSpectrum  * s1
        )
{
    (void) art_gv;

    Pnt2D * min_0 = s0->array;
    Pnt2D * min_1 = s1->array;
    Pnt2D * max_0 = min_0 + 1;
    Pnt2D * max_1 = min_1 + 1;
    Pnt2D * end_0 = min_0 + s0->size;
    Pnt2D * end_1 = min_1 + s1->size;
    double integral = 0.0;
    double slope_0 = 0.0;               /* init to avoid compiler warning */
    double slope_1 = 0.0;               /* -"- */
    int step;

    while (max_0 < end_0
            && (XC(*max_0) < XC(*min_1) || XC(*min_0) >= XC(*max_0)))
        min_0 = max_0++;
    if (max_0 == end_0) return 0.0;
    while (max_1 < end_1
            && (XC(*max_1) < XC(*min_0) || XC(*min_1) >= XC(*max_1)))
        min_1 = max_1++;
    if (max_1 == end_1) return 0.0;

    if (XC(*min_1) < XC(*min_0))
    {
        step = STEP_0;
        slope_1 = (YC(*max_1)-YC(*min_1)) / (XC(*max_1)-XC(*min_1));
    }
    else if (XC(*min_0) < XC(*min_1))
    {
        step = STEP_1;
        slope_0 = (YC(*max_0)-YC(*min_0)) / (XC(*max_0)-XC(*min_0));
    }
    else
        step = STEP_BOTH;

    while (max_0 < end_0 && max_1 < end_1)
    {
        double xmin = 0.0;              /* init to avoid compiler warning */
        double ymin_0 = 0.0;            /* -"- */
        double ymin_1 = 0.0;            /* -"- */
        double size;
        switch (step)
        {
            case STEP_0:
            {
                xmin = XC(*min_0);
                slope_0 = (YC(*max_0)-YC(*min_0)) / (XC(*max_0)-XC(*min_0));
                ymin_0 = YC(*min_0);
                ymin_1 = YC(*min_1) + (xmin-XC(*min_1)) * slope_1;
                break;
            }
            case STEP_1:
            {
                xmin = XC(*min_1);
                slope_1 = (YC(*max_1)-YC(*min_1)) / (XC(*max_1)-XC(*min_1));
                ymin_0 = YC(*min_0) + (xmin-XC(*min_0)) * slope_0;
                ymin_1 = YC(*min_1);
                break;
            }
            case STEP_BOTH:
            {
                xmin = XC(*min_0);
                slope_0 = (YC(*max_0)-YC(*min_0)) / (XC(*max_0)-XC(*min_0));
                slope_1 = (YC(*max_1)-YC(*min_1)) / (XC(*max_1)-XC(*min_1));
                ymin_0 = YC(*min_0);
                ymin_1 = YC(*min_1);
            }
        }

        if (XC(*max_0) < XC(*max_1))
        {
            size = XC(*max_0) - xmin;
            do { min_0 = max_0++; }
                    while (max_0 < end_0 && XC(*min_0) >= XC(*max_0));
            step = STEP_0;
        }
        else if (XC(*max_1) < XC(*max_0))
        {
            size = XC(*max_1) - xmin;
            do { min_1 = max_1++; }
                    while (max_1 < end_1 && XC(*min_1) >= XC(*max_1));
            step = STEP_1;
        }
        else
        {
            size = XC(*max_0) - xmin;
            do { min_0 = max_0++; }
                    while (max_0 < end_0 && XC(*min_0) >= XC(*max_0));
            do { min_1 = max_1++; }
                    while (max_1 < end_1 && XC(*min_1) >= XC(*max_1));
            step = STEP_BOTH;
        }

        integral += integral_evaluate(slope_0,ymin_0,slope_1,ymin_1,size);
    }
    return integral * s0->scale * s1->scale;
}

double pss_sd_value_at_wavelength(
        const ART_GV        * art_gv,
        const ArPSSpectrum  * s0,
        const double          d0
        )
{
    (void) art_gv;

    if ( d0 >= XC(ARPSS_ARRAY_I( *s0, 0 )) && d0 <= XC(ARPSS_ARRAY_I( *s0, ARPSSPECTRUM_SIZE( *s0 ) - 2 ) ) )
    {
        int i = 0;

        while( d0 > XC(ARPSS_ARRAY_I( *s0, i )) )
            i++;

        double scale = ( 1.0 / ( XC(ARPSS_ARRAY_I( *s0, i )) - XC(ARPSS_ARRAY_I( *s0, i-1 )) ) ) *
                        ( XC(ARPSS_ARRAY_I( *s0, i )) - d0 );
        return YC( ARPSS_ARRAY_I( *s0, i ) ) * ( 1.0 - scale ) + YC( ARPSS_ARRAY_I( *s0, i-1 ) ) * scale;

    }
    else
    {
        ART_ERRORHANDLING_FATAL_ERROR("requested value outside defined spectrum");

        return 0.0;
    }

}

void pss_s_debugprintf(
        const ART_GV        * art_gv,
        const ArPSSpectrum  * pss
        )
{
    (void) art_gv;

    printf("psspectrum( %d, %f", ARPSS_SIZE(*pss), ARPSS_SCALE(*pss) );

    for ( unsigned long i = 0; i < ARPSS_SIZE(*pss); i++ )
       printf( ", PNT2D( %4.1f nm, % 5.3f)",
       NANO_FROM_UNIT(XC(ARPSS_ARRAY_I(*pss,i))),YC(ARPSS_ARRAY_I(*pss,i)));

    printf(")\n");

    fflush(stdout);
}

void pss_s_mathematicaprintf(
        const ART_GV        * art_gv,
        const ArPSSpectrum  * pss
        )
{
    (void) art_gv;

    printf("psspectrum{ ");

    if ( ARPSS_SIZE(*pss) > 0 )
        printf( "{%4.1f, %f}",
           NANO_FROM_UNIT(XC(ARPSS_ARRAY_I(*pss,0))),YC(ARPSS_ARRAY_I(*pss,0)));

    for ( unsigned long i = 1; i < ARPSS_SIZE(*pss); i++ )
        printf( ", {%4.1f, %f}",
           NANO_FROM_UNIT(XC(ARPSS_ARRAY_I(*pss,i))),YC(ARPSS_ARRAY_I(*pss,i)));

    printf(" }\n");

    fflush(stdout);
}


/* ======================================================================== */
