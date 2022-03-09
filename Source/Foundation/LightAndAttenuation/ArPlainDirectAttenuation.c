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

#define ART_MODULE_NAME     ArPlainDirectAttenuation

#include "ArPlainDirectAttenuation.h"

#include "_ArDirectAttenuation_GV.h"
#include "ArLightAndAttenuationSubsystemManagement.h"
#include "AttenuationImplementationMacros.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

SWITCHING_IMPLEMENTATION_FOR_ACT( arplaindirectattenuation )

const char * arplaindirectattenuation_act_string(
        const ART_GV  * art_gv
        )
{
    (void) art_gv;
    
    return "plain attenuation";
}

ArPlainDirectAttenuation *
    arplaindirectattenuation_drr_alloc_init_depolarising(
        const ART_GV              * art_gv,
        const double                d0,
        const ArReferenceFrame    * r0,
        const ArReferenceFrame    * r1
        )
{
    (void) art_gv;
    (void) d0;
    (void) r0;
    (void) r1;
    
    ART_ERRORHANDLING_FATAL_ERROR(
        "polarisation related functionality accessed for plain "
        "attenuation data"
        );

    return 0;
}

ArPlainDirectAttenuation *
    arplaindirectattenuation_drr_alloc_init_nonpolarising(
        const ART_GV              * art_gv,
        const double                d0,
        const ArReferenceFrame    * r0,
        const ArReferenceFrame    * r1
        )
{
    (void) art_gv;
    (void) d0;
    (void) r0;
    (void) r1;
    
    ART_ERRORHANDLING_FATAL_ERROR(
        "polarisation related functionality accessed for plain "
        "attenuation data"
        );

    return 0;
}

ArPlainDirectAttenuation * arplaindirectattenuation_rr_alloc_init_none(
        const ART_GV              * art_gv,
        const ArReferenceFrame    * r0,
        const ArReferenceFrame    * r1
        )
{
    (void) art_gv;
    (void) r0;
    (void) r1;
    
    ART_ERRORHANDLING_FATAL_ERROR(
        "polarisation related functionality accessed for plain "
        "attenuation data"
        );

    return 0;
}

ArPlainDirectAttenuation * arplaindirectattenuation_rr_alloc_init_total(
        const ART_GV              * art_gv,
        const ArReferenceFrame    * r0,
        const ArReferenceFrame    * r1
        )
{
    (void) art_gv;
    (void) r0;
    (void) r1;
    
    ART_ERRORHANDLING_FATAL_ERROR(
        "polarisation related functionality accessed for plain "
        "attenuation data"
        );

    return 0;
}

void arplaindirectattenuation_drr_init_depolarising_a(
        const ART_GV              * art_gv,
        const double                d0,
        const ArReferenceFrame    * r0,
        const ArReferenceFrame    * r1,
              ArPlainDirectAttenuation  * ar
        )
{
    (void) art_gv;
    (void) d0;
    (void) r0;
    (void) r1;
    (void) ar;
    
    ART_ERRORHANDLING_FATAL_ERROR(
        "polarisation related functionality accessed for plain "
        "attenuation data"
        );
}

void arplaindirectattenuation_drr_init_nonpolarising_a(
        const ART_GV              * art_gv,
        const double                d0,
        const ArReferenceFrame    * r0,
        const ArReferenceFrame    * r1,
              ArPlainDirectAttenuation  * ar
        )
{
    (void) art_gv;
    (void) d0;
    (void) r0;
    (void) r1;
    (void) ar;
    
    ART_ERRORHANDLING_FATAL_ERROR(
        "polarisation related functionality accessed for plain "
        "attenuation data"
        );
}

void arplaindirectattenuation_srr_init_depolarising_a(
        const ART_GV              * art_gv,
        const ArSpectrum            * c0,
        const ArReferenceFrame    * r0,
        const ArReferenceFrame    * r1,
              ArPlainDirectAttenuation  * ar
        )
{
    (void) art_gv;
    (void) c0;
    (void) r0;
    (void) r1;
    (void) ar;
    
    ART_ERRORHANDLING_FATAL_ERROR(
        "polarisation related functionality accessed for plain "
        "attenuation data"
        );
}

void arplaindirectattenuation_srr_init_nonpolarising_a(
        const ART_GV              * art_gv,
        const ArSpectrum            * c0,
        const ArReferenceFrame    * r0,
        const ArReferenceFrame    * r1,
              ArPlainDirectAttenuation  * ar
        )
{
    (void) art_gv;
    (void) c0;
    (void) r0;
    (void) r1;
    (void) ar;
    
    ART_ERRORHANDLING_FATAL_ERROR(
        "polarisation related functionality accessed for plain "
        "attenuation data"
        );
}

void arplaindirectattenuation_mm_rf_init_polarising_a(
        const ART_GV              * art_gv,
        const ArMuellerMatrix     * m0,
        const ArReferenceFrame    * r0,
              ArPlainDirectAttenuation  * ar
        )
{
    (void) art_gv;
    (void) m0;
    (void) r0;
    (void) ar;
    
    ART_ERRORHANDLING_FATAL_ERROR(
        "polarisation related functionality accessed for plain "
        "attenuation data"
        );
}

void arplaindirectattenuation_mm_rr_init_polarising_a(
        const ART_GV              * art_gv,
        const ArMuellerMatrix     * m0,
        const ArReferenceFrame    * r0,
        const ArReferenceFrame    * r1,
              ArPlainDirectAttenuation  * ar
        )
{
    (void) art_gv;
    (void) m0;
    (void) r0;
    (void) r1;
    (void) ar;
    
    ART_ERRORHANDLING_FATAL_ERROR(
        "polarisation related functionality accessed for plain "
        "attenuation data"
        );
}

void arplaindirectattenuation_ddrr_init_linear_polariser_a(
        const ART_GV              * art_gv,
        const double                d0,
        const double                d1,
        const ArReferenceFrame    * r0,
        const ArReferenceFrame    * r1,
              ArPlainDirectAttenuation  * ar
        )
{
    (void) art_gv;
    (void) d0;
    (void) d1;
    (void) r0;
    (void) r1;
    (void) ar;
    
    ART_ERRORHANDLING_FATAL_ERROR(
        "polarisation related functionality accessed for plain "
        "attenuation data"
        );
}

unsigned int arplaindirectattenuation_a_polarising(
        const ART_GV              * art_gv,
        const ArPlainDirectAttenuation  * a0
        )
{
    (void) art_gv;
    (void) a0;
    
    ART_ERRORHANDLING_FATAL_ERROR(
        "polarisation related functionality accessed for plain "
        "attenuation data"
        );

    return 0;
}

ArReferenceFrame const * arplaindirectattenuation_a_entry_rf(
        const ART_GV              * art_gv,
        const ArPlainDirectAttenuation  * a0
        )
{
    (void) art_gv;
    (void) a0;
    
    ART_ERRORHANDLING_FATAL_ERROR(
        "polarisation related functionality accessed for plain "
        "attenuation data"
        );

    return 0;
}

ArReferenceFrame const * arplaindirectattenuation_a_exit_rf(
        const ART_GV              * art_gv,
        const ArPlainDirectAttenuation  * a0
        )
{
    (void) art_gv;
    (void) a0;
    
    ART_ERRORHANDLING_FATAL_ERROR(
        "polarisation related functionality accessed for plain "
        "attenuation data"
        );

    return 0;
}

void arplaindirectattenuation_a_to_mm(
        const ART_GV                    * art_gv,
        const ArPlainDirectAttenuation  * a0,
              ArMuellerMatrix           * mm
        )
{
    (void) art_gv;
    (void) a0;
    (void) mm;
    
    ART_ERRORHANDLING_FATAL_ERROR(
        "polarisation related functionality accessed for plain "
        "attenuation data"
        );
}

void arplaindirectattenuation_a_realign_to_coaxial_exit_rf_mm(
        const ART_GV                    * art_gv,
        const ArPlainDirectAttenuation  * a0,
        const ArReferenceFrame          * r0,
              ArMuellerMatrix           * mm
        )
{
    (void) art_gv;
    (void) a0;
    (void) r0;
    (void) mm;
    
    ART_ERRORHANDLING_FATAL_ERROR(
        "polarisation related functionality accessed for plain "
        "attenuation data"
        );
}

void arplaindirectattenuation_realign_to_coaxial_exit_rf_a(
        const ART_GV              * art_gv,
        const ArReferenceFrame    * r0,
              ArPlainDirectAttenuation  * ar
        )
{
    (void) art_gv;
    (void) r0;
    (void) ar;
    
    ART_ERRORHANDLING_FATAL_ERROR(
        "polarisation related functionality accessed for plain "
        "attenuation data"
        );
}

void arplaindirectattenuation_a_realign_to_coaxial_exit_rf_a(
        const ART_GV              * art_gv,
        const ArPlainDirectAttenuation  * a0,
        const ArReferenceFrame    * r0,
              ArPlainDirectAttenuation  * ar
        )
{
    (void) art_gv;
    (void) a0;
    (void) r0;
    (void) ar;
    
    ART_ERRORHANDLING_FATAL_ERROR(
        "polarisation related functionality accessed for plain "
        "attenuation data"
        );
}

void arplaindirectattenuation_a_complement_from_one_a(
        const ART_GV              * art_gv,
        const ArPlainDirectAttenuation  * a0,
              ArPlainDirectAttenuation  * ar
        )
{
    int  cc = spc_channels( art_gv );

    for ( int i = 0; i < cc; i++ )
    {
        double  a0i = arplaindirectattenuation_ai( art_gv, a0, i );

        arplainlight_set_lid(
            art_gv,
            ar,
            i,
            1.0 - a0i
            );
    }
}

void arplaindirectattenuation_ia_singleband_complement_from_one_a(
        const ART_GV              * art_gv,
        const unsigned int          i0,
        const ArPlainDirectAttenuation  * a0,
              ArPlainDirectAttenuation  * ar
        )
{
    (void) art_gv;
    (void) i0;
    (void) a0;
    (void) ar;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

void arplaindirectattenuation_dia_singleband_mul_copy_a(
        const ART_GV              * art_gv,
        const double                d0,
        const unsigned int          i0,
        const ArPlainDirectAttenuation  * a0,
              ArPlainDirectAttenuation  * ar
        )
{
    (void) art_gv;
    (void) d0;
    (void) i0;
    (void) a0;
    (void) ar;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

void arplaindirectattenuation_ax_mul_l(
        const ART_GV              * art_gv,
        const ArPlainDirectAttenuation  * a0,
        const ArCrosstalk         * x0,
              ArPlainLight        * lr
        )
{
    int  i, j, s = 0;

    ArPlainLight  * temp = arplainlight_alloc( art_gv );

    int  cc = spc_channels( art_gv );

    for ( i = 0; i < cc; i++ )
    {
        double  a0i = arplaindirectattenuation_ai( art_gv, a0, i );
        double  lri = arplainlight_li( art_gv, lr, i );

        double  temp_i = a0i * lri;

        for ( j = 0; j < i; j++ )
            temp_i +=
                  arplainlight_li( art_gv, lr, j )
                * ARCROSSTALK_I( *x0, s + j );

        arplainlight_set_lid(
            art_gv,
            temp,
            i,
            temp_i
            );

        s += i;
    }

    arplainlight_l_init_l(
          art_gv,
          temp,
          lr
        );

    arplainlight_free( art_gv, temp );
}

void arplaindirectattenuation_axl_mul_l(
        const ART_GV              * art_gv,
        const ArPlainDirectAttenuation  * a0,
        const ArCrosstalk         * x0,
        const ArPlainLight        * l0,
              ArPlainLight        * lr
        )
{
    (void) art_gv;
    (void) a0;
    (void) x0;
    (void) l0;
    (void) lr;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

void arplaindirectattenuation_ax_mul_i(
        const ART_GV              * art_gv,
        const ArPlainDirectAttenuation  * a0,
        const ArCrosstalk         * x0,
              ArLightIntensity    * lr
        )
{
    (void) art_gv;
    (void) a0;
    (void) x0;
    (void) lr;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

void arplaindirectattenuation_axi_mul_i(
        const ART_GV              * art_gv,
        const ArPlainDirectAttenuation  * a0,
        const ArCrosstalk         * x0,
        const ArLightIntensity    * l0,
              ArLightIntensity    * lr
        )
{
    (void) art_gv;
    (void) a0;
    (void) x0;
    (void) l0;
    (void) lr;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

unsigned int arplaindirectattenuation_a_valid(
        const ART_GV                    * art_gv,
        const ArPlainDirectAttenuation  * a0
        )
{
    if ( ! spc_s_valid( art_gv, a0 ) )
    {
        ART_ERRORHANDLING_WARNING( "invalid colour" );
        return 0;
    }

    const unsigned int  numberOfChannels = spc_channels( art_gv );

    unsigned int result = 1;

    for ( unsigned int i = 0; i < numberOfChannels; i++ )
    {
        if ( spc_si( art_gv, a0, i ) < -1E-8 )
        {
            ART_ERRORHANDLING_WARNING(
                "channel %d of %d: %.8f < 0.0",
                i,
                numberOfChannels,
                spc_si( art_gv, a0, i )
                );

            result = 0;
        }
    }

    return result;
}

void arplaindirectattenuation_a_debugprintf(
        const ART_GV              * art_gv,
        const ArPlainDirectAttenuation  * a0
        )
{
    printf("ArPlainDirectAttenuation: ");

    spc_s_debugprintf(
        art_gv,
        a0
        );
}

// ===========================================================================
