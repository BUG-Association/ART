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

#include "ArOrder.h"

void arorder_init_permutation(
        unsigned long number,
        unsigned long * permutation
        )
{
    unsigned long i;
    for (i = 0; i < number; i++)
        permutation[i] = i;
}

#define ARORDER_QSORT_PIVOT_M(_p,_l,_m,_r) \
        do { \
            ASSIGN(_p, EL(_m)); \
        } \
        while (0)


#define ARORDER_QSORT_PIVOT(_p,_l,_m,_r) \
        do { \
            if (ORDER(EL(_l), <, EL(_m))) \
            { \
                if (ORDER(EL(_m), <, EL(_r))) \
                { \
                    ASSIGN(_p, EL(_m)); \
                    ASSIGN(EL(_m), EL(_l)); \
                } \
                else \
                { \
                    if (ORDER(EL(_l), <, EL(_r))) \
                    { \
                        ASSIGN(_p, EL(_r)); \
                        ASSIGN(EL(_r), EL(_l)); \
                    } \
                    else \
                        ASSIGN(_p, EL(_l)); \
                } \
            } \
            else \
            { \
                if (ORDER(EL(_r), <, EL(_m))) \
                { \
                    ASSIGN(_p, EL(_m)); \
                    ASSIGN(EL(_m), EL(_l)); \
                } \
                else \
                { \
                    if (ORDER(EL(_r), <, EL(_l))) \
                    { \
                        ASSIGN(_p, EL(_r)); \
                        ASSIGN(EL(_r), EL(_l)); \
                    } \
                    else \
                        ASSIGN(_p, EL(_l)); \
                } \
            } \
        } \
        while (0)


#define ARORDER_QSORT_STEP(_p,_l,_r) \
        while (1) \
        { \
            while (INDEX(_l) < INDEX(_r) && ORDER(_p, <=, EL(_r))) \
                DEC(_r); \
            if (INDEX(_l) == INDEX(_r)) \
            { \
                ASSIGN(EL(_l), _p); \
                break; \
            } \
            ASSIGN(EL(_l), EL(_r)); \
            INC(_l); \
            while (INDEX(_l) < INDEX(_r) && ORDER(EL(_l), <=, _p)) \
                INC(_l); \
            if (INDEX(_l) == INDEX(_r)) \
            { \
                ASSIGN(EL(_r), _p); \
                break; \
            } \
            ASSIGN(EL(_r), EL(_l)); \
            DEC(_r); \
        }

#define ARORDER_SORT_3(_store,_0,_1,_2) \
        do { \
            if (ORDER(EL(_0), >, EL(_1))) \
            { \
                if (ORDER(EL(_0), >, EL(_2)))   /* EL(_0) is biggest */ \
                { \
                    ASSIGN(_store, EL(_2)); \
                    ASSIGN(EL(_2), EL(_0)); \
                    if (ORDER(EL(_1), >, _store)) \
                        ASSIGN(EL(_0),_store); \
                    else \
                    { \
                        ASSIGN(EL(_0),EL(_1)); \
                        ASSIGN(EL(_1),_store); \
                    } \
                } \
                else \
                { \
                    ASSIGN(_store, EL(_1)); \
                    ASSIGN(EL(_1), EL(_0)); \
                    ASSIGN(EL(_0), _store); \
                } \
            } \
            else \
            { \
                if (ORDER(EL(_1), >, EL(_2)))   /* EL(_1) is biggest */ \
                { \
                    ASSIGN(_store, EL(_2)); \
                    ASSIGN(EL(_2), EL(_1)); \
                    if (ORDER(EL(_0), >, _store)) \
                    { \
                        ASSIGN(EL(_1), EL(_0)); \
                        ASSIGN(EL(_0), _store); \
                    } \
                    else \
                        ASSIGN(EL(_1), _store); \
                } \
            } \
        } \
        while (0)

#define ARORDER_SORT_2(_store,_l,_r) \
        do { \
            if (ORDER(EL(_r), <, EL(_l))) \
            { \
                ASSIGN(_store,  EL(_l)); \
                ASSIGN(EL(_l), EL(_r)); \
                ASSIGN(EL(_r), _store); \
            } \
        } \
        while (0)

#define LENGTH(_head,_tail)     (INDEX(_tail)-INDEX(_head))

#define INDEX(_i)               (_i)
#define INC(_i)                 (++(_i))
#define DEC(_i)                 (--(_i))

#define ASSIGN(_dst,_src)       (_dst = _src)
#define EL(_i)                  perm[_i]
#define OBJ(_index)             (array+size*(_index))
#define ORDER(_0,_cmp,_1)       ARORDER(OBJ(_0),_cmp,OBJ(_1),order)


int aroder_permutation_is_correct(
        const void * array,
        unsigned long size,
        unsigned long number,
        ArOrderFunction order,
        unsigned long * perm
        )
{
    unsigned long i = 0;
    unsigned long j = 1;

    while (INDEX(j) < number)
    {
        if (ORDER(EL(i), >, EL(j))) return 0;
        i = j; INC(j);
    }
    return 1;
}

void arorder_permutation_heapsort(
        const void * array,
        unsigned long size,
        unsigned long number,
        ArOrderFunction order,
        unsigned long * perm
        )
{
    unsigned long store;
    unsigned long h;
    for (h = 1; h < number; h++)
    {
        unsigned long j = h;
        unsigned long i = (j-1)/2;

        ASSIGN(store, EL(h));

        while (j > 0 && ORDER(EL(i), <, store))
        {
            ASSIGN(EL(j), EL(i));
            j = i; i = (i-1)/2;
        }
        ASSIGN(EL(j), store);
    }

    for (h = number-1; h > 0; h--)
    {
        unsigned long i = 0;
        unsigned long j = 1;

        ASSIGN(store, EL(h));
        ASSIGN(EL(h), EL(0));

        while (j < h)
        {
            unsigned long k = j+1;
            if (k < h && ORDER(EL(j), <, EL(k)))
                j = k;

            if (ORDER(store, <, EL(j)))
            {
                ASSIGN(EL(i), EL(j));
                j = 2 * i + 1;
                i = j;
            }
            else
            {
                ASSIGN(EL(i), store);
                break;
            }
        }
    }
}

void arorder_permutation_quicksort_slow(
        void * array,
        unsigned long size,
        unsigned long number,
        ArOrderFunction order,
        unsigned long * perm
        )
{
    unsigned long pivot;
    while (number > 3)
    {
        long left = 0;
        long right = number;

        DEC(right);
        ASSIGN(pivot, EL(0));
        ARORDER_QSORT_STEP(pivot, left, right);
        INC(right);

        arorder_permutation_quicksort_slow(array, size, number-right,
                                           order, perm+right);
        number = left;
    }
    if (number == 3)
    {
        ARORDER_SORT_3(pivot,0,1,2);
    }
    else if (number == 2)
    {
        ARORDER_SORT_2(pivot,0,1);
    }
}


void arorder_permutation_quicksort(
        const void * array,
        unsigned long size,
        unsigned long number,
        ArOrderFunction order,
        unsigned long * perm
        )
{
    unsigned long pivot;
    while (number > 3)
    {
        unsigned long left = 0;
        unsigned long right = number;
        unsigned long mid = number/2;

        DEC(right);
        ARORDER_QSORT_PIVOT(pivot, left, mid, right);
        ARORDER_QSORT_STEP(pivot, left, right);
        INC(right);

        arorder_permutation_quicksort(array, size, number-right,
                                      order, perm+right);
        number = left;
    }
    if (number == 3)
    {
        ARORDER_SORT_3(pivot,0,1,2);
    }
    else if (number == 2)
    {
        ARORDER_SORT_2(pivot,0,1);
    }
}

void arorder_permutation_quickmedian(
        const void * array,
        unsigned long size,
        unsigned long number,
        ArOrderFunction order,
        unsigned long * perm
        )
{
    unsigned long head = 0;
    unsigned long tail = number;
    unsigned long median = number/2;
    unsigned long pivot;
    while (1)
    {
        if (tail - head > 2)
        {
            unsigned long left = head;
            unsigned long right = tail;
            unsigned long mid = (head + tail)/2;

            DEC(right);
            ARORDER_QSORT_PIVOT(pivot, left, mid, right);
            ARORDER_QSORT_STEP(pivot, left, right);
            INC(right);

            if (INDEX(median) < INDEX(left))
                tail = left;
            else if (INDEX(median) > INDEX(left))
                head = right;
            else
                break;
        }
        else if (tail - head == 2)
        {
            DEC(tail);
            ARORDER_SORT_2(pivot,head,tail);
            break;
        }
        else
            break;
    }
}

#undef EL
#undef ORDER
#undef ASSIGN

/* ===========================================================================
        Direct element sorting.
=========================================================================== */
#define EL(_index)              (array+size*(_index))
#define ORDER(_one,_cmp,_two)   ARORDER(_one, _cmp, _two, order)
#define ASSIGN(_dst,_src) \
        do { \
            unsigned long _i; Byte * _d = _dst; Byte * _s = _src; \
            for (_i = 0; _i < size; _i++) { _d[_i] = _s[_i]; } \
        } \
        while (0)

int aroder_elements_is_correct(
        const void * array,
        unsigned long size,
        unsigned long number,
        ArOrderFunction order
        )
{
    unsigned long i = 0;
    unsigned long j = 1;

    while (INDEX(j) < number)
    {
        if (ORDER(EL(i), >, EL(j))) return 0;
        i = j; INC(j);
    }
    return 1;
}


void arorder_elements_quicksort(
        void             * array,
        unsigned long      size,
        unsigned long      number,
        unsigned long      recdepth,
        ArOrderFunction    order
        )
{
    void * pivot = ALLOC_ARRAY( Byte, size );
    
    while ( number > 3 )
    {
        unsigned long  left = 0;
        unsigned long  right = number;
        unsigned long  mid = number/2;

        DEC( right );
        ARORDER_QSORT_PIVOT( pivot, left, mid, right );

//   leave this here: this prints the ratio of entries equal to the pivot
//   these are causing the deep recursion, so some heuristic that works
//   on this will be better than the stupid "we hit recursion level x,
//   and switch to median sort" that is in place as a band aid now will
//   eventually be a better solution.

//        if ( recdepth > 10 )
//        {
//            unsigned long  equal = 0;
//            for ( int i = 0; i < number; i++ )
//            {
//                if ( ORDER(EL(i), ==, pivot) )
//                    equal++;
//            }
//            
//            debugprintf("%f ",(equal*1.0)/number)
//        }
        ARORDER_QSORT_STEP( pivot, left, right );
        INC( right );
        
        if ( recdepth > 1000 )
        {
            arorder_elements_quickmedian(
                  array + size * right,
                  size,
                  number - right,
                  order
                );
        }
        else
        {
            arorder_elements_quicksort(
                  array + size * right,
                  size,
                  number - right,
                  recdepth + 1,
                  order
                );
        }

        number = left;
    }
    
    if ( number == 3 )
    {
        ARORDER_SORT_3(pivot,0,1,2);
    }
    else if (number == 2)
    {
        ARORDER_SORT_2(pivot,0,1);
    }
    
    FREE(pivot);
}

void arorder_scramble(
        ART_GV         * art_gv,
        void           * array,
        unsigned long    size,
        unsigned long    number
        )
{
    void  * temp = ALLOC_ARRAY( Byte, size );

    for ( unsigned long i = 0; i < number; i++ )
    {
        unsigned long  newIndex = RANDOM_UNIFORM(art_gv) * number;
        
        if ( newIndex + 1 > number )
        {
            newIndex = 0;
        }
        
        if ( newIndex != i )
        {
            ASSIGN(temp, EL(i));
            ASSIGN(EL(i), EL(newIndex));
            ASSIGN(EL(newIndex), temp);
        }
    }

    FREE(temp);
}

void arorder_elements_quickmedian(
        void * array,
        unsigned long size,
        unsigned long number,
        ArOrderFunction order
        )
{
    long head = 0;
    long tail = number;
    long median = number/2;
    void * pivot = ALLOC_ARRAY(Byte, size);
    long length = number;
    while (length > 3)
    {
        long left = head;
        long right = tail;
        long mid = (head + tail)/2;

        DEC(right);
        ARORDER_QSORT_PIVOT(pivot, left, mid, right);
        ARORDER_QSORT_STEP(pivot, left, right);
        INC(right);

        if (INDEX(median) <= INDEX(left)) tail = left;
        if (INDEX(median) >= INDEX(left)) head = right;

        length = LENGTH(head,tail);
    }
    if (length == 3)
    {
        long mid = head;
        INC(mid);
        DEC(tail);
        ARORDER_SORT_3(pivot, head, mid, tail);
    }
    else if (length == 2)
    {
        DEC(tail);
        ARORDER_SORT_2(pivot,head,tail);
    }
    FREE(pivot);
}

int aroder_is_correct(
        const void * array,
        unsigned long size,
        unsigned long number,
        ArOrderFunction order,
        unsigned long * perm
        )
{
    if (perm == NULL)
        return aroder_elements_is_correct(array, size, number, order);
    else
        return aroder_permutation_is_correct(array, size, number, order, perm);
}

void arorder_quicksort(
        void             * array,
        unsigned long      size, // of the elements
        unsigned long      number,
        ArOrderFunction    order,
        unsigned long    * permutation
        )
{
    if ( permutation == NULL )
    {
        arorder_elements_quicksort(
              array,
              size,
              number,
              0,
              order
            );
    }
    else
    {
        arorder_permutation_quicksort(
              array,
              size,
              number,
              order,
              permutation
            );
    }
}

#undef EL
#undef ORDER
#undef ASSIGN

#undef  INDEX
#undef  INC
#undef  DEC

void arorder_quickmedian(
        void * array,
        unsigned long element_size,
        unsigned long number,
        ArOrderFunction order,
        unsigned long * permutation
        )
{
    if (permutation == NULL)
        arorder_elements_quickmedian(array, element_size, number, order);
    else
        arorder_permutation_quickmedian(array, element_size, number, order,
                                        permutation);
}

/* ======================================================================== */
