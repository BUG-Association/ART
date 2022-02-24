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

#ifndef _ART_FOUNDATION_DATASTRUCTURES_ARORDER_H_
#define _ART_FOUNDATION_DATASTRUCTURES_ARORDER_H_

/* ===========================================================================

    DESCRIPTION
        This module provides sorting routines that return the result of the
        sort in a permutation vector, and leave the original data array
        untouched.  This makes it easily possible to have different orders
        of the same data available at the same time.
=========================================================================== */

#include "ART_Foundation_System.h"

typedef enum ArOrderValue
{
    arorder_lessthan = -1,
    arorder_equal = 0,
    arorder_greaterthan = 1
}
ArOrderValue;

/* ---------------------------------------------------------------------------
    'ArOrderFunction'
        Analoguous to the ordering function needed by the standard liberary
        function 'qsort', an 'ArOrderFunction' needs to return an integer
        value smaller than, equal to, or greater than 0, when the first
        of its arguments is smaller than, equal to, or greater than the
        second one.
--------------------------------------------------------------------------- */
typedef int (*ArOrderFunction)(const void * a, const void * b);

#define ARORDER(_one,_cmp,_two,_ordfun) ((_ordfun)(_one,_two) _cmp 0)

/* ---------------------------------------------------------------------------
    'arorder_init_permutation'
        Initializes the permutation vector to contain the numbers from 0 to
        'number'-1.
--------------------------------------------------------------------------- */
void arorder_init_permutation(
        unsigned long number,
        unsigned long * permutation
        );

int aroder_is_correct(
        const void * array,
        unsigned long element_size,
        unsigned long number,
        ArOrderFunction order,
        unsigned long * permutation
        );

/* ---------------------------------------------------------------------------
    'arorder_heapsort'
        Standard heapsort algorithm.
--------------------------------------------------------------------------- */
void arorder_permutation_heapsort(
        const void * array,
        unsigned long element_size,
        unsigned long number,
        ArOrderFunction order,
        unsigned long * permutation
        );

/* ---------------------------------------------------------------------------
    'arorder_quicksort_slow'
        Standard quicksort algorithm that has a worst case Order of O(n^2).
--------------------------------------------------------------------------- */
void arorder_permutation_quicksort_slow(
        void * array,
        unsigned long element_size,
        unsigned long number,
        ArOrderFunction order,
        unsigned long * permutation
        );


/* ---------------------------------------------------------------------------
    'arorder_permutation_...'
        Permutation array based sorting and median algorithms.  These
        functions do not modify the given array of elements, they just
        modify the given permutation array to contain the desired order.
--------------------------------------------------------------------------- */
int aroder_permutation_is_correct(
        const void * array,
        unsigned long size,
        unsigned long number,
        ArOrderFunction order,
        unsigned long * perm
        );
void arorder_permutation_quicksort(
        const void * array,
        unsigned long element_size,
        unsigned long number,
        ArOrderFunction order,
        unsigned long * permutation
        );
void arorder_permutation_quickmedian(
        const void * array,
        unsigned long element_size,
        unsigned long number,
        ArOrderFunction order,
        unsigned long * permutation
        );

/* ---------------------------------------------------------------------------
    'arorder_elements_...'
--------------------------------------------------------------------------- */
int aroder_elements_is_correct(
        const void * array,
        unsigned long size,
        unsigned long number,
        ArOrderFunction order
        );

void arorder_elements_quickmedian(
        void * array,
        unsigned long element_size,
        unsigned long number,
        ArOrderFunction order
        );


/* ---------------------------------------------------------------------------
    'arorder_is_correct'
        Checks the order of the given array.
--------------------------------------------------------------------------- */

int aroder_is_correct(
        const void * array,
        unsigned long size,
        unsigned long number,
        ArOrderFunction order,
        unsigned long * perm
        );

/* ---------------------------------------------------------------------------
    'arorder_quicksort'
        Improved quicksort algorithm that uses the median of three elements
        as a pivot.  This is currently the fastest sorting algorithm in
        this module.  Note that if the 'permutation' parameter is specified,
        the array is left untouched, and the sorting is returned in this
        permutation vector.
--------------------------------------------------------------------------- */

void arorder_quicksort(
        void * array,
        unsigned long element_size,
        unsigned long number,
        ArOrderFunction order,
        unsigned long * permutation
        );

/* ---------------------------------------------------------------------------
    'arorder_quickmedian'
        Improved quick median algorithm that uses the median of three
        elements as a pivot.  This is currently the fastest median algorithm
        in this module.
--------------------------------------------------------------------------- */

void arorder_quickmedian(
        void * array,
        unsigned long element_size,
        unsigned long number,
        ArOrderFunction order,
        unsigned long * permutation
        );

/* ---------------------------------------------------------------------------
    'arorder_scamble'
        Scrambles the array order with one pass of random exchanges.
--------------------------------------------------------------------------- */

void arorder_scramble(
        ART_GV         * art_gv,
        void           * array,
        unsigned long    element_size,
        unsigned long    number
        );

#endif /* _ART_FOUNDATION_DATASTRUCTURES_ARORDER_H_ */
/* ======================================================================== */
