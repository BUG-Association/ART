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

#define ART_MODULE_NAME     ArnHTrafo3D

#import "ArnHTrafo3D.h"

#import "ArnTrafoPair.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnHTrafo3D registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


ArnHTrafo3D * create_arnhtrafo3d(
        ART_GV    * art_gv,
        HTrafo3D  * newForward,
        HTrafo3D  * newBackward
        )
{
    return
        [ ALLOC_INIT_OBJECT(ArnHTrafo3D)
            :  *newForward
            :  *newBackward
            ];
}

@implementation ArnHTrafo3D

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnHTrafo3D)

- (id) init
{
    self = [ super init ];

    if ( self )
    {
        forward  = HTRAFO3D_UNIT;
        backward = HTRAFO3D_UNIT;
    }
    
    return self;
}

- (id) init
        : (HTrafo3D) newForward
        : (HTrafo3D) newBackward
{
    self = [ super init ];
    
    if ( self )
    {
        forward  = newForward;
        backward = newBackward;
    }
    
    return self;
}

- (id) initFrom
        : (const HTrafo3D *) newForward
        : (const HTrafo3D *) newBackward
{
    self = [ super init ];

    if ( self )
    {
        forward  = *newForward;
        backward = *newBackward;
    }
    
    return self;
}

- (id) copy
{
    ArnHTrafo3D  * copiedInstance = [ super copy ];

    copiedInstance->forward  = forward;
    copiedInstance->backward = backward;

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnHTrafo3D  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    copiedInstance->forward  = forward;
    copiedInstance->backward = backward;

    return copiedInstance;
}

- (void) backtrafoPnt3D
        : (const Pnt3D *) inPoint
        : (Pnt3D *) outPoint
{
    pnt3d_p_htrafo3d_p(
          inPoint,
        & backward,
          outPoint
        );
}

- (void) backtrafoVec3D
        : (const Vec3D *) inVector
        : (Vec3D *) outVector
{
    vec3d_v_htrafo3d_v(
          inVector,
        & backward,
          outVector
        );
}

- (void) backtrafoRay3D
        : (const Ray3D *) inRay
        : (Ray3D *) outRay
{
    ray3d_r_htrafo3d_r(
          inRay,
        & backward,
          outRay
        );
}

- (void) backtrafoNormalVec3D
        : (const Vec3D *) inNormal
        : (Vec3D *) outNormal
{
    vec3d_n_htrafo3d_n(
          inNormal,
        & forward,
          outNormal
        );
}

- (void) transformPnt3D
        : (const Pnt3D *) inPoint
        : (Pnt3D *) outPoint
{
    pnt3d_p_htrafo3d_p(
          inPoint,
        & forward,
          outPoint
        );
}

- (void) transformVec3D
        : (const Vec3D *) inVector
        : (Vec3D *) outVector
{
    vec3d_v_htrafo3d_v(
          inVector,
        & forward,
          outVector
        );
}

- (void) transformRay3D
        : (const Ray3D *) inRay
        : (Ray3D *) outRay
{
    ray3d_r_htrafo3d_r(
          inRay,
        & forward,
          outRay
        );
}

- (void) transformNormalVec3D
        : (const Vec3D *) inNormal
        : (Vec3D *) outNormal
{
    vec3d_n_htrafo3d_n(
          inNormal,
        & backward,
          outNormal
        );
}

- (void) getHTrafo3Ds
        : (HTrafo3D *) outForward
        : (HTrafo3D *) outBackward
{
    *outForward  = forward;
    *outBackward = backward;
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

    trafo3d_hh_mul_h(
        & forward,
        & otherForward,
        & newForward
        );

    trafo3d_hh_mul_h(
        & otherBackward,
        & backward,
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

    [ coder codeHTrafo3D
        : & forward
        ];

    [ coder codeHTrafo3D
        : & backward
        ];
}

@end

// ===========================================================================
