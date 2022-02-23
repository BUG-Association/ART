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

#define ART_MODULE_NAME     ArProgress

#include "ArProgress.h"
#include <string.h>             /* strcpy */
#include <assert.h>             /* assert */

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


ArProgress arprogress(
        unsigned long range,
        unsigned int comma
        )
{
    ArProgress p;
    p.out_long = -1;
    p.scale_double = 100 / (double)range;
    p.scale_long = 1.0;
    assert(comma < 7);
    p.comma = comma;
    while (comma-- > 0)
        p.scale_long *= 10.0;
    return p;
}

void arprogress_sprint(
        ArProgress * p,
        unsigned long new_value,
        char * buffer
        )
{
    double new_double = (double)new_value * p->scale_double;
    long new_long = (long)(new_double * p->scale_long);
    if (p->out_long != new_long)
    {
        static const char * back =  "\b\b\b\b\b\b\b\b"
                                    "\b\b\b\b";
        static const char * perc =  "           %";
        if (p->out_long < 0)
        {
            if (p->comma == 0)
            {
                strcpy(buffer,"   %\b\b\b\b");
                buffer += 8;
            }
            else
            {
                strcpy(buffer,perc + 7 - p->comma); buffer += 5 + p->comma;
                strcpy(buffer,back + 7 - p->comma); buffer += 5 + p->comma;
            }
        }
        p->out_long = new_long;
        if (p->comma == 0)
            sprintf(buffer, "% 3ld\b\b\b", new_long);
        else
        {
            sprintf(buffer, "%*.*f%s", 4+p->comma, p->comma,
                                      new_double, back+8-p->comma);
        }
    }
    else
        buffer[0] = 0;
}

void arprogress_fprint(
        ArProgress * p,
        unsigned long new_value,
        FILE * file
        )
{
    char buffer[32];
    arprogress_sprint(p, new_value, buffer);
    if (buffer[0])
    {
        fputs(buffer, file);
        fflush(file);
    }
}

/* ======================================================================== */
