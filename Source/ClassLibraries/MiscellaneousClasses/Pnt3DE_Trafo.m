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

#define ART_MODULE_NAME     Pnt3DE_Trafo

#import "Pnt3DE_Trafo.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


void pnt3de_p_trafo_p(
        const Pnt3DE          * p0,
              id <ArpTrafo3D>   trafo,
              Pnt3DE          * pr
        )
{
    if ( trafo )
    {
        PNT3DE_FACEINDEX(*pr) = PNT3DE_FACEINDEX(*p0);

        [ trafo transformPnt3D
            : & PNT3DE_COORD(*p0)
            : & PNT3DE_COORD(*pr)
            ];

        if ( pnt3de_p_validNormal(p0) )
        {
            [ trafo transformNormalVec3D
                : & PNT3DE_NORMAL(*p0)
                : & PNT3DE_NORMAL(*pr)
                ];

            vec3d_norm_v( & PNT3DE_NORMAL(*pr) );
        }
        else
            PNT3DE_NORMAL(*pr) = PNT3DE_NORMAL(*p0);
    }
    else
        *pr = *p0;
}

void pnt3de_p_backtrafo_p(
        const Pnt3DE          * p0,
              id <ArpTrafo3D>   trafo,
              Pnt3DE          * pr
        )
{
    if ( trafo )
    {
        PNT3DE_FACEINDEX(*pr) = PNT3DE_FACEINDEX(*p0);

        [ trafo backtrafoPnt3D
            : & PNT3DE_COORD(*p0)
            : & PNT3DE_COORD(*pr) ];

        if ( pnt3de_p_validNormal(p0) )
        {
            [ trafo backtrafoNormalVec3D
                : & PNT3DE_NORMAL(*p0)
                : & PNT3DE_NORMAL(*pr) ];

            vec3d_norm_v( & PNT3DE_NORMAL(*pr) );
        }
        else
            PNT3DE_NORMAL(*pr) = PNT3DE_NORMAL(*p0);
    }
    else
        *pr = *p0;
}

void pnt3de_trafo_p(
        id <ArpTrafo3D>   trafo,
        Pnt3DE          * pr
        )
{
    if ( trafo )
    {
        Pnt3DE  pt;

        pnt3de_p_trafo_p( pr, trafo, & pt );

        *pr = pt;
    }
}

void pnt3de_backtrafo_p(
        id <ArpTrafo3D>   trafo,
        Pnt3DE          * pr
        )
{
    if ( trafo )
    {
        Pnt3DE  pt;

        pnt3de_p_backtrafo_p( pr, trafo, & pt );

        *pr = pt;
    }
}

// ===========================================================================

