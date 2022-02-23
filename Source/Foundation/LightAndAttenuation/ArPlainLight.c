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

#define ART_MODULE_NAME     ArPlainLight

#include "ArPlainLight.h"

#include "_ArLight_GV.h"
#include "ArLightAndAttenuationSubsystemManagement.h"
#include "LightImplementationMacros.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

SWITCHING_IMPLEMENTATION_FOR_LCT( arplainlight )

const char * arplainlight_lct_string(
        const ART_GV  * art_gv
        )
{
    (void) art_gv;
    
    return "plain light";
}

void arplainlight_s_rf_init_polarised_l(
        const ART_GV            * art_gv,
        const ArStokesVector    * s0,
        const ArReferenceFrame  * r0,
              ArPlainLight      * lr
        )
{
    (void) r0;
    
    arplainlight_s_init_unpolarised_l(
        art_gv,
        ARSV_I( *s0, 0 ),
        lr
        );
}

unsigned int arplainlight_l_polarised(
        const ART_GV        * art_gv,
        const ArPlainLight  * l0
        )
{
    (void) art_gv;
    (void) l0;
    
    return 0;
}

ArReferenceFrame const * arplainlight_l_refframe(
        const ART_GV        * art_gv,
        const ArPlainLight  * l0
        )
{
    (void) art_gv;
    (void) l0;
    
    return 0;
}

void arplainlight_l_to_sv(
        const ART_GV          * art_gv,
        const ArPlainLight    * l0,
              ArStokesVector  * sr
        )
{
    arplainlight_l_init_s(
        art_gv,
        l0,
        ARSV_I( *sr, 0 )
        );
}

void arplainlight_ld_realign_to_coaxial_refframe_sv(
        const ART_GV            * art_gv,
        const ArPlainLight      * l0,
        const double              d0,
        const ArReferenceFrame  * r0,
              ArStokesVector    * sr
        )
{
    (void) d0;
    (void) r0;
    
    arplainlight_l_to_sv(
        art_gv,
        l0,
        sr
        );
}

void arplainlight_realign_to_coaxial_refframe_l(
        const ART_GV            * art_gv,
        const ArReferenceFrame  * r0,
              ArPlainLight      * lr
        )
{
    (void) art_gv;
    (void) r0;
    (void) lr;
    
    //   This space intentionally left blank; this should really
    //   do nothing except leave 'lr' exactly as the way it is.
}

void arplainlight_l_realign_to_coaxial_refframe_l(
        const ART_GV            * art_gv,
        const ArPlainLight      * l0,
        const ArReferenceFrame  * r0,
              ArPlainLight      * lr
        )
{
    (void) r0;
    
    arplainlight_l_init_l(
        art_gv,
        l0,
        lr
        );
}

void arplainlight_dl_sloppy_add_l(
        const ART_GV        * art_gv,
        const double          d0,
        const ArPlainLight  * l0,
              ArPlainLight  * lr
        )
{
    (void) d0;
    
    arplainlight_l_add_l(
          art_gv,
          l0,
          lr
        );
}

void arplainlight_lld_sloppy_add_l(
        const ART_GV        * art_gv,
        const ArPlainLight  * l0,
        const ArPlainLight  * l1,
        const double          d0,
              ArPlainLight  * lr
        )
{
    (void) d0;
    
    arplainlight_ll_add_l(
          art_gv,
          l0,
          l1,
          lr
        );
}

void arplainlight_dld_mul_sloppy_add_l(
        const ART_GV        * art_gv,
        const double          d0,
        const ArPlainLight  * l0,
        const double          d1,
              ArPlainLight  * lr
        )
{
    (void) d1;
    
    arplainlight_dl_mul_add_l(
          art_gv,
          d0,
          l0,
          lr
        );
}

const ArSpectrum500  ** s500_ciexyz_primary(
        const ART_GV  * art_gv
        );

void arplainlight_ws_add_l(
        const ART_GV                         * art_gv,
        const ArPlainLightSample             * s0,
        const ArWavelength                   * w0,
        const ArSpectralSampleSplattingData  * sd,
              ArPlainLight                   * lr
        )
{
    ASSERT_VALID_WAVELENGTH( w0 )
    ASSERT_VALID_SPECTRUMTYPE( sps, ArSpectralSample, s0, s )
    ASSERT_VALID_SPECTRUM( lr )
    
    for ( unsigned int i = 0; i < HERO_SAMPLES_TO_SPLAT; i++ )
    {
        ASSERT_DOUBLE_LARGER_THAN(
            NANO_FROM_UNIT(ARWL_WI(*w0,i)),
            sd->s500_range_start
            )
        
        ASSERT_DOUBLE_LESS_THAN(
            NANO_FROM_UNIT(ARWL_WI(*w0,i)),
            sd->s500_range_end
            )
        
        unsigned int  s500_index =
            (unsigned int)(   NANO_FROM_UNIT(ARWL_WI(*w0,i))
                            - sd->s500_range_start );
        
        if ( art_foundation_isr(art_gv) == ardt_xyz )
        {
            for ( int j = 0; j < 3; j++ )
            {
                double  val  = arplainlight_li( art_gv, lr, j );
                
                double  cmf =
                    s500_si(
                          art_gv,
                        & ARCMF_CURVE_500( *DEFAULT_CMF, j ),
                          s500_index
                        );
                
                arplainlight_set_lid(
                      art_gv,
                      lr,
                      j,
                      val + SPS_CI(*s0, i) * cmf
                    );
            }
        }
        else
        {
            if ( sd->splatChannel0[s500_index] != INVALID_SPLAT_CHANNEL)
            {
                double  temp =
                    arplainlight_li(
                          art_gv,
                          lr,
                          sd->splatChannel0[s500_index]
                        );

                arplainlight_set_lid(
                      art_gv,
                      lr,
                      sd->splatChannel0[s500_index],
                      temp + SPS_CI(*s0, i) * sd->splatFactor0[s500_index]
                    );
            }

            if ( sd->splatChannel1[s500_index] != INVALID_SPLAT_CHANNEL)
            {
                double  temp =
                    arplainlight_li(
                          art_gv,
                          lr,
                          sd->splatChannel1[s500_index]
                        );

                arplainlight_set_lid(
                      art_gv,
                      lr,
                      sd->splatChannel1[s500_index],
                      temp + SPS_CI(*s0, i) * sd->splatFactor1[s500_index]
                    );
            }
        }
    }
}

void arplainlight_wsd_sloppy_add_l(
        const ART_GV                         * art_gv,
        const ArPlainLightSample             * h0,
        const ArWavelength                   * w0,
        const ArSpectralSampleSplattingData  * s0,
        const double                           d0,
              ArPlainLight                   * lr
        )
{
    (void) d0;
    
    //  For plain light, we ignore the angular tolerance
    //  specified in d0 - this is only checked for polarised light

    arplainlight_ws_add_l(
          art_gv,
          h0,
          w0,
          s0,
          lr
        );
}

void arplainlight_dws_mul_add_l(
        const ART_GV                         * art_gv,
        const double                           d0,
        const ArPlainLightSample             * h0,
        const ArWavelength                   * w0,
        const ArSpectralSampleSplattingData  * s0,
              ArPlainLight                   * lr
        )
{
    ArPlainLightSample  temp;
    
    arplainlightsample_dl_mul_l(
          art_gv,
          d0,
          h0,
        & temp
        );
    
    arplainlight_ws_add_l(
          art_gv,
        & temp,
          w0,
          s0,
          lr
        );
}

void arplainlight_dwsd_mul_sloppy_add_l(
        const ART_GV                         * art_gv,
        const double                           d0,
        const ArPlainLightSample             * h0,
        const ArWavelength                   * w0,
        const ArSpectralSampleSplattingData  * s0,
        const double                           d1,
              ArPlainLight                   * lr
        )
{
    (void) d1;
    
    //  For plain light, we ignore the angular tolerance
    //  specified in d1 - this is only checked for polarised light

    arplainlight_dws_mul_add_l(
          art_gv,
          d0,
          h0,
          w0,
          s0,
          lr
        );
}

unsigned int arplainlight_l_valid(
        const ART_GV        * art_gv,
        const ArPlainLight  * l0
        )
{
    if ( ! spc_s_valid( art_gv, l0 ) )
    {
        ART_ERRORHANDLING_WARNING( "colour data are invalid" );
        return 0;
    }

    const unsigned int  numberOfChannels = spc_channels( art_gv );

    unsigned int result = 1;

    for ( unsigned int i = 0; i < numberOfChannels; i++ )
    {
        if ( spc_si( art_gv, l0, i ) < 0.0 )
        {
            ART_ERRORHANDLING_WARNING(
                "colour channel %d is negative (%f)",
                i,
                spc_si( art_gv, l0, i )
                );
            result = 0;
        }
    }

    return result;
}

unsigned int arplainlight_ll_collinear(
        const ART_GV        * art_gv,
        const ArPlainLight  * l0,
        const ArPlainLight  * l1
        )
{
    (void) art_gv;
    (void) l0;
    (void) l1;
    
    return 1;
}

void arplainlight_l_debugprintf(
        const ART_GV        * art_gv,
        const ArPlainLight  * l0
        )
{
    printf("ArPlainLight: ");

    spc_s_debugprintf(
        art_gv,
        l0
        );
}

// ===========================================================================
