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

#define ART_MODULE_NAME     ArRGB

#include "ArRGB.h"

#include "SpectralDatatype_ImplementationMacros.h"

CANONICAL_GV_FOR_ISR_WITH_ADDITIONAL_FIELDS(ArRGB, pthread_mutex_t mutex; ArColourSpace const  * default_rgbspace_ref; )

#define ARRGB_GV                     art_gv->arrgb_gv
#define ARRGB_MUTEX                  ARRGB_GV->mutex
#define ARRGB_DEFAULT_RGBSPACE_REF   ARRGB_GV->default_rgbspace_ref
#define ARRGB_CHANNELS               ARRGB_GV->channels
#define ARRGB_FIRST_VISIBLE_CHANNEL  ARRGB_GV->first_visible_channel
#define ARRGB_SAMPLE_BOUND           ARRGB_GV->sample_bound
#define ARRGB_SAMPLING_RANGE         ARRGB_GV->sampling_range
#define ARRGB_SAMPLE_CENTER          ARRGB_GV->sample_center
#define ARRGB_SAMPLE_WIDTH           ARRGB_GV->sample_width
#define ARRGB_SAMPLE_WIDTH_DIV_2     ARRGB_GV->sample_width_div_2
#define ARRGB_SAMPLE_WEIGHT          ARRGB_GV->sample_weight
#define ARRGB_SAMPLE_BASIS           ARRGB_GV->sample_basis
#define ARRGB_SHORTNAME_STRING       ARRGB_GV->shortname_string
#define ARRGB_TYPENAME_STRING        ARRGB_GV->typename_string
#define ARRGB_DESCRIPTION_STRING     ARRGB_GV->description_string
#define ARRGB_ZERO                   ARRGB_GV->zero
#define ARRGB_UNIT                   ARRGB_GV->unit

ART_MODULE_INITIALISATION_FUNCTION
(
    ARRGB_GV = ALLOC( ArRGB_GV );

    pthread_mutex_init( & ARRGB_MUTEX, NULL );

    ARRGB_DEFAULT_RGBSPACE_REF = arcolourspace_sRGB( art_gv );

    ARRGB_CHANNELS = 3;
    ARRGB_FIRST_VISIBLE_CHANNEL = 0;
    ARRGB_SAMPLE_BOUND = ALLOC_ARRAY( double, 4 );

    for ( unsigned int i = 0; i < 4; i++ )
        ARRGB_SAMPLE_BOUND[i] = 380.0 NM + i * 110.0 NM;

    ARRGB_SAMPLING_RANGE =
        ARRGB_SAMPLE_BOUND[ARRGB_CHANNELS] - ARRGB_SAMPLE_BOUND[0];

    ARRGB_SAMPLE_CENTER      = ALLOC_ARRAY( double, 3 );
    ARRGB_SAMPLE_WIDTH       = ALLOC_ARRAY( double, 3 );
    ARRGB_SAMPLE_WIDTH_DIV_2 = ALLOC_ARRAY( double, 3 );
    ARRGB_SAMPLE_WEIGHT      = ALLOC_ARRAY( double, 3 );
    ARRGB_SAMPLE_BASIS       = ALLOC_ARRAY( ArPSSpectrum, 3 );

    for ( unsigned int i = 0; i < ARRGB_CHANNELS; i++ )
    {
        ARRGB_SAMPLE_CENTER[i] =
            ( ARRGB_SAMPLE_BOUND[ i + 1 ] + ARRGB_SAMPLE_BOUND[ i ] ) / 2.0;

        ARRGB_SAMPLE_WIDTH[i] =
            ARRGB_SAMPLE_BOUND[ i + 1 ] - ARRGB_SAMPLE_BOUND[ i ];

        ARRGB_SAMPLE_WIDTH_DIV_2[i] = ARRGB_SAMPLE_WIDTH[i] / 2.0;

        ARRGB_SAMPLE_WEIGHT[i] = ARRGB_SAMPLE_WIDTH[i] / ARRGB_SAMPLING_RANGE;

        ARRGB_SAMPLE_BASIS[i].size  = 2;
        ARRGB_SAMPLE_BASIS[i].scale =
            1.0 / (  ARRGB_SAMPLE_BOUND[ i + 1 ]
                   - ARRGB_SAMPLE_BOUND[ i     ] );

        ARRGB_SAMPLE_BASIS[i].array = ALLOC_ARRAY( Pnt2D, 2 );
        ARRGB_SAMPLE_BASIS[i].array[0] = PNT2D( ARRGB_SAMPLE_BOUND[ i    ], 1.0 );
        ARRGB_SAMPLE_BASIS[i].array[1] = PNT2D( ARRGB_SAMPLE_BOUND[ i + 1], 1.0 );
    }

    ARRGB_SHORTNAME_STRING = "RGB";
    ARRGB_TYPENAME_STRING = "RGB";
    ARRGB_DESCRIPTION_STRING = "visible range, colour values";

    ARRGB_ZERO = rgb_d_alloc_init( art_gv, 0.0 );
    ARRGB_UNIT = rgb_d_alloc_init( art_gv, 1.0 );
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    FREE_ARRAY(ARRGB_SAMPLE_BOUND);
    FREE_ARRAY(ARRGB_SAMPLE_CENTER);
    FREE_ARRAY(ARRGB_SAMPLE_WIDTH);
    FREE_ARRAY(ARRGB_SAMPLE_WIDTH_DIV_2);
    FREE_ARRAY(ARRGB_SAMPLE_WEIGHT);

    for ( unsigned int i = 0; i < ARRGB_CHANNELS; i++ )
        pss_freearray_s( art_gv, & ARRGB_SAMPLE_BASIS[i] );

    FREE_ARRAY(ARRGB_SAMPLE_BASIS);

    rgb_free( art_gv, ARRGB_ZERO );
    rgb_free( art_gv, ARRGB_UNIT );

    pthread_mutex_destroy( & ARRGB_MUTEX );

    FREE(ARRGB_GV);
)

#define _ISR_CHANNELS           3
#define _ISR_C(_s0)             (_s0).c
#define _ISR_CI(_s0,_i)         C3_CI(_ISR_C(_s0),(_i))

CANONICAL_IMPLEMENTATION_FOR_ISR( ArRGB, arrgb, ARRGB, rgb, c3, s );

#undef _ISR_CHANNELS
#undef _ISR_C
#undef _ISR_CI

void set_default_rgbspace_ref(
        ART_GV               * art_gv,
        ArColourSpace const  * newRef
        )
{
    pthread_mutex_lock( & ARRGB_MUTEX );

    ARRGB_DEFAULT_RGBSPACE_REF = newRef;

    art_foundation_initialise_spectral_subsystem( art_gv );

    pthread_mutex_unlock( & ARRGB_MUTEX );
}

ArColourSpace const * default_rgbspace_ref(
        const ART_GV  * art_gv
        )
{
    return ARRGB_DEFAULT_RGBSPACE_REF;
}



/* ---------------------------------------------------------------------------

    'rgb_cc_convolve_d' function

    Raises an exception and terminates the calling application. This is the
    correct thing to do, since this kind of operation does not make sense in
    colour space. Sometimes the "ArSpectrum" data type polymorphism breaks down
    after all...

------------------------------------------------------------------------aw- */

void rgb_ss_convolve_d(
        const ART_GV    * art_gv,
        const ArRGB  * r0,
        const ArRGB  * r1,
              double    * dr
        )
{
    (void) art_gv;
    (void) r0;
    (void) r1;
    (void) dr;
    
    ART_ERRORHANDLING_FATAL_ERROR(
        "convolution operation not defined in colour space - "
        "switch ART to a spectral ISR to avoid this error"
        );
}

double rgb_sd_value_at_wavelength(
        const ART_GV    * art_gv,
        const ArRGB  * r_0,
        const double      d_0
        )
{
    (void) art_gv;
    (void) r_0;
    (void) d_0;
    
    ART_ERRORHANDLING_FATAL_ERROR(
        "ArSpectrum value queries for a specific wavelength not "
        "defined in colour space - "
        "switch ART to a spectral ISR to avoid this error"
        );

    return 0.0;
}

void rgb_sdd_sample_at_wavelength_s(
        const ART_GV    * art_gv,
        const ArRGB  * r_0,
        const double      d_0,
        const double      d_1,
              ArRGB  * r_r
        )
{
    (void) art_gv;
    (void) r_0;
    (void) d_0;
    (void) d_1;
    (void) r_r;
    
    ART_ERRORHANDLING_FATAL_ERROR(
        "ArSpectrum sample queries for a specific wavelength not "
        "defined in colour space - "
        "switch ART to a spectral ISR to avoid this error"
        );
}

double rgb_cc_dist(
        const ART_GV  * art_gv,
        const ArRGB   * c0,
        const ArRGB   * c1
        )
{
    (void) art_gv;
    
    return c3_cc_dist( & c0->c, & c1->c );
}

double rgb_ss_convolve(
        const ART_GV    * art_gv,
        const ArRGB  * c0,
        const ArRGB  * c1
        )
{
    (void) art_gv;
    (void) c0;
    (void) c1;
    
    ART_ERRORHANDLING_FATAL_ERROR(
        "ArSpectrum value convolution not defined in colour space - "
        "switch ART to a spectral ISR to avoid this error"
        );

    return 0.0;
}

void rgb_s_debugprintf(
        const ART_GV    * art_gv,
        const ArRGB  * c_0
        )
{
    printf( "ArRGB( % 5.3f, % 5.3f, % 5.3f, %s )\n",
        ARRGB_R(*c_0),
        ARRGB_G(*c_0),
        ARRGB_B(*c_0),
        ARCSR_NAME( DEFAULT_RGB_SPACE_REF ) );

    fflush(stdout);
}

void rgb_s_mathematicaprintf(
        const ART_GV    * art_gv,
        const ArRGB  * c_0
        )
{
    printf( "ArRGB{ % 5.3f, % 5.3f, % 5.3f, %s }\n",
        ARRGB_R(*c_0),
        ARRGB_G(*c_0),
        ARRGB_B(*c_0),
        ARCSR_NAME( DEFAULT_RGB_SPACE_REF ) );

    fflush(stdout);
}

void frgb_c_debugprintf(
        const ART_GV     * art_gv,
        const ArFloatRGB  * c_0
        )
{
    (void) art_gv;
    
    printf( "ArFloatRGB( % 5.3f, % 5.3f, % 5.3f )\n",
        ARRGB_R(*c_0),
        ARRGB_G(*c_0),
        ARRGB_B(*c_0)
        );

    fflush(stdout);
}

/* ======================================================================== */
