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

#define ART_MODULE_NAME     ArPlainLightSample

#include "ArPlainLightSample.h"

#include "_ArLightSample_GV.h"
#include "ArLightAndAttenuationSubsystemManagement.h"
#include "LightSampleImplementationMacros.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

SWITCHING_IMPLEMENTATION_FOR_SAMPLE_LCT( arplainlightsample )

const char * arplainlightsample_lct_string(
        const ART_GV                * art_gv
        )
{
    (void) art_gv;
    
    return " plain light";
}

void arplainlightsample_s_rf_init_polarised_l(
        const ART_GV                * art_gv,
        const ArStokesVectorSample    * s0,
        const ArReferenceFrame      * r0,
              ArPlainLightSample      * lr
        )
{
    (void) r0;
    
    arplainlightsample_s_init_unpolarised_l(
          art_gv,
        & ARSVS_I( *s0, 0 ),
          lr
        );
}

unsigned int arplainlightsample_l_polarised(
        const ART_GV                * art_gv,
        const ArPlainLightSample      * l0
        )
{
    (void) art_gv;
    (void) l0;
    
    return 0;
}

ArReferenceFrame const * arplainlightsample_l_refframe(
        const ART_GV                * art_gv,
        const ArPlainLightSample      * l0
        )
{
    (void) art_gv;
    (void) l0;
    
    return 0;
}

void arplainlightsample_l_to_sv(
        const ART_GV                * art_gv,
        const ArPlainLightSample      * l0,
              ArStokesVectorSample    * sr
        )
{
    arplainlightsample_l_init_s(
          art_gv,
          l0,
        & ARSVS_I( *sr, 0 )
        );
}

void arplainlightsample_ld_realign_to_coaxial_refframe_sv(
        const ART_GV                * art_gv,
        const ArPlainLightSample      * l0,
        const double                  d0,
        const ArReferenceFrame      * r0,
              ArStokesVectorSample    * sr
        )
{
    (void) d0;
    (void) r0;
    
    arplainlightsample_l_to_sv(
        art_gv,
        l0,
        sr
        );
}

void arplainlightsample_realign_to_coaxial_refframe_l(
        const ART_GV                * art_gv,
        const ArReferenceFrame      * r0,
              ArPlainLightSample      * lr
        )
{
    (void) art_gv;
    (void) r0;
    (void) lr;
    
    //   This space intentionally left blank; this should really
    //   do nothing except leave 'lr' exactly as the way it is.
}

void arplainlightsample_l_realign_to_coaxial_refframe_l(
        const ART_GV                * art_gv,
        const ArPlainLightSample      * l0,
        const ArReferenceFrame      * r0,
              ArPlainLightSample      * lr
        )
{
    (void) r0;
    
    arplainlightsample_l_init_l(
        art_gv,
        l0,
        lr
        );
}

void arplainlightsample_dl_sloppy_add_l(
        const ART_GV                * art_gv,
        const double                  d0,
        const ArPlainLightSample      * l0,
              ArPlainLightSample      * lr
        )
{
    (void) d0;
    
    arplainlightsample_l_add_l(
          art_gv,
          l0,
          lr
        );
}

void arplainlightsample_lld_sloppy_add_l(
        const ART_GV                * art_gv,
        const ArPlainLightSample      * l0,
        const ArPlainLightSample      * l1,
        const double                  d0,
              ArPlainLightSample      * lr
        )
{
    (void) d0;
    
    arplainlightsample_ll_add_l(
          art_gv,
          l0,
          l1,
          lr
        );
}

void arplainlightsample_dld_mul_sloppy_add_l(
        const ART_GV                * art_gv,
        const double                  d0,
        const ArPlainLightSample    * l0,
        const double                  d1,
              ArPlainLightSample    * lr
        )
{
    (void) d1;
    
    arplainlightsample_dl_mul_add_l(
          art_gv,
          d0,
          l0,
          lr
        );
}

unsigned int arplainlightsample_l_valid(
        const ART_GV                * art_gv,
        const ArPlainLightSample      * l0
        )
{
    if ( ! sps_s_valid( art_gv, l0 ) )
    {
        ART_ERRORHANDLING_WARNING( "value/intensity data are invalid" );
        return 0;
    }

    if ( sps_si( art_gv, l0, 0 ) < 0.0 )
    {
        ART_ERRORHANDLING_WARNING(
            "value/intensity channel is negative (%f)",
            sps_si( art_gv, l0, 0 )
            );

        return 0;
    }

    return 1;
}

unsigned int arplainlightsample_ll_collinear(
        const ART_GV                * art_gv,
        const ArPlainLightSample      * l0,
        const ArPlainLightSample      * l1
        )
{
    (void) art_gv;
    (void) l0;
    (void) l1;
    
    return 1;
}

void arplainlightsample_l_debugprintf(
        const ART_GV                * art_gv,
        const ArPlainLightSample      * l0
        )
{
    printf("ArPlainLightSample: ");

    sps_s_debugprintf(
        art_gv,
        l0
        );
}

// ===========================================================================
