/* ===========================================================================

    Copyright (c) 1996-2021 The ART Development Team
    ------------------------------------------------

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

#define ART_MODULE_NAME     ArCIEColourConversions

#include "ArCIEColourConversions.h"
#include "ArRGB.h"
#include "ArCIExy.h"
#include "ArCIExyY.h"
#include "ArCIEXYZ.h"
#include "ColourAndSpectralDataConversion.h"
#include "SystemWhitepoint.h"

#include <pthread.h>

typedef struct ArCIEColourConversions_GV
{
    pthread_mutex_t    mutex;

    ArRGBGamutMapping  gm_method;
    int                recursion_depth;
    double             focus_luminance;
    ArRGB              negative_flag_colour;
    ArRGB              above_one_flag_colour;
    ArRGB              both_flag_colour;
}
ArCIEColourConversions_GV;

#define ARCIECV_GV                  art_gv->arciecolourconversions_gv
#define ARCIECV_MUTEX               ARCIECV_GV->mutex
#define ARCIECV_GM_METHOD           ARCIECV_GV->gm_method
#define ARCIECV_GM_RECDEPTH         ARCIECV_GV->recursion_depth
#define ARCIECV_GM_FOCUS            ARCIECV_GV->focus_luminance
#define ARCIECV_NEG_FLAG_RGB        ARCIECV_GV->negative_flag_colour
#define ARCIECV_POS_FLAG_RGB        ARCIECV_GV->above_one_flag_colour
#define ARCIECV_BOTH_FLAG_RGB       ARCIECV_GV->both_flag_colour

ART_MODULE_INITIALISATION_FUNCTION
(
    ARCIECV_GV = ALLOC(ArCIEColourConversions_GV);

    pthread_mutex_init( & ARCIECV_MUTEX, NULL );
 
#ifndef _ART_WITHOUT_LCMS_
    ARCIECV_GM_METHOD     = arrgb_gm_lcms;
//    ARCIECV_GM_METHOD    = arrgb_gm_linear;
#else
    ARCIECV_GM_METHOD     = arrgb_gm_linear;
#endif
    ARCIECV_GM_RECDEPTH   = 20;
    ARCIECV_GM_FOCUS      = 0.1;
    ARCIECV_POS_FLAG_RGB  = ARRGB(0,0,0);
    ARCIECV_NEG_FLAG_RGB  = ARRGB(1,1,1);
    ARCIECV_BOTH_FLAG_RGB = ARRGB(1,0,0);
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    pthread_mutex_destroy( & ARCIECV_MUTEX );

    FREE( ARCIECV_GV );
)


void setRGBGamutMappingMethod(
              ART_GV             * art_gv,
        const ArRGBGamutMapping    method,
        const double               focus_luminance
        )
{
    pthread_mutex_lock( & ARCIECV_MUTEX );
    
    ARCIECV_GM_METHOD = method;

    if ( focus_luminance >= 0. && focus_luminance <= 1. )
        ARCIECV_GM_FOCUS = focus_luminance;
    
    pthread_mutex_unlock( & ARCIECV_MUTEX );
}

ArRGBGamutMapping currentRGBGamutMappingMethod(
        const ART_GV  * art_gv
        )
{
    return  ARCIECV_GM_METHOD;
}

#include "ColourAndSpectralDataConversion_ImplementationMacros.h"

void xyz_mat_to_xyz(
        const ART_GV    * art_gv,
        const ArCIEXYZ  * xyz_0,
        const Mat3      * mat_0,
              ArCIEXYZ  * xyz_r
        )
{
   (void) art_gv;
    
    c3_cm_mul_c(
        & ARCIEXYZ_C(*xyz_0),
          mat_0,
        & ARCIEXYZ_C(*xyz_r)
        );
}

void xyz_mat_to_rgb(
        const ART_GV    * art_gv,
        const ArCIEXYZ  * xyz_0,
        const Mat3      * mat_0,
              ArRGB     * rgb_r
        )
{
   (void) art_gv;
    
    c3_cm_mul_c(
        & ARCIEXYZ_C(*xyz_0),
          mat_0,
        & ARRGB_C(*rgb_r)
        );
}

void rgb_mat_to_xyz(
        const ART_GV    * art_gv,
        const ArRGB     * rgb_0,
        const Mat3      * mat_0,
              ArCIEXYZ  * xyz_r
        )
{
    (void) art_gv;
    
    c3_cm_mul_c(
        & ARRGB_C(*rgb_0),
          mat_0,
        & ARCIEXYZ_C(*xyz_r)
        );
}


void xyz_move2unit_gamut(
        const ART_GV    * art_gv,
        const ArCIEXYZ  * outsideXYZ,
        const ArCIEXYZ  * insideXYZ,
        const Mat3      * xyz2rgb,
        const int         depth,
              ArRGB     * finalRGB
        )
{
    ArCIEXYZ  midpointXYZ;
    ArRGB     midpointRGB;
    
    XC(midpointXYZ) = ( XC(*outsideXYZ) + XC(*insideXYZ) ) / 2.0;
    YC(midpointXYZ) = ( YC(*outsideXYZ) + YC(*insideXYZ) ) / 2.0;
    ZC(midpointXYZ) = ( ZC(*outsideXYZ) + ZC(*insideXYZ) ) / 2.0;
    
    xyz_mat_to_rgb(
          art_gv,
        & midpointXYZ,
          xyz2rgb,
        & midpointRGB
        );

    if ( depth > 0 )
    {
    if (   XC(midpointRGB)<0.0 || YC(midpointRGB)<0.0 || ZC(midpointRGB)<0.0
        || XC(midpointRGB)>1.0 || YC(midpointRGB)>1.0 || ZC(midpointRGB)>1.0)
        xyz_move2unit_gamut(art_gv,&midpointXYZ, insideXYZ, xyz2rgb, depth-1, finalRGB);
    else
        xyz_move2unit_gamut(art_gv,outsideXYZ, &midpointXYZ, xyz2rgb, depth-1, finalRGB);
    }
    else
    {
        if (   XC(midpointRGB)<0.0 || YC(midpointRGB)<0.0 || ZC(midpointRGB)<0.0
            || XC(midpointRGB)>1.0 || YC(midpointRGB)>1.0 || ZC(midpointRGB)>1.0)
        {
            xyz_mat_to_rgb(
                  art_gv,
                  insideXYZ,
                  xyz2rgb,
                  finalRGB
                );
        }
        else
            *finalRGB = midpointRGB;
    }
}

void xyz_move2gamut(
        const ART_GV    * art_gv,
        const ArCIEXYZ  * outsideXYZ,
        const ArCIEXYZ  * insideXYZ,
        const Mat3      * xyz2rgb,
        const int         depth,
              ArRGB     * finalRGB
        )
{
    ArCIEXYZ  midpointXYZ;
    ArRGB     midpointRGB;
    
    XC(midpointXYZ) = ( XC(*outsideXYZ) + XC(*insideXYZ) ) / 2.0;
    YC(midpointXYZ) = ( YC(*outsideXYZ) + YC(*insideXYZ) ) / 2.0;
    ZC(midpointXYZ) = ( ZC(*outsideXYZ) + ZC(*insideXYZ) ) / 2.0;
    
    xyz_mat_to_rgb(
          art_gv,
        & midpointXYZ,
          xyz2rgb,
        & midpointRGB
        );

    if ( depth > 0 )
    {
    if ( XC(midpointRGB)<0.0 || YC(midpointRGB)<0.0 || ZC(midpointRGB)<0.0 )
        xyz_move2gamut(art_gv,&midpointXYZ, insideXYZ, xyz2rgb, depth-1, finalRGB);
    else
        xyz_move2gamut(art_gv,outsideXYZ, &midpointXYZ, xyz2rgb, depth-1, finalRGB);
    }
    else
    {
        if ( XC(midpointRGB)<0.0 || YC(midpointRGB)<0.0 || ZC(midpointRGB)<0.0)
        {
            xyz_mat_to_rgb(
                  art_gv,
                  insideXYZ,
                  xyz2rgb,
                  finalRGB
                );
        }
        else
            *finalRGB = midpointRGB;
    }
}

void xyz_conversion_to_unit_rgb_with_gamma(
        const ART_GV    * art_gv,
        const ArCIEXYZ  * xyz_0,
              ArRGB     * rgb_r
        )
{
#ifndef _ART_WITHOUT_LCMS_
        if ( ( ARCIECV_GM_METHOD & arrgb_gm_technique_mask ) == arrgb_gm_lcms )
        {
            cmsDoTransform(
                   ARCSR_XYZ_TO_RGB_TRAFO(DEFAULT_RGB_SPACE_REF),
                 & ARCIEXYZ_C(*xyz_0),
                 & ARRGB_C(*rgb_r),
                   1
                 );
            
            goto gammacorrection;
        }
        else
        {
#endif
            xyz_mat_to_rgb(
                  art_gv,
                  xyz_0,
                & ARCSR_XYZ_TO_RGB(DEFAULT_RGB_SPACE_REF),
                  rgb_r
                );

        //   Nothing to do if we are already in gamut

        if (   ARRGB_R(*rgb_r) < 0. || ARRGB_R(*rgb_r) > 1.
            || ARRGB_G(*rgb_r) < 0. || ARRGB_G(*rgb_r) > 1.
            || ARRGB_B(*rgb_r) < 0. || ARRGB_B(*rgb_r) > 1. )
        {
            if ( ( ARCIECV_GM_METHOD & arrgb_gm_technique_mask ) == arrgb_gm_linear )
            {
                //   ART internal "move to gamut" functionality
                
                double  focusdelta = ARCIECV_GM_FOCUS - YC(*xyz_0);
                double  focus = ARCIECV_GM_FOCUS - focusdelta;
                
                focus = M_CLAMP(focus, 0.1, 0.9);
                
                ArCIExyY  inside_xyy =
                    ARCIExyY(
                        XC(ARCSR_W(DEFAULT_RGB_SPACE_REF)),
                        YC(ARCSR_W(DEFAULT_RGB_SPACE_REF)),
                        focus
                        );
                
                ArCIEXYZ  inside_xyz;
                
                xyy_to_xyz(art_gv, & inside_xyy, & inside_xyz );
                
                xyz_move2unit_gamut(
                      art_gv,
                      xyz_0,
                    & inside_xyz,
                    & ARCSR_XYZ_TO_RGB(DEFAULT_RGB_SPACE_REF),
                      ARCIECV_GM_RECDEPTH,
                      rgb_r
                    );
                
                goto gammacorrection;
            }
        }
        
        //   Default behaviour: clamping. In this case, we might flag
        //   out of gamut colours before clamping

        int  flags = 0;
        
        if ( ( ARCIECV_GM_METHOD & arrgb_gm_feature_mask ) & arrgb_gm_flag_neg )
        {
            if (   ARRGB_R(*rgb_r) < 0.
                || ARRGB_R(*rgb_r) < 0.
                || ARRGB_R(*rgb_r) < 0. )
            {
                ARRGB_C(*rgb_r) = ARCIECV_NEG_FLAG_RGB.c;
                flags++;
            }
        }

        if ( ( ARCIECV_GM_METHOD & arrgb_gm_feature_mask ) & arrgb_gm_flag_above_one )
        {
            if (   ARRGB_R(*rgb_r) > 1.
                || ARRGB_R(*rgb_r) > 1.
                || ARRGB_R(*rgb_r) > 1. )
            {
                ARRGB_C(*rgb_r) = ARCIECV_POS_FLAG_RGB.c;
                flags++;
            }
        }
        
        if ( flags == 2 )
        {
            ARRGB_C(*rgb_r) = ARCIECV_BOTH_FLAG_RGB.c;
        }
#ifndef _ART_WITHOUT_LCMS_
    }
#endif

    gammacorrection:

    //   Clamp the result of all preceding operations to positive values

    rgb_dd_clamp_c( art_gv, 0.0, 1.0, rgb_r );
    
#ifndef _ART_WITHOUT_LCMS_
    if ( ( ARCIECV_GM_METHOD & arrgb_gm_technique_mask ) != arrgb_gm_lcms )
    {
#endif
        ARRGB_R(*rgb_r) = ARCSR_GAMMAFUNCTION(DEFAULT_RGB_SPACE_REF,ARRGB_R(*rgb_r));
        ARRGB_G(*rgb_r) = ARCSR_GAMMAFUNCTION(DEFAULT_RGB_SPACE_REF,ARRGB_G(*rgb_r));
        ARRGB_B(*rgb_r) = ARCSR_GAMMAFUNCTION(DEFAULT_RGB_SPACE_REF,ARRGB_B(*rgb_r));
#ifndef _ART_WITHOUT_LCMS_
    }
#endif
}

void xyz_conversion_to_linear_rgb(
        const ART_GV    * art_gv,
        const ArCIEXYZ  * xyz_0,
              ArRGB     * rgb_r
        )
{
    xyz_mat_to_rgb(
          art_gv,
          xyz_0,
        & ARCSR_XYZ_TO_RGB(DEFAULT_RGB_SPACE_REF),
          rgb_r
        );

    if ( ( ARCIECV_GM_METHOD & arrgb_gm_technique_mask ) == arrgb_gm_clipping )
    {
        if ( ( ARCIECV_GM_METHOD & arrgb_gm_feature_mask ) & arrgb_gm_flag_neg )
        {
            if (   ARRGB_R(*rgb_r) < 0.
                || ARRGB_R(*rgb_r) < 0.
                || ARRGB_R(*rgb_r) < 0. )
                ARRGB_C(*rgb_r) = ARCIECV_NEG_FLAG_RGB.c;
        }

        if ( ( ARCIECV_GM_METHOD & arrgb_gm_feature_mask ) & arrgb_gm_flag_above_one )
        {
            if (   ARRGB_R(*rgb_r) > 1.
                || ARRGB_R(*rgb_r) > 1.
                || ARRGB_R(*rgb_r) > 1. )
                ARRGB_C(*rgb_r) = ARCIECV_POS_FLAG_RGB.c;
        }

        //   Clamp the result of all preceding operations to positive values

        rgb_dd_clamp_c( art_gv, 0.0, MATH_HUGE_DOUBLE, rgb_r );
    }
    else
    {
        //   Nothing to do if we are already in gamut

        if (   ARRGB_R(*rgb_r) < 0.
            || ARRGB_G(*rgb_r) < 0.
            || ARRGB_B(*rgb_r) < 0. )
        {
            //   As this is gamut mapping in an open ended HDR RGB space,
            //   we move towards the neutral axis only, i.e. towards
            //   a point with the same CIE Y coordinate, but on the main
            //   diagonal
            
            ArCIExyY  inside_xyy =
                ARCIExyY(
                    XC(ARCSR_W(DEFAULT_RGB_SPACE_REF)),
                    YC(ARCSR_W(DEFAULT_RGB_SPACE_REF)),
                    YC(*xyz_0)
                    );
            
            ArCIEXYZ  inside_xyz;
            
            xyy_to_xyz(art_gv, & inside_xyy, & inside_xyz );

            xyz_move2gamut(
                  art_gv,
                  xyz_0,
                & inside_xyz,
                & ARCSR_XYZ_TO_RGB(DEFAULT_RGB_SPACE_REF),
                  ARCIECV_GM_RECDEPTH,
                  rgb_r
                );
        }
    }
}

#ifdef _ART_WITHOUT_LCMS_


void lab_conversion_to_rgb(
        const ArCIELab              * lab_0,
        const ArColourSpaceRef        rgb_colourspace_ref,
        const ArColourTransformRef    transform,
              ArRGB                 * rgb_r
        )
{
    (void) lab_0;
    (void) rgb_colourspace_ref;
    (void) transform;
    (void) rgb_r;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

#else  // _ART_WITHOUT_LCMS_

void lab_conversion_to_rgb(
        const ART_GV                * art_gv,
        const ArCIELab              * lab_0,
        ArColourSpace const         * rgb_colourspace_ref,
        const ArColourTransformRef    transform,
              ArRGB                 * rgb_r
        )
{
    (void) art_gv;
    (void) rgb_colourspace_ref;
    
    CC_START_DEBUGPRINTF( lab_conversion_to_rgb )
    ArCIELab  labValue = *lab_0;

    m_dd_clamp_d( 0.0, 100.0, & ARCIELab_L(labValue) );

    double  labArray[3];
    double  rgbArray[3];

    labArray[0] = ARCIELab_L(labValue);
    labArray[1] = ARCIELab_a(labValue);
    labArray[2] = ARCIELab_b(labValue);

    cmsDoTransform(
         transform,
         labArray,
         rgbArray,
         1
         );

    ARRGB_R(*rgb_r) = rgbArray[0];
    ARRGB_G(*rgb_r) = rgbArray[1];
    ARRGB_B(*rgb_r) = rgbArray[2];
    CC_END_DEBUGPRINTF( lab_conversion_to_rgb )
}

#endif  // _ART_WITHOUT_LCMS_

void xyy_to_xyz(
        const ART_GV    * art_gv,
        const ArCIExyY  * xyy_0,
              ArCIEXYZ  * xyz_r
        )
{
    (void) art_gv;
    
    CC_START_DEBUGPRINTF( xyy_to_xyz )
    double Yy = ARCIExyY_Y(*xyy_0) / ARCIExyY_y(*xyy_0);

    ARCIEXYZ_X(*xyz_r) = ARCIExyY_x(*xyy_0) * Yy;
    ARCIEXYZ_Y(*xyz_r) = ARCIExyY_Y(*xyy_0);
    ARCIEXYZ_Z(*xyz_r) = (  1
                          - ARCIExyY_x(*xyy_0)
                          - ARCIExyY_y(*xyy_0) ) * Yy;

    CC_END_DEBUGPRINTF( xyy_to_xyz )
}

void xyz_to_xyy(
        const ART_GV    * art_gv,
        const ArCIEXYZ  * xyz_0,
              ArCIExyY  * xyy_r
        )
{
    (void) art_gv;
    
    CC_START_DEBUGPRINTF( xyz_to_xyy )
    double xyz =   ARCIEXYZ_X(*xyz_0)
                 + ARCIEXYZ_Y(*xyz_0)
                 + ARCIEXYZ_Z(*xyz_0);
    
    if ( xyz > 0. )
    {
        ARCIExyY_x(*xyy_r) = ARCIEXYZ_X(*xyz_0) / xyz;
        ARCIExyY_y(*xyy_r) = ARCIEXYZ_Y(*xyz_0) / xyz;
    }
    else
    {
        ARCIExyY_x(*xyy_r) = 0.;
        ARCIExyY_y(*xyy_r) = 0.;
    }

    ARCIExyY_Y(*xyy_r) = ARCIEXYZ_Y(*xyz_0);

    CC_END_DEBUGPRINTF( xyz_to_xyy )
}

#define  DELTA              6.0 / 29.0
#define  DELTA_SQR_MUL_3    3.0 * M_SQR(DELTA)

void lab_wp_to_xyz(
        const ART_GV    * art_gv,
        const ArCIELab  * lab_0,
        const ArCIEXYZ  * xyz_w,
              ArCIEXYZ  * xyz_r
        )
{
    (void) art_gv;
    
    CC_START_DEBUGPRINTF( lab_wp_to_xyz )
    double f_Y = ( ARCIELab_L(*lab_0) + 16.0 ) / 116.0;
    double f_X = f_Y + ARCIELab_a(*lab_0) / 500.0;
    double f_Z = f_Y - ARCIELab_b(*lab_0) / 200.0;

    if ( f_Y > DELTA )
        ARCIEXYZ_Y(*xyz_r) = ARCIEXYZ_Y(*xyz_w) * M_CUBE(f_Y);
    else
        ARCIEXYZ_Y(*xyz_r) =   ( f_Y - 16.0 / 116.0 )
                             * DELTA_SQR_MUL_3 * ARCIEXYZ_Y(*xyz_w);

    if ( f_X > DELTA )
        ARCIEXYZ_X(*xyz_r) = ARCIEXYZ_X(*xyz_w) * M_CUBE(f_X);
    else
        ARCIEXYZ_X(*xyz_r) =   ( f_X - 16.0 / 116.0 )
                             * DELTA_SQR_MUL_3 * ARCIEXYZ_X(*xyz_w);

    if ( f_Z > DELTA )
        ARCIEXYZ_Z(*xyz_r) = ARCIEXYZ_Z(*xyz_w) * M_CUBE(f_Z);
    else
        ARCIEXYZ_Z(*xyz_r) =   ( f_Z - 16.0 / 116.0 )
                             * DELTA_SQR_MUL_3 * ARCIEXYZ_Z(*xyz_w);

    CC_END_DEBUGPRINTF( lab_wp_to_xyz )
}

void lab_to_xyz(
        const ART_GV    * art_gv,
        const ArCIELab  * lab_0,
              ArCIEXYZ  * xyz_r
        )
{
    CC_START_DEBUGPRINTF( lab_to_xyz )
    lab_wp_to_xyz( art_gv, lab_0, & ARCIEXYZ_SYSTEM_WHITE_POINT, xyz_r );
    CC_END_DEBUGPRINTF( lab_to_xyz )
}

double _f_lab(
        const double  d_0
        )
{
    if ( d_0 > 0.008856 )
        return m_d_cbrt( d_0 );
    else
        return 7.787 * d_0 + 16.0 / 116.0;
}

void xyz_wp_to_lab(
        const ART_GV    * art_gv,
        const ArCIEXYZ  * xyz_0,
        const ArCIEXYZ  * xyz_w,
              ArCIELab  * lab_r
        )
{
    (void) art_gv;
    
    CC_START_DEBUGPRINTF( xyz_wp_to_lab )
    
    if ( ARCIEXYZ_Y(*xyz_0) > 0. )
    {
        double  f_X = _f_lab( ARCIEXYZ_X(*xyz_0) / ARCIEXYZ_X(*xyz_w) );
        double  f_Y = _f_lab( ARCIEXYZ_Y(*xyz_0) / ARCIEXYZ_Y(*xyz_w) );
        double  f_Z = _f_lab( ARCIEXYZ_Z(*xyz_0) / ARCIEXYZ_Z(*xyz_w) );

        ARCIELab_L(*lab_r) = 116.0 * f_Y - 16.0;
        ARCIELab_a(*lab_r) = 500.0 * ( f_X - f_Y );
        ARCIELab_b(*lab_r) = 200.0 * ( f_Y - f_Z );
    }
    else
    {
        ARCIELab_L(*lab_r) = 0.;
        ARCIELab_a(*lab_r) = 0.;
        ARCIELab_b(*lab_r) = 0.;
    }

    CC_END_DEBUGPRINTF( xyz_wp_to_lab )
}

void xyz_to_lab(
        const ART_GV    * art_gv,
        const ArCIEXYZ  * xyz_0,
              ArCIELab  * lab_r
        )
{
    CC_START_DEBUGPRINTF( xyz_to_lab )

    xyz_wp_to_lab(
          art_gv,
          xyz_0,
        & ARCIEXYZ_SYSTEM_WHITE_POINT,
          lab_r
        );

    CC_END_DEBUGPRINTF( xyz_to_lab )
}

double luv_u_prime_from_xyz(
        const ART_GV    * art_gv,
        const ArCIEXYZ  * xyz_0
        )
{
    (void) art_gv;
    
    double  result = 0.;
    
    if ( ARCIEXYZ_Y(*xyz_0) > 0. )
    {
        result =
               4.0 * ARCIEXYZ_X(*xyz_0)
            /  (         ARCIEXYZ_X(*xyz_0)
                + 15.0 * ARCIEXYZ_Y(*xyz_0)
                +  3.0 * ARCIEXYZ_Z(*xyz_0) );
    }
    
    return  result;
}

double luv_v_prime_from_xyz(
        const ART_GV    * art_gv,
        const ArCIEXYZ  * xyz_0
        )
{
    (void) art_gv;
    
    double  result = 0.;
    
    if ( ARCIEXYZ_Y(*xyz_0) > 0. )
    {
        result =
               9.0 * ARCIEXYZ_Y(*xyz_0)
            /  (         ARCIEXYZ_X(*xyz_0)
                + 15.0 * ARCIEXYZ_Y(*xyz_0)
                +  3.0 * ARCIEXYZ_Z(*xyz_0) );
    }
    
    return  result;
}

double luv_u_prime_wp_from_luv(
        const ART_GV    * art_gv,
        const ArCIELuv  * luv_0,
        const ArCIEXYZ  * xyz_w
        )
{
    double  u_prime_n = luv_u_prime_from_xyz( art_gv, xyz_w );
    
    return
         ( ARCIELuv_u(*luv_0) / 13.0 * ARCIELuv_L(*luv_0) )
       + u_prime_n;
}

double luv_v_prime_wp_from_luv(
        const ART_GV    * art_gv,
        const ArCIELuv  * luv_0,
        const ArCIEXYZ  * xyz_w
        )
{
    double  v_prime_n = luv_v_prime_from_xyz( art_gv, xyz_w );
    
    return
         ( ARCIELuv_v(*luv_0) / 13.0 * ARCIELuv_L(*luv_0) )
       + v_prime_n;
}

double luv_u_prime_from_luv(
        const ART_GV    * art_gv,
        const ArCIELuv  * luv_0
        )
{
    return
        luv_u_prime_wp_from_luv(
              art_gv,
              luv_0,
            & ARCIEXYZ_SYSTEM_WHITE_POINT
            );
}

double luv_v_prime_from_luv(
        const ART_GV    * art_gv,
        const ArCIELuv  * luv_0
        )
{
    return
        luv_v_prime_wp_from_luv(
              art_gv,
              luv_0,
            & ARCIEXYZ_SYSTEM_WHITE_POINT
            );
}

void xyz_wp_to_luv(
        const ART_GV    * art_gv,
        const ArCIEXYZ  * xyz_0,
        const ArCIEXYZ  * xyz_w,
              ArCIELuv  * luv_r
        )
{
    CC_START_DEBUGPRINTF( xyz_wp_to_luv )
    
    if ( ARCIEXYZ_Y(*xyz_0) > 0. )
    {
        double  wY = ARCIEXYZ_Y(*xyz_0) / ARCIEXYZ_Y(*xyz_w);
        
        if ( wY <= M_CUBE(6.0 / 29.0) )
        {
            ARCIELuv_L(*luv_r) = M_CUBE( 29.0 / 3.0 ) * wY;
        }
        else
        {
            ARCIELuv_L(*luv_r) = 116.0 * cbrt(wY) - 16.0;
        }

        double  u_prime   = luv_u_prime_from_xyz( art_gv, xyz_0 );
        double  v_prime   = luv_v_prime_from_xyz( art_gv, xyz_0 );

        double  u_prime_n = luv_u_prime_from_xyz( art_gv, xyz_w );
        double  v_prime_n = luv_v_prime_from_xyz( art_gv, xyz_w );
        
        ARCIELuv_u(*luv_r) = 13.0 * ARCIELuv_L(*luv_r) * ( u_prime - u_prime_n );
        ARCIELuv_v(*luv_r) = 13.0 * ARCIELuv_L(*luv_r) * ( v_prime - v_prime_n );
    }
    else
    {
        ARCIELuv_L(*luv_r) = 0.;
        ARCIELuv_u(*luv_r) = 0.;
        ARCIELuv_v(*luv_r) = 0.;
    }

    CC_END_DEBUGPRINTF( xyz_wp_to_luv )
}

void xyz_to_luv(
        const ART_GV    * art_gv,
        const ArCIEXYZ  * xyz_0,
              ArCIELuv  * luv_r
        )
{
    CC_START_DEBUGPRINTF( xyz_to_luv )

    xyz_wp_to_luv( art_gv, xyz_0, & ARCIEXYZ_SYSTEM_WHITE_POINT , luv_r );

    CC_END_DEBUGPRINTF( xyz_to_luv )
}

void luv_wp_to_xyz(
        const ART_GV    * art_gv,
        const ArCIELuv  * luv_0,
        const ArCIEXYZ  * xyz_w,
              ArCIEXYZ  * xyz_r
        )
{
    CC_START_DEBUGPRINTF( luv_to_xyz )
    
    if ( ARCIELuv_L(*luv_0) > 0. )
    {
        double  u_prime_n = luv_u_prime_from_xyz( art_gv, xyz_w );
        double  v_prime_n = luv_v_prime_from_xyz( art_gv, xyz_w );
        
        double u_prime =
            ( ARCIELuv_u(*luv_0) / ( 13.0 * ARCIELuv_L(*luv_0) ) ) + u_prime_n;
        double v_prime =
            ( ARCIELuv_v(*luv_0) / ( 13.0 * ARCIELuv_L(*luv_0) ) ) + v_prime_n;
        
        if ( ARCIELuv_L(*luv_0) < 8.0 )
        {
            ARCIEXYZ_Y(*xyz_r) =
                ARCIEXYZ_Y(*xyz_w) * ARCIELuv_L(*luv_0) * M_CUBE( 3 / 29 );
        }
        else
        {
            ARCIEXYZ_Y(*xyz_r) =
                ARCIEXYZ_Y(*xyz_w) * M_CUBE( ( ARCIELuv_L(*luv_0) + 16.0 ) / 116.0 );
        }

        ARCIEXYZ_X(*xyz_r) =
            ARCIEXYZ_Y(*xyz_r) * ( ( 9.0 * u_prime ) / ( 4.0 * v_prime ) );
        ARCIEXYZ_Z(*xyz_r) =
               ARCIEXYZ_Y(*xyz_r)
            * ( ( 12.0 - 3.0 * u_prime - 20.0 * v_prime ) / ( 4.0 * v_prime ) );
    }
    else
    {
        ARCIEXYZ_X(*xyz_r) = 0.;
        ARCIEXYZ_Y(*xyz_r) = 0.;
        ARCIEXYZ_Z(*xyz_r) = 0.;
    }

    CC_END_DEBUGPRINTF( luv_to_xyz )
}

void luv_to_xyz(
        const ART_GV    * art_gv,
        const ArCIELuv  * luv_0,
              ArCIEXYZ  * xyz_r
        )
{
    CC_START_DEBUGPRINTF( luv_to_xyz )

    luv_wp_to_xyz( art_gv, luv_0, & ARCIEXYZ_SYSTEM_WHITE_POINT, xyz_r );

    CC_END_DEBUGPRINTF( luv_to_xyz )
}


/* ======================================================================== */
