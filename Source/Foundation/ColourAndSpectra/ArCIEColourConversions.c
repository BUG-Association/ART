/* ===========================================================================

    Copyright (c) 1996-2018 The ART Development Team
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

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

#include "ART_Foundation_Math.h"

#include "ColourAndSpectralDataConversion_ImplementationMacros.h"

void lab_find_nearest_below_L100(
        const ART_GV         * art_gv,
        const ArCIELab       * lab_outside,
        const ArCIELab       * lab_inside,
        const int              recursionDepth,
              ArCIELab       * lab_r
        )
{
    ArCIELab  midpoint;

    ARCIELab_L( midpoint ) = ( ARCIELab_L(*lab_outside) + ARCIELab_L(*lab_inside) ) / 2.0;
    ARCIELab_a( midpoint ) = ( ARCIELab_a(*lab_outside) + ARCIELab_a(*lab_inside) ) / 2.0;
    ARCIELab_b( midpoint ) = ( ARCIELab_b(*lab_outside) + ARCIELab_b(*lab_inside) ) / 2.0;

    if ( recursionDepth > 0 )
    {
        if ( ARCIELab_L( midpoint ) > 100.0 )
            lab_find_nearest_below_L100(
                  art_gv,
                & midpoint,
                  lab_inside,
                  recursionDepth - 1,
                  lab_r );
        else
            lab_find_nearest_below_L100(
                  art_gv,
                  lab_outside,
                & midpoint,
                  recursionDepth - 1,
                  lab_r );
    }
    else
    {
        if ( ARCIELab_L( midpoint ) > 100.0 )
            *lab_r = *lab_inside;
        else
            *lab_r = midpoint;
    }
}

void lab_move_luminance_below_100(
        const ART_GV    * art_gv,
        const double      focusLuminance,
              ArCIELab  * lab_r
        )
{
    if ( ARCIELab_L( *lab_r ) > 100.0 )
    {
        ArCIELab  focusPoint = ARCIELab( focusLuminance, 0.0, 0.0 );

        lab_find_nearest_below_L100(
              art_gv,
              lab_r,
            & focusPoint,
              20,
              lab_r );
    }
}

#ifdef _ART_WITHOUT_LCMS_

/*
void moveToGamut(
        const ArCIEXYZ       * xyz_out,
        const ArCIEXYZ       * xyz_in,
        const ArColourSpace  * colourspace,
        const int              maximalRecursion,
              ArRGB          * rgb_r
        )
{
    ArCIEXYZ  midpoint;
    ArRGB     midpointRGB;

    CIEXYZ_XC(midpoint) = (CIEXYZ_XC(*outside) + CIEXYZ_XC(*inside)) / 2.0;
    CIEXYZ_YC(midpoint) = (CIEXYZ_YC(*outside) + CIEXYZ_YC(*inside)) / 2.0;
    CIEXYZ_ZC(midpoint) = (CIEXYZ_ZC(*outside) + CIEXYZ_ZC(*inside)) / 2.0;

    c3_cm_mul_c( &midpoint.c, &colourspace->ciexyz_to_rgb, &midpointRGB.c );

    if (depth > 0)
    {
        if (   RC(midpointRGB)<0.0 || GC(midpointRGB)<0.0 || BC(midpointRGB)<0.0
            || RC(midpointRGB)>1.0 || GC(midpointRGB)>1.0 || BC(midpointRGB)>1.0)
            moveToGamut(&midpoint, inside, colourspace, depth-1, finalRGB);
        else
            moveToGamut(outside, &midpoint, colourspace, depth-1, finalRGB);
    }
    else
    {
        if (   RC(midpointRGB)<0.0 || GC(midpointRGB)<0.0 || BC(midpointRGB)<0.0
            || RC(midpointRGB)>1.0 || GC(midpointRGB)>1.0 || BC(midpointRGB)>1.0)
            c3_cm_mul_c( &inside->c, &colourspace->ciexyz_to_rgb,
                         &finalRGB->c );
        else
            *finalRGB = midpointRGB;
    }
}
*/

void xyz_conversion_to_rgb(
        const ART_GV                * art_gv,
        const ArCIEXYZ              * xyz_0,
        const ArColourSpaceRef        rgb_colourspace_ref,
        const ArColourTransformRef    transform,
              ArRGB                 * rgb_r
        )
{
// -----   apply XYZ -> RGB transformation matrix   ----------------------------

    c3_cm_mul_c(
        & ARCIEXYZ_C(*xyz_0),
        & ARCSR_XYZ_TO_RGB(rgb_colourspace_ref),
        & ARRGB_C(*rgb_r) );

/*
    if(     gamutmapping->method == arrgb_gamutmapping_linear
        && (   RC(*result) < 0.0 || GC(*result) < 0.0 || BC(*result) < 0.0
            || RC(*result) > 1.0 || GC(*result) > 1.0 || BC(*result) > 1.0 ) )
    {
        RGB        rgbWhite = RGB_WHITE;
        ART_CIEXYZ     sciexyz, greyPoint, whitePoint;
        ShearZX3D  whiteShear;

        c3_cm_mul_c( &rgbWhite.c, &colourspace->rgb_to_ciexyz, &whitePoint.c );
        XC(whiteShear) = CIEXYZ_XC(whitePoint) / CIEXYZ_YC(whitePoint);
        YC(whiteShear) = CIEXYZ_ZC(whitePoint) / CIEXYZ_YC(whitePoint);
        g3d_p_shearzx_p(((Pnt3D*) & cciexyz),&whiteShear,((Pnt3D*) &sciexyz));

        if ( CIEXYZ_YC(sciexyz) < gamutmapping->threshold[0] )
        {
            double  cdist, k, whiteY;

            cdist = sqrt(  CIEXYZ_XC(sciexyz)*CIEXYZ_XC(sciexyz)
                         + CIEXYZ_ZC(sciexyz)*CIEXYZ_ZC(sciexyz));
            k =   ( YC(gamutmapping->focus[0]) - CIEXYZ_YC(sciexyz) )
                / ( cdist + XC(gamutmapping->focus[0]) );
            whiteY = cdist * k + CIEXYZ_YC(sciexyz);
            CIEXYZ_XC(greyPoint) = whiteY * CIEXYZ_XC(whitePoint);
            CIEXYZ_YC(greyPoint) = whiteY * CIEXYZ_YC(whitePoint);
            CIEXYZ_ZC(greyPoint) = whiteY * CIEXYZ_ZC(whitePoint);
        }
        else
        {
            if ( CIEXYZ_YC(cciexyz) > gamutmapping->threshold[1] )
            {
                double  cdist, k, whiteY;

                cdist = sqrt(  CIEXYZ_XC(sciexyz)*CIEXYZ_XC(sciexyz)
                             + CIEXYZ_ZC(sciexyz)*CIEXYZ_ZC(sciexyz));
                k =   (   CIEXYZ_YC(sciexyz)
                        - YC(gamutmapping->focus[1]) )
                    / ( cdist + XC(gamutmapping->focus[1]) );
                whiteY =   XC(gamutmapping->focus[1]) * k
                         + YC(gamutmapping->focus[1]);
                CIEXYZ_XC(greyPoint) = whiteY * CIEXYZ_XC(whitePoint);
                CIEXYZ_YC(greyPoint) = whiteY * CIEXYZ_YC(whitePoint);
                CIEXYZ_ZC(greyPoint) = whiteY * CIEXYZ_ZC(whitePoint);
            }
            else
            {
                CIEXYZ_XC(greyPoint) =   CIEXYZ_YC(sciexyz)
                                       * CIEXYZ_XC(whitePoint);
                CIEXYZ_YC(greyPoint) =   CIEXYZ_YC(sciexyz)
                                       * CIEXYZ_YC(whitePoint);
                CIEXYZ_ZC(greyPoint) =   CIEXYZ_YC(sciexyz)
                                       * CIEXYZ_ZC(whitePoint);
            }
        }

        if ( CIEXYZ_YC(greyPoint) > CIEXYZ_YC(whitePoint) )
            *result = RGB_WHITE;
        else
            moveToGamut(&cciexyz, &greyPoint, colourspace, 20, result);
    }
*/
// -----   paint out-of-gamut colours according to specifications   ------------

/*
    unsigned int  outOfGamutColourIndex = 0;

    if (    ARRGB_GM_FLAG_VALUES_BELOW_ZERO(*gamutmapping)
        && (   ARRGB_R(*rgb_r) < 0.0
            || ARRGB_G(*rgb_r) < 0.0
            || ARRGB_B(*rgb_r) < 0.0 ) )
        outOfGamutColourIndex |= 0x01;

    if (    ARRGB_GM_FLAG_VALUES_ABOVE_ONE(*gamutmapping)
        && (   ARRGB_R(*rgb_r) > 1.0
            || ARRGB_G(*rgb_r) > 1.0
            || ARRGB_B(*rgb_r) > 1.0 ) )
        outOfGamutColourIndex |= 0x02;
*/
    //   If the out of gamut colour index is > 0 convert the untagged
    //   RGB colour which is provided by the gamut mapping struct for flagging
    //   purposes to the target RGB colour space; this ensures that the flag
    //   colour is always a maximal RGB colour regardless of target colour
    //   space.

/*
    if ( outOfGamutColourIndex != 0 )
        utf_rgb_cs_to_rgb(
            & ARRGB_GM_OOGC_I( *gamutmapping, outOfGamutColourIndex ),
              rgb_colourspace_ref,
              rgb_r );
*/
    //   Clamp the result of all preceding operations to positive values

    rgb_dd_clamp_c( 0.0, MATH_HUGE_DOUBLE, rgb_r );

    //   Set the correct colour space ref in the result

    ARRGB_S(*rgb_r) = rgb_colourspace_ref;
}

void lab_conversion_to_rgb(
        const ArCIELab              * lab_0,
        const ArColourSpaceRef        rgb_colourspace_ref,
        const ArColourTransformRef    transform,
              ArRGB                 * rgb_r
        )
{
}

#else  // _ART_WITHOUT_LCMS_

void xyz_conversion_to_rgb(
        const ART_GV                * art_gv,
        const ArCIEXYZ              * xyz_0,
        const ArColourSpaceRef        rgb_colourspace_ref,
        const ArColourTransformRef    transform,
              ArRGB                 * rgb_r
        )
{
    CC_START_DEBUGPRINTF( xyz_conversion_to_rgb )
    ArCIELab  labValue;

    xyz_to_lab(
          art_gv,
          xyz_0,
        & labValue );

    lab_conversion_to_rgb(
          art_gv,
        & labValue,
          rgb_colourspace_ref,
          transform,
          rgb_r );
    CC_END_DEBUGPRINTF( xyz_conversion_to_rgb )
}

void lab_conversion_to_rgb(
        const ART_GV                * art_gv,
        const ArCIELab              * lab_0,
        ArColourSpace const         * rgb_colourspace_ref,
        const ArColourTransformRef    transform,
              ArRGB                 * rgb_r
        )
{
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
    ARRGB_S(*rgb_r) = rgb_colourspace_ref;
    CC_END_DEBUGPRINTF( lab_conversion_to_rgb )
}

#endif  // _ART_WITHOUT_LCMS_

void xyy_to_xyz(
        const ART_GV    * art_gv,
        const ArCIExyY  * xyy_0,
              ArCIEXYZ  * xyz_r
        )
{
    CC_START_DEBUGPRINTF( xyy_to_xyz )
    double Yy = ARCIExyY_Y(*xyy_0) / ARCIExyY_y(*xyy_0);

    ARCIEXYZ_X(*xyz_r) = ARCIExyY_x(*xyy_0) * Yy;
    ARCIEXYZ_Y(*xyz_r) = ARCIExyY_Y(*xyy_0);
    ARCIEXYZ_Z(*xyz_r) = (  1
                          - ARCIExyY_x(*xyy_0)
                          - ARCIExyY_y(*xyy_0) ) * Yy;

    ARTCV_S(*xyz_r) = ARCSR_CIEXYZ;
    CC_END_DEBUGPRINTF( xyy_to_xyz )
}

void xyz_to_xyy(
        const ART_GV    * art_gv,
        const ArCIEXYZ  * xyz_0,
              ArCIExyY  * xyy_r
        )
{
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

    ARTCV_S(*xyy_r) = ARCSR_CIExyY;
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

    ARCIEXYZ_S(*xyz_r) = ARCSR_CIEXYZ;
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

    ARCIELab_S(*lab_r) = ARCSR_CIELab;
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

    ARTCV_S(*luv_r) = ARCSR_CIELuv;

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

    ARTCV_S(*xyz_r) = ARCSR_CIEXYZ;
    
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

void xyz_d_mul_c(
        const ART_GV    * art_gv,
        const double      d0,
              ArCIEXYZ  * cr
        )
{
    ArCIExyY  temp;

    xyz_to_xyy( art_gv, cr, & temp );
    ARCIExyY_Y(temp) *= d0;
    xyy_to_xyz( art_gv, & temp, cr );
}

/* ======================================================================== */
