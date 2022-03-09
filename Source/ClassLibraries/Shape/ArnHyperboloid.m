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

#define ART_MODULE_NAME     ArnHyperboloid

#import "ArnHyperboloid.h"

Vec3D ARNHYPERBOLOID_OBJECTSPACE_NORMAL_TABLE[3];

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArnHyperboloid

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnHyperboloid)
ARPSHAPE_DEFAULT_IMPLEMENTATION(0,0,3)

- (void) getLocalExtremalCoordinates
        : (const Vec3D *) inDirection
        : (Crd4 *) minCrd
        : (Crd4 *) maxCrd
{
    Vec3D extremalVector =
    VEC3D(
        XC(*inDirection),
        YC(*inDirection),
        M_SIGN(ZC(*inDirection))
    );
    double squaredLength =   M_SQR(XC(extremalVector))
                           + M_SQR(YC(extremalVector));

    if (squaredLength > 0.0)
    {
        double r = sqrt(1.0 + M_SIGN(parameter) * M_SQR(parameter));
        double inverseLength = r * 1.0 / sqrt(squaredLength);
        XC(extremalVector) *= inverseLength;
        YC(extremalVector) *= inverseLength;
    }

    if (minCrd)
    {
        Pnt3D  point;

        pnt3d_vp_sub_p(
            & extremalVector,
            & PNT3D_ZERO,
            & point
            );

        c4_p3_to_c(
            & point,
              minCrd
            );
    }
    if (maxCrd)
    {
        Pnt3D  point;

        pnt3d_vp_add_p(
            & extremalVector,
            & PNT3D_ZERO,
            & point
            );

        c4_p3_to_c(
            & point,
              maxCrd
            );
    }
}

- (void) getLocalCentroid
        : (const ArTraversalState *) traversalState
        : (Pnt3D *) outCentroid
{
    (void) traversalState;
    
    *outCentroid = PNT3D_ZERO;
}

- (void) calculateNormalForLocalPnt3DE
        : (const ArTraversalState *) traversalState
        : (Pnt3DE *) point
{
    (void) traversalState;
    (void) point;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

- (ArNode <ArpMapping> *) createMappingFor
        : (ArMappingCriteria) criteria
{
    (void) criteria;
    
    return
        [ ALLOC_INIT_OBJECT(ArnHyperboloidQuadrangularMapping)
            :   HARD_NODE_REFERENCE(self)
            ];
}

@end

#define PARAMETER \
((ArnParameterShape *)ARNODEREF_POINTER(subnodeRef))->parameter

//   This #import #defines the IN_3D_..., OUT_3D_..., etc. macros used
//   as shortcuts / code beatification by the mapping code below.

//   NEVER include these macros in a header file - they are ONLY intended
//   for use within the .m files where 2D<->3D mappings are implemented!

#import "ArSurfacePointMappingMacros.h"

@implementation ArnHyperboloidQuadrangularMapping

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnHyperboloidQuadrangularMapping)

ARPMAPPING_DEFAULT_IMPLEMENTATION(3)
ARPMAPPING_PATCH_PARAMETERISATION(ArcRectangularParameterisation)
ARPMAPPING_ONE_TO_ONE_PATCHING
ARPMAPPING_NO_ADJACENT_PATCHES

- (BOOL) getPnt2DE_for_ObjectSpacePnt3DE
        : (const ArTraversalState *) traversalState
        : (const Pnt3DE *) inPoint3D
        : (Pnt2DE *) outPoint2D
{
    (void) traversalState;
    
    OUT_2D_PATCHINDEX = IN_3D_FACEINDEX;

    OUT_2D_U = MATH_1_DIV_2_PI * atan2( IN_3D_YC, IN_3D_XC );

    if ( PARAMETER < 0.0 || IN_3D_FACEINDEX != 0)
    {
        double r =  sqrt( M_SQR(IN_3D_XC) + M_SQR(IN_3D_YC) )
                  / sqrt( 1.0 + M_SIGN(PARAMETER) * M_SQR(PARAMETER) );

        switch ( IN_3D_FACEINDEX )
        {
            case 0:
            {
                OUT_2D_V = 0.5 + 0.5 * r * M_SIGN(IN_3D_ZC);
                break;
            }
            case 1:
            {
                OUT_2D_V = 1.0 - r;
                break;
            }
            case 2:
            {
                OUT_2D_V = r;
                break;
            }
        }
    }
    else
        OUT_2D_V = 0.5 + 0.5 * IN_3D_ZC;

    return YES;
}

- (BOOL) getObjectSpacePnt3DE_for_Pnt2DE
        : (const ArTraversalState *) traversalState
        : (const Pnt2DE *) inPoint2D
        : (Pnt3DE *) outPoint3D
{
    (void) traversalState;
    (void) inPoint2D;
    (void) outPoint3D;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
    
    return NO;
}

@end

// ===========================================================================
