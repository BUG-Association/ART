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

#define ART_MODULE_NAME     ArRandom

#include "ArRandom.h"

#include "ART_SystemDatatypes.h"

#include <pthread.h>
#include <assert.h>             /* assert */

typedef struct ArRandom_GV
{
    pthread_mutex_t  protectedGlobalNR2_mutex;
    ArRandomNR2      protectedGlobalNR2_state;
    long             global_seed;
}
ArRandom_GV;

#define ARRANDOM_GV             art_gv->arrandom_gv
#define ARRANDOM_NR2_MUTEX      ARRANDOM_GV->protectedGlobalNR2_mutex
#define ARRANDOM_NR2_STATE      ARRANDOM_GV->protectedGlobalNR2_state
#define ARRANDOM_GLOBAL_SEED    ARRANDOM_GV->global_seed

ART_MODULE_INITIALISATION_FUNCTION
(
    ARRANDOM_GV = ALLOC( ArRandom_GV );

    pthread_mutex_init(
        & ARRANDOM_NR2_MUTEX,
          NULL
        );

    ARRANDOM_GLOBAL_SEED = 18091970;
    
    arrandom_nr2_seed(
        & ARRANDOM_NR2_STATE,
          ARRANDOM_GLOBAL_SEED
        );
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    pthread_mutex_destroy(
        & ARRANDOM_NR2_MUTEX
        );

    FREE( ARRANDOM_GV );
)


void arrandom_global_set_seed(
        ART_GV  * art_gv,
        long      seed
        )
{
    pthread_mutex_lock( & ARRANDOM_NR2_MUTEX );
    
    ARRANDOM_GLOBAL_SEED = seed;

    arrandom_nr2_seed(
        & ARRANDOM_NR2_STATE,
          ARRANDOM_GLOBAL_SEED
        );

    pthread_mutex_unlock( & ARRANDOM_NR2_MUTEX );
}

long arrandom_global_seed(
        ART_GV  * art_gv
        )
{
    return ARRANDOM_GLOBAL_SEED;
}


double arrandom_global(
        ART_GV  * art_gv
        )
{
    double  result;

    pthread_mutex_lock( & ARRANDOM_NR2_MUTEX );

    result = arrandom_nr2_double( & ARRANDOM_NR2_STATE );

    pthread_mutex_unlock( & ARRANDOM_NR2_MUTEX );

    return result;
}


#define ARRANDOM_SEED_MASK      1968071319

int32_t arrandom_linear_next(
        int32_t  value,
        int32_t  iq,
        int32_t  ia,
        int32_t  ir,
        int32_t  im
        )
{
    int32_t  k = value / iq;
    value = ia * ( value - k * iq ) - k * ir;
    return ( value >= 0 ? value : value + im );
}

/* ---------------------------------------------------------------------------
    'arrandom_nr0_...'
        Adapted from 'ran0', the minimal random generator of Numerical
        Recipes in C.
--------------------------------------------------------------------------- */

#define IA 16807
#define IM 2147483647
#define AM (1.0/(1.0+(double)IM))
#define IQ 127773
#define IR 2836

#define ARRANDOM_NR0_BITS               31

void arrandom_nr0_seed(
        ArRandomNR0 * state,
        long seed
        )
{
    seed &= ART_INT32_MAX;
    state->i32 = seed ^ ARRANDOM_SEED_MASK;
    if (state->i32 == 0) state->i32 = seed;
}

void arrandom_nr0_step(
        ArRandomNR0 * state
        )
{
    state->i32 = arrandom_linear_next(state->i32,IQ,IA,IR,IM);
}

unsigned long arrandom_nr0_ulong(
        ArRandomNR0 * state
        )
{
    arrandom_nr0_step(state);
    return state->i32;
}

double arrandom_nr0_double(
        ArRandomNR0 * state
        )
{
    arrandom_nr0_step(state);
    return AM * state->i32;
}

#undef IA
#undef IM
#undef AM
#undef IQ
#undef IR

/* ---------------------------------------------------------------------------
    'arrandom_nr1_...'
        Adapted from 'ran1', the second random generator of Numerical Recipes
        in C.
--------------------------------------------------------------------------- */

#define ARRANDOM_NR1_BITS               31

#define IM 2147483647
#define AM (1.0/(1.0+(double)IM))
#define IA 16807
#define IQ 127773
#define IR 2836
#define NDIV (1+(IM-1)/ARRANDOM_NR1_NTAB)

void arrandom_nr1_seed(
        ArRandomNR1 * state,
        long seed
        )
{
    int j;

    seed &= ART_INT32_MAX;
    state->idum = seed ^ ARRANDOM_SEED_MASK;
    if (state->idum == 0) state->idum = seed;

    for (j = ARRANDOM_NR1_NTAB+7; j >= 0; j--)
    {
        state->idum = arrandom_linear_next(state->idum,IQ,IA,IR,IM);
        if (j < ARRANDOM_NR1_NTAB) state->iv[j] = state->idum;
    }
    state->iy = state->iv[0];
}

void arrandom_nr1_step(
        ArRandomNR1 * state
        )
{
    long j;
    state->idum = arrandom_linear_next(state->idum,IQ,IA,IR,IM);
    j = state->iy / NDIV;
    state->iy = state->iv[j];
    state->iv[j] = state->idum;
}

unsigned long arrandom_nr1_ulong(
        ArRandomNR1 * state
        )
{
    arrandom_nr1_step(state);
    return state->iy;
}

double arrandom_nr1_double(
        ArRandomNR1 * state
        )
{
    arrandom_nr1_step(state);
    return AM * state->iy;
}

#undef IM
#undef AM
#undef IA
#undef IQ
#undef IR
#undef NDIV
/* ---------------------------------------------------------------------------
    'arrandom_nr2_...'
        Adapted from 'ran2', the second random generator of Numerical Recipes
        in C.
--------------------------------------------------------------------------- */

#define ARRANDOM_NR2_BITS               31

#define IM1 2147483563
#define IM2 2147483399
#define AM (1.0/(1.0+(double)IM1))
#define IMM1 (IM1-1)
#define IA1 40014
#define IA2 40692
#define IQ1 53668
#define IQ2 52774
#define IR1 12211
#define IR2 3791
#define NDIV (1+IMM1/ARRANDOM_NR2_NTAB)

void arrandom_nr2_seed(
        ArRandomNR2 * state,
        long seed
        )
{
    long  j;

    seed &= ART_INT32_MAX;
    state->idum = seed ^ ARRANDOM_SEED_MASK;
    if (state->idum == 0) state->idum = seed;

    state->idum2 = state->idum;

    for (j = ARRANDOM_NR2_NTAB+7; j >= 0; j--)
    {
        state->idum = arrandom_linear_next(state->idum,IQ1,IA1,IR1,IM1);
        if (j < ARRANDOM_NR2_NTAB) state->iv[j] = state->idum;
    }
    state->iy = state->iv[0];
}

void arrandom_nr2_step(
        ArRandomNR2 * state
        )
{
    long j;
    state->idum = arrandom_linear_next(state->idum,IQ1,IA1,IR1,IM1);
    state->idum2 = arrandom_linear_next(state->idum2,IQ2,IA2,IR2,IM2);
    j = state->iy / NDIV;
    state->iy = state->iv[j] - state->idum2;
    state->iv[j] = state->idum;
    if (state->iy < 1) state->iy += IMM1;
}

unsigned long arrandom_nr2_ulong(
        ArRandomNR2 * state
        )
{
    arrandom_nr2_step(state);
    return state->iy;
}

double arrandom_nr2_double(
        ArRandomNR2 * state
        )
{
    arrandom_nr2_step(state);
    return AM * state->iy;
}

#undef IM1
#undef IM2
#undef AM
#undef IMM1
#undef IA1
#undef IA2
#undef IQ1
#undef IQ2
#undef IR1
#undef IR2
#undef NDIV

/* ===========================================================================
   RI_vdC(), RI_S() and RI_LP() are from
   Kollig, Keller: Efficient Multidimensional Sampling. (Eurographics 2002)
   Randomized digit scrambling is activated by calling the functions with r>0
   --------------------------------------------------------------------gz--- */

double arrandom_RI_vdC(
        UInt32  bits,
        UInt32  r
        )
{
    bits = ( bits << 16) | (bits >> 16);
    bits = ((bits & 0x00ff00ff)  << 8) | ((bits & 0xff00ff00)  >> 8);
    bits = ((bits & 0x0f0f0f0f)  << 4) | ((bits & 0xf0f0f0f0)  >> 4);
    bits = ((bits & 0x33333333)  << 2) | ((bits & 0xcccccccc)  >> 2);
    bits = ((bits & 0x55555555)  << 1) | ((bits & 0xaaaaaaaa)  >> 1);
    bits ^= r;

    return (double) bits / (double) 0x100000000LL;
}

double arrandom_RI_S(
        UInt32  i,
        UInt32  r
        )
{
    for ( UInt32 v = 1 << 31; i; i >>= 1, v ^= v >> 1 )
        if ( i & 1 ) r ^= v;

    return (double) r / (double) 0x100000000LL;
}

double arrandom_RI_LP(
        UInt32  i,
        UInt32  r
        )
{
    for ( UInt32 v = 1 << 31; i; i >>= 1, v |= v >> 1 )
        if ( i & 1 ) r ^= v;

    return (double) r / (double) 0x100000000LL;
}

/* ======================================================================== */
