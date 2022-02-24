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

#define ART_MODULE_NAME     ArAttenuationSample

#include "ArAttenuationSample.h"
#include "ArPlainDirectAttenuationSample.h"
#include "ArMMDirectAttenuationSample.h"
#include "FoundationAssertionMacros.h"

#define  YES  1
#define  NO   0

typedef struct ArAttenuationSample_GV
{
    ArAttenuationSample  * free_transmission;
    ArAttenuationSample  * total_blocker;
}
ArAttenuationSample_GV;

#define ARATTENUATIONSAMPLE_GV                     art_gv->arattenuationsample_gv
#define ARATTENUATIONSAMPLE_FREE_TRANSMISSION_GV   ARATTENUATIONSAMPLE_GV->free_transmission
#define ARATTENUATIONSAMPLE_TOTAL_BLOCKER_GV       ARATTENUATIONSAMPLE_GV->total_blocker

ART_MODULE_INITIALISATION_FUNCTION
(
    ARATTENUATIONSAMPLE_GV = ALLOC(ArAttenuationSample_GV);

    ARATTENUATIONSAMPLE_FREE_TRANSMISSION_GV = NULL;
    ARATTENUATIONSAMPLE_TOTAL_BLOCKER_GV     = NULL;
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    if ( ! LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE )
    {
        if ( ARATTENUATIONSAMPLE_FREE_TRANSMISSION_GV )
            arattenuationsample_free( art_gv, ARATTENUATIONSAMPLE_FREE_TRANSMISSION_GV );
        if ( ARATTENUATIONSAMPLE_TOTAL_BLOCKER_GV )
            arattenuationsample_free( art_gv, ARATTENUATIONSAMPLE_TOTAL_BLOCKER_GV );
    }

    FREE( ARATTENUATIONSAMPLE_GV );
)

ART_LIGHT_AND_ATTENUATION_MODULE_INITIALISATION_FUNCTION
(
    if ( ! LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE )
    {
        ARATTENUATIONSAMPLE_FREE_TRANSMISSION_GV =
            arattenuationsample_d_alloc_init(
                art_gv,
                1.0
                );

        ARATTENUATIONSAMPLE_TOTAL_BLOCKER_GV =
            arattenuationsample_d_alloc_init(
                art_gv,
                0.0
                );
    }
    else
    {
        //   Just to make sure...

        ARATTENUATIONSAMPLE_FREE_TRANSMISSION_GV = NULL;
        ARATTENUATIONSAMPLE_TOTAL_BLOCKER_GV     = NULL;
    }
)

ART_LIGHT_AND_ATTENUATION_MODULE_SHUTDOWN_FUNCTION
(
    if ( ! LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE )
    {
        if ( ARATTENUATIONSAMPLE_FREE_TRANSMISSION_GV )
            arattenuationsample_free( art_gv, ARATTENUATIONSAMPLE_FREE_TRANSMISSION_GV );
        if ( ARATTENUATIONSAMPLE_TOTAL_BLOCKER_GV )
            arattenuationsample_free( art_gv, ARATTENUATIONSAMPLE_TOTAL_BLOCKER_GV );
    }
)


ArAttenuationSample * arattenuationsample_alloc(
        const ART_GV  * art_gv
        )
{
    ArAttenuationSample  * ar = ALLOC( ArAttenuationSample );

    ar->next = NULL;

    ARATTENUATIONSAMPLE_CROSSTALK( ar ) =
        arcrosstalksample_alloc(
            art_gv
            );

    ARATTENUATIONSAMPLE_MAIN_DIAGONAL( ar ) =
        ardirectattenuationsample_alloc(
            art_gv
            );

    ARATTENUATIONSAMPLE_INITIALISED( ar ) = NO;

    ASSERT_ALLOCATED_ATTENUATION_SAMPLE( ar )

    return ar;
}

void arattenuationsample_free(
        const ART_GV             * art_gv,
              ArAttenuationSample  * ar
        )
{
    ASSERT_ALLOCATED_ATTENUATION_SAMPLE( ar )

    if ( ARATTENUATIONSAMPLE_CROSSTALK( ar ) )
        arcrosstalksample_free(
            art_gv,
            ARATTENUATIONSAMPLE_CROSSTALK( ar )
            );

    ardirectattenuationsample_free(
        art_gv,
        ARATTENUATIONSAMPLE_MAIN_DIAGONAL( ar )
        );

    FREE( ar );
}

ArAttenuationSample * arattenuationsample_d_alloc_init_depolarising(
        const ART_GV  * art_gv,
        const double    d0
        )
{
    ASSERT_LIGHT_SUBSYSTEM_NOT_IN_POLARISATION_MODE

    ASSERT_NONNEGATIVE_DOUBLE( d0 )

    ArAttenuationSample  * ar = arattenuationsample_alloc( art_gv );

    ASSERT_ALLOCATED_ATTENUATION_SAMPLE( ar )

    ARATTENUATIONSAMPLE_FLUORESCENT( ar ) = NO;

    ardirectattenuationsample_d_init_a(
        art_gv,
        d0,
        ARATTENUATIONSAMPLE_MAIN_DIAGONAL( ar )
        );

    ARATTENUATIONSAMPLE_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION_SAMPLE( ar )

    return ar;
}

ArAttenuationSample * arattenuationsample_d_alloc_init(
        const ART_GV  * art_gv,
        const double    d0
        )
{
    ASSERT_LIGHT_SUBSYSTEM_NOT_IN_POLARISATION_MODE

    ASSERT_NONNEGATIVE_DOUBLE( d0 )

    ArAttenuationSample  * ar = arattenuationsample_alloc( art_gv );

    ASSERT_ALLOCATED_ATTENUATION_SAMPLE( ar )

    ARATTENUATIONSAMPLE_FLUORESCENT( ar ) = NO;

    ardirectattenuationsample_d_init_a(
        art_gv,
        d0,
        ARATTENUATIONSAMPLE_MAIN_DIAGONAL( ar )
        );

    ARATTENUATIONSAMPLE_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION_SAMPLE( ar )

    return ar;
}

ArAttenuationSample * arattenuationsample_a_alloc_init(
        const ART_GV             * art_gv,
        const ArAttenuationSample  * a0
        )
{
    ASSERT_VALID_ATTENUATION_SAMPLE( a0 )

    ArAttenuationSample  * ar = arattenuationsample_alloc( art_gv );

    ASSERT_ALLOCATED_ATTENUATION_SAMPLE( ar )

    arattenuationsample_a_init_a(
        art_gv,
        a0,
        ar
        );

    ARATTENUATIONSAMPLE_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION_SAMPLE( ar )
    ASSERT_IDENTICAL_SAMPLE_REFERENCE_FRAMES_AA( a0, ar )

    return ar;
}

ArAttenuationSample const * arattenuationsample_total(
        const ART_GV  * art_gv
        )
{
    ASSERT_LIGHT_SUBSYSTEM_NOT_IN_POLARISATION_MODE

    ASSERT_VALID_ATTENUATION_SAMPLE( ARATTENUATIONSAMPLE_TOTAL_BLOCKER_GV )

    return
        ARATTENUATIONSAMPLE_TOTAL_BLOCKER_GV;
}

ArAttenuationSample const * arattenuationsample_none(
        const ART_GV  * art_gv
        )
{
    ASSERT_LIGHT_SUBSYSTEM_NOT_IN_POLARISATION_MODE

    ASSERT_VALID_ATTENUATION_SAMPLE( ARATTENUATIONSAMPLE_FREE_TRANSMISSION_GV )

    return
        ARATTENUATIONSAMPLE_FREE_TRANSMISSION_GV;
}

void arattenuationsample_d_init_a(
        const ART_GV             * art_gv,
        const double               d0,
              ArAttenuationSample  * ar
        )
{
    ASSERT_LIGHT_SUBSYSTEM_NOT_IN_POLARISATION_MODE

    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_ALLOCATED_ATTENUATION_SAMPLE( ar )

    ARATTENUATIONSAMPLE_FLUORESCENT( ar ) = NO;

    ardirectattenuationsample_d_init_a(
        art_gv,
        d0,
        ARATTENUATIONSAMPLE_MAIN_DIAGONAL( ar )
        );

    ARATTENUATIONSAMPLE_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION_SAMPLE( ar )
}

void arattenuationsample_s_init_a(
        const ART_GV             * art_gv,
        const ArSpectralSample     * c0,
              ArAttenuationSample  * ar
        )
{
    ASSERT_LIGHT_SUBSYSTEM_NOT_IN_POLARISATION_MODE

    ASSERT_VALID_SPECTRAL_SAMPLE( c0 )
    ASSERT_ALLOCATED_ATTENUATION_SAMPLE( ar )

    ARATTENUATIONSAMPLE_FLUORESCENT( ar ) = NO;

    ardirectattenuationsample_s_init_a(
        art_gv,
        c0,
        ARATTENUATIONSAMPLE_MAIN_DIAGONAL( ar )
        );

    ARATTENUATIONSAMPLE_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION_SAMPLE( ar )
}

void arattenuationsample_sxrr_init_depolarising_a(
        const ART_GV                * art_gv,
        const ArSpectralSample        * c0,
        const ArCrosstalkSample       * x0,
        const ArReferenceFrame      * r0,
        const ArReferenceFrame      * r1,
              ArAttenuationSample     * ar
        )
{
    ASSERT_LIGHT_SUBSYSTEM_IN_POLARISATION_MODE

    ASSERT_VALID_SPECTRAL_SAMPLE( c0 )
    ASSERT_VALID_CROSSTALK_SAMPLE( x0 )
    ASSERT_VALID_REFERENCE_FRAME( r0 )
    ASSERT_VALID_REFERENCE_FRAME( r1 )
    ASSERT_ALLOCATED_ATTENUATION_SAMPLE( ar )

    ARATTENUATIONSAMPLE_FLUORESCENT( ar ) = YES;

    arcrosstalksample_x_init_x(
        art_gv,
        x0,
        ARATTENUATIONSAMPLE_CROSSTALK( ar )
        );

    ardirectattenuationsample_srr_init_depolarising_a(
        art_gv,
        c0,
        r0,
        r1,
        ARATTENUATIONSAMPLE_MAIN_DIAGONAL( ar )
        );

    ARATTENUATIONSAMPLE_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION_SAMPLE( ar )
}

void arattenuationsample_sxrr_init_nonpolarising_a(
        const ART_GV                * art_gv,
        const ArSpectralSample        * c0,
        const ArCrosstalkSample       * x0,
        const ArReferenceFrame      * r0,
        const ArReferenceFrame      * r1,
              ArAttenuationSample     * ar
        )
{
    ASSERT_LIGHT_SUBSYSTEM_IN_POLARISATION_MODE

    ASSERT_VALID_SPECTRAL_SAMPLE( c0 )
    ASSERT_VALID_CROSSTALK_SAMPLE( x0 )
    ASSERT_VALID_REFERENCE_FRAME( r0 )
    ASSERT_VALID_REFERENCE_FRAME( r1 )
    ASSERT_ALLOCATED_ATTENUATION_SAMPLE( ar )

    ARATTENUATIONSAMPLE_FLUORESCENT( ar ) = YES;

    arcrosstalksample_x_init_x(
        art_gv,
        x0,
        ARATTENUATIONSAMPLE_CROSSTALK( ar )
        );

    ardirectattenuationsample_srr_init_nonpolarising_a(
        art_gv,
        c0,
        r0,
        r1,
        ARATTENUATIONSAMPLE_MAIN_DIAGONAL( ar )
        );

    ARATTENUATIONSAMPLE_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION_SAMPLE( ar )
}

void arattenuationsample_srr_init_depolarising_a(
        const ART_GV               * art_gv,
        const ArSpectralSample     * c0,
        const ArReferenceFrame     * r0,
        const ArReferenceFrame     * r1,
              ArAttenuationSample  * ar
        )
{
    ASSERT_LIGHT_SUBSYSTEM_IN_POLARISATION_MODE

    ASSERT_VALID_SPECTRAL_SAMPLE( c0 )
    ASSERT_VALID_REFERENCE_FRAME( r0 )
    ASSERT_VALID_REFERENCE_FRAME( r1 )
    ASSERT_ALLOCATED_ATTENUATION_SAMPLE( ar )

    ARATTENUATIONSAMPLE_FLUORESCENT( ar ) = NO;

    ardirectattenuationsample_srr_init_depolarising_a(
        art_gv,
        c0,
        r0,
        r1,
        ARATTENUATIONSAMPLE_MAIN_DIAGONAL( ar )
        );

    ARATTENUATIONSAMPLE_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION_SAMPLE( ar )
}

void arattenuationsample_srr_init_nonpolarising_a(
        const ART_GV                * art_gv,
        const ArSpectralSample        * c0,
        const ArReferenceFrame      * r0,
        const ArReferenceFrame      * r1,
              ArAttenuationSample     * ar
        )
{
    ASSERT_LIGHT_SUBSYSTEM_IN_POLARISATION_MODE

    ASSERT_VALID_SPECTRAL_SAMPLE( c0 )
    ASSERT_VALID_REFERENCE_FRAME( r0 )
    ASSERT_VALID_REFERENCE_FRAME( r1 )
    ASSERT_ALLOCATED_ATTENUATION_SAMPLE( ar )

    ARATTENUATIONSAMPLE_FLUORESCENT( ar ) = NO;

    ardirectattenuationsample_srr_init_nonpolarising_a(
        art_gv,
        c0,
        r0,
        r1,
        ARATTENUATIONSAMPLE_MAIN_DIAGONAL( ar )
        );

    ARATTENUATIONSAMPLE_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION_SAMPLE( ar )
}

void arattenuationsample_drr_init_depolarising_a(
        const ART_GV                * art_gv,
        const double                  d0,
        const ArReferenceFrame      * r0,
        const ArReferenceFrame      * r1,
              ArAttenuationSample     * ar
        )
{
    ASSERT_LIGHT_SUBSYSTEM_IN_POLARISATION_MODE

    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_VALID_REFERENCE_FRAME( r0 )
    ASSERT_VALID_REFERENCE_FRAME( r1 )
    ASSERT_ALLOCATED_ATTENUATION_SAMPLE( ar )

    ARATTENUATIONSAMPLE_FLUORESCENT( ar ) = NO;

    ardirectattenuationsample_drr_init_depolarising_a(
        art_gv,
        d0,
        r0,
        r1,
        ARATTENUATIONSAMPLE_MAIN_DIAGONAL( ar )
        );

    ARATTENUATIONSAMPLE_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION_SAMPLE( ar )
}

void arattenuationsample_drr_init_nonpolarising_a(
        const ART_GV                * art_gv,
        const double                  d0,
        const ArReferenceFrame      * r0,
        const ArReferenceFrame      * r1,
              ArAttenuationSample     * ar
        )
{
    ASSERT_LIGHT_SUBSYSTEM_IN_POLARISATION_MODE

    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_VALID_REFERENCE_FRAME( r0 )
    ASSERT_VALID_REFERENCE_FRAME( r1 )
    ASSERT_ALLOCATED_ATTENUATION_SAMPLE( ar )

    ARATTENUATIONSAMPLE_FLUORESCENT( ar ) = NO;

    ardirectattenuationsample_drr_init_nonpolarising_a(
        art_gv,
        d0,
        r0,
        r1,
        ARATTENUATIONSAMPLE_MAIN_DIAGONAL( ar )
        );

    ARATTENUATIONSAMPLE_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION_SAMPLE( ar )
}

void arattenuationsample_arr_init_depolarising_a(
        const ART_GV                * art_gv,
        const ArAttenuationSample     * a0,
        const ArReferenceFrame      * r0,
        const ArReferenceFrame      * r1,
              ArAttenuationSample     * ar
        )
{
    if ( ! ARATTENUATIONSAMPLE_INITIALISED( a0 ) )
    {
        ARATTENUATIONSAMPLE_INITIALISED( ar ) = NO;
        return;
    }

    ASSERT_ALLOCATED_ATTENUATION_SAMPLE( ar )
    ASSERT_VALID_ATTENUATION_SAMPLE( a0 )

    ARATTENUATIONSAMPLE_FLUORESCENT( ar ) = ARATTENUATIONSAMPLE_FLUORESCENT( a0 );

    if ( ARATTENUATIONSAMPLE_FLUORESCENT( ar ) )
        arcrosstalksample_x_init_x(
            art_gv,
            ARATTENUATIONSAMPLE_CROSSTALK( a0 ),
            ARATTENUATIONSAMPLE_CROSSTALK( ar )
            );

    ArSpectralSample  temp_ss;

    ardirectattenuationsample_a_init_s(
          art_gv,
          ARATTENUATIONSAMPLE_MAIN_DIAGONAL( a0 ),
        & temp_ss
        );

    ardirectattenuationsample_srr_init_depolarising_a(
          art_gv,
        & temp_ss,
          r0,
          r1,
          ARATTENUATIONSAMPLE_MAIN_DIAGONAL( ar )
        );

    ARATTENUATIONSAMPLE_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION_SAMPLE( ar )
    ASSERT_IDENTICAL_SAMPLE_REFERENCE_FRAMES_AA( a0, ar )
}

void arattenuationsample_arr_init_nonpolarising_a(
        const ART_GV                * art_gv,
        const ArAttenuationSample     * a0,
        const ArReferenceFrame      * r0,
        const ArReferenceFrame      * r1,
              ArAttenuationSample     * ar
        )
{
    if ( ! ARATTENUATIONSAMPLE_INITIALISED( a0 ) )
    {
        ARATTENUATIONSAMPLE_INITIALISED( ar ) = NO;
        return;
    }

    ASSERT_ALLOCATED_ATTENUATION_SAMPLE( ar )
    ASSERT_VALID_ATTENUATION_SAMPLE( a0 )

    ARATTENUATIONSAMPLE_FLUORESCENT( ar ) = ARATTENUATIONSAMPLE_FLUORESCENT( a0 );

    if ( ARATTENUATIONSAMPLE_FLUORESCENT( ar ) )
        arcrosstalksample_x_init_x(
            art_gv,
            ARATTENUATIONSAMPLE_CROSSTALK( a0 ),
            ARATTENUATIONSAMPLE_CROSSTALK( ar )
            );

    ArSpectralSample  temp_ss;

    ardirectattenuationsample_a_init_s(
          art_gv,
          ARATTENUATIONSAMPLE_MAIN_DIAGONAL( a0 ),
        & temp_ss
        );

    ardirectattenuationsample_srr_init_nonpolarising_a(
          art_gv,
        & temp_ss,
          r0,
          r1,
          ARATTENUATIONSAMPLE_MAIN_DIAGONAL( ar )
        );

    ARATTENUATIONSAMPLE_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION_SAMPLE( ar )
    ASSERT_IDENTICAL_SAMPLE_REFERENCE_FRAMES_AA( a0, ar )
}

void  arattenuationsample_a_init_s(
        const ART_GV             * art_gv,
        const ArAttenuationSample  * a0,
              ArSpectralSample     * cr
        )
{
    ASSERT_VALID_ATTENUATION_SAMPLE( a0 )
    ASSERT_ALLOCATED_SPECTRUM( cr )

    ardirectattenuationsample_a_init_s(
        art_gv,
        ARATTENUATIONSAMPLE_MAIN_DIAGONAL( a0 ),
        cr
        );

    ASSERT_VALID_SPECTRAL_SAMPLE( cr )
}

void  arattenuationsample_a_init_a(
        const ART_GV               * art_gv,
        const ArAttenuationSample  * a0,
              ArAttenuationSample  * ar
        )
{
    if ( ! ARATTENUATIONSAMPLE_INITIALISED( a0 ) )
    {
        ARATTENUATIONSAMPLE_INITIALISED( ar ) = NO;
        return;
    }

    ASSERT_ALLOCATED_ATTENUATION_SAMPLE( ar )
    ASSERT_VALID_ATTENUATION_SAMPLE( a0 )

    ARATTENUATIONSAMPLE_FLUORESCENT( ar ) = ARATTENUATIONSAMPLE_FLUORESCENT( a0 );

    if ( ARATTENUATIONSAMPLE_FLUORESCENT( ar ) )
        arcrosstalksample_x_init_x(
            art_gv,
            ARATTENUATIONSAMPLE_CROSSTALK( a0 ),
            ARATTENUATIONSAMPLE_CROSSTALK( ar )
            );

    ardirectattenuationsample_a_init_a(
        art_gv,
        ARATTENUATIONSAMPLE_MAIN_DIAGONAL( a0 ),
        ARATTENUATIONSAMPLE_MAIN_DIAGONAL( ar )
        );

    ARATTENUATIONSAMPLE_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION_SAMPLE( ar )
    ASSERT_IDENTICAL_SAMPLE_REFERENCE_FRAMES_AA( a0, ar )
}

void arattenuationsample_mm_rr_init_polarising_a(
        const ART_GV                 * art_gv,
        const ArMuellerMatrixSample  * m0,
        const ArReferenceFrame       * r0,
        const ArReferenceFrame       * r1,
              ArAttenuationSample    * ar
        )
{
    ASSERT_LIGHT_SUBSYSTEM_IN_POLARISATION_MODE

    ASSERT_VALID_MUELLER_MATRIX_SAMPLE( m0 )
    ASSERT_VALID_REFERENCE_FRAME( r0 )
    ASSERT_VALID_REFERENCE_FRAME( r1 )
    ASSERT_ALLOCATED_ATTENUATION_SAMPLE( ar )

    ARATTENUATIONSAMPLE_FLUORESCENT( ar ) = NO;

    ardirectattenuationsample_mm_rr_init_polarising_a(
        art_gv,
        m0,
        r0,
        r1,
        ARATTENUATIONSAMPLE_MAIN_DIAGONAL( ar )
        );

    ARATTENUATIONSAMPLE_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION_SAMPLE( ar )
}

void arattenuationsample_ddrr_init_linear_polariser_a(
        const ART_GV               * art_gv,
        const double                 d0,
        const double                 d1,
        const ArReferenceFrame     * r0,
        const ArReferenceFrame     * r1,
              ArAttenuationSample  * ar
        )
{
    ASSERT_LIGHT_SUBSYSTEM_IN_POLARISATION_MODE

    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_NONNEGATIVE_DOUBLE( d1 )
    ASSERT_ALLOCATED_ATTENUATION_SAMPLE( ar )

    ARATTENUATIONSAMPLE_FLUORESCENT( ar ) = NO;

    ardirectattenuationsample_ddrr_init_linear_polariser_a(
        art_gv,
        d0,
        d1,
        r0,
        r1,
        ARATTENUATIONSAMPLE_MAIN_DIAGONAL( ar )
        );

    ARATTENUATIONSAMPLE_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION_SAMPLE( ar )
}

void arattenuationsample_sx_init_a(
        const ART_GV               * art_gv,
        const ArSpectralSample     * c0,
        const ArCrosstalkSample    * x0,
              ArAttenuationSample  * ar
        )
{
    ASSERT_VALID_SPECTRAL_SAMPLE( c0 )
    ASSERT_VALID_CROSSTALK_SAMPLE( x0 )
    ASSERT_ALLOCATED_ATTENUATION_SAMPLE( ar )

    ARATTENUATIONSAMPLE_FLUORESCENT( ar ) = YES;

    arcrosstalksample_x_init_x(
        art_gv,
        x0,
        ARATTENUATIONSAMPLE_CROSSTALK( ar )
        );

    ardirectattenuationsample_s_init_a(
        art_gv,
        c0,
        ARATTENUATIONSAMPLE_MAIN_DIAGONAL( ar )
        );

    ARATTENUATIONSAMPLE_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION_SAMPLE( ar )
}

void arattenuationsample_a_deactivate(
        const ART_GV               * art_gv,
              ArAttenuationSample  * ar
        )
{
    (void) art_gv;
    
    ARATTENUATIONSAMPLE_INITIALISED(ar) = NO;
}

unsigned int  arattenuationsample_a_polarising(
        const ART_GV               * art_gv,
        const ArAttenuationSample  * a0
        )
{
    return
        ardirectattenuationsample_a_polarising(
            art_gv,
            ARATTENUATIONSAMPLE_MAIN_DIAGONAL( a0 )
            );
}

ArReferenceFrame const * arattenuationsample_a_entry_rf(
        const ART_GV               * art_gv,
        const ArAttenuationSample  * a0
        )
{
    return
        ardirectattenuationsample_a_entry_rf(
            art_gv,
            ARATTENUATIONSAMPLE_MAIN_DIAGONAL( a0 )
            );
}

ArReferenceFrame const * arattenuationsample_a_exit_rf(
        const ART_GV             * art_gv,
        const ArAttenuationSample  * a0
        )
{
    return
        ardirectattenuationsample_a_exit_rf(
            art_gv,
            ARATTENUATIONSAMPLE_MAIN_DIAGONAL( a0 )
            );
}

void arattenuationsample_a_realign_to_coaxial_exit_rf_a(
        const ART_GV               * art_gv,
        const ArAttenuationSample  * a0,
        const ArReferenceFrame     * r0,
              ArAttenuationSample  * ar
        )
{
    ASSERT_VALID_ATTENUATION_SAMPLE( a0 )
    ASSERT_VALID_REFERENCE_FRAME( r0 )
    ASSERT_COAXIAL_SAMPLE_EXIT_REFERENCE_FRAME_RA( r0, a0, 3.0 DEGREES)
    ASSERT_ALLOCATED_ATTENUATION_SAMPLE( ar )

    ARATTENUATIONSAMPLE_FLUORESCENT( ar ) =
        ARATTENUATIONSAMPLE_FLUORESCENT( a0 );

    ARATTENUATIONSAMPLE_CROSSTALK( ar ) =
        ARATTENUATIONSAMPLE_CROSSTALK( a0 );

    ardirectattenuationsample_a_realign_to_coaxial_exit_rf_a(
        art_gv,
        ARATTENUATIONSAMPLE_MAIN_DIAGONAL( a0 ),
        r0,
        ARATTENUATIONSAMPLE_MAIN_DIAGONAL( ar )
        );

    ASSERT_VALID_ATTENUATION_SAMPLE( ar )
    ASSERT_COAXIAL_SAMPLE_EXIT_REFERENCE_FRAME_RA( r0, ar, 3.0 DEGREES)
}

double arattenuationsample_a_norm(
        const ART_GV               * art_gv,
        const ArAttenuationSample  * a0
        )
{
    ASSERT_VALID_ATTENUATION_SAMPLE( a0 )

    return
        ardirectattenuationsample_a_norm(
            art_gv,
            ARATTENUATIONSAMPLE_MAIN_DIAGONAL( a0 )
            );
}

void arattenuationsample_inv_a(
        const ART_GV               * art_gv,
              ArAttenuationSample  * ar
        )
{
    ASSERT_VALID_ATTENUATION_SAMPLE( ar )
    STORE_ATTENUATION_SAMPLE_REFERENCE_FRAMES( ar )

    if ( ARATTENUATIONSAMPLE_FLUORESCENT( ar ) )
        arcrosstalksample_inv_x(
            art_gv,
            ARATTENUATIONSAMPLE_CROSSTALK( ar )
            );

    ardirectattenuationsample_inv_a(
        art_gv,
        ARATTENUATIONSAMPLE_MAIN_DIAGONAL( ar )
        );

    ASSERT_VALID_ATTENUATION_SAMPLE( ar )
    ASSERT_IDENTICAL_SAMPLE_REFERENCE_FRAMES_SA( ar )
}

void arattenuationsample_a_inv_a(
        const ART_GV               * art_gv,
        const ArAttenuationSample  * a0,
              ArAttenuationSample  * ar
        )
{
    if ( ! ARATTENUATIONSAMPLE_INITIALISED( a0 ) )
    {
        ARATTENUATIONSAMPLE_INITIALISED( ar ) = NO;
        return;
    }

    ASSERT_VALID_ATTENUATION_SAMPLE( a0 )
    ASSERT_ALLOCATED_ATTENUATION_SAMPLE( ar )

    if ( ARATTENUATIONSAMPLE_FLUORESCENT( a0 ) )
    {
        arcrosstalksample_x_inv_x(
            art_gv,
            ARATTENUATIONSAMPLE_CROSSTALK( a0 ),
            ARATTENUATIONSAMPLE_CROSSTALK( ar )
            );

        ARATTENUATIONSAMPLE_FLUORESCENT( ar ) = YES;
    }
    else
        ARATTENUATIONSAMPLE_FLUORESCENT( ar ) = NO;

    ardirectattenuationsample_a_inv_a(
        art_gv,
        ARATTENUATIONSAMPLE_MAIN_DIAGONAL( a0 ),
        ARATTENUATIONSAMPLE_MAIN_DIAGONAL( ar )
        );

    ARATTENUATIONSAMPLE_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION_SAMPLE( ar )
}

double arattenuationsample_a_max(
        const ART_GV               * art_gv,
        const ArAttenuationSample  * a0
        )
{
    ASSERT_VALID_ATTENUATION_SAMPLE( a0 )

    if ( ARATTENUATIONSAMPLE_FLUORESCENT( a0 ) )
    {
        return
            M_MAX(
                ardirectattenuationsample_a_max( art_gv, ARATTENUATIONSAMPLE_MAIN_DIAGONAL( a0 ) ),
                arcrosstalksample_x_max( art_gv, ARATTENUATIONSAMPLE_CROSSTALK( a0 ) )
                );
    }
    else
        return
            ardirectattenuationsample_a_max( art_gv, ARATTENUATIONSAMPLE_MAIN_DIAGONAL(a0) );
}

void arattenuationsample_aa_max_a(
        const ART_GV               * art_gv,
        const ArAttenuationSample  * a0,
        const ArAttenuationSample  * a1,
              ArAttenuationSample  * ar
        )
{
    (void) art_gv;
    (void) a0;
    (void) a1;
    (void) ar;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

double arattenuationsample_a_min(
        const ART_GV               * art_gv,
        const ArAttenuationSample  * a0
        )
{
    ASSERT_VALID_ATTENUATION_SAMPLE( a0 )

    if ( ARATTENUATIONSAMPLE_FLUORESCENT( a0 ) )
    {
        return
            M_MIN(
                ardirectattenuationsample_a_min( art_gv, ARATTENUATIONSAMPLE_MAIN_DIAGONAL( a0 ) ),
                arcrosstalksample_x_min( art_gv, ARATTENUATIONSAMPLE_CROSSTALK( a0 ) )
                );
    }
    else
        return
            ardirectattenuationsample_a_min( art_gv, ARATTENUATIONSAMPLE_MAIN_DIAGONAL(a0) );
}

void arattenuationsample_aa_min_a(
        const ART_GV               * art_gv,
        const ArAttenuationSample  * a0,
        const ArAttenuationSample  * a1,
              ArAttenuationSample  * ar
        )
{
    (void) art_gv;
    (void) a0;
    (void) a1;
    (void) ar;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

double arattenuationsample_a_avg(
        const ART_GV               * art_gv,
        const ArAttenuationSample  * a0
        )
{
    ASSERT_VALID_ATTENUATION_SAMPLE( a0 )

    if ( ARATTENUATIONSAMPLE_FLUORESCENT( a0 ) )
    {
        return
            M_MAX(
                ardirectattenuationsample_a_avg( art_gv, ARATTENUATIONSAMPLE_MAIN_DIAGONAL( a0 ) ),
                arcrosstalksample_x_avg( art_gv, ARATTENUATIONSAMPLE_CROSSTALK( a0 ) )
                );
    }
    else
        return
            ardirectattenuationsample_a_avg( art_gv, ARATTENUATIONSAMPLE_MAIN_DIAGONAL(a0) );
}

double arattenuationsample_aa_maxdiff(
        const ART_GV             * art_gv,
        const ArAttenuationSample  * a0,
        const ArAttenuationSample  * a1
        )
{
    (void) art_gv;
    (void) a0;
    (void) a1;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return 0.0;
}

void arattenuationsample_a_add_a(
        const ART_GV             * art_gv,
        const ArAttenuationSample  * a0,
              ArAttenuationSample  * ar
        )
{
    if (    ! ARATTENUATIONSAMPLE_INITIALISED( a0 )
         || ! ARATTENUATIONSAMPLE_INITIALISED( ar ))
    {
        ARATTENUATIONSAMPLE_INITIALISED( ar ) = NO;
        return;
    }

    ASSERT_VALID_ATTENUATION_SAMPLE( a0 )
    ASSERT_VALID_ATTENUATION_SAMPLE( ar )

    if ( ARATTENUATIONSAMPLE_FLUORESCENT( a0 ) )
    {
        if ( ARATTENUATIONSAMPLE_FLUORESCENT( ar ) )
        {
            arcrosstalksample_x_add_x(
                art_gv,
                ARATTENUATIONSAMPLE_CROSSTALK( a0 ),
                ARATTENUATIONSAMPLE_CROSSTALK( ar )
                );
        }
        else
        {
            ARATTENUATIONSAMPLE_FLUORESCENT( ar ) = YES;
            arcrosstalksample_x_init_x(
                art_gv,
                ARATTENUATIONSAMPLE_CROSSTALK( a0 ),
                ARATTENUATIONSAMPLE_CROSSTALK( ar )
                );
        }
    }

    ardirectattenuationsample_a_add_a(
        art_gv,
        ARATTENUATIONSAMPLE_MAIN_DIAGONAL( a0 ),
        ARATTENUATIONSAMPLE_MAIN_DIAGONAL( ar )
        );

    ARATTENUATIONSAMPLE_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION_SAMPLE( ar )
}

void arattenuationsample_aa_add_a(
        const ART_GV             * art_gv,
        const ArAttenuationSample  * a0,
        const ArAttenuationSample  * a1,
              ArAttenuationSample  * ar
        )
{
    if (    ! ARATTENUATIONSAMPLE_INITIALISED( a0 )
         || ! ARATTENUATIONSAMPLE_INITIALISED( a1 ))
    {
        ARATTENUATIONSAMPLE_INITIALISED( ar ) = NO;
        return;
    }

    ASSERT_VALID_ATTENUATION_SAMPLE( a0 )
    ASSERT_VALID_ATTENUATION_SAMPLE( a1 )
    ASSERT_ALLOCATED_ATTENUATION_SAMPLE( ar )

    if ( ARATTENUATIONSAMPLE_FLUORESCENT( a0 ) )
    {
        if ( ARATTENUATIONSAMPLE_FLUORESCENT( a1 ) )
            arcrosstalksample_xx_add_x(
                art_gv,
                ARATTENUATIONSAMPLE_CROSSTALK( a0 ),
                ARATTENUATIONSAMPLE_CROSSTALK( a1 ),
                ARATTENUATIONSAMPLE_CROSSTALK( ar )
                );
        else
            arcrosstalksample_x_init_x(
                art_gv,
                ARATTENUATIONSAMPLE_CROSSTALK( a0 ),
                ARATTENUATIONSAMPLE_CROSSTALK( ar )
                );

        ARATTENUATIONSAMPLE_FLUORESCENT( ar ) = YES;
    }
    else
    {
        if ( ARATTENUATIONSAMPLE_FLUORESCENT( a1 ) )
        {
            arcrosstalksample_x_init_x(
                art_gv,
                ARATTENUATIONSAMPLE_CROSSTALK( a1 ),
                ARATTENUATIONSAMPLE_CROSSTALK( ar )
                );

            ARATTENUATIONSAMPLE_FLUORESCENT( ar ) = YES;
        }
        else
            ARATTENUATIONSAMPLE_FLUORESCENT( ar ) = NO;
    }

    ardirectattenuationsample_aa_add_a(
        art_gv,
        ARATTENUATIONSAMPLE_MAIN_DIAGONAL( a0 ),
        ARATTENUATIONSAMPLE_MAIN_DIAGONAL( a1 ),
        ARATTENUATIONSAMPLE_MAIN_DIAGONAL( ar )
        );

    ARATTENUATIONSAMPLE_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION_SAMPLE( ar )
}

void arattenuationsample_aa_sub_a(
        const ART_GV             * art_gv,
        const ArAttenuationSample  * a0,
        const ArAttenuationSample  * a1,
              ArAttenuationSample  * ar
        )
{
    if (    ! ARATTENUATIONSAMPLE_INITIALISED( a0 )
         || ! ARATTENUATIONSAMPLE_INITIALISED( a1 ))
    {
        ARATTENUATIONSAMPLE_INITIALISED( ar ) = NO;
        return;
    }

    ASSERT_VALID_ATTENUATION_SAMPLE( a0 )
    ASSERT_VALID_ATTENUATION_SAMPLE( a1 )
    ASSERT_ALLOCATED_ATTENUATION_SAMPLE( ar )

    if ( ARATTENUATIONSAMPLE_FLUORESCENT( a0 ) )
    {
        if ( ARATTENUATIONSAMPLE_FLUORESCENT( a1 ) )
            arcrosstalksample_xx_sub_x(
                art_gv,
                ARATTENUATIONSAMPLE_CROSSTALK( a0 ),
                ARATTENUATIONSAMPLE_CROSSTALK( a1 ),
                ARATTENUATIONSAMPLE_CROSSTALK( ar )
                );
        else
            arcrosstalksample_dx_mul_x(
                art_gv,
                -1.0,
                ARATTENUATIONSAMPLE_CROSSTALK( a0 ),
                ARATTENUATIONSAMPLE_CROSSTALK( ar )
                );

        ARATTENUATIONSAMPLE_FLUORESCENT( ar ) = YES;
    }
    else
    {
        if ( ARATTENUATIONSAMPLE_FLUORESCENT( a1 ) )
        {
            arcrosstalksample_dx_mul_x(
                art_gv,
                -1.0,
                ARATTENUATIONSAMPLE_CROSSTALK( a1 ),
                ARATTENUATIONSAMPLE_CROSSTALK( ar )
                );

            ARATTENUATIONSAMPLE_FLUORESCENT( ar ) = YES;
        }
        else
            ARATTENUATIONSAMPLE_FLUORESCENT( ar ) = NO;
    }

    ardirectattenuationsample_aa_sub_a(
        art_gv,
        ARATTENUATIONSAMPLE_MAIN_DIAGONAL( a0 ),
        ARATTENUATIONSAMPLE_MAIN_DIAGONAL( a1 ),
        ARATTENUATIONSAMPLE_MAIN_DIAGONAL( ar )
        );

    ARATTENUATIONSAMPLE_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION_SAMPLE( ar )
}


void arattenuationsample_d_mul_a(
        const ART_GV             * art_gv,
        const double               d0,
              ArAttenuationSample  * ar
        )
{
    if ( ! ARATTENUATIONSAMPLE_INITIALISED( ar ) )
        return;

    ASSERT_NONNEGATIVE_FINITE_DOUBLE( d0 )
    ASSERT_VALID_ATTENUATION_SAMPLE( ar )

    if ( ARATTENUATIONSAMPLE_FLUORESCENT( ar ) )
        arcrosstalksample_d_mul_x(
            art_gv,
            d0,
            ARATTENUATIONSAMPLE_CROSSTALK( ar )
            );

    ardirectattenuationsample_d_mul_a(
        art_gv,
        d0,
        ARATTENUATIONSAMPLE_MAIN_DIAGONAL( ar )
        );

    ASSERT_VALID_ATTENUATION_SAMPLE( ar )
}


void arattenuationsample_s_mul_a(
        const ART_GV               * art_gv,
        const ArSpectralSample     * s0,
              ArAttenuationSample  * ar
        )
{
    if ( ! ARATTENUATIONSAMPLE_INITIALISED( ar ) )
        return;

    ASSERT_VALID_SPECTRAL_SAMPLE( s0 )
    ASSERT_VALID_ATTENUATION_SAMPLE( ar )

    if ( ARATTENUATIONSAMPLE_FLUORESCENT( ar ) )
        arcrosstalksample_s_mul_x(
              art_gv,
              s0,
              ARATTENUATIONSAMPLE_CROSSTALK( ar )
            );

    ardirectattenuationsample_s_mul_a(
          art_gv,
          s0,
          ARATTENUATIONSAMPLE_MAIN_DIAGONAL( ar )
        );

    ASSERT_VALID_ATTENUATION_SAMPLE( ar )
}


void arattenuationsample_da_mul_a(
        const ART_GV             * art_gv,
        const double               d0,
        const ArAttenuationSample  * a0,
              ArAttenuationSample  * ar
        )
{
    if ( ! ARATTENUATIONSAMPLE_INITIALISED( a0 ) )
    {
        ARATTENUATIONSAMPLE_INITIALISED( ar ) = NO;
        return;
    }

    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_VALID_ATTENUATION_SAMPLE( a0 )
    ASSERT_ALLOCATED_ATTENUATION_SAMPLE( ar )

    if ( ARATTENUATIONSAMPLE_FLUORESCENT( a0 ) )
    {
        arcrosstalksample_dx_mul_x(
            art_gv,
            d0,
            ARATTENUATIONSAMPLE_CROSSTALK( a0 ),
            ARATTENUATIONSAMPLE_CROSSTALK( ar )
            );

        ARATTENUATIONSAMPLE_FLUORESCENT( ar ) = YES;
    }
    else
        ARATTENUATIONSAMPLE_FLUORESCENT( ar ) = NO;

    ardirectattenuationsample_da_mul_a(
        art_gv,
        d0,
        ARATTENUATIONSAMPLE_MAIN_DIAGONAL( a0 ),
        ARATTENUATIONSAMPLE_MAIN_DIAGONAL( ar )
        );

    ARATTENUATIONSAMPLE_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION_SAMPLE( ar )
    ASSERT_IDENTICAL_SAMPLE_REFERENCE_FRAMES_AA( a0, ar )
}


void arattenuationsample_a_mul_a(
        const ART_GV             * art_gv,
        const ArAttenuationSample  * a0,
              ArAttenuationSample  * ar
        )
{
    if ( ARATTENUATIONSAMPLE_INITIALISED( a0 ) )
    {
        ASSERT_VALID_ATTENUATION_SAMPLE( a0 )
        ASSERT_VALID_ATTENUATION_SAMPLE( ar )
        ASSERT_SAMPLE_REFERENCE_FRAME_COMPATIBILITY_AA( a0, ar, 3.0 DEGREES)

        ArAttenuationSample  * temp = arattenuationsample_alloc( art_gv );

        if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE )
        {
            arattenuationsample_drr_init_nonpolarising_a(
                art_gv,
                0.0,
                arattenuationsample_a_entry_rf(art_gv,a0),
                arattenuationsample_a_exit_rf(art_gv,ar),
                temp
                );
        }
        else
        {
            arattenuationsample_d_init_a(
                art_gv,
                0.0,
                temp
                );
        }

        arattenuationsample_aa_mul_a( art_gv, a0, ar, temp );

        ASSERT_VALID_ATTENUATION_SAMPLE( temp )
        ASSERT_COAXIAL_SAMPLE_ENTRY_REFERENCE_FRAME_AA(
            a0, temp, 3.0 DEGREES
            )
        ASSERT_COAXIAL_SAMPLE_EXIT_REFERENCE_FRAME_AA(
            ar, temp, 3.0 DEGREES
            )

        arattenuationsample_a_init_a(
            art_gv,
            temp,
            ar
            );

        arattenuationsample_free( art_gv, temp );
    }

    //   else do nothing - if a0 is uninitialised, ar stays the way it is

    ASSERT_VALID_ATTENUATION_SAMPLE( ar )
}

#define FM(__m,__x,__y)     (__m)[number_of_colour_channels*(__y)+(__x)]

void arattenuationsample_aa_mul_a(
        const ART_GV             * art_gv,
        const ArAttenuationSample  * a0,
        const ArAttenuationSample  * a1,
              ArAttenuationSample  * ar
        )
{
    if ( ARATTENUATIONSAMPLE_INITIALISED( a0 ) )
    {
        ASSERT_VALID_ATTENUATION_SAMPLE( a0 )
        ASSERT_VALID_ATTENUATION_SAMPLE( a1 )
        ASSERT_ALLOCATED_ATTENUATION_SAMPLE( ar )
        ASSERT_SAMPLE_REFERENCE_FRAME_COMPATIBILITY_AA( a0, a1, 3.0 DEGREES)

        if (   ARATTENUATIONSAMPLE_FLUORESCENT( a0 )
            || ARATTENUATIONSAMPLE_FLUORESCENT( a1 ) )
        {
ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
#ifdef NEVERMORE
            // do something unspeakable here

            // this is just as unspeakable - but at least it works
#warning channel number FIXIT
            int number_of_colour_channels = 1;

            double  * m0, *m1, *mr;

            m0 = ALLOC_ARRAY(double, number_of_colour_channels * number_of_colour_channels);
            m1 = ALLOC_ARRAY(double, number_of_colour_channels * number_of_colour_channels);
            mr = ALLOC_ARRAY(double, number_of_colour_channels * number_of_colour_channels);

            int  c = 0;

            //   We build two quadratic 2D arrays from the information in
            //   the attenuation.

            for ( int i = 0; i < number_of_colour_channels; i++ )
                for ( int j = 0; j < number_of_colour_channels; j++ )
                {

                    FM( mr, i, j ) = 0.0;

                    if ( j > i )
                    {
                        FM( m0, i, j ) = 0.0;
                        FM( m1, i, j ) = 0.0;
                    }

                    if ( j == i )
                    {
                        FM( m0, i, j ) = ARATTENUATIONSAMPLE_MAIN_DIAGONAL_AI( a0 , i );
                        FM( m1, i, j ) = ARATTENUATIONSAMPLE_MAIN_DIAGONAL_AI( a1 , i );
                    }

                    if ( j < i )
                    {
                        if ( ARATTENUATIONSAMPLE_FLUORESCENT( a0 ) )
                            FM( m0, i, j ) = ARATTENUATIONSAMPLE_CROSSTALK_XI( a0, c );
                        else
                            FM( m0, i, j ) = 0.0;

                        if ( ARATTENUATIONSAMPLE_FLUORESCENT( a1 ) )
                            FM( m1, i, j ) = ARATTENUATIONSAMPLE_CROSSTALK_XI( a1, c );
                        else
                            FM( m1, i, j ) = 0.0;

                        c++;
                    }
                }

            //   These arrays can then be used for a real matrix multiplication

            for( int i = 0; i < number_of_colour_channels; i++ )
                for( int j = 0; j < number_of_colour_channels; j++ )
                    for( int k = 0; k < number_of_colour_channels; k++ )
                        FM( mr, i, j ) += FM( m0, i, k ) * FM( m1, k, j );

            //   And the contents of the result are then transferred back to
            //   the result attenuation.

            c = 0;

            for ( int i = 0; i < number_of_colour_channels; i++ )
                for ( int j = 0; j < number_of_colour_channels; j++ )
                {
                    if ( j == i )
                        ARATTENUATIONSAMPLE_ATTENUATION_SET_AID( ar, i, FM( mr, i, j ) );

                    if ( j < i )
                        ARATTENUATIONSAMPLE_CROSSTALK_XI( ar, c++ ) = FM( mr, i, j );
                 }

            FREE_ARRAY(m0);
            FREE_ARRAY(m1);
            FREE_ARRAY(mr);

            ARATTENUATIONSAMPLE_FLUORESCENT( ar ) = YES;
#endif
        }
        else
        {
            ardirectattenuationsample_aa_mul_a(
                art_gv,
                ARATTENUATIONSAMPLE_MAIN_DIAGONAL( a0 ),
                ARATTENUATIONSAMPLE_MAIN_DIAGONAL( a1 ),
                ARATTENUATIONSAMPLE_MAIN_DIAGONAL( ar )
                );

            arcrosstalksample_d_init_x(
                art_gv,
                0.0,
                ARATTENUATIONSAMPLE_CROSSTALK( ar )
                );

            ARATTENUATIONSAMPLE_FLUORESCENT( ar ) = NO;
        }

        ASSERT_VALID_ATTENUATION_SAMPLE( ar )
        ASSERT_COAXIAL_SAMPLE_ENTRY_REFERENCE_FRAME_AA( a0, ar, 3.0 DEGREES)
        ASSERT_COAXIAL_SAMPLE_EXIT_REFERENCE_FRAME_AA( a1, ar, 3.0 DEGREES)
    }
    else
    {
        //   If a0 is uninitialised, ar is simply a copy of a1.

        ASSERT_VALID_ATTENUATION_SAMPLE( a1 )
        ASSERT_ALLOCATED_ATTENUATION_SAMPLE( ar )

        arattenuationsample_a_init_a(
            art_gv,
            a1,
            ar
            );

        ASSERT_VALID_ATTENUATION_SAMPLE( ar )
    }
}

void arattenuationsample_d_div_a(
        const ART_GV             * art_gv,
        const double               d0,
              ArAttenuationSample  * ar
        )
{
    if ( d0 > 0.0 )
    {
        if ( ARATTENUATIONSAMPLE_FLUORESCENT( ar ) )
            arcrosstalksample_d_mul_x(
                art_gv,
                1.0 / d0,
                ARATTENUATIONSAMPLE_CROSSTALK( ar )
                );

        ardirectattenuationsample_d_mul_a(
            art_gv,
            1.0 / d0,
            ARATTENUATIONSAMPLE_MAIN_DIAGONAL( ar )
            );
    }
}

void arattenuationsample_da_div_a(
        const ART_GV             * art_gv,
        const double               d0,
        const ArAttenuationSample  * a0,
              ArAttenuationSample  * ar
        )
{
    if ( ! ARATTENUATIONSAMPLE_INITIALISED( a0 ) )
    {
        ARATTENUATIONSAMPLE_INITIALISED( ar ) = NO;
        return;
    }

    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_VALID_ATTENUATION_SAMPLE( a0 )
    ASSERT_ALLOCATED_ATTENUATION_SAMPLE( ar )

    if ( d0 > 0.0 )
    {
        if ( ARATTENUATIONSAMPLE_FLUORESCENT( a0 ) )
        {
            arcrosstalksample_dx_mul_x(
                art_gv,
                1.0 / d0,
                ARATTENUATIONSAMPLE_CROSSTALK( a0 ),
                ARATTENUATIONSAMPLE_CROSSTALK( ar )
                );
            ARATTENUATIONSAMPLE_FLUORESCENT( ar ) = YES;
        }
        else
            ARATTENUATIONSAMPLE_FLUORESCENT( ar ) = NO;

        ardirectattenuationsample_da_mul_a(
            art_gv,
            1.0 / d0,
            ARATTENUATIONSAMPLE_MAIN_DIAGONAL( a0 ),
            ARATTENUATIONSAMPLE_MAIN_DIAGONAL( ar )
            );
    }

    ARATTENUATIONSAMPLE_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION_SAMPLE( ar )
    ASSERT_IDENTICAL_SAMPLE_REFERENCE_FRAMES_AA( a0, ar )
}

void arattenuationsample_da_pow_a(
        const ART_GV             * art_gv,
        const double               d0,
        const ArAttenuationSample  * a0,
              ArAttenuationSample  * ar
        )
{
    if ( ! ARATTENUATIONSAMPLE_INITIALISED( a0 ) )
    {
        ARATTENUATIONSAMPLE_INITIALISED( ar ) = NO;
        return;
    }

    ASSERT_VALID_ATTENUATION_SAMPLE( a0 )
    ASSERT_ALLOCATED_ATTENUATION_SAMPLE( ar )

    if ( ARATTENUATIONSAMPLE_FLUORESCENT( a0 ) )
    {
        arcrosstalksample_dx_pow_x(
            art_gv,
            d0,
            ARATTENUATIONSAMPLE_CROSSTALK( a0 ),
            ARATTENUATIONSAMPLE_CROSSTALK( ar )
            );

        ARATTENUATIONSAMPLE_FLUORESCENT( ar ) = YES;
    }
    else
        ARATTENUATIONSAMPLE_FLUORESCENT( ar ) = NO;

    ardirectattenuationsample_da_pow_a(
        art_gv,
        d0,
        ARATTENUATIONSAMPLE_MAIN_DIAGONAL( a0 ),
        ARATTENUATIONSAMPLE_MAIN_DIAGONAL( ar )
        );

    ARATTENUATIONSAMPLE_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION_SAMPLE( ar )
    ASSERT_IDENTICAL_SAMPLE_REFERENCE_FRAMES_AA( a0, ar )
}

void arattenuationsample_da_negpow_a(
        const ART_GV             * art_gv,
        const double               d0,
        const ArAttenuationSample  * a0,
              ArAttenuationSample  * ar
        )
{
    if ( ! ARATTENUATIONSAMPLE_INITIALISED( a0 ) )
    {
        ARATTENUATIONSAMPLE_INITIALISED( ar ) = NO;
        return;
    }

    ASSERT_VALID_ATTENUATION_SAMPLE( a0 )
    ASSERT_ALLOCATED_ATTENUATION_SAMPLE( ar )

    if ( ARATTENUATIONSAMPLE_FLUORESCENT( a0 ) )
    {
        arcrosstalksample_dx_negpow_x(
            art_gv,
            d0,
            ARATTENUATIONSAMPLE_CROSSTALK( a0 ),
            ARATTENUATIONSAMPLE_CROSSTALK( ar )
            );

        ARATTENUATIONSAMPLE_FLUORESCENT( ar ) = YES;
    }
    else
        ARATTENUATIONSAMPLE_FLUORESCENT( ar ) = NO;

    ardirectattenuationsample_da_negpow_a(
        art_gv,
        d0,
        ARATTENUATIONSAMPLE_MAIN_DIAGONAL( a0 ),
        ARATTENUATIONSAMPLE_MAIN_DIAGONAL( ar )
        );

    ARATTENUATIONSAMPLE_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION_SAMPLE( ar )
    ASSERT_IDENTICAL_SAMPLE_REFERENCE_FRAMES_AA( a0, ar )
}

void arattenuationsample_dda_clamp_a(
        const ART_GV             * art_gv,
        const double               d0,
        const double               d1,
        const ArAttenuationSample  * a0,
              ArAttenuationSample  * ar
        )
{
    if ( ! ARATTENUATIONSAMPLE_INITIALISED( a0 ) )
    {
        ARATTENUATIONSAMPLE_INITIALISED( ar ) = NO;
        return;
    }

    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_NONNEGATIVE_DOUBLE( d1 )
    ASSERT_VALID_ATTENUATION_SAMPLE( a0 )
    ASSERT_ALLOCATED_ATTENUATION_SAMPLE( ar )

    if ( ARATTENUATIONSAMPLE_FLUORESCENT( a0 ) )
    {
        arcrosstalksample_ddx_clamp_x(
            art_gv,
            d0,
            d1,
            ARATTENUATIONSAMPLE_CROSSTALK( a0 ),
            ARATTENUATIONSAMPLE_CROSSTALK( ar )
            );

        ARATTENUATIONSAMPLE_FLUORESCENT( ar ) = YES;
    }
    else
        ARATTENUATIONSAMPLE_FLUORESCENT( ar ) = NO;

    ardirectattenuationsample_dda_clamp_a(
        art_gv,
        d0,
        d1,
        ARATTENUATIONSAMPLE_MAIN_DIAGONAL( a0 ),
        ARATTENUATIONSAMPLE_MAIN_DIAGONAL( ar )
        );

    ARATTENUATIONSAMPLE_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION_SAMPLE( ar )
    ASSERT_IDENTICAL_SAMPLE_REFERENCE_FRAMES_AA( a0, ar )
}

void arattenuationsample_da_mul_add_a(
        const ART_GV             * art_gv,
        const double               d0,
        const ArAttenuationSample  * a0,
              ArAttenuationSample  * ar
        )
{
    if ( ! ARATTENUATIONSAMPLE_INITIALISED( a0 ) )
    {
        ARATTENUATIONSAMPLE_INITIALISED( ar ) = NO;
        return;
    }

    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_VALID_ATTENUATION_SAMPLE( a0 )
    ASSERT_VALID_ATTENUATION_SAMPLE( ar )

    if ( ARATTENUATIONSAMPLE_FLUORESCENT( a0 ) )
    {
        if ( ARATTENUATIONSAMPLE_FLUORESCENT( ar ) )
            arcrosstalksample_dx_mul_add_x(
                art_gv,
                d0,
                ARATTENUATIONSAMPLE_CROSSTALK( a0 ),
                ARATTENUATIONSAMPLE_CROSSTALK( ar )
                );
        else
        {
            ARATTENUATIONSAMPLE_FLUORESCENT( ar ) = YES;
            arcrosstalksample_dx_mul_x(
                art_gv,
                d0,
                ARATTENUATIONSAMPLE_CROSSTALK( a0 ),
                ARATTENUATIONSAMPLE_CROSSTALK( ar )
                );
        }
    }

    ardirectattenuationsample_da_mul_add_a(
        art_gv,
        d0,
        ARATTENUATIONSAMPLE_MAIN_DIAGONAL( a0 ),
        ARATTENUATIONSAMPLE_MAIN_DIAGONAL( ar )
        );

    ARATTENUATIONSAMPLE_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION_SAMPLE( ar )
}

void arattenuationsample_daa_interpol_a(
        const ART_GV             * art_gv,
        const double               d0,
        const ArAttenuationSample  * a0,
        const ArAttenuationSample  * a1,
              ArAttenuationSample  * ar
        )
{
    if (    ! ARATTENUATIONSAMPLE_INITIALISED( a0 )
         || ! ARATTENUATIONSAMPLE_INITIALISED( a1 ))
    {
        ARATTENUATIONSAMPLE_INITIALISED( ar ) = NO;
        return;
    }

    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_VALID_ATTENUATION_SAMPLE( a0 )
    ASSERT_ALLOCATED_ATTENUATION_SAMPLE( ar )

    if ( ARATTENUATIONSAMPLE_FLUORESCENT( a0 ) )
    {
        if ( ARATTENUATIONSAMPLE_FLUORESCENT( a1 ) )
            arcrosstalksample_dxx_interpol_x(
                art_gv,
                d0,
                ARATTENUATIONSAMPLE_CROSSTALK( a0 ),
                ARATTENUATIONSAMPLE_CROSSTALK( a1 ),
                ARATTENUATIONSAMPLE_CROSSTALK( ar )
                );
        else
            arcrosstalksample_dxx_interpol_x(
                art_gv,
                d0,
                ARATTENUATIONSAMPLE_CROSSTALK( a0 ),
                ARCROSSTALKSAMPLE_NONE,
                ARATTENUATIONSAMPLE_CROSSTALK( ar )
                );

        ARATTENUATIONSAMPLE_FLUORESCENT( ar ) = YES;
    }
    else
    {
        if ( ARATTENUATIONSAMPLE_FLUORESCENT( a1 ) )
        {
            arcrosstalksample_dxx_interpol_x(
                art_gv,
                d0,
                ARCROSSTALKSAMPLE_NONE,
                ARATTENUATIONSAMPLE_CROSSTALK( a1 ),
                ARATTENUATIONSAMPLE_CROSSTALK( ar )
                );

            ARATTENUATIONSAMPLE_FLUORESCENT( ar ) = YES;
        }
        else
            ARATTENUATIONSAMPLE_FLUORESCENT( ar ) = NO;
    }

    ardirectattenuationsample_daa_interpol_a(
        art_gv,
        d0,
        ARATTENUATIONSAMPLE_MAIN_DIAGONAL( a0 ),
        ARATTENUATIONSAMPLE_MAIN_DIAGONAL( a1 ),
        ARATTENUATIONSAMPLE_MAIN_DIAGONAL( ar )
        );

    ARATTENUATIONSAMPLE_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION_SAMPLE( ar )
}

void arattenuationsample_a_complement_from_one_a(
        const ART_GV             * art_gv,
        const ArAttenuationSample  * a0,
              ArAttenuationSample  * ar
        )
{
    if ( ! ARATTENUATIONSAMPLE_INITIALISED( a0 ) )
    {
        ARATTENUATIONSAMPLE_INITIALISED( ar ) = NO;
        return;
    }

    ASSERT_VALID_ATTENUATION_SAMPLE( a0 )
    ASSERT_ALLOCATED_ATTENUATION_SAMPLE( ar )

    if ( ARATTENUATIONSAMPLE_FLUORESCENT( a0 ) )
    {
        arcrosstalksample_x_complement_from_one_x(
            art_gv,
            ARATTENUATIONSAMPLE_CROSSTALK( a0 ),
            ARATTENUATIONSAMPLE_CROSSTALK( ar )
            );

        ARATTENUATIONSAMPLE_FLUORESCENT( ar ) = YES;
    }
    else
        ARATTENUATIONSAMPLE_FLUORESCENT( ar ) = NO;

    ardirectattenuationsample_a_complement_from_one_a(
        art_gv,
        ARATTENUATIONSAMPLE_MAIN_DIAGONAL( a0 ),
        ARATTENUATIONSAMPLE_MAIN_DIAGONAL( ar )
        );

    ARATTENUATIONSAMPLE_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION_SAMPLE( ar )
    ASSERT_IDENTICAL_SAMPLE_REFERENCE_FRAMES_AA( a0, ar )
}

void arlightsample_a_mul_l(
        const ART_GV             * art_gv,
        const ArAttenuationSample  * a0,
              ArLightSample        * lr
        )
{
    ASSERT_ALLOCATED_ATTENUATION_SAMPLE( a0 )

    if ( ARATTENUATIONSAMPLE_INITIALISED( a0 ) )
    {
        ASSERT_VALID_ATTENUATION_SAMPLE( a0 )
        ASSERT_VALID_LIGHT_SAMPLE( lr )
        ASSERT_COAXIAL_SAMPLE_ENTRY_REFERENCE_FRAME_LA( lr, a0, 3.0 DEGREES)

        if ( ARATTENUATIONSAMPLE_FLUORESCENT( a0 ) )
            ardirectattenuationsample_ax_mul_l(
                art_gv,
                ARATTENUATIONSAMPLE_MAIN_DIAGONAL( a0 ),
                ARATTENUATIONSAMPLE_CROSSTALK( a0 ),
                lr
                );
        else
            ardirectattenuationsample_a_mul_l(
                art_gv,
                ARATTENUATIONSAMPLE_MAIN_DIAGONAL( a0 ),
                lr
                );

        ASSERT_VALID_LIGHT_SAMPLE( lr )
        ASSERT_COAXIAL_SAMPLE_EXIT_REFERENCE_FRAME_LA( lr, a0, 3.0 DEGREES)
    }
    else
    {
        ASSERT_VALID_LIGHT_SAMPLE( lr )
    }
}

void arlightsample_al_mul_l(
        const ART_GV             * art_gv,
        const ArAttenuationSample  * a0,
        const ArLightSample        * l0,
              ArLightSample        * lr
        )
{
    ASSERT_ALLOCATED_ATTENUATION_SAMPLE( a0 )

    if ( ARATTENUATIONSAMPLE_INITIALISED( a0 ) )
    {
        ASSERT_VALID_ATTENUATION_SAMPLE( a0 )
        ASSERT_VALID_LIGHT_SAMPLE( l0 )
        ASSERT_ALLOCATED_LIGHT_SAMPLE( lr )
        ASSERT_COAXIAL_SAMPLE_ENTRY_REFERENCE_FRAME_LA( l0, a0, 3.0 DEGREES)

        if ( ARATTENUATIONSAMPLE_FLUORESCENT( a0 ) )
            ardirectattenuationsample_axl_mul_l(
                art_gv,
                ARATTENUATIONSAMPLE_MAIN_DIAGONAL( a0 ),
                ARATTENUATIONSAMPLE_CROSSTALK( a0 ),
                l0,
                lr
                );
        else
            ardirectattenuationsample_al_mul_l(
                art_gv,
                ARATTENUATIONSAMPLE_MAIN_DIAGONAL( a0 ),
                l0,
                lr
                );

        ASSERT_VALID_LIGHT_SAMPLE( lr )
        ASSERT_COAXIAL_SAMPLE_EXIT_REFERENCE_FRAME_LA( lr, a0, 3.0 DEGREES)
    }
    else
    {
        ASSERT_VALID_LIGHT_SAMPLE( lr )
    }
}

void arlightintensitysample_a_mul_i(
        const ART_GV                * art_gv,
        const ArAttenuationSample     * a0,
              ArLightIntensitySample  * lr
        )
{
    if ( ARATTENUATIONSAMPLE_FLUORESCENT( a0 ) )
        ardirectattenuationsample_ax_mul_i(
            art_gv,
            ARATTENUATIONSAMPLE_MAIN_DIAGONAL( a0 ),
            ARATTENUATIONSAMPLE_CROSSTALK( a0 ),
            lr
            );
    else
        ardirectattenuationsample_a_mul_i(
            art_gv,
            ARATTENUATIONSAMPLE_MAIN_DIAGONAL( a0 ),
            lr
            );
}

void arlightintensitysample_ai_mul_i(
        const ART_GV                * art_gv,
        const ArAttenuationSample     * a0,
        const ArLightIntensitySample  * l0,
              ArLightIntensitySample  * lr
        )
{
    if ( ARATTENUATIONSAMPLE_FLUORESCENT( a0 ) )
        ardirectattenuationsample_axi_mul_i(
            art_gv,
            ARATTENUATIONSAMPLE_MAIN_DIAGONAL( a0 ),
            ARATTENUATIONSAMPLE_CROSSTALK( a0 ),
            l0,
            lr
            );
    else
        ardirectattenuationsample_ai_mul_i(
            art_gv,
            ARATTENUATIONSAMPLE_MAIN_DIAGONAL( a0 ),
            l0,
            lr
            );
}

unsigned int arattenuationsample_a_valid(
        const ART_GV             * art_gv,
        const ArAttenuationSample  * a0
        )
{
    if ( ! a0 )
    {
        ART_ERRORHANDLING_WARNING( "null pointer" );
        return 0;
    }

    unsigned int  ir = 0;

    if ( ARATTENUATIONSAMPLE_FLUORESCENT( a0 ) )
    {
        ir =
            arcrosstalksample_x_valid(
                art_gv,
                ARATTENUATIONSAMPLE_CROSSTALK( a0 )
                );

        if ( ! ir )
            ART_ERRORHANDLING_WARNING( "invalid crosstalk" );

        //   No point in checking the validity of the attenuation
        //   if the crosstalk is already defective.

        if ( ir )
        {
            ir =
                ardirectattenuationsample_a_valid(
                    art_gv,
                    ARATTENUATIONSAMPLE_MAIN_DIAGONAL( a0 )
                    );

            if ( ! ir )
                ART_ERRORHANDLING_WARNING( "invalid direct attenuation" );
        }
    }
    else
    {
        ir =
            ardirectattenuationsample_a_valid(
                art_gv,
                ARATTENUATIONSAMPLE_MAIN_DIAGONAL( a0 )
                );

        if ( ! ir )
            ART_ERRORHANDLING_WARNING( "invalid direct attenuation" );
    }

    return ir;
}

void arattenuationsample_a_debugprintf(
        const ART_GV            * art_gv,
        const ArAttenuationSample * a0
        )
{
    int number_of_colour_channels = 1;

    if ( ARATTENUATIONSAMPLE_FLUORESCENT( a0 ) )
        printf("Fluorescent attenuation\n");
    else
        printf("Nonfluorescent attenuation\n");

    ardirectattenuationsample_a_debugprintf(
        art_gv,
        ARATTENUATIONSAMPLE_MAIN_DIAGONAL( a0 )
        );

    if ( ARATTENUATIONSAMPLE_FLUORESCENT( a0 ) )
    {
        double  * m0;

        m0 =
            ALLOC_ARRAY(
                double,
                M_SQR(number_of_colour_channels)
                );

        int  c = 0;

        //   We build two quadratic 2D arrays from the information in
        //   the attenuation.

        for ( int i = 0; i < number_of_colour_channels; i++ )
            for ( int j = 0; j < number_of_colour_channels; j++ )
            {
                if ( j > i )
                    FM( m0, i, j ) = 0.0;

                if ( j == i )
                    FM( m0, i, j ) = ARATTENUATIONSAMPLE_MAIN_DIAGONAL_AI( a0 , i );
                if ( j < i )
                    FM( m0, i, j ) = ARATTENUATIONSAMPLE_CROSSTALK_XI( a0, c++ );
            }

        //   These arrays can then be used for a real matrix multiplication

        for( int i = 0; i < number_of_colour_channels; i++ )
        {
            for( int j = 0; j < number_of_colour_channels; j++ )
                printf("% 5.3f ", FM( m0, i, j ) );
            printf("\n");
        }

        FREE_ARRAY(m0);
    }

    fflush(stdout);
}

// ===========================================================================
