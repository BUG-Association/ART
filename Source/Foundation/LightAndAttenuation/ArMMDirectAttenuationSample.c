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

#define ART_MODULE_NAME     ArMMDirectAttenuationSample

#include "ArMMDirectAttenuationSample.h"
#include "_ArDirectAttenuationSample_GV.h"
#include "AttenuationSampleImplementationMacros.h"
#include "ArLightAndAttenuationSubsystemManagement.h"
#include "FoundationAssertionMacros.h"
#include "ArPlainLightSample.h"
#include "ArSVLightSample.h"
#include "ArAttenuationSample.h"
#include <stdlib.h>

typedef struct ArMMDirectAttenuationSample_GV
{
}
ArMMDirectAttenuationSample_GV;

ART_MODULE_INITIALISATION_FUNCTION
(
    ArMMDirectAttenuationSample_GV  * armmdirectattenuationsample_gv =
        ALLOC(ArMMDirectAttenuationSample_GV);

    art_gv->armmdirectattenuationsample_gv = armmdirectattenuationsample_gv;
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    FREE(art_gv->armmdirectattenuationsample_gv);
)

ART_LIGHT_AND_ATTENUATION_MODULE_INITIALISATION_FUNCTION
(
 (void) art_gv;
)

ART_LIGHT_AND_ATTENUATION_MODULE_NO_SHUTDOWN_FUNCTION_NECESSARY


#define  YES  1
#define  NO   0

SWITCHING_IMPLEMENTATION_FOR_SAMPLE_ACT( armmdirectattenuationsample )

//   Two macros to make MM constructors more legible.

#define INIT_MM_ID(__mm,__i,__d) \
    sps_d_init_s( art_gv, (__d), ARMMDA_S_MATRIX_I((__mm),(__i)) );

#define INIT_MM_IC(__mm,__i,__c) \
    sps_s_init_s( art_gv, (__c), ARMMDA_S_MATRIX_I((__mm),(__i)) );


const char * armmdirectattenuationsample_act_string(
        const ART_GV  * art_gv
        )
{
    (void) art_gv;
    
    return "Mueller matrix attenuation";
}

ArMMDirectAttenuationSample * armmdirectattenuationsample_alloc(
        const ART_GV  * art_gv
        )
{
    ArMMDirectAttenuationSample  * newMMA =
        ALLOC( ArMMDirectAttenuationSample );

    ARMMDA_S_MATRIX( *newMMA ) =
        armuellermatrixsample_alloc( art_gv );

    return newMMA;
}

void armmdirectattenuationsample_free(
        const ART_GV                     * art_gv,
              ArMMDirectAttenuationSample  * ar
        )
{
    armuellermatrixsample_free(
        art_gv,
        ARMMDA_S_MATRIX( *ar )
        );

    FREE(ar);
}

ArMMDirectAttenuationSample * armmdirectattenuationsample_d_alloc_init(
        const ART_GV  * art_gv,
        const double    d0
        )
{
    (void) art_gv;
    (void) d0;
    
    ART_ERRORHANDLING_FATAL_ERROR(
        "non-polarisation aware attenuation functionality used on "
        "a polarising attenuation struct"
        );

    return 0;
}

ArMMDirectAttenuationSample * armmdirectattenuationsample_drr_alloc_init_depolarising(
        const ART_GV              * art_gv,
        const double                d0,
        const ArReferenceFrame    * r0,
        const ArReferenceFrame    * r1
        )
{
    ArMMDirectAttenuationSample  * ar;

    ar = armmdirectattenuationsample_alloc( art_gv );

    armmdirectattenuationsample_drr_init_depolarising_a(
        art_gv,
        d0,
        r0,
        r1,
        ar
        );

    return ar;
}

ArMMDirectAttenuationSample * armmdirectattenuationsample_drr_alloc_init_nonpolarising(
        const ART_GV            * art_gv,
        const double              d0,
        const ArReferenceFrame  * r0,
        const ArReferenceFrame  * r1
        )
{
    ArMMDirectAttenuationSample  * ar;

    ar = armmdirectattenuationsample_alloc( art_gv );

    armmdirectattenuationsample_drr_init_nonpolarising_a(
        art_gv,
        d0,
        r0,
        r1,
        ar
        );

    return ar;
}

ArMMDirectAttenuationSample * armmdirectattenuationsample_a_alloc_init(
        const ART_GV                     * art_gv,
        const ArMMDirectAttenuationSample  * a0
        )
{
    ArMMDirectAttenuationSample  * ar;

    ar = armmdirectattenuationsample_alloc( art_gv );

    armmdirectattenuationsample_a_init_a(
        art_gv,
        a0,
        ar
        );

    return ar;
}

ArMMDirectAttenuationSample const * armmdirectattenuationsample_total(
        const ART_GV  * art_gv
        )
{
    (void) art_gv;
    
    ART_ERRORHANDLING_FATAL_ERROR(
        "non-polarisation aware attenuation functionality used on "
        "a polarising attenuation struct"
        );

    return 0;
}

ArMMDirectAttenuationSample * armmdirectattenuationsample_rr_alloc_init_total(
        const ART_GV              * art_gv,
        const ArReferenceFrame    * r0,
        const ArReferenceFrame    * r1
        )
{
    (void) art_gv;
    (void) r0;
    (void) r1;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return 0;
}

ArMMDirectAttenuationSample const * armmdirectattenuationsample_none(
        const ART_GV  * art_gv
        )
{
    (void) art_gv;
    
    ART_ERRORHANDLING_FATAL_ERROR(
        "non-polarisation aware attenuation functionality used on "
        "a polarising attenuation struct"
        );

    return 0;
}

ArMMDirectAttenuationSample * armmdirectattenuationsample_rr_alloc_init_none(
        const ART_GV              * art_gv,
        const ArReferenceFrame    * r0,
        const ArReferenceFrame    * r1
        )
{
    (void) art_gv;
    (void) r0;
    (void) r1;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return 0;
}

void armmdirectattenuationsample_d_init_a(
        const ART_GV                     * art_gv,
        const double                       d0,
              ArMMDirectAttenuationSample  * ar
        )
{
    (void) art_gv;
    (void) d0;
    (void) ar;
    
    ART_ERRORHANDLING_FATAL_ERROR(
        "non-polarisation aware attenuation functionality used on "
        "a polarising attenuation struct"
        );
}

void armmdirectattenuationsample_drr_init_depolarising_a(
        const ART_GV                     * art_gv,
        const double                       d0,
        const ArReferenceFrame           * r0,
        const ArReferenceFrame           * r1,
              ArMMDirectAttenuationSample  * ar
        )
{
    armuellermatrixsample_d_init_depolarising_m(
        art_gv,
        d0,
        ARMMDA_S_MATRIX( *ar )
        );

    ARMMDIRECTATTENUATIONSAMPLE_SET_PROPERTY_DEPOLARISER( *ar );

    ARMMDA_S_REFFRAME_ENTRY( *ar ) = *r0;
    ARMMDA_S_REFFRAME_EXIT( *ar )  = *r1;
}

void armmdirectattenuationsample_drr_init_nonpolarising_a(
        const ART_GV                     * art_gv,
        const double                       d0,
        const ArReferenceFrame           * r0,
        const ArReferenceFrame           * r1,
              ArMMDirectAttenuationSample  * ar
        )
{
    armuellermatrixsample_d_init_nonpolarising_m(
        art_gv,
        d0,
        ARMMDA_S_MATRIX( *ar )
        );

    ARMMDIRECTATTENUATIONSAMPLE_SET_PROPERTY_ROTATIONALLY_INVARIANT( *ar );

    ARMMDA_S_REFFRAME_ENTRY( *ar ) = *r0;
    ARMMDA_S_REFFRAME_EXIT( *ar )  = *r1;
}

void armmdirectattenuationsample_s_init_a(
        const ART_GV                     * art_gv,
        const ArSpectralSample             * c0,
              ArMMDirectAttenuationSample  * ar
        )
{
    (void) art_gv;
    (void) c0;
    (void) ar;
    
    ART_ERRORHANDLING_FATAL_ERROR(
        "non-polarisation aware attenuation functionality used on "
        "a polarising attenuation struct"
        );
}

void armmdirectattenuationsample_srr_init_depolarising_a(
        const ART_GV                     * art_gv,
        const ArSpectralSample             * c0,
        const ArReferenceFrame           * r0,
        const ArReferenceFrame           * r1,
              ArMMDirectAttenuationSample  * ar
        )
{
    armuellermatrixsample_h_init_depolarising_m(
        art_gv,
        c0,
        ARMMDA_S_MATRIX( *ar )
        );

    ARMMDIRECTATTENUATIONSAMPLE_SET_PROPERTY_DEPOLARISER( *ar );

    ARMMDA_S_REFFRAME_ENTRY( *ar ) = *r0;
    ARMMDA_S_REFFRAME_EXIT( *ar )  = *r1;
}

void armmdirectattenuationsample_s_init_nonpolarising_a(
        const ART_GV                     * art_gv,
        const ArSpectralSample             * c0,
              ArMMDirectAttenuationSample  * ar
        )
{
    (void) art_gv;
    (void) c0;
    (void) ar;
    
    ART_ERRORHANDLING_FATAL_ERROR(
        "non-polarisation aware attenuation functionality used on "
        "a polarising attenuation struct"
        );
}

void armmdirectattenuationsample_srr_init_nonpolarising_a(
        const ART_GV                     * art_gv,
        const ArSpectralSample             * c0,
        const ArReferenceFrame           * r0,
        const ArReferenceFrame           * r1,
              ArMMDirectAttenuationSample  * ar
        )
{
    armuellermatrixsample_h_init_nonpolarising_m(
        art_gv,
        c0,
        ARMMDA_S_MATRIX( *ar )
        );

    ARMMDIRECTATTENUATIONSAMPLE_SET_PROPERTY_ROTATIONALLY_INVARIANT( *ar );

    ARMMDA_S_REFFRAME_ENTRY( *ar ) = *r0;
    ARMMDA_S_REFFRAME_EXIT( *ar )  = *r1;
}

void armmdirectattenuationsample_mm_rr_init_polarising_a(
        const ART_GV                       * art_gv,
        const ArMuellerMatrixSample        * m0,
        const ArReferenceFrame             * r0,
        const ArReferenceFrame             * r1,
              ArMMDirectAttenuationSample  * ar
        )
{
    armuellermatrixsample_m_init_m(
        art_gv,
        m0,
        ARMMDA_S_MATRIX( *ar )
        );

    ARMMDA_S_REFFRAME_ENTRY( *ar ) = *r0;
    ARMMDA_S_REFFRAME_EXIT( *ar )  = *r1;

    ARMMDIRECTATTENUATIONSAMPLE_SET_PROPERTY_GENERAL_MATRIX( *ar );
}

void armmdirectattenuationsample_ddrr_init_linear_polariser_a(
        const ART_GV                     * art_gv,
        const double                       d0,
        const double                       d1,
        const ArReferenceFrame           * r0,
        const ArReferenceFrame           * r1,
              ArMMDirectAttenuationSample  * ar
        )
{
    ARMMDA_S_REFFRAME_ENTRY( *ar ) = *r0;
    ARMMDA_S_REFFRAME_EXIT( *ar ) = *r1;

    armuellermatrixsample_dd_init_linear_polariser_m(
        art_gv,
        d0,
        d1,
        ARMMDA_S_MATRIX( *ar )
        );

    ARMMDIRECTATTENUATIONSAMPLE_SET_PROPERTY_GENERAL_MATRIX( *ar );
}

void armmdirectattenuationsample_a_init_s(
        const ART_GV                     * art_gv,
        const ArMMDirectAttenuationSample  * a0,
              ArSpectralSample             * cr
        )
{
    sps_s_init_s(
          art_gv,
          ARMMDA_S_MATRIX_II( *a0, 0, 0 ),
          cr
        );
}

void  armmdirectattenuationsample_a_init_a(
        const ART_GV                      * art_gv,
        const ArMMDirectAttenuationSample   * a0,
              ArMMDirectAttenuationSample   * ar
        )
{
    armuellermatrixsample_m_init_m(
        art_gv,
        ARMMDA_S_MATRIX( *a0 ),
        ARMMDA_S_MATRIX( *ar )
        );

    ARMMDA_S_REFFRAME_ENTRY( *ar ) = ARMMDA_S_REFFRAME_ENTRY( *a0 );
    ARMMDA_S_REFFRAME_EXIT( *ar )  = ARMMDA_S_REFFRAME_EXIT( *a0 );

    ARMMDA_S_PROPERTIES( *ar ) = ARMMDA_S_PROPERTIES( *a0 );
}


unsigned int armmdirectattenuationsample_a_polarising(
        const ART_GV                     * art_gv,
        const ArMMDirectAttenuationSample  * a0
        )
{
    (void) art_gv;
    
    return
        ARMMDIRECTATTENUATIONSAMPLE_IS_GENERAL_MATRIX( *a0 );
}

ArReferenceFrame const * armmdirectattenuationsample_a_entry_rf(
        const ART_GV                     * art_gv,
        const ArMMDirectAttenuationSample  * a0
        )
{
    (void) art_gv;
    
    return
        & ARMMDA_S_REFFRAME_ENTRY( *a0 );
}

ArReferenceFrame const * armmdirectattenuationsample_a_exit_rf(
        const ART_GV                     * art_gv,
        const ArMMDirectAttenuationSample  * a0
        )
{
    (void) art_gv;
    
    return
        & ARMMDA_S_REFFRAME_EXIT( *a0 );
}

/* ---------------------------------------------------------------------------

    'ardirectattenuationsample_a_to_mm'

    Wrapper to extract the Mueller matrix from a given attenuation struct.

------------------------------------------------------------------------aw- */

void armmdirectattenuationsample_a_to_mm(
        const ART_GV                     * art_gv,
        const ArMMDirectAttenuationSample  * a0,
              ArMuellerMatrixSample        * mm
        )
{
    armuellermatrixsample_m_init_m(
        art_gv,
        ARMMDA_S_MATRIX( *a0 ),
        mm
        );
}

/* ---------------------------------------------------------------------------

    'armmdirectattenuationsample_a_align_for_mul_and_extract_matrices_amm'
    'armmdirectattenuationsample_aa_align_for_mul_and_extract_matrices_amm'

    Both functions take two attenuation elements, align them into a common
    reference system and return their raw (but aligned) Mueller matrices.
    The only difference is that one is intended for two-operand cases, and
    the other one for three operands.

    It is assumed that at least one of the two inputs is polarising. The
    actual alignment works in such a way that the first operator is aligned
    to the system of the second if a reference frame is present there,
    otherwise the reference of the first is used and used on the second.
    The only direct change to the result element is that the correct
    reference system is written.

    In the two-operand case the first attenuation element is always
    preserved unchanged, while in the latter case alignment changes are made
    to the second (result) operand. In the three-operand case both input
    elements are preserved unchanged.

    This sequence of operations may seem totally arbitrary, but it is the
    common prelude for most actual operations on attenuation elements found
    further down, and therefore a valuable shorthand in these functions.

---------------------------------------------------------------------ip-aw- */

void armmdirectattenuationsample_a_align_for_mul_and_extract_matrices_amm(
        const ART_GV                     * art_gv,
        const ArMMDirectAttenuationSample  * a0,
              ArMMDirectAttenuationSample  * ar,
              ArMuellerMatrixSample        * m0,
              ArMuellerMatrixSample        * m1
        )
{
    ASSERT_COAXIAL_REFERENCE_FRAMES_RR(
        & ARMMDIRECTATTENUATIONSAMPLE_EXIT_REFFRAME( *a0 ),
        & ARMMDIRECTATTENUATIONSAMPLE_ENTRY_REFFRAME( *ar ),
          3.0 DEGREES
        );

    //  We only rotate a0 if it is a general matrix, and not rotationally
    //  invariant.

    if ( ARMMDIRECTATTENUATIONSAMPLE_IS_ROTATIONALLY_INVARIANT( *a0 ) )
    {
        armmdirectattenuationsample_a_to_mm(
              art_gv,
              a0,
              m0
            );
    }
    else
    {
        armmdirectattenuationsample_a_realign_to_coaxial_exit_rf_mm(
              art_gv,
              a0,
            & ARMMDA_S_REFFRAME_ENTRY( *ar ),
              m0
            );
    }

    //   a1 is always taken "as is", no rotation

    armmdirectattenuationsample_a_to_mm(
        art_gv,
        ar,
        m1
        );

    //   The result has the entry frame of a0, and exit frame of ar

    ARMMDA_S_REFFRAME_ENTRY( *ar ) = ARMMDA_S_REFFRAME_ENTRY( *a0 );
}

void armmdirectattenuationsample_aa_align_for_mul_and_extract_matrices_amm(
        const ART_GV                     * art_gv,
        const ArMMDirectAttenuationSample  * a0,
        const ArMMDirectAttenuationSample  * a1,
              ArMMDirectAttenuationSample  * ar,
              ArMuellerMatrixSample        * m0,
              ArMuellerMatrixSample        * m1
        )
{
    ASSERT_COAXIAL_REFERENCE_FRAMES_RR(
        & ARMMDIRECTATTENUATIONSAMPLE_EXIT_REFFRAME( *a0 ),
        & ARMMDIRECTATTENUATIONSAMPLE_ENTRY_REFFRAME( *a1 ),
          3.0 DEGREES
        );

    //  We only rotate a0 if it is a general matrix, and not rotationally
    //  invariant.

    if ( ARMMDIRECTATTENUATIONSAMPLE_IS_ROTATIONALLY_INVARIANT( *a0 ) )
    {
        armmdirectattenuationsample_a_to_mm(
              art_gv,
              a0,
              m0
            );
    }
    else
    {
        armmdirectattenuationsample_a_realign_to_coaxial_exit_rf_mm(
              art_gv,
              a0,
            & ARMMDA_S_REFFRAME_ENTRY( *a1 ),
              m0
            );
    }

    //   a1 is always taken "as is", no rotation

    armmdirectattenuationsample_a_to_mm(
        art_gv,
        a1,
        m1
        );

    //   The result has the entry frame of a0, and exit frame of a1

    ARMMDA_S_REFFRAME_ENTRY( *ar ) = ARMMDA_S_REFFRAME_ENTRY( *a0 );
    ARMMDA_S_REFFRAME_EXIT( *ar )  = ARMMDA_S_REFFRAME_EXIT( *a1 );
}

void armmdirectattenuationsample_aa_align_for_add_and_extract_matrix_am(
        const ART_GV                     * art_gv,
        const ArMMDirectAttenuationSample  * a0,
        const ArMMDirectAttenuationSample  * a1,
              ArMMDirectAttenuationSample  * ar,
              ArMuellerMatrixSample        * m0
        )
{
    ASSERT_VALID_REFERENCE_FRAME( & ARMMDA_S_REFFRAME_ENTRY( *a0 ) );
    ASSERT_VALID_REFERENCE_FRAME( & ARMMDA_S_REFFRAME_EXIT(  *a0 ) );

    ASSERT_VALID_REFERENCE_FRAME( & ARMMDA_S_REFFRAME_ENTRY( *a1 ) );
    ASSERT_VALID_REFERENCE_FRAME( & ARMMDA_S_REFFRAME_EXIT(  *a1 ) );

    ASSERT_COAXIAL_SAMPLE_ENTRY_REFERENCE_FRAME_MM_DA_AA(
        a0, a1, 3.0 DEGREES
        );
    ASSERT_COAXIAL_SAMPLE_EXIT_REFERENCE_FRAME_MM_DA_AA(
        a0, a1, 3.0 DEGREES
        );

    if (   ARMMDIRECTATTENUATIONSAMPLE_IS_ROTATIONALLY_INVARIANT( *a0 )
        && ARMMDIRECTATTENUATIONSAMPLE_IS_ROTATIONALLY_INVARIANT( *a1 ) )
    {
        /* -------------------------------------------------------------------
            Both MMs are rotationally invariant so there is no need to rotate
            them, we just use them as they are.
        ----------------------------------------------------------------ip- */

        armmdirectattenuationsample_a_to_mm(
            art_gv,
            a0,
            m0
            );

        ARMMDA_S_REFFRAME_ENTRY( *ar ) = ARMMDA_S_REFFRAME_ENTRY( *a1 );
        ARMMDA_S_REFFRAME_EXIT( *ar )  = ARMMDA_S_REFFRAME_EXIT( *a1 );
    }
    else if (   ARMMDIRECTATTENUATIONSAMPLE_IS_ROTATIONALLY_INVARIANT( *a0 )
             || ARMMDIRECTATTENUATIONSAMPLE_IS_ROTATIONALLY_INVARIANT( *a1 ) )
    {
        /* -------------------------------------------------------------------
            There is one rotationally invariant and one general MM. There is
            no need to rotate the rotationally invariant MM if we use
            the general MM's reference frames for the result.
        ----------------------------------------------------------------ip- */

        armmdirectattenuationsample_a_to_mm(
            art_gv,
            a0,
            m0
            );

        if ( ARMMDIRECTATTENUATIONSAMPLE_IS_GENERAL_MATRIX( *a0 ) )
        {
            ARMMDA_S_REFFRAME_ENTRY( *ar ) = ARMMDA_S_REFFRAME_ENTRY( *a0 );
            ARMMDA_S_REFFRAME_EXIT( *ar )  = ARMMDA_S_REFFRAME_EXIT( *a0 );
        }
        else
        {
            ARMMDA_S_REFFRAME_ENTRY( *ar ) = ARMMDA_S_REFFRAME_ENTRY( *a1 );
            ARMMDA_S_REFFRAME_EXIT( *ar )  = ARMMDA_S_REFFRAME_EXIT( *a1 );
        }
    }
    else
    {
        /* -------------------------------------------------------------------
            Both MMs are general. We will rotate a0 to the refframes of ar.
            However, we can still save some rotations if entry and/or exit
            reference frames are equal. We, of course, use the reference
            frames of ar for the result.
        ----------------------------------------------------------------ip- */

        const unsigned int entry_rfs_equal =
            arrefframe_rf_rf_d_equal(
                art_gv,
                armmdirectattenuationsample_a_entry_rf( art_gv, a0 ),
                armmdirectattenuationsample_a_entry_rf( art_gv, a1 ),
                0.01
                );
        const unsigned int exit_rfs_equal =
            arrefframe_rf_rf_d_equal(
                art_gv,
                armmdirectattenuationsample_a_exit_rf( art_gv, a0 ),
                armmdirectattenuationsample_a_exit_rf( art_gv, a1 ),
                0.01
                );

        if ( entry_rfs_equal && exit_rfs_equal )
        {
            /* ---------------------------------------------------------------
                Both entry and exit refframes are identical so there is no
                need to rotate a0 MM, we just return it as it is.
            ------------------------------------------------------------ip- */

            armmdirectattenuationsample_a_to_mm(
                art_gv,
                a0,
                m0
                );
        }
        else if ( entry_rfs_equal && ! exit_rfs_equal )
        {
            ART__CODE_IS_NOT_TESTED__EXIT_WITH_ERROR

            /* ---------------------------------------------------------------
                Only the exit reference frames are not equal. Rotate just
                the a0 exit refframe.
            ------------------------------------------------------------ip- */

            //  We first compute the angle for conversion from the a0 exit
            //  refframe to the a1 exit refframe

            const double  exit_rot_cos =
                vec3d_vv_dot(
                    & ARMMDIRECTATTENUATIONSAMPLE_EXIT_REFFRAME_I( *a0, 0 ),
                    & ARMMDIRECTATTENUATIONSAMPLE_EXIT_REFFRAME_I( *a1, 0 )
                    );
            const double  exit_rot_sin =
                vec3d_vv_dot(
                    & ARMMDIRECTATTENUATIONSAMPLE_EXIT_REFFRAME_I( *a0, 0 ),
                    & ARMMDIRECTATTENUATIONSAMPLE_EXIT_REFFRAME_I( *a1, 1 )
                    );

            //  Then we do the rotation itself

            armuellermatrixsample_mdd_rotate_output_m(
                art_gv,
                ARMMDIRECTATTENUATIONSAMPLE_MM( *a0 ),
                exit_rot_cos,
                exit_rot_sin,
                m0
                );
        }
        else if ( ! entry_rfs_equal && exit_rfs_equal )
        {
            ART__CODE_IS_NOT_TESTED__EXIT_WITH_ERROR

            /* ---------------------------------------------------------------
                Only the entry reference frames are not equal. Rotate just
                the a0 entry refframe.
            ------------------------------------------------------------ip- */

            //  We first compute the angle for conversion from the a1 entry
            //  refframe to the a0 entry refframe

            const double  entry_rot_cos =
                vec3d_vv_dot(
                    & ARMMDIRECTATTENUATIONSAMPLE_ENTRY_REFFRAME_I( *a1, 0 ),
                    & ARMMDIRECTATTENUATIONSAMPLE_ENTRY_REFFRAME_I( *a0, 0 )
                    );
            const double  entry_rot_sin =
                vec3d_vv_dot(
                    & ARMMDIRECTATTENUATIONSAMPLE_ENTRY_REFFRAME_I( *a1, 0 ),
                    & ARMMDIRECTATTENUATIONSAMPLE_ENTRY_REFFRAME_I( *a0, 1 )
                    );

            //  Then we do the rotation itself

            armuellermatrixsample_mdd_rotate_input_m(
                art_gv,
                ARMMDIRECTATTENUATIONSAMPLE_MM( *a0 ),
                entry_rot_cos,
                entry_rot_sin,
                m0
                );
        }
        else
        {
            ART__CODE_IS_NOT_TESTED__EXIT_WITH_ERROR

            /* ---------------------------------------------------------------
                Completely general case - both entry and exit reference
                frames are not equal. We need to rotate both entry and exit
                refframe of a0.
            ------------------------------------------------------------ip- */

            //  We first compute the angles for conversions
            //  - from the a1 entry refframe to the a0 entry refframe
            //  - from the a0 exit refframe to the a1 exit refframe

            const double  entry_rot_cos =
                vec3d_vv_dot(
                    & ARMMDIRECTATTENUATIONSAMPLE_ENTRY_REFFRAME_I( *a1, 0 ),
                    & ARMMDIRECTATTENUATIONSAMPLE_ENTRY_REFFRAME_I( *a0, 0 )
                    );
            const double  entry_rot_sin =
                vec3d_vv_dot(
                    & ARMMDIRECTATTENUATIONSAMPLE_ENTRY_REFFRAME_I( *a1, 0 ),
                    & ARMMDIRECTATTENUATIONSAMPLE_ENTRY_REFFRAME_I( *a0, 1 )
                    );

            const double  exit_rot_cos =
                vec3d_vv_dot(
                    & ARMMDIRECTATTENUATIONSAMPLE_EXIT_REFFRAME_I( *a0, 0 ),
                    & ARMMDIRECTATTENUATIONSAMPLE_EXIT_REFFRAME_I( *a1, 0 )
                    );
            const double  exit_rot_sin =
                vec3d_vv_dot(
                    & ARMMDIRECTATTENUATIONSAMPLE_EXIT_REFFRAME_I( *a0, 0 ),
                    & ARMMDIRECTATTENUATIONSAMPLE_EXIT_REFFRAME_I( *a1, 1 )
                    );

            //  Then we do both rotations in one step

            armuellermatrixsample_mdddd_rotate_input_and_output_m(
                art_gv,
                ARMMDIRECTATTENUATIONSAMPLE_MM( *a0 ),
                entry_rot_cos,
                entry_rot_sin,
                exit_rot_cos,
                exit_rot_sin,
                m0
                );
        }

        ARMMDA_S_REFFRAME_ENTRY( *ar ) = ARMMDA_S_REFFRAME_ENTRY( *a1 );
        ARMMDA_S_REFFRAME_EXIT( *ar )  = ARMMDA_S_REFFRAME_EXIT( *a1 );
    }
}

void armmdirectattenuationsample_a_align_for_add_and_extract_matrix_am(
        const ART_GV                     * art_gv,
        const ArMMDirectAttenuationSample  * a0,
              ArMMDirectAttenuationSample  * ar,
              ArMuellerMatrixSample        * m0
        )
{
    ASSERT_VALID_REFERENCE_FRAME( & ARMMDA_S_REFFRAME_ENTRY( *a0 ) );
    ASSERT_VALID_REFERENCE_FRAME( & ARMMDA_S_REFFRAME_EXIT(  *a0 ) );

    ASSERT_VALID_REFERENCE_FRAME( & ARMMDA_S_REFFRAME_ENTRY( *ar ) );
    ASSERT_VALID_REFERENCE_FRAME( & ARMMDA_S_REFFRAME_EXIT(  *ar ) );

    ASSERT_COAXIAL_SAMPLE_ENTRY_REFERENCE_FRAME_MM_DA_AA(
        a0, ar, 3.0 DEGREES
        );
    ASSERT_COAXIAL_SAMPLE_EXIT_REFERENCE_FRAME_MM_DA_AA(
        a0, ar, 3.0 DEGREES
        );

    if (   ARMMDIRECTATTENUATIONSAMPLE_IS_ROTATIONALLY_INVARIANT( *a0 )
        && ARMMDIRECTATTENUATIONSAMPLE_IS_ROTATIONALLY_INVARIANT( *ar ) )
    {
        /* -------------------------------------------------------------------
            Both MMs are rotationally invariant so there is no need to rotate
            them, we just use them as they are. The reference frames stay
            untouched as well.
        ----------------------------------------------------------------ip- */

        armmdirectattenuationsample_a_to_mm(
            art_gv,
            a0,
            m0
            );
    }
    else if (   ARMMDIRECTATTENUATIONSAMPLE_IS_ROTATIONALLY_INVARIANT( *a0 )
             || ARMMDIRECTATTENUATIONSAMPLE_IS_ROTATIONALLY_INVARIANT( *ar ) )
    {
        /* -------------------------------------------------------------------
            There is one rotationally invariant and one general MM. There is
            no need to rotate the rotationally invariant MM if we use
            the general MM reference frames for the result.
        ----------------------------------------------------------------ip- */

        armmdirectattenuationsample_a_to_mm(
            art_gv,
            a0,
            m0
            );

        if ( ARMMDIRECTATTENUATIONSAMPLE_IS_GENERAL_MATRIX( *a0 ) )
        {
            ARMMDA_S_REFFRAME_ENTRY( *ar ) = ARMMDA_S_REFFRAME_ENTRY( *a0 );
            ARMMDA_S_REFFRAME_EXIT( *ar )  = ARMMDA_S_REFFRAME_EXIT( *a0 );
        }
    }
    else
    {
        /* -------------------------------------------------------------------
            Both MMs are general. We will rotate a0 to the refframes of ar.
            However, we can still save some rotations if entry and/or exit
            reference frames are equal. The reference frames stay untouched.
        ----------------------------------------------------------------ip- */

        const unsigned int entry_rfs_equal =
            arrefframe_rf_rf_d_equal(
                art_gv,
                armmdirectattenuationsample_a_entry_rf( art_gv, a0 ),
                armmdirectattenuationsample_a_entry_rf( art_gv, ar ),
                0.01
                );
        const unsigned int exit_rfs_equal =
            arrefframe_rf_rf_d_equal(
                art_gv,
                armmdirectattenuationsample_a_exit_rf( art_gv, a0 ),
                armmdirectattenuationsample_a_exit_rf( art_gv, ar ),
                0.01
                );

        if ( entry_rfs_equal && exit_rfs_equal )
        {
            /* ---------------------------------------------------------------
                Both entry and exit refframes are identical so there is no
                need to rotate a0 MM, we just return it as it is.
            ------------------------------------------------------------ip- */

            armmdirectattenuationsample_a_to_mm(
                art_gv,
                a0,
                m0
                );
        }
        else if ( entry_rfs_equal && ! exit_rfs_equal )
        {

            /* ---------------------------------------------------------------
                Only the exit reference frames are not equal. Rotate just
                the a0 exit refframe.
            ------------------------------------------------------------ip- */

            //  We first compute the angle for conversion from the a0 exit
            //  refframe to the ar exit refframe

            const double  exit_rot_cos =
                vec3d_vv_dot(
                    & ARMMDIRECTATTENUATIONSAMPLE_EXIT_REFFRAME_I( *a0, 0 ),
                    & ARMMDIRECTATTENUATIONSAMPLE_EXIT_REFFRAME_I( *ar, 0 )
                    );
            const double  exit_rot_sin =
                vec3d_vv_dot(
                    & ARMMDIRECTATTENUATIONSAMPLE_EXIT_REFFRAME_I( *a0, 0 ),
                    & ARMMDIRECTATTENUATIONSAMPLE_EXIT_REFFRAME_I( *ar, 1 )
                    );

            //  Then we do the rotation itself

            armuellermatrixsample_mdd_rotate_output_m(
                art_gv,
                ARMMDIRECTATTENUATIONSAMPLE_MM( *a0 ),
                exit_rot_cos,
                exit_rot_sin,
                m0
                );
        }
        else if ( ! entry_rfs_equal && exit_rfs_equal )
        {
            /* ---------------------------------------------------------------
                Only the entry reference frames are not equal. Rotate just
                the a0 entry refframe.
            ------------------------------------------------------------ip- */

            //  We first compute the angle for conversion from the ar entry
            //  refframe to the a0 entry refframe

            const double  entry_rot_cos =
                vec3d_vv_dot(
                    & ARMMDIRECTATTENUATIONSAMPLE_ENTRY_REFFRAME_I( *ar, 0 ),
                    & ARMMDIRECTATTENUATIONSAMPLE_ENTRY_REFFRAME_I( *a0, 0 )
                    );
            const double  entry_rot_sin =
                vec3d_vv_dot(
                    & ARMMDIRECTATTENUATIONSAMPLE_ENTRY_REFFRAME_I( *ar, 0 ),
                    & ARMMDIRECTATTENUATIONSAMPLE_ENTRY_REFFRAME_I( *a0, 1 )
                    );

            //  Then we do the rotation itself

            armuellermatrixsample_mdd_rotate_input_m(
                art_gv,
                ARMMDIRECTATTENUATIONSAMPLE_MM( *a0 ),
                entry_rot_cos,
                entry_rot_sin,
                m0
                );
        }
        else
        {
            /* ---------------------------------------------------------------
                Completely general case - both entry and exit reference
                frames are not equal. We need to rotate both entry and exit
                refframe of a0.
            ------------------------------------------------------------ip- */

            //  We first compute the angles for conversions
            //  - from the ar entry refframe to the a0 entry refframe
            //  - from the a0 exit refframe to the ar exit refframe

            const double  entry_rot_cos =
                vec3d_vv_dot(
                    & ARMMDIRECTATTENUATIONSAMPLE_ENTRY_REFFRAME_I( *ar, 0 ),
                    & ARMMDIRECTATTENUATIONSAMPLE_ENTRY_REFFRAME_I( *a0, 0 )
                    );
            const double  entry_rot_sin =
                vec3d_vv_dot(
                    & ARMMDIRECTATTENUATIONSAMPLE_ENTRY_REFFRAME_I( *ar, 0 ),
                    & ARMMDIRECTATTENUATIONSAMPLE_ENTRY_REFFRAME_I( *a0, 1 )
                    );

            const double  exit_rot_cos =
                vec3d_vv_dot(
                    & ARMMDIRECTATTENUATIONSAMPLE_EXIT_REFFRAME_I( *a0, 0 ),
                    & ARMMDIRECTATTENUATIONSAMPLE_EXIT_REFFRAME_I( *ar, 0 )
                    );
            const double  exit_rot_sin =
                vec3d_vv_dot(
                    & ARMMDIRECTATTENUATIONSAMPLE_EXIT_REFFRAME_I( *a0, 0 ),
                    & ARMMDIRECTATTENUATIONSAMPLE_EXIT_REFFRAME_I( *ar, 1 )
                    );

            //  Then we do both rotations in one step

            armuellermatrixsample_mdddd_rotate_input_and_output_m(
                art_gv,
                ARMMDIRECTATTENUATIONSAMPLE_MM( *a0 ),
                entry_rot_cos,
                entry_rot_sin,
                exit_rot_cos,
                exit_rot_sin,
                m0
                );
        }
    }
}

double armmdirectattenuationsample_ai(
        const ART_GV                     * art_gv,
        const ArMMDirectAttenuationSample  * a0,
        const unsigned int                 i0
        )
{
    return
        sps_si(
            art_gv,
            ARMMDIRECTATTENUATIONSAMPLE( *a0 ),
            i0
            );
}

void armmdirectattenuationsample_set_aid(
        const ART_GV                     * art_gv,
        const ArMMDirectAttenuationSample  * a0,
        const unsigned int                 i0,
        const double                       d0
        )
{
    sps_set_sid(
        art_gv,
        ARMMDIRECTATTENUATIONSAMPLE( *a0 ),
        i0,
        d0
    );
}

/* ---------------------------------------------------------------------------

    'armmdirectattenuationsample_a_realign_to_coaxial_exit_rf_mm'


    FIXME:
        This is wrong! This code rotates the whole Mueller matrix instead
        of just rotating the exiting Stokes vector to fit the next coordinate
        system in the optical elements chain.


    This function has a limited but highly important function: it modifies
    the Mueller matrix of an attenuation element to fit a different collinear
    reference coordinate system, i.e. one that is just rotated from the original
    along the axis of propagation. It leaves the original data intact, and
    returns the modified Mueller matrix as a separate result. It is assumed
    that the input attenuation element is a polarising Mueller matrix,
    otherwise the function does nothing.


    IMPORTANT NOTE:
    ---------------

    This procedure returns complete garbage if it is used to compute the
    transformation between non-collinear reference systems. Which is the way
    it should be, since that kind of transformation does not make sense from
    a physical standpoint. The procedure does not perform any sanity checks in
    this direction for performance reasons, so be careful when using it.

---------------------------------------------------------------------ip-aw- */

void armmdirectattenuationsample_a_realign_to_coaxial_exit_rf_mm(
        const ART_GV                     * art_gv,
        const ArMMDirectAttenuationSample  * a0,
        const ArReferenceFrame           * r0,
              ArMuellerMatrixSample        * mr
        )
{
    if ( ! ARMMDIRECTATTENUATIONSAMPLE_IS_GENERAL_MATRIX( *a0 ) )
        return;
    
    /* -------------------------------------------------------------------
        This function is modified at May-2017 during touchbar sim project, 
        as the original code seems to rotate the MM twice during re-alignment  
        --CW
        
        To rotate the exit frame of a MM, we need to left multiply
        a rotation matrix to original MM.
        
        <Polarized light,Second Edition, Revised and Expanded> 
            section 5.5: THE MUELLER MATRIX OF A ROTATOR
            rotation matrix equation 5.41
    ------------------------------------------------------------------- */
    

    /* -------------------------------------------------------------------
        Here we figure out the angle between the two reference systems.
    ------------------------------------------------------------------- */
    
    ASSERT_COAXIAL_REFERENCE_FRAMES_RR(
        & ARMMDIRECTATTENUATIONSAMPLE_EXIT_REFFRAME( *a0),
          r0,
          3 DEGREES
          );

    double  cos_phi =
        vec3d_vv_dot(
            & ARMMDIRECTATTENUATIONSAMPLE_EXIT_REFFRAME_I( *a0, 0 ),
            & ARREFFRAME_RF_I( *r0, 0 )
            );

    double  sin_phi =
        vec3d_vv_dot(
            & ARMMDIRECTATTENUATIONSAMPLE_EXIT_REFFRAME_I( *a0, 1 ),
            & ARREFFRAME_RF_I( *r0, 0 )
            );

    double  cos_2phi = M_SQR( cos_phi ) - M_SQR( sin_phi );
    double  sin_2phi = 2 * cos_phi * sin_phi;

    
    //  copied elements (0,0), (0,1), (0,2), (0,3),
    //  (3,0), (3,1), (3,2), (3,3)
    //  because these elements will not be changed

    sps_s_init_s(
        art_gv,
        ARMMDA_S_MATRIX_II( *a0, 0,0 ),
        ARMUELLER_S_M_II( *mr,   0,0 )
        );

    sps_s_init_s(
        art_gv,
        ARMMDA_S_MATRIX_II( *a0, 0,1 ),
        ARMUELLER_S_M_II( *mr,   0,1 )
        );
    sps_s_init_s(
        art_gv,
        ARMMDA_S_MATRIX_II( *a0, 0,2 ),
        ARMUELLER_S_M_II( *mr,   0,2 )
        );

    sps_s_init_s(
        art_gv,
        ARMMDA_S_MATRIX_II( *a0, 0,3 ),
        ARMUELLER_S_M_II( *mr,   0,3 )
        );

    sps_s_init_s(
        art_gv,
        ARMMDA_S_MATRIX_II( *a0, 3,0 ),
        ARMUELLER_S_M_II( *mr,   3,0 )
        );

    sps_s_init_s(
        art_gv,
        ARMMDA_S_MATRIX_II( *a0, 3,1 ),
        ARMUELLER_S_M_II( *mr,   3,1 )
        );

    sps_s_init_s(
        art_gv,
        ARMMDA_S_MATRIX_II( *a0, 3,2 ),
        ARMUELLER_S_M_II( *mr,   3,2 )
        );

    sps_s_init_s(
        art_gv,
        ARMMDA_S_MATRIX_II( *a0, 3,3 ),
        ARMUELLER_S_M_II( *mr,   3,3 )
        );
    
    
    //   element 1,0   -----------------------------------------------------
    //   cos_2phi * m_10 + sin_2phi * m_20

    sps_ds_mul_s(
        art_gv,
        cos_2phi,
        ARMMDA_S_MATRIX_II( *a0, 1,0 ),
        ARMUELLER_S_M_II( *mr,   1,0 )
        );
    
    sps_ds_mul_add_s(
        art_gv,
        sin_2phi,
        ARMMDA_S_MATRIX_II( *a0, 2,0 ),
        ARMUELLER_S_M_II( *mr,   1,0 )
        );
    
    //   element 1,1   -----------------------------------------------------
    //   cos_2phi * m_11 + sin_2phi * m_21
    
    sps_ds_mul_s(
        art_gv,
        cos_2phi,
        ARMMDA_S_MATRIX_II( *a0, 1,1 ),
        ARMUELLER_S_M_II( *mr,   1,1 )
        );
    
    sps_ds_mul_add_s(
        art_gv,
        sin_2phi,
        ARMMDA_S_MATRIX_II( *a0, 2,1 ),
        ARMUELLER_S_M_II( *mr,   1,1 )
        );
    
    //   element 1,2   -----------------------------------------------------
    //   cos_2phi * m_12 + sin_2phi * m_22
    
    sps_ds_mul_s(
        art_gv,
        cos_2phi,
        ARMMDA_S_MATRIX_II( *a0, 1,2 ),
        ARMUELLER_S_M_II( *mr,   1,2 )
        );
    
    sps_ds_mul_add_s(
        art_gv,
        sin_2phi,
        ARMMDA_S_MATRIX_II( *a0, 2,2 ),
        ARMUELLER_S_M_II( *mr,   1,2 )
        );

    //   element 1,3   -----------------------------------------------------
    //   cos_2phi * m_13 + sin_2phi * m_23
    
    sps_ds_mul_s(
        art_gv,
        cos_2phi,
        ARMMDA_S_MATRIX_II( *a0, 1,3 ),
        ARMUELLER_S_M_II( *mr,   1,3 )
        );
    
    sps_ds_mul_add_s(
        art_gv,
        sin_2phi,
        ARMMDA_S_MATRIX_II( *a0, 2,3 ),
        ARMUELLER_S_M_II( *mr,   1,3 )
        );
    
    //   element 2,0   -----------------------------------------------------
    //   -sin_2phi * m_10 + cos_2phi * m_20
    
    sps_ds_mul_s(
        art_gv,
        -sin_2phi,
        ARMMDA_S_MATRIX_II( *a0, 1,0 ),
        ARMUELLER_S_M_II( *mr,   2,0 )
        );
    
    sps_ds_mul_add_s(
        art_gv,
        cos_2phi,
        ARMMDA_S_MATRIX_II( *a0, 2,0 ),
        ARMUELLER_S_M_II( *mr,   2,0 )
        );
    
    //   element 2,1   -----------------------------------------------------
    //   -sin_2phi * m_11 + cos_2phi * m_21
    
    sps_ds_mul_s(
        art_gv,
        -sin_2phi,
        ARMMDA_S_MATRIX_II( *a0, 1,1 ),
        ARMUELLER_S_M_II( *mr,   2,1 )
        );
    
    sps_ds_mul_add_s(
        art_gv,
        cos_2phi,
        ARMMDA_S_MATRIX_II( *a0, 2,1 ),
        ARMUELLER_S_M_II( *mr,   2,1 )
        );

    //   element 2,2   -----------------------------------------------------
    //   -sin_2phi * m_12 + cos_2phi * m_22
    
    sps_ds_mul_s(
        art_gv,
        -sin_2phi,
        ARMMDA_S_MATRIX_II( *a0, 1,2 ),
        ARMUELLER_S_M_II( *mr,   2,2 )
        );
    
    sps_ds_mul_add_s(
        art_gv,
        cos_2phi,
        ARMMDA_S_MATRIX_II( *a0, 2,2 ),
        ARMUELLER_S_M_II( *mr,   2,2 )
        );
    
    
    //   element 2,3   -----------------------------------------------------
    //   -sin_2phi * m_13 + cos_2phi * m_23
    
    sps_ds_mul_s(
        art_gv,
        -sin_2phi,
        ARMMDA_S_MATRIX_II( *a0, 1,3 ),
        ARMUELLER_S_M_II( *mr,   2,3 )
        );
    
    sps_ds_mul_add_s(
        art_gv,
        cos_2phi,
        ARMMDA_S_MATRIX_II( *a0, 2,3 ),
        ARMUELLER_S_M_II( *mr,   2,3 )
        );

    
    // The orginal rotation code seems to be not correct in my point of view
    // basically, it rorates the MM twice: rotate the entry reference
    // frame with theta angle, and then rotate the exit frame with theta angle.
    // ---- CW
    
//    /* -------------------------------------------------------------------
//        In the case of a polarising input attenuation we have to rotate
//        its Mueller matrix to alignment with the provided reference
//        system.
//
//        The formulas for this section (actually, for most of the
//        polarisation code in ART) were taken from
//
//        Self-study Manual on Optical Radiation Measurements
//        Part I - Concepts, Chapter 6
//        Fred E. Nicodemus, Editor
//        National Bureau of Standards (now NIST), 1977
//        Referenced as [Nico77]
//
//        Of particular interest to this section is formula 6.39 on page 27.
//    ----------------------------------------------------------------aw- */
//
//
//    /* -------------------------------------------------------------------
//        Here we figure out the angle between the two reference systems.
//    ----------------------------------------------------------------aw- */
//
//    double  cos_phi =
//        vec3d_vv_dot(
//            & ARMMDIRECTATTENUATIONSAMPLE_EXIT_REFFRAME_I( *a0, 0 ),
//            & ARREFFRAME_RF_I( *r0, 0 )
//            );
//
//    double  sin_phi =
//        vec3d_vv_dot(
//            & ARMMDIRECTATTENUATIONSAMPLE_EXIT_REFFRAME_I( *a0, 1 ),
//            & ARREFFRAME_RF_I( *r0, 0 )
//            );
//
//    double  cos_2phi = M_SQR( cos_phi ) - M_SQR( sin_phi );
//    double  sin_2phi = 2 * cos_phi * sin_phi;
//
//    double  sin_sqr_2phi = M_SQR( sin_2phi );
//    double  sin_cos_2phi = sin_2phi * cos_2phi;
//
//    ArSpectralSample  temp_A;
//    ArSpectralSample  temp_B;
//    ArSpectralSample  temp_0;
//    ArSpectralSample  temp_1;
//    ArSpectralSample  temp_2;
//    ArSpectralSample  temp_3;
//
//    /* -------------------------------------------------------------------
//        From here on we perform the actual rotation operation according
//        to formula 6.39.
//
//        The four corner elements remain unchanged, and after simply
//        copying them we compute a number of needed temporary variables
//        before assembling the rest of the elements.
//    ----------------------------------------------------------------aw- */
//
//    //   copied elements (0,0), (0,3), (3,0), (3,3)  ---------------------
//
//    sps_s_init_s(
//        art_gv,
//        ARMMDA_S_MATRIX_II( *a0, 0,0 ),
//        ARMUELLER_S_M_II( *mr,   0,0 )
//        );
//
//    sps_s_init_s(
//        art_gv,
//        ARMMDA_S_MATRIX_II( *a0, 0,3 ),
//        ARMUELLER_S_M_II( *mr,   0,3 )
//        );
//
//    sps_s_init_s(
//        art_gv,
//        ARMMDA_S_MATRIX_II( *a0, 3,0 ),
//        ARMUELLER_S_M_II( *mr,   3,0 )
//        );
//
//    sps_s_init_s(
//        art_gv,
//        ARMMDA_S_MATRIX_II( *a0, 3,3 ),
//        ARMUELLER_S_M_II( *mr,   3,3 )
//        );
//
//    //   temp elements   -------------------------------------------------
//
//    sps_ss_sub_s(
//        art_gv,
//          ARMMDA_S_MATRIX_II( *a0, 2,2 ),
//          ARMMDA_S_MATRIX_II( *a0, 1,1 ),
//        & temp_0
//        );
//
//    sps_ss_add_s(
//        art_gv,
//          ARMMDA_S_MATRIX_II( *a0, 1,2 ),
//          ARMMDA_S_MATRIX_II( *a0, 2,1 ),
//        & temp_1
//        );
//
//    sps_ds_mul_s(
//          art_gv,
//          sin_sqr_2phi,
//        & temp_0,
//        & temp_2
//        );
//
//    sps_ds_mul_s(
//          art_gv,
//          sin_cos_2phi,
//        & temp_1,
//        & temp_3
//        );
//
//    sps_ss_add_s(
//          art_gv,
//        & temp_2,
//        & temp_3,
//        & temp_A
//        );
//
//    sps_ds_mul_s(
//          art_gv,
//          sin_cos_2phi,
//        & temp_0,
//        & temp_2
//        );
//
//    sps_ds_mul_s(
//          art_gv,
//          sin_sqr_2phi,
//        & temp_1,
//        & temp_3
//        );
//
//    sps_ss_sub_s(
//          art_gv,
//        & temp_3,
//        & temp_2,
//        & temp_B
//        );
//
//    //   element 1,1   -----------------------------------------------------
//
//    sps_ss_sub_s(
//          art_gv,
//        & temp_A,
//          ARMMDA_S_MATRIX_II( *a0, 1,1 ),
//          ARMUELLER_S_M_II( *mr,   1,1 )
//        );
//
//    //   element 2,2   ----------------------------------------------------
//
//    sps_ss_add_s(
//          art_gv,
//          ARMMDA_S_MATRIX_II( *a0, 2,2 ),
//        & temp_A,
//          ARMUELLER_S_M_II( *mr,   2,2 )
//        );
//
//    //   element 1,2   -----------------------------------------------------
//
//    sps_ss_add_s(
//          art_gv,
//          ARMMDA_S_MATRIX_II( *a0, 1,2 ),
//        & temp_B,
//          ARMUELLER_S_M_II( *mr,   1,2 )
//        );
//
//    //   element 2,1   -----------------------------------------------------
//
//    sps_ss_add_s(
//          art_gv,
//          ARMMDA_S_MATRIX_II( *a0, 2,1 ),
//        & temp_B,
//          ARMUELLER_S_M_II( *mr,   2,1 )
//        );
//
//    //   element 0,1   -----------------------------------------------------
//
//    sps_ds_mul_s(
//          art_gv,
//          cos_2phi,
//          ARMMDA_S_MATRIX_II( *a0, 0,1 ),
//        & temp_0
//        );
//
//    sps_ds_mul_s(
//          art_gv,
//          sin_2phi,
//          ARMMDA_S_MATRIX_II( *a0, 0,2 ),
//        & temp_1
//        );
//
//    sps_ss_sub_s(
//          art_gv,
//        & temp_1,
//        & temp_0,
//          ARMUELLER_S_M_II( *mr, 0,1 )
//        );
//
//    //   element 0,2   -----------------------------------------------------
//
//    sps_ds_mul_s(
//          art_gv,
//          sin_2phi,
//          ARMMDA_S_MATRIX_II( *a0, 0,1 ),
//        & temp_0
//        );
//
//    sps_ds_mul_s(
//          art_gv,
//          cos_2phi,
//          ARMMDA_S_MATRIX_II( *a0, 0,2 ),
//        & temp_1
//        );
//
//    sps_ss_add_s(
//          art_gv,
//        & temp_0,
//        & temp_1,
//          ARMUELLER_S_M_II( *mr, 0,2 )
//        );
//
//    //   element 1,0   -----------------------------------------------------
//
//    sps_ds_mul_s(
//          art_gv,
//          cos_2phi,
//          ARMMDA_S_MATRIX_II( *a0, 1,0 ),
//        & temp_0
//        );
//
//    sps_ds_mul_s(
//          art_gv,
//          sin_2phi,
//          ARMMDA_S_MATRIX_II( *a0, 2,0 ),
//        & temp_1
//        );
//
//    sps_ss_sub_s(
//          art_gv,
//        & temp_1,
//        & temp_0,
//          ARMUELLER_S_M_II( *mr, 1,0 )
//        );
//
//    //   element 2,0   -----------------------------------------------------
//
//    sps_ds_mul_s(
//          art_gv,
//          sin_2phi,
//          ARMMDA_S_MATRIX_II( *a0, 1,0 ),
//        & temp_0
//        );
//
//    sps_ds_mul_s(
//          art_gv,
//          cos_2phi,
//          ARMMDA_S_MATRIX_II( *a0, 2,0 ),
//        & temp_1
//        );
//
//    sps_ss_add_s(
//          art_gv,
//        & temp_0,
//        & temp_1,
//          ARMUELLER_S_M_II( *mr, 2,0 )
//        );
//
//    //   element 1,3   -----------------------------------------------------
//
//    sps_ds_mul_s(
//          art_gv,
//          cos_2phi,
//          ARMMDA_S_MATRIX_II( *a0, 1,3 ),
//        & temp_0
//        );
//
//    sps_ds_mul_s(
//          art_gv,
//          sin_2phi,
//          ARMMDA_S_MATRIX_II( *a0, 2,3 ),
//        & temp_1
//        );
//
//    sps_ss_sub_s(
//          art_gv,
//        & temp_1,
//        & temp_0,
//          ARMUELLER_S_M_II( *mr, 1,3 )
//        );
//
//    //   element 2,3   ----------------------------------------------------
//
//    sps_ds_mul_s(
//          art_gv,
//          sin_2phi,
//          ARMMDA_S_MATRIX_II( *a0, 1,3 ),
//        & temp_0
//        );
//
//    sps_ds_mul_s(
//          art_gv,
//          cos_2phi,
//          ARMMDA_S_MATRIX_II( *a0, 2,3 ),
//        & temp_1
//        );
//
//    sps_ss_add_s(
//          art_gv,
//        & temp_0,
//        & temp_1,
//          ARMUELLER_S_M_II( *mr, 2,3 )
//        );
//
//    //   element 3,1   ----------------------------------------------------
//
//    sps_ds_mul_s(
//          art_gv,
//          cos_2phi,
//          ARMMDA_S_MATRIX_II( *a0, 3,1 ),
//        & temp_0
//        );
//
//    sps_ds_mul_s(
//          art_gv,
//          sin_2phi,
//          ARMMDA_S_MATRIX_II( *a0, 3,2 ),
//        & temp_1
//        );
//
//    sps_ss_sub_s(
//          art_gv,
//        & temp_1,
//        & temp_0,
//          ARMUELLER_S_M_II( *mr, 3,1 )
//        );
//
//    //   element 3,2   ----------------------------------------------------
//
//    sps_ds_mul_s(
//          art_gv,
//          sin_2phi,
//          ARMMDA_S_MATRIX_II( *a0, 3,1 ),
//        & temp_0
//        );
//
//    sps_ds_mul_s(
//          art_gv,
//          cos_2phi,
//          ARMMDA_S_MATRIX_II( *a0, 3,2 ),
//        & temp_1
//        );
//
//    sps_ss_add_s(
//          art_gv,
//        & temp_0,
//        & temp_1,
//          ARMUELLER_S_M_II( *mr, 3,2 )
//        );

}

/* ---------------------------------------------------------------------------

    'armmdirectattenuationsample_a_realign_to_coaxial_exit_rf_a'

    This procedure modifies an attenuation element to fit a different
    collinear reference coordinate system. It is basically a wrapper around
    'armmdirectattenuationsample_a_realign_to_coaxial_exit_rf_mm' - see the
    description of this function for more information and the restrictions
    on its usage.

------------------------------------------------------------------------aw- */

void armmdirectattenuationsample_a_realign_to_coaxial_exit_rf_a(
        const ART_GV                     * art_gv,
        const ArMMDirectAttenuationSample  * a0,
        const ArReferenceFrame           * r0,
              ArMMDirectAttenuationSample  * ar
        )
{
    if ( ARMMDIRECTATTENUATIONSAMPLE_IS_GENERAL_MATRIX( *a0 ) )
    {
        armmdirectattenuationsample_a_realign_to_coaxial_exit_rf_mm(
            art_gv,
            a0,
            r0,
            ARMMDA_S_MATRIX( *ar )
            );

        ARMMDIRECTATTENUATIONSAMPLE_EXIT_REFFRAME( *ar ) = *r0;

        ARMMDIRECTATTENUATIONSAMPLE_SET_PROPERTY_GENERAL_MATRIX( *ar );
    }
    else
        armmdirectattenuationsample_a_init_a(
            art_gv,
            a0,
            ar
            );
}

void armmdirectattenuationsample_realign_to_coaxial_exit_rf_a(
        const ART_GV                * art_gv,
        const ArReferenceFrame      * r0,
              ArMMDirectAttenuationSample   * ar
        )
{
    (void) art_gv;
    (void) r0;
    (void) ar;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR;
}

/* ---------------------------------------------------------------------------

    'armmdirectattenuationsample_a_min'
    'armmdirectattenuationsample_a_max'
    'armmdirectattenuationsample_a_average'
    'armmdirectattenuationsample_a_norm'

    These functions compute the min, max, average and L1 norm of a given
    attenuation element. Only entry (0,0) of the matrix is used for this,
    since the polarisation aspect does not enter into absolute attenuation
    properties such as these.

------------------------------------------------------------------------aw- */

double armmdirectattenuationsample_a_avg(
        const ART_GV                     * art_gv,
        const ArMMDirectAttenuationSample  * a0
        )
{
    return
        sps_s_avg(
            art_gv,
            ARMMDIRECTATTENUATIONSAMPLE( *a0 )
            );
}

double armmdirectattenuationsample_a_norm(
        const ART_GV                     * art_gv,
        const ArMMDirectAttenuationSample  * a0
        )
{
    return
        sps_s_l1_norm(
            art_gv,
            ARMMDIRECTATTENUATIONSAMPLE( *a0 )
            );
}

double armmdirectattenuationsample_a_max(
        const ART_GV                     * art_gv,
        const ArMMDirectAttenuationSample  * a0
        )
{
    return
        sps_s_max(
            art_gv,
            ARMMDIRECTATTENUATIONSAMPLE( *a0 )
            );
}

double armmdirectattenuationsample_a_min(
        const ART_GV                     * art_gv,
        const ArMMDirectAttenuationSample  * a0
        )
{
    return
        sps_s_min(
            art_gv,
            ARMMDIRECTATTENUATIONSAMPLE( *a0 )
            );
}

void armmdirectattenuationsample_a_inv_a(
        const ART_GV                     * art_gv,
        const ArMMDirectAttenuationSample  * a0,
              ArMMDirectAttenuationSample  * ar
        )
{
    (void) art_gv;
    (void) a0;
    (void) ar;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR;
}

void armmdirectattenuationsample_inv_a(
        const ART_GV                     * art_gv,
              ArMMDirectAttenuationSample  * ar
        )
{
    (void) art_gv;
    (void) ar;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR;
}

void armmdirectattenuationsample_aa_max_a(
        const ART_GV                     * art_gv,
        const ArMMDirectAttenuationSample  * a0,
        const ArMMDirectAttenuationSample  * a1,
              ArMMDirectAttenuationSample  * ar
        )
{
    (void) art_gv;
    (void) a0;
    (void) a1;
    (void) ar;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR;
}

void armmdirectattenuationsample_aa_min_a(
        const ART_GV                     * art_gv,
        const ArMMDirectAttenuationSample  * a0,
        const ArMMDirectAttenuationSample  * a1,
              ArMMDirectAttenuationSample  * ar
        )
{
    (void) art_gv;
    (void) a0;
    (void) a1;
    (void) ar;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR;
}

double armmdirectattenuationsample_aa_maxdiff(
        const ART_GV                     * art_gv,
        const ArMMDirectAttenuationSample  * a0,
        const ArMMDirectAttenuationSample  * a1
        )
{
    (void) art_gv;
    (void) a0;
    (void) a1;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR;
    return 0.0;
}

/* ---------------------------------------------------------------------------

    'armmdirectattenuationsample_aa_add_a'
    'armmdirectattenuationsample_a_add_a'
    'armmdirectattenuationsample_aa_sub_a'
    'armmdirectattenuationsample_ad_mul_add_a'

    The first three functions add and subtract attenuation elements; the
    fourth one is a shorthand for multiplying one attenuation by a scalar
    and then adding it to another one.

    All these operations are not physically plausible and SHOULD BE AVOIDED!
    Proper concatenation of attenuation elements can only be performed
    through multiplication (e.g. by 'armmdirectattenuationsample_aa_mul_a').

    However, some of the less than perfectly realistic components of ART
    (e.g. ArnGeneralSurfaceMaterial) need such functions, so here they are.
    But when using them one should always keep in mind that

    a) they are EVIL from a physics standpoint and
    b) their results are probably complete garbage for polarised light.

    We are providing some best-guess code here in order not to actively
    break the polarised renderer if non-realistic components are used, but
    the chances that it does something particularly meaningful are
    unfortunately not all that good.

    UPDATE:

    It turns out, that piece-wise algebraic operations on Mueller matrices
    are not necessarily as nonsensical as they may seem at first (or second)
    sight. It can be shown that piece-wise summing up two MMs yields a
    MM which will generate Stokes vectors equal to the piece-wise sum of
    the two Stokes vectors generated by the original MMs. This is something
    we consider a desired property.

---------------------------------------------------------------------ip-aw- */

void armmdirectattenuationsample_aa_add_a(
        const ART_GV                     * art_gv,
        const ArMMDirectAttenuationSample  * a0,
        const ArMMDirectAttenuationSample  * a1,
              ArMMDirectAttenuationSample  * ar
        )
{
    ASSERT_VALID_MM_DIRECT_ATTENUATION_SAMPLE( a0 );
    ASSERT_VALID_MM_DIRECT_ATTENUATION_SAMPLE( a1 );

    ArMuellerMatrixSample  * m0 = armuellermatrixsample_alloc( art_gv );

    //  This function aligns the reference frames for purposes of
    //  addition of two Mueller matrices and sets them to the result
    //  attenuation structure.                                             (ip)

    armmdirectattenuationsample_aa_align_for_add_and_extract_matrix_am(
        art_gv, a0, a1, ar, m0
        );

    //  Addition itself

    armuellermatrixsample_mm_add_m(
        art_gv,
        m0,
        ARMMDA_S_MATRIX( *a1 ),
        ARMMDA_S_MATRIX( *ar )
        );

    //  Setting the properties of the resulting attenuation

    if (   ARMMDIRECTATTENUATIONSAMPLE_IS_DEPOLARISER( *a0 )
        && ARMMDIRECTATTENUATIONSAMPLE_IS_DEPOLARISER( *a1 ) )
    {
        //  Both MMs are depolarisers; the result is also a depolariser    (ip)

        ARMMDIRECTATTENUATIONSAMPLE_SET_PROPERTY_DEPOLARISER( *ar );
    }
    else if (   ARMMDIRECTATTENUATIONSAMPLE_IS_ROTATIONALLY_INVARIANT( *a0 )
             && ARMMDIRECTATTENUATIONSAMPLE_IS_ROTATIONALLY_INVARIANT( *a1 ) )
    {
        ART__CODE_IS_NOT_TESTED__EXIT_WITH_ERROR

        //  Depolarising and/or non-polarising MMs; the result is rotationally
        //  invariant.                                                     (ip)

        ARMMDIRECTATTENUATIONSAMPLE_SET_PROPERTY_ROTATIONALLY_INVARIANT( *ar );
    }
    else
    {
        //  At least one of the matrices is general; the result is a general
        //  matrix                                                         (ip)

        ARMMDIRECTATTENUATIONSAMPLE_SET_PROPERTY_GENERAL_MATRIX( *ar );
    }

    armuellermatrixsample_free( art_gv, m0 );


    //  TODO: The following is an incomplete optimisation attempt. To make
    //  optimisations like this work, all code manipulating MMs must be aware
    //  of the fact that in some cases some MM components are not valid and
    //  MUST NOT be touched and that is not true at the moment this comment
    //  was written at. Moreover, this particular optimisation will have to be
    //  extended. For instance: the last branch handles also the case when
    //  one of the matrices has some of the fields invalid. That will,
    //  obviously, lead to an invalid result, etc.                         (ip)


    //  ADD TODO: For testing purposes, when foundation assertions are on,
    //  the unset components should be set to some invalid value (Nan?) which
    //  will cause a runtime error when accessed. This will speed up searching
    //  for code which is unaware of this optimization.                    (ip)


    //if (   ARMMDIRECTATTENUATIONSAMPLE_IS_DEPOLARISER( *a0 )
    //    && ARMMDIRECTATTENUATIONSAMPLE_IS_DEPOLARISER( *a1 ) )
    //{
    //    ART__CODE_IS_NOT_TESTED__EXIT_WITH_ERROR
    //
    //    //  Both MMs are depolarisers. Just directly add the non-polarising
    //    //  MMs components and the result is also depolariser.             (ip)
    //
    //    sps_ss_add_s(
    //        art_gv,
    //        ARMUELLER_S_M_I( *m0, 0 ),
    //        ARMUELLER_S_M_I( *m1, 0 ),
    //        ARMMDA_S_MATRIX_I( *ar, 0 )
    //        );
    //
    //    ARMMDIRECTATTENUATIONSAMPLE_SET_PROPERTY_DEPOLARISER( *ar );
    //}
    //else if (   ARMMDIRECTATTENUATIONSAMPLE_IS_ROTATIONALLY_INVARIANT( *a0 )
    //         && ARMMDIRECTATTENUATIONSAMPLE_IS_ROTATIONALLY_INVARIANT( *a1 ) )
    //{
    //    ART__CODE_IS_NOT_TESTED__EXIT_WITH_ERROR
    //
    //    //  Depolarising and/or non-polarising MMs. Therefore, we can reduce
    //    //  the process to operation on the diagonals only. The result is also
    //    //  rotationally invariant.                                        (ip)
    //
    //    for (int element = 0; element < 4; element++)
    //    {
    //        sps_ss_add_s(
    //            art_gv,
    //            ARMUELLER_S_M_II( *m0, element, element ),
    //            ARMUELLER_S_M_II( *m1, element, element ),
    //            ARMMDA_S_MATRIX_II( *ar, element, element )
    //            );
    //    }
    //
    //    ARMMDIRECTATTENUATIONSAMPLE_SET_PROPERTY_ROTATIONALLY_INVARIANT( *ar );
    //}
    //else
    //{
    //    ART__CODE_IS_NOT_TESTED__EXIT_WITH_ERROR
    //
    //    //  At least one of the matrices is general one. We have to do the full
    //    //  addition and the result is a general matrix.                   (ip)
    //
    //    armuellermatrixsample_mm_add_m(art_gv, m0, m1, ARMMDA_S_MATRIX( *ar ));
    //
    //    ARMMDIRECTATTENUATIONSAMPLE_SET_PROPERTY_GENERAL_MATRIX( *ar );
    //}

    ASSERT_VALID_MM_DIRECT_ATTENUATION_SAMPLE( ar );
}

void armmdirectattenuationsample_a_add_a(
        const ART_GV                     * art_gv,
        const ArMMDirectAttenuationSample  * a0,
              ArMMDirectAttenuationSample  * ar
        )
{
    ASSERT_VALID_MM_DIRECT_ATTENUATION_SAMPLE( a0 );
    ASSERT_VALID_MM_DIRECT_ATTENUATION_SAMPLE( ar );

    ArMuellerMatrixSample  * m0 = armuellermatrixsample_alloc( art_gv );

    //  This function aligns the reference frames for purposes of
    //  addition of two Mueller matrices and sets them to the result
    //  attenuation structure.                                             (ip)

    armmdirectattenuationsample_a_align_for_add_and_extract_matrix_am(
        art_gv, a0, ar, m0
        );

    //  Addition itself

    armuellermatrixsample_m_add_m(
        art_gv,
        m0,
        ARMMDA_S_MATRIX( *ar )
        );

    //  Setting the properties of the resulting attenuation

    if (   ARMMDIRECTATTENUATIONSAMPLE_IS_DEPOLARISER( *a0 )
        && ARMMDIRECTATTENUATIONSAMPLE_IS_DEPOLARISER( *ar ) )
    {
        //  Both MMs are depolarisers; the result stays a depolariser      (ip)

        ARMMDIRECTATTENUATIONSAMPLE_SET_PROPERTY_DEPOLARISER( *ar );
    }
    else if (   ARMMDIRECTATTENUATIONSAMPLE_IS_ROTATIONALLY_INVARIANT( *a0 )
             && ARMMDIRECTATTENUATIONSAMPLE_IS_ROTATIONALLY_INVARIANT( *ar ) )
    {
        ART__CODE_IS_NOT_TESTED__EXIT_WITH_ERROR

        //  Depolarising and/or non-polarising MMs; the result stays
        //  rotationally invariant.                                        (ip)

        ARMMDIRECTATTENUATIONSAMPLE_SET_PROPERTY_ROTATIONALLY_INVARIANT( *ar );
    }
    else
    {
        //  At least one of the matrices is general; the result is a general
        //  matrix                                                         (ip)

        ARMMDIRECTATTENUATIONSAMPLE_SET_PROPERTY_GENERAL_MATRIX( *ar );
    }

    armuellermatrixsample_free( art_gv, m0 );


    //  TODO: The following is an incomplete optimisation attempt. To make
    //  optimisations like this work, all code manipulating MMs must be aware
    //  of the fact that in some cases some MM components are not valid and
    //  MUST NOT be touched and that is not true at the moment this comment
    //  was written at. Moreover, this particular optimisation will have to be
    //  extended. For instance: the last branch handles also the case when
    //  one of the matrices has some of the fields invalid. That will,
    //  obviously, lead to an invalid result, etc.                         (ip)


    //  ADD TODO: For testing purposes, when foundation assertions are on,
    //  the unset components should be set to some invalid value (Nan?) which
    //  will cause a runtime error when accessed. This will speed up searching
    //  for code which is unaware of this optimization.                    (ip)


    //if (   ARMMDIRECTATTENUATIONSAMPLE_IS_DEPOLARISER( *a0 )
    //    && ARMMDIRECTATTENUATIONSAMPLE_IS_DEPOLARISER( *ar ) )
    //{
    //    ART__CODE_IS_NOT_TESTED__EXIT_WITH_ERROR
    //
    //    //  Both MMs are depolarisers. Just directly add the non-polarising
    //    //  MMs components and the result is also depolariser.             (ip)
    //
    //    sps_ss_add_s(
    //        art_gv,
    //        ARMUELLER_S_M_I( *m0, 0 ),
    //        ARMUELLER_S_M_I( *m1, 0 ),
    //        ARMMDA_S_MATRIX_I( *ar, 0 )
    //        );
    //
    //    ARMMDIRECTATTENUATIONSAMPLE_SET_PROPERTY_DEPOLARISER( *ar );
    //}
    //else if (   ARMMDIRECTATTENUATIONSAMPLE_IS_ROTATIONALLY_INVARIANT( *a0 )
    //         && ARMMDIRECTATTENUATIONSAMPLE_IS_ROTATIONALLY_INVARIANT( *ar ) )
    //{
    //    ART__CODE_IS_NOT_TESTED__EXIT_WITH_ERROR
    //
    //    //  Depolarising and/or non-polarising MMs. Therefore, we can reduce
    //    //  the process to operation on the diagonals only. The result is also
    //    //  rotationally invariant.                                        (ip)
    //
    //    for (int element = 0; element < 4; element++)
    //    {
    //        sps_ss_add_s(
    //            art_gv,
    //            ARMUELLER_S_M_II( *m0, element, element ),
    //            ARMUELLER_S_M_II( *m1, element, element ),
    //            ARMMDA_S_MATRIX_II( *ar, element, element )
    //            );
    //    }
    //
    //    ARMMDIRECTATTENUATIONSAMPLE_SET_PROPERTY_ROTATIONALLY_INVARIANT( *ar );
    //}
    //else
    //{
    //    ART__CODE_IS_NOT_TESTED__EXIT_WITH_ERROR
    //
    //    //  At least one of the matrices is general one. We have to do the full
    //    //  addition and the result is a general matrix.                   (ip)
    //
    //    armuellermatrixsample_mm_add_m(art_gv, m0, m1, ARMMDA_S_MATRIX( *ar ));
    //
    //    ARMMDIRECTATTENUATIONSAMPLE_SET_PROPERTY_GENERAL_MATRIX( *ar );
    //}

    ASSERT_VALID_MM_DIRECT_ATTENUATION_SAMPLE( ar );
}

void armmdirectattenuationsample_aa_sub_a(
        const ART_GV                     * art_gv,
        const ArMMDirectAttenuationSample  * a0,
        const ArMMDirectAttenuationSample  * a1,
              ArMMDirectAttenuationSample  * ar
        )
{
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    ASSERT_VALID_MM_DIRECT_ATTENUATION_SAMPLE( a0 );
    ASSERT_VALID_MM_DIRECT_ATTENUATION_SAMPLE( a1 );

    sps_ss_sub_s(
        art_gv,
        ARMMDIRECTATTENUATIONSAMPLE_MM_I( *a0, 0 ),
        ARMMDIRECTATTENUATIONSAMPLE_MM_I( *a1, 0 ),
        ARMMDIRECTATTENUATIONSAMPLE_MM_I( *ar, 0 )
        );

    if (   ARMMDIRECTATTENUATIONSAMPLE_IS_GENERAL_MATRIX( *a0 )
        || ARMMDIRECTATTENUATIONSAMPLE_IS_GENERAL_MATRIX( *a1 ) )
    {
        ArMuellerMatrixSample  * m0 = armuellermatrixsample_alloc( art_gv );
        ArMuellerMatrixSample  * m1 = ARMMDA_S_MATRIX( *a1 );

        //  This function aligns the reference frames for purposes of
        //  addition of two Mueller matrices and sets them to the result
        //  attenuation structure. It also works for subtraction.          (ip)

        armmdirectattenuationsample_aa_align_for_add_and_extract_matrix_am(
            art_gv, a0, a1, ar, m0
            );

        for ( unsigned int i = 1; i < 16; i++ )
            sps_ss_sub_s(
                art_gv,
                ARMUELLER_S_M_I( *m0, i ),
                ARMUELLER_S_M_I( *m1, i ),
                ARMMDA_S_MATRIX_I( *ar, i )
                );

        armuellermatrixsample_free( art_gv, m0 );
    }
    else
        ARMMDIRECTATTENUATIONSAMPLE_SET_PROPERTY_ROTATIONALLY_INVARIANT( *ar );

    ASSERT_VALID_MM_DIRECT_ATTENUATION_SAMPLE( ar );
}

void armmdirectattenuationsample_da_mul_add_a(
        const ART_GV                     * art_gv,
        const double                       d0,
        const ArMMDirectAttenuationSample  * a0,
              ArMMDirectAttenuationSample  * ar
        )
{
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    ASSERT_VALID_MM_DIRECT_ATTENUATION_SAMPLE( a0 );
    ASSERT_VALID_MM_DIRECT_ATTENUATION_SAMPLE( ar );

    sps_ds_mul_add_s(
        art_gv,
        d0,
        ARMMDIRECTATTENUATIONSAMPLE_MM_I( *a0, 0 ),
        ARMMDIRECTATTENUATIONSAMPLE_MM_I( *ar, 0 )
        );

    if (   ARMMDIRECTATTENUATIONSAMPLE_IS_GENERAL_MATRIX( *a0 )
        || ARMMDIRECTATTENUATIONSAMPLE_IS_GENERAL_MATRIX( *ar ) )
    {
        ArMuellerMatrixSample  * m0 = armuellermatrixsample_alloc( art_gv );
        ArMuellerMatrixSample  * m1 = armuellermatrixsample_alloc( art_gv );

        // FIXME: This function aligns reference frames for the concatenating
        //        scenario, which is not what we need in case of
        //        addition or subtraction.

        armmdirectattenuationsample_a_align_for_mul_and_extract_matrices_amm(
            art_gv, a0, ar, m0, m1
            );

        for ( unsigned int i = 1; i < 16; i++ )
        {
            sps_d_mul_s(
                art_gv,
                d0,
                ARMUELLER_S_M_I( *m0, i )
                );

            sps_ss_add_s(
                art_gv,
                ARMUELLER_S_M_I( *m0, i ),
                ARMUELLER_S_M_I( *m1, i ),
                ARMMDA_S_MATRIX_I( *ar, i )
                );
        }

        armuellermatrixsample_free( art_gv, m0 );
        armuellermatrixsample_free( art_gv, m1 );
    }
    else
        ARMMDIRECTATTENUATIONSAMPLE_SET_PROPERTY_ROTATIONALLY_INVARIANT( *ar );

    ASSERT_VALID_MM_DIRECT_ATTENUATION_SAMPLE( ar );
}

/* ---------------------------------------------------------------------------

    'armmdirectattenuationsample_d_mul_a'
    'armmdirectattenuationsample_ad_mul_a'

    Mutiplies an attenuation element with a scalar. Two- and three operand
    versions are provided.

------------------------------------------------------------------------aw- */

void armmdirectattenuationsample_d_mul_a(
        const ART_GV                     * art_gv,
        const double                       d0,
              ArMMDirectAttenuationSample  * ar
        )
{
    ASSERT_VALID_MM_DIRECT_ATTENUATION_SAMPLE( ar );

    sps_d_mul_s(
        art_gv,
        d0,
        ARMMDIRECTATTENUATIONSAMPLE( *ar )
        );

    if ( ARMMDIRECTATTENUATIONSAMPLE_HAS_FULL_MATRIX( *ar ) )
    {
        for ( unsigned int i = 1; i < 16; i++ )
            sps_d_mul_s(
                art_gv,
                d0,
                ARMMDA_S_MATRIX_I( *ar, i )
                );
    }

    ASSERT_VALID_MM_DIRECT_ATTENUATION_SAMPLE( ar );
}

void armmdirectattenuationsample_s_mul_a(
        const ART_GV                       * art_gv,
        const ArSpectralSample             * h0,
              ArMMDirectAttenuationSample  * ar
        )
{
    ASSERT_VALID_MM_DIRECT_ATTENUATION_SAMPLE( ar );

    sps_s_mul_s(
        art_gv,
        h0,
        ARMMDIRECTATTENUATIONSAMPLE( *ar )
        );

    if ( ARMMDIRECTATTENUATIONSAMPLE_HAS_FULL_MATRIX( *ar ) )
    {
        for ( unsigned int i = 1; i < 16; i++ )
            sps_s_mul_s(
                art_gv,
                h0,
                ARMMDA_S_MATRIX_I( *ar, i )
                );
    }

    ASSERT_VALID_MM_DIRECT_ATTENUATION_SAMPLE( ar );
}

void armmdirectattenuationsample_da_mul_a(
        const ART_GV                     * art_gv,
        const double                       d0,
        const ArMMDirectAttenuationSample  * a0,
              ArMMDirectAttenuationSample  * ar
        )
{
    ASSERT_VALID_MM_DIRECT_ATTENUATION_SAMPLE( a0 );

    sps_ds_mul_s(
        art_gv,
        d0,
        ARMMDIRECTATTENUATIONSAMPLE( *a0 ),
        ARMMDIRECTATTENUATIONSAMPLE( *ar )
        );

    if ( ARMMDIRECTATTENUATIONSAMPLE_HAS_FULL_MATRIX( *ar ) )
    {
        for ( unsigned int i = 1; i < 16; i++ )
            sps_ds_mul_s(
                art_gv,
                d0,
                ARMMDA_S_MATRIX_I( *a0, i ),
                ARMMDA_S_MATRIX_I( *ar, i )
                );
    }

    ARMMDA_S_REFFRAME_ENTRY( *ar ) = ARMMDA_S_REFFRAME_ENTRY( *a0 );
    ARMMDA_S_REFFRAME_EXIT( *ar )  = ARMMDA_S_REFFRAME_EXIT( *a0 );

    ARMMDA_S_PROPERTIES( *ar ) = ARMMDA_S_PROPERTIES( *a0 );

    ASSERT_VALID_MM_DIRECT_ATTENUATION_SAMPLE( ar );
}

/* ---------------------------------------------------------------------------

    'armmdirectattenuationsample_a_mul_a'
    'armmdirectattenuationsample_aa_mul_a'

        These two functions perform the concatenation of attenuation
        elements through multiplications.

        CAVEAT: In order to be consistent with the matrix-based formalism
                from the source [Nico77] mentioned in the comments for
                'armmdirectattenuationsample_a_realign_to_coaxial_exit_rf_mm', these
                functions internally use a "reversed" notion of light traversal
                through attenuation elements insofar as the traversal order is
                assumed to be from right to left, instead of the seemingly more
                logical left to right which the order of operators in the
                function names would suggest.

                For example, if one considers light L0 that is subjected to
                two attenuations and arrives at X, the chain of matrices and
                vectors is written like this by [Nico77]:

                X  <- A1  <-  A0   <-  L0

                The net result of this is that the order of the operands
                is reversed internally, i.e. that the marix multiplications
                which are actually performed are

                a_r = a_r . a0    and    a_r = a_1 . ar

                instead of

                a_r = a0 . a_r    and    a_r = a0 . a_1

                as the semantics of the notation would suggest.

------------------------------------------------------------------------aw- */

void armmdirectattenuationsample_a_mul_a(
        const ART_GV                     * art_gv,
        const ArMMDirectAttenuationSample  * a0,
              ArMMDirectAttenuationSample  * ar
        )
{
    ASSERT_VALID_MM_DIRECT_ATTENUATION_SAMPLE( a0 );
    ASSERT_VALID_MM_DIRECT_ATTENUATION_SAMPLE( ar );

    ArMMDirectAttenuationSample  * result = armmdirectattenuationsample_alloc( art_gv );

    armmdirectattenuationsample_aa_mul_a(
        art_gv,
        a0,
        ar,
        result
        );

    armmdirectattenuationsample_a_init_a(
        art_gv,
        result,
        ar
        );

    armmdirectattenuationsample_free( art_gv, result );

    ASSERT_VALID_MM_DIRECT_ATTENUATION_SAMPLE( ar );
}

void armmdirectattenuationsample_aa_mul_a(
        const ART_GV                     * art_gv,
        const ArMMDirectAttenuationSample  * a0,
        const ArMMDirectAttenuationSample  * a1,
              ArMMDirectAttenuationSample  * ar
        )
{
    ASSERT_VALID_MM_DIRECT_ATTENUATION_SAMPLE( a0 );
    ASSERT_VALID_MM_DIRECT_ATTENUATION_SAMPLE( a1 );

    ArMuellerMatrixSample  * m0 = armuellermatrixsample_alloc( art_gv );
    ArMuellerMatrixSample  * m1 = armuellermatrixsample_alloc( art_gv );

    armmdirectattenuationsample_aa_align_for_mul_and_extract_matrices_amm(
        art_gv, a0, a1, ar, m0, m1
        );

    armuellermatrixsample_mm_mul_m(
        art_gv,
        m1,
        m0,
        ARMMDA_S_MATRIX( *ar )
        );

    if (    ARMMDIRECTATTENUATIONSAMPLE_IS_DEPOLARISER( *a0 )
         && ARMMDIRECTATTENUATIONSAMPLE_IS_DEPOLARISER( *a1 ))
    {
        //  Two depolarisers yield another one                             (ip)

        //  TODO: No matter what kind of MM is the first one. If the second
        //        operation is a depolariser, concatenating the two will always
        //        result in depolarising MM.

        ARMMDIRECTATTENUATIONSAMPLE_SET_PROPERTY_DEPOLARISER( *ar );
    }
    else if (    ARMMDIRECTATTENUATIONSAMPLE_IS_ROTATIONALLY_INVARIANT( *a0 )
              && ARMMDIRECTATTENUATIONSAMPLE_IS_ROTATIONALLY_INVARIANT( *a1 ))
    {
        //  Two rotationally invariant MMs yield a rotationally invariant MM
        //                                                                 (ip)

        ARMMDIRECTATTENUATIONSAMPLE_SET_PROPERTY_ROTATIONALLY_INVARIANT( *ar );
    }
    else
    {
        //  We assume a general matrix as result as soon as at least one
        //  general matrix is involved.                                    (ip)

        ARMMDIRECTATTENUATIONSAMPLE_SET_PROPERTY_GENERAL_MATRIX( *ar );
    }


    //  TODO: The following is an incomplete optimisation attempt. To make
    //  optimisations like this work, all code manipulating MMs must be aware
    //  of the fact that in some cases some MM components are not valid and
    //  MUST NOT be touched and that is not true at the moment this comment
    //  was written at. Moreover, this particular optimisation will have to be
    //  extended. For instance: the last branch handles also the case when
    //  one of the matrices has some of the fields invalid. That will,
    //  obviously, lead to an invalid result, etc.                         (ip)


    //  ADD TODO: For testing purposes, when foundation assertions are on,
    //  the unset components should be set to some invalid value (Nan?) which
    //  will cause a runtime error when accessed. This will speed up searching
    //  for code which is unaware of this optimization.                    (ip)


    //if (    ARMMDIRECTATTENUATIONSAMPLE_IS_DEPOLARISER( *a0 )
    //     && ARMMDIRECTATTENUATIONSAMPLE_IS_DEPOLARISER( *ar ))
    //{
    //    ART__CODE_IS_NOT_TESTED__EXIT_WITH_ERROR
    //
    //    //  Two depolarisers yield another one. Just multiplying the
    //    //  non-polarising components is sufficient in such case.          (ip)
    //
    //    sps_ss_mul_s(
    //          art_gv,
    //          ARMMDIRECTATTENUATIONSAMPLE( *a0 ),
    //          ARMMDIRECTATTENUATIONSAMPLE( *a1 ),
    //          ARMMDIRECTATTENUATIONSAMPLE( *ar )
    //        );
    //
    //    ARMMDIRECTATTENUATIONSAMPLE_SET_PROPERTY_DEPOLARISER( *ar );
    //}
    //else if (    ARMMDIRECTATTENUATIONSAMPLE_IS_ROTATIONALLY_INVARIANT( *a0 )
    //          && ARMMDIRECTATTENUATIONSAMPLE_IS_ROTATIONALLY_INVARIANT( *ar ))
    //{
    //    ART__CODE_IS_NOT_TESTED__EXIT_WITH_ERROR
    //
    //    //  Two depolarisers yield another one.
    //    //  Since it is assumed that the only rotationally invariant MM which
    //    //  is also non-depolarising is the non-polarising MM (non-diagonal
    //    //  components are zero), we can reduce the process to multiplication
    //    //  of the diagonal and the SV.                                    (ip)
    //
    //    for (int element = 0; element < 4; element++)
    //    {
    //        sps_ss_mul_s(
    //            art_gv,
    //            ARMMDIRECTATTENUATIONSAMPLE_MM_II( *a0, element, element ),
    //            ARMMDIRECTATTENUATIONSAMPLE_MM_II( *a1, element, element ),
    //            ARMMDIRECTATTENUATIONSAMPLE_MM_II( *ar, element, element )
    //            );
    //    }
    //
    //    ARMMDIRECTATTENUATIONSAMPLE_SET_PROPERTY_ROTATIONALLY_INVARIANT( *ar );
    //}
    //else
    //{
    //    ART__CODE_IS_NOT_TESTED__EXIT_WITH_ERROR
    //
    //    //  We assume a general matrix as result as soon as at least one
    //    //  general matrix is involved.                                    (ip)
    //
    //    armuellermatrixsample_mm_mul_m(
    //        art_gv,
    //        m1,
    //        m0,
    //        ARMMDA_S_MATRIX( *ar )
    //        );
    //
    //    ARMMDIRECTATTENUATIONSAMPLE_SET_PROPERTY_GENERAL_MATRIX( *ar );
    //}

    armuellermatrixsample_free( art_gv, m0 );
    armuellermatrixsample_free( art_gv, m1 );

    ASSERT_VALID_MM_DIRECT_ATTENUATION_SAMPLE( ar );
}

void armmdirectattenuationsample_d_div_a(
        const ART_GV                     * art_gv,
        const double                       d0,
              ArMMDirectAttenuationSample  * ar
        )
{
    (void) art_gv;
    (void) d0;
    (void) ar;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR;
}

void armmdirectattenuationsample_da_div_a(
        const ART_GV                     * art_gv,
        const double                       d0,
        const ArMMDirectAttenuationSample  * a0,
              ArMMDirectAttenuationSample  * ar
        )
{
    (void) art_gv;
    (void) d0;
    (void) a0;
    (void) ar;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR;
}

void armmdirectattenuationsample_da_pow_a(
        const ART_GV                     * art_gv,
        const double                       d0,
        const ArMMDirectAttenuationSample  * a0,
              ArMMDirectAttenuationSample  * ar
        )
{
    (void) art_gv;
    (void) d0;
    (void) a0;
    (void) ar;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR;
}

void armmdirectattenuationsample_da_negpow_a(
        const ART_GV                     * art_gv,
        const double                       d0,
        const ArMMDirectAttenuationSample  * a0,
              ArMMDirectAttenuationSample  * ar
        )
{
    (void) art_gv;
    (void) d0;
    (void) a0;
    (void) ar;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR;
}

void armmdirectattenuationsample_dda_clamp_a(
        const ART_GV                     * art_gv,
        const double                       d0,
        const double                       d1,
        const ArMMDirectAttenuationSample  * a0,
              ArMMDirectAttenuationSample  * ar
        )
{
    (void) art_gv;
    (void) d0;
    (void) d1;
    (void) a0;
    (void) ar;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR;
}

void armmdirectattenuationsample_daa_interpol_a(
        const ART_GV                     * art_gv,
        const double                       d0,
        const ArMMDirectAttenuationSample  * a0,
        const ArMMDirectAttenuationSample  * a1,
              ArMMDirectAttenuationSample  * ar
        )
{
    (void) art_gv;
    (void) d0;
    (void) a0;
    (void) a1;
    (void) ar;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR;
}

void armmdirectattenuationsample_a_complement_from_one_a(
        const ART_GV                     * art_gv,
        const ArMMDirectAttenuationSample  * a0,
              ArMMDirectAttenuationSample  * ar
        )
{
    (void) art_gv;
    (void) a0;
    (void) ar;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR;
}

void armmdirectattenuationsample_ia_singleband_complement_from_one_a(
        const ART_GV                     * art_gv,
        const unsigned int                 i0,
        const ArMMDirectAttenuationSample  * a0,
              ArMMDirectAttenuationSample  * ar
        )
{
    (void) art_gv;
    (void) i0;
    (void) a0;
    (void) ar;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR;
}

void armmdirectattenuationsample_dia_singleband_mul_copy_a(
        const ART_GV                     * art_gv,
        const double                       d0,
        const unsigned int                 i0,
        const ArMMDirectAttenuationSample  * a0,
              ArMMDirectAttenuationSample  * ar
        )
{
    (void) art_gv;
    (void) d0;
    (void) i0;
    (void) a0;
    (void) ar;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR;
}

/* ---------------------------------------------------------------------------

    'armmdirectattenuationsample_a_mul_l'
    'armmdirectattenuationsample_al_mul_l'

        These two functions provide the core functionality of calculating the
        interaction between light and attenuation elements. Two- and
        three-operator versions are provided.

        CAVEAT: In order to be consistent with the matrix-based formalism
                from the source mentioned in the comments for
                'ardirectattenuationsample_a_with_ref_to_mm', these functions internally
                use the same "reversed" notion of light traversal discussed in
                the comments for 'ardirectattenuationsample_aa_mul_a'.

------------------------------------------------------------------------aw- */

void armmdirectattenuationsample_a_mul_l(
        const ART_GV                     * art_gv,
        const ArMMDirectAttenuationSample  * a0,
              ArSVLightSample              * lr
        )
{
    ASSERT_VALID_MM_DIRECT_ATTENUATION_SAMPLE( a0 )
    ASSERT_VALID_SV_LIGHT_SAMPLE( lr )

    if ( ARMMDIRECTATTENUATIONSAMPLE_IS_GENERAL_MATRIX( *a0 ) )
    {
        /* -------------------------------------------------------------------
            Case 1: Polarising attenuation - we have to do full or almost
                    full attenuation-light multiplication.
        -------------------------------------------------------------ip-aw- */

        ArStokesVectorSample   * s0 = arstokesvectorsample_alloc( art_gv );
        ArMuellerMatrixSample  * m0 = armuellermatrixsample_alloc( art_gv );

        armmdirectattenuationsample_a_to_mm(
              art_gv,
              a0,
              m0
            );

        if ( ARSVLIGHTSAMPLE_POLARISED( *lr ) )
        {
            /* ---------------------------------------------------------------
                Case 1.1: Polarised light - we have to rotate the light to
                          match the attenuation entry refframe before
                          multiplication.
            ------------------------------------------------------------ip- */

            arsvlightsample_ld_realign_to_coaxial_refframe_sv(
                  art_gv,
                  lr,
                  3.0 DEGREES,
                & ARMMDIRECTATTENUATIONSAMPLE_ENTRY_REFFRAME( *a0 ),
                  s0
                );
        }
        else
        {
            /* ---------------------------------------------------------------
                Case 1.2: Depolarised light - we can skip the rotation of
                          the light to the attenuation entry frame
                          and use it directly as it is.
            ------------------------------------------------------------ip- */

            arsvlightsample_l_to_sv(
                art_gv,
                lr,
                s0
                );
        }

        arstokesvectorsample_sv_mm_mul_sv(
              art_gv,
              s0,
              m0,
            & ARSVLIGHTSAMPLE_SV( *lr )
            );

        ARSVLIGHTSAMPLE_REFFRAME( *lr ) = ARMMDA_S_REFFRAME_EXIT( *a0 );

        ARSVLIGHTSAMPLE_POLARISED( *lr ) = YES;

        arstokesvectorsample_free( art_gv, s0 );
        armuellermatrixsample_free( art_gv, m0 );
    }

    else if (      ARMMDIRECTATTENUATIONSAMPLE_IS_ROTATIONALLY_INVARIANT( *a0 )
              &&   ARSVLIGHTSAMPLE_POLARISED( *lr )
              && ! ARMMDIRECTATTENUATIONSAMPLE_IS_DEPOLARISER( *a0 )
              )
    {
        /* -------------------------------------------------------------------
            Case 2: Non-depolarising, rotationally invariant attenuation.
                    In this case we do not have to rotate the light into the
                    reference frame of the attenuation. Since non-depolarising,
                    rotationally invariant attenuations always describe an
                    optical element which does not change the direction of
                    light, we must use the original refframe of the incident
                    light for the result.
        ----------------------------------------------------------------ip- */

        ArStokesVectorSample   * s0 = arstokesvectorsample_alloc( art_gv );
        ArMuellerMatrixSample  * m0 = armuellermatrixsample_alloc( art_gv );

        armmdirectattenuationsample_a_to_mm(
              art_gv,
              a0,
              m0
            );

        arsvlightsample_l_to_sv(
               art_gv,
               lr,
               s0
             );

        // Since it is assumed that the only rotationally invariant MM which
        // is also non-depolarising is the non-polarising MM (non-diagonal
        // components are zero), we can reduce the process to multiplication
        // of the diagonal and the SV
        for (int element = 0; element < 4; element++)
        {
            sps_s_mul_s(
                  art_gv,
                  ARMMDIRECTATTENUATIONSAMPLE_MM_II( *a0, element, element ),
                & ARSVLIGHTSAMPLE_SV_I( *lr, element )
                );
        }

        arstokesvectorsample_free( art_gv, s0 );
        armuellermatrixsample_free( art_gv, m0 );
    }

    else if (   ARMMDIRECTATTENUATIONSAMPLE_IS_DEPOLARISER( *a0 )
             || (      ARMMDIRECTATTENUATIONSAMPLE_IS_ROTATIONALLY_INVARIANT( *a0 )
                  && ! ARSVLIGHTSAMPLE_POLARISED( *lr ) )
            )

    {
        /* -------------------------------------------------------------------
            Case 3: Depolarising attenuation.
                    We can strip any polarisation information off the light
                    (if there was any), and a plain multiplication of
                    non-polarising components suffices.

                    OR

                    Rotationally invariant attenuation with depolarized light.
                    Since it is assumed that the only rotationally invariant MM
                    which is also non-depolarising is the non-polarising MM,
                    whose first column is zero after the first element,
                    multiplication of non-polarising components is,
                    again, sufficient. The resulting light is unpolarised
                    because non-polarising attenuation cannot switch the
                    unpolarised incident light into polarised.
        -------------------------------------------------------------ip-aw- */

        sps_s_mul_s(
              art_gv,
              ARMMDIRECTATTENUATIONSAMPLE( *a0 ),
            & ARSVLIGHTSAMPLE_INTENSITY( *lr )
            );

        ARSVLIGHTSAMPLE_POLARISED( *lr ) = NO;
    }
    else
    {
        ART_ERRORHANDLING_FATAL_ERROR(
            "unexpected branch in polarising attenuation-light multiplication function"
            );
    }

    ASSERT_VALID_SV_LIGHT_SAMPLE( lr )
}

void armmdirectattenuationsample_al_mul_l(
        const ART_GV                     * art_gv,
        const ArMMDirectAttenuationSample  * a0,
        const ArSVLightSample              * l0,
              ArSVLightSample              * lr
        )
{
    ASSERT_VALID_MM_DIRECT_ATTENUATION_SAMPLE( a0 )
    ASSERT_VALID_SV_LIGHT_SAMPLE( l0 )
    ASSERT_ALLOCATED_SV_LIGHT_SAMPLE( lr )

    if ( ARMMDIRECTATTENUATIONSAMPLE_IS_GENERAL_MATRIX( *a0 ) )
    {
        /* -------------------------------------------------------------------
            Case 1: Polarising attenuation - we have to do full or almost
                    full attenuation-light multiplication.
        -------------------------------------------------------------ip-aw- */

        ArStokesVectorSample   * s0 = arstokesvectorsample_alloc( art_gv );
        ArMuellerMatrixSample  * m0 = armuellermatrixsample_alloc( art_gv );

        armmdirectattenuationsample_a_to_mm(
              art_gv,
              a0,
              m0
            );

        if ( ARSVLIGHTSAMPLE_POLARISED( *l0 ) )
        {
            /* ---------------------------------------------------------------
                Case 1.1: Polarised light - we have to rotate the light to
                          match the attenuation entry refframe before
                          multiplication.
            ------------------------------------------------------------ip- */

            arsvlightsample_ld_realign_to_coaxial_refframe_sv(
                  art_gv,
                  l0,
                  3.0 DEGREES,
                & ARMMDIRECTATTENUATIONSAMPLE_ENTRY_REFFRAME( *a0 ),
                  s0
                );
        }
        else
        {
            /* ---------------------------------------------------------------
                Case 1.2: Depolarised light - we can skip the rotation of
                          the light to the attenuation entry frame
                          and use it directly as it is.
            ------------------------------------------------------------ip- */

            arsvlightsample_l_to_sv(
                art_gv,
                l0,
                s0
                );
        }

        arstokesvectorsample_sv_mm_mul_sv(
              art_gv,
              s0,
              m0,
            & ARSVLIGHTSAMPLE_SV( *lr )
            );

        ARSVLIGHTSAMPLE_REFFRAME( *lr ) = ARMMDA_S_REFFRAME_EXIT( *a0 );

        ARSVLIGHTSAMPLE_POLARISED( *lr ) = YES;

        arstokesvectorsample_free( art_gv, s0 );
        armuellermatrixsample_free( art_gv, m0 );
    }

    else if (      ARMMDIRECTATTENUATIONSAMPLE_IS_ROTATIONALLY_INVARIANT( *a0 )
              &&   ARSVLIGHTSAMPLE_POLARISED( *l0 )
              && ! ARMMDIRECTATTENUATIONSAMPLE_IS_DEPOLARISER( *a0 )
              )
    {
        /* -------------------------------------------------------------------
            Case 2: Non-depolarising, rotationally invariant attenuation.
                    In this case we do not have to rotate the light into the
                    reference frame of the attenuation. Since non-depolarising,
                    rotationally invariant attenuations always describe an
                    optical element which does not change the direction of
                    light, we must use the original refframe of the incident
                    light for the result.
        ----------------------------------------------------------------ip- */

        ArStokesVectorSample   * s0 = arstokesvectorsample_alloc( art_gv );
        ArMuellerMatrixSample  * m0 = armuellermatrixsample_alloc( art_gv );

        armmdirectattenuationsample_a_to_mm(
              art_gv,
              a0,
              m0
            );

        arsvlightsample_l_to_sv(
               art_gv,
               l0,
               s0
             );

        // Since it is assumed that the only rotationally invariant MM which
        // is also non-depolarising is the non-polarising MM (non-diagonal
        // components are zero), we can reduce the process to multiplication
        // of the diagonal and the SV
        for (int element = 0; element < 4; element++)
        {
            sps_s_mul_s(
                  art_gv,
                  ARMMDIRECTATTENUATIONSAMPLE_MM_II( *a0, element, element ),
                & ARSVLIGHTSAMPLE_SV_I( *lr, element )
                );
        }

        ARSVLIGHTSAMPLE_REFFRAME( *lr ) = ARSVLIGHTSAMPLE_REFFRAME( *l0 );

        arstokesvectorsample_free( art_gv, s0 );
        armuellermatrixsample_free( art_gv, m0 );
    }

    else if (   ARMMDIRECTATTENUATIONSAMPLE_IS_DEPOLARISER( *a0 )
             || (      ARMMDIRECTATTENUATIONSAMPLE_IS_ROTATIONALLY_INVARIANT( *a0 )
                  && ! ARSVLIGHTSAMPLE_POLARISED( *l0 ) )
            )

    {
        /* -------------------------------------------------------------------
            Case 3: Depolarising attenuation.
                    We can strip any polarisation information off the light
                    (if there was any), and a plain multiplication of
                    non-polarising components suffices.

                    OR

                    Rotationally invariant attenuation with depolarized light.
                    Since it is assumed that the only rotationally invariant MM
                    which is also non-depolarising is the non-polarising MM,
                    whose first column is zero after the first element,
                    multiplication of non-polarising components is,
                    again, sufficient. Producing unpolarised light is
                    consistent with the fact that non-polarising attenuation
                    cannot switch the unpolarised incident light into polarised
        -------------------------------------------------------------ip-aw- */

        sps_ss_mul_s(
              art_gv,
            & ARSVLIGHTSAMPLE_INTENSITY( *l0 ),
              ARMMDIRECTATTENUATIONSAMPLE( *a0 ),
            & ARSVLIGHTSAMPLE_INTENSITY( *lr )
            );

        ARSVLIGHTSAMPLE_POLARISED( *lr ) = NO;
    }
    else
    {
        ART_ERRORHANDLING_FATAL_ERROR(
            "unexpected branch in polarising attenuation-light multiplication function"
            );
    }

    ASSERT_VALID_SV_LIGHT_SAMPLE( lr )
}

void armmdirectattenuationsample_ax_mul_l(
        const ART_GV                     * art_gv,
        const ArMMDirectAttenuationSample  * a0,
        const ArCrosstalkSample            * x0,
              ArSVLightSample              * lr
        )
{
    ASSERT_VALID_MM_DIRECT_ATTENUATION_SAMPLE( a0 )
    ASSERT_VALID_CROSSTALK_SAMPLE( x0 )
    ASSERT_VALID_SV_LIGHT_SAMPLE( lr )

    //  Step 1 - compute the influence of the crosstalk on the
    //           final result. This is similar to the code found in
    //           arplaindirectattenuationsample_ax_mul_l(), except that the
    //           influence of the main diagonal is not computed.

    int  i, j, s = 0;

    ArSVLightSample  * temp_light = arsvlightsample_d_alloc_init_unpolarised( art_gv, 0.0 );

ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
    int  cc = 1;

    for ( i = 0; i < cc; i++ )
    {
        double  temp_i = 0;

        for ( j = 0; j < i; j++ )
            temp_i +=
                  arlightintensitysample_li( art_gv, &ARSVLIGHTSAMPLE_INTENSITY(*lr), j )
                * ARCROSSTALKSAMPLE_I( *x0, s + j );

        arlightintensitysample_set_lid(
            art_gv,
            &ARSVLIGHTSAMPLE_INTENSITY(*temp_light),
            i,
            temp_i
            );

        s += i;
    }

    //  Step 2 - compute the main diagonal. The crosstalk influence from
    //           step 1 gets added to this right before the function returns.

    if ( ! ARMMDIRECTATTENUATIONSAMPLE_IS_DEPOLARISER( *a0 ) )
    {
        /* -------------------------------------------------------------------
            Case 1: polarising attenuation.
        ----------------------------------------------------------------aw- */

        if ( ARSVLIGHTSAMPLE_POLARISED( *lr ) )
        {
            /* ---------------------------------------------------------------
                Case 1.1: polarised light.
            ------------------------------------------------------------aw- */

            ArStokesVectorSample   * s0 = arstokesvectorsample_alloc( art_gv );
            ArMuellerMatrixSample  * m0 = armuellermatrixsample_alloc( art_gv );

            armmdirectattenuationsample_a_to_mm(
                  art_gv,
                  a0,
                  m0
                );

            //   Note that we rotate to match the attenuation entry refframe,
            //   but later use the exit refframe for the result.

            arsvlightsample_ld_realign_to_coaxial_refframe_sv(
                  art_gv,
                  lr,
                  3.0 DEGREES,
                & ARMMDIRECTATTENUATIONSAMPLE_ENTRY_REFFRAME( *a0 ),
                  s0
                );

            arstokesvectorsample_sv_mm_mul_sv(
                  art_gv,
                  s0,
                  m0,
                & ARSVLIGHTSAMPLE_SV( *lr )
                );

            ARSVLIGHTSAMPLE_REFFRAME( *lr ) = ARMMDA_S_REFFRAME_EXIT( *a0 );

            arstokesvectorsample_free( art_gv, s0 );
            armuellermatrixsample_free( art_gv, m0 );

            //   Adding the crosstalk influence.

            arsvlightsample_l_add_l(
                  art_gv,
                  temp_light,
                  lr
                );

            arsvlightsample_free( art_gv, temp_light );

            ASSERT_VALID_SV_LIGHT_SAMPLE( lr )

            return;
        }

        /* ---------------------------------------------------------------
            Case 1.2: unpolarised light.
        ------------------------------------------------------------aw- */

        ArStokesVectorSample   * s0 = arstokesvectorsample_alloc( art_gv );
        ArMuellerMatrixSample  * m0 = armuellermatrixsample_alloc( art_gv );

        armmdirectattenuationsample_a_to_mm(
              art_gv,
              a0,
              m0
            );

        arsvlightsample_l_to_sv(
               art_gv,
               lr,
               s0
             );

        ARSVLIGHTSAMPLE_REFFRAME( *lr ) = ARMMDA_S_REFFRAME_EXIT( *a0 );
        ARSVLIGHTSAMPLE_POLARISED( *lr ) = YES;

        arstokesvectorsample_sv_mm_mul_sv(
              art_gv,
              s0,
              m0,
            & ARSVLIGHTSAMPLE_SV( *lr )
            );

        arstokesvectorsample_free( art_gv, s0 );
        armuellermatrixsample_free( art_gv, m0 );

        //   Adding the crosstalk influence.

        arsvlightsample_l_add_l(
              art_gv,
              temp_light,
              lr
            );

        arsvlightsample_free( art_gv, temp_light );

        ASSERT_VALID_SV_LIGHT_SAMPLE( lr )

        return;
    }

    /* -----------------------------------------------------------------------
        Case 2: depolarising attenuation. We can strip any polarisation
                information off the light (if there was any), and a plain
                multiplication sufffices.
    --------------------------------------------------------------------aw- */

    sps_s_mul_s(
          art_gv,
          ARMMDIRECTATTENUATIONSAMPLE( *a0 ),
        & ARSVLIGHTSAMPLE_INTENSITY( *lr )
        );

    ARSVLIGHTSAMPLE_POLARISED( *lr ) = NO;

    //   Adding the crosstalk influence.

    arsvlightsample_l_add_l(
          art_gv,
          temp_light,
          lr
        );

    arsvlightsample_free( art_gv, temp_light );

    ASSERT_VALID_SV_LIGHT_SAMPLE( lr )
}

void armmdirectattenuationsample_axl_mul_l(
        const ART_GV                     * art_gv,
        const ArMMDirectAttenuationSample  * a0,
        const ArCrosstalkSample            * x0,
        const ArSVLightSample              * l0,
              ArSVLightSample              * lr
        )
{
    (void) art_gv;
    (void) a0;
    (void) x0;
    (void) l0;
    (void) lr;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR;
}

void armmdirectattenuationsample_a_mul_i(
        const ART_GV                     * art_gv,
        const ArMMDirectAttenuationSample  * a0,
              ArLightIntensitySample       * lr
        )
{
    (void) art_gv;
    (void) a0;
    (void) lr;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR;
}

void armmdirectattenuationsample_ai_mul_i(
        const ART_GV                     * art_gv,
        const ArMMDirectAttenuationSample  * a0,
        const ArLightIntensitySample       * l0,
              ArLightIntensitySample       * lr
        )
{
    (void) art_gv;
    (void) a0;
    (void) l0;
    (void) lr;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR;
}

void armmdirectattenuationsample_ax_mul_i(
        const ART_GV                     * art_gv,
        const ArMMDirectAttenuationSample  * a0,
        const ArCrosstalkSample            * x0,
              ArLightIntensitySample       * lr
        )
{
    (void) art_gv;
    (void) a0;
    (void) x0;
    (void) lr;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR;
}

void armmdirectattenuationsample_axi_mul_i(
        const ART_GV                     * art_gv,
        const ArMMDirectAttenuationSample  * a0,
        const ArCrosstalkSample            * x0,
        const ArLightIntensitySample       * l0,
              ArLightIntensitySample       * lr
        )
{
    (void) art_gv;
    (void) a0;
    (void) x0;
    (void) l0;
    (void) lr;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR;
}

unsigned int armmdirectattenuationsample_a_valid(
        const ART_GV                     * art_gv,
        const ArMMDirectAttenuationSample  * a0
        )
{
    if ( a0 )
    {
        int properties_consistency = 1;

        if (      ARMMDIRECTATTENUATIONSAMPLE_IS_DEPOLARISER(*a0)
             && ! ARMMDIRECTATTENUATIONSAMPLE_IS_ROTATIONALLY_INVARIANT(*a0) )
        {
            ART_ERRORHANDLING_WARNING(
                "Mueller matrix is depolarising, but not rotationaly invariant"
                );
            properties_consistency = 0;
        }

        if ( ! properties_consistency )
            return 0;

        //  Warnings are eventually thrown directly in the following
        //  functions themselves

        if ( ! armuellermatrixsample_m_valid( art_gv, ARMMDA_S_MATRIX( *a0 ), 1, 0 ) )
            return 0;

        if ( ! arrefframe_rf_valid( art_gv, & ARMMDA_S_REFFRAME_ENTRY(*a0) ) )
            return 0;

        if ( ! arrefframe_rf_valid( art_gv, & ARMMDA_S_REFFRAME_EXIT(*a0) ) )
            return 0;

        return 1;
    }
    else
        return 0;
}

/* ---------------------------------------------------------------------------

    'ardirectattenuationsample_a_debugprintf'

        Prints a properly formatted and comprehensible version of an
        ArDirectAttenuationSample struct contents to stdout. Only used for debugging
        purposes.

------------------------------------------------------------------------aw- */


void armmdirectattenuationsample_a_debugprintf(
        const ART_GV                     * art_gv,
        const ArMMDirectAttenuationSample  * a0
        )
{
    if ( ARMMDIRECTATTENUATIONSAMPLE_HAS_FULL_MATRIX( *a0 ) )
    {
        if ( ARMMDIRECTATTENUATIONSAMPLE_IS_ROTATIONALLY_INVARIANT ( *a0 ) )
            printf("rotationally invariant Mueller matrix\n");
        else
            printf("full Mueller matrix\n");
    }
    else
    {
        printf("depolarising Mueller matrix\n");
    }

    printf("\n");

    armuellermatrixsample_mi_debugprintf( art_gv, ARMMDA_S_MATRIX( *a0 ), 2);

    printf("\nIncoming ");
    arrefframe_rf_debugprintf( art_gv, & ARMMDA_S_REFFRAME_ENTRY( *a0 ));
    printf("\nOutgoing ");
    arrefframe_rf_debugprintf( art_gv, & ARMMDA_S_REFFRAME_EXIT( *a0 ));
}

// ===========================================================================
