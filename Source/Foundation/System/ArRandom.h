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

#ifndef _ART_MATH_ARRANDOM_H_
#define _ART_MATH_ARRANDOM_H_

#include "ART_SystemDatatypes.h"
#include "ART_ModuleManagement.h"

ART_MODULE_INTERFACE(ArRandom)


/* ---------------------------------------------------------------------------
    'arrandom_nr0_...'
        Adapted from 'ran0', the minimal random generator of Numerical
        Recipes in C.
--------------------------------------------------------------------------- */

typedef struct ArRandomNR0
{
    int32_t  i32;
}
ArRandomNR0;

void arrandom_nr0_seed(
        ArRandomNR0 * state,
        long seed
        );

void arrandom_nr0_step(
        ArRandomNR0 * state
        );

unsigned long arrandom_nr0_ulong(
        ArRandomNR0 * state
        );

double arrandom_nr0_double(
        ArRandomNR0 * state
        );


/* ---------------------------------------------------------------------------
    'arrandom_nr1_...'
        Adapted from 'ran1', the second random generator of Numerical Recipes
        in C.
--------------------------------------------------------------------------- */

#define ARRANDOM_NR1_NTAB 32

typedef struct ArRandomNR1
{
    int32_t  idum;
    int32_t  iy;
    int32_t  iv[ARRANDOM_NR1_NTAB];
}
ArRandomNR1;

void arrandom_nr1_seed(
        ArRandomNR1 * state,
        long seed
        );

void arrandom_nr1_step(
        ArRandomNR1 * state
        );

unsigned long arrandom_nr1_ulong(
        ArRandomNR1 * state
        );

double arrandom_nr1_double(
        ArRandomNR1 * state
        );


/* ---------------------------------------------------------------------------
    'arrandom_nr2_...'
        Adapted from 'ran2', the second random generator of Numerical Recipes
        in C.
--------------------------------------------------------------------------- */

#define ARRANDOM_NR2_NTAB 32

typedef struct ArRandomNR2
{
    int32_t  idum;
    int32_t  idum2;
    int32_t  iy;
    int32_t  iv[ARRANDOM_NR2_NTAB];
}
ArRandomNR2;

void arrandom_nr2_seed(
        ArRandomNR2 * state,
        long seed
        );

void arrandom_nr2_step(
        ArRandomNR2 * state
        );

unsigned long arrandom_nr2_ulong(
        ArRandomNR2 * state
        );

double arrandom_nr2_double(
        ArRandomNR2 * state
        );

/* ---------------------------------------------------------------------------

    'arrandom_global_set_seed()'
    'arrandom_global()'

    There is a state entry in the global system variables for one of the
    random generators (currently, NR2) that is protected by a mutex lock, and
    should be used as the "global, system-wide random generator" for
    inter-platform reproducibility of results that depend on random decisions
    made during system startup (i.e. before any algorithm-specific RG is
    used).
    
    Please note that if no global random seed is specified, the system will
    always use exactly the same, in-built seed value. This way, repeated runs
    of the renderer yield exactly the same result.

------------------------------------------------------------------------aw- */

double arrandom_global(
        ART_GV  * agv
        );

void arrandom_global_set_seed(
        ART_GV  * art_gv,
        long      seed
        );

long arrandom_global_seed(
        ART_GV  * agv
        );

#define RANDOM_UNIFORM(__agv)    arrandom_global(__agv)


/* ===========================================================================
   RI_vdC(), RI_S() and RI_LP() are from
   Kollig, Keller: Efficient Multidimensional Sampling. (Eurographics 2002)
   Randomized digit scrambling is activated by calling the functions with r>0
   --------------------------------------------------------------------gz--- */

double arrandom_RI_vdC(UInt32 bits, UInt32 r);
double arrandom_RI_S(UInt32 i, UInt32 r);
double arrandom_RI_LP(UInt32 i, UInt32 r);


#endif /* _ART_MATH_ARRANDOM_H_ */
/* ======================================================================== */
