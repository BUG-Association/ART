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

#ifndef _ART_FOUNDATION_ARQUEUE_H_
#define _ART_FOUNDATION_ARQUEUE_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArQueue)

#define ARQUEUE_TYPE(_Type) Ar##_Type##Queue

#define ARQUEUE_INTERFACE_FOR_TYPE( _Type, _type, _TYPE ) \
    typedef struct { \
        long tail, head, length, max_size; \
        _Type * data; \
    }ARQUEUE_TYPE(_Type); \
    \
    void _type##_queue_init(ARQUEUE_TYPE(_Type) * queue, size_t initialSize); \
    void _type##_queue_free(ARQUEUE_TYPE(_Type) * queue); \
    void _type##_queue_pop(ARQUEUE_TYPE(_Type) * queue); \
    _Type _type##_queue_peek(ARQUEUE_TYPE(_Type) * queue); \
    void _type##_queue_push(ARQUEUE_TYPE(_Type) * queue, _Type element); \
    void _type##_queue_prepend(ARQUEUE_TYPE(_Type) * queue, _Type element);


#define ARQUEUE_IMPLEMENTATION_FOR_TYPE( _Type, _type, _TYPE ) \
    void _type##_queue_init(ARQUEUE_TYPE(_Type) * queue, size_t initialSize){\
        queue->tail = 0; \
        queue->head = 0; \
        queue->length = 0; \
        queue->max_size = initialSize; \
        queue->data = ALLOC_ARRAY_ZERO(_Type, initialSize); \
        if(!queue->data){ \
            ART_ERRORHANDLING_FATAL_ERROR("Failed queue buffer allocation"); \
        } \
    }\
    \
    void _type##_queue_free(ARQUEUE_TYPE(_Type) * queue){\
        FREE_ARRAY(queue->data); \
    }\
    \
    void _type##_queue_pop(ARQUEUE_TYPE(_Type) * queue){\
        queue->tail = (queue->tail+1) % queue->max_size; \
        queue->length--; \
    }\
    \
    _Type _type##_queue_peek(ARQUEUE_TYPE(_Type) * queue){\
            return queue->data[queue->tail]; \
    }\
    \
    void _type##_queue_push(ARQUEUE_TYPE(_Type) * queue, _Type element){\
        if(queue->length + 1 > queue->max_size){ \
            size_t new_size = queue->max_size*2; \
            _Type * new_ptr = REALLOC_ARRAY((queue->data), _Type, new_size); \
            if(new_ptr){ \
                queue->data = new_ptr; \
                queue->max_size = new_size; \
            }else{ \
                ART_ERRORHANDLING_FATAL_ERROR("Failed queue buffer reallocation\n"); \
            } \
        } \
        queue->data[queue->head] = element; \
        queue->head = (queue->head + 1) % queue->max_size; \
        queue->length++; \
    }\
    \
    void _type##_queue_prepend(ARQUEUE_TYPE(_Type) * queue, _Type element){\
        if(queue->length + 1 > queue->max_size){ \
            size_t new_size = queue->max_size*2; \
            _Type * new_ptr = REALLOC_ARRAY((queue->data), _Type, new_size); \
            if(new_ptr){ \
                queue->data = new_ptr; \
                queue->max_size = new_size; \
            }else{ \
                ART_ERRORHANDLING_FATAL_ERROR("Failed queue buffer reallocation"); \
            } \
        } \
        if(queue->tail == 0) \
            queue->tail = queue->max_size-1; \
        else \
            queue->tail--; \
        queue->data[queue->tail] = element; \
        queue->length++; \
    }



#endif /* _ART_FOUNDATION_ARQUEUE_H_ */
/* ======================================================================== */
