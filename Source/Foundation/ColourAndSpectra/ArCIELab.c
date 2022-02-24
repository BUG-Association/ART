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

#define ART_MODULE_NAME     ArCIELab

#include "ArCIELab.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


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

void lab_c_debugprintf(
        const ART_GV    * art_gv,
        const ArCIELab  * c_0
        )
{
    (void) art_gv;
    
    printf( "CIE L*a*b* ( % 5.3f, % 5.3f, % 5.3f )\n",
        ARCIELab_L(*c_0),
        ARCIELab_a(*c_0),
        ARCIELab_b(*c_0)
        );

    fflush(stdout);
}

/* ======================================================================== */
