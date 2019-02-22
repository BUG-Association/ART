/* ===========================================================================

    Copyright (c) 1996-2019 The ART Development Team
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

#define ART_MODULE_NAME     UpliftCoefficientCube

#import "UpliftCoefficientCube.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


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
 
    'lattice_rgb'               The RGB coords this point should represent
                                in the original RGB unit cube.
*/

typedef struct UCCEntry
{
    Crd3   c;
    ArRGB  rgb;
}
UCCEntry;

#define  UCC_DIMENSION(cc)              (cc)->dimension
#define  UCC_ROW_SIZE(cc)               UCC_DIMENSION(cc)
#define  UCC_LEVEL_SIZE(cc)             (cc)->level_size
#define  UCC_OVERALL_SIZE(cc)           (cc)->overall_size
#define  UCC_INV_LATTICE_SPACING(cc)    (cc)->inv_lattice_spacing
#define  UCC_ENTRY_ARRAY(cc)            (cc)->ucce
#define  UCC_ENTRY(cc,i)                UCC_ENTRY_ARRAY(cc)[(i)]
#define  UCC_ENTRY_RGB(cc,i)            UCC_ENTRY_ARRAY(cc)[(i)].rgb
#define  UCC_ENTRY_C(cc,i)              UCC_ENTRY_ARRAY(cc)[(i)].c

#define  UCC_XYZ_TO_I(cc,x,y,z)  \
    ((x) + UCC_ROW_SIZE(cc) * (y) + UCC_LEVEL_SIZE(cc) * (z))

#define  UCC_I_TO_X(cc,i)     ((int)(((i) % UCC_LEVEL_SIZE(cc)) % UCC_ROW_SIZE(cc)))
#define  UCC_I_TO_Y(cc,i)     ((int)(((i) % UCC_LEVEL_SIZE(cc)) / UCC_ROW_SIZE(cc)))
#define  UCC_I_TO_Z(cc,i)     ((int) ((i) / UCC_LEVEL_SIZE(cc)))

#define  UCC_F_TO_I(cc,f)     (floor(f*((double)(UCC_DIMENSION(cc)-1.))))

void  sps_sigmoid_sample(
              ART_GV            * art_gv,
        const ArWavelength      * wl,
        const Crd3              * c,
              ArSpectralSample  * sps
        )
{
    for ( int i = 0; i < HERO_SAMPLES_TO_SPLAT; i++ )
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
    ArString    full_filename;
    
    arstring_pe_copy_add_extension_p(
          filename,
          UCC_FILE_EXTENSION,
        & full_filename
        );

    FILE  * inputFile = fopen(full_filename, "r");
    
    *ucc = ALLOC(UCC);
    
    //   This is just to avoid the silly double indirections needed when
    //   working with a double pointer
    
    UCC  *cc = *ucc;

    //   Read the lattice size
    
    art_binary_read_int( inputFile, & UCC_DIMENSION(cc) );

    //   Precompute frequently needed multiples
    
    UCC_LEVEL_SIZE(cc) = M_SQR(UCC_DIMENSION(cc));
    UCC_OVERALL_SIZE(cc) = M_CUBE(UCC_DIMENSION(cc));
    UCC_INV_LATTICE_SPACING(cc) = UCC_DIMENSION(cc) - 1.;

    float  f;

    //   Read & ignore the fitting illuminant
    
    for ( int j = 0; j < 360; j++ )
    {
        art_binary_read_float( inputFile, & f );
    }

    //   Read & ignore the fitting threshold

    art_binary_read_float( inputFile, & f );

    UCC_ENTRY_ARRAY(cc) = ALLOC_ARRAY(UCCEntry, UCC_OVERALL_SIZE(cc));
    
    //   The stepsize is unit div one less than the number of lattice points

    double  stepsize = 1. / ( UCC_DIMENSION(cc) - 1.);

    int  untreated_points = 0;
    
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
                full_filename
                );
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

void ucc_free(
              UCC  * ucc
        )
{
    FREE_ARRAY(UCC_ENTRY_ARRAY(ucc));
    FREE(ucc);
}

/* ======================================================================== */
