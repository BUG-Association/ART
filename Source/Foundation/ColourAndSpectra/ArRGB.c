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

#define ART_MODULE_NAME     ArRGB

#include "ArRGB.h"

typedef struct ArRGB_GV
{
    pthread_mutex_t        mutex;
    ArColourSpace const  * default_rgbspace_ref;
    ArRGB                  default_white;
    ArRGB                  default_black;
}
ArRGB_GV;


#define ARRGB_GV                    art_gv->arrgb_gv
#define ARRGB_MUTEX                 ARRGB_GV->mutex
#define ARRGB_DEFAULT_RGBSPACE_REF  ARRGB_GV->default_rgbspace_ref
#define ARRGB_DEFAULT_WHITE         ARRGB_GV->default_white
#define ARRGB_DEFAULT_BLACK         ARRGB_GV->default_black


ART_MODULE_INITIALISATION_FUNCTION
(
    ARRGB_GV = ALLOC(ArRGB_GV);

    pthread_mutex_init( & ARRGB_MUTEX, NULL );

    ARRGB_DEFAULT_RGBSPACE_REF = arcolourspace_sRGB( art_gv );

    ArRGB_initialise_spectrum_subsystem( art_gv );
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    pthread_mutex_destroy( & art_gv->arrgb_gv->mutex );

    FREE( art_gv->arrgb_gv );
)

ART_SPECTRUM_MODULE_INITIALISATION_FUNCTION
(
    ARRGB_DEFAULT_WHITE = ARRGB( 1.0, 1.0, 1.0 );
    ARRGB_DEFAULT_BLACK = ARRGB( 0.0, 0.0, 0.0 );
)


void set_default_rgbspace_ref(
        ART_GV               * art_gv,
        ArColourSpace const  * newRef
        )
{
    pthread_mutex_lock( & art_gv->arrgb_gv->mutex );

    art_gv->arrgb_gv->default_rgbspace_ref = newRef;

    art_foundation_initialise_spectral_subsystem( art_gv );

    pthread_mutex_unlock( & art_gv->arrgb_gv->mutex );
}

ArColourSpace const * default_rgbspace_ref(
        const ART_GV  * art_gv
        )
{
    return art_gv->arrgb_gv->default_rgbspace_ref;
}

ArRGB const * arrgb_unit(
        const ART_GV  * art_gv
        )
{
    return & art_gv->arrgb_gv->default_white;
}

ArRGB const * arrgb_zero(
        const ART_GV  * art_gv
        )
{
    return & art_gv->arrgb_gv->default_black;
}

void rgb_ssd_interpol_s(
        const ART_GV  * art_gv,
        const ArRGB   * c0,
        const ArRGB   * c1,
        const double    d0,
              ArRGB   * cr
        )
{
    c3_dcc_interpol_c(
          d0,
        & ARRGB_C( *c0 ),
        & ARRGB_C( *c1 ),
        & ARRGB_C( *cr ) );
}

void rgb_dd_clamp_s(
        const ART_GV  * art_gv,
        const double    d0,
        const double    d1,
              ArRGB   * cr
        )
{
    c3_dd_clamp_c(
          d0,
          d1,
        & ARRGB_C(*cr) );
}

void rgb_d_init_s(
        const ART_GV  * art_gv,
        const double    d0,
              ArRGB   * cr
        )
{
    for ( int i = 0; i < ARRGB_CHANNELS; i++ )
        ARRGB_CI( *cr, i ) = d0;

    ARRGB_S( *cr ) = DEFAULT_RGB_SPACE_REF;
}

double rgb_sd_value_at_wavelength(
        const ART_GV  * art_gv,
        const ArRGB   * c0,
        const double    d0
        )
{
    ART_ERRORHANDLING_FATAL_ERROR(
        "ArSpectrum value queries for a specific wavelength not "
        "defined in colour space - "
        "switch ART to a spectral ISR to avoid this error"
        );

    return 0.0;
}

void rgb_s_debugprintf(
        const ART_GV  * art_gv,
        const ArRGB   * c0
        )
{
    printf(
        "ArRGB( % 5.3f, % 5.3f, % 5.3f, %s )\n",
        ARRGB_R(*c0),
        ARRGB_G(*c0),
        ARRGB_B(*c0),
        ARCSR_NAME( ARRGB_S(*c0) ) );

    fflush(stdout);
}

/* ======================================================================== */
