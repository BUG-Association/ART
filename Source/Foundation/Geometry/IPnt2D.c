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

#define ART_MODULE_NAME     IPnt2D

#include "IPnt2D.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


ARARRAY_IMPLEMENTATION_FOR_TYPE(IPnt2D, ipnt2d);


void ipnt2d_p_min_p(
        const IPnt2D  * p0,
              IPnt2D  * pr
        )
{
    ic2_c_min_c(
        & p0->c,
        & pr->c
        );
}

void ipnt2d_pp_min_p(
        const IPnt2D  * p0,
        const IPnt2D  * p1,
              IPnt2D  * pr
        )
{
    ic2_cc_min_c(
        & p0->c,
        & p1->c,
        & pr->c
        );
}

void ipnt2d_p_max_p(
        const IPnt2D  * p0,
              IPnt2D  * pr
        )
{
    ic2_c_max_c(
        & p0->c,
        & pr->c
        );
}

void ipnt2d_pp_max_p(
        const IPnt2D  * p0,
        const IPnt2D  * p1,
              IPnt2D  * pr
        )
{
    ic2_cc_max_c(
        & p0->c,
        & p1->c,
        & pr->c
        );
}

void ipnt2d_negate_p(
        IPnt2D  * pr
        )
{
    ic2_negate_c(
        & pr->c
        );
}

void ipnt2d_p_add_p(
        const IPnt2D  * p0,
              IPnt2D  * pr
        )
{
    ic2_c_add_c(
        & p0->c,
        & pr->c
        );
}

void ipnt2d_pp_add_p(
        const IPnt2D  * p0,
        const IPnt2D  * p1,
              IPnt2D  * pr
        )
{
    ic2_cc_add_c(
        & p0->c,
        & p1->c,
        & pr->c
        );
}

void ipnt2d_i_mul_p(
        const long       l0,
              IPnt2D   * pr
        )
{
    ic2_i_mul_c(
          l0,
        & pr->c
        );
}

void ipnt2d_i_div_p(
        const long       l0,
              IPnt2D   * pr
        )
{
    ic2_i_div_c(
          l0,
        & pr->c
        );
}

void ipnt2d_ip_div_p(
        const long       l0,
        const IPnt2D   * p0,
              IPnt2D   * pr
        )
{
    ic2_ic_div_c(
          l0,
        & p0->c,
        & pr->c
        );
}


/* ======================================================================== */
