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

#ifndef _ART_FOUNDATION_ARFREELIST_H_
#define _ART_FOUNDATION_ARFREELIST_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArFreelist)


/* ---------------------------------------------------------------------------

    'ArFreelist' struct
    -------------------

    ArFreelists are intended as a minimum-overhead way of providing pools of
    pre-allocated C datastructures for frequent use.

    They come in handy in all those usage scenarios where one regularly
    needs the same kind of non-atomic datastructure that has to be explicitly
    allocated, e.g. the many instances of BSDF samples, ArSpectrum, ArLight,
    ArAttenuation and the like one needs during rendering.

    Each ArFreelist can only contain pre-allocated objects of exactly one
    type, so an actual renderer usually uses quite a number of these lists -
    one per type which is created and discarded frequently enough to warrant
    this kind of treatment.

    For technical reasons, there are two constraints on the data type that
    a freelist can provide: it MUST be a structure, and the first entry in
    this structure MUST be a pointer to a similar structure (which is
    usually named "next").

    Individual instances of the data type in question are taken from the
    freelist, and placed back there, via calls to the "pop" and "push"
    methods of the freelist.

    If a freelist is empty, and a caller requests "allocation" of another
    instance of the data type in question, the freelist automatically and
    transparently refills itself with a pre-determined number of newly
    allocated instances of the data type. The first one of these new
    instances is then returned to the caller.

    Optionally, one can provide a function pointer to explicit "alloc" and
    "free" functions for the content data type. These are then used instead
    of plain mallo/free to allocate the memory needed for the individual
    instances. Usage examples of this functionality are e.g. freelists for
    ArSpectrum and ArLight structs; depending on the state of the colour
    subsystem, these structures vary in size, and require explicit
    initialisation.

    ----------------------------------------------------------------------
    NOTE: in order to keep things simple, ArFreelists are NOT thread-safe!
    ----------------------------------------------------------------------

    PROVIDED INTERFACE
    ==================

    'arfreelist_pop'
        "Allocates" a node by popping it from the freelist.  If the freelist
        is empty, a new batch of nodes is allocated.

    'arfreelist_push'
        "Frees" a node by pushing it back onto the freelist.

        Two important caveats apply: the pointer to the node that is being
        pushed MUST be valid - it is NOT checked against 0 for performance
        reasons. Also, a "push" onto a completely empty, newly created
        freelist will fail, since that particular sanity check was also
        omitted for performance reasons.

        However, as soon as a single "pop" operation has taken place on a
        newly created freelist (i.e. once it has had a chance to refill
        itself at least once), an arbitrary number of "push" operations can
        be performed.

    'arfreelist_free_contents'
        Frees all contents of the freelist, but NOT the freelist structure
        itself.

------------------------------------------------------------------------aw- */

typedef struct ArFreeNode
{
    struct ArFreeNode  * next;
}
ArFreeNode;

typedef struct ArFreelist
{
    ART_GV         * art_gv;
    ArFreeNode     * next_free_element;
    unsigned long    element_size;
    unsigned long    new_elements_per_refill;

    void * (* _element_alloc)
    ( const ART_GV * );

    void (* _element_free)
    ( const ART_GV *, void * );
}
ArFreelist;

#define ARFREELIST_INIT_FOR_STRUCT(_fl,_type,_refill) \
    arfreelist_init_for_plain_struct( \
          art_gv, \
        & (_fl), \
          sizeof(_type), \
          (_refill) \
        );

#define ARFREELIST_INIT_FOR_TYPE(_fl,_type_prefix,_refill) \
    arfreelist_init_with_functions( \
          art_gv, \
        & (_fl), \
          (_refill), \
          ((void * (*) ( const ART_GV * )) _type_prefix##_alloc), \
          ((void   (*)  ( const ART_GV *, void * )) _type_prefix##_free) \
        );

void arfreelist_init_for_plain_struct(
        ART_GV         * art_gv,
        ArFreelist     * freelist,
        unsigned int     element_size,
        unsigned long    new_elements_per_refill
        );

void arfreelist_init_with_functions(
        ART_GV         * art_gv,
        ArFreelist     * freelist,
        unsigned long    new_elements_per_refill,
        void * (* _element_alloc) ( const ART_GV * ),
        void   (* _element_free)  ( const ART_GV *, void * )
        );

void * arfreelist_pop(
        ArFreelist  * freelist
        );

void arfreelist_push(
        ArFreelist  * freelist,
        void        * node_to_push
        );

void arfreelist_free_contents(
        ArFreelist  * freelist
        );

#endif /* _ART_FOUNDATION_ARFREELIST_H_ */
/* ======================================================================== */
