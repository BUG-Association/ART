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

#define ART_MODULE_NAME     ArFreelist

#include "ArFreeList.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


void arfreelist_refill(
        ArFreelist  * freelist
        );

void arfreelist_init_for_plain_struct(
        ART_GV         * art_gv,
        ArFreelist     * freelist,
        unsigned int     element_size,
        unsigned long    new_elements_per_refill
        )
{
    freelist->art_gv = art_gv;
    freelist->element_size = element_size;
    freelist->new_elements_per_refill = new_elements_per_refill;
    freelist->next_free_element = 0;
    freelist->_element_alloc = 0;
    freelist->_element_free = 0;

    arfreelist_refill( freelist );
}

void arfreelist_init_with_functions(
        ART_GV         * art_gv,
        ArFreelist     * freelist,
        unsigned long    new_elements_per_refill,
        void * (* _element_alloc) ( const ART_GV * ),
        void   (* _element_free)  ( const ART_GV *, void * )
        )
{
    freelist->art_gv = art_gv;
    freelist->element_size = 0;
    freelist->new_elements_per_refill = new_elements_per_refill;
    freelist->next_free_element = 0;
    freelist->_element_alloc = _element_alloc;
    freelist->_element_free = _element_free;

    arfreelist_refill( freelist );
}

void * arfreelist_pop(
        ArFreelist  * freelist
        )
{
    ArFreeNode  * new_node = freelist->next_free_element;

    freelist->next_free_element = freelist->next_free_element->next;

    if ( ! freelist->next_free_element )
        arfreelist_refill(freelist);

    return (void *)new_node;
}

void arfreelist_push(
        ArFreelist  * freelist,
        void        * element_to_push
        )
{
    ((ArFreeNode *)element_to_push)->next = freelist->next_free_element;
    freelist->next_free_element = (ArFreeNode *)element_to_push;
}

void arfreelist_free_contents(
        ArFreelist  * freelist
        )
{
    ArFreeNode  * element = freelist->next_free_element;

    while ( element )
    {
        ArFreeNode  * next = element->next;

        if ( freelist->_element_free )
            freelist->_element_free( freelist->art_gv, element );
        else
            FREE( element );

        element = next;
    }

    freelist->next_free_element = NULL;
}

void arfreelist_refill(
        ArFreelist * freelist
        )
{
    ArFreeNode  * new_element = NULL;

    for ( unsigned int i = 0; i < freelist->new_elements_per_refill; i++)
    {
        if ( freelist->_element_alloc )
            new_element = freelist->_element_alloc( freelist->art_gv );
        else
            new_element = ((ArFreeNode *) ALLOC_SIZE(freelist->element_size));

        new_element->next = freelist->next_free_element;

        freelist->next_free_element = new_element;
    }
}

/* ======================================================================== */
