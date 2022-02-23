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

#define ART_MODULE_NAME     ArnTorus

#import "ArnTorus.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnTorus registerWithRuntime ];
    [ ArnTorusQuadrangularMapping registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArnTorus

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnTorus)
ARPSHAPE_DEFAULT_IMPLEMENTATION(0,0,1)

- (void) getLocalCentroid
        : (const ArTraversalState *) traversalState
        : (Pnt3D *) outCentroid
{
    *outCentroid = PNT3D_ZERO;
}

- (void) getExtremalPoints
        : (const Vec3D *) inDirection
        : (Pnt3D *) minPnt
        : (Pnt3D *) maxPnt
{
    Vec3D circleVector =
        VEC3D(
            XC(*inDirection),
            YC(*inDirection),
            0.0
            );

    double squaredLength =
        M_SQR( XC(circleVector) ) + M_SQR( YC(circleVector) );

    if (squaredLength > 0.0)
    {
        double inverseLength =
            1.0 / sqrt(squaredLength);

        XC(circleVector) *= inverseLength;      // BUGFIX: rft 13-Oct-1997
        YC(circleVector) *= inverseLength;      // BUGFIX: rft 13-Oct-1997
    }

    Vec3D  extremalVector;

    vec3d_v_norm_v(
          inDirection,
        & extremalVector
        );

    vec3d_dv_mul_add_v(
          parameter,
        & extremalVector,
        & circleVector
        );

    if ( minPnt )
        pnt3d_vp_sub_p(
            & circleVector,
            & PNT3D_ZERO,
              minPnt
            );

    if ( maxPnt )
        pnt3d_vp_add_p(
            & circleVector,
            & PNT3D_ZERO,
              maxPnt
            );
}


- (void) calculateNormalForLocalPnt3DE
        : (const ArTraversalState *) traversalState
        : (Pnt3DE *) point
{
    Pnt3D  circlePoint = PNT3DE_COORD(*point);

    ZC(circlePoint) = 0.0;

    vec3d_norm_v(
        (Vec3D *) & circlePoint
        );

    vec3d_pp_sub_v(
        & PNT3DE_COORD(*point),
        & circlePoint,
        & PNT3DE_NORMAL(*point)
        );

    if (   parameter > 1.0
        && vec3d_v_sqrlen( & PNT3DE_NORMAL(*point) ) < M_SQR(parameter) )
    {
        vec3d_d_mul_v(
            2.0,
            (Vec3D *) & circlePoint
            );

        vec3d_v_add_v(
            (Vec3D *) & circlePoint,
            & PNT3DE_NORMAL(*point)
            );
    }
}

- (ArNode <ArpMapping> *) createMappingFor
        : (ArMappingCriteria) criteria
{
    return
        [ ALLOC_INIT_OBJECT(ArnTorusQuadrangularMapping)
            :   HARD_NODE_REFERENCE(self)
            ];
}

@end

//   This #import #defines the IN_3D_..., OUT_3D_..., etc. macros used
//   as shortcuts / code beatification by the mapping code below.

#import "ArSurfacePointMappingMacros.h"

#define PARAMETER \
((ArnParameterShape *)ARNODEREF_POINTER(subnodeRef))->parameter

@implementation ArnTorusQuadrangularMapping

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnTorusQuadrangularMapping)

ARPMAPPING_DEFAULT_IMPLEMENTATION(1)
ARPMAPPING_PATCH_PARAMETERISATION(ArcRectangularParameterisation)
ARPMAPPING_ONE_TO_ONE_PATCHING
ARPMAPPING_NO_ADJACENT_PATCHES

- (BOOL) getPnt2DE_for_ObjectSpacePnt3DE
        : (const ArTraversalState *) traversalState
        : (const Pnt3DE *) inPoint3D
        : (Pnt2DE *) outPoint2D
{
    double u, cos_u, sin_u;
    Pnt3D pnt = IN_3D_COORD;

    OUT_2D_PATCHINDEX = IN_3D_FACEINDEX;
    u = atan2(-YC(pnt),-XC(pnt));
    cos_u = cos(MATH_PI - u);
    sin_u = sin(MATH_PI - u);
    c3_dd_rot2_mul_c(cos_u, sin_u, &pnt.c);
    XC(pnt) -= 1.0;

    OUT_2D_U = 0.5 + MATH_1_DIV_2_PI * u;
    OUT_2D_V = 0.5 + MATH_1_DIV_2_PI * atan2(-ZC(pnt),-XC(pnt));

        return YES;
}

- (BOOL) getObjectSpacePnt3DE_for_Pnt2DE
        : (const ArTraversalState *) traversalState
        : (const Pnt2DE *) inPoint2D
        : (Pnt3DE *) outPoint3D
{
    double  rv0 = IN_2D_U * MATH_2_MUL_PI;
    double  sr  = PARAMETER * cos(IN_2D_V * MATH_2_MUL_PI);

    OUT_3D_FACEINDEX = IN_2D_PATCHINDEX;

    OUT_3D_XC = cos(rv0) * (1 + sr);
    OUT_3D_YC = sin(rv0) * (1 + sr);
    OUT_3D_ZC = PARAMETER * sin(IN_2D_V * MATH_2_MUL_PI);

    return YES;
}

@end

// ===========================================================================
