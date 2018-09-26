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

//#define ARSPECTRUMSPACE_DEBUGPRINTF_ON_LOADING

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
}
ArColourSpace_GV;

ART_MODULE_INITIALISATION_FUNCTION
(
    ArColourSpace_GV  * arcolourspace_gv;

    arcolourspace_gv = ALLOC(ArColourSpace_GV);

    pthread_mutex_init( & arcolourspace_gv->mutex, NULL );

    arcolourspace_gv->table = artable_alloc_init();

    art_gv->arcolourspace_gv = arcolourspace_gv;

    ArColourSpace  temp;

    ARCS_TYPE( temp )    = arcolourspacetype_ciexyz;
    ARCS_NAME( temp )    = arsymbol( art_gv, "CIE XYZ" );
    ARCS_XYZ_TO_RGB( temp ) =
        MAT3(  1.0, 0.0, 0.0,
               0.0, 1.0, 0.0,
               0.0, 0.0, 1.0 );
    ARCS_RGB_TO_XYZ( temp ) =
        MAT3(  1.0, 0.0, 0.0,
               0.0, 1.0, 0.0,
               0.0, 0.0, 1.0 );
    ARCS_GAMMA( temp ) = 1.0;

#ifndef _ART_WITHOUT_LCMS_
    ARCS_PROFILE( temp ) = cmsCreateXYZProfile();
#endif
    arcolourspace_gv->CIEXYZ = register_arcolourspace( art_gv, & temp );

    ARCS_TYPE( temp )    = arcolourspacetype_cielab;
    ARCS_NAME( temp )    = arsymbol( art_gv, "CIE L*a*b*" );
#ifndef _ART_WITHOUT_LCMS_
    ARCS_PROFILE( temp ) = cmsCreateLab4Profile( 0 );
#endif
    arcolourspace_gv->CIELab = register_arcolourspace( art_gv, & temp );

    ARCS_TYPE( temp )    = arcolourspacetype_cieluv;
    ARCS_NAME( temp )    = arsymbol( art_gv, "CIE L*u*v*" );
    arcolourspace_gv->CIELuv = register_arcolourspace( art_gv, & temp );

    ARCS_TYPE( temp )    = arcolourspacetype_ciexyy;
    ARCS_NAME( temp )    = arsymbol( art_gv, "CIE xyY" );
    arcolourspace_gv->CIExyY = register_arcolourspace( art_gv, & temp );

    ARCS_TYPE( temp )    = arcolourspacetype_rgb;
    ARCS_NAME( temp )    = arsymbol( art_gv, "sRGB" );
#ifndef _ART_WITHOUT_LCMS_
    ARCS_PROFILE( temp ) = cmsCreate_sRGBProfile();
#endif
    arcolourspace_gv->sRGB = register_arcolourspace( art_gv, & temp );
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    artable_free( art_gv->arcolourspace_gv->table );

    pthread_mutex_destroy(
        & art_gv->arcolourspace_gv->mutex
        );

    FREE( art_gv->arcolourspace_gv );
)

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

ArColourSpaceRef register_arcolourspace(
        ART_GV         * art_gv,
        ArColourSpace  * newCS
        )
{
    ArColourSpaceRef  newRef = 0;

    if ( newCS && ARSPECTRUMSPACE_NAME(*newCS) )
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
    ARCS_RGB_TO_XYZ( temp ) = C3_M_UNIT;
    ARCS_XYZ_TO_RGB( temp ) = C3_M_UNIT;

    //   Sanity check #2 - inspection of the colour space the profile uses

    switch ( cmsGetColorSpace( profile ) )
    {
        case cmsSigXYZData:
            ARCS_TYPE( temp ) = arcolourspacetype_ciexyz;
            break;

        case cmsSigLabData:
            ARCS_TYPE( temp ) = arcolourspacetype_cielab;
            break;

        case cmsSigLuvData:
            ARCS_TYPE( temp ) = arcolourspacetype_cieluv;
            break;

        case cmsSigYxyData:
            ARCS_TYPE( temp ) = arcolourspacetype_ciexyy;
            break;

        case cmsSigRgbData:
            ARCS_TYPE( temp ) = arcolourspacetype_rgb;
            break;

        default:
            ARCS_TYPE( temp ) = arcolourspacetype_none;

            ART_ERRORHANDLING_FATAL_ERROR(
                "ICC profile '%s' is for an unsupported colour space"
                ,   profileInfoDescriptionBuffer
                );

            break;
    }

    ARCS_NAME( temp ) = arsymbol( art_gv, profileInfoDescriptionBuffer );

    if ( ARCS_TYPE( temp ) == arcolourspacetype_rgb )
    {
        //   Sanity check #3 - if this is a RGB space, it has to be a matrix shaper
        //                     for us to be able to extract the transform matrices

        if ( ! cmsIsMatrixShaper( profile ) )
            ART_ERRORHANDLING_FATAL_ERROR(
                "ICC profile '%s' contains a non-matrix shaper RGB space"
                ,   ARCS_NAME( temp )
                );

        //   Extraction of RGB primaries from profile

        cmsCIEXYZ  * primary_R;
        cmsCIEXYZ  * primary_G;
        cmsCIEXYZ  * primary_B;

        primary_R = (cmsCIEXYZ*) cmsReadTag( profile, cmsSigRedMatrixColumnTag );
        primary_G = (cmsCIEXYZ*) cmsReadTag( profile, cmsSigGreenMatrixColumnTag );
        primary_B = (cmsCIEXYZ*) cmsReadTag( profile, cmsSigBlueMatrixColumnTag );

        //   Assembly of RGB -> XYZ transform matrix from primaries

        ARCS_RGB_TO_XYZ( temp ) =
            MAT3(
                primary_R->X, primary_G->X, primary_B->X,
                primary_R->Y, primary_G->Y, primary_B->Y,
                primary_R->Z, primary_G->Z, primary_B->Z
                );

       c3_transpose_m( & ARCS_RGB_TO_XYZ( temp ) );

        //   The XYZ -> RGB matrix is the inverse of the RGB -> XYZ matrix

        double  det = c3_m_det( & ARCS_RGB_TO_XYZ( temp ) );

        c3_md_invert_m(
            & ARCS_RGB_TO_XYZ( temp ),
              det,
            & ARCS_XYZ_TO_RGB( temp )
            );

        //   Estimation of the RGB colour space gamma; for the sake of simplicity
        //   we just use the green gamma curve

        cmsToneCurve * greenToneCuve =
            (cmsToneCurve *) cmsReadTag( profile, cmsSigGreenTRCTag );

        ARCS_GAMMA( temp ) = cmsEstimateGamma( greenToneCuve, 1.0 );

#ifdef ARSPECTRUMSPACE_DEBUGPRINTF_ON_LOADING
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
        printf("GAMMA: %f\n",ARCS_GAMMA( temp ));

        printf("\n"C3_M_FORMAT("%6.5f") "\n",C3_M_PRINTF(ARCS_RGB_TO_XYZ( temp )));fflush(stdout);
        printf(C3_M_FORMAT("%6.5f") "\n",C3_M_PRINTF(ARCS_XYZ_TO_RGB( temp )));fflush(stdout);
#endif
    }
    else
        ARCS_GAMMA( temp ) = 1.0;

    ARCS_PROFILE( temp ) = profile;

    ArColourSpaceRef  csr = register_arcolourspace( art_gv, & temp );

#ifdef ARSPECTRUMSPACE_DEBUGPRINTF_ON_LOADING
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

        printf("\n"C3_M_FORMAT("%6.5f") "\n",C3_M_PRINTF(ARCS_RGB_TO_XYZ( temp )));fflush(stdout);
        printf(C3_M_FORMAT("%6.5f") "\n",C3_M_PRINTF(ARCS_XYZ_TO_RGB( temp )));fflush(stdout);
*/
}

/* ======================================================================== */
