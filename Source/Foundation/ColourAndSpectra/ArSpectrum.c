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

#define ART_MODULE_NAME     ArSpectrum

#include "ArSpectrum.h"

#include "_ArSpectrum_GV.h"
#include "ART_Foundation_Math.h"


/* ---------------------------------------------------------------------------

    ARSPECTRUM_DEBUG_ASSERTIONS debug switch
    ========================================

    ***************

    IMPORTANT NOTE: the actual debug switch is located in _ArSpectrum_GV.h! The
    explanation is here, though, because practically the entire functionality
    that is triggered by the #define resides in this file!

    ***************

    Uncomment #define ARSPECTRUM_DEBUG_ASSERTIONS to enable a debug mechanism
    that checks memory management assertions for every operation on an
    ArSpectrum. This makes the calling code *extremely* slow, but is pretty
    much guaranteed to pinpoint the use of a) uninitialised and b) already
    freed instances of ArSpectrum.

    Basically, this option switches on a system that keeps track of every
    ArSpectrum that was ever allocated and freed, and throws an exception if
    any spc_... operation tries to use an ArSpectrum pointer that is not in
    the global list of already allocated ArSpectrums, or in the list of those
    that were allocated, but have already been freed.

    In order to pinpoint the offending code in the most efficient way,
    this option should be enabled, and the system then run via gdb.

    --->>>   NEVER leave this switched on if you are not    <<<---
    --->>>   performing this sort of debugging!             <<<---

    NOTE: the current DynArray-based implementation of these assertions
    is neither pretty nor efficient. In particular, it is very slow, and
    might be prone to use huge amounts of memory for long debugging runs.
    However, it works, and gets the job done, so here you are...
    (and the reason we did not use dynamic tables instead of arrays was
    that ArTable was still in a dodgy state at the time this was written)

------------------------------------------------------------------------aw- */

/* ---------------------------------------------------------------------------

    The normal assertion check is whether the arguments in question are

    a) not null,
    b) valid pointers (i.e. properly allocated instances of ArSpectrum),
    c) have been initialised (alloc and init are separate, after all), and
    d) have not been freed yet.

    The _NOINIT version of the assertion macros omits check c) - these
    are used by the initialisation functions, which are used to init
    newly allocated ArSpectrums.

------------------------------------------------------------------------aw- */

#ifdef ARSPECTRUM_DEBUG_ASSERTIONS

//   Assertion check a) - is the argument pointer a null pointer?

#define CHECK_ARSPECTRUM_DEBUG_ASSERTION_NULL_ARGUMENT(__var) \
do{ \
\
    if ( (__var) == 0 ) \
        ART_ERRORHANDLING_FATAL_ERROR("NULL argument " #__var); \
\
} while(0);

//   Assertion check b) - is the argument pointer a reference to an
//   ArSpectrum that was allocated at some point in time, but that has
//   already been freed in the meantime?

#define CHECK_ARSPECTRUM_DEBUG_ASSERTION_FREED_ARGUMENT(__var) \
do{ \
\
    int  freed = arintdynarray_size( & FREED_INSTANCE_ARRAY ); \
\
    int  foundInFreed = 0; \
\
    for ( int i = 0; i < freed; i++ ) \
    { \
        if (    arintdynarray_i( & FREED_INSTANCE_ARRAY, i ) \
             == ((int)(__var)) ) \
        { \
            foundInFreed = 1; \
            continue; \
        } \
    } \
\
    if ( foundInFreed ) \
        ART_ERRORHANDLING_FATAL_ERROR("freed argument " #__var); \
\
} while(0);

//   Assertion check c) - does the argument pointer refrence an
//   instance of ArSpectrum that has been properly allocated?

#define CHECK_ARSPECTRUM_DEBUG_ASSERTION_UNALLOCATED_ARGUMENT(__var) \
do{ \
\
    int  allocated = arintdynarray_size( & ALLOCATED_INSTANCE_ARRAY ); \
\
    int  foundInAllocated = 0; \
\
    for ( int i = 0; i < allocated; i++ ) \
    { \
        if (    arintdynarray_i( & ALLOCATED_INSTANCE_ARRAY, i ) \
             == ((int)(__var)) ) \
        { \
            foundInAllocated = 1; \
            continue; \
        } \
    } \
\
    if ( ! foundInAllocated ) \
        ART_ERRORHANDLING_FATAL_ERROR("unallocated argument " #__var); \
\
} while(0);

//   Assertion check d) - does the argument pointer refrence an
//   instance of ArSpectrum that has been properly initialised?

#define CHECK_ARSPECTRUM_DEBUG_ASSERTION_UNINITIALISED_ARGUMENT(__var) \
do{ \
\
    int  initialised = arintdynarray_size( & INITIALISED_INSTANCE_ARRAY ); \
\
    int  foundInInitialised = 0; \
\
    for ( int i = 0; i < initialised; i++ ) \
    { \
        if (    arintdynarray_i( & INITIALISED_INSTANCE_ARRAY, i ) \
             == ((int)(__var)) ) \
        { \
            foundInInitialised = 1; \
            continue; \
        } \
    } \
\
    if ( ! foundInInitialised ) \
        ART_ERRORHANDLING_FATAL_ERROR("uninitialised argument " #__var); \
\
} while(0);

//  _NOINIT version of the assertion block - without initialisation check

#define CHECK_ARSPECTRUM_DEBUG_ASSERTION_NOINIT(__var) \
do{ \
\
    pthread_mutex_lock( & INSTANCE_ARRAY_MUTEX ); \
\
    CHECK_ARSPECTRUM_DEBUG_ASSERTION_NULL_ARGUMENT(__var); \
    CHECK_ARSPECTRUM_DEBUG_ASSERTION_FREED_ARGUMENT(__var); \
    CHECK_ARSPECTRUM_DEBUG_ASSERTION_UNALLOCATED_ARGUMENT(__var); \
\
    pthread_mutex_unlock( & INSTANCE_ARRAY_MUTEX ); \
\
} while(0);

//   Full version of the assertion block - with initialisation check

#define CHECK_ARSPECTRUM_DEBUG_ASSERTION(__var) \
do{ \
\
    pthread_mutex_lock( & INSTANCE_ARRAY_MUTEX ); \
\
    CHECK_ARSPECTRUM_DEBUG_ASSERTION_NULL_ARGUMENT(__var); \
    CHECK_ARSPECTRUM_DEBUG_ASSERTION_FREED_ARGUMENT(__var); \
    CHECK_ARSPECTRUM_DEBUG_ASSERTION_UNALLOCATED_ARGUMENT(__var); \
    CHECK_ARSPECTRUM_DEBUG_ASSERTION_UNINITIALISED_ARGUMENT(__var); \
\
    pthread_mutex_unlock( & INSTANCE_ARRAY_MUTEX ); \
\
} while(0);

#define ARSPECTRUM_DEBUG_ASSERTIONS_REGISTER_AS_INITIALISED(__var) \
do{ \
\
    pthread_mutex_lock( & INSTANCE_ARRAY_MUTEX ); \
\
    int  initialised = arintdynarray_size( & INITIALISED_INSTANCE_ARRAY ); \
\
    int  foundInInitialised = 0; \
\
    for ( int i = 0; i < initialised; i++ ) \
    { \
        if (    arintdynarray_i( & INITIALISED_INSTANCE_ARRAY, i ) \
             == ((int)(__var)) ) \
        { \
            foundInInitialised = 1; \
            continue; \
        } \
    } \
\
    if ( ! foundInInitialised ) \
        arintdynarray_push( \
            & INITIALISED_INSTANCE_ARRAY, \
              (int)(__var) \
            ); \
\
    pthread_mutex_unlock( & INSTANCE_ARRAY_MUTEX ); \
\
} while(0);

#else // ARSPECTRUM_DEBUG_ASSERTIONS

#define ARSPECTRUM_DEBUG_ASSERTIONS_REGISTER_AS_INITIALISED(__var)
#define CHECK_ARSPECTRUM_DEBUG_ASSERTION(__var)
#define CHECK_ARSPECTRUM_DEBUG_ASSERTION_NOINIT(__var)

#endif // ARSPECTRUM_DEBUG_ASSERTIONS


//   Assembly of the actual assertion macros for the functions.
//   These of course expand to nothing if the debug option is not set.

#define CHECK_ARSPECTRUM_DEBUG_ASSERTIONS_NOINIT__C0 \
CHECK_ARSPECTRUM_DEBUG_ASSERTION_NOINIT(c0)

#define CHECK_ARSPECTRUM_DEBUG_ASSERTIONS_NOINIT__CR \
CHECK_ARSPECTRUM_DEBUG_ASSERTION_NOINIT(cr)

#define CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0 \
CHECK_ARSPECTRUM_DEBUG_ASSERTION(c0)

#define CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__CR \
CHECK_ARSPECTRUM_DEBUG_ASSERTION(cr)

#define CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0_CR \
CHECK_ARSPECTRUM_DEBUG_ASSERTION(c0) \
CHECK_ARSPECTRUM_DEBUG_ASSERTION(cr)

#define CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0_NICR \
CHECK_ARSPECTRUM_DEBUG_ASSERTION(c0) \
CHECK_ARSPECTRUM_DEBUG_ASSERTION_NOINIT(cr)

#define CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0_C1 \
CHECK_ARSPECTRUM_DEBUG_ASSERTION(c0) \
CHECK_ARSPECTRUM_DEBUG_ASSERTION(c1)

#define CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0_C1_NICR \
CHECK_ARSPECTRUM_DEBUG_ASSERTION(c0) \
CHECK_ARSPECTRUM_DEBUG_ASSERTION(c1) \
CHECK_ARSPECTRUM_DEBUG_ASSERTION_NOINIT(cr)

#define CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0_C1_C2_NICR \
CHECK_ARSPECTRUM_DEBUG_ASSERTION(c0) \
CHECK_ARSPECTRUM_DEBUG_ASSERTION(c1) \
CHECK_ARSPECTRUM_DEBUG_ASSERTION(c2) \
CHECK_ARSPECTRUM_DEBUG_ASSERTION_NOINIT(cr)


ART_MODULE_INITIALISATION_FUNCTION
(
    ArSpectrum_GV  * arspectrum_gv = ALLOC(ArSpectrum_GV);

    art_gv->arspectrum_gv = arspectrum_gv;

    ISR_HAS_BEEN_INITIALISED = 0;

    SPC_ZERO_GV = 0;
    SPC_UNIT_GV = 0;

#ifdef ARSPECTRUM_DEBUG_ASSERTIONS

    ALLOCATED_INSTANCE_ARRAY   = arintdynarray_init( 16 );
    INITIALISED_INSTANCE_ARRAY = arintdynarray_init( 16 );
    FREED_INSTANCE_ARRAY       = arintdynarray_init( 16 );

    pthread_mutex_init( & INSTANCE_ARRAY_MUTEX, NULL );

#endif
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    if ( SPC_ZERO_GV ) spc_free( art_gv, SPC_ZERO_GV );
    if ( SPC_UNIT_GV ) spc_free( art_gv, SPC_UNIT_GV );

#ifdef ARSPECTRUM_DEBUG_ASSERTIONS

    arintdynarray_free_contents( & ALLOCATED_INSTANCE_ARRAY );
    arintdynarray_free_contents( & INITIALISED_INSTANCE_ARRAY );
    arintdynarray_free_contents( & FREED_INSTANCE_ARRAY );

    pthread_mutex_destroy( & INSTANCE_ARRAY_MUTEX );

#endif

    FREE( art_gv->arspectrum_gv );
)

ART_SPECTRUM_MODULE_INITIALISATION_FUNCTION
(
    if ( ISR_HAS_BEEN_INITIALISED )
    {
        if ( SPC_ZERO_GV ) spc_free( art_gv, SPC_ZERO_GV );
        if ( SPC_UNIT_GV ) spc_free( art_gv, SPC_UNIT_GV );

        SPC_ZERO_GV =
            spc_d_alloc_init(
                art_gv,
                0.0
                );

        SPC_UNIT_GV =
            spc_d_alloc_init(
                art_gv,
                1.0
                );

        art_gv->arspectrum_gv->number_of_channels =
            spc_channels(
                art_gv
                );
    }
)

void  ardt_printf_current_isr(
        const ART_GV  * art_gv
        )
{
    if ( ISR_HAS_BEEN_INITIALISED )
        printf( "%s\n", spc_typename_string(art_gv) );
    else
        printf("(subsystem not initialised yet)\n");

    fflush(stdout);
}

ArSpectrum * spc_alloc(
        const ART_GV  * art_gv
        )
{
    ArSpectrum  * newSpectrum = ALLOC( ArSpectrum );

    newSpectrum->value =
        art_gv->arspectrum_gv->_acf_alloc(
            art_gv
            );

#ifdef ARSPECTRUM_DEBUG_ASSERTIONS

    pthread_mutex_lock( & INSTANCE_ARRAY_MUTEX );

    //   We have to check whether this address has not been used for
    //   an ArSpectrum instance before, and remove the entry from the
    //   list of freed instances if it has.

    int  freed = arintdynarray_size( & FREED_INSTANCE_ARRAY );

    for ( int i = 0; i < freed; i++ )
    {
        if (    arintdynarray_i( & FREED_INSTANCE_ARRAY, i )
             == ((int)(newSpectrum)) )
        {
            arintdynarray_set_i( & FREED_INSTANCE_ARRAY, 0, i );
            continue;
        }
    }

    arintdynarray_push(
        & ALLOCATED_INSTANCE_ARRAY,
          (int)newSpectrum
        );

    pthread_mutex_unlock( & INSTANCE_ARRAY_MUTEX );

#endif

    return newSpectrum;
}

void spc_free(
        const ART_GV      * art_gv,
              ArSpectrum  * cr
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS_NOINIT__CR;

#ifdef ARSPECTRUM_DEBUG_ASSERTIONS

    pthread_mutex_lock( & INSTANCE_ARRAY_MUTEX );

    //   We remove the entry from the array of active instances.

    int  allocated = arintdynarray_size( & ALLOCATED_INSTANCE_ARRAY );

    for ( int i = 0; i < allocated; i++ )
    {
        if (    arintdynarray_i( & ALLOCATED_INSTANCE_ARRAY, i )
             == ((int)(cr)) )
        {
            arintdynarray_set_i( & ALLOCATED_INSTANCE_ARRAY, 0, i );
            continue;
        }
    }

    //   We remove the entry from the array of initialised instances.

    int  initialised = arintdynarray_size( & INITIALISED_INSTANCE_ARRAY );

    for ( int i = 0; i < initialised; i++ )
    {
        if (    arintdynarray_i( & INITIALISED_INSTANCE_ARRAY, i )
             == ((int)(cr)) )
        {
            arintdynarray_set_i( & INITIALISED_INSTANCE_ARRAY, 0, i );
            continue;
        }
    }

    arintdynarray_push(
        & FREED_INSTANCE_ARRAY,
          (int)cr
        );

    pthread_mutex_unlock( & INSTANCE_ARRAY_MUTEX );

#endif

    art_gv->arspectrum_gv->_acf_free(
        art_gv,
        cr->value
        );

    FREE( cr );
}

ArSpectrum * spc_d_alloc_init(
        const ART_GV  * art_gv,
        const double    d0
        )
{
    ArSpectrum  * newSpectrum = spc_alloc( art_gv );

    ARSPECTRUM_DEBUG_ASSERTIONS_REGISTER_AS_INITIALISED(newSpectrum);

    art_gv->arspectrum_gv->_acf_d_init_s(
        art_gv,
        d0,
        newSpectrum->value
        );

    return newSpectrum;
}

ArSpectrum *  spc_s_alloc_init(
        const ART_GV      * art_gv,
        const ArSpectrum  * c0
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0;

    ArSpectrum  * newSpectrum = spc_alloc( art_gv );

    ARSPECTRUM_DEBUG_ASSERTIONS_REGISTER_AS_INITIALISED(newSpectrum);

    art_gv->arspectrum_gv->_acf_s_init_s(
        art_gv,
        c0->value,
        newSpectrum->value
        );

    return newSpectrum;
}

ArSpectrum const * spc_zero(
        const ART_GV  * art_gv
        )
{
    return SPC_ZERO_GV;
}

ArSpectrum const * spc_unit(
        const ART_GV  * art_gv
        )
{
    return SPC_UNIT_GV;
}

void spc_d_init_s(
        const ART_GV      * art_gv,
        const double      d0,
              ArSpectrum  * cr
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS_NOINIT__CR;

    art_gv->arspectrum_gv->_acf_d_init_s(
        art_gv,
        d0,
        cr->value
        );

    ARSPECTRUM_DEBUG_ASSERTIONS_REGISTER_AS_INITIALISED(cr);
}

void spc_s_init_s(
        const ART_GV      * art_gv,
        const ArSpectrum  * c0,
              ArSpectrum  * cr
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0;
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS_NOINIT__CR;

    art_gv->arspectrum_gv->_acf_s_init_s(
        art_gv,
        c0->value,
        cr->value
        );

    ARSPECTRUM_DEBUG_ASSERTIONS_REGISTER_AS_INITIALISED(cr);
}

unsigned int spc_channels(
        const ART_GV  * art_gv
        )
{
    return
        art_gv->arspectrum_gv->_acf_channels(
            art_gv
            );
}

unsigned int spc_first_visible_channel(
        const ART_GV  * art_gv
        )
{
    return
        art_gv->arspectrum_gv->_acf_first_visible_channel(
            art_gv
            );
}

double spc_si(
        const ART_GV          * art_gv,
        const ArSpectrum      * c0,
        const unsigned int    i
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0;

    return
        art_gv->arspectrum_gv->_acf_si(
            art_gv,
            c0->value,
            i
            );
}

void spc_set_sid(
        const ART_GV          * art_gv,
              ArSpectrum      * cr,
        const unsigned int    i,
        const double          d0
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS_NOINIT__CR;

    art_gv->arspectrum_gv->_acf_set_sid(
        art_gv,
        cr->value,
        i,
        d0
        );

    ARSPECTRUM_DEBUG_ASSERTIONS_REGISTER_AS_INITIALISED(cr);
}

double spc_channel_lower_bound(
        const ART_GV        * art_gv,
        const unsigned int    i0
        )
{
    return
        art_gv->arspectrum_gv->_acf_channel_lower_bound(
            art_gv,
            i0
            );
}

double spc_channel_center(
        const ART_GV        * art_gv,
        const unsigned int    i0
        )
{
    return
        art_gv->arspectrum_gv->_acf_channel_center(
            art_gv,
            i0
            );
}

double spc_channel_width(
        const ART_GV        * art_gv,
        const unsigned int    i0
        )
{
    return
        art_gv->arspectrum_gv->_acf_channel_width(
            art_gv,
            i0
            );
}

double spc_channel_width_div_2(
        const ART_GV        * art_gv,
        const unsigned int    i0
        )
{
    return
        art_gv->arspectrum_gv->_acf_channel_width_div_2(
            art_gv,
            i0
            );
}

double spc_channel_weight(
        const ART_GV        * art_gv,
        const unsigned int    i0
        )
{
    return
        art_gv->arspectrum_gv->_acf_channel_weight(
            art_gv,
            i0
            );
}

ArPSSpectrum const * spc_sample_basis(
        const ART_GV        * art_gv,
        const unsigned int    i0
        )
{
    return
        art_gv->arspectrum_gv->_acf_sample_basis(
            art_gv,
            i0
            );
}

const char * spc_shortname_string(
        const ART_GV  * art_gv
        )
{
    return
        art_gv->arspectrum_gv->_acf_shortname_string(
            art_gv
            );
}

const char * spc_typename_string(
        const ART_GV  * art_gv
        )
{
    return
        art_gv->arspectrum_gv->_acf_typename_string(
            art_gv
            );
}

const char * spc_description_string(
        const ART_GV  * art_gv
        )
{
    return
        art_gv->arspectrum_gv->_acf_description_string(
            art_gv
            );
}

double spc_s_max(
        const ART_GV      * art_gv,
        const ArSpectrum  * c0
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0;

    return
        art_gv->arspectrum_gv->_acf_s_max(
            art_gv,
            c0->value
            );
}

double spc_s_min(
        const ART_GV      * art_gv,
        const ArSpectrum  * c0
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0;

    return
        art_gv->arspectrum_gv->_acf_s_min(
            art_gv,
            c0->value
            );
}

double spc_s_sum(
        const ART_GV      * art_gv,
        const ArSpectrum  * c0
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0;

    return
        art_gv->arspectrum_gv->_acf_s_sum(
            art_gv,
            c0->value
            );
}

double spc_s_avg(
        const ART_GV      * art_gv,
        const ArSpectrum  * c0
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0;

    return
        art_gv->arspectrum_gv->_acf_s_avg(
            art_gv,
            c0->value
            );
}

double spc_s_l1_norm(
        const ART_GV      * art_gv,
        const ArSpectrum  * c0
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0;

    return
        art_gv->arspectrum_gv->_acf_s_l1_norm(
            art_gv,
            c0->value
            );
}

double spc_s_l1_norm_visible_range(
        const ART_GV      * art_gv,
        const ArSpectrum  * c0
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0;

    return
        art_gv->arspectrum_gv->_acf_s_l1_norm_visible_range(
            art_gv,
            c0->value
            );
}

double spc_s_l2_norm(
        const ART_GV      * art_gv,
        const ArSpectrum  * c0
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0;

    return
        art_gv->arspectrum_gv->_acf_s_l2_norm(
            art_gv,
            c0->value
            );
}

double spc_s_linf_norm(
        const ART_GV      * art_gv,
        const ArSpectrum  * c0
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0;

    return
        art_gv->arspectrum_gv->_acf_s_linf_norm(
            art_gv,
            c0->value
            );
}

void spc_inv_s(
        const ART_GV      * art_gv,
              ArSpectrum  * cr
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__CR;

    art_gv->arspectrum_gv->_acf_inv_s(
        art_gv,
        cr->value
        );
}

void spc_s_inv_s(
        const ART_GV      * art_gv,
        const ArSpectrum  * c0,
              ArSpectrum  * cr
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0_NICR;

    art_gv->arspectrum_gv->_acf_s_inv_s(
        art_gv,
        c0->value,
        cr->value
        );

    ARSPECTRUM_DEBUG_ASSERTIONS_REGISTER_AS_INITIALISED(cr);
}

void spc_s_fast_inv_s(
        const ART_GV      * art_gv,
        const ArSpectrum  * c0,
              ArSpectrum  * cr
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0_NICR;

    art_gv->arspectrum_gv->_acf_s_fast_inv_s(
        art_gv,
        c0->value,
        cr->value
        );

    ARSPECTRUM_DEBUG_ASSERTIONS_REGISTER_AS_INITIALISED(cr);
}

void spc_abs_s(
        const ART_GV      * art_gv,
              ArSpectrum  * cr
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__CR;

    art_gv->arspectrum_gv->_acf_abs_s(
        art_gv,
        cr->value
        );
}

void spc_s_abs_s(
        const ART_GV      * art_gv,
        const ArSpectrum  * c0,
              ArSpectrum  * cr
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0_NICR;

    art_gv->arspectrum_gv->_acf_s_abs_s(
        art_gv,
        c0->value,
        cr->value
        );

    ARSPECTRUM_DEBUG_ASSERTIONS_REGISTER_AS_INITIALISED(cr);
}

unsigned int spc_ss_equal(
        const ART_GV      * art_gv,
        const ArSpectrum  * c0,
        const ArSpectrum  * c1
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0_C1;

    return
        art_gv->arspectrum_gv->_acf_ss_equal(
            art_gv,
            c0->value,
            c1->value
            );
}

unsigned int spc_ssd_equal(
        const ART_GV      * art_gv,
        const ArSpectrum  * c0,
        const ArSpectrum  * c1,
        const double        max_relative_error
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0_C1;

    return
        art_gv->arspectrum_gv->_acf_ssd_equal(
            art_gv,
            c0->value,
            c1->value,
            max_relative_error
            );
}

void spc_ss_max_s(
        const ART_GV      * art_gv,
        const ArSpectrum  * c0,
        const ArSpectrum  * c1,
              ArSpectrum  * cr
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0_C1_NICR;

    art_gv->arspectrum_gv->_acf_ss_max_s(
        art_gv,
        c0->value,
        c1->value,
        cr->value
        );

    ARSPECTRUM_DEBUG_ASSERTIONS_REGISTER_AS_INITIALISED(cr);
}

void spc_ss_min_s(
        const ART_GV      * art_gv,
        const ArSpectrum  * c0,
        const ArSpectrum  * c1,
              ArSpectrum  * cr
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0_C1_NICR;

    art_gv->arspectrum_gv->_acf_ss_min_s(
        art_gv,
        c0->value,
        c1->value,
        cr->value
        );

    ARSPECTRUM_DEBUG_ASSERTIONS_REGISTER_AS_INITIALISED(cr);
}

void spc_ss_mean_s(
        const ART_GV      * art_gv,
        const ArSpectrum  * c0,
        const ArSpectrum  * c1,
              ArSpectrum  * cr
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0_C1_NICR;

    art_gv->arspectrum_gv->_acf_ss_mean_s(
        art_gv,
        c0->value,
        c1->value,
        cr->value
        );

    ARSPECTRUM_DEBUG_ASSERTIONS_REGISTER_AS_INITIALISED(cr);
}

double spc_ss_maxdiff(
        const ART_GV      * art_gv,
        const ArSpectrum  * c0,
        const ArSpectrum  * c1
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0_C1;

    return
        art_gv->arspectrum_gv->_acf_ss_maxdiff(
            art_gv,
            c0->value,
            c1->value
            );
}

void spc_d_add_s(
        const ART_GV      * art_gv,
        const double      d0,
              ArSpectrum  * cr
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__CR;

    art_gv->arspectrum_gv->_acf_d_add_s(
        art_gv,
        d0,
        cr->value
        );
}

void spc_d_atomic_add_s(
        const ART_GV      * art_gv,
        const double      d0,
              ArSpectrum  * cr
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__CR;

    art_gv->arspectrum_gv->_acf_d_atomic_add_s(
        art_gv,
        d0,
        cr->value
        );
}

void spc_d_sub_s(
        const ART_GV      * art_gv,
        const double      d0,
              ArSpectrum  * cr
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__CR;

    art_gv->arspectrum_gv->_acf_d_sub_s(
        art_gv,
        d0,
        cr->value
        );
}

void spc_d_mul_s(
        const ART_GV      * art_gv,
        const double        d0,
              ArSpectrum  * cr
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__CR;

    art_gv->arspectrum_gv->_acf_d_mul_s(
        art_gv,
        d0,
        cr->value
        );
}

void spc_d_div_s(
        const ART_GV      * art_gv,
        const double      d0,
              ArSpectrum  * cr
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__CR;

    art_gv->arspectrum_gv->_acf_d_div_s(
        art_gv,
        d0,
        cr->value
        );
}

void spc_s_add_s(
        const ART_GV      * art_gv,
        const ArSpectrum  * c0,
              ArSpectrum  * cr
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0_CR;

    art_gv->arspectrum_gv->_acf_s_add_s(
        art_gv,
        c0->value,
        cr->value
        );
}

void spc_s_atomic_add_s(
        const ART_GV      * art_gv,
        const ArSpectrum  * c0,
              ArSpectrum  * cr
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0_CR;

    art_gv->arspectrum_gv->_acf_s_atomic_add_s(
        art_gv,
        c0->value,
        cr->value
        );
}

void spc_s_sub_s(
        const ART_GV      * art_gv,
        const ArSpectrum  * c0,
              ArSpectrum  * cr
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0_CR;

    art_gv->arspectrum_gv->_acf_s_sub_s(
        art_gv,
        c0->value,
        cr->value
        );
}

void spc_s_mul_s(
        const ART_GV      * art_gv,
        const ArSpectrum  * c0,
              ArSpectrum  * cr
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0_CR;

    art_gv->arspectrum_gv->_acf_s_mul_s(
        art_gv,
        c0->value,
        cr->value
        );
}

void spc_s_div_s(
        const ART_GV      * art_gv,
        const ArSpectrum  * c0,
              ArSpectrum  * cr
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0_CR;

    art_gv->arspectrum_gv->_acf_s_div_s(
        art_gv,
        c0->value,
        cr->value
        );
}

void spc_exp_s(
        const ART_GV      * art_gv,
              ArSpectrum  * cr
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__CR;

    art_gv->arspectrum_gv->_acf_exp_s(
        art_gv,
        cr->value
        );
}

void spc_s_exp_s(
        const ART_GV      * art_gv,
        const ArSpectrum  * c0,
              ArSpectrum  * cr
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0_CR;

    art_gv->arspectrum_gv->_acf_s_exp_s(
        art_gv,
        c0->value,
        cr->value
        );
}

void spc_negexp_s(
        const ART_GV      * art_gv,
              ArSpectrum  * cr
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__CR;

    art_gv->arspectrum_gv->_acf_negexp_s(
        art_gv,
        cr->value
        );
}

void spc_s_negexp_s(
        const ART_GV      * art_gv,
        const ArSpectrum  * c0,
              ArSpectrum  * cr
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0_CR;

    art_gv->arspectrum_gv->_acf_s_negexp_s(
        art_gv,
        c0->value,
        cr->value
        );
}

void spc_sd_pow_s(
        const ART_GV      * art_gv,
        const ArSpectrum  * c0,
        const double      d0,
              ArSpectrum  * cr
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0_NICR;

    art_gv->arspectrum_gv->_acf_sd_pow_s(
        art_gv,
        c0->value,
        d0,
        cr->value
        );

    ARSPECTRUM_DEBUG_ASSERTIONS_REGISTER_AS_INITIALISED(cr);
}

void spc_sd_negpow_s(
        const ART_GV      * art_gv,
        const ArSpectrum  * c0,
        const double      d0,
              ArSpectrum  * cr
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0_NICR;

    art_gv->arspectrum_gv->_acf_sd_negpow_s(
        art_gv,
        c0->value,
        d0,
        cr->value
        );

    ARSPECTRUM_DEBUG_ASSERTIONS_REGISTER_AS_INITIALISED(cr);
}

void spc_ds_pow_s(
        const ART_GV      * art_gv,
        const double      d0,
        const ArSpectrum  * c0,
              ArSpectrum  * cr
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0_NICR;

    art_gv->arspectrum_gv->_acf_ds_pow_s(
        art_gv,
        d0,
        c0->value,
        cr->value
        );

    ARSPECTRUM_DEBUG_ASSERTIONS_REGISTER_AS_INITIALISED(cr);
}

void spc_ds_negpow_s(
        const ART_GV      * art_gv,
        const double      d0,
        const ArSpectrum  * c0,
              ArSpectrum  * cr
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0_NICR;

    art_gv->arspectrum_gv->_acf_ds_negpow_s(
        art_gv,
        d0,
        c0->value,
        cr->value
        );

    ARSPECTRUM_DEBUG_ASSERTIONS_REGISTER_AS_INITIALISED(cr);
}

void spc_sqrt_s(
        const ART_GV      * art_gv,
              ArSpectrum  * cr
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__CR;

    art_gv->arspectrum_gv->_acf_sqrt_s(
        art_gv,
        cr->value
        );
}

void spc_s_sqrt_s(
        const ART_GV      * art_gv,
        const ArSpectrum  * c0,
              ArSpectrum  * cr
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0_CR;

    art_gv->arspectrum_gv->_acf_s_sqrt_s(
        art_gv,
        c0->value,
        cr->value
        );
}

void spc_sd_add_s(
        const ART_GV      * art_gv,
        const ArSpectrum  * c0,
        const double      d0,
              ArSpectrum  * cr
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0_NICR;

    art_gv->arspectrum_gv->_acf_sd_add_s(
        art_gv,
        c0->value,
        d0,
        cr->value
        );

    ARSPECTRUM_DEBUG_ASSERTIONS_REGISTER_AS_INITIALISED(cr);
}

void spc_sd_sub_s(
        const ART_GV      * art_gv,
        const ArSpectrum  * c0,
        const double      d0,
              ArSpectrum  * cr
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0_NICR;

    art_gv->arspectrum_gv->_acf_sd_sub_s(
        art_gv,
        c0->value,
        d0,
        cr->value
        );

    ARSPECTRUM_DEBUG_ASSERTIONS_REGISTER_AS_INITIALISED(cr);
}

void spc_sd_mul_s(
        const ART_GV      * art_gv,
        const ArSpectrum  * c0,
        const double      d0,
              ArSpectrum  * cr
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0_NICR;

    art_gv->arspectrum_gv->_acf_sd_mul_s(
        art_gv,
        c0->value,
        d0,
        cr->value
        );

    ARSPECTRUM_DEBUG_ASSERTIONS_REGISTER_AS_INITIALISED(cr);
}

void spc_sd_div_s(
        const ART_GV      * art_gv,
        const ArSpectrum  * c0,
        const double      d0,
              ArSpectrum  * cr
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0_NICR;

    art_gv->arspectrum_gv->_acf_sd_div_s(
        art_gv,
        c0->value,
        d0,
        cr->value
        );

    ARSPECTRUM_DEBUG_ASSERTIONS_REGISTER_AS_INITIALISED(cr);
}

void spc_ds_add_s(
        const ART_GV      * art_gv,
        const double      d0,
        const ArSpectrum  * c0,
              ArSpectrum  * cr
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0_NICR;

    art_gv->arspectrum_gv->_acf_ds_add_s(
        art_gv,
        d0,
        c0->value,
        cr->value
        );

    ARSPECTRUM_DEBUG_ASSERTIONS_REGISTER_AS_INITIALISED(cr);
}

void spc_ds_sub_s(
        const ART_GV      * art_gv,
        const double      d0,
        const ArSpectrum  * c0,
              ArSpectrum  * cr
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0_NICR;

    art_gv->arspectrum_gv->_acf_ds_sub_s(
        art_gv,
        d0,
        c0->value,
        cr->value
        );

    ARSPECTRUM_DEBUG_ASSERTIONS_REGISTER_AS_INITIALISED(cr);
}

void spc_ds_mul_s(
        const ART_GV      * art_gv,
        const double      d0,
        const ArSpectrum  * c0,
              ArSpectrum  * cr
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0_NICR;

    art_gv->arspectrum_gv->_acf_ds_mul_s(
        art_gv,
        d0,
        c0->value,
        cr->value
        );

    ARSPECTRUM_DEBUG_ASSERTIONS_REGISTER_AS_INITIALISED(cr);
}

void spc_ds_div_s(
        const ART_GV      * art_gv,
        const double      d0,
        const ArSpectrum  * c0,
              ArSpectrum  * cr
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0_NICR;

    art_gv->arspectrum_gv->_acf_ds_div_s(
        art_gv,
        d0,
        c0->value,
        cr->value
        );

    ARSPECTRUM_DEBUG_ASSERTIONS_REGISTER_AS_INITIALISED(cr);
}

void spc_ss_mul_s(
        const ART_GV      * art_gv,
        const ArSpectrum  * c0,
        const ArSpectrum  * c1,
              ArSpectrum  * cr
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0_C1_NICR;

    art_gv->arspectrum_gv->_acf_ss_mul_s(
        art_gv,
        c0->value,
        c1->value,
        cr->value
        );

    ARSPECTRUM_DEBUG_ASSERTIONS_REGISTER_AS_INITIALISED(cr);
}

void spc_ss_add_s(
        const ART_GV      * art_gv,
        const ArSpectrum  * c0,
        const ArSpectrum  * c1,
              ArSpectrum  * cr
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0_C1_NICR;

    art_gv->arspectrum_gv->_acf_ss_add_s(
        art_gv,
        c0->value,
        c1->value,
        cr->value
        );

    ARSPECTRUM_DEBUG_ASSERTIONS_REGISTER_AS_INITIALISED(cr);
}

void spc_ss_sub_s(
        const ART_GV      * art_gv,
        const ArSpectrum  * c0,
        const ArSpectrum  * c1,
              ArSpectrum  * cr
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0_C1_NICR;

    art_gv->arspectrum_gv->_acf_ss_sub_s(
        art_gv,
        c0->value,
        c1->value,
        cr->value
        );

    ARSPECTRUM_DEBUG_ASSERTIONS_REGISTER_AS_INITIALISED(cr);
}

void spc_ss_div_s(
        const ART_GV      * art_gv,
        const ArSpectrum  * c0,
        const ArSpectrum  * c1,
              ArSpectrum  * cr
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0_C1_NICR;

    art_gv->arspectrum_gv->_acf_ss_div_s(
        art_gv,
        c0->value,
        c1->value,
        cr->value
        );

    ARSPECTRUM_DEBUG_ASSERTIONS_REGISTER_AS_INITIALISED(cr);
}

void spc_dd_clamp_s(
        const ART_GV      * art_gv,
        const double      d0,
        const double      d1,
              ArSpectrum  * cr
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__CR;

    art_gv->arspectrum_gv->_acf_dd_clamp_s(
        art_gv,
        d0,
        d1,
        cr->value
        );
}

void spc_dds_clamp_s(
        const ART_GV      * art_gv,
        const double      d0,
        const double      d1,
        const ArSpectrum  * c0,
              ArSpectrum  * cr
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0_CR;

    art_gv->arspectrum_gv->_acf_dds_clamp_s(
        art_gv,
        d0,
        d1,
        c0->value,
        cr->value
        );
}

void spc_ds_mul_add_s(
        const ART_GV      * art_gv,
        const double      d0,
        const ArSpectrum  * c0,
              ArSpectrum  * cr
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0_CR;

    art_gv->arspectrum_gv->_acf_ds_mul_add_s(
        art_gv,
        d0,
        c0->value,
        cr->value
        );
}

void spc_dss_interpol_s(
        const ART_GV      * art_gv,
        const double      d0,
        const ArSpectrum  * c0,
        const ArSpectrum  * c1,
              ArSpectrum  * cr
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0_C1_NICR;

    art_gv->arspectrum_gv->_acf_dss_interpol_s(
        art_gv,
        d0,
        c0->value,
        c1->value,
        cr->value
        );

    ARSPECTRUM_DEBUG_ASSERTIONS_REGISTER_AS_INITIALISED(cr);
}

void spc_ds_mul_ds_mul_add_s(
        const ART_GV      * art_gv,
        const double      d0,
        const ArSpectrum  * c0,
        const double      d1,
        const ArSpectrum  * c1,
              ArSpectrum  * cr
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0_C1_NICR;

    art_gv->arspectrum_gv->_acf_ds_mul_ds_mul_add_s(
        art_gv,
        d0,
        c0->value,
        d1,
        c1->value,
        cr->value
        );

    ARSPECTRUM_DEBUG_ASSERTIONS_REGISTER_AS_INITIALISED(cr);
}

void spc_ds_mul_ds_mul_ds_mul_add3_s(
        const ART_GV      * art_gv,
        const double      d0,
        const ArSpectrum  * c0,
        const double      d1,
        const ArSpectrum  * c1,
        const double      d2,
        const ArSpectrum  * c2,
              ArSpectrum  * cr
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0_C1_C2_NICR;

    art_gv->arspectrum_gv->_acf_ds_mul_ds_mul_ds_mul_add3_s(
        art_gv,
        d0,
        c0->value,
        d1,
        c1->value,
        d2,
        c2->value,
        cr->value
        );

    ARSPECTRUM_DEBUG_ASSERTIONS_REGISTER_AS_INITIALISED(cr);
}

double spc_sd_value_at_wavelength(
        const ART_GV      * art_gv,
        const ArSpectrum  * c0,
        const double        d0
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0;

    return
        art_gv->arspectrum_gv->_acf_sd_value_at_wavelength(
            art_gv,
            c0->value,
            d0
            );
}

void spc_sdd_sample_at_wavelength_s(
        const ART_GV      * art_gv,
        const ArSpectrum  * c0,
        const double        d0,
        const double        d1,
              ArSpectrum  * cr
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0;

    art_gv->arspectrum_gv->_acf_sdd_sample_at_wavelength_s(
        art_gv,
        c0->value,
        d0,
        d1,
        cr->value
        );
}

double spc_ss_convolve(
        const ART_GV      * art_gv,
        const ArSpectrum  * c0,
        const ArSpectrum  * c1
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0_C1;

    return
        art_gv->arspectrum_gv->_acf_ss_convolve(
            art_gv,
            c0->value,
            c1->value
            );
}

unsigned int spc_s_valid(
        const ART_GV      * art_gv,
        const ArSpectrum  * c0
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS_NOINIT__C0;

    return
        art_gv->arspectrum_gv->_acf_s_valid(
            art_gv,
            c0->value
            );
}

void spc_s_debugprintf(
        const ART_GV      * art_gv,
        const ArSpectrum  * c0
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0;

    printf("ArSpectrum[%lu]: ",(long unsigned int)c0);fflush(stdout);
    art_gv->arspectrum_gv->_acf_s_debugprintf(
        art_gv,
        c0->value
        );
}

void spc_s_mathematicaprintf(
        const ART_GV      * art_gv,
        const ArSpectrum  * c0
        )
{
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0;

    art_gv->arspectrum_gv->_acf_s_mathematicaprintf(
        art_gv,
        c0->value
        );
}

#define SPC_CONVERSION_IMPLEMENTATION(_Type,_type,_new) \
void spc_to_##_type##_new( \
        const ART_GV     * art_gv, \
        const ArSpectrum * c0, \
              _Type      * cr \
        ) \
{ \
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS__C0; \
\
    art_gv->arspectrum_gv->_acf_s_to_##_type##_new( \
        art_gv, \
        c0->value, \
        cr \
        ); \
} \
\
void _type##_to_spc( \
        const ART_GV     * art_gv, \
        const _Type      * c0, \
              ArSpectrum * cr \
        ) \
{ \
    CHECK_ARSPECTRUM_DEBUG_ASSERTIONS_NOINIT__CR; \
\
    art_gv->arspectrum_gv->_acf_##_type##_to_s( \
        art_gv, \
        c0, \
        cr->value \
        ); \
\
    ARSPECTRUM_DEBUG_ASSERTIONS_REGISTER_AS_INITIALISED(cr); \
}

SPC_CONVERSION_IMPLEMENTATION(ArGrey,g,)
SPC_CONVERSION_IMPLEMENTATION(ArGreyAlpha,ga,)
SPC_CONVERSION_IMPLEMENTATION(ArGrey8,g8,)
SPC_CONVERSION_IMPLEMENTATION(ArGrey16,g16,)
SPC_CONVERSION_IMPLEMENTATION(ArGreyAlpha16,ga16,)
//SPC_CONVERSION_IMPLEMENTATION(ArGrey32,g32,)
SPC_CONVERSION_IMPLEMENTATION(ArGreyAlpha32,ga32,)

SPC_CONVERSION_IMPLEMENTATION(ArRGB,rgb,)
SPC_CONVERSION_IMPLEMENTATION(ArRGBA,rgba,)
SPC_CONVERSION_IMPLEMENTATION(ArRGB24,rgb24,)
SPC_CONVERSION_IMPLEMENTATION(ArRGBA32,rgba32,)
SPC_CONVERSION_IMPLEMENTATION(ArRGB48,rgb48,)
SPC_CONVERSION_IMPLEMENTATION(ArRGBA64,rgba64,)
//SPC_CONVERSION_IMPLEMENTATION(ArRGB96,rgb96,)
//SPC_CONVERSION_IMPLEMENTATION(ArRGBA128,rgba128,)

SPC_CONVERSION_IMPLEMENTATION(ArCIEXYZ,xyz,)
SPC_CONVERSION_IMPLEMENTATION(ArCIEXYZA,xyza,)

SPC_CONVERSION_IMPLEMENTATION(ArSpectrum8,s8,)
SPC_CONVERSION_IMPLEMENTATION(ArSpectrum11,s11,)
SPC_CONVERSION_IMPLEMENTATION(ArSpectrum18,s18,)
SPC_CONVERSION_IMPLEMENTATION(ArSpectrum46,s46,)
SPC_CONVERSION_IMPLEMENTATION(ArSpectrum500,s500,)
SPC_CONVERSION_IMPLEMENTATION(ArRSSpectrum,rss,_new)
SPC_CONVERSION_IMPLEMENTATION(ArPSSpectrum,pss,_new)

#include "ColourAndSpectralDataConversion.h"

/* ======================================================================== */
