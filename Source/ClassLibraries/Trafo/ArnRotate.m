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

#define ART_MODULE_NAME     ArnRotate

#import "ArnRotate.h"

#import "ArnHTrafo3D.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnRotX3D registerWithRuntime ];
    [ ArnRotY3D registerWithRuntime ];
    [ ArnRotZ3D registerWithRuntime ];
    [ ArnRot3D  registerWithRuntime ];
    [ ArnVarRotX3D registerWithRuntime ];
    [ ArnVarRotY3D registerWithRuntime ];
    [ ArnVarRotZ3D registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArnRotX3D

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnRotX3D)

- (id) init
{
    self = [ super init ];
    
    if ( self )
    {
        rot = ROTX3D( 0.0 );
    }
    
    return self;
}

- (id) copy
{
    ArnRotX3D  * copiedInstance = [ super copy ];

    copiedInstance->rot = rot;

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnRotX3D  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    copiedInstance->rot = rot;

    return copiedInstance;
}

- (void) _setupRotX3D
{
    rot.c = cos(rot.a);
    rot.s = sin(rot.a);
}

- (id) init
        : (double) alpha
{
    self = [ super init ];
    
    if ( self )
    {
        rot.a = alpha;
        [self _setupRotX3D];
    }
    
    return self;
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [super code :coder];
    [coder codeDouble : &ALPHA(rot)];
    if ([coder isReading]) [self _setupRotX3D];
}

- (void) backtrafoPnt3D
        : (const Pnt3D *) inPoint
        : (Pnt3D *) outPoint
{
    pnt3d_p_inv_rotx_p(inPoint, &rot, outPoint);
}

- (void) backtrafoVec3D
        : (const Vec3D *) inVector
        : (Vec3D *) outVector
{
    vec3d_v_inv_rotx_v(inVector, &rot, outVector);
}

- (void) backtrafoRay3D
        : (const Ray3D *) inRay
        : (Ray3D *) outRay
{
    ray3d_r_inv_rotx_r(inRay, &rot, outRay);
}

- (void) backtrafoNormalVec3D
        : (const Vec3D *) inNormal
        : (Vec3D *) outNormal
{
    vec3d_v_inv_rotx_v(inNormal, &rot, outNormal);
}

- (void) transformPnt3D
        : (const Pnt3D *) inPoint
        : (Pnt3D *) outPoint
{
    pnt3d_p_rotx_p(inPoint, &rot, outPoint);
}

- (void) transformVec3D
        : (const Vec3D *) inVector
        : (Vec3D *) outVector
{
    vec3d_v_rotx_v(inVector, &rot, outVector);
}

- (void) transformRay3D
        : (const Ray3D *) inRay
        : (Ray3D *) outRay
{
    ray3d_r_rotx_r(inRay, &rot, outRay);
}

- (void) transformNormalVec3D
        : (const Vec3D *) inNormal
        : (Vec3D *) outNormal
{
    vec3d_v_rotx_v(inNormal, &rot, outNormal);
}

- (void) getHTrafo3Ds
        : (HTrafo3D *) outForward
        : (HTrafo3D *) outBackward
{
    trafo3d_rotx_to_h(&rot, outForward);
    trafo3d_rotx_inv_to_h(&rot, outBackward);
}

- (ArNode <ArpTrafo3D> *) reduceToSingleHTrafo3D
{
    return self;
}

- (ArNode <ArpTrafo3D> *) mul
        : (ArNode <ArpTrafo3D> *) other
{
    HTrafo3D  otherForward;
    HTrafo3D  otherBackward;

    [ other getHTrafo3Ds
        : & otherForward
        : & otherBackward
        ];

    HTrafo3D  newForward;
    HTrafo3D  newBackward;

    trafo3d_rotx_h_mul_h(
        & rot,
        & otherForward,
        & newForward
        );

    trafo3d_h_rotx_inv_mul_h(
        & otherBackward,
        & rot,
        & newBackward
        );

    return
        create_arnhtrafo3d(
              art_gv,
            & newForward,
            & newBackward
            );
}

@end

/* ===========================================================================
    'ArnRotY3D'
=========================================================================== */
@implementation ArnRotY3D

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnRotY3D)

- (id) init
{
    self = [ super init ];
    
    if ( self )
    {
        rot = ROTY3D( 0.0 );
    }
    
    return self;
}

- (id) copy
{
    ArnRotY3D  * copiedInstance = [ super copy ];

    copiedInstance->rot = rot;

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnRotY3D  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    copiedInstance->rot = rot;

    return copiedInstance;
}

- (void) _setupRotY3D
{
    rot.c = cos(rot.a);
    rot.s = sin(rot.a);
}

- (id) init
        : (double) alpha
{
    self = [ super init ];
    
    if ( self )
    {
        rot.a = alpha;
        [self _setupRotY3D];
    }
    
    return self;
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [super code :coder];
    [coder codeDouble : &ALPHA(rot)];
    if ([coder isReading]) [self _setupRotY3D];
}

- (void) backtrafoPnt3D
        : (const Pnt3D *) inPoint
        : (Pnt3D *) outPoint
{
    pnt3d_p_inv_roty_p(inPoint, &rot, outPoint);
}

- (void) backtrafoVec3D
        : (const Vec3D *) inVector
        : (Vec3D *) outVector
{
    vec3d_v_inv_roty_v(inVector, &rot, outVector);
}

- (void) backtrafoRay3D
        : (const Ray3D *) inRay
        : (Ray3D *) outRay
{
    ray3d_r_inv_roty_r(inRay, &rot, outRay);
}

- (void) backtrafoNormalVec3D
        : (const Vec3D *) inNormal
        : (Vec3D *) outNormal
{
    vec3d_v_inv_roty_v(inNormal, &rot, outNormal);
}

- (void) transformPnt3D
        : (const Pnt3D *) inPoint
        : (Pnt3D *) outPoint
{
    pnt3d_p_roty_p(inPoint, &rot, outPoint);
}

- (void) transformVec3D
        : (const Vec3D *) inVector
        : (Vec3D *) outVector
{
    vec3d_v_roty_v(inVector, &rot, outVector);
}

- (void) transformRay3D
        : (const Ray3D *) inRay
        : (Ray3D *) outRay
{
    ray3d_r_roty_r(inRay, &rot, outRay);
}

- (void) transformNormalVec3D
        : (const Vec3D *) inNormal
        : (Vec3D *) outNormal
{
    vec3d_v_roty_v(inNormal, &rot, outNormal);
}

- (void) getHTrafo3Ds
        : (HTrafo3D *) outForward
        : (HTrafo3D *) outBackward
{
    trafo3d_roty_to_h(&rot, outForward);
    trafo3d_roty_inv_to_h(&rot, outBackward);
}

- (ArNode <ArpTrafo3D> *) reduceToSingleHTrafo3D
{
    return self;
}

- (ArNode <ArpTrafo3D> *) mul
        : (ArNode <ArpTrafo3D> *) other
{
    HTrafo3D  otherForward;
    HTrafo3D  otherBackward;

    [ other getHTrafo3Ds
        : & otherForward
        : & otherBackward
        ];

    HTrafo3D  newForward;
    HTrafo3D  newBackward;

    trafo3d_roty_h_mul_h(
        & rot,
        & otherForward,
        & newForward
        );

    trafo3d_h_roty_inv_mul_h(
        & otherBackward,
        & rot,
        & newBackward
        );

    return
        create_arnhtrafo3d(
              art_gv,
            & newForward,
            & newBackward
            );
}

@end

/* ===========================================================================
    'ArnRotZ3D'
=========================================================================== */
@implementation ArnRotZ3D

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnRotZ3D)

- (id) init
{
    self  = [ super init ];
    
    if ( self )
    {
        rot = ROTZ3D( 0.0 );
    }
    
    return self;
}

- (id) copy
{
    ArnRotZ3D  * copiedInstance = [ super copy ];

    copiedInstance->rot = rot;

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnRotZ3D  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    copiedInstance->rot = rot;

    return copiedInstance;
}

- (void) _setupRotZ3D
{
    rot.c = cos(rot.a);
    rot.s = sin(rot.a);
}

- (id) init
        : (double) alpha
{
    self = [ super init ];
    
    if ( self )
    {
        rot.a = alpha;
        [self _setupRotZ3D];
    }
    return self;
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [super code :coder];
    [coder codeDouble : &ALPHA(rot)];
    if ([coder isReading]) [self _setupRotZ3D];
}

- (void) backtrafoPnt3D
        : (const Pnt3D *) inPoint
        : (Pnt3D *) outPoint
{
    pnt3d_p_inv_rotz_p(inPoint, &rot, outPoint);
}

- (void) backtrafoVec3D
        : (const Vec3D *) inVector
        : (Vec3D *) outVector
{
    vec3d_v_inv_rotz_v(inVector, &rot, outVector);
}

- (void) backtrafoRay3D
        : (const Ray3D *) inRay
        : (Ray3D *) outRay
{
    ray3d_r_inv_rotz_r(inRay, &rot, outRay);
}

- (void) backtrafoNormalVec3D
        : (const Vec3D *) inNormal
        : (Vec3D *) outNormal
{
    vec3d_v_inv_rotz_v(inNormal, &rot, outNormal);
}

- (void) transformPnt3D
        : (const Pnt3D *) inPoint
        : (Pnt3D *) outPoint
{
    pnt3d_p_rotz_p(inPoint, &rot, outPoint);
}

- (void) transformVec3D
        : (const Vec3D *) inVector
        : (Vec3D *) outVector
{
    vec3d_v_rotz_v(inVector, &rot, outVector);
}

- (void) transformRay3D
        : (const Ray3D *) inRay
        : (Ray3D *) outRay
{
    ray3d_r_rotz_r(inRay, &rot, outRay);
}

- (void) transformNormalVec3D
        : (const Vec3D *) inNormal
        : (Vec3D *) outNormal
{
    vec3d_v_rotz_v(inNormal, &rot, outNormal);
}

- (void) getHTrafo3Ds
        : (HTrafo3D *) outForward
        : (HTrafo3D *) outBackward
{
    trafo3d_rotz_to_h(&rot, outForward);
    trafo3d_rotz_inv_to_h(&rot, outBackward);
}

- (ArNode <ArpTrafo3D> *) reduceToSingleHTrafo3D
{
    return self;
}

- (ArNode <ArpTrafo3D> *) mul
        : (ArNode <ArpTrafo3D> *) other
{
    HTrafo3D  otherForward;
    HTrafo3D  otherBackward;

    [ other getHTrafo3Ds
        : & otherForward
        : & otherBackward
        ];

    HTrafo3D  newForward;
    HTrafo3D  newBackward;

    trafo3d_rotz_h_mul_h(
        & rot,
        & otherForward,
        & newForward
        );

    trafo3d_h_rotz_inv_mul_h(
        & otherBackward,
        & rot,
        & newBackward
        );

    return
        create_arnhtrafo3d(
              art_gv,
            & newForward,
            & newBackward
            );
}

@end

/* ===========================================================================
    'ArnRot3D'
=========================================================================== */
@implementation ArnRot3D

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnRot3D)

- (id) init
{
    self = [ super init ];
    
    if ( self )
    {
        rot = ROT3D(VEC3D(1,0,0), 0);
    }
    
    return self;
}

- (id) init
        : (Rot3D) newRot
{
    self = [super init];
    
    if ( self )
    {
        rot = newRot;
    }
    
    return self;
}

- (id) copy
{
    ArnRot3D  * copiedInstance = [ super copy ];

    copiedInstance->rot = rot;

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnRot3D  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    copiedInstance->rot = rot;

    return copiedInstance;
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [super code :coder];
    [coder codeRot3D :&rot];
}

- (void) backtrafoPnt3D
        : (const Pnt3D *) inPoint
        : (Pnt3D *) outPoint
{
    pnt3d_p_inv_rot_p(inPoint, &rot, outPoint);
}

- (void) backtrafoVec3D
        : (const Vec3D *) inVector
        : (Vec3D *) outVector
{
    vec3d_v_inv_rot_v(inVector, &rot, outVector);
}

- (void) backtrafoRay3D
        : (const Ray3D *) inRay
        : (Ray3D *) outRay
{
    ray3d_r_inv_rot_r(inRay, &rot, outRay);
}

- (void) backtrafoNormalVec3D
        : (const Vec3D *) inNormal
        : (Vec3D *) outNormal
{
    vec3d_v_inv_rot_v(inNormal, &rot, outNormal);
}

- (void) transformPnt3D
        : (const Pnt3D *) inPoint
        : (Pnt3D *) outPoint
{
    pnt3d_p_rot_p(inPoint, &rot, outPoint);
}

- (void) transformVec3D
        : (const Vec3D *) inVector
        : (Vec3D *) outVector
{
    vec3d_v_rot_v(inVector, &rot, outVector);
}

- (void) transformRay3D
        : (const Ray3D *) inRay
        : (Ray3D *) outRay
{
    ray3d_r_rot_r(inRay, &rot, outRay);
}

- (void) transformNormalVec3D
        : (const Vec3D *) inNormal
        : (Vec3D *) outNormal
{
    vec3d_v_rot_v(inNormal, &rot, outNormal);
}

- (void) getHTrafo3Ds
        : (HTrafo3D *) outForward
        : (HTrafo3D *) outBackward
{
    trafo3d_rot_to_h(&rot, outForward);
    trafo3d_rot_inv_to_h(&rot, outBackward);
}

- (ArNode <ArpTrafo3D> *) reduceToSingleHTrafo3D
{
    return self;
}

- (ArNode <ArpTrafo3D> *) mul
        : (ArNode <ArpTrafo3D> *) other
{
    HTrafo3D  otherForward;
    HTrafo3D  otherBackward;

    [ other getHTrafo3Ds
        : & otherForward
        : & otherBackward
        ];

    HTrafo3D  newForward;
    HTrafo3D  newBackward;

    trafo3d_h_rot_mul_h(
        & otherForward,
        & rot,
        & newForward
        );

    trafo3d_h_rot_inv_mul_h(
        & otherBackward,
        & rot,
        & newBackward
        );

    return
        create_arnhtrafo3d(
              art_gv,
            & newForward,
            & newBackward
            );
}

@end

#define ROTATION_ALPHA \
((ArNode <ArpDoubleValues> *) ARNODEREF_POINTER(subnodeRef))

/* ===========================================================================
    'ArnVarRotX3D'
=========================================================================== */
@implementation ArnVarRotX3D

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnVarRotX3D)

- (void) _checkVarRotX3D
{
}

- (id) init
        : (ArNode *) alpha
{
    self = [ super init : HARD_NODE_REFERENCE(alpha) ];
    
    if ( self )
    {
        [ self _checkVarRotX3D ];
    }
    
    return self;
}

- (ArNode <ArpTrafo3D> *) evaluateToFixedTrafo3D
        : (ArcObject <ArpEvaluationEnvironment> *) env
{
    double alpha;

    [ ROTATION_ALPHA getDoubleValues
        :   env
        :   1
        : & alpha
        ];

    return
        [ ALLOC_INIT_OBJECT(ArnRotX3D)
            :  alpha
            ];
}

@end

/* ===========================================================================
    'ArnVarRotY3D'
=========================================================================== */
@implementation ArnVarRotY3D

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnVarRotY3D)

- (void) _checkVarRotY3D
{
}

- (id) init
        : (ArNode *) alpha
{
    self = [ super init : HARD_NODE_REFERENCE(alpha) ];
    
    if ( self )
    {
        [ self _checkVarRotY3D ];
    }
    
    return self;
}

- (ArNode <ArpTrafo3D> *) evaluateToFixedTrafo3D
        : (ArcObject <ArpEvaluationEnvironment> *) env
{
    double alpha;

    [ ROTATION_ALPHA getDoubleValues
        :   env
        :   1
        : & alpha
        ];

    return
        [ ALLOC_INIT_OBJECT(ArnRotY3D)
            :  alpha
            ];
}

@end

/* ===========================================================================
    'ArnVarRotZ3D'
=========================================================================== */
@implementation ArnVarRotZ3D

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnVarRotZ3D)

- (void) _checkVarRotZ3D
{
}

- (id) init
        : (ArNode *) alpha
{
    self = [ super init : HARD_NODE_REFERENCE(alpha) ];
    
    if ( self )
    {
        [ self _checkVarRotZ3D ];
    }
    
    return self;
}

- (ArNode <ArpTrafo3D> *) evaluateToFixedTrafo3D
        : (ArcObject <ArpEvaluationEnvironment> *) env
{
    double alpha;

    [ ROTATION_ALPHA getDoubleValues
        :   env
        :   1
        : & alpha
        ];

    return
        [ ALLOC_INIT_OBJECT(ArnRotZ3D)
            :  alpha
            ];
}

@end

// ===========================================================================
