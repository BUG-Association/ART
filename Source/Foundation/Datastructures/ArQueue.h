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
//if neccessary it can be macrofied
typedef struct art_task_t art_task_t;
typedef struct {
        long tail, head, length, max_size;
        art_task_t* data;
}queue_t;




#define ARQUEUE_INTERFACE_FOR_TYPE( _Type, _type, _TYPE )


#endif /* _ART_FOUNDATION_ARQUEUE_H_ */
/* ======================================================================== */
