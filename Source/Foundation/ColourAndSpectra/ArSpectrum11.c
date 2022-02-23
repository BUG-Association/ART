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

#define ART_MODULE_NAME     ArSpectrum11

#include "ART_Foundation_Geometry.h"
#include "ArSpectrum11.h"

#include "SpectralDatatype_ImplementationMacros.h"

CANONICAL_GV_FOR_ISR(ArSpectrum11)

#define _ISR_CHANNELS           11

#define S11_GV                     art_gv->arspectrum11_gv
#define S11_CHANNELS               S11_GV->channels
#define S11_FIRST_VISIBLE_CHANNEL  S11_GV->first_visible_channel
#define S11_SAMPLE_BOUND           S11_GV->sample_bound
#define S11_SAMPLING_RANGE         S11_GV->sampling_range
#define S11_SAMPLE_CENTER          S11_GV->sample_center
#define S11_SAMPLE_WIDTH           S11_GV->sample_width
#define S11_SAMPLE_WIDTH_DIV_2     S11_GV->sample_width_div_2
#define S11_SAMPLE_WEIGHT          S11_GV->sample_weight
#define S11_SAMPLE_BASIS           S11_GV->sample_basis
#define S11_SHORTNAME_STRING       S11_GV->shortname_string
#define S11_TYPENAME_STRING        S11_GV->typename_string
#define S11_DESCRIPTION_STRING     S11_GV->description_string
#define S11_ZERO                   S11_GV->zero
#define S11_UNIT                   S11_GV->unit
#define S11_COMPUTATION_SPACE      S11_GV->computation_space

ART_MODULE_INITIALISATION_FUNCTION
(
    S11_GV = ALLOC( ArSpectrum11_GV );

    S11_CHANNELS = _ISR_CHANNELS;
    S11_FIRST_VISIBLE_CHANNEL = -1;   //   placeholder - determined later

    S11_SAMPLE_BOUND = ALLOC_ARRAY( double, S11_CHANNELS + 1 );

    double  range_start = 320.0 NM;
    double  channel_width = 40.0 NM;

    for ( unsigned int i = 0; i < (S11_CHANNELS + 1); i++ )
    {
        S11_SAMPLE_BOUND[i] = range_start + i * channel_width;

        double  next_sample_bound =
            range_start + (i+1) * channel_width;

        //   we already count a channel towards the visible range if it
        //   partially lies within it

        if (    S11_FIRST_VISIBLE_CHANNEL == -1
             && next_sample_bound > 380.0 NM )
            S11_FIRST_VISIBLE_CHANNEL = i;
    }

    S11_SAMPLING_RANGE =
        S11_SAMPLE_BOUND[S11_CHANNELS] - S11_SAMPLE_BOUND[0];

    double  uniform_channel_width = S11_SAMPLING_RANGE / S11_CHANNELS;

    S11_SAMPLE_CENTER      = ALLOC_ARRAY( double, S11_CHANNELS );
    S11_SAMPLE_WIDTH       = ALLOC_ARRAY( double, S11_CHANNELS );
    S11_SAMPLE_WIDTH_DIV_2 = ALLOC_ARRAY( double, S11_CHANNELS );
    S11_SAMPLE_WEIGHT      = ALLOC_ARRAY( double, S11_CHANNELS );
    S11_SAMPLE_BASIS       = ALLOC_ARRAY( ArPSSpectrum, S11_CHANNELS );

    for ( unsigned int i = 0; i < S11_CHANNELS; i++ )
    {
        S11_SAMPLE_CENTER[i] =
            ( S11_SAMPLE_BOUND[ i + 1 ] + S11_SAMPLE_BOUND[ i ] ) / 2.0;

        S11_SAMPLE_WIDTH[i] =
            S11_SAMPLE_BOUND[ i + 1 ] - S11_SAMPLE_BOUND[ i ];

        S11_SAMPLE_WIDTH_DIV_2[i] = S11_SAMPLE_WIDTH[i] / 2.0;

        if ( S11_SAMPLE_WIDTH[i] == 0. )
        {
            S11_SAMPLE_WEIGHT[i] = 0.;
            ART_ERRORHANDLING_WARNING(
                "zero sample width - this is almost certainly not intended, "
                "and should be fixed!");
        }
        else
        {
            S11_SAMPLE_WEIGHT[i] = uniform_channel_width / S11_SAMPLE_WIDTH[i];
        }

        S11_SAMPLE_BASIS[i].size  = 2;
        S11_SAMPLE_BASIS[i].scale =
            1.0 / (  S11_SAMPLE_BOUND[ i + 1 ]
                   - S11_SAMPLE_BOUND[ i     ] );

        S11_SAMPLE_BASIS[i].array = ALLOC_ARRAY( Pnt2D, 2 );
        S11_SAMPLE_BASIS[i].array[0] = PNT2D( S11_SAMPLE_BOUND[ i    ], 1.0 );
        S11_SAMPLE_BASIS[i].array[1] = PNT2D( S11_SAMPLE_BOUND[ i + 1], 1.0 );
    }

    S11_SHORTNAME_STRING = "s11e";
    S11_TYPENAME_STRING = "Spectrum 11";
    S11_DESCRIPTION_STRING =
        "UV + visible range, 11 spectral samples, 40nm spacing";

    S11_ZERO = s11_d_alloc_init( art_gv, 0.0 );
    S11_UNIT = s11_d_alloc_init( art_gv, 1.0 );
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    FREE_ARRAY(S11_SAMPLE_BOUND);
    FREE_ARRAY(S11_SAMPLE_CENTER);
    FREE_ARRAY(S11_SAMPLE_WIDTH);
    FREE_ARRAY(S11_SAMPLE_WIDTH_DIV_2);
    FREE_ARRAY(S11_SAMPLE_WEIGHT);

    for ( unsigned int i = 0; i < S11_CHANNELS; i++ )
        pss_freearray_s( art_gv, & S11_SAMPLE_BASIS[i] );

    FREE_ARRAY(S11_SAMPLE_BASIS);

    s11_free( art_gv, S11_ZERO );
    s11_free( art_gv, S11_UNIT );

    FREE(S11_GV);
)

#define _ISR_C(_s0)             (_s0).c
#define _ISR_CI(_s0,_i)         C11_CI(_ISR_C(_s0),(_i))

CANONICAL_IMPLEMENTATION_FOR_ISR( ArSpectrum11, arspectrum11, S11, s11, c11, s );

ADDITIONAL_IMPLEMENTATION_FOR_SPECTRAL_ISR( ArSpectrum11, S11, s11, c11, s );
ISR_VALUE_AT_WAVELENGTH_IMPLEMENTATION( ArSpectrum11, arspectrum11, S11, s11, c11, s );

#undef _ISR_CHANNELS
#undef _ISR_C
#undef _ISR_CI

/* ======================================================================== */
