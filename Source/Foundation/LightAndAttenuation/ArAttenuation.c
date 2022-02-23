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

#define ART_MODULE_NAME     ArAttenuation

#include "ArAttenuation.h"
#include "FoundationAssertionMacros.h"

#define  YES  1
#define  NO   0

typedef struct ArAttenuation_GV
{
    ArAttenuation  * free_transmission;
    ArAttenuation  * total_blocker;
}
ArAttenuation_GV;

#define ARATTENUATION_GV                     art_gv->arattenuation_gv
#define ARATTENUATION_FREE_TRANSMISSION_GV   ARATTENUATION_GV->free_transmission
#define ARATTENUATION_TOTAL_BLOCKER_GV       ARATTENUATION_GV->total_blocker

ART_MODULE_INITIALISATION_FUNCTION
(
    ARATTENUATION_GV = ALLOC(ArAttenuation_GV);

    ARATTENUATION_FREE_TRANSMISSION_GV = NULL;
    ARATTENUATION_TOTAL_BLOCKER_GV     = NULL;
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    if ( ! LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE )
    {
        if ( ARATTENUATION_FREE_TRANSMISSION_GV )
            arattenuation_free( art_gv, ARATTENUATION_FREE_TRANSMISSION_GV );
        if ( ARATTENUATION_TOTAL_BLOCKER_GV )
            arattenuation_free( art_gv, ARATTENUATION_TOTAL_BLOCKER_GV );
    }

    FREE( ARATTENUATION_GV );
)

ART_LIGHT_AND_ATTENUATION_MODULE_INITIALISATION_FUNCTION
(
    if ( ! LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE )
    {
        ARATTENUATION_FREE_TRANSMISSION_GV =
            arattenuation_d_alloc_init(
                art_gv,
                1.0
                );

        ARATTENUATION_TOTAL_BLOCKER_GV =
            arattenuation_d_alloc_init(
                art_gv,
                0.0
                );
    }
    else
    {
        //   Just to make sure...

        ARATTENUATION_FREE_TRANSMISSION_GV = NULL;
        ARATTENUATION_TOTAL_BLOCKER_GV     = NULL;
    }
)

ART_LIGHT_AND_ATTENUATION_MODULE_SHUTDOWN_FUNCTION
(
    if ( ! LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE )
    {
        if ( ARATTENUATION_FREE_TRANSMISSION_GV )
            arattenuation_free( art_gv, ARATTENUATION_FREE_TRANSMISSION_GV );
        if ( ARATTENUATION_TOTAL_BLOCKER_GV )
            arattenuation_free( art_gv, ARATTENUATION_TOTAL_BLOCKER_GV );
    }
)

ArAttenuation * arattenuation_alloc(
        const ART_GV  * art_gv
        )
{
    ArAttenuation  * ar = ALLOC( ArAttenuation );

    ar->next = NULL;

    ARATTENUATION_CROSSTALK( ar ) =
        arcrosstalk_alloc(
            art_gv
            );

    ARATTENUATION_MAIN_DIAGONAL( ar ) =
        ardirectattenuation_alloc(
            art_gv
            );

    ARATTENUATION_INITIALISED( ar ) = NO;

    ASSERT_ALLOCATED_ATTENUATION( ar )

    return ar;
}

void arattenuation_free(
        const ART_GV         * art_gv,
              ArAttenuation  * ar
        )
{
    ASSERT_ALLOCATED_ATTENUATION( ar )

    arcrosstalk_free(
        art_gv,
        ARATTENUATION_CROSSTALK( ar )
        );

    ardirectattenuation_free(
        art_gv,
        ARATTENUATION_MAIN_DIAGONAL( ar )
        );

    FREE( ar );
}

ArAttenuation * arattenuation_d_alloc_init_depolarising(
        const ART_GV  * art_gv,
        const double    d0
        )
{
    ASSERT_LIGHT_SUBSYSTEM_NOT_IN_POLARISATION_MODE

    ASSERT_NONNEGATIVE_DOUBLE( d0 )

    ArAttenuation  * ar = arattenuation_alloc( art_gv );

    ASSERT_ALLOCATED_ATTENUATION( ar )

    ARATTENUATION_FLUORESCENT( ar ) = NO;

    ardirectattenuation_d_init_a(
        art_gv,
        d0,
        ARATTENUATION_MAIN_DIAGONAL( ar )
        );

    ARATTENUATION_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION( ar )

    return ar;
}

ArAttenuation * arattenuation_d_alloc_init(
        const ART_GV  * art_gv,
        const double    d0
        )
{
    ASSERT_LIGHT_SUBSYSTEM_NOT_IN_POLARISATION_MODE

    ASSERT_NONNEGATIVE_DOUBLE( d0 )

    ArAttenuation  * ar = arattenuation_alloc( art_gv );

    ASSERT_ALLOCATED_ATTENUATION( ar )

    ARATTENUATION_FLUORESCENT( ar ) = NO;

    ardirectattenuation_d_init_a(
        art_gv,
        d0,
        ARATTENUATION_MAIN_DIAGONAL( ar )
        );

    ARATTENUATION_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION( ar )

    return ar;
}

ArAttenuation * arattenuation_a_alloc_init(
        const ART_GV         * art_gv,
        const ArAttenuation  * a0
        )
{
    ASSERT_VALID_ATTENUATION( a0 )

    ArAttenuation  * ar = arattenuation_alloc( art_gv );

    ASSERT_ALLOCATED_ATTENUATION( ar )

    arattenuation_a_init_a(
        art_gv,
        a0,
        ar
        );

    ARATTENUATION_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION( ar )
    ASSERT_IDENTICAL_REFERENCE_FRAMES_AA( a0, ar )

    return ar;
}

ArAttenuation const * arattenuation_total(
        const ART_GV  * art_gv
        )
{
    ASSERT_LIGHT_SUBSYSTEM_NOT_IN_POLARISATION_MODE

    ASSERT_VALID_ATTENUATION( ARATTENUATION_TOTAL_BLOCKER_GV )

    return
        ARATTENUATION_TOTAL_BLOCKER_GV;
}

ArAttenuation const * arattenuation_none(
        const ART_GV  * art_gv
        )
{
    ASSERT_LIGHT_SUBSYSTEM_NOT_IN_POLARISATION_MODE

    ASSERT_VALID_ATTENUATION( ARATTENUATION_FREE_TRANSMISSION_GV )

    return
        ARATTENUATION_FREE_TRANSMISSION_GV;
}

void arattenuation_d_init_a(
        const ART_GV         * art_gv,
        const double           d0,
              ArAttenuation  * ar
        )
{
    ASSERT_LIGHT_SUBSYSTEM_NOT_IN_POLARISATION_MODE

    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_ALLOCATED_ATTENUATION( ar )

    ARATTENUATION_FLUORESCENT( ar ) = NO;

    ardirectattenuation_d_init_a(
        art_gv,
        d0,
        ARATTENUATION_MAIN_DIAGONAL( ar )
        );

    ARATTENUATION_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION( ar )
}

void arattenuation_s_init_a(
        const ART_GV         * art_gv,
        const ArSpectrum       * c0,
              ArAttenuation  * ar
        )
{
    ASSERT_LIGHT_SUBSYSTEM_NOT_IN_POLARISATION_MODE

    ASSERT_VALID_SPECTRUM( c0 )
    ASSERT_ALLOCATED_ATTENUATION( ar )

    ARATTENUATION_FLUORESCENT( ar ) = NO;

    ardirectattenuation_s_init_a(
        art_gv,
        c0,
        ARATTENUATION_MAIN_DIAGONAL( ar )
        );

    ARATTENUATION_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION( ar )
}

void arattenuation_sxrr_init_depolarising_a(
        const ART_GV            * art_gv,
        const ArSpectrum          * c0,
        const ArCrosstalk       * x0,
        const ArReferenceFrame  * r0,
        const ArReferenceFrame  * r1,
              ArAttenuation     * ar
        )
{
    ASSERT_LIGHT_SUBSYSTEM_IN_POLARISATION_MODE

    ASSERT_VALID_SPECTRUM( c0 )
    ASSERT_VALID_CROSSTALK( x0 )
    ASSERT_VALID_REFERENCE_FRAME( r0 )
    ASSERT_VALID_REFERENCE_FRAME( r1 )
    ASSERT_ALLOCATED_ATTENUATION( ar )

    ARATTENUATION_FLUORESCENT( ar ) = YES;

    arcrosstalk_x_init_x(
        art_gv,
        x0,
        ARATTENUATION_CROSSTALK( ar )
        );

    ardirectattenuation_srr_init_depolarising_a(
        art_gv,
        c0,
        r0,
        r1,
        ARATTENUATION_MAIN_DIAGONAL( ar )
        );

    ARATTENUATION_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION( ar )
}

void arattenuation_sxrr_init_nonpolarising_a(
        const ART_GV            * art_gv,
        const ArSpectrum          * c0,
        const ArCrosstalk       * x0,
        const ArReferenceFrame  * r0,
        const ArReferenceFrame  * r1,
              ArAttenuation     * ar
        )
{
    ASSERT_LIGHT_SUBSYSTEM_IN_POLARISATION_MODE

    ASSERT_VALID_SPECTRUM( c0 )
    ASSERT_VALID_CROSSTALK( x0 )
    ASSERT_VALID_REFERENCE_FRAME( r0 )
    ASSERT_VALID_REFERENCE_FRAME( r1 )
    ASSERT_ALLOCATED_ATTENUATION( ar )

    ARATTENUATION_FLUORESCENT( ar ) = YES;

    arcrosstalk_x_init_x(
        art_gv,
        x0,
        ARATTENUATION_CROSSTALK( ar )
        );

    ardirectattenuation_srr_init_nonpolarising_a(
        art_gv,
        c0,
        r0,
        r1,
        ARATTENUATION_MAIN_DIAGONAL( ar )
        );

    ARATTENUATION_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION( ar )
}

void arattenuation_srr_init_depolarising_a(
        const ART_GV            * art_gv,
        const ArSpectrum          * c0,
        const ArReferenceFrame  * r0,
        const ArReferenceFrame  * r1,
              ArAttenuation     * ar
        )
{
    ASSERT_LIGHT_SUBSYSTEM_IN_POLARISATION_MODE

    ASSERT_VALID_SPECTRUM( c0 )
    ASSERT_VALID_REFERENCE_FRAME( r0 )
    ASSERT_VALID_REFERENCE_FRAME( r1 )
    ASSERT_ALLOCATED_ATTENUATION( ar )

    ARATTENUATION_FLUORESCENT( ar ) = NO;

    ardirectattenuation_srr_init_depolarising_a(
        art_gv,
        c0,
        r0,
        r1,
        ARATTENUATION_MAIN_DIAGONAL( ar )
        );

    ARATTENUATION_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION( ar )
}

void arattenuation_srr_init_nonpolarising_a(
        const ART_GV            * art_gv,
        const ArSpectrum          * c0,
        const ArReferenceFrame  * r0,
        const ArReferenceFrame  * r1,
              ArAttenuation     * ar
        )
{
    ASSERT_LIGHT_SUBSYSTEM_IN_POLARISATION_MODE

    ASSERT_VALID_SPECTRUM( c0 )
    ASSERT_VALID_REFERENCE_FRAME( r0 )
    ASSERT_VALID_REFERENCE_FRAME( r1 )
    ASSERT_ALLOCATED_ATTENUATION( ar )

    ARATTENUATION_FLUORESCENT( ar ) = NO;

    ardirectattenuation_srr_init_nonpolarising_a(
        art_gv,
        c0,
        r0,
        r1,
        ARATTENUATION_MAIN_DIAGONAL( ar )
        );

    ARATTENUATION_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION( ar )
}

void arattenuation_arr_init_depolarising_a(
        const ART_GV            * art_gv,
        const ArAttenuation     * a0,
        const ArReferenceFrame  * r0,
        const ArReferenceFrame  * r1,
              ArAttenuation     * ar
        )
{
    if ( ! ARATTENUATION_INITIALISED( a0 ) )
    {
        ARATTENUATION_INITIALISED( ar ) = NO;
        return;
    }

    ASSERT_ALLOCATED_ATTENUATION( ar )
    ASSERT_VALID_ATTENUATION( a0 )

    ARATTENUATION_FLUORESCENT( ar ) = ARATTENUATION_FLUORESCENT( a0 );

    if ( ARATTENUATION_FLUORESCENT( ar ) )
        arcrosstalk_x_init_x(
            art_gv,
            ARATTENUATION_CROSSTALK( a0 ),
            ARATTENUATION_CROSSTALK( ar )
            );

    ArSpectrum  * temp_col = spc_alloc(art_gv);

    ardirectattenuation_a_init_s(
          art_gv,
          ARATTENUATION_MAIN_DIAGONAL( a0 ),
          temp_col
        );

    ardirectattenuation_srr_init_depolarising_a(
          art_gv,
          temp_col,
          r0,
          r1,
          ARATTENUATION_MAIN_DIAGONAL( ar )
        );

    spc_free( art_gv, temp_col );

    ARATTENUATION_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION( ar )
    ASSERT_IDENTICAL_REFERENCE_FRAMES_AA( a0, ar )
}

void arattenuation_arr_init_nonpolarising_a(
        const ART_GV            * art_gv,
        const ArAttenuation     * a0,
        const ArReferenceFrame  * r0,
        const ArReferenceFrame  * r1,
              ArAttenuation     * ar
        )
{
    if ( ! ARATTENUATION_INITIALISED( a0 ) )
    {
        ARATTENUATION_INITIALISED( ar ) = NO;
        return;
    }

    ASSERT_ALLOCATED_ATTENUATION( ar )
    ASSERT_VALID_ATTENUATION( a0 )

    ARATTENUATION_FLUORESCENT( ar ) = ARATTENUATION_FLUORESCENT( a0 );

    if ( ARATTENUATION_FLUORESCENT( ar ) )
        arcrosstalk_x_init_x(
            art_gv,
            ARATTENUATION_CROSSTALK( a0 ),
            ARATTENUATION_CROSSTALK( ar )
            );

    ArSpectrum  * temp_col = spc_alloc(art_gv);

    ardirectattenuation_a_init_s(
          art_gv,
          ARATTENUATION_MAIN_DIAGONAL( a0 ),
          temp_col
        );

    ardirectattenuation_srr_init_nonpolarising_a(
          art_gv,
          temp_col,
          r0,
          r1,
          ARATTENUATION_MAIN_DIAGONAL( ar )
        );

    spc_free( art_gv, temp_col );

    ARATTENUATION_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION( ar )
    ASSERT_IDENTICAL_REFERENCE_FRAMES_AA( a0, ar )
}

void  arattenuation_a_init_c(
        const ART_GV         * art_gv,
        const ArAttenuation  * a0,
              ArSpectrum       * cr
        )
{
    ASSERT_VALID_ATTENUATION( a0 )
    ASSERT_ALLOCATED_SPECTRUM( cr )

    ardirectattenuation_a_init_s(
        art_gv,
        ARATTENUATION_MAIN_DIAGONAL( a0 ),
        cr
        );

    ASSERT_VALID_SPECTRUM( cr )
}

void  arattenuation_a_init_a(
        const ART_GV         * art_gv,
        const ArAttenuation  * a0,
              ArAttenuation  * ar
        )
{
    if ( ! ARATTENUATION_INITIALISED( a0 ) )
    {
        ARATTENUATION_INITIALISED( ar ) = NO;
        return;
    }

    ASSERT_ALLOCATED_ATTENUATION( ar )
    ASSERT_VALID_ATTENUATION( a0 )

    ARATTENUATION_FLUORESCENT( ar ) = ARATTENUATION_FLUORESCENT( a0 );

    if ( ARATTENUATION_FLUORESCENT( ar ) )
        arcrosstalk_x_init_x(
            art_gv,
            ARATTENUATION_CROSSTALK( a0 ),
            ARATTENUATION_CROSSTALK( ar )
            );

    ardirectattenuation_a_init_a(
        art_gv,
        ARATTENUATION_MAIN_DIAGONAL( a0 ),
        ARATTENUATION_MAIN_DIAGONAL( ar )
        );

    ARATTENUATION_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION( ar )
    ASSERT_IDENTICAL_REFERENCE_FRAMES_AA( a0, ar )
}

void arattenuation_mm_rr_init_polarising_a(
        const ART_GV            * art_gv,
        const ArMuellerMatrix   * m0,
        const ArReferenceFrame  * r0,
        const ArReferenceFrame  * r1,
              ArAttenuation     * ar
        )
{
    ASSERT_LIGHT_SUBSYSTEM_IN_POLARISATION_MODE

    ASSERT_VALID_MUELLER_MATRIX( m0 )
    ASSERT_VALID_REFERENCE_FRAME( r0 )
    ASSERT_VALID_REFERENCE_FRAME( r1 )
    ASSERT_ALLOCATED_ATTENUATION( ar )

    ARATTENUATION_FLUORESCENT( ar ) = NO;

    ardirectattenuation_mm_rr_init_polarising_a(
        art_gv,
        m0,
        r0,
        r1,
        ARATTENUATION_MAIN_DIAGONAL( ar )
        );

    ARATTENUATION_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION( ar )
}

void arattenuation_ddrr_init_linear_polariser_a(
        const ART_GV           * art_gv,
        const double             d0,
        const double             d1,
        const ArReferenceFrame  * r0,
        const ArReferenceFrame  * r1,
              ArAttenuation     * ar
        )
{
    ASSERT_LIGHT_SUBSYSTEM_IN_POLARISATION_MODE

    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_NONNEGATIVE_DOUBLE( d1 )
    ASSERT_ALLOCATED_ATTENUATION( ar )

    ARATTENUATION_FLUORESCENT( ar ) = NO;

    ardirectattenuation_ddrr_init_linear_polariser_a(
        art_gv,
        d0,
        d1,
        r0,
        r1,
        ARATTENUATION_MAIN_DIAGONAL( ar )
        );

    ARATTENUATION_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION( ar )
}

void arattenuation_sx_init_a(
        const ART_GV         * art_gv,
        const ArSpectrum       * c0,
        const ArCrosstalk    * x0,
              ArAttenuation  * ar
        )
{
    ASSERT_VALID_SPECTRUM( c0 )
    ASSERT_VALID_CROSSTALK( x0 )
    ASSERT_ALLOCATED_ATTENUATION( ar )

    ARATTENUATION_FLUORESCENT( ar ) = YES;

    arcrosstalk_x_init_x(
        art_gv,
        x0,
        ARATTENUATION_CROSSTALK( ar )
        );

    ardirectattenuation_s_init_a(
        art_gv,
        c0,
        ARATTENUATION_MAIN_DIAGONAL( ar )
        );

    ARATTENUATION_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION( ar )
}

void arattenuation_a_deactivate(
        const ART_GV         * art_gv,
              ArAttenuation  * ar
        )
{
    (void) art_gv;
    
    ARATTENUATION_INITIALISED(ar) = NO;
}

unsigned int  arattenuation_a_polarising(
        const ART_GV         * art_gv,
        const ArAttenuation  * a0
        )
{
    return
        ardirectattenuation_a_polarising(
            art_gv,
            ARATTENUATION_MAIN_DIAGONAL( a0 )
            );
}

ArReferenceFrame const * arattenuation_a_entry_rf(
        const ART_GV         * art_gv,
        const ArAttenuation  * a0
        )
{
    return
        ardirectattenuation_a_entry_rf(
            art_gv,
            ARATTENUATION_MAIN_DIAGONAL( a0 )
            );
}

ArReferenceFrame const * arattenuation_a_exit_rf(
        const ART_GV         * art_gv,
        const ArAttenuation  * a0
        )
{
    return
        ardirectattenuation_a_exit_rf(
            art_gv,
            ARATTENUATION_MAIN_DIAGONAL( a0 )
            );
}

void arattenuation_a_realign_to_coaxial_exit_rf_a(
        const ART_GV            * art_gv,
        const ArAttenuation     * a0,
        const ArReferenceFrame  * r0,
              ArAttenuation     * ar
        )
{
    ASSERT_VALID_ATTENUATION( a0 )
    ASSERT_VALID_REFERENCE_FRAME( r0 )
    ASSERT_COAXIAL_EXIT_REFERENCE_FRAME_RA( r0, a0, 3.0 DEGREES)
    ASSERT_ALLOCATED_ATTENUATION( ar )

    ARATTENUATION_FLUORESCENT( ar ) = ARATTENUATION_FLUORESCENT( a0 );

    ARATTENUATION_CROSSTALK( ar )   = ARATTENUATION_CROSSTALK( a0 );

    ardirectattenuation_a_realign_to_coaxial_exit_rf_a(
        art_gv,
        ARATTENUATION_MAIN_DIAGONAL( a0 ),
        r0,
        ARATTENUATION_MAIN_DIAGONAL( ar )
        );

    ASSERT_VALID_ATTENUATION( ar )
    ASSERT_COAXIAL_EXIT_REFERENCE_FRAME_RA( r0, ar, 3.0 DEGREES)
}

double arattenuation_a_norm(
        const ART_GV         * art_gv,
        const ArAttenuation  * a0
        )
{
    ASSERT_VALID_ATTENUATION( a0 )

    return
        ardirectattenuation_a_norm(
            art_gv,
            ARATTENUATION_MAIN_DIAGONAL( a0 )
            );
}

void arattenuation_inv_a(
        const ART_GV         * art_gv,
              ArAttenuation  * ar
        )
{
    ASSERT_VALID_ATTENUATION( ar )
    STORE_ATTENUATION_REFERENCE_FRAMES( ar )

    if ( ARATTENUATION_FLUORESCENT( ar ) )
        arcrosstalk_inv_x(
            art_gv,
            ARATTENUATION_CROSSTALK( ar )
            );

    ardirectattenuation_inv_a(
        art_gv,
        ARATTENUATION_MAIN_DIAGONAL( ar )
        );

    ASSERT_VALID_ATTENUATION( ar )
    ASSERT_IDENTICAL_REFERENCE_FRAMES_SA( ar )
}

void arattenuation_a_inv_a(
        const ART_GV         * art_gv,
        const ArAttenuation  * a0,
              ArAttenuation  * ar
        )
{
    if ( ! ARATTENUATION_INITIALISED( a0 ) )
    {
        ARATTENUATION_INITIALISED( ar ) = NO;
        return;
    }

    ASSERT_VALID_ATTENUATION( a0 )
    ASSERT_ALLOCATED_ATTENUATION( ar )

    if ( ARATTENUATION_FLUORESCENT( a0 ) )
    {
        arcrosstalk_x_inv_x(
            art_gv,
            ARATTENUATION_CROSSTALK( a0 ),
            ARATTENUATION_CROSSTALK( ar )
            );

        ARATTENUATION_FLUORESCENT( ar ) = YES;
    }
    else
        ARATTENUATION_FLUORESCENT( ar ) = NO;

    ardirectattenuation_a_inv_a(
        art_gv,
        ARATTENUATION_MAIN_DIAGONAL( a0 ),
        ARATTENUATION_MAIN_DIAGONAL( ar )
        );

    ARATTENUATION_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION( ar )
}

double arattenuation_a_max(
        const ART_GV         * art_gv,
        const ArAttenuation  * a0
        )
{
    ASSERT_VALID_ATTENUATION( a0 )

    if ( ARATTENUATION_FLUORESCENT( a0 ) )
    {
        return
            M_MAX(
                ardirectattenuation_a_max( art_gv, ARATTENUATION_MAIN_DIAGONAL( a0 ) ),
                arcrosstalk_x_max( art_gv, ARATTENUATION_CROSSTALK( a0 ) )
                );
    }
    else
        return
            ardirectattenuation_a_max( art_gv, ARATTENUATION_MAIN_DIAGONAL(a0) );
}

void arattenuation_aa_max_a(
        const ART_GV         * art_gv,
        const ArAttenuation  * a0,
        const ArAttenuation  * a1,
              ArAttenuation  * ar
        )
{
    (void) art_gv;
    (void) a0;
    (void) a1;
    (void) ar;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

double arattenuation_a_min(
        const ART_GV    * art_gv,
        const ArAttenuation  * a0
        )
{
    ASSERT_VALID_ATTENUATION( a0 )

    if ( ARATTENUATION_FLUORESCENT( a0 ) )
    {
        return
            M_MIN(
                ardirectattenuation_a_min( art_gv, ARATTENUATION_MAIN_DIAGONAL( a0 ) ),
                arcrosstalk_x_min( art_gv, ARATTENUATION_CROSSTALK( a0 ) )
                );
    }
    else
        return
            ardirectattenuation_a_min( art_gv, ARATTENUATION_MAIN_DIAGONAL(a0) );
}

void arattenuation_aa_min_a(
        const ART_GV         * art_gv,
        const ArAttenuation  * a0,
        const ArAttenuation  * a1,
              ArAttenuation  * ar
        )
{
    (void) art_gv;
    (void) a0;
    (void) a1;
    (void) ar;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

double arattenuation_a_avg(
        const ART_GV         * art_gv,
        const ArAttenuation  * a0
        )
{
    ASSERT_VALID_ATTENUATION( a0 )

    if ( ARATTENUATION_FLUORESCENT( a0 ) )
    {
        return
            M_MAX(
                ardirectattenuation_a_avg( art_gv, ARATTENUATION_MAIN_DIAGONAL( a0 ) ),
                arcrosstalk_x_avg( art_gv, ARATTENUATION_CROSSTALK( a0 ) )
                );
    }
    else
        return
            ardirectattenuation_a_avg( art_gv, ARATTENUATION_MAIN_DIAGONAL(a0) );
}

double arattenuation_aa_maxdiff(
        const ART_GV         * art_gv,
        const ArAttenuation  * a0,
        const ArAttenuation  * a1
        )
{
    (void) art_gv;
    (void) a0;
    (void) a1;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return 0.0;
}

void arattenuation_a_add_a(
        const ART_GV         * art_gv,
        const ArAttenuation  * a0,
              ArAttenuation  * ar
        )
{
    if (    ! ARATTENUATION_INITIALISED( a0 )
         || ! ARATTENUATION_INITIALISED( ar ))
    {
        ARATTENUATION_INITIALISED( ar ) = NO;
        return;
    }

    ASSERT_VALID_ATTENUATION( a0 )
    ASSERT_VALID_ATTENUATION( ar )

    if ( ARATTENUATION_FLUORESCENT( a0 ) )
    {
        if ( ARATTENUATION_FLUORESCENT( ar ) )
        {
            arcrosstalk_x_add_x(
                art_gv,
                ARATTENUATION_CROSSTALK( a0 ),
                ARATTENUATION_CROSSTALK( ar )
                );
        }
        else
        {
            ARATTENUATION_FLUORESCENT( ar ) = YES;
            arcrosstalk_x_init_x(
                art_gv,
                ARATTENUATION_CROSSTALK( a0 ),
                ARATTENUATION_CROSSTALK( ar )
                );
        }
    }

    ardirectattenuation_a_add_a(
        art_gv,
        ARATTENUATION_MAIN_DIAGONAL( a0 ),
        ARATTENUATION_MAIN_DIAGONAL( ar )
        );

    ARATTENUATION_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION( ar )
}

void arattenuation_aa_add_a(
        const ART_GV         * art_gv,
        const ArAttenuation  * a0,
        const ArAttenuation  * a1,
              ArAttenuation  * ar
        )
{
    if (    ! ARATTENUATION_INITIALISED( a0 )
         || ! ARATTENUATION_INITIALISED( a1 ))
    {
        ARATTENUATION_INITIALISED( ar ) = NO;
        return;
    }

    ASSERT_VALID_ATTENUATION( a0 )
    ASSERT_VALID_ATTENUATION( a1 )
    ASSERT_ALLOCATED_ATTENUATION( ar )

    if ( ARATTENUATION_FLUORESCENT( a0 ) )
    {
        if ( ARATTENUATION_FLUORESCENT( a1 ) )
            arcrosstalk_xx_add_x(
                art_gv,
                ARATTENUATION_CROSSTALK( a0 ),
                ARATTENUATION_CROSSTALK( a1 ),
                ARATTENUATION_CROSSTALK( ar )
                );
        else
            arcrosstalk_x_init_x(
                art_gv,
                ARATTENUATION_CROSSTALK( a0 ),
                ARATTENUATION_CROSSTALK( ar )
                );

        ARATTENUATION_FLUORESCENT( ar ) = YES;
    }
    else
    {
        if ( ARATTENUATION_FLUORESCENT( a1 ) )
        {
            arcrosstalk_x_init_x(
                art_gv,
                ARATTENUATION_CROSSTALK( a1 ),
                ARATTENUATION_CROSSTALK( ar )
                );

            ARATTENUATION_FLUORESCENT( ar ) = YES;
        }
        else
            ARATTENUATION_FLUORESCENT( ar ) = NO;
    }

    ardirectattenuation_aa_add_a(
        art_gv,
        ARATTENUATION_MAIN_DIAGONAL( a0 ),
        ARATTENUATION_MAIN_DIAGONAL( a1 ),
        ARATTENUATION_MAIN_DIAGONAL( ar )
        );

    ARATTENUATION_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION( ar )
}

void arattenuation_aa_sub_a(
        const ART_GV         * art_gv,
        const ArAttenuation  * a0,
        const ArAttenuation  * a1,
              ArAttenuation  * ar
        )
{
    if (    ! ARATTENUATION_INITIALISED( a0 )
         || ! ARATTENUATION_INITIALISED( a1 ))
    {
        ARATTENUATION_INITIALISED( ar ) = NO;
        return;
    }

    ASSERT_VALID_ATTENUATION( a0 )
    ASSERT_VALID_ATTENUATION( a1 )
    ASSERT_ALLOCATED_ATTENUATION( ar )

    if ( ARATTENUATION_FLUORESCENT( a0 ) )
    {
        if ( ARATTENUATION_FLUORESCENT( a1 ) )
            arcrosstalk_xx_sub_x(
                art_gv,
                ARATTENUATION_CROSSTALK( a0 ),
                ARATTENUATION_CROSSTALK( a1 ),
                ARATTENUATION_CROSSTALK( ar )
                );
        else
            arcrosstalk_dx_mul_x(
                art_gv,
                -1.0,
                ARATTENUATION_CROSSTALK( a0 ),
                ARATTENUATION_CROSSTALK( ar )
                );

        ARATTENUATION_FLUORESCENT( ar ) = YES;
    }
    else
    {
        if ( ARATTENUATION_FLUORESCENT( a1 ) )
        {
            arcrosstalk_dx_mul_x(
                art_gv,
                -1.0,
                ARATTENUATION_CROSSTALK( a1 ),
                ARATTENUATION_CROSSTALK( ar )
                );

            ARATTENUATION_FLUORESCENT( ar ) = YES;
        }
        else
            ARATTENUATION_FLUORESCENT( ar ) = NO;
    }

    ardirectattenuation_aa_sub_a(
        art_gv,
        ARATTENUATION_MAIN_DIAGONAL( a0 ),
        ARATTENUATION_MAIN_DIAGONAL( a1 ),
        ARATTENUATION_MAIN_DIAGONAL( ar )
        );

    ARATTENUATION_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION( ar )
}


void arattenuation_d_mul_a(
        const ART_GV         * art_gv,
        const double           d0,
              ArAttenuation  * ar
        )
{
    if ( ! ARATTENUATION_INITIALISED( ar ) )
        return;

    ASSERT_NONNEGATIVE_FINITE_DOUBLE( d0 )
    ASSERT_VALID_ATTENUATION( ar )

    if ( ARATTENUATION_FLUORESCENT( ar ) )
        arcrosstalk_d_mul_x(
            art_gv,
            d0,
            ARATTENUATION_CROSSTALK( ar )
            );

    ardirectattenuation_d_mul_a(
        art_gv,
        d0,
        ARATTENUATION_MAIN_DIAGONAL( ar )
        );

    ASSERT_VALID_ATTENUATION( ar )
}


void arattenuation_da_mul_a(
        const ART_GV         * art_gv,
        const double           d0,
        const ArAttenuation  * a0,
              ArAttenuation  * ar
        )
{
    if ( ! ARATTENUATION_INITIALISED( a0 ) )
    {
        ARATTENUATION_INITIALISED( ar ) = NO;
        return;
    }

    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_VALID_ATTENUATION( a0 )
    ASSERT_ALLOCATED_ATTENUATION( ar )

    if ( ARATTENUATION_FLUORESCENT( a0 ) )
    {
        arcrosstalk_dx_mul_x(
            art_gv,
            d0,
            ARATTENUATION_CROSSTALK( a0 ),
            ARATTENUATION_CROSSTALK( ar )
            );

        ARATTENUATION_FLUORESCENT( ar ) = YES;
    }
    else
        ARATTENUATION_FLUORESCENT( ar ) = NO;

    ardirectattenuation_da_mul_a(
        art_gv,
        d0,
        ARATTENUATION_MAIN_DIAGONAL( a0 ),
        ARATTENUATION_MAIN_DIAGONAL( ar )
        );

    ARATTENUATION_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION( ar )
    ASSERT_IDENTICAL_REFERENCE_FRAMES_AA( a0, ar )
}


void arattenuation_a_mul_a(
        const ART_GV         * art_gv,
        const ArAttenuation  * a0,
              ArAttenuation  * ar
        )
{
    if ( ARATTENUATION_INITIALISED( a0 ) )
    {
        ASSERT_VALID_ATTENUATION( a0 )
        ASSERT_VALID_ATTENUATION( ar )
        ASSERT_REFERENCE_FRAME_COMPATIBILITY_AA( a0, ar, 3.0 DEGREES)

        ArAttenuation  * temp = arattenuation_alloc( art_gv );

        if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE )
        {
            arattenuation_srr_init_nonpolarising_a(
                art_gv,
                spc_zero(art_gv),
                arattenuation_a_entry_rf(art_gv,a0),
                arattenuation_a_exit_rf(art_gv,ar),
                temp
                );
        }
        else
        {
            arattenuation_a_init_a(
                art_gv,
                arattenuation_total(art_gv),
                temp
                );
        }

        arattenuation_aa_mul_a( art_gv, a0, ar, temp );

        ASSERT_VALID_ATTENUATION( temp )
        ASSERT_COAXIAL_ENTRY_REFERENCE_FRAME_AA( a0, temp, 3.0 DEGREES)
        ASSERT_COAXIAL_EXIT_REFERENCE_FRAME_AA( ar, temp, 3.0 DEGREES)

        arattenuation_a_init_a(
            art_gv,
            temp,
            ar
            );

        arattenuation_free( art_gv, temp );
    }

    //   else do nothing - if a0 is uninitialised, ar stays the way it is

    ASSERT_VALID_ATTENUATION( ar )
}

#define FM(__m,__x,__y)     (__m)[number_of_colour_channels*(__y)+(__x)]

void arattenuation_aa_mul_a(
        const ART_GV         * art_gv,
        const ArAttenuation  * a0,
        const ArAttenuation  * a1,
              ArAttenuation  * ar
        )
{
    if ( ARATTENUATION_INITIALISED( a0 ) )
    {
        ASSERT_VALID_ATTENUATION( a0 )
        ASSERT_VALID_ATTENUATION( a1 )
        ASSERT_ALLOCATED_ATTENUATION( ar )
        ASSERT_REFERENCE_FRAME_COMPATIBILITY_AA( a0, a1, 3.0 DEGREES)

        if (   ARATTENUATION_FLUORESCENT( a0 )
            || ARATTENUATION_FLUORESCENT( a1 ) )
        {
            // do something unspeakable here

            // this is just as unspeakable - but at least it works

            int number_of_colour_channels = spc_channels( art_gv );

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
                        FM( m0, i, j ) = ARATTENUATION_MAIN_DIAGONAL_AI( a0 , i );
                        FM( m1, i, j ) = ARATTENUATION_MAIN_DIAGONAL_AI( a1 , i );
                    }

                    if ( j < i )
                    {
                        if ( ARATTENUATION_FLUORESCENT( a0 ) )
                            FM( m0, i, j ) = ARATTENUATION_CROSSTALK_XI( a0, c );
                        else
                            FM( m0, i, j ) = 0.0;

                        if ( ARATTENUATION_FLUORESCENT( a1 ) )
                            FM( m1, i, j ) = ARATTENUATION_CROSSTALK_XI( a1, c );
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
                        ARATTENUATION_ATTENUATION_SET_AID( ar, i, FM( mr, i, j ) );

                    if ( j < i )
                        ARATTENUATION_CROSSTALK_XI( ar, c++ ) = FM( mr, i, j );
                 }

            FREE_ARRAY(m0);
            FREE_ARRAY(m1);
            FREE_ARRAY(mr);

            ARATTENUATION_FLUORESCENT( ar ) = YES;
        }
        else
        {
            ardirectattenuation_aa_mul_a(
                art_gv,
                ARATTENUATION_MAIN_DIAGONAL( a0 ),
                ARATTENUATION_MAIN_DIAGONAL( a1 ),
                ARATTENUATION_MAIN_DIAGONAL( ar )
                );

            arcrosstalk_d_init_x(
                art_gv,
                0.0,
                ARATTENUATION_CROSSTALK( ar )
                );

            ARATTENUATION_FLUORESCENT( ar ) = NO;
        }

        ASSERT_VALID_ATTENUATION( ar )
        ASSERT_COAXIAL_ENTRY_REFERENCE_FRAME_AA( a0, ar, 3.0 DEGREES)
        ASSERT_COAXIAL_EXIT_REFERENCE_FRAME_AA( a1, ar, 3.0 DEGREES)
    }
    else
    {
        //   If a0 is uninitialised, ar is simply a copy of a1.

        ASSERT_VALID_ATTENUATION( a1 )
        ASSERT_ALLOCATED_ATTENUATION( ar )

        arattenuation_a_init_a(
            art_gv,
            a1,
            ar
            );

        ASSERT_VALID_ATTENUATION( ar )
    }
}

void arattenuation_d_div_a(
        const ART_GV         * art_gv,
        const double           d0,
              ArAttenuation  * ar
        )
{
    if ( d0 > 0.0 )
    {
        if ( ARATTENUATION_FLUORESCENT( ar ) )
            arcrosstalk_d_mul_x(
                art_gv,
                1.0 / d0,
                ARATTENUATION_CROSSTALK( ar )
                );

        ardirectattenuation_d_mul_a(
            art_gv,
            1.0 / d0,
            ARATTENUATION_MAIN_DIAGONAL( ar )
            );
    }
}

void arattenuation_da_div_a(
        const ART_GV         * art_gv,
        const double           d0,
        const ArAttenuation  * a0,
              ArAttenuation  * ar
        )
{
    if ( ! ARATTENUATION_INITIALISED( a0 ) )
    {
        ARATTENUATION_INITIALISED( ar ) = NO;
        return;
    }

    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_VALID_ATTENUATION( a0 )
    ASSERT_ALLOCATED_ATTENUATION( ar )

    if ( d0 > 0.0 )
    {
        if ( ARATTENUATION_FLUORESCENT( a0 ) )
        {
            arcrosstalk_dx_mul_x(
                art_gv,
                1.0 / d0,
                ARATTENUATION_CROSSTALK( a0 ),
                ARATTENUATION_CROSSTALK( ar )
                );
            ARATTENUATION_FLUORESCENT( ar ) = YES;
        }
        else
            ARATTENUATION_FLUORESCENT( ar ) = NO;

        ardirectattenuation_da_mul_a(
            art_gv,
            1.0 / d0,
            ARATTENUATION_MAIN_DIAGONAL( a0 ),
            ARATTENUATION_MAIN_DIAGONAL( ar )
            );
    }

    ARATTENUATION_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION( ar )
    ASSERT_IDENTICAL_REFERENCE_FRAMES_AA( a0, ar )
}

void arattenuation_da_pow_a(
        const ART_GV         * art_gv,
        const double           d0,
        const ArAttenuation  * a0,
              ArAttenuation  * ar
        )
{
    if ( ! ARATTENUATION_INITIALISED( a0 ) )
    {
        ARATTENUATION_INITIALISED( ar ) = NO;
        return;
    }

    ASSERT_VALID_ATTENUATION( a0 )
    ASSERT_ALLOCATED_ATTENUATION( ar )

    if ( ARATTENUATION_FLUORESCENT( a0 ) )
    {
        arcrosstalk_dx_pow_x(
            art_gv,
            d0,
            ARATTENUATION_CROSSTALK( a0 ),
            ARATTENUATION_CROSSTALK( ar )
            );

        ARATTENUATION_FLUORESCENT( ar ) = YES;
    }
    else
        ARATTENUATION_FLUORESCENT( ar ) = NO;

    ardirectattenuation_da_pow_a(
        art_gv,
        d0,
        ARATTENUATION_MAIN_DIAGONAL( a0 ),
        ARATTENUATION_MAIN_DIAGONAL( ar )
        );

    ARATTENUATION_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION( ar )
    ASSERT_IDENTICAL_REFERENCE_FRAMES_AA( a0, ar )
}

void arattenuation_da_negpow_a(
        const ART_GV         * art_gv,
        const double           d0,
        const ArAttenuation  * a0,
              ArAttenuation  * ar
        )
{
    if ( ! ARATTENUATION_INITIALISED( a0 ) )
    {
        ARATTENUATION_INITIALISED( ar ) = NO;
        return;
    }

    ASSERT_VALID_ATTENUATION( a0 )
    ASSERT_ALLOCATED_ATTENUATION( ar )

    if ( ARATTENUATION_FLUORESCENT( a0 ) )
    {
        arcrosstalk_dx_negpow_x(
            art_gv,
            d0,
            ARATTENUATION_CROSSTALK( a0 ),
            ARATTENUATION_CROSSTALK( ar )
            );

        ARATTENUATION_FLUORESCENT( ar ) = YES;
    }
    else
        ARATTENUATION_FLUORESCENT( ar ) = NO;

    ardirectattenuation_da_negpow_a(
        art_gv,
        d0,
        ARATTENUATION_MAIN_DIAGONAL( a0 ),
        ARATTENUATION_MAIN_DIAGONAL( ar )
        );

    ARATTENUATION_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION( ar )
    ASSERT_IDENTICAL_REFERENCE_FRAMES_AA( a0, ar )
}

void arattenuation_dda_clamp_a(
        const ART_GV         * art_gv,
        const double           d0,
        const double           d1,
        const ArAttenuation  * a0,
              ArAttenuation  * ar
        )
{
    if ( ! ARATTENUATION_INITIALISED( a0 ) )
    {
        ARATTENUATION_INITIALISED( ar ) = NO;
        return;
    }

    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_NONNEGATIVE_DOUBLE( d1 )
    ASSERT_VALID_ATTENUATION( a0 )
    ASSERT_ALLOCATED_ATTENUATION( ar )

    if ( ARATTENUATION_FLUORESCENT( a0 ) )
    {
        arcrosstalk_ddx_clamp_x(
            art_gv,
            d0,
            d1,
            ARATTENUATION_CROSSTALK( a0 ),
            ARATTENUATION_CROSSTALK( ar )
            );

        ARATTENUATION_FLUORESCENT( ar ) = YES;
    }
    else
        ARATTENUATION_FLUORESCENT( ar ) = NO;

    ardirectattenuation_dda_clamp_a(
        art_gv,
        d0,
        d1,
        ARATTENUATION_MAIN_DIAGONAL( a0 ),
        ARATTENUATION_MAIN_DIAGONAL( ar )
        );

    ARATTENUATION_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION( ar )
    ASSERT_IDENTICAL_REFERENCE_FRAMES_AA( a0, ar )
}

void arattenuation_da_mul_add_a(
        const ART_GV         * art_gv,
        const double           d0,
        const ArAttenuation  * a0,
              ArAttenuation  * ar
        )
{
    if ( ! ARATTENUATION_INITIALISED( a0 ) )
    {
        ARATTENUATION_INITIALISED( ar ) = NO;
        return;
    }

    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_VALID_ATTENUATION( a0 )
    ASSERT_VALID_ATTENUATION( ar )

    if ( ARATTENUATION_FLUORESCENT( a0 ) )
    {
        if ( ARATTENUATION_FLUORESCENT( ar ) )
            arcrosstalk_dx_mul_add_x(
                art_gv,
                d0,
                ARATTENUATION_CROSSTALK( a0 ),
                ARATTENUATION_CROSSTALK( ar )
                );
        else
        {
            ARATTENUATION_FLUORESCENT( ar ) = YES;
            arcrosstalk_dx_mul_x(
                art_gv,
                d0,
                ARATTENUATION_CROSSTALK( a0 ),
                ARATTENUATION_CROSSTALK( ar )
                );
        }
    }

    ardirectattenuation_da_mul_add_a(
        art_gv,
        d0,
        ARATTENUATION_MAIN_DIAGONAL( a0 ),
        ARATTENUATION_MAIN_DIAGONAL( ar )
        );

    ARATTENUATION_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION( ar )
}

void arattenuation_daa_interpol_a(
        const ART_GV         * art_gv,
        const double           d0,
        const ArAttenuation  * a0,
        const ArAttenuation  * a1,
              ArAttenuation  * ar
        )
{
    if (    ! ARATTENUATION_INITIALISED( a0 )
         || ! ARATTENUATION_INITIALISED( a1 ))
    {
        ARATTENUATION_INITIALISED( ar ) = NO;
        return;
    }

    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_VALID_ATTENUATION( a0 )
    ASSERT_ALLOCATED_ATTENUATION( ar )

    if ( ARATTENUATION_FLUORESCENT( a0 ) )
    {
        if ( ARATTENUATION_FLUORESCENT( a1 ) )
            arcrosstalk_dxx_interpol_x(
                art_gv,
                d0,
                ARATTENUATION_CROSSTALK( a0 ),
                ARATTENUATION_CROSSTALK( a1 ),
                ARATTENUATION_CROSSTALK( ar )
                );
        else
            arcrosstalk_dxx_interpol_x(
                art_gv,
                d0,
                ARATTENUATION_CROSSTALK( a0 ),
                ARCROSSTALK_NONE,
                ARATTENUATION_CROSSTALK( ar )
                );

        ARATTENUATION_FLUORESCENT( ar ) = YES;
    }
    else
    {
        if ( ARATTENUATION_FLUORESCENT( a1 ) )
        {
            arcrosstalk_dxx_interpol_x(
                art_gv,
                d0,
                ARCROSSTALK_NONE,
                ARATTENUATION_CROSSTALK( a1 ),
                ARATTENUATION_CROSSTALK( ar )
                );

            ARATTENUATION_FLUORESCENT( ar ) = YES;
        }
        else
            ARATTENUATION_FLUORESCENT( ar ) = NO;
    }

    ardirectattenuation_daa_interpol_a(
        art_gv,
        d0,
        ARATTENUATION_MAIN_DIAGONAL( a0 ),
        ARATTENUATION_MAIN_DIAGONAL( a1 ),
        ARATTENUATION_MAIN_DIAGONAL( ar )
        );

    ARATTENUATION_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION( ar )
}

void arattenuation_a_complement_from_one_a(
        const ART_GV         * art_gv,
        const ArAttenuation  * a0,
              ArAttenuation  * ar
        )
{
    if ( ! ARATTENUATION_INITIALISED( a0 ) )
    {
        ARATTENUATION_INITIALISED( ar ) = NO;
        return;
    }

    ASSERT_VALID_ATTENUATION( a0 )
    ASSERT_ALLOCATED_ATTENUATION( ar )

    if ( ARATTENUATION_FLUORESCENT( a0 ) )
    {
        arcrosstalk_x_complement_from_one_x(
            art_gv,
            ARATTENUATION_CROSSTALK( a0 ),
            ARATTENUATION_CROSSTALK( ar )
            );

        ARATTENUATION_FLUORESCENT( ar ) = YES;
    }
    else
        ARATTENUATION_FLUORESCENT( ar ) = NO;

    ardirectattenuation_a_complement_from_one_a(
        art_gv,
        ARATTENUATION_MAIN_DIAGONAL( a0 ),
        ARATTENUATION_MAIN_DIAGONAL( ar )
        );

    ARATTENUATION_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION( ar )
    ASSERT_IDENTICAL_REFERENCE_FRAMES_AA( a0, ar )
}

void arattenuation_ia_singleband_complement_from_one_a(
        const ART_GV         * art_gv,
        const unsigned int     i0,
        const ArAttenuation  * a0,
              ArAttenuation  * ar
        )
{
    if ( ! ARATTENUATION_INITIALISED( a0 ) )
    {
        ARATTENUATION_INITIALISED( ar ) = NO;
        return;
    }

    ASSERT_NONNEGATIVE_INTEGER( i0 )
    ASSERT_VALID_ATTENUATION( a0 )
    ASSERT_ALLOCATED_ATTENUATION( ar )

    if ( ARATTENUATION_FLUORESCENT( a0 ) )
    {
        arcrosstalk_ix_singleband_complement_from_one_x(
            art_gv,
            i0,
            ARATTENUATION_CROSSTALK( a0 ),
            ARATTENUATION_CROSSTALK( ar )
            );

        ARATTENUATION_FLUORESCENT( ar ) = YES;
    }
    else
        ARATTENUATION_FLUORESCENT( ar ) = NO;

    ardirectattenuation_ia_singleband_complement_from_one_a(
        art_gv,
        i0,
        ARATTENUATION_MAIN_DIAGONAL( a0 ),
        ARATTENUATION_MAIN_DIAGONAL( ar )
        );

    ARATTENUATION_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION( ar )
    ASSERT_IDENTICAL_REFERENCE_FRAMES_AA( a0, ar )
}

void arattenuation_dia_singleband_mul_copy_a(
        const ART_GV         * art_gv,
        const double           d0,
        const unsigned int     i0,
        const ArAttenuation  * a0,
              ArAttenuation  * ar
        )
{
    if ( ! ARATTENUATION_INITIALISED( a0 ) )
    {
        ARATTENUATION_INITIALISED( ar ) = NO;
        return;
    }

    ASSERT_NONNEGATIVE_DOUBLE( d0 )
    ASSERT_NONNEGATIVE_INTEGER( i0 )
    ASSERT_VALID_ATTENUATION( a0 )
    ASSERT_ALLOCATED_ATTENUATION( ar )

    if ( ARATTENUATION_FLUORESCENT( a0 ) )
    {
        arcrosstalk_dix_singleband_mul_copy_x(
            art_gv,
            d0,
            i0,
            ARATTENUATION_CROSSTALK( a0 ),
            ARATTENUATION_CROSSTALK( ar )
            );

        ARATTENUATION_FLUORESCENT( ar ) = YES;
    }
    else
        ARATTENUATION_FLUORESCENT( ar ) = NO;

    ardirectattenuation_dia_singleband_mul_copy_a(
        art_gv,
        d0,
        i0,
        ARATTENUATION_MAIN_DIAGONAL( a0 ),
        ARATTENUATION_MAIN_DIAGONAL( ar )
        );

    ARATTENUATION_INITIALISED( ar ) = YES;

    ASSERT_VALID_ATTENUATION( ar )
    ASSERT_IDENTICAL_REFERENCE_FRAMES_AA( a0, ar )
}

void arlight_a_mul_l(
        const ART_GV         * art_gv,
        const ArAttenuation  * a0,
              ArLight        * lr
        )
{
    ASSERT_ALLOCATED_ATTENUATION( a0 )

    if ( ARATTENUATION_INITIALISED( a0 ) )
    {
        ASSERT_VALID_ATTENUATION( a0 )
        ASSERT_VALID_LIGHT( lr )
        ASSERT_COAXIAL_ENTRY_REFERENCE_FRAME_LA( lr, a0, 3.0 DEGREES)

        if ( ARATTENUATION_FLUORESCENT( a0 ) )
            ardirectattenuation_ax_mul_l(
                art_gv,
                ARATTENUATION_MAIN_DIAGONAL( a0 ),
                ARATTENUATION_CROSSTALK( a0 ),
                lr
                );
        else
            ardirectattenuation_a_mul_l(
                art_gv,
                ARATTENUATION_MAIN_DIAGONAL( a0 ),
                lr
                );

        ASSERT_VALID_LIGHT( lr )
        ASSERT_COAXIAL_EXIT_REFERENCE_FRAME_LA( lr, a0, 3.0 DEGREES)
    }
    else
    {
        ASSERT_VALID_LIGHT( lr )
    }
}

void arlight_al_mul_l(
        const ART_GV         * art_gv,
        const ArAttenuation  * a0,
        const ArLight        * l0,
              ArLight        * lr
        )
{
    ASSERT_ALLOCATED_ATTENUATION( a0 )

    if ( ARATTENUATION_INITIALISED( a0 ) )
    {
        ASSERT_VALID_ATTENUATION( a0 )
        ASSERT_VALID_LIGHT( l0 )
        ASSERT_ALLOCATED_LIGHT( lr )
        ASSERT_COAXIAL_ENTRY_REFERENCE_FRAME_LA( l0, a0, 3.0 DEGREES)

        if ( ARATTENUATION_FLUORESCENT( a0 ) )
            ardirectattenuation_axl_mul_l(
                art_gv,
                ARATTENUATION_MAIN_DIAGONAL( a0 ),
                ARATTENUATION_CROSSTALK( a0 ),
                l0,
                lr
                );
        else
            ardirectattenuation_al_mul_l(
                art_gv,
                ARATTENUATION_MAIN_DIAGONAL( a0 ),
                l0,
                lr
                );

        ASSERT_VALID_LIGHT( lr )
        ASSERT_COAXIAL_EXIT_REFERENCE_FRAME_LA( lr, a0, 3.0 DEGREES)
    }
    else
    {
        ASSERT_VALID_LIGHT( lr )
    }
}

void arlightintensity_a_mul_i(
        const ART_GV            * art_gv,
        const ArAttenuation     * a0,
              ArLightIntensity  * lr
        )
{
    if ( ARATTENUATION_FLUORESCENT( a0 ) )
        ardirectattenuation_ax_mul_i(
            art_gv,
            ARATTENUATION_MAIN_DIAGONAL( a0 ),
            ARATTENUATION_CROSSTALK( a0 ),
            lr
            );
    else
        ardirectattenuation_a_mul_i(
            art_gv,
            ARATTENUATION_MAIN_DIAGONAL( a0 ),
            lr
            );
}

void arlightintensity_ai_mul_i(
        const ART_GV            * art_gv,
        const ArAttenuation     * a0,
        const ArLightIntensity  * l0,
              ArLightIntensity  * lr
        )
{
    if ( ARATTENUATION_FLUORESCENT( a0 ) )
        ardirectattenuation_axi_mul_i(
            art_gv,
            ARATTENUATION_MAIN_DIAGONAL( a0 ),
            ARATTENUATION_CROSSTALK( a0 ),
            l0,
            lr
            );
    else
        ardirectattenuation_ai_mul_i(
            art_gv,
            ARATTENUATION_MAIN_DIAGONAL( a0 ),
            l0,
            lr
            );
}

unsigned int arattenuation_a_valid(
        const ART_GV         * art_gv,
        const ArAttenuation  * a0
        )
{
    if ( ! a0 )
    {
        ART_ERRORHANDLING_WARNING( "null pointer" );
        return 0;
    }

    unsigned int  ir = 0;

    if ( ARATTENUATION_FLUORESCENT( a0 ) )
    {
        ir =
            arcrosstalk_x_valid(
                art_gv,
                ARATTENUATION_CROSSTALK( a0 )
                );

        if ( ! ir )
            ART_ERRORHANDLING_WARNING( "invalid crosstalk" );

        //   No point in checking the validity of the attenuation
        //   if the crosstalk is already defective.

        if ( ir )
        {
            ir =
                ardirectattenuation_a_valid(
                    art_gv,
                    ARATTENUATION_MAIN_DIAGONAL( a0 )
                    );

            if ( ! ir )
                ART_ERRORHANDLING_WARNING( "invalid direct attenuation" );
        }
    }
    else
    {
        ir =
            ardirectattenuation_a_valid(
                art_gv,
                ARATTENUATION_MAIN_DIAGONAL( a0 )
                );

        if ( ! ir )
            ART_ERRORHANDLING_WARNING( "invalid direct attenuation" );
    }

    return ir;
}

void arattenuation_a_debugprintf(
        const ART_GV        * art_gv,
        const ArAttenuation * a0
        )
{
    int number_of_colour_channels = spc_channels( art_gv );

    if ( ARATTENUATION_FLUORESCENT( a0 ) )
        printf("Fluorescent attenuation\n");
    else
        printf("Nonfluorescent attenuation\n");

    ardirectattenuation_a_debugprintf(
        art_gv,
        ARATTENUATION_MAIN_DIAGONAL( a0 )
        );

    if ( ARATTENUATION_FLUORESCENT( a0 ) )
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
                    FM( m0, i, j ) = ARATTENUATION_MAIN_DIAGONAL_AI( a0 , i );
                if ( j < i )
                    FM( m0, i, j ) = ARATTENUATION_CROSSTALK_XI( a0, c++ );
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
