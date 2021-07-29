/* ===========================================================================

 Copyright (c) 1996-2021 The ART Development Team
 -------------------------------------------

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

#include "ART_Foundation.h"

ART_MODULE_INTERFACE(ArnPDFSampling)

#import "ART_Scenegraph.h"

/* ===========================================================================
    'ArnDiracPDF'
        This is the Dirac delta function at position 0.
=========================================================================== */

@interface ArnDiracPDF
        : ArNode < ArpConcreteClass, ArpPDF >
{
}

@end


/* ===========================================================================
    'ArnUniformPDF'
        A uniform distribution in the interval [0, [1.  The upper end of the
        interval has been excluded, since this can be useful in some cases.
=========================================================================== */

@interface ArnUniformPDF
        : ArNode < ArpConcreteClass, ArpPDF >
{
}

@end

/* ===========================================================================
    'ArnBlinnPDF2D'
        Blinn D1 (i.e., cosine lobe) PDF. See PBR book.
=========================================================================== */

@interface ArnBlinnPDF
        : ArNode <ArpConcreteClass, ArpPDF >
{
    double exponent;
}

- (id) init
        : (double) newExponent
        ;

@end

/* ===========================================================================
    'ArnHTSGAdaptedPDF'
=========================================================================== */

@interface ArnHTSGAdaptedPDF
        : ArNode < ArpConcreteClass, ArpPDF >
{
    double tau;
    double sigma0;
    double lambdaAvg;
    double d_omega_i;
}

- (id) init
        : (double) newTau
        : (double) newSigma0
        : (double) newLambdaAvg
        : (double) newD_omega_i
        ;

@end

