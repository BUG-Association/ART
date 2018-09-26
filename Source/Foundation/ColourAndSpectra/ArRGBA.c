/* ===========================================================================

    Copyright (c) 1996-2018 The ART Development Team
    ------------------------------------------------

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

#define ART_MODULE_NAME     ArRGBA

#include "ArRGBA.h"

typedef struct ArRGBA_GV
{
    ArRGBA  default_white;
    ArRGBA  default_black;
}
ArRGBA_GV;

ART_MODULE_INITIALISATION_FUNCTION
(
    ArRGBA_GV  * arrgba_gv;

    arrgba_gv = ALLOC(ArRGBA_GV);

    art_gv->arrgba_gv = arrgba_gv;

    ArRGBA_initialise_spectrum_subsystem( art_gv );
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    FREE( art_gv->arrgba_gv );
)

ART_SPECTRUM_MODULE_INITIALISATION_FUNCTION
(
    art_gv->arrgba_gv->default_white = ARRGBA( 1.0, 1.0, 1.0, 1.0 );
    art_gv->arrgba_gv->default_black = ARRGBA( 0.0, 0.0, 0.0, 1.0 );
)

ArRGBA const * arrgba_unit(
        const ART_GV  * art_gv
        )
{
    return & art_gv->arrgba_gv->default_white;
}

ArRGBA const * arrgba_zero(
        const ART_GV  * art_gv
        )
{
    return & art_gv->arrgba_gv->default_black;
}


void rgba_d_init_c(
        const ART_GV  * art_gv,
        const double    d0,
              ArRGBA  * cr
        )
{
    for ( int i = 0; i < ARRGB_CHANNELS; i++ )
        ARRGBA_CI( *cr, i ) = d0;

    ARRGBA_A( *cr ) = 1.0;
    ARRGBA_S( *cr ) = DEFAULT_RGB_SPACE_REF;
}


void rgba_c_add_c(
        const ART_GV  * art_gv,
        const ArRGBA  * c0,
              ArRGBA  * cr
        )
{
    c3_c_add_c(
        & ARRGB_C(ARRGBA_C(*c0)),
        & ARRGB_C(ARRGBA_C(*cr)) );

    ARRGBA_A(*cr) += ARRGBA_A(*c0);
}

void rgba_d_mul_c(
        const ART_GV  * art_gv,
        const double    d0,
              ArRGBA  * cr
        )
{
    c3_d_mul_c(
          d0,
        & ARRGB_C(ARRGBA_C(*cr)) );

    ARRGBA_A(*cr) *= d0;
}

void rgba_dd_clamp_c(
        const ART_GV  * art_gv,
        const double    d0,
        const double    d1,
              ArRGBA  * cr
        )
{
    c3_dd_clamp_c(
          d0,
          d1,
        & ARRGB_C(ARRGBA_C(*cr)) );

    m_dd_clamp_d(
          d0,
          d1,
        & ARRGBA_A(*cr) );
}

void rgba_ccd_interpol_c(
        const ART_GV  * art_gv,
        const ArRGBA  * c0,
        const ArRGBA  * c1,
        const double    d0,
              ArRGBA  * cr
        )
{
    c3_dcc_interpol_c(
          d0,
        & ARRGB_C(ARRGBA_C(*c0)),
        & ARRGB_C(ARRGBA_C(*c1)),
        & ARRGB_C(ARRGBA_C(*cr)) );

    m_ddd_interpol_d(
          ARRGBA_A(*c0),
          ARRGBA_A(*c1),
          d0,
        & ARRGBA_A(*cr) );
}

void rgba_s_debugprintf(
        const ART_GV  * art_gv,
        const ArRGBA  * c_0
        )
{
    printf( "ArRGBA( % 5.3f, % 5.3f, % 5.3f, % 5.3f, %s )\n",
        ARRGBA_R(*c_0),
        ARRGBA_G(*c_0),
        ARRGBA_B(*c_0),
        ARRGBA_A(*c_0),
        ARCSR_NAME( ARRGBA_S(*c_0) ) );

    fflush(stdout);
}

int rgba_s_valid(
        const ART_GV  * art_gv,
        const ArRGBA  * c_0
        )
{
    //   Null pointer? Direct return in that case.
    
    if ( ! c_0 )
        return 0;
    
    unsigned int result = 1;
    
    //   Sanity checks on various features
    
    //   Nonzero content array?
    
    for ( int i = 0; i < 3; i++ )
    {
        if (   ARRGBA_CI( *c_0, i ) < 0.0
            || m_d_isInf( ARRGBA_CI( *c_0, i ) )
            || m_d_isNaN( ARRGBA_CI( *c_0, i ) ) )
        {
            ART_ERRORHANDLING_WARNING(
                "ArRGB coordinate %d is invalid (%f)",
                i,
                ARRGBA_CI( *c_0, i )
                );

            result = 0;
        }
    }

    return result;
}

/* ======================================================================== */
