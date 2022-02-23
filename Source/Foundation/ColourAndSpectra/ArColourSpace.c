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

#define ART_MODULE_NAME     ArColourSpace

#include "ArColourSpace.h"
#include "ArRGB.h"
#include "ArCIExyY.h"
#include "ArCIEXYZ.h"

//   uncomment the following line to get debug output for each ICC profile
//   that is loaded

//#define ARCOLOURSPACE_DEBUGPRINTF_ON_LOADING

#include <stdio.h>
#include <pthread.h>

typedef struct ArColourSpace_GV
{
    ArTable           * table;
    pthread_mutex_t     mutex;

    ArColourSpaceRef    CIEXYZ;
    ArColourSpaceRef    CIExyY;
    ArColourSpaceRef    CIELab;
    ArColourSpaceRef    CIELuv;
    ArColourSpaceRef    sRGB;
    ArColourSpaceRef    aRGB;
    ArColourSpaceRef    wRGB;
    ArColourSpaceRef    ap0RGB;
    ArColourSpaceRef    ap1RGB;
}
ArColourSpace_GV;

#define ARCS_GV         art_gv->arcolourspace_gv
#define ARCS_MUTEX      ARCS_GV->mutex
#define ARCS_TABLE      ARCS_GV->table
#define ARCS_XYZ        ARCS_GV->CIEXYZ
#define ARCS_XYY        ARCS_GV->CIExyY
#define ARCS_LAB        ARCS_GV->CIELab
#define ARCS_LUV        ARCS_GV->CIELuv
#define ARCS_SRGB       ARCS_GV->sRGB
#define ARCS_ARGB       ARCS_GV->aRGB
#define ARCS_WRGB       ARCS_GV->wRGB
#define ARCS_AP0RGB     ARCS_GV->ap0RGB
#define ARCS_AP1RGB     ARCS_GV->ap1RGB

#ifndef _ART_WITHOUT_LCMS_
void initLCMSProfileBuffer(
              cmsHPROFILE        profile,
              cmsUInt32Number  * profileBufferSize,
              cmsUInt8Number  ** profileBuffer
        )
{
    cmsSaveProfileToMem(
          profile,
          NULL,
          profileBufferSize
        );

    *profileBuffer = ALLOC_ARRAY(cmsUInt8Number, *profileBufferSize);

    cmsSaveProfileToMem(
          profile,
         *profileBuffer,
          profileBufferSize
        );
}

void setICCProfileDescription(
              cmsHPROFILE    profile,
        const char         * description
        )
{
    char  copyrightString[2048];
    
    sprintf(
        copyrightString,
        "Public Domain, created by the %s %s via littlecms 2.x routines",
        art_long_name_string,
        art_version_string
        );

    cmsMLU *mlu0 = cmsMLUalloc(NULL, 1);
    cmsMLUsetASCII(mlu0, "en", "US", copyrightString);
    cmsMLU *mlu1 = cmsMLUalloc(NULL, 1);
    cmsMLUsetASCII(mlu1, "en", "US", description);
    cmsWriteTag( profile, cmsSigCopyrightTag,          mlu0);
    cmsWriteTag( profile, cmsSigProfileDescriptionTag, mlu1);
    cmsMLUfree(mlu0);
    cmsMLUfree(mlu1);
}

void createLCMSProfileFromARTColours(
              ArColourSpace    * cs,
        const char             * description,
        const double             gamma,
              cmsHPROFILE      * profile
        )
{
    cmsCIExyY lcms_white_xyy;

    lcms_white_xyy.x = XC(ARCS_W(*cs));
    lcms_white_xyy.y = YC(ARCS_W(*cs));
    lcms_white_xyy.Y = 1.0;

    cmsToneCurve  * gammacurve = cmsBuildGamma(NULL, gamma);
    cmsToneCurve  * gamma_rgb[3];

    gamma_rgb[0] = gammacurve;
    gamma_rgb[1] = gammacurve;
    gamma_rgb[2] = gammacurve;
 
    cmsCIExyYTRIPLE rgb_primaries;
 
    rgb_primaries.Red.x = XC(ARCS_R(*cs));
    rgb_primaries.Red.y = YC(ARCS_R(*cs));
    rgb_primaries.Red.Y = 1.0;

    rgb_primaries.Green.x = XC(ARCS_G(*cs));
    rgb_primaries.Green.y = YC(ARCS_G(*cs));
    rgb_primaries.Green.Y = 1.0;

    rgb_primaries.Blue.x = XC(ARCS_B(*cs));
    rgb_primaries.Blue.y = YC(ARCS_B(*cs));
    rgb_primaries.Blue.Y = 1.0;

    *profile =
        cmsCreateRGBProfile(
            & lcms_white_xyy,
            & rgb_primaries,
              gamma_rgb
            );
    
    setICCProfileDescription( *profile, description );
}

void createCompleteLCMSProfileFromARTColours(
              ArColourSpace    * cs,
        const char             * description,
        const double             gamma,
              cmsHPROFILE      * profile,
              cmsUInt32Number  * profileBufferSize,
              cmsUInt8Number  ** profileBuffer
        )
{
    createLCMSProfileFromARTColours(
          cs,
          description,
          gamma,
          profile
        );
    
    initLCMSProfileBuffer(
         *profile,
          profileBufferSize,
          profileBuffer
        );
}

#endif

void calculateColourspaceMatrices(
              ART_GV         * art_gv,
              ArColourSpace  * cs
        )
{
    ARCS_XYZ_TO_RGB( *cs ) =
        xyz2rgb_via_primaries(
              art_gv,
            & ARCS_R(*cs),
            & ARCS_G(*cs),
            & ARCS_B(*cs),
            & ARCS_W(*cs)
            );
 
    double  det = c3_m_det( & ARCS_XYZ_TO_RGB( *cs  ) );

    c3_md_invert_m(
        & ARCS_XYZ_TO_RGB( *cs  ),
          det,
        & ARCS_RGB_TO_XYZ( *cs  )
        );
}

#define TYPE                        ARCS_TYPE(cs)
#define NAME                        ARCS_NAME(cs)

#define RED                         ARCS_R(cs)
#define GREEN                       ARCS_G(cs)
#define BLUE                        ARCS_B(cs)
#define WHITE                       ARCS_W(cs)
#define GAMMA                       ARCS_GAMMA(cs)
#define GAMMAFUNCTION               cs.gammafunction
#define INV_GAMMAFUNCTION           cs.inv_gammafunction

#define XYZ_TO_RGB                  ARCS_XYZ_TO_RGB(cs)
#define RGB_TO_XYZ                  ARCS_RGB_TO_XYZ(cs)

#define CALCULATE_MATRICES  calculateColourspaceMatrices( art_gv, & cs );

#ifndef _ART_WITHOUT_LCMS_
#define PROFILE                     ARCS_PROFILE(cs)
#define LINEAR_PROFILE              ARCS_LINEAR_PROFILE(cs)
#define XYZ_TO_RGB_TRAFO            ARCS_XYZ_TO_RGB_TRAFO(cs)
#define XYZ_TO_LINEAR_RGB_TRAFO     ARCS_XYZ_TO_LINEAR_RGB_TRAFO(cs)

#define CREATE_LCMS_PROFILE(__desc) \
    createCompleteLCMSProfileFromARTColours( \
        & cs, \
          (__desc), \
          ARCS_GAMMA(cs), \
        & ARCS_PROFILE(cs), \
        & ARCS_PROFILEBUFFERSIZE(cs), \
        & ARCS_PROFILEBUFFER(cs) \
        );

#define CREATE_LINEAR_LCMS_PROFILE(__desc) \
    createCompleteLCMSProfileFromARTColours( \
        & cs, \
          (__desc), \
          1.0, \
        & ARCS_LINEAR_PROFILE(cs), \
        & ARCS_LINEAR_PROFILEBUFFERSIZE(cs), \
        & ARCS_LINEAR_PROFILEBUFFER(cs) \
        );

#define CREATE_LCMS_TRANSFORMS \
    XYZ_TO_RGB_TRAFO = \
        cmsCreateTransform( \
              xyz_profile, \
              TYPE_XYZ_DBL, \
              PROFILE, \
              TYPE_RGB_DBL, \
              INTENT_RELATIVE_COLORIMETRIC, \
              cmsFLAGS_BLACKPOINTCOMPENSATION | cmsFLAGS_NOOPTIMIZE \
              ); \
\
    XYZ_TO_LINEAR_RGB_TRAFO = \
        cmsCreateTransform( \
              xyz_profile, \
              TYPE_XYZ_DBL, \
              LINEAR_PROFILE, \
              TYPE_RGB_DBL, \
              INTENT_RELATIVE_COLORIMETRIC, \
              cmsFLAGS_BLACKPOINTCOMPENSATION | cmsFLAGS_NOOPTIMIZE \
              );

#endif

ART_MODULE_INITIALISATION_FUNCTION
(
    ARCS_GV = ALLOC(ArColourSpace_GV);

    pthread_mutex_init( & ARCS_MUTEX, NULL );

    ARCS_TABLE = artable_alloc_init();

    ArCIExy  d50_xy = ARCIExy(0.3457,0.3585);
    ArCIExy  d65_xy = ARCIExy(0.3127,0.3290);

    ArColourSpace  cs;
 
    //   ------  CIE XYZ   ------------------------------------------------

    TYPE = arcolourspacetype_ciexyz;
    NAME = arsymbol( art_gv, "CIE XYZ" );
 
    RED   = ARCIExy(1,0);
    GREEN = ARCIExy(0,1);
    BLUE  = ARCIExy(0,0);
    WHITE = d50_xy;
    GAMMA = 1.0;

    XYZ_TO_RGB =
        MAT3(  1.0, 0.0, 0.0,
               0.0, 1.0, 0.0,
               0.0, 0.0, 1.0 );
    RGB_TO_XYZ =
        MAT3(  1.0, 0.0, 0.0,
               0.0, 1.0, 0.0,
               0.0, 0.0, 1.0 );
 
#ifndef _ART_WITHOUT_LCMS_
    cmsHPROFILE  xyz_profile = cmsCreateXYZProfile();
    PROFILE = xyz_profile;
    LINEAR_PROFILE = xyz_profile;
#endif

    ARCS_XYZ = register_arcolourspace( art_gv, & cs );

    //   ------  CIE xyY   ------------------------------------------------

    TYPE = arcolourspacetype_ciexyy;
    NAME = arsymbol( art_gv, "CIE xyY" );
    ARCS_W(cs) = d50_xy;
    ARCS_XYY = register_arcolourspace( art_gv, & cs );

    //   ------  CIE L*a*b*   ---------------------------------------------

    TYPE = arcolourspacetype_cielab;
    NAME = arsymbol( art_gv, "CIE L*a*b*" );
    ARCS_W(cs) = d50_xy;
#ifndef _ART_WITHOUT_LCMS_
    ARCS_PROFILE(cs) = cmsCreateLab4Profile( 0 );
#endif
    ARCS_LAB = register_arcolourspace( art_gv, & cs );

    //   ------  CIE L*u*v*   ---------------------------------------------

    TYPE = arcolourspacetype_cieluv;
    NAME = arsymbol( art_gv, "CIE L*u*v*" );
    ARCS_W(cs) = d50_xy;
    ARCS_LUV = register_arcolourspace( art_gv, & cs );

    //   ------  sRGB   ---------------------------------------------------

    TYPE = arcolourspacetype_rgb;
    NAME = arsymbol( art_gv, "sRGB" );

    RED   = ARCIExy(.64,.33);
    GREEN = ARCIExy(.30,.60);
    BLUE  = ARCIExy(.15,.06);
    WHITE = d65_xy;
    GAMMA = 2.2;

    GAMMAFUNCTION = arcolourspace_srgb_gamma;
    INV_GAMMAFUNCTION = arcolourspace_srgb_inv_gamma;

    CALCULATE_MATRICES;
 
#ifndef _ART_WITHOUT_LCMS_

    PROFILE = cmsCreate_sRGBProfile();

    setICCProfileDescription( PROFILE, "sRGB" );

    initLCMSProfileBuffer(
          ARCS_PROFILE(cs),
        & ARCS_PROFILEBUFFERSIZE(cs),
        & ARCS_PROFILEBUFFER(cs)
        );
        
    CREATE_LINEAR_LCMS_PROFILE("linear Rec. 709");

    CREATE_LCMS_TRANSFORMS;
#endif

    ARCS_SRGB = register_arcolourspace( art_gv, & cs );

    //   ------  Adobe RGB (1998)  ----------------------------------------

    TYPE = arcolourspacetype_rgb;
    NAME = arsymbol( art_gv, "Adobe RGB (1998)" );

    RED   = ARCIExy(.64,.33);
    GREEN = ARCIExy(.21,.71);
    BLUE  = ARCIExy(.15,.06);
    WHITE = d65_xy;
    GAMMA = 563. / 256.;

    GAMMAFUNCTION = arcolourspace_standard_gamma;
    INV_GAMMAFUNCTION = arcolourspace_standard_inv_gamma;

    CALCULATE_MATRICES;
 
#ifndef _ART_WITHOUT_LCMS_

    CREATE_LCMS_PROFILE("Adobe RGB (1998)");
    CREATE_LINEAR_LCMS_PROFILE("linear Adobe RGB (1998)");
    CREATE_LCMS_TRANSFORMS;

#endif

    ARCS_ARGB = register_arcolourspace( art_gv, & cs );

    //   ------  Adobe Wide Gamut RGB -------------------------------------

    TYPE = arcolourspacetype_rgb;
    NAME = arsymbol( art_gv, "Adobe Wide Gamut RGB" );

    RED   = ARCIExy(0.7347,0.2653);
    GREEN = ARCIExy(0.1152,0.8264);
    BLUE  = ARCIExy(0.1566,0.0177);
    WHITE = d50_xy;
    GAMMA = 563. / 256.;

    GAMMAFUNCTION = arcolourspace_standard_gamma;
    INV_GAMMAFUNCTION = arcolourspace_standard_inv_gamma;

    CALCULATE_MATRICES;
 
#ifndef _ART_WITHOUT_LCMS_

    CREATE_LCMS_PROFILE("Adobe Wide Gamut RGB");
    CREATE_LINEAR_LCMS_PROFILE("linear Adobe Wide Gamut RGB");
    CREATE_LCMS_TRANSFORMS;

#endif

    ARCS_WRGB = register_arcolourspace( art_gv, & cs );

    //   ------  ACES AP0 RGB ---------------------------------------------

    TYPE = arcolourspacetype_rgb;
    NAME = arsymbol( art_gv, "ACES AP0 RGB" );

    RED   = ARCIExy( 0.73470,  0.26530 );
    GREEN = ARCIExy( 0.00000,  1.00000 );
    BLUE  = ARCIExy( 0.00010, -0.07700 );
    WHITE = ARCIExy( 0.32168,  0.33767 );
    GAMMA = 563. / 256.;

    GAMMAFUNCTION = arcolourspace_standard_gamma;
    INV_GAMMAFUNCTION = arcolourspace_standard_inv_gamma;

    CALCULATE_MATRICES;
 
#ifndef _ART_WITHOUT_LCMS_

    CREATE_LCMS_PROFILE("ACES AP0 RGB");
    CREATE_LINEAR_LCMS_PROFILE("linear ACES AP0 RGB");
    CREATE_LCMS_TRANSFORMS;

#endif

    ARCS_AP0RGB = register_arcolourspace( art_gv, & cs );

    //   ------  ACES AP1 RGB ---------------------------------------------

    TYPE = arcolourspacetype_rgb;
    NAME = arsymbol( art_gv, "ACES AP1 RGB" );

    RED   = ARCIExy( 0.71300, 0.29300 );
    GREEN = ARCIExy( 0.16500, 0.83000 );
    BLUE  = ARCIExy( 0.12800, 0.04400 );
    WHITE = ARCIExy( 0.32168, 0.33767 );
    GAMMA = 563. / 256.;

    GAMMAFUNCTION = arcolourspace_standard_gamma;
    INV_GAMMAFUNCTION = arcolourspace_standard_inv_gamma;

    CALCULATE_MATRICES;
 
#ifndef _ART_WITHOUT_LCMS_

    CREATE_LCMS_PROFILE("ACES AP1 RGB");
    CREATE_LINEAR_LCMS_PROFILE("linear ACES AP1 RGB");
    CREATE_LCMS_TRANSFORMS;

#endif

    ARCS_AP1RGB = register_arcolourspace( art_gv, & cs );
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    artable_free( art_gv->arcolourspace_gv->table );

    pthread_mutex_destroy(
        & art_gv->arcolourspace_gv->mutex
        );

    FREE( art_gv->arcolourspace_gv );
)

double arcolourspace_standard_gamma(
        const double  gamma,
        const double  value
        )
{
    return m_dd_pow( value, 1 / gamma);
}

double arcolourspace_standard_inv_gamma(
        const double  gamma,
        const double  value
        )
{
    return m_dd_pow( value, gamma);
}

double arcolourspace_srgb_gamma(
        const double  gamma,
        const double  value
        )
{
    double  a      = 0.055;
    double  result = value;

    if ( result < 0.0031308 )
    {
        result *= 12.92;
    }
    else
    {
        result = ( 1 + a ) * m_dd_pow(result, 1 / gamma) - a;
    }
    
    return result;
}

double arcolourspace_srgb_inv_gamma(
        const double  gamma,
        const double  value
        )
{
    double  a      = 0.055;
    double  result = value;

    if ( result < 0.04045 )
    {
        result /= 12.92;
    }
    else
    {
        result = m_dd_pow((result+a)/(1.+a), gamma);
    }
    
    return result;
}

ArColourSpace const * arcolourspace_CIEXYZ(
        const ART_GV  * art_gv
        )
{
    return art_gv->arcolourspace_gv->CIEXYZ;
}

ArColourSpace const * arcolourspace_CIExyY(
        const ART_GV  * art_gv
        )
{
    return art_gv->arcolourspace_gv->CIExyY;
}

ArColourSpace const * arcolourspace_CIELab(
        const ART_GV  * art_gv
        )
{
    return art_gv->arcolourspace_gv->CIELab;
}

ArColourSpace const * arcolourspace_CIELuv(
        const ART_GV  * art_gv
        )
{
    return art_gv->arcolourspace_gv->CIELuv;
}

ArColourSpace const * arcolourspace_sRGB(
        const ART_GV  * art_gv
        )
{
    return ARCS_SRGB;
}

ArColourSpace const * arcolourspace_aRGB(
        const ART_GV  * art_gv
        )
{
    return ARCS_ARGB;
}

ArColourSpace const * arcolourspace_wRGB(
        const ART_GV  * art_gv
        )
{
    return ARCS_WRGB;
}

ArColourSpace const * arcolourspace_ap0RGB(
        const ART_GV  * art_gv
        )
{
    return ARCS_AP0RGB;
}

ArColourSpace const * arcolourspace_ap1RGB(
        const ART_GV  * art_gv
        )
{
    return ARCS_AP1RGB;
}

ArColourSpaceRef register_arcolourspace(
        ART_GV         * art_gv,
        ArColourSpace  * newCS
        )
{
    ArColourSpaceRef  newRef = 0;

    if ( newCS && ARCOLOURSPACE_NAME(*newCS) )
    {
        pthread_mutex_lock( & art_gv->arcolourspace_gv->mutex );
        newRef = artable_add_cs( art_gv->arcolourspace_gv->table, *newCS );
        pthread_mutex_unlock( & art_gv->arcolourspace_gv->mutex );
    }

    return newRef;
}

ArColourSpaceRef arcolourspaceref_for_csname(
        const ART_GV    * art_gv,
        const ArSymbol    name
        )
{
    return artable_get_cs_with_key( art_gv->arcolourspace_gv->table, name );
}

#ifndef _ART_WITHOUT_LCMS_

ArColourSpaceRef create_and_register_arcolourspace_from_icc(
        ART_GV       * art_gv,
        cmsHPROFILE    profile
        )
{
    char  profileInfoDescriptionBuffer[256];

    cmsGetProfileInfoASCII(
        profile,
        cmsInfoDescription,
        "en",
        "US",
        profileInfoDescriptionBuffer,
        256
        );

    //   Sanity check #1 - we can only use monitor or colour space profiles as input

    if ( ! (   ( cmsGetDeviceClass( profile ) == cmsSigDisplayClass )
            || ( cmsGetDeviceClass( profile ) == cmsSigColorSpaceClass ) ) )
    {
        ART_ERRORHANDLING_FATAL_ERROR(
            "ICC profile '%s' is neither a monitor nor colour space profile"
            ,   profileInfoDescriptionBuffer
            );

        return 0;
    }

    ArColourSpace  cs;

    ARCS_PROFILEBUFFERSIZE(cs) = 0;
    ARCS_PROFILEBUFFER(cs) = 0;
    ARCS_RGB_TO_XYZ(cs) = C3_M_UNIT;
    ARCS_XYZ_TO_RGB(cs) = C3_M_UNIT;

    //   Sanity check #2 - inspection of the colour space the profile uses

    switch ( cmsGetColorSpace( profile ) )
    {
        case cmsSigXYZData:
            ARCS_TYPE(cs) = arcolourspacetype_ciexyz;
            break;

        case cmsSigLabData:
            ARCS_TYPE(cs) = arcolourspacetype_cielab;
            break;

        case cmsSigLuvData:
            ARCS_TYPE(cs) = arcolourspacetype_cieluv;
            break;

        case cmsSigYxyData:
            ARCS_TYPE(cs) = arcolourspacetype_ciexyy;
            break;

        case cmsSigRgbData:
            ARCS_TYPE(cs) = arcolourspacetype_rgb;
            break;

        default:
            ARCS_TYPE(cs) = arcolourspacetype_none;

            ART_ERRORHANDLING_FATAL_ERROR(
                "ICC profile '%s' is for an unsupported colour space"
                ,   profileInfoDescriptionBuffer
                );

            break;
    }

    ARCS_NAME(cs) = arsymbol( art_gv, profileInfoDescriptionBuffer );

    if ( ARCS_TYPE(cs) == arcolourspacetype_rgb )
    {
        //   Sanity check #3 - if this is a RGB space, it has to be a matrix shaper
        //                     for us to be able to extract the transform matrices

        if ( ! cmsIsMatrixShaper( profile ) )
            ART_ERRORHANDLING_FATAL_ERROR(
                "ICC profile '%s' contains a non-matrix shaper RGB space"
                ,   ARCS_NAME(cs)
                );

        //   Extraction of RGB primaries from profile

        cmsCIEXYZ  * primary_R;
        cmsCIEXYZ  * primary_G;
        cmsCIEXYZ  * primary_B;

        primary_R = (cmsCIEXYZ*) cmsReadTag( profile, cmsSigRedMatrixColumnTag );
        primary_G = (cmsCIEXYZ*) cmsReadTag( profile, cmsSigGreenMatrixColumnTag );
        primary_B = (cmsCIEXYZ*) cmsReadTag( profile, cmsSigBlueMatrixColumnTag );

        //   Assembly of RGB -> XYZ transform matrix from primaries

        ARCS_RGB_TO_XYZ(cs) =
            MAT3(
                primary_R->X, primary_G->X, primary_B->X,
                primary_R->Y, primary_G->Y, primary_B->Y,
                primary_R->Z, primary_G->Z, primary_B->Z
                );

       c3_transpose_m( & ARCS_RGB_TO_XYZ(cs) );

        //   The XYZ -> RGB matrix is the inverse of the RGB -> XYZ matrix

        double  det = c3_m_det( & ARCS_RGB_TO_XYZ(cs) );

        c3_md_invert_m(
            & ARCS_RGB_TO_XYZ(cs),
              det,
            & ARCS_XYZ_TO_RGB(cs)
            );

        //   Estimation of the RGB colour space gamma; for the sake of simplicity
        //   we just use the green gamma curve

        cmsToneCurve * greenToneCuve =
            (cmsToneCurve *) cmsReadTag( profile, cmsSigGreenTRCTag );

        ARCS_GAMMA(cs) = cmsEstimateGamma( greenToneCuve, 1.0 );

#ifdef ARCOLOURSPACE_DEBUGPRINTF_ON_LOADING
        ArCIEXYZ  xyz = ARCIEXYZ(whitepoint.X,whitepoint.Y,whitepoint.Z);
        ArCIExyY  xyy;

        printf("\nLoaded ICC profile '%s'\n", cmsTakeProductName( profile ) );
        printf("WHITE: ");xyz_c_debugprintf( & xyz );
        printf("       ");xyz_to_xyy( & xyz, & xyy ); xyy_c_debugprintf( & xyy );
        xyz = ARCIEXYZ(illuminant.X,illuminant.Y,illuminant.Z);
        printf("ILLUM: ");xyz_c_debugprintf( & xyz ); xyz_to_xyy( & xyz, & xyy );
        printf("       ");xyy_c_debugprintf( & xyy );
        xyz = ARCIEXYZ(primaries.Red.X,primaries.Red.Y,primaries.Red.Z);
        printf("RED  : ");xyz_c_debugprintf( & xyz ); xyz_to_xyy( & xyz, & xyy );
        printf("       ");xyy_c_debugprintf( & xyy );
        xyz = ARCIEXYZ(primaries.Green.X,primaries.Green.Y,primaries.Green.Z);
        printf("GREEN: ");xyz_c_debugprintf( & xyz ); xyz_to_xyy( & xyz, & xyy );
        printf("       ");xyy_c_debugprintf( & xyy );
        xyz = ARCIEXYZ(primaries.Blue.X,primaries.Blue.Y,primaries.Blue.Z);
        printf("BLUE : ");xyz_c_debugprintf( & xyz ); xyz_to_xyy( & xyz, & xyy );
        printf("       ");xyy_c_debugprintf( & xyy );
        printf("GAMMA: %f\n",ARCS_GAMMA(cs));

        printf("\n"C3_M_FORMAT("%6.5f") "\n",C3_M_PRINTF(ARCS_RGB_TO_XYZ(cs)));fflush(stdout);
        printf(C3_M_FORMAT("%6.5f") "\n",C3_M_PRINTF(ARCS_XYZ_TO_RGB(cs)));fflush(stdout);
#endif
    }
    else
        ARCS_GAMMA(cs) = 1.0;

    ARCS_PROFILE(cs) = profile;

    ArColourSpaceRef  csr = register_arcolourspace( art_gv, & cs );

#ifdef ARCOLOURSPACE_DEBUGPRINTF_ON_LOADING
    arcolourspace_debugprintf( csr );
#endif

    return csr;
}

#endif

ARTABLE_IMPLEMENTATION_FOR_STRUCTURE_WITH_ARSYMBOL_INDEX_FIELD(
        ArColourSpace,
        cs,
        name
        )

void arcolourspace_debugprintf(
        ART_GV            * art_gv,
        ArColourSpaceRef    csr
        )
{
    (void) art_gv;
    (void) csr;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
/*
        ArCIEXYZ  xyz = ARCIEXYZ(whitepoint.X,whitepoint.Y,whitepoint.Z);
        ArCIExyY  xyy;

        printf("\nLoaded ICC profile '%s'\n", cmsTakeProductName( profile ) );
        printf("WHITE: ");xyz_c_debugprintf( & xyz );
        printf("       ");xyz_to_xyy( & xyz, & xyy ); xyy_c_debugprintf( & xyy );
        xyz = ARCIEXYZ(illuminant.X,illuminant.Y,illuminant.Z);
        printf("ILLUM: ");xyz_c_debugprintf( & xyz ); xyz_to_xyy( & xyz, & xyy );
        printf("       ");xyy_c_debugprintf( & xyy );
        xyz = ARCIEXYZ(primaries.Red.X,primaries.Red.Y,primaries.Red.Z);
        printf("RED  : ");xyz_c_debugprintf( & xyz ); xyz_to_xyy( & xyz, & xyy );
        printf("       ");xyy_c_debugprintf( & xyy );
        xyz = ARCIEXYZ(primaries.Green.X,primaries.Green.Y,primaries.Green.Z);
        printf("GREEN: ");xyz_c_debugprintf( & xyz ); xyz_to_xyy( & xyz, & xyy );
        printf("       ");xyy_c_debugprintf( & xyy );
        xyz = ARCIEXYZ(primaries.Blue.X,primaries.Blue.Y,primaries.Blue.Z);
        printf("BLUE : ");xyz_c_debugprintf( & xyz ); xyz_to_xyy( & xyz, & xyy );
        printf("       ");xyy_c_debugprintf( & xyy );

        printf("\n"C3_M_FORMAT("%6.5f") "\n",C3_M_PRINTF(ARCS_RGB_TO_XYZ(cs)));fflush(stdout);
        printf(C3_M_FORMAT("%6.5f") "\n",C3_M_PRINTF(ARCS_XYZ_TO_RGB(cs)));fflush(stdout);
*/
}

#include "ArCIEColourConversions.h"

Mat3  xyz2rgb_via_primaries(
              ART_GV   * art_gv,
        const ArCIExy  * r,
        const ArCIExy  * g,
        const ArCIExy  * b,
        const ArCIExy  * w
        )
{
    ArCIEXYZ r_xyz, g_xyz, b_xyz, w_xyz;
    
    ARCIEXYZ_X(r_xyz) = XC(*r)/YC(*r);
    ARCIEXYZ_Y(r_xyz) = 1;
    ARCIEXYZ_Z(r_xyz) = (1-XC(*r)-YC(*r))/YC(*r);
    
    ARCIEXYZ_X(g_xyz) = XC(*g)/YC(*g);
    ARCIEXYZ_Y(g_xyz) = 1;
    ARCIEXYZ_Z(g_xyz) = (1-XC(*g)-YC(*g))/YC(*g);
    
    ARCIEXYZ_X(b_xyz) = XC(*b)/YC(*b);
    ARCIEXYZ_Y(b_xyz) = 1;
    ARCIEXYZ_Z(b_xyz) = (1-XC(*b)-YC(*b))/YC(*b);
    
    ARCIEXYZ_X(w_xyz) = XC(*w)/YC(*w);
    ARCIEXYZ_Y(w_xyz) = 1;
    ARCIEXYZ_Z(w_xyz) = (1-XC(*w)-YC(*w))/YC(*w);
    
    Mat3  rgb2xyz =
        MAT3( ARCIEXYZ_X(r_xyz), ARCIEXYZ_X(g_xyz), ARCIEXYZ_X(b_xyz),
              ARCIEXYZ_Y(r_xyz), ARCIEXYZ_Y(g_xyz), ARCIEXYZ_Y(b_xyz),
              ARCIEXYZ_Z(r_xyz), ARCIEXYZ_Z(g_xyz), ARCIEXYZ_Z(b_xyz));

//    c3_transpose_m( & rgb2xyz );

    double  det = c3_m_det( & rgb2xyz );

    Mat3  xyz2rgb_temp;
    
    c3_md_invert_m( & rgb2xyz, det, & xyz2rgb_temp );
    
    ArRGB  w_rgb;
    
    c3_transpose_m( & xyz2rgb_temp );

    xyz_mat_to_rgb(
          art_gv,
        & w_xyz,
        & xyz2rgb_temp,
        & w_rgb
        );

    Mat3  rgb2xyz_final =
        MAT3( ARCIEXYZ_X(r_xyz)*XC(w_rgb), ARCIEXYZ_X(g_xyz)*YC(w_rgb), ARCIEXYZ_X(b_xyz)*ZC(w_rgb),
              ARCIEXYZ_Y(r_xyz)*XC(w_rgb), ARCIEXYZ_Y(g_xyz)*YC(w_rgb), ARCIEXYZ_Y(b_xyz)*ZC(w_rgb),
              ARCIEXYZ_Z(r_xyz)*XC(w_rgb), ARCIEXYZ_Z(g_xyz)*YC(w_rgb), ARCIEXYZ_Z(b_xyz)*ZC(w_rgb));
    
    det = c3_m_det( & rgb2xyz_final );

    Mat3  xyz2rgb_final;
    
    c3_md_invert_m( & rgb2xyz_final, det, & xyz2rgb_final );
    
    c3_transpose_m( & xyz2rgb_final );

    return  xyz2rgb_final;
}

/* ======================================================================== */
