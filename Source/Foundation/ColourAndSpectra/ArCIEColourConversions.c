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
    ArRGB              negative_flag_colour;
    ArRGB              above_one_flag_colour;
    ArRGB              both_flag_colour;
}
ArCIEColourConversions_GV;

#define ARCIECV_GV                  art_gv->arciecolourconversions_gv
#define ARCIECV_MUTEX               ARCIECV_GV->mutex
#define ARCIECV_GM_METHOD           ARCIECV_GV->gm_method
#define ARCIECV_GM_RECDEPTH         ARCIECV_GV->recursion_depth
#define ARCIECV_NEG_FLAG_RGB        ARCIECV_GV->negative_flag_colour
#define ARCIECV_POS_FLAG_RGB        ARCIECV_GV->above_one_flag_colour
#define ARCIECV_BOTH_FLAG_RGB       ARCIECV_GV->both_flag_colour

static double ARCIECV_GM_FOCUS[360] =
{
    47.117667, 47.465743, 47.670518, 48.137339, 48.791513, 49.629431, 49.876513, 50.385664,
    50.857281, 51.442624, 51.980828, 52.930511, 53.331701, 54.065367, 54.625968, 54.806434,
    55.308008, 55.781699, 56.238347, 56.274596, 56.675374, 57.118464, 57.467996, 57.494885,
    57.870143, 57.716397, 58.042766, 58.414036, 58.880382, 59.144615, 59.567406, 59.411553,
    59.257341, 59.730842, 59.267487, 59.226562, 60.056652, 60.455674, 60.523344, 61.209694,
    61.553487, 61.509901, 61.466841, 61.759624, 61.876630, 62.285087, 62.629962, 62.638917,
    63.458371, 63.683694, 63.385072, 63.339425, 63.400970, 63.250058, 63.150314, 63.213363,
    63.058978, 62.957100, 62.692491, 62.534759, 62.267006, 61.998775, 61.621359, 61.186433,
    60.751421, 60.258650, 59.818852, 59.320973, 58.876339, 58.427260, 57.918864, 57.465250,
    57.007270, 56.549503, 56.087432, 55.625678, 55.159483, 54.750757, 54.280783, 53.868689,
    53.395364, 52.980251, 52.503802, 52.085989, 51.665986, 51.183478, 50.761304, 50.276385,
    49.852312, 49.365553, 48.940057, 48.452303, 48.025820, 47.537648, 47.048738, 46.560461,
    46.072346, 45.522836, 45.035962, 44.550889, 44.005151, 43.522737, 42.980718, 42.442347,
    41.845141, 41.312637, 40.722847, 40.136694, 39.556989, 38.921652, 38.293670, 37.673869,
    37.000550, 36.336255, 35.630955, 34.876846, 34.133244, 33.354266, 32.540435, 31.680156,
    30.789104, 29.869926, 28.879609, 27.864032, 26.782231, 25.637070, 24.432300, 23.133589,
    21.795281, 20.470143, 19.023730, 17.458957, 17.575832, 17.962299, 18.868671, 19.873734,
    21.080785, 22.752110, 24.508177, 26.463741, 28.856395, 31.254744, 33.937915, 36.755697,
    39.677693, 42.747799, 44.166040, 45.405425, 46.260099, 47.149552, 48.065344, 48.644702,
    49.097674, 49.516167, 49.501656, 49.729505, 49.915499, 49.894569, 50.041619, 50.355083,
    50.678709, 51.057060, 51.126418, 51.250977, 51.220550, 51.245008, 51.378155, 51.409639,
    51.495795, 51.425918, 51.357044, 51.342109, 51.534951, 51.573433, 51.457110, 51.185987,
    51.279705, 51.425404, 51.571987, 51.563472, 51.555393, 51.547744, 51.748468, 51.950103,
    51.995929, 52.249553, 51.985354, 51.876594, 51.923405, 51.970694, 52.069856, 51.962038,
    51.854259, 51.746549, 51.638915, 51.531348, 51.215893, 51.264259, 51.105083, 50.738534,
    50.683534, 50.472908, 50.418437, 50.364085, 50.258161, 50.100125, 49.889624, 49.887630,
    49.781345, 49.831719, 50.038348, 49.724930, 49.723450, 49.878447, 49.824966, 49.979692,
    49.822033, 49.820808, 49.819628, 50.025780, 50.439096, 50.798031, 51.414765, 52.027652,
    52.636395, 53.552190, 54.464776, 55.370505, 56.476484, 57.474073, 58.774239, 60.062932,
    61.293696, 62.511962, 63.716840, 64.863778, 65.996415, 67.071451, 68.131295, 69.134065,
    70.169853, 71.148728, 72.110867, 73.102981, 74.038945, 74.956831, 75.865539, 76.755468,
    77.626274, 78.487434, 79.328916, 80.192878, 81.035764, 81.867925, 82.648501, 83.448066,
    84.225460, 84.980274, 85.749780, 86.460708, 87.146717, 87.699085, 88.231304, 88.704209,
    89.044956, 89.291320, 89.491618, 89.553345, 89.600369, 89.602910, 89.562235, 89.479571,
    89.319477, 89.116803, 88.834060, 88.581847, 88.289087, 87.987472, 87.714922, 87.521623,
    87.267702, 87.082754, 86.841231, 86.634993, 86.369036, 86.046146, 85.701619, 85.433604,
    85.185300, 84.918488, 84.641859, 84.447106, 84.187035, 83.874316, 83.601285, 83.313997,
    83.008313, 82.647033, 82.379123, 82.044617, 81.748788, 81.393990, 81.077700, 80.908459,
    80.573442, 80.118725, 79.751947, 79.488801, 79.113666, 78.715040, 78.359093, 77.990797,
    77.615992, 77.229320, 76.830274, 76.418863, 75.995102, 75.386595, 74.919704, 74.682407,
    74.274555, 73.738544, 73.224784, 72.758434, 72.278498, 71.785072, 71.218117, 70.673097,
    70.113430, 69.539261, 68.950799, 68.380449, 67.796867, 67.016464, 66.379890, 65.657674,
    64.986791, 64.301944, 63.528857, 62.742109, 61.864790, 61.040865, 60.123833, 59.193538,
    58.167400, 57.159637, 56.052788, 54.932490, 53.799169, 52.724999, 51.638891, 50.541342,
    49.968089, 49.307831, 48.555194, 48.237110, 47.917091, 47.357637, 47.237487, 46.961555
};

ART_MODULE_INITIALISATION_FUNCTION
(
    ARCIECV_GV = ALLOC(ArCIEColourConversions_GV);

    pthread_mutex_init( & ARCIECV_MUTEX, NULL );
 
#ifndef _ART_WITHOUT_LCMS_
    ARCIECV_GM_METHOD     = arrgb_gm_lcms;
#else
    ARCIECV_GM_METHOD     = arrgb_gm_linear;
#endif
    ARCIECV_GM_RECDEPTH   = 20;
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
        const ArRGBGamutMapping    method
        )
{
    pthread_mutex_lock( & ARCIECV_MUTEX );
    
    ARCIECV_GM_METHOD = method;

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
            xyz_move2gamut(
                  art_gv,
                & midpointXYZ,
                  insideXYZ,
                  xyz2rgb,
                  depth - 1,
                  finalRGB
                );
        else
            xyz_move2gamut(
                  art_gv,
                  outsideXYZ,
                & midpointXYZ,
                  xyz2rgb,
                  depth-1,
                  finalRGB
                );
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

void lab_move2unit_gamut(
        const ART_GV    * art_gv,
        const ArCIELab  * outsideLab,
        const ArCIELab  * insideLab,
        const ArCIEXYZ  * rgbWhitepoint,
        const Mat3      * xyz2rgb,
        const int         depth,
              ArRGB     * finalRGB
        )
{
    ArCIELab  midpointLab;
    ArRGB     midpointRGB;
    
    ARCIELab_L(midpointLab) = ( ARCIELab_L(*outsideLab) + ARCIELab_L(*insideLab) ) / 2.0;
    ARCIELab_a(midpointLab) = ( ARCIELab_a(*outsideLab) + ARCIELab_a(*insideLab) ) / 2.0;
    ARCIELab_b(midpointLab) = ( ARCIELab_b(*outsideLab) + ARCIELab_b(*insideLab) ) / 2.0;
    
    ArCIEXYZ  midpointXYZ;
    
    lab_wp_to_xyz(
          art_gv,
        & midpointLab,
          rgbWhitepoint,
        & midpointXYZ
        );
    
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
            lab_move2unit_gamut(
                  art_gv,
                & midpointLab,
                  insideLab,
                  rgbWhitepoint,
                  xyz2rgb,
                  depth - 1,
                  finalRGB
                );
        else
            lab_move2unit_gamut(
                  art_gv,
                  outsideLab,
                & midpointLab,
                  rgbWhitepoint,
                  xyz2rgb,
                  depth-1,
                  finalRGB
                );
    }
    else
    {
        if ( XC(midpointRGB)<0.0 || YC(midpointRGB)<0.0 || ZC(midpointRGB)<0.0)
        {
            ArCIEXYZ  insideXYZ;
            
            lab_wp_to_xyz(
                  art_gv,
                  insideLab,
                  rgbWhitepoint,
                & insideXYZ
                );
    
            xyz_mat_to_rgb(
                  art_gv,
                & insideXYZ,
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

        //   We only do the whole "move to gamut" thing if we are not clipping
        
        if ( ( ARCIECV_GM_METHOD & arrgb_gm_technique_mask ) != arrgb_gm_clipping )
        {
            //   Nothing to do if we are already in gamut -- all ART gamut
            //   mapping strategies except clipping are absolute colorimetric, with
            //   variations on the trajectory that the out of gamut colours take towards
            //   the gamut.

            if (   ARRGB_R(*rgb_r) < 0. || ARRGB_R(*rgb_r) > 1.
                || ARRGB_G(*rgb_r) < 0. || ARRGB_G(*rgb_r) > 1.
                || ARRGB_B(*rgb_r) < 0. || ARRGB_B(*rgb_r) > 1. )
            {
                ArCIELab  lab_0;
                
                xyz_wp_to_lab( art_gv, xyz_0, & ARCIEXYZ_D65, & lab_0 );
                
                double  focus_L = 50.;
                
                if ( ( ARCIECV_GM_METHOD & arrgb_gm_technique_mask ) == arrgb_gm_linear )
                {
                    //   here we just move horizontally, no change in luminance
                    focus_L = M_CLAMP( ARCIELab_L(lab_0), 0.0, 100.0 );
                }
                    
                if ( ( ARCIECV_GM_METHOD & arrgb_gm_technique_mask ) == arrgb_gm_node )
                {
                    //   nothing to do here, focus L = 50. is already the default value.
                }
                    
                if ( ( ARCIECV_GM_METHOD & arrgb_gm_technique_mask ) == arrgb_gm_cusp )
                {
                    
                    ArCIELch  lch_0;
                    
                    lab_to_lch( art_gv, & lab_0, &  lch_0 );
                    
                    //  the clamp is just there for safety reasons, normally this
                    //  should behave itself
                    
                    int  angle = M_CLAMP( ARCIELch_h(lch_0) * MATH_RAD_TO_DEG, 0, 359 );

                    focus_L = ARCIECV_GM_FOCUS[angle];
                }
                    
                ArCIELab  inside_lab =
                    ARCIELab(
                        focus_L,
                        0.,
                        0.
                        );
                
                lab_move2unit_gamut(
                      art_gv,
                    & lab_0,
                    & inside_lab,
                    & ARCIEXYZ_D65,
                    & ARCSR_XYZ_TO_RGB(DEFAULT_RGB_SPACE_REF),
                      ARCIECV_GM_RECDEPTH,
                      rgb_r
                    );
                
                goto gammacorrection;
            }
        }
        
        //   If we get here, clamping will be applied. But if the user requested
        //   flagging of out of gamut pixels, this will be done first.

        int  flags = 0;
        
        if ( ( ARCIECV_GM_METHOD & arrgb_gm_feature_mask ) & arrgb_gm_flag_neg )
        {
            if (   ARRGB_R(*rgb_r) < 0.
                || ARRGB_G(*rgb_r) < 0.
                || ARRGB_B(*rgb_r) < 0. )
            {
                ARRGB_C(*rgb_r) = ARCIECV_NEG_FLAG_RGB.c;
                flags++;
            }
        }

        if ( ( ARCIECV_GM_METHOD & arrgb_gm_feature_mask ) & arrgb_gm_flag_above_one )
        {
            if (   ARRGB_R(*rgb_r) > 1.
                || ARRGB_G(*rgb_r) > 1.
                || ARRGB_B(*rgb_r) > 1. )
            {
                ARRGB_C(*rgb_r) = ARCIECV_POS_FLAG_RGB.c;
                flags++;
            }
        }
        
        if ( flags == 2 )
        {
            ARRGB_C(*rgb_r) = ARCIECV_BOTH_FLAG_RGB.c;
        }
        
        rgb_dd_clamp_c( art_gv, 0.0, 1.0, rgb_r );

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

void lch_to_lab(
        const ART_GV    * art_gv,
        const ArCIELch  * lch,
              ArCIELab  * lab
        )
{
    (void)art_gv;
    
    ARCIELab_L(*lab) = ARCIELch_L(*lch);
    //  minus pi because we want the hue angle to be [0..360], not [-180..180]
    ARCIELab_a(*lab) = cos( ARCIELch_h(*lch) - MATH_PI ) * ARCIELch_c(*lch);
    ARCIELab_b(*lab) = sin( ARCIELch_h(*lch) - MATH_PI ) * ARCIELch_c(*lch);
}

void lab_to_lch(
        const ART_GV    * art_gv,
        const ArCIELab  * lab,
              ArCIELch  * lch
        )
{
    (void)art_gv;

    ARCIELch_L(*lch) = ARCIELab_L(*lab);
    ARCIELch_c(*lch) = sqrt( M_SQR(ARCIELab_b(*lab)) + M_SQR(ARCIELab_a(*lab)) );
    //  plus pi because we want the hue angle to be [0..360], not [-180..180]
    ARCIELch_h(*lch) = atan2( ARCIELab_b(*lab), ARCIELab_a(*lab) ) + MATH_PI;
}


/* ======================================================================== */
