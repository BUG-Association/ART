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

#define ART_MODULE_NAME     ArUntaggedRGB

#include "ArUntaggedRGB.h"

#include "SpectralDatatype_ImplementationMacros.h"

CANONICAL_GV_FOR_ISR_WITH_ADDITIONAL_FIELD(
        ArUT_RGB,
        ArColourSpace const *,
        computation_space
        )

#define ARUT_RGB_GV                     art_gv->arut_rgb_gv
#define ARUT_RGB_CHANNELS               ARUT_RGB_GV->channels
#define ARUT_RGB_FIRST_VISIBLE_CHANNEL  ARUT_RGB_GV->first_visible_channel
#define ARUT_RGB_SAMPLE_BOUND           ARUT_RGB_GV->sample_bound
#define ARUT_RGB_SAMPLING_RANGE         ARUT_RGB_GV->sampling_range
#define ARUT_RGB_SAMPLE_CENTER          ARUT_RGB_GV->sample_center
#define ARUT_RGB_SAMPLE_WIDTH           ARUT_RGB_GV->sample_width
#define ARUT_RGB_SAMPLE_WIDTH_DIV_2     ARUT_RGB_GV->sample_width_div_2
#define ARUT_RGB_SAMPLE_WEIGHT          ARUT_RGB_GV->sample_weight
#define ARUT_RGB_SAMPLE_BASIS           ARUT_RGB_GV->sample_basis
#define ARUT_RGB_SHORTNAME_STRING       ARUT_RGB_GV->shortname_string
#define ARUT_RGB_TYPENAME_STRING        ARUT_RGB_GV->typename_string
#define ARUT_RGB_DESCRIPTION_STRING     ARUT_RGB_GV->description_string
#define ARUT_RGB_ZERO                   ARUT_RGB_GV->zero
#define ARUT_RGB_UNIT                   ARUT_RGB_GV->unit
#define ARUT_RGB_COMPUTATION_SPACE      ARUT_RGB_GV->computation_space

ART_MODULE_INITIALISATION_FUNCTION
(
    ARUT_RGB_GV = ALLOC( ArUT_RGB_GV );

    ARUT_RGB_CHANNELS = 3;
    ARUT_RGB_FIRST_VISIBLE_CHANNEL = 0;
    ARUT_RGB_SAMPLE_BOUND = ALLOC_ARRAY( double, 4 );

    for ( unsigned int i = 0; i < 4; i++ )
        ARUT_RGB_SAMPLE_BOUND[i] = 380.0 NM + i * 110.0 NM;

    ARUT_RGB_SAMPLING_RANGE =
        ARUT_RGB_SAMPLE_BOUND[ARUT_RGB_CHANNELS] - ARUT_RGB_SAMPLE_BOUND[0];

    ARUT_RGB_SAMPLE_CENTER      = ALLOC_ARRAY( double, 3 );
    ARUT_RGB_SAMPLE_WIDTH       = ALLOC_ARRAY( double, 3 );
    ARUT_RGB_SAMPLE_WIDTH_DIV_2 = ALLOC_ARRAY( double, 3 );
    ARUT_RGB_SAMPLE_WEIGHT      = ALLOC_ARRAY( double, 3 );
    ARUT_RGB_SAMPLE_BASIS       = ALLOC_ARRAY( ArPSSpectrum, 3 );

    for ( unsigned int i = 0; i < ARUT_RGB_CHANNELS; i++ )
    {
        ARUT_RGB_SAMPLE_CENTER[i] =
            ( ARUT_RGB_SAMPLE_BOUND[ i + 1 ] + ARUT_RGB_SAMPLE_BOUND[ i ] ) / 2.0;

        ARUT_RGB_SAMPLE_WIDTH[i] =
            ARUT_RGB_SAMPLE_BOUND[ i + 1 ] - ARUT_RGB_SAMPLE_BOUND[ i ];

        ARUT_RGB_SAMPLE_WIDTH_DIV_2[i] = ARUT_RGB_SAMPLE_WIDTH[i] / 2.0;

        ARUT_RGB_SAMPLE_WEIGHT[i] = ARUT_RGB_SAMPLE_WIDTH[i] / ARUT_RGB_SAMPLING_RANGE;

        ARUT_RGB_SAMPLE_BASIS[i].size  = 2;
        ARUT_RGB_SAMPLE_BASIS[i].scale =
            1.0 / (  ARUT_RGB_SAMPLE_BOUND[ i + 1 ]
                   - ARUT_RGB_SAMPLE_BOUND[ i     ] );

        ARUT_RGB_SAMPLE_BASIS[i].array = ALLOC_ARRAY( Pnt2D, 2 );
        ARUT_RGB_SAMPLE_BASIS[i].array[0] = PNT2D( ARUT_RGB_SAMPLE_BOUND[ i    ], 1.0 );
        ARUT_RGB_SAMPLE_BASIS[i].array[1] = PNT2D( ARUT_RGB_SAMPLE_BOUND[ i + 1], 1.0 );
    }

    ARUT_RGB_SHORTNAME_STRING = "RGB";
    ARUT_RGB_TYPENAME_STRING = "RGB";
    ARUT_RGB_DESCRIPTION_STRING = "visible range, colour values";

    ARUT_RGB_ZERO = ut_rgb_d_alloc_init( art_gv, 0.0 );
    ARUT_RGB_UNIT = ut_rgb_d_alloc_init( art_gv, 1.0 );

    ARUT_RGB_COMPUTATION_SPACE = 0;
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    FREE_ARRAY(ARUT_RGB_SAMPLE_BOUND);
    FREE_ARRAY(ARUT_RGB_SAMPLE_CENTER);
    FREE_ARRAY(ARUT_RGB_SAMPLE_WIDTH);
    FREE_ARRAY(ARUT_RGB_SAMPLE_WIDTH_DIV_2);
    FREE_ARRAY(ARUT_RGB_SAMPLE_WEIGHT);

    for ( unsigned int i = 0; i < ARUT_RGB_CHANNELS; i++ )
        pss_freearray_s( art_gv, & ARUT_RGB_SAMPLE_BASIS[i] );

    FREE_ARRAY(ARUT_RGB_SAMPLE_BASIS);

    ut_rgb_free( art_gv, ARUT_RGB_ZERO );
    ut_rgb_free( art_gv, ARUT_RGB_UNIT );

    FREE(ARUT_RGB_GV);
)

#define _ISR_CHANNELS           3
#define _ISR_C(_s0)             (_s0).c
#define _ISR_CI(_s0,_i)         C3_CI(_ISR_C(_s0),(_i))

CANONICAL_IMPLEMENTATION_FOR_ISR( ArUT_RGB, arut_rgb, ARUT_RGB, ut_rgb, c3, s );

#undef _ISR_CHANNELS
#undef _ISR_C
#undef _ISR_CI


void set_rgb_computationspace_ref(
        ART_GV               * art_gv,
        ArColourSpace const  * newRef
        )
{
    art_gv->arut_rgb_gv->computation_space = newRef;
}

ArColourSpace const * rgb_computationspace_ref(
        const ART_GV  * art_gv
        )
{
    return art_gv->arut_rgb_gv->computation_space;
}

void utf_rgb_cs_to_rgb(
        const ART_GV            * art_gv,
        const ArUTF_RGB         * utf_rgb,
        const ArColourSpaceRef    rgbspace,
              ArRGB             * rgb
        )
{
    c3_fc_to_c( & ARUTF_RGB_C(*utf_rgb), & ARRGB_C(*rgb) );
    ARRGB_S(*rgb) = rgbspace;
}


/* ---------------------------------------------------------------------------

    'ut_rgb_cc_convolve_d' function

    Raises an exception and terminates the calling application. This is the
    correct thing to do, since this kind of operation does not make sense in
    colour space. Sometimes the "ArSpectrum" data type polymorphism breaks down
    after all...

------------------------------------------------------------------------aw- */

void ut_rgb_ss_convolve_d(
        const ART_GV    * art_gv,
        const ArUT_RGB  * r0,
        const ArUT_RGB  * r1,
              double    * dr
        )
{
    ART_ERRORHANDLING_FATAL_ERROR(
        "convolution operation not defined in colour space - "
        "switch ART to a spectral ISR to avoid this error"
        );
}

double ut_rgb_sd_value_at_wavelength(
        const ART_GV    * art_gv,
        const ArUT_RGB  * r_0,
        const double      d_0
        )
{
    ART_ERRORHANDLING_FATAL_ERROR(
        "ArSpectrum value queries for a specific wavelength not "
        "defined in colour space - "
        "switch ART to a spectral ISR to avoid this error"
        );

    return 0.0;
}

void ut_rgb_sdd_sample_at_wavelength_s(
        const ART_GV    * art_gv,
        const ArUT_RGB  * r_0,
        const double      d_0,
        const double      d_1,
              ArUT_RGB  * r_r
        )
{
    ART_ERRORHANDLING_FATAL_ERROR(
        "ArSpectrum sample queries for a specific wavelength not "
        "defined in colour space - "
        "switch ART to a spectral ISR to avoid this error"
        );
}

double ut_rgb_ss_convolve(
        const ART_GV    * art_gv,
        const ArUT_RGB  * c0,
        const ArUT_RGB  * c1
        )
{
    ART_ERRORHANDLING_FATAL_ERROR(
        "ArSpectrum value convolution not defined in colour space - "
        "switch ART to a spectral ISR to avoid this error"
        );

    return 0.0;
}

void ut_rgb_s_debugprintf(
        const ART_GV    * art_gv,
        const ArUT_RGB  * c_0
        )
{
    printf( "ArUT_RGB( % 5.3f, % 5.3f, % 5.3f, %s )\n",
        ARUT_RGB_R(*c_0),
        ARUT_RGB_G(*c_0),
        ARUT_RGB_B(*c_0),
        ARCSR_NAME( DEFAULT_RGB_SPACE_REF ) );

    fflush(stdout);
}

void ut_rgb_s_mathematicaprintf(
        const ART_GV    * art_gv,
        const ArUT_RGB  * c_0
        )
{
    printf( "ArUT_RGB{ % 5.3f, % 5.3f, % 5.3f, %s }\n",
        ARUT_RGB_R(*c_0),
        ARUT_RGB_G(*c_0),
        ARUT_RGB_B(*c_0),
        ARCSR_NAME( DEFAULT_RGB_SPACE_REF ) );

    fflush(stdout);
}

void utf_rgb_s_debugprintf(
        const ART_GV     * art_gv,
        const ArUTF_RGB  * c_0
        )
{
    printf( "ArUTF_RGB( % 5.3f, % 5.3f, % 5.3f, %s )\n",
        ARUTF_RGB_R(*c_0),
        ARUTF_RGB_G(*c_0),
        ARUTF_RGB_B(*c_0),
        ARCSR_NAME( DEFAULT_RGB_SPACE_REF ) );

    fflush(stdout);
}

/* ======================================================================== */
