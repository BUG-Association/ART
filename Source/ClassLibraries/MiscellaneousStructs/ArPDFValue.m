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

#define ART_MODULE_NAME     ArPDFValue

#import "ArPDFValue.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

void arpdfvalue_p_debugprintf(
        ArPDFValue  * p0
        )
{
    printf("ArPDFValue: ");

    if ( ARPDFVALUE_IS_INFINITE(*p0) )
    {
        printf("infinity");
    }
    else
    {
        printf(
            "%f",
            ARPDFVALUE_MAIN(*p0)
            );
    }

    printf("\n");
}

void arpdfvalue_p_div_s(
        ArPDFValue       * p0,
        ArSpectralSample * sr
        )
{
    for ( int i = 0; i < 4; i++ )
    {
        SPS_CI(*sr, i) /= M_ABS(ARPDFVAL_PI(*p0,i));
        if(m_d_isInf(SPS_CI(*sr, i)))
            SPS_CI(*sr, i) = 0.0;
    }
}

void arpdfvalue_p_add_p(
        ArPDFValue  * p0,
        ArPDFValue  * pr
        )
{
    for ( int i = 0; i < 4; i++ )
    {
        if(ARPDFVALUE_IS_INFINITE_I(*p0, i))
        {
            if(!ARPDFVALUE_IS_INFINITE_I(*pr, i))
            {
                // copy the infinite pdf over
                ARPDFVAL_PI(*pr,i) = ARPDFVAL_PI(*p0,i);
                continue;
            }
        }
        else
            if(ARPDFVALUE_IS_INFINITE_I(*pr, i))
                continue; // preserve the infinite pdf
        
        // only the same state (regular/infinite) is left
        ARPDFVAL_PI(*pr,i) += ARPDFVAL_PI(*p0,i);
    }
}

void arpdfvalue_pp_add_p(
        ArPDFValue  * p0,
        ArPDFValue  * p1,
        ArPDFValue  * pr
        )
{
    for ( int i = 0; i < 4; i++ )
    {
        if(ARPDFVALUE_IS_INFINITE_I(*p0, i))
        {
            if(!ARPDFVALUE_IS_INFINITE_I(*p1, i))
            {
                // copy the infinite pdf over
                ARPDFVAL_PI(*pr,i) = ARPDFVAL_PI(*p0,i);
                continue;
            }
        }
        else
            if(ARPDFVALUE_IS_INFINITE_I(*p1, i))
            {
                // copy the infinite pdf over
                ARPDFVAL_PI(*pr,i) = ARPDFVAL_PI(*p1,i);
                continue;
            }
        
        // only the same state (regular/infinite) is left
        ARPDFVAL_PI(*pr,i) = ARPDFVAL_PI(*p0,i) + ARPDFVAL_PI(*p1,i);
    }
}

void arpdfvalue_d_mul_p(
        double        d0,
        ArPDFValue  * pr
        )
{
    ASSERT_NONNEGATIVE_DOUBLE( d0 ); // negative numbers would break how we treat infinite values
    
    for ( int i = 0; i < 4; i++ )
    {
        // regular multiplication preserves the state (infinite/regular) of the pdf
        ARPDFVAL_PI(*pr,i) *= d0;
    }
}

void arpdfvalue_dp_mul_p(
        double        d0,
        ArPDFValue  * p0,
        ArPDFValue  * pr
        )
{
    ASSERT_NONNEGATIVE_DOUBLE( d0 ); // negative numbers would break how we treat infinite values

    for ( int i = 0; i < 4; i++ )
    {
        // regular multiplication preserves the state (infinite/regular) of the pdf
        ARPDFVAL_PI(*pr,i) = d0 * ARPDFVAL_PI(*p0,i);
    }
}

void arpdfvalue_s_mul_p(
        ArSpectralSample  * s0,
        ArPDFValue        * pr
        )
{
    ASSERT_NONNEGATIVE_DOUBLE( SPS_CI(*s0,0) );
    ASSERT_NONNEGATIVE_DOUBLE( SPS_CI(*s0,1) );
    ASSERT_NONNEGATIVE_DOUBLE( SPS_CI(*s0,2) );
    ASSERT_NONNEGATIVE_DOUBLE( SPS_CI(*s0,3) );
    
    for ( int i = 0; i < 4; i++ )
    {
        // regular multiplication preserves the state (infinite/regular) of the pdf
        ARPDFVAL_PI(*pr,i) *= SPS_CI(*s0, i);
    }
}

void arpdfvalue_sp_mul_p(
        ArSpectralSample  * s0,
        ArPDFValue        * p0,
        ArPDFValue        * pr
        )
{
    ASSERT_NONNEGATIVE_DOUBLE( SPS_CI(*s0,0) );
    ASSERT_NONNEGATIVE_DOUBLE( SPS_CI(*s0,1) );
    ASSERT_NONNEGATIVE_DOUBLE( SPS_CI(*s0,2) );
    ASSERT_NONNEGATIVE_DOUBLE( SPS_CI(*s0,3) );
    
    for ( int i = 0; i < 4; i++ )
    {
        // regular multiplication preserves the state (infinite/regular) of the pdf
        ARPDFVAL_PI(*pr,i) = SPS_CI(*s0, i) * ARPDFVAL_PI(*p0,i);
    }
}


void arpdfvalue_p_mul_p(
        ArPDFValue  * p0,
        ArPDFValue  * pr
        )
{
    for ( int i = 0; i < 4; i++ )
    {
        // if either of the pdfs are infinite, the result should also be infinite
        // regular multiplication needs additional correction only in the case that both of the pdfs were infinite to begin with
        if (   ARPDFVALUE_IS_INFINITE_I(*p0,i)
            && ARPDFVALUE_IS_INFINITE_I(*pr,i) )
        {
            ARPDFVAL_PI(*pr,i) *= ARPDFVALUE_INFINITE;
        }

        ARPDFVAL_PI(*pr,i) *= ARPDFVAL_PI(*p0,i);
    }
}

void arpdfvalue_pp_mul_p(
        ArPDFValue  * p0,
        ArPDFValue  * p1,
        ArPDFValue  * pr
        )
{
    for ( int i = 0; i < 4; i++ )
    {
        ARPDFVAL_PI(*pr,i) = ARPDFVAL_PI(*p0,i) * ARPDFVAL_PI(*p1,i);
        
        // same as above
        if (   ARPDFVALUE_IS_INFINITE_I(*p0,i)
            && ARPDFVALUE_IS_INFINITE_I(*p1,i) )
        {
            ARPDFVAL_PI(*pr,i) *= ARPDFVALUE_INFINITE;
        }
    }
}

double arpdfvalue_pp_ratio(
        ArPDFValue  * p0,
        ArPDFValue  * p1
        )
{
    if (   ! ARPDFVALUE_IS_INFINITE(*p0)
        &&   ARPDFVALUE_IS_INFINITE(*p1) )
        return 0.0;

    if (     ARPDFVALUE_IS_INFINITE(*p0)
        && ! ARPDFVALUE_IS_INFINITE(*p1) )
        return 1.0; //MATH_HUGE_DOUBLE

    if ( ARPDFVALUE_MAIN(*p1) > 0.0 )
        return ARPDFVALUE_MAIN(*p0) / ARPDFVALUE_MAIN(*p1);
    else
    {
        if ( ARPDFVALUE_MAIN(*p0) > 0.0 )
            return 1.0; //MATH_HUGE_DOUBLE
        else
            return 0.0;
    }
}

void arpdfvalue_pp_div_p(
        ArPDFValue  * p0,
        ArPDFValue  * p1,
        ArPDFValue  * pr
        )
{
    for ( int i = 0; i < 4; i++ )
    {
        if (     ARPDFVALUE_IS_INFINITE_I(*p0,i)
            &&   ARPDFVALUE_IS_INFINITE_I(*p1,i) )
        {
            // regular division, preserve infinite state
            ARPDFVAL_PI(*pr,i) = ARPDFVALUE_INFINITE * ARPDFVAL_PI(*p0,i) / ARPDFVAL_PI(*p1,i);
            
            continue;
        }

        if (   ! ARPDFVALUE_IS_INFINITE_I(*p0,i)
            &&   ARPDFVALUE_IS_INFINITE_I(*p1,i) )
        {
            // v / inf = 0
            ARPDFVAL_PI(*pr,i) = 0.0;
            
            continue;
        }

        if (     ARPDFVALUE_IS_INFINITE_I(*p0,i)
            && ! ARPDFVALUE_IS_INFINITE_I(*p1,i) )
        {
            // inf / v = inf
            ARPDFVAL_PI(*pr,i) = ARPDFVAL_PI(*p0,i);
            
            continue;
        }

        // only case left is where both pdfs are regular
        if ( ARPDFVAL_PI(*p1,i) > 0.0 )
            ARPDFVAL_PI(*pr,i) =
                ARPDFVAL_PI(*p0,i) / ARPDFVAL_PI(*p1,i);
        else
        {
            if ( ARPDFVAL_PI(*p0,i) > 0.0 )
                ARPDFVAL_PI(*pr,i) = MATH_HUGE_DOUBLE;
            else
                ARPDFVAL_PI(*pr,i) = 0.0;
        }
    }
}

void arpdfvalue_sqr_p(
        ArPDFValue  * pr
        )
{
    for ( int i = 0; i < 4; i++ )
    {
        ARPDFVAL_PI(*pr,i) = M_SQR(ARPDFVAL_PI(*pr,i))
            * (ARPDFVALUE_IS_INFINITE_I(*pr,i) ? ARPDFVALUE_INFINITE : 1); // preserve infinity state
    }
}

BOOL arpdfvalue_p_is_nonzero(
        ArPDFValue  * p0
        )
{
    return (   ARPDFVALUE_IS_INFINITE(*p0)
            || ARPDFVALUE_MAIN(*p0) > 0.0 );
}

void arpdfvalue_ppd_interpol_p(
        ArPDFValue  * p0,
        ArPDFValue  * p1,
        double        d0,
        ArPDFValue  * pr
        )
{
    for ( int i = 0; i < 4; i++ )
    {
        if ((     ARPDFVALUE_IS_INFINITE_I(*p0,i)
             &&   ARPDFVALUE_IS_INFINITE_I(*p1,i))
            || (! ARPDFVALUE_IS_INFINITE_I(*p0,i)
             && ! ARPDFVALUE_IS_INFINITE_I(*p1,i))
            )
        {
            // if both pdfs are of the same type, regular interpolation can take place
            // as they are both either positive (regular pdf) or both negative (infinite pdf)
            
            ARPDFVAL_PI(*pr,i) =
                  ( 1.0 - d0 ) * ARPDFVAL_PI(*p0,i)
                + (       d0 ) * ARPDFVAL_PI(*p1,i);
            
            continue;
        }

        if ( ARPDFVALUE_IS_INFINITE_I(*p0,i) )
        {
            if ( d0 == 1.0 )
            {
                ARPDFVAL_PI(*pr,i) = ARPDFVAL_PI(*p1,i);
            }
            else
            {
                ARPDFVAL_PI(*pr,i) = ARPDFVAL_PI(*p0,i); // the infinite one
            }
            
            continue;
        }

        if ( ARPDFVALUE_IS_INFINITE_I(*p1,i) )
        {
            if ( d0 == 0.0 )
            {
                ARPDFVAL_PI(*pr,i) = ARPDFVAL_PI(*p0,i);
            }
            else
            {
                ARPDFVAL_PI(*pr,i) = ARPDFVAL_PI(*p1,i); // the infinite one
            }

            continue;
        }
    }
}

/*
    Differences between p_mul_p, p_combine_p, p_concat_p, p_reverse_concat_p for operands (Value, IsInfinite):
    
    (A, a)       mul      (B, b) = (A * B, a || b)
    (A, a)     combine    (B, b) = (A * B, a && b)
    (A, a)     concat     (B, b) = (A * B, b)
    (A, a) reverse_concat (B, b) = (A * B, a)
 
    (and the arguments are p_op_p( (B, b), (A, a) ) and pp_op_p( (B, b), (A, a), result ) )
*/

void arpdfvalue_p_combine_p(
        ArPDFValue  * p0,
        ArPDFValue  * pr
        )
{
    for ( int i = 0; i < 4; i++ )
    {
        // if both of the pdfs are infinite, the result should also be infinite, otherwise regular
        // regular multiplication needs additional correction only in the case that exactly one of the pdfs were infinite to begin with
        if ((    ARPDFVALUE_IS_INFINITE_I(*p0,i)
             && !ARPDFVALUE_IS_INFINITE_I(*pr,i))
            || (!ARPDFVALUE_IS_INFINITE_I(*p0,i)
             &&  ARPDFVALUE_IS_INFINITE_I(*pr,i)))
        {
            ARPDFVAL_PI(*pr,i) *= ARPDFVALUE_INFINITE;
        }

        ARPDFVAL_PI(*pr,i) *= ARPDFVAL_PI(*p0,i);
    }
}

void arpdfvalue_p_concat_p(
        ArPDFValue  * p0,
        ArPDFValue  * pr
        )
{
    for ( int i = 0; i < 4; i++ )
    {
        // keep the state (regular/infinite) of p0, discard original state of pr
        ARPDFVAL_PI(*pr,i) = M_ABS(ARPDFVAL_PI(*pr,i)) * ARPDFVAL_PI(*p0,i);
    }
}

void arpdfvalue_pp_concat_p(
        ArPDFValue  * p0,
        ArPDFValue  * p1,
        ArPDFValue  * pr
        )
{
    for ( int i = 0; i < 4; i++ )
    {
        // keep the state (regular/infinite) of p0, discard original state of pr
        ARPDFVAL_PI(*pr,i) = M_ABS(ARPDFVAL_PI(*p1,i)) * ARPDFVAL_PI(*p0,i);
    }
}

void arpdfvalue_p_reverse_concat_p(
        ArPDFValue  * p0,
        ArPDFValue  * pr
        )
{
    for ( int i = 0; i < 4; i++ )
    {
        // keep the state (regular/infinite) of pr, discard original state of p0
        ARPDFVAL_PI(*pr,i) *= M_ABS(ARPDFVAL_PI(*p0,i));
    }
}


void arpdfvalue_sum_p(
        ArPDFValue  * pr
        )
{
    // skip the first element, as it is already initialized
    for ( int i = 1; i < 4; i++ )
    {
        if(ARPDFVALUE_IS_INFINITE_I(*pr, i))
        {
            if(!ARPDFVALUE_IS_INFINITE_I(*pr, 0))
            {
                // copy the infinite pdf over
                ARPDFVAL_PI(*pr,0) = ARPDFVAL_PI(*pr,i);
                continue;
            }
        }
        else
            if(ARPDFVALUE_IS_INFINITE_I(*pr, 0))
                continue; // preserve the infinite pdf
        
        // only the same state (regular/infinite) is left
        ARPDFVAL_PI(*pr,0) += ARPDFVAL_PI(*pr,i);
    }
}

// ===========================================================================
