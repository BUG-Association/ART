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

#define ART_MODULE_NAME     ArColourSpace

#include "ArColourSpace.h"
#include "ArCIEColourValues.h"

//   uncomment the following line to get debug output for each ICC profile
//   that is loaded

//#define ARCOLOURSPACE_DEBUGPRINTF_ON_LOADING

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

void createLCMSProfileFromARTColours(
        const Pnt2D            * r,
        const Pnt2D            * g,
        const Pnt2D            * b,
        const ArCIEXYZ         * whitepoint,
        const char             * description,
        const double             gamma,
              cmsHPROFILE      * profile
        )
{
    cmsCIEXYZ lcms_white_xyz;
 
    lcms_white_xyz.X = XC(*whitepoint);
    lcms_white_xyz.Y = YC(*whitepoint);
    lcms_white_xyz.Z = ZC(*whitepoint);

    cmsCIExyY lcms_white_xyy;
 
    cmsXYZ2xyY( & lcms_white_xyy, & lcms_white_xyz);

    cmsToneCurve  * gammacurve = cmsBuildGamma(NULL, gamma);
    cmsToneCurve  * gamma_rgb[3];

    gamma_rgb[0] = gammacurve;
    gamma_rgb[1] = gammacurve;
    gamma_rgb[2] = gammacurve;
 
    cmsCIExyYTRIPLE rgb_primaries;
 
    rgb_primaries.Red.x = XC(*r);
    rgb_primaries.Red.y = YC(*r);
    rgb_primaries.Red.Y = 1.0;

    rgb_primaries.Green.x = XC(*g);
    rgb_primaries.Green.y = YC(*g);
    rgb_primaries.Green.Y = 1.0;

    rgb_primaries.Blue.x = XC(*b);
    rgb_primaries.Blue.y = YC(*b);
    rgb_primaries.Blue.Y = 1.0;

    *profile =
        cmsCreateRGBProfile(
            & lcms_white_xyy,
            & rgb_primaries,
              gamma_rgb
            );

    cmsMLU *mlu0 = cmsMLUalloc(NULL, 1);
    cmsMLUsetASCII(mlu0, "en", "US", "Public Domain, created by the Advanced Rendering Toolkit via littlecms routines");
    cmsMLU *mlu1 = cmsMLUalloc(NULL, 1);
    cmsMLUsetASCII(mlu1, "en", "US", description);
    cmsWriteTag(*profile, cmsSigCopyrightTag,          mlu0);
    cmsWriteTag(*profile, cmsSigProfileDescriptionTag, mlu1);
    cmsMLUfree(mlu0);
    cmsMLUfree(mlu1);
}

void createCompleteLCMSProfileFromARTColours(
        const Pnt2D            * r,
        const Pnt2D            * g,
        const Pnt2D            * b,
        const ArCIEXYZ         * whitepoint,
        const char             * description,
        const double             gamma,
              cmsHPROFILE      * profile,
              cmsUInt32Number  * profileBufferSize,
              cmsUInt8Number  ** profileBuffer
        )
{
    createLCMSProfileFromARTColours(
          r,
          g,
          b,
          whitepoint,
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
        const Pnt2D          * r,
        const Pnt2D          * g,
        const Pnt2D          * b,
        const ArCIEXYZ       * whitepoint,
        const double           gamma,
              ArColourSpace  * cs
        )
{
    ARCS_WHITEPOINT(*cs) = whitepoint->c;
    
    ARCS_XYZ_TO_RGB( *cs ) =
        xyz2rgb_via_primaries(
              art_gv,
              r,
              g,
              b,
              whitepoint
            );
 
    double  det = c3_m_det( & ARCS_XYZ_TO_RGB( *cs  ) );

    c3_md_invert_m(
        & ARCS_XYZ_TO_RGB( *cs  ),
          det,
        & ARCS_RGB_TO_XYZ( *cs  )
        );

    ARCS_GAMMA( *cs  ) = gamma;
 
}


ART_MODULE_INITIALISATION_FUNCTION
(
    ARCS_GV = ALLOC(ArColourSpace_GV);

    pthread_mutex_init( & ARCS_MUTEX, NULL );

    ARCS_TABLE = artable_alloc_init();

    Pnt2D  r; Pnt2D  g; Pnt2D  b;

    ArCIEXYZ d65_xyz = ARCIEXYZ(0.95047,1.000,1.08883);
    ArCIEXYZ d50_xyz = ARCIEXYZ(0.96422,1.000,0.82521);

    ArColourSpace  temp;
 
    //   ------  CIE XYZ   ------------------------------------------------

    ARCS_TYPE(temp) = arcolourspacetype_ciexyz;
    ARCS_NAME(temp) = arsymbol( art_gv, "CIE XYZ" );
 
    ARCS_XYZ_TO_RGB(temp) =
        MAT3(  1.0, 0.0, 0.0,
               0.0, 1.0, 0.0,
               0.0, 0.0, 1.0 );
    ARCS_RGB_TO_XYZ(temp) =
        MAT3(  1.0, 0.0, 0.0,
               0.0, 1.0, 0.0,
               0.0, 0.0, 1.0 );
 
    ARCS_WHITEPOINT(temp) = d50_xyz.c;
    ARCS_GAMMA(temp) = 1.0;

#ifndef _ART_WITHOUT_LCMS_
    ARCS_PROFILE(temp) = cmsCreateXYZProfile();
    ARCS_LINEAR_PROFILE(temp) = cmsCreateXYZProfile();
#endif
    ARCS_XYZ = register_arcolourspace( art_gv, & temp );

    //   ------  CIE xyY   ------------------------------------------------

    ARCS_TYPE(temp) = arcolourspacetype_ciexyy;
    ARCS_NAME(temp) = arsymbol( art_gv, "CIE xyY" );
    ARCS_WHITEPOINT(temp) = d50_xyz.c;
    ARCS_XYY = register_arcolourspace( art_gv, & temp );

    //   ------  CIE L*a*b*   ---------------------------------------------

    ARCS_TYPE(temp) = arcolourspacetype_cielab;
    ARCS_NAME(temp) = arsymbol( art_gv, "CIE L*a*b*" );
    ARCS_WHITEPOINT(temp) = d50_xyz.c;
#ifndef _ART_WITHOUT_LCMS_
    ARCS_PROFILE(temp) = cmsCreateLab4Profile( 0 );
#endif
    ARCS_LAB = register_arcolourspace( art_gv, & temp );

    //   ------  CIE L*u*v*   ---------------------------------------------

    ARCS_TYPE(temp) = arcolourspacetype_cieluv;
    ARCS_NAME(temp) = arsymbol( art_gv, "CIE L*u*v*" );
    ARCS_WHITEPOINT(temp) = d50_xyz.c;
    ARCS_LUV = register_arcolourspace( art_gv, & temp );

    //   ------  sRGB   ---------------------------------------------------

    ARCS_TYPE(temp) = arcolourspacetype_rgb;
    ARCS_NAME(temp) = arsymbol( art_gv, "sRGB" );

    XC(r) = .64; YC(r) = .33;
    XC(g) = .30; YC(g) = .60;
    XC(b) = .15; YC(b) = .06;

    calculateColourspaceMatrices(
          art_gv,
        & r, & g, & b, & d65_xyz, 2.2,
        & temp
        );
 
    temp.gammafunction = arcolourspace_srgb_gamma;

#ifndef _ART_WITHOUT_LCMS_

    ARCS_PROFILE(temp) = cmsCreate_sRGBProfile();

    initLCMSProfileBuffer(
          ARCS_PROFILE(temp),
        & ARCS_PROFILEBUFFERSIZE(temp),
        & ARCS_PROFILEBUFFER(temp)
        );

    createCompleteLCMSProfileFromARTColours(
        & r, & g, & b, & d65_xyz,
          "linear sRGB",
          1.0,
        & ARCS_LINEAR_PROFILE(temp),
        & ARCS_LINEAR_PROFILEBUFFERSIZE(temp),
        & ARCS_LINEAR_PROFILEBUFFER(temp)
        );
#endif
    ARCS_SRGB = register_arcolourspace( art_gv, & temp );

    //   ------  Adobe RGB (1998)  ----------------------------------------

    ARCS_TYPE(temp) = arcolourspacetype_rgb;
    ARCS_NAME(temp) = arsymbol( art_gv, "Adobe RGB (1998)" );

    XC(r) = .64; YC(r) = .33;
    XC(g) = .21; YC(g) = .71;
    XC(b) = .15; YC(b) = .06;

    calculateColourspaceMatrices(
          art_gv,
        & r, & g, & b, & d65_xyz, 563. / 256.,
        & temp
        );
 
    temp.gammafunction = arcolourspace_standard_gamma;

#ifndef _ART_WITHOUT_LCMS_
    createCompleteLCMSProfileFromARTColours(
        & r, & g, & b, & d65_xyz,
          "Adobe RGB (1998)",
          ARCS_GAMMA(temp),
        & ARCS_PROFILE(temp),
        & ARCS_PROFILEBUFFERSIZE(temp),
        & ARCS_PROFILEBUFFER(temp)
        );

    createCompleteLCMSProfileFromARTColours(
        & r, & g, & b, & d65_xyz,
          "linear Adobe RGB (1998)",
          1.0,
        & ARCS_LINEAR_PROFILE(temp),
        & ARCS_LINEAR_PROFILEBUFFERSIZE(temp),
        & ARCS_LINEAR_PROFILEBUFFER(temp)
        );
#endif
    ARCS_ARGB = register_arcolourspace( art_gv, & temp );

    //   ------  Adobe Wide Gamut RGB -------------------------------------

    ARCS_TYPE(temp) = arcolourspacetype_rgb;
    ARCS_NAME(temp) = arsymbol( art_gv, "Adobe Wide Gamut RGB" );

    XC(r) = 0.7347; YC(r) = 0.2653;
    XC(g) = 0.1152; YC(g) = 0.8264;
    XC(b) = 0.1566; YC(b) = 0.0177;

    calculateColourspaceMatrices(
          art_gv,
        & r, & g, & b, & d50_xyz, 563. / 256.,
        & temp
        );
 
    temp.gammafunction = arcolourspace_standard_gamma;

#ifndef _ART_WITHOUT_LCMS_
    createCompleteLCMSProfileFromARTColours(
        & r, & g, & b, & d50_xyz,
          "Adobe Wide Gamut RGB",
          ARCS_GAMMA(temp),
        & ARCS_PROFILE(temp),
        & ARCS_PROFILEBUFFERSIZE(temp),
        & ARCS_PROFILEBUFFER(temp)
        );

    createCompleteLCMSProfileFromARTColours(
        & r, & g, & b, & d50_xyz,
          "linear Adobe Wide Gamut RGB",
          1.0,
        & ARCS_LINEAR_PROFILE(temp),
        & ARCS_LINEAR_PROFILEBUFFERSIZE(temp),
        & ARCS_LINEAR_PROFILEBUFFER(temp)
        );
#endif
    ARCS_WRGB = register_arcolourspace( art_gv, & temp );
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
    return art_gv->arcolourspace_gv->sRGB;
}

ArColourSpace const * arcolourspace_aRGB(
        const ART_GV  * art_gv
        )
{
    return art_gv->arcolourspace_gv->aRGB;
}

ArColourSpace const * arcolourspace_wRGB(
        const ART_GV  * art_gv
        )
{
    return art_gv->arcolourspace_gv->wRGB;
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

    ArColourSpace  temp;

    ARCS_PROFILEBUFFERSIZE(temp) = 0;
    ARCS_PROFILEBUFFER(temp) = 0;
    ARCS_RGB_TO_XYZ(temp) = C3_M_UNIT;
    ARCS_XYZ_TO_RGB(temp) = C3_M_UNIT;

    //   Sanity check #2 - inspection of the colour space the profile uses

    switch ( cmsGetColorSpace( profile ) )
    {
        case cmsSigXYZData:
            ARCS_TYPE(temp) = arcolourspacetype_ciexyz;
            break;

        case cmsSigLabData:
            ARCS_TYPE(temp) = arcolourspacetype_cielab;
            break;

        case cmsSigLuvData:
            ARCS_TYPE(temp) = arcolourspacetype_cieluv;
            break;

        case cmsSigYxyData:
            ARCS_TYPE(temp) = arcolourspacetype_ciexyy;
            break;

        case cmsSigRgbData:
            ARCS_TYPE(temp) = arcolourspacetype_rgb;
            break;

        default:
            ARCS_TYPE(temp) = arcolourspacetype_none;

            ART_ERRORHANDLING_FATAL_ERROR(
                "ICC profile '%s' is for an unsupported colour space"
                ,   profileInfoDescriptionBuffer
                );

            break;
    }

    ARCS_NAME(temp) = arsymbol( art_gv, profileInfoDescriptionBuffer );

    if ( ARCS_TYPE(temp) == arcolourspacetype_rgb )
    {
        //   Sanity check #3 - if this is a RGB space, it has to be a matrix shaper
        //                     for us to be able to extract the transform matrices

        if ( ! cmsIsMatrixShaper( profile ) )
            ART_ERRORHANDLING_FATAL_ERROR(
                "ICC profile '%s' contains a non-matrix shaper RGB space"
                ,   ARCS_NAME(temp)
                );

        //   Extraction of RGB primaries from profile

        cmsCIEXYZ  * primary_R;
        cmsCIEXYZ  * primary_G;
        cmsCIEXYZ  * primary_B;

        primary_R = (cmsCIEXYZ*) cmsReadTag( profile, cmsSigRedMatrixColumnTag );
        primary_G = (cmsCIEXYZ*) cmsReadTag( profile, cmsSigGreenMatrixColumnTag );
        primary_B = (cmsCIEXYZ*) cmsReadTag( profile, cmsSigBlueMatrixColumnTag );

        //   Assembly of RGB -> XYZ transform matrix from primaries

        ARCS_RGB_TO_XYZ(temp) =
            MAT3(
                primary_R->X, primary_G->X, primary_B->X,
                primary_R->Y, primary_G->Y, primary_B->Y,
                primary_R->Z, primary_G->Z, primary_B->Z
                );

       c3_transpose_m( & ARCS_RGB_TO_XYZ(temp) );

        //   The XYZ -> RGB matrix is the inverse of the RGB -> XYZ matrix

        double  det = c3_m_det( & ARCS_RGB_TO_XYZ(temp) );

        c3_md_invert_m(
            & ARCS_RGB_TO_XYZ(temp),
              det,
            & ARCS_XYZ_TO_RGB(temp)
            );

        //   Estimation of the RGB colour space gamma; for the sake of simplicity
        //   we just use the green gamma curve

        cmsToneCurve * greenToneCuve =
            (cmsToneCurve *) cmsReadTag( profile, cmsSigGreenTRCTag );

        ARCS_GAMMA(temp) = cmsEstimateGamma( greenToneCuve, 1.0 );

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
        printf("GAMMA: %f\n",ARCS_GAMMA(temp));

        printf("\n"C3_M_FORMAT("%6.5f") "\n",C3_M_PRINTF(ARCS_RGB_TO_XYZ(temp)));fflush(stdout);
        printf(C3_M_FORMAT("%6.5f") "\n",C3_M_PRINTF(ARCS_XYZ_TO_RGB(temp)));fflush(stdout);
#endif
    }
    else
        ARCS_GAMMA(temp) = 1.0;

    ARCS_PROFILE(temp) = profile;

    ArColourSpaceRef  csr = register_arcolourspace( art_gv, & temp );

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

        printf("\n"C3_M_FORMAT("%6.5f") "\n",C3_M_PRINTF(ARCS_RGB_TO_XYZ(temp)));fflush(stdout);
        printf(C3_M_FORMAT("%6.5f") "\n",C3_M_PRINTF(ARCS_XYZ_TO_RGB(temp)));fflush(stdout);
*/
}

/* ======================================================================== */
