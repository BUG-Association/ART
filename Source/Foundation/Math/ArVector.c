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

#include "Constants.h"
#include "Functions.h"

#define ART_MATH_VECTOR_NO_INLINE
#include "ArVector.h"

void vector_hartley_transform_simple(
        double         * v,
        unsigned long    size
        )
{
    unsigned long old_len = 0;          /* initialize to avoid warning */
    unsigned long len;
    unsigned long new_len = 0;          /* initialize to avoid warning */

    if (size == 0 || (size & (size-1)))
    {
        ART_ERRORHANDLING_FATAL_ERROR(
            ERROR_VECTOR_LENGTH_D_NOT_POWER_OF_2
            ,(int)size
            );
        return;
    }
    else
    {
        /* -------------------------------------------------------------------
            The bit-reversal part of the function.
        ------------------------------------------------------------------- */
        long highest_bit = size >> 1;
        long second_highest_bit = highest_bit >> 1;
        long third_highest_bit = second_highest_bit >> 1;
        unsigned long index = 0;
        unsigned long reversed = 0;

        while (index < size)
        {
            if (index < reversed)
            {
                double help = v[reversed];
                v[reversed] = v[index];
                v[index] = help;
            }
            index++;
            reversed ^= highest_bit;
            if ((index & 1) == 0)
            {
                long reverse_bit = third_highest_bit;
                long bit;
                reversed ^= second_highest_bit;
                for (bit = 2; (index & bit) == 0; bit <<= 1)
                {
                    reversed ^= reverse_bit; reverse_bit >>= 1;
                }
            }
        }
    }

    /* -----------------------------------------------------------------------
        The transforming part of the function.  It assumes that all indices
        are already bit-reversed.  For successive evaluation of the
        trigonometric functions the following recurrence is used:

                a = 2 sin(square(d/2))
                b  = sin d

                cos(t + d) = cos t - [ a * cos t + b sin t ]
                sin(t + d) = sin t - [ a * sin t - b cos t ]

        The algorithm is based on the following recursion:

                H[f] = Heven[f]
                        + cos(2 PI f / n) Hodd[f]
                        + sin(2 PI f / n) Hodd[n-f]     f in [0, n-1]

                Heven[n/2 + g] = Heven[g]
                Hodd [n/2 + g] = Hodd [g]               g in [0, n/2-1]
    ----------------------------------------------------------------------- */
    for (len = 1; len < size; old_len = len, len = new_len)
    {
        unsigned long i, j;
        double hi, hj;

        new_len = 2 * len;

        for (i = 0; i < size; i += new_len)             /* for all blocks */
        {
            j = i + len;                                /* special case: */
            hi = v[i]; hj = v[j];
            v[i] = hi + hj;                             /* f = 0 */
            v[j] = hi - hj;                             /* f = PI */
        }
        if (len < 2) continue;

        for (i = old_len; i < size; i += new_len)       /* for all blocks */
        {
            j = i + len;                                /* special case: */
            hi = v[i]; hj = v[j];
            v[i] = hi + hj;                             /* f = PI/2 */
            v[j] = hi - hj;                             /* f = 3 * PI/2 */
        }

        if (len >= 4)
        {
            double d = MATH_2_MUL_PI / new_len;

            double a = 2.0 * M_SQR(sin(d * 0.5));       /* initialize trig. */
            double b = sin(d);                          /* recurrence */

            double cos_t = 1.0;
            double sin_t = 0.0;

            unsigned long f;

            /* ---------------------------------------------------------------
                To increase the locality of references, the following two
                loops could be exchanged, but then the trigonometric
                functions need to be tabulated.
            --------------------------------------------------------------- */

            for (f = 1; f < old_len; f++)               /* for all freqs in */
            {                                           /*   the first quad */
                double one = a * cos_t + b * sin_t;     /* trig. recurrence */
                double two = a * sin_t - b * cos_t;
                cos_t -= one;                           /*   cos (t + d) */
                sin_t -= two;                           /*   sin (t + d) */

                for (i = f, j = len - f;                /* for all blocks */
                     i < size;
                     i += new_len, j += new_len)
                {
                    long k = i + len;
                    long l = j + len;

                    one = cos_t * v[k] + sin_t * v[l];
                    two = cos_t * v[l] - sin_t * v[k];
                    hi = v[i]; hj = v[j];
                    v[i] = hi + one; v[k] = hi - one;   /* all four quads */
                    v[j] = hj - two; v[l] = hj + two;   /* (i,j,k,l) */
                }

            }
        }
    }
}

/* ---------------------------------------------------------------------------
    'vector_hartley_transform'
--------------------------------------------------------------------------- */
void vector_hartley_transform(
        double         * v,
        unsigned long    size,
        long             stride     /* element-element offset */
        )
{
    long end = size * stride;

    long old_len = 0;                   /* initialize to avoid warning */
    long len;
    long new_len = 0;                   /* initialize to avoid warning */

    if (size == 0 || (size & (size-1)))
    {
        ART_ERRORHANDLING_FATAL_ERROR(
            ERROR_VECTOR_LENGTH_D_NOT_POWER_OF_2
            ,(int)size
            );
        return;
    }
    else
    {
        /* -------------------------------------------------------------------
            The bit-reversal part of the function.
        ------------------------------------------------------------------- */
        long highest_bit = size >> 1;
        long second_highest_bit = highest_bit >> 1;
        long third_highest_bit = second_highest_bit >> 1;
        unsigned long index = 0;
        unsigned long reversed = 0;
        long position = 0;

        while (index < size)
        {
            if (index < reversed)
            {
                long rev_pos = reversed * stride;
                double help = v[rev_pos];
                v[rev_pos] = v[position];
                v[position] = help;
            }
            index++;
            position += stride;
            reversed ^= highest_bit;
            if ((index & 1) == 0)
            {
                long reverse_bit = third_highest_bit;
                long bit;
                reversed ^= second_highest_bit;
                for (bit = 2; (index & bit) == 0; bit <<= 1)
                {
                    reversed ^= reverse_bit; reverse_bit >>= 1;
                }
            }
        }
    }

    for (len = stride; len != end; old_len = len, len = new_len)
    {
        long i, j;
        double hi, hj;

        new_len = 2 * len;

        for (i = 0; i != end; i += new_len)             /* for all blocks */
        {
            j = i + len;                                /* special case: */
            hi = v[i]; hj = v[j];
            v[i] = hi + hj;                             /* f = 0 */
            v[j] = hi - hj;                             /* f = PI */
        }
        if (len < 2 * stride) continue;

        for (i = old_len; i != end+old_len; i += new_len) /* for all blocks */
        {
            j = i + len;                                /* special case: */
            hi = v[i]; hj = v[j];
            v[i] = hi + hj;                             /* f = PI/2 */
            v[j] = hi - hj;                             /* f = 3 * PI/2 */
        }

        if (len >= 4 * stride)
        {
            double d = MATH_2_MUL_PI / (new_len/stride);

            double a = 2.0 * M_SQR(sin(d * 0.5));       /* initialize trig. */
            double b = sin(d);                          /* recurrence */

            double cos_t = 1.0;
            double sin_t = 0.0;

            long f;

            /* ---------------------------------------------------------------
                To increse the locality of references, the following two
                loops could be exchanged, but then the trigonometric
                functions need to be tabulated.
            --------------------------------------------------------------- */

            for (f = stride;                            /* for all freqs in */
                 f != old_len;                          /*   the first quad */
                 f += stride)
            {
                double one = a * cos_t + b * sin_t;     /* trig. recurrence */
                double two = a * sin_t - b * cos_t;
                cos_t -= one;                           /*   cos (t + d) */
                sin_t -= two;                           /*   sin (t + d) */

                for (i = f, j = len - f;                /* for all blocks */
                     i != end+f;
                     i += new_len, j += new_len)
                {
                    long k = i + len;
                    long l = j + len;

                    one = cos_t * v[k] + sin_t * v[l];
                    two = cos_t * v[l] - sin_t * v[k];
                    hi = v[i]; hj = v[j];
                    v[i] = hi + one; v[k] = hi - one;   /* all four quads */
                    v[j] = hj - two; v[l] = hj + two;   /* (i,j,k,l) */
                }
            }
        }
    }
}

/* ======================================================================== */
