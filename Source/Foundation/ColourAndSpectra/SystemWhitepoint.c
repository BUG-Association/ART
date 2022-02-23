
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

#define ART_MODULE_NAME     SystemWhitepoint

#include "SystemWhitepoint.h"
#include <stdio.h>
#include <pthread.h>
#include "ArSpectrum500.h"
#include "ColourAndSpectralDataConversion.h"

typedef struct SystemWhitepoint_GV
{
    pthread_mutex_t  mutex;

    ArCIExy          syswhite_xy;
    ArCIEXYZ         syswhite_xyz;
    ArSymbol         syswhite_symbol;
    int              manual_syswhite;
    
    Mat3             ma;
    Mat3             ma_inv;
}
SystemWhitepoint_GV;

#define ARSWP_GV                art_gv->systemwhitepoint_gv
#define ARSWP_MUTEX             ARSWP_GV->mutex
#define ARSWP_SYSWHITE_XY       ARSWP_GV->syswhite_xy
#define ARSWP_SYSWHITE_XYZ      ARSWP_GV->syswhite_xyz
#define ARSWP_SYSWHITE_SYMBOL   ARSWP_GV->syswhite_symbol
#define ARSWP_MANUAL_SYSWHITE   ARSWP_GV->manual_syswhite
#define ARSWP_MA                ARSWP_GV->ma
#define ARSWP_MA_INV            ARSWP_GV->ma_inv

ART_MODULE_INITIALISATION_FUNCTION
(
    ARSWP_GV = ALLOC(SystemWhitepoint_GV);

    pthread_mutex_init( & ARSWP_MUTEX, NULL );

    //  set to -1, the first invocation immediately afterwards
    //  sets it to 0, all subsequent ones increase it

    ARSWP_MANUAL_SYSWHITE = -1;
    ARSWP_SYSWHITE_SYMBOL = NULL;
    art_set_system_white_point_by_desc(art_gv, "D50");

    ARSWP_MA = C3_M_UNIT;

    c3_transpose_m(&ARSWP_MA);

    double  det = c3_m_det( & ARSWP_MA );

    c3_md_invert_m( & ARSWP_MA, det, & ARSWP_MA_INV );
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    pthread_mutex_destroy( & ARSWP_MUTEX );

    FREE( ARSWP_GV );
)

Mat3 art_xyz_scaling_ma()
{
    return  MAT3_UNIT;
}

Mat3 art_von_kries_ma()
{
    return
        MAT3(
             0.4002400,  0.7076000, -0.0808100,
            -0.2263000,  1.1653200,  0.0457000,
             0.0000000,  0.0000000,  0.9182200
             );
}

Mat3 art_bradford_ma()
{
    return
        MAT3(
             0.8951000,  0.2664000, -0.1614000,
            -0.7502000,  1.7135000,  0.0367000,
             0.0389000, -0.0685000,  1.0296000
             );
}

Mat3 art_cat02_suesstrunk_ma()
{
    return
        MAT3(
             0.7328, 0.4296, -0.1624,
            -0.7036, 1.6975,  0.0061,
             0.0000, 0.0000,  1.0000
             );
}

Mat3 art_chromatic_adaptation_matrix(
              ART_GV                      * art_gv,
        const ArChromaticAdaptationSpace    cas,
        const ArCIExy                     * source_white,
        const ArCIExy                     * target_white
        )
{
    ArCIExyY  source_xyy = ARCIExyY_xy1(*source_white);
    ArCIExyY  target_xyy = ARCIExyY_xy1(*target_white);

    ArCIEXYZ  source_xyz, target_xyz;

    xyy_to_xyz( art_gv, & target_xyy, & target_xyz );
    xyy_to_xyz( art_gv, & source_xyy, & source_xyz );

    ArCIEXYZ source_crs, target_crs;
    
    Mat3  ma;
    
    switch (cas)
    {
        case arcas_von_kries:
            ma = art_von_kries_ma();
            break;

        case arcas_bradford:
            ma = art_bradford_ma();
            break;

        case arcas_cat_02:
            ma = art_cat02_suesstrunk_ma();
            break;

        default:
            ma = art_xyz_scaling_ma();
            break;
    }
    
    xyz_mat_to_xyz( art_gv, & source_xyz, & ma, & source_crs );
    xyz_mat_to_xyz( art_gv, & target_xyz, & ma, & target_crs );

    double  xf = 1.; double  yf = 1.; double  zf = 1.;
    
    if ( ARCIEXYZ_X(source_crs) > 0. )
        xf = ARCIEXYZ_X(target_crs) / ARCIEXYZ_X(source_crs);
    if ( ARCIEXYZ_Y(source_crs) > 0. )
        yf = ARCIEXYZ_Y(target_crs) / ARCIEXYZ_Y(source_crs);
    if ( ARCIEXYZ_Z(source_crs) > 0. )
        zf = ARCIEXYZ_Z(target_crs) / ARCIEXYZ_Z(source_crs);

    Mat3  scale =
        MAT3(
             xf, 0., 0.,
             0., yf, 0.,
             0., 0., zf
             );
    
    c3_transpose_m( & ma );

    double  det = c3_m_det( & ma );

    Mat3  ma_inv;

    c3_md_invert_m( & ma, det, & ma_inv );

    Mat3  tmp;
    
    c3_mm_mul_m( & scale, & ma_inv, & tmp );
    
    Mat3  cam;

    c3_mm_mul_m( & ma, & tmp, & cam );
    
    c3_transpose_m( & cam );
    
    return cam;
}


//   This thing lives elsewhere in ART, and in lieu of including that
//   header (which would complicate things), we put the prototype here
//   instead.

void blackbody_d_s500(
        const ART_GV         * art_gv,
        const double           temperature,
              ArSpectrum500  * spectrum500
        );

void art_set_system_white_point_by_desc(
              ART_GV  * art_gv,
        const char    * wp_desc
        )
{
    pthread_mutex_lock( & ARSWP_MUTEX );

    int     valid_wp_desc = 0;
    char  * my_wp_desc = 0;

    if (    strlen(wp_desc) == 1
         && (wp_desc[0] == 'E' || wp_desc[0] == 'e') )
    {
        //   Option 1: Illuminant E. Not that this makes much sense from
        //             a practical viewpoint, but let's include it anyway.
    
        ARSWP_SYSWHITE_XY  = ARCIEXY_E;
        ARSWP_SYSWHITE_XYZ = ARCIEXYZ_E;
        my_wp_desc = "E";
        valid_wp_desc = 1;
    }
    
    if (    strlen(wp_desc) == 1
         && (wp_desc[0] == 'A' || wp_desc[0] == 'a') )
    {
        //   Option 2: Illuminant A. Extremely red, probably not very
        //             useful, either.
    
        ARSWP_SYSWHITE_XY  = ARCIEXY_A;
        ARSWP_SYSWHITE_XYZ = ARCIEXYZ_A;
        my_wp_desc = "A";
        valid_wp_desc = 1;
    }
    
    if (    strlen(wp_desc) == 3
         && (wp_desc[0] == 'D' || wp_desc[0] == 'd') )
    {
        //   Option 3: the user wrote something which starts with "D"
        //             We interpret this to mean one of the CIE Dxx
        //             illuminants: 50, 55, 65, 75 are on offer.
    
        if ( wp_desc[1] == '5' && wp_desc[2] == '0' )
        {
            ARSWP_SYSWHITE_XY  = ARCIEXY_D50;
            ARSWP_SYSWHITE_XYZ = ARCIEXYZ_D50;
            my_wp_desc = "D50";
            valid_wp_desc = 1;
        }

        if ( wp_desc[1] == '5' && wp_desc[2] == '5' )
        {
            ARSWP_SYSWHITE_XY  = ARCIEXY_D55;
            ARSWP_SYSWHITE_XYZ = ARCIEXYZ_D55;
            my_wp_desc = "D55";
            valid_wp_desc = 1;
        }

        if ( wp_desc[1] == '6' && wp_desc[2] == '5' )
        {
            ARSWP_SYSWHITE_XY  = ARCIEXY_D65;
            ARSWP_SYSWHITE_XYZ = ARCIEXYZ_D65;
            my_wp_desc = "D65";
            valid_wp_desc = 1;
        }

        if ( wp_desc[1] == '7' && wp_desc[2] == '5' )
        {
            ARSWP_SYSWHITE_XY  = ARCIEXY_D75;
            ARSWP_SYSWHITE_XYZ = ARCIEXYZ_D75;
            my_wp_desc = "D75";
            valid_wp_desc = 1;
        }
    }

    if (   strlen(wp_desc) == 2
        || strlen(wp_desc) == 4 )
    {
        //  Option 4: a two or four digit CCT, with 3k < cct < 9k
        
        int  cct = atoi(wp_desc);
        
        if ( strlen(wp_desc) == 2 )
            cct *= 100;
        
        if ( cct > 3000 && cct < 9900 )
        {
            ArSpectrum500  bb500;
            
            blackbody_d_s500(
                  art_gv,
                  (double) cct,
                & bb500
                );
            
            ArCIEXYZ  bbxyz;
            
            s500_to_xyz( art_gv, & bb500, & bbxyz );

            ArCIExyY  bbxyy;

            xyz_to_xyy( art_gv, & bbxyz, & bbxyy );
            
            ARSWP_SYSWHITE_XY = ARCIExy(ARCIExyY_x(bbxyy),ARCIExyY_y(bbxyy));

            ARCIExyY_Y(bbxyy) = 1.0;

            xyy_to_xyz( art_gv, & bbxyy, & ARSWP_SYSWHITE_XYZ );

            asprintf(
                & my_wp_desc,
                  "%d K BB",
                  cct
                );

            valid_wp_desc = 1;
        }
    }

    pthread_mutex_unlock( & ARSWP_MUTEX );

    if ( valid_wp_desc )
    {
        ARSWP_MANUAL_SYSWHITE += 1;
        ARSWP_SYSWHITE_SYMBOL = arsymbol(art_gv, my_wp_desc);
    }
    else
    {
        ART_ERRORHANDLING_FATAL_ERROR(
            "invalid white point specification '%s'",
            wp_desc
            );
    }
}

void art_set_system_white_point(
              ART_GV   * art_gv,
        const char     * wp_desc,
        const ArCIExy  * wp
        )
{
    pthread_mutex_lock( & ARSWP_MUTEX );

    ARSWP_MANUAL_SYSWHITE += 1;
    ARSWP_SYSWHITE_SYMBOL = arsymbol(art_gv, wp_desc);
    
    ARSWP_SYSWHITE_XY = *wp;
    
    ArCIExyY  new_syswhite;
    
    ARCIExyY_x(new_syswhite) = XC(*wp);
    ARCIExyY_y(new_syswhite) = YC(*wp);
    ARCIExyY_Y(new_syswhite) = 1.0;

    xyy_to_xyz( art_gv, & new_syswhite, & ARSWP_SYSWHITE_XYZ );

    pthread_mutex_unlock( & ARSWP_MUTEX );
}

int art_system_white_point_has_been_manually_set(
        const ART_GV  * art_gv
        )
{
    return ARSWP_MANUAL_SYSWHITE;
}

ArSymbol art_system_white_point_symbol(
        const ART_GV  * art_gv
        )
{
    return ARSWP_SYSWHITE_SYMBOL;
}

ArCIEXYZ const * art_system_white_point_xyz(
        const ART_GV  * art_gv
        )
{
    return & ARSWP_SYSWHITE_XYZ;
}

ArCIExy const * art_system_white_point_xy(
        const ART_GV  * art_gv
        )
{
    return & ARSWP_SYSWHITE_XY;
}

/* ======================================================================== */
