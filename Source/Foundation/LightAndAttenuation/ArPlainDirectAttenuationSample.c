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

#define ART_MODULE_NAME     ArPlainDirectAttenuationSample

#include "ArPlainDirectAttenuationSample.h"

#include "_ArDirectAttenuationSample_GV.h"
#include "ArLightAndAttenuationSubsystemManagement.h"
#include "AttenuationSampleImplementationMacros.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

SWITCHING_IMPLEMENTATION_FOR_SAMPLE_ACT( arplaindirectattenuationsample )

const char * arplaindirectattenuationsample_act_string(
        const ART_GV  * art_gv
        )
{
    (void) art_gv;
    
    return "plain attenuation";
}

ArPlainDirectAttenuationSample *
    arplaindirectattenuationsample_drr_alloc_init_depolarising(
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

ArPlainDirectAttenuationSample *
    arplaindirectattenuationsample_drr_alloc_init_nonpolarising(
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

ArPlainDirectAttenuationSample * arplaindirectattenuationsample_rr_alloc_init_none(
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

ArPlainDirectAttenuationSample * arplaindirectattenuationsample_rr_alloc_init_total(
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

void arplaindirectattenuationsample_drr_init_depolarising_a(
        const ART_GV                          * art_gv,
        const double                            d0,
        const ArReferenceFrame                * r0,
        const ArReferenceFrame                * r1,
              ArPlainDirectAttenuationSample  * ar
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

void arplaindirectattenuationsample_drr_init_nonpolarising_a(
        const ART_GV                          * art_gv,
        const double                            d0,
        const ArReferenceFrame                * r0,
        const ArReferenceFrame                * r1,
              ArPlainDirectAttenuationSample  * ar
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

void arplaindirectattenuationsample_srr_init_depolarising_a(
        const ART_GV                          * art_gv,
        const ArSpectralSample                * c0,
        const ArReferenceFrame                * r0,
        const ArReferenceFrame                * r1,
              ArPlainDirectAttenuationSample  * ar
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

void arplaindirectattenuationsample_srr_init_nonpolarising_a(
        const ART_GV                          * art_gv,
        const ArSpectralSample                * c0,
        const ArReferenceFrame                * r0,
        const ArReferenceFrame                * r1,
              ArPlainDirectAttenuationSample  * ar
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

void arplaindirectattenuationsample_mm_rf_init_polarising_a(
        const ART_GV                        * art_gv,
        const ArMuellerMatrixSample           * m0,
        const ArReferenceFrame              * r0,
              ArPlainDirectAttenuationSample  * ar
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

void arplaindirectattenuationsample_mm_rr_init_polarising_a(
        const ART_GV                          * art_gv,
        const ArMuellerMatrixSample           * m0,
        const ArReferenceFrame                * r0,
        const ArReferenceFrame                * r1,
              ArPlainDirectAttenuationSample  * ar
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

void arplaindirectattenuationsample_ddrr_init_linear_polariser_a(
        const ART_GV                          * art_gv,
        const double                            d0,
        const double                            d1,
        const ArReferenceFrame                * r0,
        const ArReferenceFrame                * r1,
              ArPlainDirectAttenuationSample  * ar
        )
{
    (void) art_gv;
    (void) d0;
    (void) d1;
    (void) r0;
    (void) r1;
    (void) d1;
    (void) ar;
    
    ART_ERRORHANDLING_FATAL_ERROR(
        "polarisation related functionality accessed for plain "
        "attenuation data"
        );
}

unsigned int arplaindirectattenuationsample_a_polarising(
        const ART_GV                          * art_gv,
        const ArPlainDirectAttenuationSample  * a0
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

ArReferenceFrame const * arplaindirectattenuationsample_a_entry_rf(
        const ART_GV                          * art_gv,
        const ArPlainDirectAttenuationSample  * a0
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

ArReferenceFrame const * arplaindirectattenuationsample_a_exit_rf(
        const ART_GV                          * art_gv,
        const ArPlainDirectAttenuationSample  * a0
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

void arplaindirectattenuationsample_a_to_mm(
        const ART_GV                          * art_gv,
        const ArPlainDirectAttenuationSample  * a0,
              ArMuellerMatrixSample           * mm
        )
{
    (void) art_gv;
    
    ASSERT_ALLOCATED_MUELLER_MATRIX_SAMPLE(mm)

    MMS_II( *mm, 0, 0 ) = *a0;
    MMS_II( *mm, 0, 1 ) = SPS( 0.0 );
    MMS_II( *mm, 0, 2 ) = SPS( 0.0 );
    MMS_II( *mm, 0, 3 ) = SPS( 0.0 );

    MMS_II( *mm, 1, 1 ) = SPS( 0.0 );
    MMS_II( *mm, 1, 1 ) = SPS( 0.0 );
    MMS_II( *mm, 1, 2 ) = SPS( 0.0 );
    MMS_II( *mm, 1, 3 ) = SPS( 0.0 );

    MMS_II( *mm, 2, 1 ) = SPS( 0.0 );
    MMS_II( *mm, 2, 1 ) = SPS( 0.0 );
    MMS_II( *mm, 2, 2 ) = SPS( 0.0 );
    MMS_II( *mm, 2, 3 ) = SPS( 0.0 );

    MMS_II( *mm, 3, 1 ) = SPS( 0.0 );
    MMS_II( *mm, 3, 1 ) = SPS( 0.0 );
    MMS_II( *mm, 3, 2 ) = SPS( 0.0 );
    MMS_II( *mm, 3, 3 ) = SPS( 0.0 );

    ASSERT_VALID_MUELLER_MATRIX_SAMPLE(mm)
}

void arplaindirectattenuationsample_a_realign_to_coaxial_exit_rf_mm(
        const ART_GV                          * art_gv,
        const ArPlainDirectAttenuationSample  * a0,
        const ArReferenceFrame                * r0,
              ArMuellerMatrixSample           * mm
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

void arplaindirectattenuationsample_realign_to_coaxial_exit_rf_a(
        const ART_GV                          * art_gv,
        const ArReferenceFrame                * r0,
              ArPlainDirectAttenuationSample  * ar
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

void arplaindirectattenuationsample_a_realign_to_coaxial_exit_rf_a(
        const ART_GV                          * art_gv,
        const ArPlainDirectAttenuationSample  * a0,
        const ArReferenceFrame                * r0,
              ArPlainDirectAttenuationSample  * ar
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

void arplaindirectattenuationsample_a_complement_from_one_a(
        const ART_GV                          * art_gv,
        const ArPlainDirectAttenuationSample  * a0,
              ArPlainDirectAttenuationSample  * ar
        )
{
    double  a0i = arplaindirectattenuationsample_ai( art_gv, a0, 0 );

    arplaindirectattenuationsample_set_aid(
        art_gv,
        ar,
        0,
        1.0 - a0i
        );
}

void arplaindirectattenuationsample_ia_singleband_complement_from_one_a(
        const ART_GV                          * art_gv,
        const unsigned int                      i0,
        const ArPlainDirectAttenuationSample  * a0,
              ArPlainDirectAttenuationSample  * ar
        )
{
    (void) art_gv;
    (void) i0;
    (void) a0;
    (void) ar;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

void arplaindirectattenuationsample_dia_singleband_mul_copy_a(
        const ART_GV                          * art_gv,
        const double                            d0,
        const unsigned int                      i0,
        const ArPlainDirectAttenuationSample  * a0,
              ArPlainDirectAttenuationSample  * ar
        )
{
    (void) art_gv;
    (void) d0;
    (void) i0;
    (void) a0;
    (void) ar;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

void arplaindirectattenuationsample_ax_mul_l(
        const ART_GV                          * art_gv,
        const ArPlainDirectAttenuationSample  * a0,
        const ArCrosstalkSample               * x0,
              ArPlainLightSample              * lr
        )
{
    int  s = 0;

    ArPlainLightSample  temp;

ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    int  cc = 1;

    for ( int i = 0; i < cc; i++ )
    {
        double  a0i = arplaindirectattenuationsample_ai( art_gv, a0, i );
        double  lri = arplainlightsample_li( art_gv, lr, i );

        double  temp_i = a0i * lri;

        for ( int j = 0; j < i; j++ )
            temp_i +=
                  arplainlightsample_li( art_gv, lr, j )
                * ARCROSSTALKSAMPLE_I( *x0, s + j );

        arplainlightsample_set_lid(
              art_gv,
            & temp,
              i,
              temp_i
            );

        s += i;
    }

    arplainlightsample_l_init_l(
          art_gv,
        & temp,
          lr
        );
}

void arplaindirectattenuationsample_axl_mul_l(
        const ART_GV                          * art_gv,
        const ArPlainDirectAttenuationSample  * a0,
        const ArCrosstalkSample               * x0,
        const ArPlainLightSample              * l0,
              ArPlainLightSample              * lr
        )
{
    (void) art_gv;
    (void) a0;
    (void) x0;
    (void) l0;
    (void) lr;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

void arplaindirectattenuationsample_ax_mul_i(
        const ART_GV                          * art_gv,
        const ArPlainDirectAttenuationSample  * a0,
        const ArCrosstalkSample               * x0,
              ArLightIntensitySample          * lr
        )
{
    (void) art_gv;
    (void) a0;
    (void) x0;
    (void) lr;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

void arplaindirectattenuationsample_axi_mul_i(
        const ART_GV                          * art_gv,
        const ArPlainDirectAttenuationSample  * a0,
        const ArCrosstalkSample               * x0,
        const ArLightIntensitySample          * l0,
              ArLightIntensitySample          * lr
        )
{
    (void) art_gv;
    (void) a0;
    (void) x0;
    (void) l0;
    (void) lr;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

unsigned int arplaindirectattenuationsample_a_valid(
        const ART_GV                          * art_gv,
        const ArPlainDirectAttenuationSample  * a0
        )
{
    if ( ! sps_s_valid( art_gv, a0 ) )
    {
        ART_ERRORHANDLING_WARNING( "invalid value/intensity" );
        return 0;
    }

    unsigned int result = 1;

    if ( sps_si( art_gv, a0, 0 ) < -1E-8 )
    {
        ART_ERRORHANDLING_WARNING(
            "attenuation sample %f < 0.0",
            sps_si( art_gv, a0, 0 )
            );

        result = 0;
    }

    return result;
}

void arplaindirectattenuationsample_a_debugprintf(
        const ART_GV                          * art_gv,
        const ArPlainDirectAttenuationSample  * a0
        )
{
    printf("ArPlainDirectAttenuationSample: ");

    sps_s_debugprintf(
        art_gv,
        a0
        );
}

// ===========================================================================
