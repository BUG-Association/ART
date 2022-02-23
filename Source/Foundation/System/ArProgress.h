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

#ifndef _ART_FOUNDATION_SYSTEM_ARPROGRESS_H_
#define _ART_FOUNDATION_SYSTEM_ARPROGRESS_H_

#include "ART_ModuleManagement.h"

ART_MODULE_INTERFACE(ArProgress)

#include <stdio.h>              /* FILE */

/* ===========================================================================
    'ArProgress'
        This is a structure with functions that make it possible to easily
        program a progress indicator.  The function 'arprogress' constructs
        a progress indicator for a supplied number of steps, and a supplied
        number of digits after the decimal point of a percentage output.
        For example to create a progress indicator with a precision of a
        1/10th of a percent for a task taking 42042 steps you can use:

                ArProgress p = arprogress(42042,1);

        To generate a progess output of the type ' 42.1%', you can then use:

                for (step = 0; step < 42042; step++)
                {
                    arprogress_fprint(&p, step, stdout);

                    // your task goes here

                }
                arprogress_fprint(&p, 42042, step);     // output 100.0%

=========================================================================== */
typedef struct ArProgress
{
    long    out_long;
    double  scale_double;
    double  scale_long;
    int     comma;
}
ArProgress;

/* ---------------------------------------------------------------------------
    'arprogress'
--------------------------------------------------------------------------- */
ArProgress arprogress(
        unsigned long range,
        unsigned int comma
        );

/* ---------------------------------------------------------------------------
    'arprogress_sprint'
--------------------------------------------------------------------------- */
void arprogress_sprint(
        ArProgress * p,
        unsigned long new_value,
        char * buffer
        );

/* ---------------------------------------------------------------------------
    'arprogress_fprint'
--------------------------------------------------------------------------- */
void arprogress_fprint(
        ArProgress * p,
        unsigned long new_value,
        FILE * file
        );


#endif /* _ART_FOUNDATION_SYSTEM_ARPROGRESS_H_ */
/* ======================================================================== */
