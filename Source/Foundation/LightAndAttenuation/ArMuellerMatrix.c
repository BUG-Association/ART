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

#define ART_MODULE_NAME     ArMuellerMatrix

#include "ArMuellerMatrix.h"
#include <ArMuellerMatrix_GSLSettings.h>
#include "ArStokesVector.h"
#include "ArString.h"

#if (GSL_FOUND == 1)

#include <gsl/gsl_math.h>
#include <gsl/gsl_eigen.h>

#else // (GSL_FOUND == 1)

#warning \
GNU Scientific Library (GSL) not found, Mueller matrix eigenvalue \
realisability test will not be available.

#endif // (GSL_FOUND == 1)

#define  YES  1
#define  NO   0

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

//   Two macros to make MM constructors more legible.

#define INIT_MM_ID(__mm,__i,__d) \
    spc_d_init_s( art_gv, (__d), ARMUELLER_M_I(__mm,__i) );

#define INIT_MM_IC(__mm,__i,__c) \
    spc_s_init_s( art_gv, (__c), ARMUELLER_M_I(__mm,__i) );


ArMuellerMatrix * armuellermatrix_alloc(
        const ART_GV  * art_gv
        )
{
    ArMuellerMatrix  * newMM = ALLOC( ArMuellerMatrix );

    for ( unsigned int i = 0; i < 16; i++ )
        ARMUELLER_M_I( *newMM, i )=
            spc_alloc(
                art_gv
                );

    return newMM;
}

void armuellermatrix_free(
        const ART_GV           * art_gv,
              ArMuellerMatrix  * mmr
        )
{
    for ( unsigned int i = 0; i < 16; i++ )
        spc_free(
            art_gv,
            ARMUELLER_M_I( *mmr, i )
            );

    FREE(mmr);
}

void armuellermatrix_m_init_m(
        const ART_GV           * art_gv,
        const ArMuellerMatrix  * mm0,
              ArMuellerMatrix  * mmr
        )
{
    ASSERT_ALLOCATED_MUELLER_MATRIX( mm0 );
    ASSERT_ALLOCATED_MUELLER_MATRIX( mmr );

    for ( unsigned int i = 0; i < 16; i++ )
        spc_s_init_s(
            art_gv,
            ARMUELLER_M_I( *mm0, i ),
            ARMUELLER_M_I( *mmr, i )
            );
}

void armuellermatrix_d_init_depolarising_m(
        const ART_GV           * art_gv,
        const double             d0,
              ArMuellerMatrix  * mm
        )
{
    ASSERT_UNIT_RANGE_DOUBLE( d0 );
    ASSERT_ALLOCATED_MUELLER_MATRIX( mm );

    /* -----------------------------------------------------------------------
        Element (0,0) holds the de-polarising attenuation.
    --------------------------------------------------------------------aw- */

    INIT_MM_ID( *mm, 0, d0 );

    /* -----------------------------------------------------------------------
        The rest of the matrix has to be filled with zeroes.
    --------------------------------------------------------------------aw- */

    for ( unsigned int i = 1; i < 16; i++ )
        INIT_MM_ID( *mm, i, 0.0 );

    ASSERT_VALID_MUELLER_MATRIX(mm);
}

void armuellermatrix_dd_init_depolarising_m(
        const ART_GV           * art_gv,
        const double             d0,
        const double             d1,
              ArMuellerMatrix  * mm
        )
{
    ASSERT_UNIT_RANGE_DOUBLE( d0 );
    ASSERT_UNIT_RANGE_DOUBLE( d1 );

    // The polarizing attenuation must be less than the total attenuation
    #ifdef FOUNDATION_ASSERTIONS
        if ( fabs( d1 ) > fabs( d0 ) )
            ART_ERRORHANDLING_FATAL_ERROR(
                "The non-polarizing attenuation %.8f "
                "is larger than total attenuation %.8f in %s",
                d1, d0, __func__
                );
    #endif

    ASSERT_ALLOCATED_MUELLER_MATRIX( mm );

    /* -----------------------------------------------------------------------
        Element (0,0) holds the absolute attenuation.
    --------------------------------------------------------------------ip- */

    INIT_MM_ID( *mm,  0, d0  );
    INIT_MM_ID( *mm,  1, 0.0 );
    INIT_MM_ID( *mm,  2, 0.0 );
    INIT_MM_ID( *mm,  3, 0.0 );

    /* -----------------------------------------------------------------------
        The diagonal defines a uniform depolarization of all Stokes components.
    --------------------------------------------------------------------ip- */

    INIT_MM_ID( *mm,  4, 0.0 );
    INIT_MM_ID( *mm,  5, d1  );
    INIT_MM_ID( *mm,  6, 0.0 );
    INIT_MM_ID( *mm,  7, 0.0 );

    INIT_MM_ID( *mm,  8, 0.0 );
    INIT_MM_ID( *mm,  9, 0.0 );
    INIT_MM_ID( *mm, 10, d1  );
    INIT_MM_ID( *mm, 11, 0.0 );

    INIT_MM_ID( *mm, 12, 0.0 );
    INIT_MM_ID( *mm, 13, 0.0 );
    INIT_MM_ID( *mm, 14, 0.0 );
    INIT_MM_ID( *mm, 15, d1  );

    ASSERT_VALID_MUELLER_MATRIX(mm);
}

void armuellermatrix_d_init_nonpolarising_m(
        const ART_GV           * art_gv,
        const double             d0,
              ArMuellerMatrix  * mm
        )
{
    ASSERT_UNIT_RANGE_DOUBLE( d0 );
    ASSERT_ALLOCATED_MUELLER_MATRIX( mm );

    INIT_MM_ID( *mm,  0, d0  );
    INIT_MM_ID( *mm,  1, 0.0 );
    INIT_MM_ID( *mm,  2, 0.0 );
    INIT_MM_ID( *mm,  3, 0.0 );

    INIT_MM_ID( *mm,  4, 0.0 );
    INIT_MM_ID( *mm,  5, d0  );
    INIT_MM_ID( *mm,  6, 0.0 );
    INIT_MM_ID( *mm,  7, 0.0 );

    INIT_MM_ID( *mm,  8, 0.0 );
    INIT_MM_ID( *mm,  9, 0.0 );
    INIT_MM_ID( *mm, 10, d0  );
    INIT_MM_ID( *mm, 11, 0.0 );

    INIT_MM_ID( *mm, 12, 0.0 );
    INIT_MM_ID( *mm, 13, 0.0 );
    INIT_MM_ID( *mm, 14, 0.0 );
    INIT_MM_ID( *mm, 15, d0  );

    ASSERT_VALID_MUELLER_MATRIX(mm);
}

void armuellermatrix_c_init_depolarising_m(
        const ART_GV           * art_gv,
        const ArSpectrum         * c0,
              ArMuellerMatrix  * mm
        )
{
    ASSERT_ALLOCATED_SPECTRUM(c0);
    ASSERT_VALID_SPECTRUM(c0);
    ASSERT_ALLOCATED_MUELLER_MATRIX( mm );

    /* -----------------------------------------------------------------------
        Element (0,0) holds the de-polarising attenuation.
    --------------------------------------------------------------------aw- */

    INIT_MM_IC( *mm, 0, c0 );

    /* -----------------------------------------------------------------------
        The rest of the matrix has to be filled with zeroes.
    --------------------------------------------------------------------aw- */

    for ( unsigned int i = 1; i < 16; i++ )
        INIT_MM_ID( *mm, i, 0.0 );

    ASSERT_VALID_MUELLER_MATRIX(mm);
}

void armuellermatrix_c_init_nonpolarising_m(
        const ART_GV           * art_gv,
        const ArSpectrum         * c0,
              ArMuellerMatrix  * mm
        )
{
    ASSERT_ALLOCATED_SPECTRUM(c0);
    ASSERT_VALID_SPECTRUM(c0);
    ASSERT_ALLOCATED_MUELLER_MATRIX( mm );

    INIT_MM_IC( *mm,  0, c0  );
    INIT_MM_ID( *mm,  1, 0.0 );
    INIT_MM_ID( *mm,  2, 0.0 );
    INIT_MM_ID( *mm,  3, 0.0 );

    INIT_MM_ID( *mm,  4, 0.0 );
    INIT_MM_IC( *mm,  5, c0  );
    INIT_MM_ID( *mm,  6, 0.0 );
    INIT_MM_ID( *mm,  7, 0.0 );

    INIT_MM_ID( *mm,  8, 0.0 );
    INIT_MM_ID( *mm,  9, 0.0 );
    INIT_MM_IC( *mm, 10, c0  );
    INIT_MM_ID( *mm, 11, 0.0 );

    INIT_MM_ID( *mm, 12, 0.0 );
    INIT_MM_ID( *mm, 13, 0.0 );
    INIT_MM_ID( *mm, 14, 0.0 );
    INIT_MM_IC( *mm, 15, c0  );

    ASSERT_VALID_MUELLER_MATRIX(mm);
}

void armuellermatrix_dd_init_linear_polariser_m(
        const ART_GV           * art_gv,
        const double             angle,
        const double             strength,
              ArMuellerMatrix  * mm
        )
{
    #ifdef FOUNDATION_ASSERTIONS
        if ( strength < 0.0001 || strength > 1.0001 )
            ART_ERRORHANDLING_FATAL_ERROR(
                "meaningless percentage %f for linear polariser strength"
                ,   strength * 100.0
                );
    #endif
    ASSERT_ALLOCATED_MUELLER_MATRIX( mm );

    //   This rather inefficient way of coding the formula is so
    //   that consistency with the notation on p. 53, "Introduction to
    //   Spectropolarimetry" by J. del Toro Iniesta, is maintained.       (aw)

    double  k1 = strength / 2.0 + 0.5;
    double  k2 = 1.0 - k1;

    double  alpha = M_SQR(k1) + M_SQR(k2);
    double  beta = ( M_SQR(k1) - M_SQR(k2) ) / alpha;
    double  gamma = ( 2 * k1 * k2 ) / alpha;

    double ca = cos( 2.0 * angle);
    double sa = sin( 2.0 * angle);

    double f = alpha / 2.0;

    INIT_MM_ID( *mm,  0, f );
    INIT_MM_ID( *mm,  1, f * ca * beta );
    INIT_MM_ID( *mm,  2, f * sa * beta );
    INIT_MM_ID( *mm,  3, 0.0 );

    INIT_MM_ID( *mm,  4, f * ca * beta);
    INIT_MM_ID( *mm,  5, f * ( M_SQR(ca) + M_SQR(sa) * gamma ) );
    INIT_MM_ID( *mm,  6, f * sa * ca * ( 1.0 - gamma ) );
    INIT_MM_ID( *mm,  7, 0.0 );

    INIT_MM_ID( *mm,  8, f * sa * beta );
    INIT_MM_ID( *mm,  9, f * ca * sa * ( 1.0 - gamma ) );
    INIT_MM_ID( *mm, 10, f * ( M_SQR(sa) + M_SQR(ca) * gamma ) );
    INIT_MM_ID( *mm, 11, 0.0 );

    INIT_MM_ID( *mm, 12, 0.0 );
    INIT_MM_ID( *mm, 13, 0.0 );
    INIT_MM_ID( *mm, 14, 0.0 );
    INIT_MM_ID( *mm, 15, f * gamma );

    ASSERT_VALID_MUELLER_MATRIX(mm);
}

void armuellermatrix_d_init_phase_shifter_m(
        const ART_GV           * art_gv,
        const double             angle,
              ArMuellerMatrix  * mm
        )
{
    ASSERT_ALLOCATED_MUELLER_MATRIX( mm );

    const double cosine = cos(angle);
    const double sine   = sin(angle);

    INIT_MM_ID( *mm,  0, 1.0 );
    INIT_MM_ID( *mm,  1, 0.0 );
    INIT_MM_ID( *mm,  2, 0.0 );
    INIT_MM_ID( *mm,  3, 0.0 );

    INIT_MM_ID( *mm,  4, 0.0 );
    INIT_MM_ID( *mm,  5, 1.0 );
    INIT_MM_ID( *mm,  6, 0.0 );
    INIT_MM_ID( *mm,  7, 0.0 );

    INIT_MM_ID( *mm,  8, 0.0 );
    INIT_MM_ID( *mm,  9, 0.0 );
    INIT_MM_ID( *mm, 10, cosine );
    INIT_MM_ID( *mm, 11, sine );

    INIT_MM_ID( *mm, 12, 0.0 );
    INIT_MM_ID( *mm, 13, 0.0 );
    INIT_MM_ID( *mm, 14, -sine );
    INIT_MM_ID( *mm, 15, cosine );

    ASSERT_VALID_MUELLER_MATRIX(mm);
}

void armuellermatrix_d_init_rotator_m(
        const ART_GV           * art_gv,
        const double             angle,
              ArMuellerMatrix  * mm
        )
{
    ASSERT_ALLOCATED_MUELLER_MATRIX( mm );

    const double cosine = cos(angle);
    const double sine   = sin(angle);

    INIT_MM_ID( *mm,  0, 1.0 );
    INIT_MM_ID( *mm,  1, 0.0 );
    INIT_MM_ID( *mm,  2, 0.0 );
    INIT_MM_ID( *mm,  3, 0.0 );

    INIT_MM_ID( *mm,  4, 0.0 );
    INIT_MM_ID( *mm,  5, cosine );
    INIT_MM_ID( *mm,  6, sine );
    INIT_MM_ID( *mm,  7, 0.0 );

    INIT_MM_ID( *mm,  8, 0.0 );
    INIT_MM_ID( *mm,  9, -sine );
    INIT_MM_ID( *mm, 10, cosine );
    INIT_MM_ID( *mm, 11, 0.0 );

    INIT_MM_ID( *mm, 12, 0.0 );
    INIT_MM_ID( *mm, 13, 0.0 );
    INIT_MM_ID( *mm, 14, 0.0 );
    INIT_MM_ID( *mm, 15, 1.0 );

    ASSERT_VALID_MUELLER_MATRIX(mm);
}

void armuellermatrix_mm_add_m(
        const ART_GV           * art_gv,
        const ArMuellerMatrix  * mm0,
        const ArMuellerMatrix  * mm1,
        ArMuellerMatrix        * mmr
        )
{
    ASSERT_ALLOCATED_MUELLER_MATRIX(mm0);
    ASSERT_ALLOCATED_MUELLER_MATRIX(mm1);
    ASSERT_ALLOCATED_MUELLER_MATRIX(mmr);
    ASSERT_VALID_MUELLER_MATRIX(mm0);
    ASSERT_VALID_MUELLER_MATRIX(mm1);

    for ( unsigned int i = 0; i < 16; i++ )
    {
        spc_ss_add_s(
            art_gv,
            ARMUELLER_M_I( *mm0, i ),
            ARMUELLER_M_I( *mm1, i ),
            ARMUELLER_M_I( *mmr, i )
            );
    }

    ASSERT_VALID_MUELLER_MATRIX(mmr);
}

void armuellermatrix_m_add_m(
        const ART_GV           * art_gv,
        const ArMuellerMatrix  * mm0,
        ArMuellerMatrix        * mmr
        )
{
    ASSERT_ALLOCATED_MUELLER_MATRIX(mm0);
    ASSERT_ALLOCATED_MUELLER_MATRIX(mmr);
    ASSERT_VALID_MUELLER_MATRIX(mm0);
    ASSERT_VALID_MUELLER_MATRIX(mmr);

    for ( unsigned int i = 0; i < 16; i++ )
    {
        spc_s_add_s(
            art_gv,
            ARMUELLER_M_I( *mm0, i ),
            ARMUELLER_M_I( *mmr, i )
            );
    }

    ASSERT_VALID_MUELLER_MATRIX(mmr);
}

/* ---------------------------------------------------------------------------

    'armuellermatrix_mm_mul_m'

        Multiplies two Mueller matrices, which is the basic operation for
        the concatenation of attenuation elements.

------------------------------------------------------------------------aw- */

void armuellermatrix_mm_mul_m(
        const ART_GV           * art_gv,
        const ArMuellerMatrix  * m0,
        const ArMuellerMatrix  * m1,
              ArMuellerMatrix  * m_r
        )
{
    ASSERT_ALLOCATED_MUELLER_MATRIX(m0);
    ASSERT_ALLOCATED_MUELLER_MATRIX(m1);
    ASSERT_ALLOCATED_MUELLER_MATRIX(m_r);
    ASSERT_VALID_MUELLER_MATRIX(m0);
    ASSERT_VALID_MUELLER_MATRIX(m1);

    ArSpectrum  * lc0 = spc_alloc( art_gv );
    ArSpectrum  * lc1 = spc_alloc( art_gv );
    ArSpectrum  * lc2 = spc_alloc( art_gv );

    for ( unsigned int i = 0; i < 16; i++ )
    {
        int  rowbase = 4 * ((int) i / 4);
        int  colbase = (i % 4);

        spc_ss_mul_s(
            art_gv,
            ARMUELLER_M_I( *m0, rowbase      ),
            ARMUELLER_M_I( *m1, colbase      ),
            lc0
            );

        spc_ss_mul_s(
            art_gv,
            ARMUELLER_M_I( *m0, rowbase +  1 ),
            ARMUELLER_M_I( *m1, colbase +  4 ),
            lc1
            );

        spc_ss_mul_s(
            art_gv,
            ARMUELLER_M_I( *m0, rowbase +  2 ),
            ARMUELLER_M_I( *m1, colbase +  8 ),
            lc2
            );

        spc_ss_mul_s(
            art_gv,
            ARMUELLER_M_I( *m0, rowbase +  3 ),
            ARMUELLER_M_I( *m1, colbase + 12 ),
            ARMUELLER_M_I( *m_r, i )
            );

        spc_s_add_s(
            art_gv,
            lc0,
            ARMUELLER_M_I( *m_r, i )
            );

        spc_s_add_s(
            art_gv,
            lc1,
            ARMUELLER_M_I( *m_r, i )
            );

        spc_s_add_s(
            art_gv,
            lc2,
            ARMUELLER_M_I( *m_r, i )
            );
    }

    spc_free( art_gv, lc0 );
    spc_free( art_gv, lc1 );
    spc_free( art_gv, lc2 );

    ASSERT_VALID_MUELLER_MATRIX(m_r);
}

void armuellermatrix_md_mul_m(
        const ART_GV           * art_gv,
        const ArMuellerMatrix  * mm0,
        const double             d0,
        ArMuellerMatrix        * mmr
        )
{
    ASSERT_ALLOCATED_MUELLER_MATRIX(mm0);
    ASSERT_ALLOCATED_MUELLER_MATRIX(mmr);
    ASSERT_VALID_MUELLER_MATRIX(mm0);

    for ( unsigned int i = 0; i < 16; i++ )
    {
        spc_ds_mul_s(
            art_gv,
            d0,
            ARMUELLER_M_I( *mm0, i ),
            ARMUELLER_M_I( *mmr, i )
            );
    }

    ASSERT_VALID_MUELLER_MATRIX(mmr);
}

void armuellermatrix_mdd_rotate_input_m(
        const ART_GV                 * art_gv,
        const ArMuellerMatrix        * m0,
        const double                   rot_cos,
        const double                   rot_sin,
              ArMuellerMatrix        * mr
        )
{
    /* -----------------------------------------------------------------------
        We first rotate the incident Stokes vector by requested angle and then
        apply the original Mueller matrix:

            ResultingSV = (MM * InputRotation) * IncidentSV

        The final concatenated Mueller matrix is as follows:

        m00 | m01 C + m02 S | m02 C - m01 S | m03
        m10 | m11 C + m12 S | m12 C - m11 S | m13
        m20 | m21 C + m22 S | m22 C - m21 S | m23
        m30 | m31 C + m32 S | m32 C - m31 S | m33

        Where:
            mij are original Mueller matrix components
            C = Cos[2 phiI]
            S = Sin[2 phiI]
    --------------------------------------------------------------------ip- */

    //  Shorthands to make the formulae consistent

    const double  cos_phi = rot_cos;
    const double  sin_phi = rot_sin;

    //  Switch to double-angle quantities using standard goniometric
    //  double-angle conversions

    double  cos_2phi = M_SQR( cos_phi ) - M_SQR( sin_phi );
    double  sin_2phi = 2 * cos_phi * sin_phi;

    // Buffers for intermediate expression

    ArSpectrum  * temp_0 = spc_alloc( art_gv );
    ArSpectrum  * temp_1 = spc_alloc( art_gv );

    //  Copied elements (0,0), (1,0), (2,0), (3,0)  ---------------------------

    spc_s_init_s(
        art_gv,
        ARMUELLER_M_II( *m0,  0,0 ),
        ARMUELLER_M_II( *mr,  0,0 )
        );

    spc_s_init_s(
        art_gv,
        ARMUELLER_M_II( *m0,  1,0 ),
        ARMUELLER_M_II( *mr,  1,0 )
        );

    spc_s_init_s(
        art_gv,
        ARMUELLER_M_II( *m0,  2,0 ),
        ARMUELLER_M_II( *mr,  2,0 )
        );

    spc_s_init_s(
        art_gv,
        ARMUELLER_M_II( *m0,  3,0 ),
        ARMUELLER_M_II( *mr,  3,0 )
        );

    //  Copied elements (0,3), (1,3), (2,3), (3,3)  ---------------------------

    spc_s_init_s(
        art_gv,
        ARMUELLER_M_II( *m0,  0,3 ),
        ARMUELLER_M_II( *mr,  0,3 )
        );

    spc_s_init_s(
        art_gv,
        ARMUELLER_M_II( *m0,  1,3 ),
        ARMUELLER_M_II( *mr,  1,3 )
        );

    spc_s_init_s(
        art_gv,
        ARMUELLER_M_II( *m0,  2,3 ),
        ARMUELLER_M_II( *mr,  2,3 )
        );

    spc_s_init_s(
        art_gv,
        ARMUELLER_M_II( *m0,  3,3 ),
        ARMUELLER_M_II( *mr,  3,3 )
        );

    //  Element 0,1:  m01 C + m02 S  ------------------------------------------

    spc_ds_mul_s(
        art_gv,
        cos_2phi,
        ARMUELLER_M_II( *m0, 0,1 ),
        temp_0
        );

    spc_ds_mul_s(
        art_gv,
        sin_2phi,
        ARMUELLER_M_II( *m0, 0,2 ),
        temp_1
        );

    spc_ss_add_s(
        art_gv,
        temp_1,
        temp_0,
        ARMUELLER_M_II( *mr, 0,1 )
        );

    //  Element 1,1:  m11 C + m12 S  ------------------------------------------

    spc_ds_mul_s(
        art_gv,
        cos_2phi,
        ARMUELLER_M_II( *m0, 1,1 ),
        temp_0
        );

    spc_ds_mul_s(
        art_gv,
        sin_2phi,
        ARMUELLER_M_II( *m0, 1,2 ),
        temp_1
        );

    spc_ss_add_s(
        art_gv,
        temp_1,
        temp_0,
        ARMUELLER_M_II( *mr, 1,1 )
        );

    //  Element 2,1:  m21 C + m22 S  ------------------------------------------

    spc_ds_mul_s(
        art_gv,
        cos_2phi,
        ARMUELLER_M_II( *m0, 2,1 ),
        temp_0
        );

    spc_ds_mul_s(
        art_gv,
        sin_2phi,
        ARMUELLER_M_II( *m0, 2,2 ),
        temp_1
        );

    spc_ss_add_s(
        art_gv,
        temp_1,
        temp_0,
        ARMUELLER_M_II( *mr, 2,1 )
        );

    //  Element 3,1:  m31 C + m32 S  ------------------------------------------

    spc_ds_mul_s(
        art_gv,
        cos_2phi,
        ARMUELLER_M_II( *m0, 3,1 ),
        temp_0
        );

    spc_ds_mul_s(
        art_gv,
        sin_2phi,
        ARMUELLER_M_II( *m0, 3,2 ),
        temp_1
        );

    spc_ss_add_s(
        art_gv,
        temp_1,
        temp_0,
        ARMUELLER_M_II( *mr, 3,1 )
        );

    //  Element 0,2:  m02 C - m01 S  ------------------------------------------

    spc_ds_mul_s(
        art_gv,
        cos_2phi,
        ARMUELLER_M_II( *m0, 0,2 ),
        temp_0
        );

    spc_ds_mul_s(
        art_gv,
        sin_2phi,
        ARMUELLER_M_II( *m0, 0,1 ),
        temp_1
        );

    spc_ss_sub_s(
        art_gv,
        temp_1,
        temp_0,
        ARMUELLER_M_II( *mr, 0,2 )
        );

    //  Element 1,2:  m12 C - m11 S  ------------------------------------------

    spc_ds_mul_s(
        art_gv,
        cos_2phi,
        ARMUELLER_M_II( *m0, 1,2 ),
        temp_0
        );

    spc_ds_mul_s(
        art_gv,
        sin_2phi,
        ARMUELLER_M_II( *m0, 1,1 ),
        temp_1
        );

    spc_ss_sub_s(
        art_gv,
        temp_1,
        temp_0,
        ARMUELLER_M_II( *mr, 1,2 )
        );

    //  Element 2,2:  m22 C - m21 S  ------------------------------------------

    spc_ds_mul_s(
        art_gv,
        cos_2phi,
        ARMUELLER_M_II( *m0, 2,2 ),
        temp_0
        );

    spc_ds_mul_s(
        art_gv,
        sin_2phi,
        ARMUELLER_M_II( *m0, 2,1 ),
        temp_1
        );

    spc_ss_sub_s(
        art_gv,
        temp_1,
        temp_0,
        ARMUELLER_M_II( *mr, 2,2 )
        );

    //  Element 3,2:  m32 C - m31 S  ------------------------------------------

    spc_ds_mul_s(
        art_gv,
        cos_2phi,
        ARMUELLER_M_II( *m0, 3,2 ),
        temp_0
        );

    spc_ds_mul_s(
        art_gv,
        sin_2phi,
        ARMUELLER_M_II( *m0, 3,1 ),
        temp_1
        );

    spc_ss_sub_s(
        art_gv,
        temp_1,
        temp_0,
        ARMUELLER_M_II( *mr, 3,2 )
        );

    //  Temps clean-up  -------------------------------------------------------

    spc_free( art_gv, temp_0 );
    spc_free( art_gv, temp_1 );

    ASSERT_VALID_MUELLER_MATRIX(mr);
}

void armuellermatrix_mdd_rotate_output_m(
        const ART_GV                 * art_gv,
        const ArMuellerMatrix        * m0,
        const double                   rot_cos,
        const double                   rot_sin,
              ArMuellerMatrix        * mr
        )
{
    /* -----------------------------------------------------------------------
        We first apply the original Mueller matrix and then rotate the
        resulting Stokes vector by requested angle:

            ResultingSV = (OutputRotation * MM) * IncidentSV

        The final concatenated Mueller matrix is as follows:

        m00           | m01           | m02           | m03
        m10 C - m20 S | m11 C - m21 S | m12 C - m22 S | m13 C - m23 S
        m20 C + m10 S | m21 C + m11 S | m22 C + m12 S | m23 C + m13 S
        m30           | m31           | m32           | m33

        Where:
            mij are original Mueller matrix components
            C = Cos[2 phiO]
            S = Sin[2 phiO]
    --------------------------------------------------------------------ip- */

    //  Shorthands to make the formulae consistent

    const double  cos_phi = rot_cos;
    const double  sin_phi = rot_sin;

    //  Switch to double-angle quantities using standard goniometric
    //  double-angle conversions

    double  cos_2phi = M_SQR( cos_phi ) - M_SQR( sin_phi );
    double  sin_2phi = 2 * cos_phi * sin_phi;

    // Buffers for intermediate expression

    ArSpectrum  * temp_0 = spc_alloc( art_gv );
    ArSpectrum  * temp_1 = spc_alloc( art_gv );

    //  Copied elements (0,0), (0,1), (0,2), (0,3)  ---------------------------

    spc_s_init_s(
        art_gv,
        ARMUELLER_M_II( *m0,  0,0 ),
        ARMUELLER_M_II( *mr,  0,0 )
        );

    spc_s_init_s(
        art_gv,
        ARMUELLER_M_II( *m0,  0,1 ),
        ARMUELLER_M_II( *mr,  0,1 )
        );

    spc_s_init_s(
        art_gv,
        ARMUELLER_M_II( *m0,  0,2 ),
        ARMUELLER_M_II( *mr,  0,2 )
        );

    spc_s_init_s(
        art_gv,
        ARMUELLER_M_II( *m0,  0,3 ),
        ARMUELLER_M_II( *mr,  0,3 )
        );

    //  Copied elements (3,0), (3,1), (3,2), (3,3)  ---------------------------

    spc_s_init_s(
        art_gv,
        ARMUELLER_M_II( *m0,  3,0 ),
        ARMUELLER_M_II( *mr,  3,0 )
        );

    spc_s_init_s(
        art_gv,
        ARMUELLER_M_II( *m0,  3,1 ),
        ARMUELLER_M_II( *mr,  3,1 )
        );

    spc_s_init_s(
        art_gv,
        ARMUELLER_M_II( *m0,  3,2 ),
        ARMUELLER_M_II( *mr,  3,2 )
        );

    spc_s_init_s(
        art_gv,
        ARMUELLER_M_II( *m0,  3,3 ),
        ARMUELLER_M_II( *mr,  3,3 )
        );

    //  Element 1,0:  m10 C - m20 S  ------------------------------------------

    spc_ds_mul_s(
        art_gv,
        cos_2phi,
        ARMUELLER_M_II( *m0, 1,0 ),
        temp_0
        );

    spc_ds_mul_s(
        art_gv,
        sin_2phi,
        ARMUELLER_M_II( *m0, 2,0 ),
        temp_1
        );

    spc_ss_sub_s(
        art_gv,
        temp_1,
        temp_0,
        ARMUELLER_M_II( *mr, 1,0 )
        );

    //  Element 1,1:  m11 C - m21 S  ------------------------------------------

    spc_ds_mul_s(
        art_gv,
        cos_2phi,
        ARMUELLER_M_II( *m0, 1,1 ),
        temp_0
        );

    spc_ds_mul_s(
        art_gv,
        sin_2phi,
        ARMUELLER_M_II( *m0, 2,1 ),
        temp_1
        );

    spc_ss_sub_s(
        art_gv,
        temp_1,
        temp_0,
        ARMUELLER_M_II( *mr, 1,1 )
        );

    //  Element 1,2:  m12 C - m22 S  ------------------------------------------

    spc_ds_mul_s(
        art_gv,
        cos_2phi,
        ARMUELLER_M_II( *m0, 1,2 ),
        temp_0
        );

    spc_ds_mul_s(
        art_gv,
        sin_2phi,
        ARMUELLER_M_II( *m0, 2,2 ),
        temp_1
        );

    spc_ss_sub_s(
        art_gv,
        temp_1,
        temp_0,
        ARMUELLER_M_II( *mr, 1,2 )
        );

    //  Element 1,3:  m13 C - m23 S  ------------------------------------------

    spc_ds_mul_s(
        art_gv,
        cos_2phi,
        ARMUELLER_M_II( *m0, 1,3 ),
        temp_0
        );

    spc_ds_mul_s(
        art_gv,
        sin_2phi,
        ARMUELLER_M_II( *m0, 2,3 ),
        temp_1
        );

    spc_ss_sub_s(
        art_gv,
        temp_1,
        temp_0,
        ARMUELLER_M_II( *mr, 1,3 )
        );

    //  Element 2,0:  m20 C + m10 S  ------------------------------------------

    spc_ds_mul_s(
        art_gv,
        cos_2phi,
        ARMUELLER_M_II( *m0, 2,0 ),
        temp_0
        );

    spc_ds_mul_s(
        art_gv,
        sin_2phi,
        ARMUELLER_M_II( *m0, 1,0 ),
        temp_1
        );

    spc_ss_add_s(
        art_gv,
        temp_1,
        temp_0,
        ARMUELLER_M_II( *mr, 2,0 )
        );

    //  Element 2,1:  m21 C + m11 S  ------------------------------------------

    spc_ds_mul_s(
        art_gv,
        cos_2phi,
        ARMUELLER_M_II( *m0, 2,1 ),
        temp_0
        );

    spc_ds_mul_s(
        art_gv,
        sin_2phi,
        ARMUELLER_M_II( *m0, 1,1 ),
        temp_1
        );

    spc_ss_add_s(
        art_gv,
        temp_1,
        temp_0,
        ARMUELLER_M_II( *mr, 2,1 )
        );

    //  Element 2,2:  m22 C + m12 S  ------------------------------------------

    spc_ds_mul_s(
        art_gv,
        cos_2phi,
        ARMUELLER_M_II( *m0, 2,2 ),
        temp_0
        );

    spc_ds_mul_s(
        art_gv,
        sin_2phi,
        ARMUELLER_M_II( *m0, 1,2 ),
        temp_1
        );

    spc_ss_add_s(
        art_gv,
        temp_1,
        temp_0,
        ARMUELLER_M_II( *mr, 2,2 )
        );

    //  Element 2,3:  m23 C + m13 S  ------------------------------------------

    spc_ds_mul_s(
        art_gv,
        cos_2phi,
        ARMUELLER_M_II( *m0, 2,3 ),
        temp_0
        );

    spc_ds_mul_s(
        art_gv,
        sin_2phi,
        ARMUELLER_M_II( *m0, 1,3 ),
        temp_1
        );

    spc_ss_add_s(
        art_gv,
        temp_1,
        temp_0,
        ARMUELLER_M_II( *mr, 2,3 )
        );

    //  Temps clean-up  -------------------------------------------------------

    spc_free( art_gv, temp_0 );
    spc_free( art_gv, temp_1 );

    ASSERT_VALID_MUELLER_MATRIX(mr);
}

void armuellermatrix_mdddd_rotate_input_and_output_m(
        const ART_GV                 * art_gv,
        const ArMuellerMatrix        * m0,
        const double                   input_rot_cos,
        const double                   input_rot_sin,
        const double                   output_rot_cos,
        const double                   output_rot_sin,
              ArMuellerMatrix        * mr
        )
{
    /* -----------------------------------------------------------------------
        We first rotate the incident Stokes vector by requested input rotation
        angle, then apply the original Mueller matrix and finally rotate
        the resulting Stokes vector by requested output rotation angle:

            ResultingSV = (OutputRotation * MM * InputRotation) * IncidentSV

        The final concatenated Mueller matrix is as follows:

m00          |m01*Ci+m02*Si                      |m02*Ci-m01*Si                      |m03
m10*Co-m20*So|m11*CiCo+m12*SiCo-m21*CiSo-m22*SiSo|m12*CiCo-m11*SiCo-m22*CiSo+m21*SiSo|m13*Co-m23*So
m20*Co+m10*So|m21*CiCo+m22*SiCo+m11*CiSo+m12*SiSo|m22*CiCo-m21*SiCo+m12*CiSo-m11*SiSo|m23*Co+m13*So
m30          |m31*Ci+m32*Si                      |m32*Ci-m31*Si                      |m33

        Where:
            mij:    original Mueller matrix components
            phiI:   input rotation angle
            phiO:   output rotation angle
            Ci = Cos[2 phiI]
            Co = Cos[2 phiO]
            Si = Sin[2 phiI]
            So = Sin[2 phiO]
            SiSo = Si * So
            SiCo = Si * Co
            CiSo = Ci * So
            CiCo = Ci * Co
    --------------------------------------------------------------------ip- */

    //  Shorthands to make the formulae consistent

    double  cos_phi_i = input_rot_cos;
    double  cos_phi_o = output_rot_cos;
    double  sin_phi_i = input_rot_sin;
    double  sin_phi_o = output_rot_sin;

    //  Switch to double-angle quantities using standard goniometric
    //  double-angle conversions

    const double  cos_2phi_i = M_SQR( cos_phi_i ) - M_SQR( sin_phi_i );
    const double  cos_2phi_o = M_SQR( cos_phi_o ) - M_SQR( sin_phi_o );
    const double  sin_2phi_i = 2 * cos_phi_i * sin_phi_i;
    const double  sin_2phi_o = 2 * cos_phi_o * sin_phi_o;

    //  A few pre-computations

    const double  sin_i_sin_o_2phi = sin_2phi_i * sin_2phi_o;   // SiSo
    const double  sin_i_cos_o_2phi = sin_2phi_i * cos_2phi_o;   // SiCo
    const double  cos_i_sin_o_2phi = cos_2phi_i * sin_2phi_o;   // CiSo
    const double  cos_i_cos_o_2phi = cos_2phi_i * cos_2phi_o;   // CiCo

    // Buffers for intermediate expression

    ArSpectrum  * temp_0 = spc_alloc( art_gv );
    ArSpectrum  * temp_1 = spc_alloc( art_gv );
    ArSpectrum  * temp_2 = spc_alloc( art_gv );

    //  Copied elements (0,0), (0,3), (3,0), (3,3)  ---------------------------

    spc_s_init_s(
        art_gv,
        ARMUELLER_M_II( *m0,  0,0 ),
        ARMUELLER_M_II( *mr,  0,0 )
        );

    spc_s_init_s(
        art_gv,
        ARMUELLER_M_II( *m0,  0,3 ),
        ARMUELLER_M_II( *mr,  0,3 )
        );

    spc_s_init_s(
        art_gv,
        ARMUELLER_M_II( *m0,  3,0 ),
        ARMUELLER_M_II( *mr,  3,0 )
        );

    spc_s_init_s(
        art_gv,
        ARMUELLER_M_II( *m0,  3,3 ),
        ARMUELLER_M_II( *mr,  3,3 )
        );

    //  Element 0,1:  m01 Ci + m02 Si  ----------------------------------------

    spc_ds_mul_s(
        art_gv,
        cos_2phi_i,
        ARMUELLER_M_II( *m0, 0,1 ),
        temp_0
        );

    spc_ds_mul_s(
        art_gv,
        sin_2phi_i,
        ARMUELLER_M_II( *m0, 0,2 ),
        temp_1
        );

    spc_ss_add_s(
        art_gv,
        temp_1,
        temp_0,
        ARMUELLER_M_II( *mr, 0,1 )
        );

    //  Element 0,2:  m02 Ci - m01 Si  ----------------------------------------

    spc_ds_mul_s(
        art_gv,
        cos_2phi_i,
        ARMUELLER_M_II( *m0, 0,2 ),
        temp_0
        );

    spc_ds_mul_s(
        art_gv,
        sin_2phi_i,
        ARMUELLER_M_II( *m0, 0,1 ),
        temp_1
        );

    spc_ss_sub_s(
        art_gv,
        temp_1,
        temp_0,
        ARMUELLER_M_II( *mr, 0,2 )
        );

    //  Element 3,1:  m31 Ci + m32 Si  ----------------------------------------

    spc_ds_mul_s(
        art_gv,
        cos_2phi_i,
        ARMUELLER_M_II( *m0, 3,1 ),
        temp_0
        );

    spc_ds_mul_s(
        art_gv,
        sin_2phi_i,
        ARMUELLER_M_II( *m0, 3,2 ),
        temp_1
        );

    spc_ss_add_s(
        art_gv,
        temp_1,
        temp_0,
        ARMUELLER_M_II( *mr, 3,1 )
        );

    //  Element 3,2:  m32 Ci - m31 Si  ----------------------------------------

    spc_ds_mul_s(
        art_gv,
        cos_2phi_i,
        ARMUELLER_M_II( *m0, 3,2 ),
        temp_0
        );

    spc_ds_mul_s(
        art_gv,
        sin_2phi_i,
        ARMUELLER_M_II( *m0, 3,1 ),
        temp_1
        );

    spc_ss_sub_s(
        art_gv,
        temp_1,
        temp_0,
        ARMUELLER_M_II( *mr, 3,2 )
        );

    //  Element 1,0:  m10 Co - m20 So  ----------------------------------------

    spc_ds_mul_s(
        art_gv,
        cos_2phi_o,
        ARMUELLER_M_II( *m0, 1,0 ),
        temp_0
        );

    spc_ds_mul_s(
        art_gv,
        sin_2phi_o,
        ARMUELLER_M_II( *m0, 2,0 ),
        temp_1
        );

    spc_ss_sub_s(
        art_gv,
        temp_1,
        temp_0,
        ARMUELLER_M_II( *mr, 1,0 )
        );

    //  Element 2,0:  m20 Co + m10 So  ----------------------------------------

    spc_ds_mul_s(
        art_gv,
        cos_2phi_o,
        ARMUELLER_M_II( *m0, 2,0 ),
        temp_0
        );

    spc_ds_mul_s(
        art_gv,
        sin_2phi_o,
        ARMUELLER_M_II( *m0, 1,0 ),
        temp_1
        );

    spc_ss_add_s(
        art_gv,
        temp_1,
        temp_0,
        ARMUELLER_M_II( *mr, 2,0 )
        );

    //  Element 1,3:  m13 Co - m23 So  ----------------------------------------

    spc_ds_mul_s(
        art_gv,
        cos_2phi_o,
        ARMUELLER_M_II( *m0, 1,3 ),
        temp_0
        );

    spc_ds_mul_s(
        art_gv,
        sin_2phi_o,
        ARMUELLER_M_II( *m0, 2,3 ),
        temp_1
        );

    spc_ss_sub_s(
        art_gv,
        temp_1,
        temp_0,
        ARMUELLER_M_II( *mr, 1,3 )
        );

    //  Element 2,3:  m23 Co + m13 So  ----------------------------------------

    spc_ds_mul_s(
        art_gv,
        cos_2phi_o,
        ARMUELLER_M_II( *m0, 2,3 ),
        temp_0
        );

    spc_ds_mul_s(
        art_gv,
        sin_2phi_o,
        ARMUELLER_M_II( *m0, 1,3 ),
        temp_1
        );

    spc_ss_add_s(
        art_gv,
        temp_1,
        temp_0,
        ARMUELLER_M_II( *mr, 2,3 )
        );

    //  Element 1,1:  ((m11 CiCo + m12 SiCo) - m21 CiSo) - m22 SiSo  ----------

    spc_ds_mul_s(
        art_gv,
        cos_i_cos_o_2phi,
        ARMUELLER_M_II( *m0, 1,1 ),
        temp_0
        );

    spc_ds_mul_s(
        art_gv,
        sin_i_cos_o_2phi,
        ARMUELLER_M_II( *m0, 1,2 ),
        temp_1
        );

    spc_ss_add_s(
        art_gv,
        temp_1,
        temp_0,
        temp_2
        );

    spc_ds_mul_s(
        art_gv,
        cos_i_sin_o_2phi,
        ARMUELLER_M_II( *m0, 2,1 ),
        temp_1
        );

    spc_ss_sub_s(
        art_gv,
        temp_1,
        temp_2,
        temp_0
        );

    spc_ds_mul_s(
        art_gv,
        sin_i_sin_o_2phi,
        ARMUELLER_M_II( *m0, 2,2 ),
        temp_1
        );

    spc_ss_sub_s(
        art_gv,
        temp_1,
        temp_0,
        ARMUELLER_M_II( *mr, 1,1 )
        );

    //  Element 1,2:  ((m12 CiCo - m11 SiCo) - m22 CiSo) + m21 SiSo  ----------

    spc_ds_mul_s(
        art_gv,
        cos_i_cos_o_2phi,
        ARMUELLER_M_II( *m0, 1,2 ),
        temp_0
        );

    spc_ds_mul_s(
        art_gv,
        sin_i_cos_o_2phi,
        ARMUELLER_M_II( *m0, 1,1 ),
        temp_1
        );

    spc_ss_sub_s(
        art_gv,
        temp_1,
        temp_0,
        temp_2
        );

    spc_ds_mul_s(
        art_gv,
        cos_i_sin_o_2phi,
        ARMUELLER_M_II( *m0, 2,2 ),
        temp_1
        );

    spc_ss_sub_s(
        art_gv,
        temp_1,
        temp_2,
        temp_0
        );

    spc_ds_mul_s(
        art_gv,
        sin_i_sin_o_2phi,
        ARMUELLER_M_II( *m0, 2,1 ),
        temp_1
        );

    spc_ss_add_s(
        art_gv,
        temp_1,
        temp_0,
        ARMUELLER_M_II( *mr, 1,2 )
        );

    //  Element 2,1:  ((m21 CiCo + m22 SiCo) + m11 CiSo) + m12 SiSo  ----------

    spc_ds_mul_s(
        art_gv,
        cos_i_cos_o_2phi,
        ARMUELLER_M_II( *m0, 2,1 ),
        temp_0
        );

    spc_ds_mul_s(
        art_gv,
        sin_i_cos_o_2phi,
        ARMUELLER_M_II( *m0, 2,2 ),
        temp_1
        );

    spc_ss_add_s(
        art_gv,
        temp_1,
        temp_0,
        temp_2
        );

    spc_ds_mul_s(
        art_gv,
        cos_i_sin_o_2phi,
        ARMUELLER_M_II( *m0, 1,1 ),
        temp_1
        );

    spc_ss_add_s(
        art_gv,
        temp_1,
        temp_2,
        temp_0
        );

    spc_ds_mul_s(
        art_gv,
        sin_i_sin_o_2phi,
        ARMUELLER_M_II( *m0, 1,2 ),
        temp_1
        );

    spc_ss_add_s(
        art_gv,
        temp_1,
        temp_0,
        ARMUELLER_M_II( *mr, 2,1 )
        );

    //  Element 2,2:  ((m22 CiCo - m21 SiCo) + m12 CiSo) - m11 SiSo  ----------

    spc_ds_mul_s(
        art_gv,
        cos_i_cos_o_2phi,
        ARMUELLER_M_II( *m0, 2,2 ),
        temp_0
        );

    spc_ds_mul_s(
        art_gv,
        sin_i_cos_o_2phi,
        ARMUELLER_M_II( *m0, 2,1 ),
        temp_1
        );

    spc_ss_sub_s(
        art_gv,
        temp_1,
        temp_0,
        temp_2
        );

    spc_ds_mul_s(
        art_gv,
        cos_i_sin_o_2phi,
        ARMUELLER_M_II( *m0, 1,2 ),
        temp_1
        );

    spc_ss_add_s(
        art_gv,
        temp_1,
        temp_2,
        temp_0
        );

    spc_ds_mul_s(
        art_gv,
        sin_i_sin_o_2phi,
        ARMUELLER_M_II( *m0, 1,1 ),
        temp_1
        );

    spc_ss_sub_s(
        art_gv,
        temp_1,
        temp_0,
        ARMUELLER_M_II( *mr, 2,2 )
        );

    //  Temps clean-up  -------------------------------------------------------

    spc_free( art_gv, temp_0 );
    spc_free( art_gv, temp_1 );
    spc_free( art_gv, temp_2 );

    ASSERT_VALID_MUELLER_MATRIX(mr);
}

unsigned int armuellermatrix_mmd_equal(
        const ART_GV           * art_gv,
        const ArMuellerMatrix  * mm0,
        const ArMuellerMatrix  * mm1,
        const double             max_relative_error
        )
{
    ASSERT_ALLOCATED_MUELLER_MATRIX(mm0);
    ASSERT_ALLOCATED_MUELLER_MATRIX(mm1);
    ASSERT_VALID_MUELLER_MATRIX(mm0);
    ASSERT_VALID_MUELLER_MATRIX(mm1);

    unsigned int  result = 1;

    for ( unsigned int i = 0; i < 16; i++ )
    {
        if ( ! spc_ssd_equal(
                 art_gv,
                 ARMUELLER_M_I( *mm0, i ),
                 ARMUELLER_M_I( *mm1, i ),
                 max_relative_error
                 ))
        {
            result = 0;
            break;
        }
    }

    return result;
}

// Formatting shortcuts for validity functions
#define USE_FORMATED_VALIDITY_OUTPUT
#ifdef USE_FORMATED_VALIDITY_OUTPUT
    #define OUTPUT_FORMAT_SUCCEEDED_START   ART_STRING_BOLD
    #define OUTPUT_FORMAT_SUCCEEDED_END     ART_STRINGCOL_NORMAL
    #define OUTPUT_FORMAT_FAILED_START      ART_STRING_BOLD ART_STRINGCOL_RED
    #define OUTPUT_FORMAT_FAILED_END        ART_STRINGCOL_NORMAL ART_STRING_NORMAL
#else
    #define OUTPUT_FORMAT_SUCCEEDED_START
    #define OUTPUT_FORMAT_SUCCEEDED_END
    #define OUTPUT_FORMAT_FAILED_START
    #define OUTPUT_FORMAT_FAILED_END
#endif

unsigned int armuellermatrix_m_colour_valid(
        const ART_GV           * art_gv,
        const ArMuellerMatrix  * mm0,
        unsigned int             debug_print_only
        )

{
    for (unsigned int mm_component = 0; mm_component < 16; mm_component++)
    {
        const ArSpectrum * colour = ARMUELLER_M_I( *mm0, mm_component );

        if ( ! spc_s_valid( art_gv, colour ) )
        {
            if ( ! debug_print_only )
                ART_ERRORHANDLING_WARNING(
                    "invalid colour in the Müller matrix component [%d, %d], "
                    "skipping the test for the remaining channels...",
                    mm_component / 4,
                    mm_component % 4
                    );
            else
                printf(
                    OUTPUT_FORMAT_FAILED_START
                    "invalid colour"
                    OUTPUT_FORMAT_FAILED_END
                    " at [%d, %d], ",
                    mm_component / 4,
                    mm_component % 4
                    );

            return NO;
        }
    }

    return YES;
}

unsigned int armuellermatrix_m_nonpol_att_valid(
        const ART_GV           * art_gv,
        const ArMuellerMatrix  * mm0,
        unsigned int             allow_attenuation_greater_than_one,
        unsigned int             debug_print_only
        )

{
    const double num_threshold = 1E-3; //1E-5; // empirical value

    const unsigned int channels_count = spc_channels( art_gv );
    const ArSpectrum * colour = ARMUELLER_M_II( *mm0, 0, 0 );

    for ( unsigned int channel = 0; channel < channels_count; channel++ )
    {
        double spc_value = spc_si( art_gv, colour, channel );

        if (    ( spc_value < (0.0 - num_threshold) )
             || (    ! allow_attenuation_greater_than_one
                  && ( spc_value > (1.0 + num_threshold) ) )
            )
        {
            if ( ! debug_print_only )
            {
                armuellermatrix_mii_debugprintf(
                    art_gv,
                    mm0,
                    channel,
                    2
                    );
                ART_ERRORHANDLING_WARNING(
                    "colour value outside interval 0..1 (%+.6f) is present "
                    "in the Mueller matrix component [0, 0] in channel %d, "
                    "skipping the test for the remaining channels...",
                    spc_value,
                    channel
                    );
            }
            else if ( channel == 0 )
            {
                printf(
                    "[0,0] "
                    OUTPUT_FORMAT_FAILED_START
                    "outside [0..1]"
                    OUTPUT_FORMAT_FAILED_END
                    " (%+.4f), ",
                    spc_value
                    );
            }

            return NO;
        }
        else if ( channel == 0 && debug_print_only )
        {
            printf(
                "[0,0] is "
                OUTPUT_FORMAT_SUCCEEDED_START
                "OK"
                OUTPUT_FORMAT_SUCCEEDED_END
                "                   , "
                );
        }
    }

    return YES;
}

unsigned int armuellermatrix_m_sanity_check(
        const ART_GV           * art_gv,
        const ArMuellerMatrix  * mm0,
        unsigned int             debug_print_only
        )

{
    const double diff_threshold = -1E-1; //-1E-5; // empirical value

    const unsigned int channels_count   = spc_channels( art_gv );
    ArSpectrum * sanity_sum               = spc_alloc(art_gv);
    ArSpectrum * colour_2                 = spc_alloc(art_gv);
    ArSpectrum * sanity_threshold         = spc_alloc(art_gv);
    ArSpectrum * sanity_diff              = spc_alloc(art_gv);

    spc_d_init_s(art_gv, 0, sanity_sum);

    // Prepare sanity check sum
    for (unsigned int mm_component = 0; mm_component < 16; mm_component++)
    {
        const ArSpectrum * colour = ARMUELLER_M_I( *mm0, mm_component );

        spc_ss_mul_s(art_gv, colour, colour, colour_2);
        spc_s_add_s(art_gv, colour_2, sanity_sum);
    }

    unsigned int result = YES;

    // The actual check

    const ArSpectrum * component_00 = ARMUELLER_M_I( *mm0, 0 );
    spc_ss_mul_s(art_gv, component_00, component_00, sanity_threshold);
    spc_ds_mul_s(art_gv, 4, sanity_threshold, sanity_threshold);
    spc_ss_sub_s(art_gv, sanity_sum, sanity_threshold, sanity_diff); // threshold - sum

    for (unsigned int channel = 0; channel < channels_count; channel++)
    {
        double diff      = spc_si( art_gv, sanity_diff, channel );
        if (diff < diff_threshold)
        {
            if ( ! debug_print_only )
            {
                armuellermatrix_mii_debugprintf(
                    art_gv,
                    mm0,
                    channel,
                    2
                    );
                ART_ERRORHANDLING_WARNING(
                    "sanity check failed for colour channel %d, the sanity criterion is negative (%.6f, threshold is %.6f), "
                    "skipping the test for the remaining channels...",
                    channel,
                    diff,
                    diff_threshold
                    );
            }
            else if ( channel == 0 && debug_print_only )
                printf(
                    "sanity check "
                    OUTPUT_FORMAT_FAILED_START
                    "FAILED"
                    OUTPUT_FORMAT_FAILED_END
                    //" (%.4f-%.4f = %+.8f, )"
                    " (%+.8f), ",
                    //sum,
                    diff
                    //diff_threshold
                    );

            result = NO;
            break;
        }
        else if ( channel == 0 && debug_print_only )
        {
            printf(
                "sanity check "
                OUTPUT_FORMAT_SUCCEEDED_START
                "OK    "
                OUTPUT_FORMAT_SUCCEEDED_END
                //" (%.4f-%.4f = %+.8f > %+.8f), ",
                "              , "
                /*sum,
                threshold,
                diff,
                diff_threshold*/
                );
        }
    }

    spc_free(art_gv, colour_2);
    spc_free(art_gv, sanity_sum);
    spc_free(art_gv, sanity_threshold);
    spc_free(art_gv, sanity_diff);

    return result;
}

/* --------------------------------------------------------------------------
    This function implements test of physical realisability of the optical
    element represented by a given Mueller matrix. The test is based on
    eigenvalue analysis and its description can be found in the book
    "Polarized Light" by Dennis Goldstein, second edition.
--------------------------------------------------------------------------ip- */

unsigned int armuellermatrix_m_realisable(
        const ART_GV           * art_gv,
        const ArMuellerMatrix  * mm0,
        unsigned int             debug_print_only
        )
{
#if (GSL_FOUND != 1)

    ART_ERRORHANDLING_FATAL_ERROR(
        "GNU Scientific Library (GSL), required for Mueller matrix analysis, was NOT found"
        );
    return NO;

#else // (GSL_FOUND != 1)

    const double num_threshold = 1E-3; //1E-5; // empirical value

    gsl_eigen_herm_workspace * gsl_workspace = gsl_eigen_herm_alloc(4);
    gsl_matrix_complex * coherency_matrix = gsl_matrix_complex_alloc (4, 4);
    gsl_vector * eigen_values = gsl_vector_alloc (4);
    if (   ( gsl_workspace == NULL )
        || ( coherency_matrix == NULL )
        || ( eigen_values == NULL )
        )
    {
        ART_ERRORHANDLING_FATAL_ERROR(
            "unable to alocate GSL data for Mueller matrix eigensystem decomposition"
            );
    }

    unsigned int result = YES;

    const unsigned int channels_count = spc_channels( art_gv );
    for ( unsigned int channel = 0; channel < channels_count; channel++ )
    {
        // Prepare nicely accessible components of the Mueller matrix.
        // Names of the variables are indexed from 1 instead of from 0 so that
        // it is consistent with the formulae in the Blue book thus making it
        // easier to check the correctness of the formulae.

        const double m_11 = spc_si( art_gv, ARMUELLER_M_II( *mm0, 0, 0 ), channel );
        const double m_12 = spc_si( art_gv, ARMUELLER_M_II( *mm0, 0, 1 ), channel );
        const double m_13 = spc_si( art_gv, ARMUELLER_M_II( *mm0, 0, 2 ), channel );
        const double m_14 = spc_si( art_gv, ARMUELLER_M_II( *mm0, 0, 3 ), channel );

        const double m_21 = spc_si( art_gv, ARMUELLER_M_II( *mm0, 1, 0 ), channel );
        const double m_22 = spc_si( art_gv, ARMUELLER_M_II( *mm0, 1, 1 ), channel );
        const double m_23 = spc_si( art_gv, ARMUELLER_M_II( *mm0, 1, 2 ), channel );
        const double m_24 = spc_si( art_gv, ARMUELLER_M_II( *mm0, 1, 3 ), channel );

        const double m_31 = spc_si( art_gv, ARMUELLER_M_II( *mm0, 2, 0 ), channel );
        const double m_32 = spc_si( art_gv, ARMUELLER_M_II( *mm0, 2, 1 ), channel );
        const double m_33 = spc_si( art_gv, ARMUELLER_M_II( *mm0, 2, 2 ), channel );
        const double m_34 = spc_si( art_gv, ARMUELLER_M_II( *mm0, 2, 3 ), channel );

        const double m_41 = spc_si( art_gv, ARMUELLER_M_II( *mm0, 3, 0 ), channel );
        const double m_42 = spc_si( art_gv, ARMUELLER_M_II( *mm0, 3, 1 ), channel );
        const double m_43 = spc_si( art_gv, ARMUELLER_M_II( *mm0, 3, 2 ), channel );
        const double m_44 = spc_si( art_gv, ARMUELLER_M_II( *mm0, 3, 3 ), channel );

        // Fill the complex hermitian "coherency matrix".
        // The coherency matrix is, again, indexed from 1 instead of from 0 so that
        // it is consistent with the formulae in the Blue book thus making it
        // easier to check the correctness of the formulae.

        #define SET_COHERENCY_MATRIX_ELEMENT(__i, __j,__real,__imaginary) \
            GSL_SET_COMPLEX( \
                &complex_val, \
                (__real), \
                (__imaginary) \
                ); \
            gsl_matrix_complex_set( \
                coherency_matrix, \
                (__i) - 1, \
                (__j) - 1, \
                complex_val \
                );

        gsl_complex complex_val;

        SET_COHERENCY_MATRIX_ELEMENT(1, 1, (m_11 + m_22 + m_33 + m_44) / 2.0, 0.0);
        SET_COHERENCY_MATRIX_ELEMENT(1, 2, (m_12 + m_21) / 2.0, -(m_34 - m_43) / 2.0);
        SET_COHERENCY_MATRIX_ELEMENT(1, 3, (m_13 + m_31) / 2.0,  (m_24 - m_42) / 2.0);
        SET_COHERENCY_MATRIX_ELEMENT(1, 4, (m_14 + m_41) / 2.0, -(m_23 - m_32) / 2.0);

        SET_COHERENCY_MATRIX_ELEMENT(2, 1, (m_12 + m_21) / 2.0,  (m_34 - m_43) / 2.0);
        SET_COHERENCY_MATRIX_ELEMENT(2, 2, (m_11 + m_22 - m_33 - m_44) / 2.0, 0.0);
        SET_COHERENCY_MATRIX_ELEMENT(2, 3, (m_23 + m_32) / 2.0,  (m_14 - m_41) / 2.0);
        SET_COHERENCY_MATRIX_ELEMENT(2, 4, (m_24 + m_42) / 2.0, -(m_13 - m_31) / 2.0);

        SET_COHERENCY_MATRIX_ELEMENT(3, 1, (m_13 + m_31) / 2.0, -(m_24 - m_42) / 2.0);
        SET_COHERENCY_MATRIX_ELEMENT(3, 2, (m_23 + m_32) / 2.0, -(m_14 - m_41) / 2.0);
        SET_COHERENCY_MATRIX_ELEMENT(3, 3, (m_11 - m_22 + m_33 - m_44) / 2.0, 0.0);
        SET_COHERENCY_MATRIX_ELEMENT(3, 4, (m_34 + m_43) / 2.0,  (m_12 - m_21) / 2.0);

        SET_COHERENCY_MATRIX_ELEMENT(4, 1, (m_14 + m_41) / 2.0,  (m_23 - m_32) / 2.0);
        SET_COHERENCY_MATRIX_ELEMENT(4, 2, (m_24 + m_42) / 2.0,  (m_13 - m_31) / 2.0);
        SET_COHERENCY_MATRIX_ELEMENT(4, 3, (m_34 + m_43) / 2.0, -(m_12 - m_21) / 2.0);
        SET_COHERENCY_MATRIX_ELEMENT(4, 4, (m_11 - m_22 - m_33 + m_44) / 2.0, 0.0);

        int eigen_status =
            gsl_eigen_herm(
                coherency_matrix,
                eigen_values,
                gsl_workspace
                );
        if ( eigen_status )
        {
            ART_ERRORHANDLING_FATAL_ERROR(
                "unable to perform eigenvalues analysis, "
                "gsl_eigen_herm() returned an error(%d): \"%s\"",
                eigen_status,
                gsl_strerror(eigen_status)
                );
        }
        else
        {
            // The analysis itself

            double min_eigen_val;
            double max_eigen_val;

            gsl_vector_minmax(eigen_values, &min_eigen_val, &max_eigen_val);

            if ( debug_print_only && ( channel == 0 ) )
            {
                printf(
                    "CM MMEVs: %+.10f, %+.10f, ",
                    min_eigen_val,
                    max_eigen_val
                    );
            }

            if (( min_eigen_val > -num_threshold ) &&
                ( max_eigen_val <  num_threshold ))
            {
                // FIXME: All zeroes: special case. Is this OK?

                if ( debug_print_only && ( channel == 0 ) )
                {
                    printf(
                        OUTPUT_FORMAT_SUCCEEDED_START
                        "EVA: Zeroes, OK?                           "
                        OUTPUT_FORMAT_SUCCEEDED_END
                        ", "
                        );
                }
            }
            else if ( max_eigen_val < num_threshold )
            {
                // All are negative: very bad

                if ( ! debug_print_only )
                {
                    armuellermatrix_mii_debugprintf(
                        art_gv,
                        mm0,
                        channel,
                        2
                        );
                    ART_ERRORHANDLING_WARNING(
                        "eigenvalue analysis for Mueller matrix failed for channel %d "
                        "(all eigenvalues are negative), "
                        "skipping the test for the remaining channels...",
                        channel
                        );
                }
                else if ( channel == 0 )
                {
                    printf(
                        OUTPUT_FORMAT_FAILED_START
                        "EVA: All negative, FAILED                  "
                        OUTPUT_FORMAT_FAILED_END
                        ", "
                        );
                }

                result = NO;
                break;
            }
            else if ( min_eigen_val < -num_threshold )
            {
                // Only some of them are negative: bad

                double max_evr = 10.0 * log10(M_ABS(min_eigen_val) / max_eigen_val);

                if ( ! debug_print_only )
                {
                    armuellermatrix_mii_debugprintf(
                        art_gv,
                        mm0,
                        channel,
                        2
                        );
                    ART_ERRORHANDLING_WARNING(
                        "eigenvalue analysis for Mueller matrix failed for channel %d "
                        "(negative eigenvalues are present, EVR: %3.2f[dB]), "
                        "skipping the test for the remaining channels...",
                        channel,
                        max_evr
                        );
                }
                else if ( channel == 0 )
                {
                    printf(
                        OUTPUT_FORMAT_FAILED_START
                        "EVA: Some negative, EVR: %3.2f[dB], FAILED"
                        OUTPUT_FORMAT_FAILED_END
                        ", ",
                        max_evr
                        );
                }

                result = NO;
                break;
            }
            else
            {
                // All eigenvalues are non-negative: OK

                if ( debug_print_only && ( channel == 0 ) )
                {
                    printf(
                        OUTPUT_FORMAT_SUCCEEDED_START
                        "EVA: OK                                    "
                        OUTPUT_FORMAT_SUCCEEDED_END
                        ", "
                        );
                }
            }
        }
    }

    gsl_eigen_herm_free(gsl_workspace);
    gsl_matrix_complex_free(coherency_matrix);
    gsl_vector_free(eigen_values);

    return result;

#endif // (GSL_FOUND != 1)
}

unsigned int armuellermatrix_m_valid(
        const ART_GV           * art_gv,
        const ArMuellerMatrix  * mm0,
        unsigned int             allow_attenuation_greater_than_one,
        unsigned int             debug_print_only
        )
{
    ASSERT_ALLOCATED_MUELLER_MATRIX(mm0);

    unsigned int result = YES;

    // General colour check
    if ( ! armuellermatrix_m_colour_valid( art_gv, mm0, debug_print_only ) )
    {
        result = NO;
    }
    else
    {
        // Non-polarizing attenuation value check
        if ( !
                armuellermatrix_m_nonpol_att_valid(
                    art_gv,
                    mm0,
                    allow_attenuation_greater_than_one,
                    debug_print_only
                    )
            )
        {
            result = NO;
        }

        // Actual Mueller matrix sanity check
        if ( ! armuellermatrix_m_sanity_check( art_gv, mm0, debug_print_only ) )
        {
            result = NO;
        }

        // Eigenvalue realizability analysis
        if ( ! armuellermatrix_m_realisable( art_gv, mm0, debug_print_only ) )
        {
            result = NO;
        }
    }

    if ( debug_print_only )
    {
        if ( result )
            printf(
                "["
                OUTPUT_FORMAT_SUCCEEDED_START
                "OK"
                OUTPUT_FORMAT_SUCCEEDED_END
                "]"
                );
        else
            printf(
                "["
                OUTPUT_FORMAT_FAILED_START
                "FAILED"
                OUTPUT_FORMAT_FAILED_END
                "]"
                );
    }

    return result;
}

void armuellermatrix_d_randomly_damage_m(
        const ART_GV           * art_gv,
        const double             damage,
              ArMuellerMatrix  * mm
        )
{
    ASSERT_ALLOCATED_MUELLER_MATRIX(mm);
    ASSERT_VALID_MUELLER_MATRIX(mm);

    #define DAMAGE_MM_COMPONENT(__component) \
    {\
        colour = ARMUELLER_M_I( *mm, __component ); \
        for (unsigned int channel = 0; channel < channels_count; channel++) \
        { \
            const double random_val = ( 2.0 * rand() / (double)RAND_MAX ) - 1.0; \
            double value = spc_si( art_gv, colour, channel ); \
            value += random_val * damage; \
            spc_set_sid( art_gv, colour, channel, value ); \
        } \
    }

    ArSpectrum * colour = NULL;
    unsigned int channels_count = spc_channels( art_gv );

    for ( unsigned int i = 0; i < 16; i++ )
        DAMAGE_MM_COMPONENT( i );
}

void armuellermatrix_mi_debugprintf(
        const ART_GV           * art_gv,
        const ArMuellerMatrix  * mm0,
        unsigned int             precision
        )
{
    unsigned int  channel;

    channel = spc_channels(art_gv) / 2;

    armuellermatrix_mii_debugprintf(art_gv, mm0, channel, precision );
}

void armuellermatrix_mii_debugprintf(
        const ART_GV           * art_gv,
        const ArMuellerMatrix  * mm0,
        unsigned int             channel,
        unsigned int             precision
        )
{
    channel = M_MIN(channel, spc_channels(art_gv));


    printf(
        "Müller matrix for colour channel %d:\n"
        "| % 5.*f % 5.*f % 5.*f % 5.*f | \n"
        "| % 5.*f % 5.*f % 5.*f % 5.*f | \n"
        "| % 5.*f % 5.*f % 5.*f % 5.*f | \n"
        "| % 5.*f % 5.*f % 5.*f % 5.*f | \n"
        ,
        channel,
        precision,
        spc_si( art_gv,ARMUELLER_M_I( *mm0, 0 ),channel ),
        precision,
        spc_si( art_gv,ARMUELLER_M_I( *mm0, 1 ),channel ),
        precision,
        spc_si( art_gv,ARMUELLER_M_I( *mm0, 2 ),channel ),
        precision,
        spc_si( art_gv,ARMUELLER_M_I( *mm0, 3 ),channel ),
        precision,
        spc_si( art_gv,ARMUELLER_M_I( *mm0, 4 ),channel ),
        precision,
        spc_si( art_gv,ARMUELLER_M_I( *mm0, 5 ),channel ),
        precision,
        spc_si( art_gv,ARMUELLER_M_I( *mm0, 6 ),channel ),
        precision,
        spc_si( art_gv,ARMUELLER_M_I( *mm0, 7 ),channel ),
        precision,
        spc_si( art_gv,ARMUELLER_M_I( *mm0, 8 ),channel ),
        precision,
        spc_si( art_gv,ARMUELLER_M_I( *mm0, 9 ),channel ),
        precision,
        spc_si( art_gv,ARMUELLER_M_I( *mm0,10 ),channel ),
        precision,
        spc_si( art_gv,ARMUELLER_M_I( *mm0,11 ),channel ),
        precision,
        spc_si( art_gv,ARMUELLER_M_I( *mm0,12 ),channel ),
        precision,
        spc_si( art_gv,ARMUELLER_M_I( *mm0,13 ),channel ),
        precision,
        spc_si( art_gv,ARMUELLER_M_I( *mm0,14 ),channel ),
        precision,
        spc_si( art_gv,ARMUELLER_M_I( *mm0,15 ),channel )
        );
}

/* ---------------------------------------------------------------------------
    Routines for testing and debugging purposes
------------------------------------------------------------------------ip- */

void armuellermatrix_test_synthetic_MMs_validity(
        const ART_GV           * art_gv
        )
{
    ArMuellerMatrix * mm = armuellermatrix_alloc(art_gv);
    ArSpectrum * colour = spc_alloc(art_gv) ;

    // Tests of correct synthetical MMs:

    //armuellermatrix_d_init_nonpolarising_m(art_gv, 0.5, mm);
    //armuellermatrix_mii_debugprintf(art_gv, mm, 0, 2);
    //printf("\n");
    //printf("Checking plain attenuator (%+.8f): ", 0.5);
    //armuellermatrix_m_valid(art_gv, mm, NO, YES);
    //printf("\n");

    // Plain attenuator (non-polarizing MM)
    //for (double attenuation = -0.0; attenuation <= 1.10001; attenuation += 0.1)
    //{
    //    armuellermatrix_d_init_nonpolarising_m(art_gv, attenuation, mm);
    //
    //    printf("Checking plain attenuator (%+.8f): ", attenuation);
    //    armuellermatrix_m_valid(art_gv, mm, NO, YES);
    //    printf("\n");
    //
    //    // debug
    //    //armuellermatrix_mii_debugprintf(art_gv, mm, 0, 2);
    //    //printf("\n");
    //}
    //printf("\n");

    // Plain attenuator (non-polarizing MM)
    //for (double attenuation = -0.2; attenuation < 1.2; attenuation += 0.1)
    //{
    //    spc_d_init_s(art_gv, attenuation, colour);
    //    armuellermatrix_c_init_nonpolarising_m(art_gv, colour, mm);
    //
    //    printf("Checking plain attenuator (%+.8f): ", attenuation);
    //    armuellermatrix_m_valid(art_gv, mm, NO, YES);
    //    printf("\n");
    //}
    //printf("\n");

    // Plain isotropic depolarizer
    for (double attenuation = -0.2; attenuation <= 1.3; attenuation += 0.1)
    {
        for (double depolarization = -0.1; depolarization <= attenuation + 0.0001; depolarization += 0.1)
        {
            //armuellermatrix_d_init_depolarising_m(art_gv, attenuation, mm);
            armuellermatrix_dd_init_depolarising_m(art_gv, attenuation, depolarization, mm);

            // debug
            //armuellermatrix_mii_debugprintf(art_gv, mm, 0, 2);

            printf("Checking isotropic depolarizer (%+.3f, %+.3f): ", attenuation, depolarization);
            armuellermatrix_m_valid(art_gv, mm, NO, YES);
            printf("\n");

            // debug
            //printf("\n");
        }
        // debug
        //printf("---------------------------------------------------\n\n");
    }
    printf("\n");

    // Plain isotropic depolarizer
    //for (double attenuation = -0.5; attenuation < 1.5; attenuation += 0.01)
    //{
    //    spc_d_init_s(art_gv, attenuation, colour);
    //    armuellermatrix_c_init_depolarising_m(art_gv, colour, mm);
    //    printf("Checking isotropic depolarizer (%+.8f): ", attenuation);
    //
    //    armuellermatrix_m_valid(art_gv, mm, NO, YES);
    //    printf("\n");
    //}
    //printf("\n");

    // Linear polarizer
    //for (double angle = -M_PI; angle <= (2 * M_PI + 0.01); angle += M_PI/100.0)
    //{
    //    //for (double strength = 0.0; strength <= 1.0; strength += 0.25)
    //    double strength = 1.0;
    //    {
    //        armuellermatrix_dd_init_linear_polariser_m(art_gv, angle, strength, mm);
    //        printf("Checking linear polarizer (angle: %+.2f, strength: %+.2f): ", angle, strength);
    //
    //        armuellermatrix_m_valid(art_gv, mm, NO, YES);
    //        printf("\n");
    //
    //        // debug
    //        //armuellermatrix_mii_debugprintf(art_gv, mm, 0, 2);
    //        //printf("\n");
    //    }
    //    //printf("\n");
    //}
    //printf("\n");

    // Retarder (phase shifter)
    //for (double angle = -M_PI_2; angle <= (2 * M_PI + 0.01); angle += M_PI/100.0)
    //{
    //    armuellermatrix_d_init_phase_shifter_m(art_gv, angle, mm);
    //    printf("Checking phase shifter (%+.2f): ", angle);
    //
    //    armuellermatrix_m_valid(art_gv, mm, NO, YES);
    //    printf("\n");
    //
    //    // debug
    //    //armuellermatrix_mii_debugprintf(art_gv, mm, 0, 2);
    //    //printf("\n");
    //}
    //printf("\n");

    // Rotator
    //for (double angle = -M_PI_2; angle <= (2 * M_PI + 0.01); angle += M_PI/100.0)
    //{
    //    armuellermatrix_d_init_rotator_m(art_gv, angle, mm);
    //    printf("Checking rotator (%+.2f): ", angle);
    //
    //    armuellermatrix_m_valid(art_gv, mm, NO, YES);
    //    printf("\n");
    //
    //    // debug
    //    //armuellermatrix_mii_debugprintf(art_gv, mm, 0, 2);
    //    //printf("\n");
    //}
    //printf("\n");

    armuellermatrix_free(art_gv, mm);
    spc_free(art_gv, colour);
}

void armuellermatrix_test_damaged_MMs_validity(
        const ART_GV           * art_gv
        )
{
    ArMuellerMatrix * mm = armuellermatrix_alloc(art_gv);

    //const double damage = 0.0001;

    // Damaged plain attenuator (non-polarizing MM)
    //for (double attenuation = -0.5; attenuation <= 1.5; attenuation += 0.1)
    //{
    //    for (unsigned int iteration = 0; iteration < 10; iteration++)
    //    {
    //        armuellermatrix_d_init_nonpolarising_m(art_gv, attenuation, mm);
    //        armuellermatrix_d_randomly_damage_m(art_gv, damage, mm);
    //
    //        // debug
    //        //armuellermatrix_mii_debugprintf(art_gv, mm, 0, 2);
    //
    //        printf(
    //            "Damaged (%.4f) plain attenuator (%+.8f): ",
    //            damage,
    //            attenuation
    //            );
    //        armuellermatrix_m_valid(art_gv, mm, NO, YES);
    //        printf("\n");
    //
    //        // debug
    //        //printf("\n");
    //    }
    //    // debug
    //    printf("---------------------------------------------------\n");
    //}
    //printf("\n");

    // Damaged plain isotropic depolarizer
    //for (double attenuation = -0.1; attenuation <= 0.1; attenuation += 0.1)
    //{
    //    for (double depolarization = -0.1; depolarization <= attenuation + 0.1001; depolarization += 0.1)
    //    {
    //        armuellermatrix_dd_init_depolarising_m(art_gv, attenuation, depolarization, mm);
    //        armuellermatrix_d_randomly_damage_m(art_gv, damage, mm);
    //
    //        // debug
    //        //armuellermatrix_mii_debugprintf(art_gv, mm, 0, 2);
    //
    //        printf(
    //            "Damaged (%.4f) isotropic depolarizer (%+.3f, %+.3f): ",
    //            damage, attenuation, depolarization
    //            );
    //        armuellermatrix_m_valid(art_gv, mm, NO, YES);
    //        printf("\n");
    //
    //        // debug
    //        //printf("\n");
    //    }
    //    // debug
    //    printf("---------------------------------------------------\n\n");
    //}
    //printf("\n");

    // Damaged linear polarizer
    //for (double angle = -M_PI; angle <= (2 * M_PI + 0.01); angle += M_PI/10.0)
    //{
    //    // debug
    //    printf(
    //           "------------------- "
    //           "Angle: %+.2f: "
    //           "-------------------\n\n", angle);
    //
    //    for (double strength = 0.0; strength <= 1.0001; strength += 0.1)
    //    //double strength = 1.0;
    //    {
    //        armuellermatrix_dd_init_linear_polariser_m(art_gv, angle, strength, mm);
    //        armuellermatrix_d_randomly_damage_m(art_gv, damage, mm);
    //
    //        // debug
    //        //armuellermatrix_mii_debugprintf(art_gv, mm, 0, 2);
    //
    //        printf(
    //            "Damaged (%.4f) linear polarizer (angle: %+.2f, strength: %+.2f): ",
    //            damage, angle, strength);
    //        armuellermatrix_m_valid(art_gv, mm, NO, YES);
    //        printf("\n");
    //
    //        // debug
    //        //printf("\n");
    //    }
    //    printf("\n");
    //}
    //printf("\n");

    // Damaged retarder (phase shifter)
    //for (double angle = -M_PI_2; angle <= (2 * M_PI + 0.01); angle += M_PI/100.0)
    //{
    //    armuellermatrix_d_init_phase_shifter_m(art_gv, angle, mm);
    //    armuellermatrix_d_randomly_damage_m(art_gv, damage, mm);
    //
    //    // debug
    //    //armuellermatrix_mii_debugprintf(art_gv, mm, 0, 2);
    //
    //    printf("Damaged (%.4f) phase shifter (%+.2f): ", damage, angle);
    //    armuellermatrix_m_valid(art_gv, mm, NO, YES);
    //    printf("\n");
    //
    //    // debug
    //    //printf("\n");
    //}
    //printf("\n");

    // Damaged rotator
    //for (double angle = -M_PI_2; angle <= (2 * M_PI + 0.01); angle += M_PI/100.0)
    //{
    //    armuellermatrix_d_init_rotator_m(art_gv, angle, mm);
    //    armuellermatrix_d_randomly_damage_m(art_gv, damage, mm);
    //
    //    // debug
    //    //armuellermatrix_mii_debugprintf(art_gv, mm, 0, 2);
    //
    //    printf("Damaged (%.4f) rotator (%+.2f): ", damage, angle);
    //    armuellermatrix_m_valid(art_gv, mm, NO, YES);
    //    printf("\n");
    //
    //    // debug
    //    //printf("\n");
    //}
    //printf("\n");

    armuellermatrix_free(art_gv, mm);
}

void armuellermatrix_test_blended_MMs_validity(
        const ART_GV           * art_gv
        )
{
    ArMuellerMatrix * mm1       = armuellermatrix_alloc(art_gv);
    ArMuellerMatrix * mm2       = armuellermatrix_alloc(art_gv);
    ArMuellerMatrix * mm_blend  = armuellermatrix_alloc(art_gv);

    // Rotator blended with linear polarizer
    //for (double rot_angle = 0.0; rot_angle <= (2 * M_PI + 0.01); rot_angle += M_PI/10.0)
    //{
    //    for (double pol_angle = 0.0; pol_angle <= (2 * M_PI + 0.01); pol_angle += M_PI/10.0)
    //    {
    //        for (double pol_strength = 0.0; pol_strength <= 1.0001; pol_strength += 0.1)
    //        {
    //            for (double blend = 0.0; blend <= 1.0001; blend += 0.05)
    //            {
    //                armuellermatrix_d_init_rotator_m(
    //                    art_gv,
    //                    rot_angle,
    //                    mm1
    //                    );
    //                armuellermatrix_dd_init_linear_polariser_m(
    //                    art_gv,
    //                    pol_angle,
    //                    pol_strength,
    //                    mm2
    //                    );
    //                // debug
    //                //armuellermatrix_mii_debugprintf(art_gv, mm1, 0, 2);
    //                //armuellermatrix_mii_debugprintf(art_gv, mm2, 0, 2);
    //
    //                armuellermatrix_md_mul_m(art_gv, mm1, (1.0 - blend), mm1);
    //                armuellermatrix_md_mul_m(art_gv, mm2, blend, mm2);
    //                armuellermatrix_mm_add_m(art_gv, mm1, mm2, mm_blend);
    //
    //                // debug
    //                //armuellermatrix_mii_debugprintf(art_gv, mm_blend, 0, 2);
    //
    //                printf(
    //                    "rotator (%+.2f) and linear polarizer (%+.2f, %3.0f%%) blended (%.2f): ",
    //                    rot_angle, pol_angle, pol_strength * 100, blend
    //                    );
    //                armuellermatrix_m_valid(art_gv, mm_blend, NO, YES);
    //                printf("\n");
    //
    //                // debug
    //                //printf("\n");
    //            }
    //            // debug
    //            printf("\n");
    //        }
    //    }
    //    //printf("\n");
    //}
    //printf("\n");

    // Retarder blended with linear polarizer
    //for (double ret_angle = 0.0; ret_angle <= (2 * M_PI + 0.01); ret_angle += M_PI/10.0)
    //{
    //    for (double pol_angle = 0.0; pol_angle <= (2 * M_PI + 0.01); pol_angle += M_PI/10.0)
    //    {
    //        for (double pol_strength = 0.0; pol_strength <= 1.0001; pol_strength += 0.1)
    //        {
    //            for (double blend = 0.0; blend <= 1.0001; blend += 0.05)
    //            {
    //                armuellermatrix_d_init_phase_shifter_m(
    //                    art_gv,
    //                    ret_angle,
    //                    mm1
    //                    );
    //                armuellermatrix_dd_init_linear_polariser_m(
    //                    art_gv,
    //                    pol_angle,
    //                    pol_strength,
    //                    mm2
    //                    );
    //                // debug
    //                //armuellermatrix_mii_debugprintf(art_gv, mm1, 0, 2);
    //                //armuellermatrix_mii_debugprintf(art_gv, mm2, 0, 2);
    //
    //                armuellermatrix_md_mul_m(art_gv, mm1, (1.0 - blend), mm1);
    //                armuellermatrix_md_mul_m(art_gv, mm2, blend, mm2);
    //                armuellermatrix_mm_add_m(art_gv, mm1, mm2, mm_blend);
    //
    //                // debug
    //                //armuellermatrix_mii_debugprintf(art_gv, mm_blend, 0, 2);
    //
    //                printf(
    //                    "retarder (%+.2f) and linear polarizer (%+.2f, %3.0f%%) blended (%.2f): ",
    //                    ret_angle, pol_angle, pol_strength * 100, blend
    //                    );
    //                armuellermatrix_m_valid(art_gv, mm_blend, NO, YES);
    //                printf("\n");
    //
    //                // debug
    //                //printf("\n");
    //            }
    //            // debug
    //            printf("\n");
    //        }
    //    }
    //    //printf("\n");
    //}
    //printf("\n");

    // Retarder blended with rotator
    //for (double ret_angle = 0.0; ret_angle <= (2 * M_PI + 0.01); ret_angle += M_PI/22.34)
    //{
    //    for (double rot_angle = 0.0; rot_angle <= (2 * M_PI + 0.01); rot_angle += M_PI/22.34)
    //    {
    //        for (double blend = 0.0; blend <= 1.0001; blend += 0.05)
    //        {
    //            armuellermatrix_d_init_phase_shifter_m(
    //                art_gv,
    //                ret_angle,
    //                mm1
    //                );
    //            armuellermatrix_d_init_rotator_m(
    //                art_gv,
    //                rot_angle,
    //                mm2
    //                );
    //            // debug
    //            //armuellermatrix_mii_debugprintf(art_gv, mm1, 0, 2);
    //            //armuellermatrix_mii_debugprintf(art_gv, mm2, 0, 2);
    //
    //            armuellermatrix_md_mul_m(art_gv, mm1, (1.0 - blend), mm1);
    //            armuellermatrix_md_mul_m(art_gv, mm2, blend, mm2);
    //            armuellermatrix_mm_add_m(art_gv, mm1, mm2, mm_blend);
    //
    //            // debug
    //            //armuellermatrix_mii_debugprintf(art_gv, mm_blend, 0, 2);
    //
    //            printf(
    //                "retarder (%+.2f) and rotator (%+.2f) blended (%.2f): ",
    //                ret_angle, rot_angle, blend
    //                );
    //            armuellermatrix_m_valid(art_gv, mm_blend, NO, YES);
    //            printf("\n");
    //
    //            // debug
    //            //printf("\n");
    //        }
    //        printf("\n");
    //    }
    //}
    //printf("\n");

    // Depolarizer blended with linear polarizer
    //for (double attenuation = -0.1; attenuation <= 1.10001; attenuation += 0.1)
    //{
    //    for (double depolarization = -0.1; depolarization <= attenuation + 0.0001; depolarization += 0.1)
    //    {
    //        for (double pol_angle = 0.0; pol_angle <= (2 * M_PI + 0.01); pol_angle += M_PI/10.0)
    //        {
    //            for (double pol_strength = 0.0; pol_strength <= 1.0001; pol_strength += 1.0/4)
    //            {
    //                for (double blend = 0.0; blend <= 1.0001; blend += 1.0/5)
    //                {
    //                    armuellermatrix_dd_init_depolarising_m(
    //                        art_gv,
    //                        attenuation,
    //                        depolarization,
    //                        mm1
    //                        );
    //                    armuellermatrix_dd_init_linear_polariser_m(
    //                        art_gv,
    //                        pol_angle,
    //                        pol_strength,
    //                        mm2
    //                        );
    //                    // debug
    //                    //armuellermatrix_mii_debugprintf(art_gv, mm1, 0, 2);
    //                    //armuellermatrix_mii_debugprintf(art_gv, mm2, 0, 2);
    //
    //                    armuellermatrix_md_mul_m(art_gv, mm1, (1.0 - blend), mm1);
    //                    armuellermatrix_md_mul_m(art_gv, mm2, blend, mm2);
    //                    armuellermatrix_mm_add_m(art_gv, mm1, mm2, mm_blend);
    //
    //                    // debug
    //                    //armuellermatrix_mii_debugprintf(art_gv, mm_blend, 0, 2);
    //
    //                    printf(
    //                        "isotropic depolarizer (%+.3f, %+.3f) and linear polarizer (%+.2f, %3.0f%%)"
    //                        " blended (%.2f): ",
    //                        attenuation, depolarization, pol_angle, pol_strength * 100, blend
    //                        );
    //                    armuellermatrix_m_valid(art_gv, mm_blend, NO, YES);
    //                    printf("\n");
    //
    //                    // debug
    //                    //printf("\n");
    //                }
    //                printf("\n");
    //            }
    //        }
    //    }
    //}
    //printf("\n");

    // Depolarizer blended with rotator
    //for (double attenuation = -0.1; attenuation <= 1.10001; attenuation += 1.0/10)
    //{
    //    for (double depolarization = -0.1; depolarization <= attenuation + 0.0001; depolarization += 1.0/10)
    //    {
    //        for (double angle = -M_PI_2; angle <= (2 * M_PI + 0.01); angle += M_PI/10.0)
    //        {
    //            for (double blend = 0.0; blend <= 1.0001; blend += 1.0/5)
    //            {
    //                armuellermatrix_dd_init_depolarising_m(
    //                    art_gv,
    //                    attenuation,
    //                    depolarization,
    //                    mm1
    //                    );
    //                armuellermatrix_d_init_rotator_m(
    //                    art_gv,
    //                    angle,
    //                    mm2
    //                    );
    //                // debug
    //                //armuellermatrix_mii_debugprintf(art_gv, mm1, 0, 2);
    //                //armuellermatrix_mii_debugprintf(art_gv, mm2, 0, 2);
    //
    //                armuellermatrix_md_mul_m(art_gv, mm1, (1.0 - blend), mm1);
    //                armuellermatrix_md_mul_m(art_gv, mm2, blend, mm2);
    //                armuellermatrix_mm_add_m(art_gv, mm1, mm2, mm_blend);
    //
    //                // debug
    //                //armuellermatrix_mii_debugprintf(art_gv, mm_blend, 0, 2);
    //
    //                printf(
    //                    "isotropic depolarizer (%+.3f, %+.3f) and rotator (%+.2f)"
    //                    " blended (%.2f): ",
    //                    attenuation, depolarization, angle, blend
    //                    );
    //                armuellermatrix_m_valid(art_gv, mm_blend, NO, YES);
    //                printf("\n");
    //
    //                // debug
    //                //printf("\n");
    //            }
    //            printf("\n");
    //        }
    //    }
    //}
    //printf("\n");

    // Depolarizer blended with retarder
    //for (double attenuation = -0.1; attenuation <= 1.10001; attenuation += 1.0/10)
    //{
    //    for (double depolarization = -0.1; depolarization <= attenuation + 0.0001; depolarization += 1.0/10)
    //    {
    //        for (double angle = -M_PI_2; angle <= (2 * M_PI + 0.01); angle += M_PI/10.0)
    //        {
    //            for (double blend = 0.0; blend <= 1.0001; blend += 1.0/5)
    //            {
    //                armuellermatrix_dd_init_depolarising_m(
    //                    art_gv,
    //                    attenuation,
    //                    depolarization,
    //                    mm1
    //                    );
    //                armuellermatrix_d_init_phase_shifter_m(
    //                    art_gv,
    //                    angle,
    //                    mm2
    //                    );
    //                // debug
    //                //armuellermatrix_mii_debugprintf(art_gv, mm1, 0, 2);
    //                //armuellermatrix_mii_debugprintf(art_gv, mm2, 0, 2);
    //
    //                armuellermatrix_md_mul_m(art_gv, mm1, (1.0 - blend), mm1);
    //                armuellermatrix_md_mul_m(art_gv, mm2, blend, mm2);
    //                armuellermatrix_mm_add_m(art_gv, mm1, mm2, mm_blend);
    //
    //                // debug
    //                //armuellermatrix_mii_debugprintf(art_gv, mm_blend, 0, 2);
    //
    //                printf(
    //                    "isotropic depolarizer (%+.3f, %+.3f) and retarder (%+.2f)"
    //                    " blended (%.2f): ",
    //                    attenuation, depolarization, angle, blend
    //                    );
    //                armuellermatrix_m_valid(art_gv, mm_blend, NO, YES);
    //                printf("\n");
    //
    //                // debug
    //                //printf("\n");
    //            }
    //            printf("\n");
    //        }
    //    }
    //}
    //printf("\n");

    armuellermatrix_free(art_gv, mm1);
    armuellermatrix_free(art_gv, mm2);
    armuellermatrix_free(art_gv, mm_blend);
}

typedef enum TestMuellerMatrixType {
    tmmt_first,

    tmmt_zero = tmmt_first,
    tmmt_identity,
    tmmt_depolariser,
    tmmt_non_polarising,
    tmmt_linear_polariser_minus_45,
    tmmt_linear_polariser_0,
    tmmt_linear_polariser_plus_45,
    tmmt_linear_polariser_plus_90,
//    tmmt_circular_filter_plus,
//    tmmt_circular_filter_minus,

    tmmt_count
} TestMuellerMatrixType;

typedef enum TestStokesVectorType {
    tsvt_first,

    tsvt_zero = tsvt_first,
    tsvt_depolarised,
    tsvt_linpol_horizontal,
    tsvt_linpol_vertical,
    tsvt_linpol_diagonal_positive,
    tsvt_linpol_diagonal_negative,
    tsvt_circular_positive,
    tsvt_circular_negative,

    tsvt_count
} TestStokesVectorType;

typedef struct TestingContext
{
    ArMuellerMatrix       * matrix_orig;
    TestMuellerMatrixType   matrix_orig_type;
    ArMuellerMatrix       * matrix_rotated;

    ArStokesVector        * vector_orig;
    TestStokesVectorType    vector_orig_type;
    ArStokesVector        * vector_result;

    double                  input_rotation;
    double                  output_rotation;

    const double            non_polarising_attenuation;

    unsigned int            passed_tests_count;
    unsigned int            failed_tests_count;
    unsigned int            skipped_tests_count;
} TestingContext;

    // Debug print

    //armuellermatrix_mi_debugprintf( art_gv, context.matrix_rotated, 3 );
    //printf( "Result: " );
    //arstokesvector_sv_debugprintf( art_gv, context.vector_result );

#define COMPARE_TO_REFERENCE(_init_type, _c0, _c1, _c2, _c3, _msg_pass_opt, _msg_fail) \
{ \
    ArStokesVector  * reference_sv = arstokesvector_alloc( art_gv ); \
    \
    arstokesvector_ ## _init_type ## _init_sv( \
        art_gv, \
        _c0, \
        _c1, \
        _c2, \
        _c3, \
        reference_sv \
        ); \
     \
    if (arstokesvector_sv_sv_equal(art_gv, reference_sv, context->vector_result)) \
    { \
        context->passed_tests_count++; \
        printf( "%s", _msg_pass_opt ); \
    } \
    else \
    { \
        context->failed_tests_count++; \
        printf( ART_STRING_BOLD "FAILED: %s!" ART_STRING_NORMAL "\n", _msg_fail ); \
        \
        printf( "\n===============================================================================================================================\n" ); \
        printf(   "===============================================================================================================================\n" ); \
        printf( "Original Stokes vector: " ); \
        arstokesvector_sv_debugprintf( art_gv, context->vector_orig ); \
        printf( "Original Mueller matrix: " ); \
        armuellermatrix_mi_debugprintf( art_gv, context->matrix_orig, 3 ); \
        printf( \
            "Input rotation %.1f degrees, Output rotation %.1f degrees\n", \
            context->input_rotation * MATH_RAD_TO_DEG, \
            context->output_rotation * MATH_RAD_TO_DEG ); \
        printf( "Rotated Mueller matrix: " ); \
        armuellermatrix_mi_debugprintf( art_gv, context->matrix_rotated, 3 ); \
        printf( "Resulting Stokes vector: " ); \
        arstokesvector_sv_debugprintf( art_gv, context->vector_result ); \
        printf( "Reference Stokes vector: " ); \
        arstokesvector_sv_debugprintf( art_gv, reference_sv ); \
        printf( "===============================================================================================================================\n" ); \
        printf( "===============================================================================================================================\n\n" ); \
    } \
    \
    arstokesvector_free( art_gv, reference_sv ); \
}

#define COMPARE_TO_REFERENCE_CCCC(_c0, _c1, _c2, _c3, _msg_pass_opt, _msg_fail) \
    COMPARE_TO_REFERENCE(cccc, _c0, _c1, _c2, _c3, _msg_pass_opt, _msg_fail)

#define COMPARE_TO_REFERENCE_DDDD(_d0, _d1, _d2, _d3, _msg_pass_opt, _msg_fail) \
    COMPARE_TO_REFERENCE(dddd, _d0, _d1, _d2, _d3, _msg_pass_opt, _msg_fail)

void armuellermatrix_test_MM_rotation_operation(
        const ART_GV                  * art_gv,
              TestingContext          * context
        )
{
    ArSpectrum * zero_colour = spc_d_alloc_init( art_gv, 0.0 );

    if ((context->matrix_orig_type == tmmt_zero) ||
        (context->vector_orig_type == tsvt_zero))
    {
        //  For zero input Stokes vector or zero Mueller matrix, the result
        //  must be the zero Stokes vector.

        COMPARE_TO_REFERENCE_DDDD(
            0.0,
            0.0,
            0.0,
            0.0,
            "", //"Passed (zero MM or zero SV)\n",//
            "Zero MM or zero input SV must produce zero output SV"
            );
    }
    else if (context->matrix_orig_type == tmmt_depolariser)
    {
        //  Rotation of depolarised light yields a depolarised light with
        //  the same intensity

        COMPARE_TO_REFERENCE_CCCC(
            ARSV_I( *context->vector_orig, 0 ),
            zero_colour,
            zero_colour,
            zero_colour,
            "", //"Passed (depolariser)\n",//
            "Rotated depolarizer should produce depolarised SV with intensity of the output SV"
            );
    }
    else if (context->matrix_orig_type == tmmt_identity)
    {
        //  Identity: the result is only modified by the rotation(s).
        //  In this case it is sufficient to check the values according to
        //  the accumulated rotation since the MM itself doesn't modify the light at all.

        const double accumulated_rotation =
            context->input_rotation + context->output_rotation;

        if (   (context->input_rotation == 0.0)
            && (context->output_rotation == 0.0))
        {
            COMPARE_TO_REFERENCE_CCCC(
                ARSV_I( *context->vector_orig, 0 ),
                ARSV_I( *context->vector_orig, 1 ),
                ARSV_I( *context->vector_orig, 2 ),
                ARSV_I( *context->vector_orig, 3 ),
                "", //"Passed (identity, no rotation)\n", //
                "Unrotated identity must not change the input SV"
                );
        }
        else if (context->vector_orig_type == tsvt_depolarised)
        {
            COMPARE_TO_REFERENCE_CCCC(
                ARSV_I( *context->vector_orig, 0 ),
                ARSV_I( *context->vector_orig, 1 ),
                ARSV_I( *context->vector_orig, 2 ),
                ARSV_I( *context->vector_orig, 3 ),
                "", //"Passed (depolarised, identity)\n", //
                "Neither identity nor rotation can change de-polarized input SV"
                );
        }
        else if (   (context->vector_orig_type == tsvt_circular_positive)
                 || (context->vector_orig_type == tsvt_circular_negative))
        {
            COMPARE_TO_REFERENCE_CCCC(
                ARSV_I( *context->vector_orig, 0 ),
                ARSV_I( *context->vector_orig, 1 ),
                ARSV_I( *context->vector_orig, 2 ),
                ARSV_I( *context->vector_orig, 3 ),
                "", //"Passed (circular, identity)\n", //
                "Neither identity nor rotation can change circularly polarised input SV"
                );
        }
        else if (context->vector_orig_type == tsvt_linpol_horizontal)
        {
            if (m_dd_equal(accumulated_rotation, 45.0 DEGREES))
            {
                COMPARE_TO_REFERENCE_DDDD(
                     1.0,
                     0.0,
                     1.0,
                     0.0,
                    "", //"Passed (horizontal lin pol, identity, +45)\n",//
                    "Incorrectly rotated linearly polarized light"
                    );
            }
            else if (m_dd_equal(accumulated_rotation, 90.0 DEGREES))
            {
                COMPARE_TO_REFERENCE_DDDD(
                     1.0,
                    -1.0,
                     0.0,
                     0.0,
                    "", //"Passed (horizontal lin pol, identity, +90)\n",//
                    "Incorrectly rotated linearly polarized light"
                    );
            }
            else if (m_dd_equal(accumulated_rotation, 135.0 DEGREES))
            {
                COMPARE_TO_REFERENCE_DDDD(
                     1.0,
                     0.0,
                    -1.0,
                     0.0,
                    "", //"Passed (horizontal lin pol, identity, +135)\n",//
                    "Incorrectly rotated linearly polarized light"
                    );
            }
            else if (m_dd_equal(accumulated_rotation, 180.0 DEGREES))
            {
                COMPARE_TO_REFERENCE_DDDD(
                     1.0,
                     1.0,
                     0.0,
                     0.0,
                    "", //"Passed (horizontal lin pol, identity, +180)\n",//
                    "Incorrectly rotated linearly polarized light"
                    );
            }
            else
                ART_ERRORHANDLING_FATAL_ERROR("unexpected case");
        }
        else if (context->vector_orig_type == tsvt_linpol_vertical)
        {
            if (m_dd_equal(accumulated_rotation, 45.0 DEGREES))
            {
                COMPARE_TO_REFERENCE_DDDD(
                     1.0,
                     0.0,
                    -1.0,
                     0.0,
                    "", //"Passed (vertical lin pol, identity, +45)\n",//
                    "Incorrectly rotated (vertical lin pol, identity, +45)"
                    );
            }
            else if (m_dd_equal(accumulated_rotation, 90.0 DEGREES))
            {
                COMPARE_TO_REFERENCE_DDDD(
                     1.0,
                     1.0,
                     0.0,
                     0.0,
                    "", //"Passed (vertical lin pol, identity, +90)\n",//
                    "Incorrectly rotated (vertical lin pol, identity, +90)"
                    );
            }
            else if (m_dd_equal(accumulated_rotation, 135.0 DEGREES))
            {
                COMPARE_TO_REFERENCE_DDDD(
                     1.0,
                     0.0,
                     1.0,
                     0.0,
                    "", //"Passed (vertical lin pol, identity, +135)\n",//
                    "Incorrectly rotated (vertical lin pol, identity, +135)"
                    );
            }
            else if (m_dd_equal(accumulated_rotation, 180.0 DEGREES))
            {
                COMPARE_TO_REFERENCE_DDDD(
                     1.0,
                    -1.0,
                     0.0,
                     0.0,
                    "", //"Passed (vertical lin pol, identity, +180)\n",//
                    "Incorrectly rotated (vertical lin pol, identity, +180)"
                    );
            }
            else
                ART_ERRORHANDLING_FATAL_ERROR("unexpected case");
        }
        else if (context->vector_orig_type == tsvt_linpol_diagonal_positive)
        {
            if (m_dd_equal(accumulated_rotation, 45.0 DEGREES))
            {
                COMPARE_TO_REFERENCE_DDDD(
                     1.0,
                    -1.0,
                     0.0,
                     0.0,
                    "", //"Passed (positive diagonal lin pol, identity, +45)\n",//
                    "Incorrectly rotated  (positive diagonal lin pol, identity, +45)"
                    );
            }
            else if (m_dd_equal(accumulated_rotation, 90.0 DEGREES))
            {
                COMPARE_TO_REFERENCE_DDDD(
                     1.0,
                     0.0,
                    -1.0,
                     0.0,
                    "", //"Passed (positive diagonal lin pol, identity, +90)\n",//
                    "Incorrectly rotated (positive diagonal lin pol, identity, +90)"
                    );
            }
            else if (m_dd_equal(accumulated_rotation, 135.0 DEGREES))
            {
                COMPARE_TO_REFERENCE_DDDD(
                     1.0,
                     1.0,
                     0.0,
                     0.0,
                    "", //"Passed (positive diagonal lin pol, identity, +135.0)\n",//
                    "Incorrectly rotated (positive diagonal lin pol, identity, +135.0)"
                    );
            }
            else if (m_dd_equal(accumulated_rotation, 180.0 DEGREES))
            {
                COMPARE_TO_REFERENCE_DDDD(
                     1.0,
                     0.0,
                     1.0,
                     0.0,
                    "", //"Passed (positive diagonal lin pol, identity, +180.0)\n",//
                    "Incorrectly rotated (positive diagonal lin pol, identity, +180.0)"
                    );
            }
            else
                ART_ERRORHANDLING_FATAL_ERROR("unexpected case");
        }
        else if (context->vector_orig_type == tsvt_linpol_diagonal_negative)
        {
            if (m_dd_equal(accumulated_rotation, 45.0 DEGREES))
            {
                COMPARE_TO_REFERENCE_DDDD(
                     1.0,
                     1.0,
                     0.0,
                     0.0,
                    "", //"Passed (negative diagonal lin pol, identity, +45)\n",//
                    "Incorrectly rotated (negative diagonal lin pol, identity, +45)"
                    );
            }
            else if (m_dd_equal(accumulated_rotation, 90.0 DEGREES))
            {
                COMPARE_TO_REFERENCE_DDDD(
                     1.0,
                     0.0,
                     1.0,
                     0.0,
                    "", //"Passed (negative diagonal lin pol, identity, +90)\n",//
                    "Incorrectly rotated (negative diagonal lin pol, identity, +90)"
                    );
            }
            else if (m_dd_equal(accumulated_rotation, 135.0 DEGREES))
            {
                COMPARE_TO_REFERENCE_DDDD(
                     1.0,
                    -1.0,
                     0.0,
                     0.0,
                    "", //"Passed (negative diagonal lin pol, identity, +135)\n",//
                    "Incorrectly rotated (negative diagonal lin pol, identity, +135)"
                    );
            }
            else if (m_dd_equal(accumulated_rotation, 180.0 DEGREES))
            {
                COMPARE_TO_REFERENCE_DDDD(
                     1.0,
                     0.0,
                    -1.0,
                     0.0,
                    "", //"Passed (negative diagonal lin pol, identity, +180.0)\n",//
                    "Incorrectly rotated (negative diagonal lin pol, identity, +180.0)"
                    );
            }
            else
                ART_ERRORHANDLING_FATAL_ERROR("unexpected case");
        }
        else
            ART_ERRORHANDLING_FATAL_ERROR("unexpected case");
    }
    else if (context->matrix_orig_type == tmmt_linear_polariser_0)
    {
        // Horizontal linear polariser

        if (    (    (context->vector_orig_type == tsvt_linpol_horizontal)
                  && (context->input_rotation == 90.0 DEGREES))
             || (    (context->vector_orig_type == tsvt_linpol_diagonal_positive)
                  && (context->input_rotation == 45.0 DEGREES))
             || (    (context->vector_orig_type == tsvt_linpol_vertical)
                  && (context->input_rotation ==  0.0 DEGREES))
           )
        {
            //  If the input vector is linearly polarised and is rotated so that
            //  it is vertical then it is blocker completely by the horizontal
            //  linear polariser.

            COMPARE_TO_REFERENCE_DDDD(
                 0.0,
                 0.0,
                 0.0,
                 0.0,
                "", //"Passed (vertical lin pol, horizontal polariser)\n",//
                "Vertically linearly polarised light must not pass horizontal linear polariser"
                );
        }
        else if (    (    (context->vector_orig_type == tsvt_linpol_horizontal)
                       && (context->input_rotation ==  0.0 DEGREES))
                  || (    (context->vector_orig_type == tsvt_linpol_vertical)
                       && (context->input_rotation == 90.0 DEGREES))
                  || (    (context->vector_orig_type == tsvt_linpol_diagonal_negative)
                       && (context->input_rotation == 45.0 DEGREES))
                )
        {
            //  If the input vector is linearly polarised and is rotated so that
            //  it is horizontal then it passes the horizontal linear
            //  polariser without change and is only modified by the output
            //  rotation.

            if (context->output_rotation == 0.0 DEGREES)
            {
                COMPARE_TO_REFERENCE_DDDD(
                     1.0,
                     1.0,
                     0.0,
                     0.0,
                    "", //"Passed (horizontal lin pol, output rot 0)\n",//
                    "Incorrectly rotated (negative diagonal lin pol, identity, +45)"
                    );
            }
            else if (context->output_rotation == 45.0 DEGREES)
            {
                COMPARE_TO_REFERENCE_DDDD(
                     1.0,
                     0.0,
                     1.0,
                     0.0,
                    "", //"Passed (horizontal lin pol, output rot 45)\n",//
                    "Incorrectly rotated (negative diagonal lin pol, identity, +90)"
                    );
            }
            else if (context->output_rotation == 90.0 DEGREES)
            {
                COMPARE_TO_REFERENCE_DDDD(
                     1.0,
                    -1.0,
                     0.0,
                     0.0,
                    "", //"Passed (horizontal lin pol, output rot 90)\n",//
                    "Incorrectly rotated (negative diagonal lin pol, identity, +135)"
                    );
            }
            else
                ART_ERRORHANDLING_FATAL_ERROR("unexpected case");
        }
        else
        {
            context->skipped_tests_count++;
            //printf( "temporarily skipping...\n" );
        }
    }
    else
    {
        context->skipped_tests_count++;
        //printf( "temporarily skipping...\n" );
    }

    spc_free( art_gv, zero_colour );
}


void armuellermatrix_test_MM_rotation_operations(
        const ART_GV           * art_gv
        )
{
    // Prepare data

    TestingContext context =
    {
        armuellermatrix_alloc( art_gv ),
        tmmt_first,
        armuellermatrix_alloc( art_gv ),

        arstokesvector_alloc( art_gv ),
        tsvt_first,
        arstokesvector_alloc( art_gv ),

        0.0 DEGREES, 0.0 DEGREES,
        0.1,
        0, 0, 0
    };

    // Create a synthetic MM

    for (context.matrix_orig_type = tmmt_first;
         context.matrix_orig_type < tmmt_count;
         context.matrix_orig_type++)
    {
        switch (context.matrix_orig_type)
        {
            case tmmt_zero:
                armuellermatrix_d_init_nonpolarising_m(
                    art_gv,
                    0.0,
                    context.matrix_orig
                    );
                break;

            case tmmt_identity:
                armuellermatrix_d_init_nonpolarising_m(
                    art_gv,
                    1.0,
                    context.matrix_orig
                    );
                break;

            case tmmt_depolariser:
                armuellermatrix_d_init_depolarising_m(
                    art_gv,
                    1.0,
                    context.matrix_orig
                    );
                break;

            case tmmt_non_polarising:
                armuellermatrix_d_init_nonpolarising_m(
                    art_gv,
                    context.non_polarising_attenuation,
                    context.matrix_orig
                    );
                break;

            case tmmt_linear_polariser_minus_45:
                armuellermatrix_dd_init_linear_polariser_m(
                    art_gv,
                    -45.0 DEGREES,
                    1.0,
                    context.matrix_orig
                    );
                break;

            case tmmt_linear_polariser_0:
                armuellermatrix_dd_init_linear_polariser_m(
                    art_gv,
                    0.0 DEGREES,
                    1.0,
                    context.matrix_orig
                    );
                break;

            case tmmt_linear_polariser_plus_45:
                armuellermatrix_dd_init_linear_polariser_m(
                    art_gv,
                    45.0 DEGREES,
                    1.0,
                    context.matrix_orig
                    );
                break;

            case tmmt_linear_polariser_plus_90:
                armuellermatrix_dd_init_linear_polariser_m(
                    art_gv,
                    90.0 DEGREES,
                    1.0,
                    context.matrix_orig
                    );
                break;

            default:
                ART_ERRORHANDLING_FATAL_ERROR("unexpected case");
        }

        for (context.vector_orig_type = tsvt_first;
             context.vector_orig_type < tsvt_count;
             context.vector_orig_type++)
        {
            // Create a synthetic SV

            switch (context.vector_orig_type)
            {
                case tsvt_zero:
                    arstokesvector_dddd_init_sv(
                        art_gv,
                        0.0,
                        0.0,
                        0.0,
                        0.0,
                        context.vector_orig
                        );
                    break;

                case tsvt_depolarised:
                    arstokesvector_dddd_init_sv(
                        art_gv,
                        1.0,
                        0.0,
                        0.0,
                        0.0,
                        context.vector_orig
                        );
                    break;

                case tsvt_linpol_horizontal:
                    arstokesvector_dddd_init_sv(
                        art_gv,
                        1.0,
                        1.0,
                        0.0,
                        0.0,
                        context.vector_orig
                        );
                    break;

                case tsvt_linpol_vertical:
                    arstokesvector_dddd_init_sv(
                        art_gv,
                         1.0,
                        -1.0,
                         0.0,
                         0.0,
                        context.vector_orig
                        );
                    break;

                case tsvt_linpol_diagonal_positive:
                    arstokesvector_dddd_init_sv(
                        art_gv,
                        1.0,
                        0.0,
                        1.0,
                        0.0,
                        context.vector_orig
                        );
                    break;

                case tsvt_linpol_diagonal_negative:
                    arstokesvector_dddd_init_sv(
                        art_gv,
                         1.0,
                         0.0,
                        -1.0,
                         0.0,
                        context.vector_orig
                        );
                    break;

                case tsvt_circular_positive:
                    arstokesvector_dddd_init_sv(
                        art_gv,
                        1.0,
                        0.0,
                        0.0,
                        1.0,
                        context.vector_orig
                        );
                    break;

                case tsvt_circular_negative:
                    arstokesvector_dddd_init_sv(
                        art_gv,
                         1.0,
                         0.0,
                         0.0,
                        -1.0,
                        context.vector_orig
                        );
                    break;

                default:
                    ART_ERRORHANDLING_FATAL_ERROR("unexpected case");
            }

            for (signed int input_rot_step = 0; input_rot_step <= 2; input_rot_step++)
            {
                // Rotate MM

                switch (input_rot_step)
                {
                    case 0:
                        context.input_rotation =   0.0 DEGREES;
                        break;
                    case 1:
                        context.input_rotation =  45.0 DEGREES;
                        break;
                    case 2:
                        context.input_rotation =  90.0 DEGREES;
                        break;
                    default:
                        ART_ERRORHANDLING_FATAL_ERROR("unexpected case");
                }

                for (signed int output_rot_step = 0; output_rot_step <= 2; output_rot_step++)
                {
                    switch (output_rot_step)
                    {
                        case 0:
                            context.output_rotation =   0.0 DEGREES;
                            break;
                        case 1:
                            context.output_rotation =  45.0 DEGREES;
                            break;
                        case 2:
                            context.output_rotation =  90.0 DEGREES;
                            break;
                        default:
                            ART_ERRORHANDLING_FATAL_ERROR("unexpected case");
                    }

                    for (unsigned int round = 0; round < 3; round++)
                    {
                        if (round == 0)
                        {
                            // Test the full rotation first

                            armuellermatrix_mdddd_rotate_input_and_output_m(
                                art_gv,
                                context.matrix_orig,
                                cos( context.input_rotation ),
                                sin( context.input_rotation ),
                                cos( context.output_rotation ),
                                sin( context.output_rotation ),
                                context.matrix_rotated
                                );
                        }
                        else if (round == 1)
                        {
                            //  If there is no output rotation, we also
                            //  explicitly check the input-only rotation
                            //  routine

                            if (context.output_rotation == (0.0 DEGREES))
                            {
                                armuellermatrix_mdd_rotate_input_m(
                                    art_gv,
                                    context.matrix_orig,
                                    cos( context.input_rotation ),
                                    sin( context.input_rotation ),
                                    context.matrix_rotated
                                    );
                            }
                            else
                                continue;
                        }
                        else if (round == 2)
                        {
                            //  If there is no input rotation, we also
                            //  explicitly check the output-only rotation
                            //  routine

                            if (context.input_rotation == (0.0 DEGREES))
                            {
                                armuellermatrix_mdd_rotate_output_m(
                                    art_gv,
                                    context.matrix_orig,
                                    cos( context.output_rotation ),
                                    sin( context.output_rotation ),
                                    context.matrix_rotated
                                    );
                            }
                            else
                                continue;
                        }
                        else
                            ART_ERRORHANDLING_FATAL_ERROR("unexpected case");

                        // Transform SV with rotated MM

                        arstokesvector_sv_mm_mul_sv(
                            art_gv,
                            context.vector_orig,
                            context.matrix_rotated,
                            context.vector_result
                            );

                        // Check the result

                        armuellermatrix_test_MM_rotation_operation(
                              art_gv,
                            & context
                            );
                    }
                }
            }
        }
    }

    printf(
        ART_STRING_BOLD
        "\n" "Passed: %d, Failed: %d, Skipped: %d, Total: %d" "\n"
        ART_STRING_NORMAL,
        context.passed_tests_count,
        context.failed_tests_count,
        context.skipped_tests_count,
        context.passed_tests_count + context.failed_tests_count + context.skipped_tests_count
        );


    // Clean-up

    armuellermatrix_free( art_gv, context.matrix_orig );
    armuellermatrix_free( art_gv, context.matrix_rotated );

    arstokesvector_free( art_gv, context.vector_orig );
    arstokesvector_free( art_gv, context.vector_result );
}

// ===========================================================================
