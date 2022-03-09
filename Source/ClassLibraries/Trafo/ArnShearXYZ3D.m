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

#define ART_MODULE_NAME     ArnShearXYZ3D

#import "ArnShearXYZ3D.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnShearXY3D registerWithRuntime ];
    [ ArnShearYZ3D registerWithRuntime ];
    [ ArnShearZX3D registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#import "ArnHTrafo3D.h"

/* ===========================================================================
    'ArnShearXY3D'
=========================================================================== */

@implementation ArnShearXY3D

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnShearXY3D)

- (id) init
{
    self = [ super init ];
    
    if ( self )
    {
        shear = SHEARXY3D( 0.0, 0.0 );
    }
    
    return self;
}

- (id) init
        : (ShearXY3D) newShear
{
    self = [ super init ];

    if ( self )
    {
        shear = newShear;
    }
    
    return self;
}

- (id) initFrom
        : (const ShearXY3D *) newShear
{
    self = [ super init ];

    if ( self )
    {
        shear = *newShear;
    }
    
    return self;
}

- (id) copy
{
    ArnShearXY3D  * copiedInstance = [ super copy ];

    copiedInstance->shear = shear;

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnShearXY3D  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    copiedInstance->shear = shear;

    return copiedInstance;
}

- (void) backtrafoPnt3D
        : (const Pnt3D *) inPoint
        : (Pnt3D *) outPoint
{
    pnt3d_p_inv_shearxy_p(
          inPoint,
        & shear,
          outPoint
        );
}

- (void) backtrafoVec3D
        : (const Vec3D *) inVector
        : (Vec3D *) outVector
{
    vec3d_v_inv_shearxy_v(
          inVector,
        & shear,
          outVector
        );
}

- (void) backtrafoRay3D
        : (const Ray3D *) inRay
        : (Ray3D *) outRay
{
    ray3d_r_inv_shearxy_r(
          inRay,
        & shear,
          outRay
        );
}

- (void) backtrafoNormalVec3D
        : (const Vec3D *) inNormal
        : (Vec3D *) outNormal
{
    ART_ERRORHANDLING_FATAL_ERROR(
        "backtrafoNormalVec3D not implemented"
        );

    vec3d_v_inv_shearxy_v(inNormal, &shear, outNormal); // not correct !
}

- (void) transformPnt3D
        : (const Pnt3D *) inPoint
        : (Pnt3D *) outPoint
{
    pnt3d_p_shearxy_p(
          inPoint,
        & shear,
          outPoint
        );
}

- (void) transformVec3D
        : (const Vec3D *) inVector
        : (Vec3D *) outVector
{
    vec3d_v_shearxy_v(
          inVector,
        & shear,
          outVector
        );
}

- (void) transformRay3D
        : (const Ray3D *) inRay
        : (Ray3D *) outRay
{
    ray3d_r_shearxy_r(
          inRay,
        & shear,
          outRay
        );
}

- (void) transformNormalVec3D
        : (const Vec3D *) inNormal
        : (Vec3D *) outNormal
{
    ART_ERRORHANDLING_FATAL_ERROR(
        "transformNormalVec3D not implemented"
        );

    vec3d_v_shearxy_v(inNormal, &shear, outNormal);     // not correct !
}

- (void) getHTrafo3Ds
        : (HTrafo3D *) outForward
        : (HTrafo3D *) outBackward
{
    trafo3d_sxy_to_h(&shear, outForward);
    trafo3d_sxy_inv_to_h(&shear, outBackward);
}

- (ArNode <ArpTrafo3D> *) reduceToSingleHTrafo3D
{
    return self;
}

- (ArNode <ArpTrafo3D> *) mul
        : (ArNode <ArpTrafo3D> *) other
{
    HTrafo3D  selfForward;
    HTrafo3D  selfBackward;

    [ self getHTrafo3Ds
        : & selfForward
        : & selfBackward
        ];

    HTrafo3D  otherForward;
    HTrafo3D  otherBackward;

    [ other getHTrafo3Ds
        : & otherForward
        : & otherBackward
        ];

    HTrafo3D  newForward;
    HTrafo3D  newBackward;

    trafo3d_hh_mul_h(
        & selfForward,
        & otherForward,
        & newForward
        );

    trafo3d_hh_mul_h(
        & otherBackward,
        & selfBackward,
        & newBackward
        );

    return
        create_arnhtrafo3d(
              art_gv,
            & newForward,
            & newBackward
            );
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code
        :   coder
        ];

    [ coder codeShearXY3D
        : & shear
        ];
}

@end

/* ===========================================================================
    'ArnShearYZ3D'
=========================================================================== */

@implementation ArnShearYZ3D

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnShearYZ3D)

- (id) init
{
    self = [ super init ];

    if ( self )
    {
        shear = SHEARYZ3D( 0.0, 0.0 );
    }
    
    return self;
}

- (id) init
        : (ShearYZ3D) newShear
{
    self = [ super init ];

    if ( self )
    {
        shear = newShear;
    }
    
    return self;
}

- (id) initFrom
        : (const ShearYZ3D *) newShear
{
    self = [ super init ];
    
    if ( self )
    {
        shear = *newShear;
    }
    
    return self;
}

- (id) copy
{
    ArnShearYZ3D  * copiedInstance = [ super copy ];

    copiedInstance->shear = shear;

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnShearYZ3D  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    copiedInstance->shear = shear;

    return copiedInstance;
}

- (void) backtrafoPnt3D
        : (const Pnt3D *) inPoint
        : (Pnt3D *) outPoint
{
    pnt3d_p_inv_shearyz_p(
          inPoint,
        & shear,
          outPoint
        );
}

- (void) backtrafoVec3D
        : (const Vec3D *) inVector
        : (Vec3D *) outVector
{
    vec3d_v_inv_shearyz_v(
          inVector,
        & shear,
          outVector
        );
}

- (void) backtrafoRay3D
        : (const Ray3D *) inRay
        : (Ray3D *) outRay
{
    ray3d_r_inv_shearyz_r(
          inRay,
        & shear,
          outRay
        );
}

- (void) backtrafoNormalVec3D
        : (const Vec3D *) inNormal
        : (Vec3D *) outNormal
{
    ART_ERRORHANDLING_FATAL_ERROR(
        "backtrafoNormalVec3D not implemented"
        );

    vec3d_v_inv_shearyz_v(inNormal, &shear, outNormal); // not correct !
}

- (void) transformPnt3D
        : (const Pnt3D *) inPoint
        : (Pnt3D *) outPoint
{
    pnt3d_p_shearyz_p(
          inPoint,
        & shear,
          outPoint
        );
}

- (void) transformVec3D
        : (const Vec3D *) inVector
        : (Vec3D *) outVector
{
    vec3d_v_shearyz_v(
          inVector,
        & shear,
          outVector
        );
}

- (void) transformRay3D
        : (const Ray3D *) inRay
        : (Ray3D *) outRay
{
    ray3d_r_shearyz_r(
          inRay,
        & shear,
          outRay
        );
}

- (void) transformNormalVec3D
        : (const Vec3D *) inNormal
        : (Vec3D *) outNormal
{
    ART_ERRORHANDLING_FATAL_ERROR(
        "transformNormalVec3D not implemented"
        );

    vec3d_v_shearyz_v(inNormal, &shear, outNormal);     // not correct !
}

- (void) getHTrafo3Ds
        : (HTrafo3D *) outForward
        : (HTrafo3D *) outBackward
{
    trafo3d_syz_to_h(&shear, outForward);
    trafo3d_syz_inv_to_h(&shear, outBackward);
}

- (ArNode <ArpTrafo3D> *) reduceToSingleHTrafo3D
{
    return self;
}

- (ArNode <ArpTrafo3D> *) mul
        : (ArNode <ArpTrafo3D> *) other
{
    HTrafo3D  selfForward;
    HTrafo3D  selfBackward;

    [ self getHTrafo3Ds
        : & selfForward
        : & selfBackward
        ];

    HTrafo3D  otherForward;
    HTrafo3D  otherBackward;

    [ other getHTrafo3Ds
        : & otherForward
        : & otherBackward
        ];

    HTrafo3D  newForward;
    HTrafo3D  newBackward;

    trafo3d_hh_mul_h(
        & selfForward,
        & otherForward,
        & newForward
        );

    trafo3d_hh_mul_h(
        & otherBackward,
        & selfBackward,
        & newBackward
        );

    return
        create_arnhtrafo3d(
              art_gv,
            & newForward,
            & newBackward
            );
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code
        :   coder
        ];

    [ coder codeShearYZ3D
        : & shear
        ];
}

@end

/* ===========================================================================
    'ArnShearZX3D'
=========================================================================== */

@implementation ArnShearZX3D

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnShearZX3D)

- (id) init
{
    self = [ super init ];
    
    if ( self )
    {
        shear = SHEARZX3D( 0.0, 0.0 );
    }
    
    return self;
}

- (id) init
        : (ShearZX3D) newShear
{
    self = [ super init ];

    if ( self )
    {
        shear = newShear;
    }
    
    return self;
}

- (id) initFrom
        : (const ShearZX3D *) newShear
{
    self = [ super init ];

    if ( self )
    {
        shear = *newShear;
    }
    
    return self;
}

- (id) copy
{
    ArnShearZX3D  * copiedInstance = [ super copy ];

    copiedInstance->shear = shear;

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnShearZX3D  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    copiedInstance->shear = shear;

    return copiedInstance;
}

- (void) backtrafoPnt3D
        : (const Pnt3D *) inPoint
        : (Pnt3D *) outPoint
{
    pnt3d_p_inv_shearzx_p(
          inPoint,
        & shear,
          outPoint
        );
}

- (void) backtrafoVec3D
        : (const Vec3D *) inVector
        : (Vec3D *) outVector
{
    vec3d_v_inv_shearzx_v(
          inVector,
        & shear,
          outVector
        );
}

- (void) backtrafoRay3D
        : (const Ray3D *) inRay
        : (Ray3D *) outRay
{
    ray3d_r_inv_shearzx_r(
          inRay,
        & shear,
          outRay
        );
}

- (void) backtrafoNormalVec3D
        : (const Vec3D *) inNormal
        : (Vec3D *) outNormal
{
    ART_ERRORHANDLING_FATAL_ERROR(
        "backtrafoNormalVec3D not implemented"
        );

    vec3d_v_inv_shearzx_v(inNormal, &shear, outNormal); // not correct !
}

- (void) transformPnt3D
        : (const Pnt3D *) inPoint
        : (Pnt3D *) outPoint
{
    pnt3d_p_shearzx_p(
          inPoint,
        & shear,
          outPoint
        );
}

- (void) transformVec3D
        : (const Vec3D *) inVector
        : (Vec3D *) outVector
{
    vec3d_v_shearzx_v(
          inVector,
        & shear,
          outVector
        );
}

- (void) transformRay3D
        : (const Ray3D *) inRay
        : (Ray3D *) outRay
{
    ray3d_r_shearzx_r(
          inRay,
        & shear,
          outRay
        );
}

- (void) transformNormalVec3D
        : (const Vec3D *) inNormal
        : (Vec3D *) outNormal
{
    ART_ERRORHANDLING_FATAL_ERROR(
        "transformNormalVec3D not implemented"
        );

    vec3d_v_shearzx_v(inNormal, &shear, outNormal);     // not correct !
}

- (void) getHTrafo3Ds
        : (HTrafo3D *) outForward
        : (HTrafo3D *) outBackward
{
    trafo3d_szx_to_h(&shear, outForward);
    trafo3d_szx_inv_to_h(&shear, outBackward);
}

- (ArNode <ArpTrafo3D> *) reduceToSingleHTrafo3D
{
    return self;
}

- (ArNode <ArpTrafo3D> *) mul
        : (ArNode <ArpTrafo3D> *) other
{
    HTrafo3D  selfForward;
    HTrafo3D  selfBackward;

    [ self getHTrafo3Ds
        : & selfForward
        : & selfBackward
        ];

    HTrafo3D  otherForward;
    HTrafo3D  otherBackward;

    [ other getHTrafo3Ds
        : & otherForward
        : & otherBackward
        ];

    HTrafo3D  newForward;
    HTrafo3D  newBackward;

    trafo3d_hh_mul_h(
        & selfForward,
        & otherForward,
        & newForward
        );

    trafo3d_hh_mul_h(
        & otherBackward,
        & selfBackward,
        & newBackward
        );

    return
        create_arnhtrafo3d(
              art_gv,
            & newForward,
            & newBackward
            );
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code
        :   coder
        ];

    [ coder codeShearZX3D
        : & shear
        ];
}

@end

// ===========================================================================
