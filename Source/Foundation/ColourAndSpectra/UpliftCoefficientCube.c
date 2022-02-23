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

#define ART_MODULE_NAME     UpliftCoefficientCube

#include "UpliftCoefficientCube.h"

typedef struct UpliftCoefficientCube_GV
{
    pthread_mutex_t    mutex;
    UCC              * ucc_srgb;
}
UpliftCoefficientCube_GV;

#define UCC_GV          art_gv->upliftcoefficientcube_gv
#define UCC_MUTEX       UCC_GV->mutex
#define UCC_SRGB        UCC_GV->ucc_srgb

ART_MODULE_INITIALISATION_FUNCTION
(
    UCC_GV = ALLOC(UpliftCoefficientCube_GV);

    pthread_mutex_init( & UCC_MUTEX, NULL );
 
    UCC_SRGB = NULL;
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    pthread_mutex_destroy( & UCC_MUTEX );
 
    if ( UCC_SRGB )
        ucc_free( UCC_SRGB );
)


/*
    'UCCEntry' - the data structure for individual lattice points

    The data structure for each lattice point in the coefficient cube. For
    using the cube data during spectral uplifting, only 'c' (the coefficients)
    and 'lattice_rgb' (the RGB coordinates for each lattice point) are needed.
    As 'lattice_rgb' is implicitly given, a cube file only needs to contain
    the bare coefficients. For debugging purposes, we currently also save
    'target_rgb' and 'treated': this data is used for internal consistency
    checks, and could be omitted in a stable implementation.
 
    'c'                         The sigmoid coefficients.
 
    'c_other'                   Additional coefficients, unused at the moment.
 
    'lattice_rgb'               The RGB coords this point should represent
                                in the original RGB unit cube.
*/

typedef struct UCCEntry
{
    Crd3   c;
    Crd3   c_other;
    ArRGB  rgb;
}
UCCEntry;

//    Versions of the coefficient cube data. We only read 1 and 2, and
//    ignore all the extra debug data in version 2

#define  UCC_SUPPORTED_VERSION          1

#define  UCC_VERSION(cc)                (cc)->version
#define  UCC_FEATURES(cc)               (cc)->features
#define  UCC_DIMENSION(cc)              (cc)->dimension
#define  UCC_ROW_SIZE(cc)               UCC_DIMENSION(cc)
#define  UCC_LEVEL_SIZE(cc)             (cc)->level_size
#define  UCC_OVERALL_SIZE(cc)           (cc)->overall_size
#define  UCC_INV_LATTICE_SPACING(cc)    (cc)->inv_lattice_spacing
#define  UCC_ENTRY_ARRAY(cc)            (cc)->ucce
#define  UCC_ENTRY(cc,i)                UCC_ENTRY_ARRAY(cc)[(i)]
#define  UCC_ENTRY_RGB(cc,i)            UCC_ENTRY_ARRAY(cc)[(i)].rgb
#define  UCC_ENTRY_C(cc,i)              UCC_ENTRY_ARRAY(cc)[(i)].c
#define  UCC_ENTRY_CO(cc,i)             UCC_ENTRY_ARRAY(cc)[(i)].c_other

#define  UCC_XYZ_TO_I(cc,x,y,z)  \
    ((x) + UCC_ROW_SIZE(cc) * (y) + UCC_LEVEL_SIZE(cc) * (z))

#define  UCC_I_TO_X(cc,i)     ((int)(((i) % UCC_LEVEL_SIZE(cc)) % UCC_ROW_SIZE(cc)))
#define  UCC_I_TO_Y(cc,i)     ((int)(((i) % UCC_LEVEL_SIZE(cc)) / UCC_ROW_SIZE(cc)))
#define  UCC_I_TO_Z(cc,i)     ((int) ((i) / UCC_LEVEL_SIZE(cc)))

#define  UCC_F_TO_I(cc,f)     (floor(f*((double)(UCC_DIMENSION(cc)-1.))))

typedef enum UCCFeatures
{
    ucc_features_none      = 0x00,
    ucc_features_debuginfo = 0x01,
    ucc_features_other     = 0x02,
}
UCCFeatures;

#define  UCC_CONTAINS_DEBUG_DATA(cc) \
    ( UCC_FEATURES(cc) & ucc_features_debuginfo )
#define  UCC_CONTAINS_OTHER_DATA(cc) \
    ( UCC_FEATURES(cc) & ucc_features_other )

void  sps_sigmoid_sample(
              ART_GV            * art_gv,
        const ArWavelength      * wl,
        const Crd3              * c,
              ArSpectralSample  * sps
        )
{
    for ( unsigned int i = 0; i < HERO_SAMPLES_TO_SPLAT; i++ )
    {
        double  iwl =  NANO_FROM_UNIT(ARWL_WI(*wl, i)) - 380. - 180.;
        
        SPS_CI(*sps, i) =
            m_sigmoid(C3_0(*c)*M_SQR(iwl) + C3_1(*c)*iwl + C3_2(*c));
    }
}


void ucc_alloc_and_read_from_file(
              UCC   ** ucc,
        const char   * filename
        )
{
    FILE  * inputFile = fopen(filename, "r");

    if ( ! inputFile )
    {
        ART_ERRORHANDLING_FATAL_ERROR(
            "could not open UCC file %s",
            filename
            );
    }

    *ucc = ALLOC(UCC);
    
    //   This is just to avoid the silly double indirections needed when
    //   working with a double pointer
    
    UCC  *cc = *ucc;

    //   Read the cube version
    
    art_binary_read_int( inputFile, & UCC_VERSION(cc) );
    
    if ( UCC_VERSION(cc) != UCC_SUPPORTED_VERSION )
    {
        ART_ERRORHANDLING_FATAL_ERROR(
            "unsupported UCC version %d, supported is %d",
            UCC_VERSION(cc),
            UCC_SUPPORTED_VERSION
            );
    }

    //   Read the features & lattice size
    
    art_binary_read_int( inputFile, & UCC_FEATURES(cc) );
    art_binary_read_int( inputFile, & UCC_DIMENSION(cc) );

    //   Precompute frequently needed multiples
    
    UCC_LEVEL_SIZE(cc) = M_SQR(UCC_DIMENSION(cc));
    UCC_OVERALL_SIZE(cc) = M_CUBE(UCC_DIMENSION(cc));
    UCC_INV_LATTICE_SPACING(cc) = UCC_DIMENSION(cc) - 1.;

    float  f;

    //   NOTE: the UCC header information is currently ignored, but this is
    //         not the final state. Future ART versions will process this
    //         information to handle different colour spaces.
    
    //   Read & ignore the fitting illuminant
    
    for ( int j = 0; j < 360; j++ )
    {
        art_binary_read_float( inputFile, & f );
    }

    //   Read & ignore the  CIE x, y coordinates of the four primaries (RGBW)
    
    for ( int i = 0; i < 4; i++ )
    {
        art_binary_read_float( inputFile, & f );
        art_binary_read_float( inputFile, & f );
    }

    //   Read & ignore the fitting threshold

    art_binary_read_float( inputFile, & f );

    UCC_ENTRY_ARRAY(cc) = ALLOC_ARRAY(UCCEntry, UCC_OVERALL_SIZE(cc));
    
    //   The stepsize is unit div one less than the number of lattice points

    double  stepsize = 1. / ( UCC_DIMENSION(cc) - 1.);

    for ( int i = 0; i < UCC_OVERALL_SIZE(cc); i++ )
    {
        //   Lattice point RGB values are computed
        
        XC( UCC_ENTRY_RGB(cc,i) ) = UCC_I_TO_X(cc,i) * stepsize;
        YC( UCC_ENTRY_RGB(cc,i) ) = UCC_I_TO_Y(cc,i) * stepsize;
        ZC( UCC_ENTRY_RGB(cc,i) ) = UCC_I_TO_Z(cc,i) * stepsize;

        //   Read the sigmoid coefficients
        
        for ( int j = 0; j < 3; j++ )
        {
            float  f;
            art_binary_read_float( inputFile, & f );
            C3_CI( UCC_ENTRY_C(cc,i), j ) = f;
        }
        
        if ( UCC_CONTAINS_OTHER_DATA(cc) )
        {
            for ( int j = 0; j < 3; j++ )
            {
                float  f;
                art_binary_read_float( inputFile, & f );
                C3_CI( UCC_ENTRY_CO(cc,i), j ) = f;
            }
        }

        //   Extra data is only present in debug versions of the cubes.
        
        if ( UCC_CONTAINS_DEBUG_DATA(cc) )
        {
            //   Read & ignore the fitting target
            
            for ( int j = 0; j < 3; j++ )
            {
                float  f;
                art_binary_read_float( inputFile, & f );
            }

            int  treated;
            
            //   Read the entry status. We don't store this value, but
            //   throw an error if there are unprocessed entries.
            //   Half-done cubes are only interesting for research purposes,
            //   but aren't useful for rendering.
            
            art_binary_read_int( inputFile, & treated );
            
            if ( treated == -1 )
            {
                ART_ERRORHANDLING_FATAL_ERROR(
                    "spectral uplifting coefficient cube %s has unprocessed "
                    "entries, and cannot be used for rendering",
                    filename
                    );
            }
        }
    }

    fclose(inputFile);
}

void ucc_rgb_to_sps(
              ART_GV            * art_gv,
        const UCC               * ucc,
        const ArRGB             * rgb,
        const ArWavelength      * wl,
              ArSpectralSample  * sps
        )
{
//printf("RGB: %f %f %f \n", XC(*rgb)*255, YC(*rgb)*255, ZC(*rgb)*255);
    IPnt3D  bc;
    
    XC(bc) = M_MIN(UCC_F_TO_I( ucc, XC(*rgb) ), UCC_DIMENSION(ucc) - 2. );
    YC(bc) = M_MIN(UCC_F_TO_I( ucc, YC(*rgb) ), UCC_DIMENSION(ucc) - 2. );
    ZC(bc) = M_MIN(UCC_F_TO_I( ucc, ZC(*rgb) ), UCC_DIMENSION(ucc) - 2. );

    int  ci[8];

    ci[0] = UCC_XYZ_TO_I( ucc, XC(bc)    , YC(bc)    , ZC(bc)     ); //000
    ci[1] = UCC_XYZ_TO_I( ucc, XC(bc)    , YC(bc)    , ZC(bc) + 1 ); //001
    ci[2] = UCC_XYZ_TO_I( ucc, XC(bc)    , YC(bc) + 1, ZC(bc)     ); //010
    ci[3] = UCC_XYZ_TO_I( ucc, XC(bc)    , YC(bc) + 1, ZC(bc) + 1 ); //011
    ci[4] = UCC_XYZ_TO_I( ucc, XC(bc) + 1, YC(bc)    , ZC(bc)     ); //100
    ci[5] = UCC_XYZ_TO_I( ucc, XC(bc) + 1, YC(bc)    , ZC(bc) + 1 ); //101
    ci[6] = UCC_XYZ_TO_I( ucc, XC(bc) + 1, YC(bc) + 1, ZC(bc)     ); //110
    ci[7] = UCC_XYZ_TO_I( ucc, XC(bc) + 1, YC(bc) + 1, ZC(bc) + 1 ); //111

    Vec3D  d;

    XC(d) =   ( XC(*rgb) - XC( UCC_ENTRY_RGB( ucc, ci[0] ) ) )
            * UCC_INV_LATTICE_SPACING(ucc);
    YC(d) =   ( YC(*rgb) - YC( UCC_ENTRY_RGB( ucc, ci[0] ) ) )
            * UCC_INV_LATTICE_SPACING(ucc);
    ZC(d) =   ( ZC(*rgb) - ZC( UCC_ENTRY_RGB( ucc, ci[0] ) ) )
            * UCC_INV_LATTICE_SPACING(ucc);

    ArSpectralSample  sv[8];
    
    sps_sigmoid_sample( art_gv, wl, & UCC_ENTRY_C(ucc, ci[0]), & sv[0] );
    sps_sigmoid_sample( art_gv, wl, & UCC_ENTRY_C(ucc, ci[1]), & sv[1] );
    sps_sigmoid_sample( art_gv, wl, & UCC_ENTRY_C(ucc, ci[2]), & sv[2] );
    sps_sigmoid_sample( art_gv, wl, & UCC_ENTRY_C(ucc, ci[3]), & sv[3] );
    sps_sigmoid_sample( art_gv, wl, & UCC_ENTRY_C(ucc, ci[4]), & sv[4] );
    sps_sigmoid_sample( art_gv, wl, & UCC_ENTRY_C(ucc, ci[5]), & sv[5] );
    sps_sigmoid_sample( art_gv, wl, & UCC_ENTRY_C(ucc, ci[6]), & sv[6] );
    sps_sigmoid_sample( art_gv, wl, & UCC_ENTRY_C(ucc, ci[7]), & sv[7] );

    ArSpectralSample  c00, c01, c10, c11;
    
    sps_dss_interpol_s( art_gv, XC(d), & sv[0], & sv[4], & c00 );
    sps_dss_interpol_s( art_gv, XC(d), & sv[1], & sv[5], & c01 );
    sps_dss_interpol_s( art_gv, XC(d), & sv[2], & sv[6], & c10 );
    sps_dss_interpol_s( art_gv, XC(d), & sv[3], & sv[7], & c11 );

    ArSpectralSample  c0, c1;

    sps_dss_interpol_s( art_gv, YC(d), & c00, & c10, & c0 );
    sps_dss_interpol_s( art_gv, YC(d), & c01, & c11, & c1 );

    sps_dss_interpol_s( art_gv, ZC(d), & c0, & c1, sps );
}

void art_find_file_on_paths(
        const char  ** paths,
        const char   * file_to_find,
              char  ** complete_path_to_file
        );

const UCC * ucc_srgb(
              ART_GV  * art_gv
        )
{
    pthread_mutex_lock( & UCC_MUTEX );
    
    if ( ! UCC_SRGB )
    {
        char  * ucc_filename = NULL;
        const char ** library_path = art_ev_resource_paths( art_gv );

        art_find_file_on_paths(
              library_path,
              "srgb.ucc",
            & ucc_filename
            );
        
        if ( ! ucc_filename )
        {
            ART_ERRORHANDLING_FATAL_ERROR(
                "could not find sRGB spectral uplifting data in "
                "ART_Resources"
                );
        }

        ucc_alloc_and_read_from_file( & UCC_SRGB, ucc_filename);
    }
    
    pthread_mutex_unlock( & UCC_MUTEX );
    
    return  UCC_SRGB;
}

void ucc_free(
              UCC  * ucc
        )
{
    FREE_ARRAY(UCC_ENTRY_ARRAY(ucc));
    FREE(ucc);
}

/* ======================================================================== */
