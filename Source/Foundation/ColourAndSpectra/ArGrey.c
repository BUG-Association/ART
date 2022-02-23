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

#define ART_MODULE_NAME     ArGrey

#include "ArGrey.h"

typedef struct ArRGBTable
{
    double  phase;
    ArRGB   colour;
}
ArRGBTable;

#define COLOURTABLE_LENGTH  11

typedef struct ArGrey_GV
{
    ArRGBTable  falsecolourTable[COLOURTABLE_LENGTH];
}
ArGrey_GV;

ART_MODULE_INITIALISATION_FUNCTION
(
    ArGrey_GV  * argrey_gv;

    argrey_gv = ALLOC(ArGrey_GV);

    argrey_gv->falsecolourTable[ 0].phase  = 0.0;
    argrey_gv->falsecolourTable[ 0].colour = ARRGB( 0.0,0.0,0.0 );
    argrey_gv->falsecolourTable[ 1].phase  = 0.1;
    argrey_gv->falsecolourTable[ 1].colour = ARRGB( 0.0,0.0,0.7 );
    argrey_gv->falsecolourTable[ 2].phase  = 0.2;
    argrey_gv->falsecolourTable[ 2].colour = ARRGB( 0.0,0.0,1.0 );
    argrey_gv->falsecolourTable[ 3].phase  = 0.3;
    argrey_gv->falsecolourTable[ 3].colour = ARRGB( 0.0,0.65,1.0 );
    argrey_gv->falsecolourTable[ 4].phase  = 0.4;
    argrey_gv->falsecolourTable[ 4].colour = ARRGB( 0.0,1.0,1.0 );
    argrey_gv->falsecolourTable[ 5].phase  = 0.5;
    argrey_gv->falsecolourTable[ 5].colour = ARRGB( 0.0,1.0,0.7 );
    argrey_gv->falsecolourTable[ 6].phase  = 0.6;
    argrey_gv->falsecolourTable[ 6].colour = ARRGB( 0.0,1.0,0.0 );
    argrey_gv->falsecolourTable[ 7].phase  = 0.7;
    argrey_gv->falsecolourTable[ 7].colour = ARRGB( 0.65,1.0,0.0 );
    argrey_gv->falsecolourTable[ 8].phase  = 0.8;
    argrey_gv->falsecolourTable[ 8].colour = ARRGB( 1.0,1.0,0.0 );
    argrey_gv->falsecolourTable[ 9].phase  = 0.9;
    argrey_gv->falsecolourTable[ 9].colour = ARRGB( 1.0,0.7,0.0 );
    argrey_gv->falsecolourTable[10].phase  = 1.0;
    argrey_gv->falsecolourTable[10].colour = ARRGB( 1.0,0.0,0.0 );

    art_gv->argrey_gv = argrey_gv;
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    FREE( art_gv->argrey_gv );
)

void g_to_plusminus_rgb(
        const ART_GV  * art_gv,
        const ArGrey  * g_0,
              ArRGB   * r_r
        )
{
    (void) art_gv;
    
    double  d = ARGREY_G(*g_0);

    if ( d > 0. )
        *r_r = ARRGB( 0, d, 0 );
    else
        *r_r = ARRGB( M_ABS(d), 0, 0 );
}

void g_to_falsecolour_rgb(
        const ART_GV  * art_gv,
        const ArGrey  * g_0,
              ArRGB   * r_r
        )
{
    double  d = ARGREY_G(*g_0);

    unsigned int imin = 0;
    unsigned int imax = COLOURTABLE_LENGTH - 1;

    if ( d < art_gv->argrey_gv->falsecolourTable[imin].phase )
    {
        (*r_r) = art_gv->argrey_gv->falsecolourTable[imin].colour;
        return;
    }

    if ( d > art_gv->argrey_gv->falsecolourTable[imax].phase )
    {
        (*r_r) = art_gv->argrey_gv->falsecolourTable[imax].colour;
    }

    while ( imax - imin > 1 )
    {
        int imid = (imin + imax) / 2;
        double dmid = art_gv->argrey_gv->falsecolourTable[imid].phase;
        if (d == dmid)
        {
            (*r_r) = art_gv->argrey_gv->falsecolourTable[imid].colour;
            return;
        }
        if (d < dmid)
            imax = imid;
        else
            imin = imid;
    }

    double dmin = art_gv->argrey_gv->falsecolourTable[imin].phase;
    double dmax = art_gv->argrey_gv->falsecolourTable[imax].phase;

    ArRGB  cmin = art_gv->argrey_gv->falsecolourTable[imin].colour;
    ArRGB  cmax = art_gv->argrey_gv->falsecolourTable[imax].colour;

    d = (d - dmin) / (dmax - dmin);

    rgb_dcc_interpol_c( art_gv, d, & cmin, & cmax, r_r );
}


double g_sd_value_at_wavelength(
        const ART_GV  * art_gv,
        const ArGrey  * g_0,
        const double    d_0
        )
{
    (void) art_gv;
    (void) d_0;
    
    return ARGREY_G(*g_0);
}

void g_dd_clamp_s(
        const ART_GV  * art_gv,
        const double    d_0,
        const double    d_1,
              ArGrey  * c_0
        )
{
    (void) art_gv;
    
    m_dd_clamp_d( d_0, d_1, & ARGREY_G(*c_0) );
}

void g_s_debugprintf(
        const ART_GV  * art_gv,
        const ArGrey  * c_0
        )
{
    (void) art_gv;
    
    if ( ARGREY_S(*c_0) )
        printf( "ArGrey( % 5.3f, %s )\n",
            ARGREY_G(*c_0),
            ARCSR_NAME( ARGREY_S(*c_0) ) );
    else
        printf( "ArGrey( % 5.3f, <missing colourspace info> )\n",
            ARGREY_G(*c_0) );

    fflush(stdout);
}

void g8_s_debugprintf(
        const ART_GV   * art_gv,
        const ArGrey8  * c_0
        )
{
    printf( "ArGrey8( %d, < %s > )\n",
        ARGREY8_G(*c_0),
        ARCSR_NAME( DEFAULT_RGB_SPACE_REF ) );

    fflush(stdout);
}

void g16_s_debugprintf(
        const ART_GV    * art_gv,
        const ArGrey16  * c_0
        )
{
    printf( "ArGrey16( %d, < %s > )\n",
        ARGREY16_G(*c_0),
        ARCSR_NAME( DEFAULT_RGB_SPACE_REF ) );

    fflush(stdout);
}

void ga_s_debugprintf(
        const ART_GV       * art_gv,
        const ArGreyAlpha  * c_0
        )
{
    (void) art_gv;
    
    if ( ARGREYALPHA_S(*c_0) )
        printf( "ArGreyAlpha( % 5.3f, %s )\n",
            ARGREYALPHA_G(*c_0),
            ARCSR_NAME( ARGREYALPHA_S(*c_0) ) );
    else
        printf( "ArGreyAlpha( % 5.3f, <missing colourspace info> )\n",
            ARGREYALPHA_G(*c_0) );

    fflush(stdout);
}

void ga16_s_debugprintf(
        const ART_GV         * art_gv,
        const ArGreyAlpha16  * c_0
        )
{
    printf( "ArGreyAlpha16( %d, < %s > )\n",
        ARGREYALPHA16_G(*c_0),
        ARCSR_NAME( DEFAULT_RGB_SPACE_REF ) );

    fflush(stdout);
}

void ga32_s_debugprintf(
        const ART_GV         * art_gv,
        const ArGreyAlpha32  * c_0
        )
{
    printf( "ArGrey32( %d, < %s > )\n",
        ARGREYALPHA32_G(*c_0),
        ARCSR_NAME( DEFAULT_RGB_SPACE_REF ) );

    fflush(stdout);
}

/* ======================================================================== */
