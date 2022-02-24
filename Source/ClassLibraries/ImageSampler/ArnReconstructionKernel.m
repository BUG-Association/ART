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

#define ART_MODULE_NAME     ArnReconstructionKernel

#import "ArnReconstructionKernel.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnBoxReconstructionKernel  registerWithRuntime ];
    [ ArnTentReconstructionKernel registerWithRuntime ];
    [ ArnCookReconstructionKernel registerWithRuntime ];
    [ ArnSincReconstructionKernel registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArnBoxReconstructionKernel

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnBoxReconstructionKernel)

- (unsigned int) supportSize
{
    return 1;
}

- (double) valueAt
        : (Pnt2D *) samplingPoint
{
    return 1.0;
}

- (double) edgeCorrectionFactor
        : (IPnt2D *) distanceFromEdge
{
    return 1.0;
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];
}

@end


@implementation ArnTentReconstructionKernel

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnTentReconstructionKernel)

- (unsigned int) supportSize
{
    return 3;
}

- (double) valueAt
        : (Pnt2D *) samplingPoint
{
    if (   M_ABS( XC(*samplingPoint) ) <= 1.0
        && M_ABS( YC(*samplingPoint) ) <= 1.0 )
        return   ( 1.0 - M_ABS( XC(*samplingPoint) ) )
               * ( 1.0 - M_ABS( YC(*samplingPoint) ) );
    else
        return 0;
}

- (double) edgeCorrectionFactor
        : (IPnt2D *) distanceFromEdge
{
    return 1.0;
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];
}

@end

@implementation ArnCookReconstructionKernel

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnCookReconstructionKernel)

- (unsigned int) supportSize
{
    return 5;
}

#define W    1.0
#define W2   W*W

- (double) valueAt
        : (Pnt2D *) samplingPoint
{
    double  x2 = M_SQR( XC(*samplingPoint) ) + M_SQR( YC(*samplingPoint) );

    if ( x2 <= W2 )
        return exp(-x2) - exp(-W2);
    else
        return 0;
}

- (double) edgeCorrectionFactor
        : (IPnt2D *) distanceFromEdge
{
    (void) distanceFromEdge;
    
    return 1.0;
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];
}

@end

@implementation ArnMitchellNetravaliReconstructionKernel

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnMitchellNetravaliReconstructionKernel)

- (id) parameterB
                   : (double) newParameterB
        parameterC : (double) newParameterC
{
    return [ self init: newParameterB : newParameterC ];
}

- (id) init
        : (double) newParameterB
        : (double) newParameterC
{
    self = [ super init ];

    if ( self )
    {
        parameterB = newParameterB;
        parameterC = newParameterC;
    }
    
    return self;
}

- (double) edgeCorrectionFactor
        : (IPnt2D *) distanceFromEdge
{
    (void) distanceFromEdge;
    
    return 1.0;
}

- (unsigned int) supportSize
{
    return 5;
}

#define B parameterB
#define C parameterC

- (double) valueAt
        : (Pnt2D *) samplingPoint
{
    double  x2 = M_SQR( XC(*samplingPoint) ) + M_SQR( YC(*samplingPoint) );

    if ( x2 > 4.0 )
        return 0.0;
    else
    {
        double x = sqrt(x2);

        if ( x > 1.0 )
            return (  (       - B -  6.0 * C ) * M_SQR(x) * x
                    + (   6.0 * B + 30.0 * C ) * M_SQR(x)
                    + ( -12.0 * B - 48.0 * C ) * x
                    + (   8.0 * B + 24.0 * C ) ) / 6.0;
        else
            return (  (  12.0 -  9.0 * B  - 6.0 * C ) * M_SQR(x) * x
                    + ( -18.0 + 12.0 * B  + 6.0 * C ) * M_SQR(x)
                    + (   6.0 -  2.0 * B ) ) / 6.0;
    }
}

#undef B
#undef C

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];

    [ coder codeDouble: & parameterB ];
    [ coder codeDouble: & parameterC ];
}

@end

@implementation ArnSincReconstructionKernel

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnSincReconstructionKernel)

- (id) supportSize
        : (unsigned int) newSupportSize
{
    return [ self init: newSupportSize ];
}

- (id) init
        : (unsigned int) newSupportSize
{
    self = [ super init ];

    if ( self )
    {
        supportSize = newSupportSize;
    }
    
    return self;
}

- (unsigned int) supportSize
{
    return supportSize;
}

- (double) valueAt
        : (Pnt2D *) samplingPoint
{
    double  x2 = M_SQR( XC(*samplingPoint) ) + M_SQR( YC(*samplingPoint) );

    if ( m_d_is_tiny(x2) )
        return 1.0;
    else
    {
        double x = MATH_2_MUL_PI * sqrt(x2);

        return sin(x) / x;
    }
}

- (double) edgeCorrectionFactor
        : (IPnt2D *) distanceFromEdge
{
    (void) distanceFromEdge;
    
    return 1.0;
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];

    [ coder codeUInt: & supportSize ];
}

@end

// ===========================================================================
