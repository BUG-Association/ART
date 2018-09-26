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

#define ART_MODULE_NAME     ArCIEColourValues

#include "ArCIEColourValues.h"

#include "ArCIEColourConversions.h"

typedef struct ArCIEColourValues_GV
{
    ArCIEXYZ  XYZ_BLACK;
    ArCIEXYZ  XYZ_WHITE;
    ArCIEXYZ  XYZ_E;
    ArCIEXYZ  XYZ_D50;
    ArCIEXYZ  XYZ_D55;
    ArCIEXYZ  XYZ_D65;
    ArCIEXYZ  XYZ_D75;
    ArCIEXYZ  XYZ_A;

    ArCIExyY  xyY_BLACK;
    ArCIExyY  xyY_WHITE;
    ArCIExyY  xyY_E;
    ArCIExyY  xyY_D50;
    ArCIExyY  xyY_D55;
    ArCIExyY  xyY_D65;
    ArCIExyY  xyY_D75;
    ArCIExyY  xyY_A;

    ArCIELab  Lab_BLACK;
    ArCIELab  Lab_WHITE;

    ArCIELuv  Luv_BLACK;
    ArCIELuv  Luv_WHITE;
}
ArCIEColourValues_GV;


ART_MODULE_INITIALISATION_FUNCTION
(
    ArCIEColourValues_GV  * arccv_gv;

    arccv_gv = ALLOC(ArCIEColourValues_GV);

    arccv_gv->XYZ_BLACK = ARCIEXYZ( 0.0, 0.0, 0.0 );
    arccv_gv->XYZ_WHITE = ARCIEXYZ( 1.0, 1.0, 1.0 );

    arccv_gv->xyY_BLACK = ARCIExyY( MATH_1_DIV_3, MATH_1_DIV_3, 0.0 );
    arccv_gv->xyY_WHITE = ARCIExyY( MATH_1_DIV_3, MATH_1_DIV_3, 1.0 );

    double  standard_Y = 1.0;

    arccv_gv->xyY_E     = ARCIExyY( MATH_1_DIV_3, MATH_1_DIV_3, standard_Y );
    arccv_gv->xyY_D50   = ARCIExyY( 0.34567, 0.35850, standard_Y );
    arccv_gv->xyY_D55   = ARCIExyY( 0.33242, 0.34743, standard_Y );
    arccv_gv->xyY_D65   = ARCIExyY( 0.31271, 0.32902, standard_Y );
    arccv_gv->xyY_D75   = ARCIExyY( 0.29902, 0.31485, standard_Y );
    arccv_gv->xyY_A     = ARCIExyY( 0.44757, 0.40745, standard_Y );

    xyy_to_xyz( art_gv, & arccv_gv->xyY_E  , & arccv_gv->XYZ_E   );
    xyy_to_xyz( art_gv, & arccv_gv->xyY_D50, & arccv_gv->XYZ_D50 );
    xyy_to_xyz( art_gv, & arccv_gv->xyY_D55, & arccv_gv->XYZ_D55 );
    xyy_to_xyz( art_gv, & arccv_gv->xyY_D65, & arccv_gv->XYZ_D65 );
    xyy_to_xyz( art_gv, & arccv_gv->xyY_D75, & arccv_gv->XYZ_D75 );
    xyy_to_xyz( art_gv, & arccv_gv->xyY_A  , & arccv_gv->XYZ_A );

    arccv_gv->Lab_BLACK = ARCIELab(   0.0, 0.0, 0.0 );
    arccv_gv->Lab_WHITE = ARCIELab( 100.0, 0.0, 0.0 );

    arccv_gv->Luv_BLACK = ARCIELuv(   0.0, 0.0, 0.0 );
    arccv_gv->Luv_WHITE = ARCIELuv( 100.0, 0.0, 0.0 );

    art_gv->arciecolourvalues_gv = arccv_gv;
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    FREE( art_gv->arciecolourvalues_gv );
)

ArCIEXYZ const * xyz_zero(
        const ART_GV  * art_gv
        )
{
    return & art_gv->arciecolourvalues_gv->XYZ_BLACK;
}

ArCIEXYZ const * xyz_illuminant_E(
        const ART_GV  * art_gv
        )
{
    return & art_gv->arciecolourvalues_gv->XYZ_E;
}

ArCIEXYZ const * xyz_illuminant_D50(
        const ART_GV  * art_gv
        )
{
    return & art_gv->arciecolourvalues_gv->XYZ_D50;
}

ArCIEXYZ const * xyz_illuminant_D65(
        const ART_GV  * art_gv
        )
{
    return & art_gv->arciecolourvalues_gv->XYZ_D65;
}

ArCIEXYZ const * xyz_illuminant_A(
        const ART_GV  * art_gv
        )
{
    return & art_gv->arciecolourvalues_gv->XYZ_A;
}

double lab_delta_L(
        const ArCIELab  * lab_0,
        const ArCIELab  * lab_1
        )
{
    return M_ABS( ARCIELab_L( *lab_0 ) - ARCIELab_L( *lab_1 ) );
}

double lab_delta_C(
        const ArCIELab  * lab_0,
        const ArCIELab  * lab_1
        )
{
    double  C_0 = sqrt( M_SQR( ARCIELab_a( *lab_0 ) ) + M_SQR( ARCIELab_b( *lab_0 ) ) );
    double  C_1 = sqrt( M_SQR( ARCIELab_a( *lab_1 ) ) + M_SQR( ARCIELab_b( *lab_1 ) ) );

    return M_ABS( C_0 - C_1 );
}

double lab_delta_H(
        const ArCIELab  * lab_0,
        const ArCIELab  * lab_1
        )
{
    double  H_0 = 0.0;
    double  H_1 = 0.0;

    if ( ARCIELab_a( *lab_0 ) != 0.0 )
        H_0 = atan( ARCIELab_b( *lab_0 ) / ARCIELab_a( *lab_0 ) );

    if ( ARCIELab_a( *lab_1 ) != 0.0 )
        H_1 = atan( ARCIELab_b( *lab_1 ) / ARCIELab_a( *lab_1 ) );

    return M_ABS( H_0 - H_1 );
}

double lab_delta_E(
        const ArCIELab  * lab_0,
        const ArCIELab  * lab_1
        )
{
    return
        sqrt(
              M_SQR( ARCIELab_L( *lab_0 ) - ARCIELab_L( *lab_1 ) )
            + M_SQR( ARCIELab_a( *lab_0 ) - ARCIELab_a( *lab_1 ) )
            + M_SQR( ARCIELab_b( *lab_0 ) - ARCIELab_b( *lab_1 ) )
            );
}

double luv_delta_E(
        const ArCIELuv  * luv_0,
        const ArCIELuv  * luv_1
        )
{
    return
        sqrt(
              M_SQR( ARCIELuv_L( *luv_0 ) - ARCIELuv_L( *luv_1 ) )
            + M_SQR( ARCIELuv_u( *luv_0 ) - ARCIELuv_u( *luv_1 ) )
            + M_SQR( ARCIELuv_v( *luv_0 ) - ARCIELuv_v( *luv_1 ) )
            );
}

double xyz_sd_value_at_wavelength(
        const ART_GV    * art_gv,
        const ArCIEXYZ  * c0,
        const double      d0
        )
{
    ART_ERRORHANDLING_FATAL_ERROR("xyz_cd_value_at_wavelength not implemented");

    return 0.0;
}

void xyz_to_utf_xyz(
        const ART_GV        * art_gv,
        const ArCIEXYZ      * xyz,
              ArUTF_CIEXYZ  * utf_xyz
        )
{
    c3_c_to_fc( & ARCIEXYZ_C(*xyz), & ARUTF_CIEXYZ_C(*utf_xyz) );
}

void utf_xyz_to_xyz(
        const ART_GV        * art_gv,
        const ArUTF_CIEXYZ  * utf_xyz,
              ArCIEXYZ      * xyz
        )
{
    c3_fc_to_c( & ARUTF_CIEXYZ_C(*utf_xyz), & ARCIEXYZ_C(*xyz) );
    ARCIEXYZ_S(*xyz) = ARCSR_CIEXYZ;
}

void xyz_s_debugprintf(
        const ART_GV    * art_gv,
        const ArCIEXYZ  * c_0
        )
{
    printf( "CIE XYZ ( % 5.3f, % 5.3f, % 5.3f )\n",
        ARCIEXYZ_X(*c_0),
        ARCIEXYZ_Y(*c_0),
        ARCIEXYZ_Z(*c_0) );

    fflush(stdout);
}

void xyza_s_debugprintf(
        const ART_GV     * art_gv,
        const ArCIEXYZA  * c_0
        )
{
    printf( "CIE XYZ Alpha ( % 5.3f, % 5.3f, % 5.3f, % 5.3f )\n",
        ARCIEXYZA_X(*c_0),
        ARCIEXYZA_Y(*c_0),
        ARCIEXYZA_Z(*c_0),
        ARCIEXYZA_A(*c_0)
        );

    fflush(stdout);
}

void xyy_s_debugprintf(
        const ART_GV    * art_gv,
        const ArCIExyY  * c_0
        )
{
    printf( "CIE x y Y ( % 5.3f, % 5.3f, % 5.3f )\n",
        ARCIExyY_x(*c_0),
        ARCIExyY_y(*c_0),
        ARCIExyY_Y(*c_0)
        );

    fflush(stdout);
}

void lab_s_debugprintf(
        const ART_GV    * art_gv,
        const ArCIELab  * c_0
        )
{
    printf( "CIE L* a* b* ( % 5.3f, % 5.3f, % 5.3f )\n",
        ARCIELab_L(*c_0),
        ARCIELab_a(*c_0),
        ARCIELab_b(*c_0)
        );

    fflush(stdout);
}

void luv_s_debugprintf(
        const ART_GV    * art_gv,
        const ArCIELuv  * c_0
        )
{
    printf( "CIE L* u* v* ( % 5.3f, % 5.3f, % 5.3f )\n",
        ARCIELuv_L(*c_0),
        ARCIELuv_u(*c_0),
        ARCIELuv_v(*c_0)
        );

    fflush(stdout);
}

// The following function is based on the code available at
// http://www.ece.rochester.edu/~/gsharma/ciede2000/
// The only changes were adaptation to ART semantics.

// Computes the CIEDE2000 color-difference between two Lab colors
// Based on the article:
// The CIEDE2000 Color-Difference Formula: Implementation Notes,
// Supplementary Test Data, and Mathematical Observations,", G. Sharma,
// W. Wu, E. N. Dalal, submitted to Color Research and Application,
// January 2004.
// Based on the C implementation by G. Sharma, which in turn was based
// on the C++ code of Ofir Pele, The Hebrew University of Jerusalem 2010.

double lab_delta_E2000(
        const ArCIELab  * lab_0,
        const ArCIELab  * lab_1
        )
{
    double Lstd = ARCIELab_L(*lab_0);
    double astd = ARCIELab_a(*lab_0);
    double bstd = ARCIELab_b(*lab_0);

    double Lsample = ARCIELab_L(*lab_1);
    double asample = ARCIELab_a(*lab_1);
    double bsample = ARCIELab_b(*lab_1);

    double _kL = 1.0;
    double _kC = 1.0;
    double _kH = 1.0;

    double Cabstd= sqrt(astd*astd+bstd*bstd);
    double Cabsample= sqrt(asample*asample+bsample*bsample);

    double Cabarithmean= (Cabstd + Cabsample)/2.0;

    double G= 0.5*( 1.0 - sqrt( pow(Cabarithmean,7.0)/(pow(Cabarithmean,7.0) + pow(25.0,7.0))));

    double apstd= (1.0+G)*astd; // aprime in paper
    double apsample= (1.0+G)*asample; // aprime in paper
    double Cpsample= sqrt(apsample*apsample+bsample*bsample);

    double Cpstd= sqrt(apstd*apstd+bstd*bstd);
    // Compute product of chromas
    double Cpprod= (Cpsample*Cpstd);


    // Ensure hue is between 0 and 2pi
    double hpstd= atan2(bstd,apstd);
    if (hpstd<0) hpstd+= MATH_2_MUL_PI;  // rollover ones that come -ve

    double hpsample= atan2(bsample,apsample);
    if (hpsample<0) hpsample+= MATH_2_MUL_PI;
    if ( (fabs(apsample)+fabs(bsample))==0.0)  hpsample= 0.0;

    double dL= (Lsample-Lstd);
    double dC= (Cpsample-Cpstd);

    // Computation of hue difference
    double dhp= (hpsample-hpstd);
    if (dhp>MATH_PI)  dhp-= MATH_2_MUL_PI;
    if (dhp<-MATH_PI) dhp+= MATH_2_MUL_PI;
    // set chroma difference to zero if the product of chromas is zero
    if (Cpprod == 0.0) dhp= 0.0;

    // Note that the defining equations actually need
    // signed Hue and chroma differences which is different
    // from prior color difference formulae

    double dH= 2.0*sqrt(Cpprod)*sin(dhp/2.0);
    //%dH2 = 4*Cpprod.*(sin(dhp/2)).^2;

    // weighting functions
    double Lp= (Lsample+Lstd)/2.0;
    double Cp= (Cpstd+Cpsample)/2.0;

    // Average Hue Computation
    // This is equivalent to that in the paper but simpler programmatically.
    // Note average hue is computed in radians and converted to degrees only
    // where needed
    double hp= (hpstd+hpsample)/2.0;
    // Identify positions for which abs hue diff exceeds 180 degrees
    if ( fabs(hpstd-hpsample)  > MATH_PI ) hp-= MATH_PI;
    // rollover ones that come -ve
    if (hp<0) hp+= MATH_2_MUL_PI;

    // Check if one of the chroma values is zero, in which case set
    // mean hue to the sum which is equivalent to other value
    if (Cpprod==0.0) hp= hpsample+hpstd;

    double Lpm502= (Lp-50.0)*(Lp-50.0);;
    double Sl= 1.0+0.015*Lpm502/sqrt(20.0+Lpm502);
    double Sc= 1.0+0.045*Cp;
    double T= 1.0 - 0.17*cos(hp - MATH_PI/6.0) + 0.24*cos(2.0*hp) + 0.32*cos(3.0*hp+MATH_PI/30.0) - 0.20*cos(4.0*hp-63.0*MATH_PI/180.0);
    double Sh= 1.0 + 0.015*Cp*T;
    double delthetarad= (30.0*MATH_PI/180.0)*exp(- pow(( (180.0/MATH_PI*hp-275.0)/25.0),2.0));
    double Rc=  2.0*sqrt(pow(Cp,7.0)/(pow(Cp,7.0) + pow(25.0,7.0)));
    double RT= -sin(2.0*delthetarad)*Rc;

    // The CIE 00 color difference
    return sqrt( pow((dL/Sl),2.0) + pow((dC/Sc),2.0) + pow((dH/Sh),2.0) + RT*(dC/Sc)*(dH/Sh) );
}

/* ======================================================================== */
