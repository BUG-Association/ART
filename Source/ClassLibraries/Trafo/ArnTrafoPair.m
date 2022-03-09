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

#define ART_MODULE_NAME     ArnTrafoPair

#import "ArnTrafoPair.h"

#import "ArnHTrafo3D.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnTrafo2DPair registerWithRuntime ];
    [ ArnTrafo3DPair registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


ArNode  * create_arntrafopair(
        ART_GV    * art_gv,
        ArNodeRef    nodeRef0,
        ArNodeRef    nodeRef1
        )
{
    if (   [ ARNODEREF_POINTER(nodeRef0) conformsToArProtocol: ARPROTOCOL(ArpTrafo2D) ]
        && [ ARNODEREF_POINTER(nodeRef1) conformsToArProtocol: ARPROTOCOL(ArpTrafo2D) ] )
        return
            create_arntrafo2dpair(
                art_gv,
                nodeRef0,
                nodeRef1
                );

    if (   [ ARNODEREF_POINTER(nodeRef0) conformsToArProtocol: ARPROTOCOL(ArpTrafo3D) ]
        && [ ARNODEREF_POINTER(nodeRef1) conformsToArProtocol: ARPROTOCOL(ArpTrafo3D) ] )
        return
            create_arntrafo3dpair(
                art_gv,
                nodeRef0,
                nodeRef1
                );

    ART_ERRORHANDLING_FATAL_ERROR(
        "required transformation protocols not supported: %s / %s"
        ,  [ ARNODEREF_POINTER(nodeRef0) cStringClassName ]
        ,  [ ARNODEREF_POINTER(nodeRef1) cStringClassName ]
        );

    return 0;
}

#define TRAFO2D_0   (ArNode <ArpTrafo2D> *)ARNODEREF_POINTER(subnodeRefArray[0])
#define TRAFO2D_1   (ArNode <ArpTrafo2D> *)ARNODEREF_POINTER(subnodeRefArray[1])

/* ===========================================================================
    'ArnTrafo2DPair'
=========================================================================== */
@implementation ArnTrafo2DPair

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnTrafo2DPair)

- (id) init
        : (ArNodeRef) node0
        : (ArNodeRef) node1
{
    self =
        [ super init
            :   node0
            :   node1
            ];

    if ( self )
    {
        if (   [ TRAFO2D_0 conformsToArProtocol: ARPROTOCOL(ArpTrafo2D) ]
            && [ TRAFO2D_1 conformsToArProtocol: ARPROTOCOL(ArpTrafo2D) ] )
            return self;

        ART_ERRORHANDLING_FATAL_ERROR(
            "required protocol ArpTrafo2D not supported"
            );
    }
    
    return 0;
}

- (void) backtrafoPnt2D
        : (const Pnt2D *) inPoint
        : (Pnt2D *) outPoint
{
    Pnt2D  localPoint;

    [ TRAFO2D_1 backtrafoPnt2D
        :   inPoint
        : & localPoint
        ];

    [ TRAFO2D_0 backtrafoPnt2D
        : & localPoint
        :   outPoint
        ];
}

- (void) backtrafoVec2D
        : (const Vec2D *) inVector
        : (Vec2D *) outVector
{
    Vec2D  localVector;

    [ TRAFO2D_1 backtrafoVec2D
        :   inVector
        : & localVector
        ];

    [ TRAFO2D_0 backtrafoVec2D
        : & localVector
        :   outVector
        ];
}

- (void) backtrafoRay2D
        : (const Ray2D *) inRay
        : (Ray2D *) outRay
{
    Ray2D  localRay;

    [ TRAFO2D_1 backtrafoRay2D
        :   inRay
        : & localRay
        ];

    [ TRAFO2D_0 backtrafoRay2D
        : & localRay
        :   outRay
        ];
}

- (void) backtrafoNormalVec2D
        : (const Vec2D *) inNormal
        : (Vec2D *) outNormal
{
    Vec2D  localNormal;

    [ TRAFO2D_1 backtrafoNormalVec2D
        :   inNormal
        : & localNormal
        ];

    [ TRAFO2D_0 backtrafoNormalVec2D
        : & localNormal
        :   outNormal
        ];
}

- (void) transformPnt2D
        : (const Pnt2D *) inPoint
        : (Pnt2D *) outPoint
{
    Pnt2D  localPoint;

    [ TRAFO2D_1 transformPnt2D
        :   inPoint
        : & localPoint
        ];

    [ TRAFO2D_0 transformPnt2D
        : & localPoint
        :   outPoint
        ];
}

- (void) transformVec2D
        : (const Vec2D *) inVector
        : (Vec2D *) outVector
{
    Vec2D  localVector;

    [ TRAFO2D_1 transformVec2D
        :   inVector
        : & localVector
        ];

    [ TRAFO2D_0 transformVec2D
        : & localVector
        :   outVector
        ];
}

- (void) transformRay2D
        : (const Ray2D *) inRay
        : (Ray2D *) outRay
{
    Ray2D  localRay;

    [ TRAFO2D_1 transformRay2D
        :   inRay
        : & localRay
        ];

    [ TRAFO2D_0 transformRay2D
        : & localRay
        :   outRay
        ];
}

- (void) transformNormalVec2D
        : (const Vec2D *) inNormal
        : (Vec2D *) outNormal
{
    Vec2D  localNormal;

    [ TRAFO2D_1 transformNormalVec2D
        :   inNormal
        : & localNormal
        ];

    [ TRAFO2D_0 transformNormalVec2D
        : & localNormal
        :   outNormal
        ];
}

- (void) getHTrafo2Ds
        : (HTrafo2D *) outForward
        : (HTrafo2D *) outBackward
{
    HTrafo2D  oneForward;
    HTrafo2D  oneBackward;
    HTrafo2D  twoForward;
    HTrafo2D  twoBackward;

    [ TRAFO2D_0 getHTrafo2Ds
        : & oneForward
        : & oneBackward
        ];

    [ TRAFO2D_1 getHTrafo2Ds
        : & twoForward
        : & twoBackward
        ];

    trafo2d_hh_mul_h(&oneForward, &twoForward, outForward);
    trafo2d_hh_mul_h(&twoBackward, &oneBackward, outBackward);
}

@end

ArnTrafo2DPair * create_arntrafo2dpair(
        ART_GV     * art_gv,
        ArNodeRef    nodeRef0,
        ArNodeRef    nodeRef1
        )
{
    return
        [ ALLOC_INIT_OBJECT(ArnTrafo2DPair)
            :   nodeRef0
            :   nodeRef1
            ];
}

/* ===========================================================================
    'ArnTrafo3DPair'
=========================================================================== */

#define TRAFO3D_0   (ArNode <ArpTrafo3D> *)ARNODEREF_POINTER(subnodeRefArray[0])
#define TRAFO3D_1   (ArNode <ArpTrafo3D> *)ARNODEREF_POINTER(subnodeRefArray[1])

@implementation ArnTrafo3DPair

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnTrafo3DPair)

- (id) init
        : (ArNodeRef) node0
        : (ArNodeRef) node1
{
    self =
        [ super init
            :   node0
            :   node1
            ];
    
    if ( self )
    {
        if (   [ TRAFO3D_0 conformsToArProtocol: ARPROTOCOL(ArpTrafo3D) ]
            && [ TRAFO3D_1 conformsToArProtocol: ARPROTOCOL(ArpTrafo3D) ] )
            return self;

        ART_ERRORHANDLING_FATAL_ERROR(
            "required protocol ArpTrafo3D not supported"
            );
    }
    
    return 0;
}

- (void) backtrafoPnt3D
        : (const Pnt3D *) inPoint
        : (Pnt3D *) outPoint
{
    Pnt3D  localPoint;

    [ TRAFO3D_1 backtrafoPnt3D
        :   inPoint
        : & localPoint
        ];

    [ TRAFO3D_0 backtrafoPnt3D
        : & localPoint
        :   outPoint
        ];
}

- (void) backtrafoVec3D
        : (const Vec3D *) inVector
        : (Vec3D *) outVector
{
    Vec3D  localVector;

    [ TRAFO3D_1 backtrafoVec3D
        :   inVector
        : & localVector
        ];

    [ TRAFO3D_0 backtrafoVec3D
        : & localVector
        :   outVector
        ];
}

- (void) backtrafoRay3D
        : (const Ray3D *) inRay
        : (Ray3D *) outRay
{
    Ray3D  localRay;

    [ TRAFO3D_1 backtrafoRay3D
        :   inRay
        : & localRay
        ];

    [ TRAFO3D_0 backtrafoRay3D
        : & localRay
        :   outRay
        ];
}

- (void) backtrafoNormalVec3D
        : (const Vec3D *) inNormal
        : (Vec3D *) outNormal
{
    Vec3D  localNormal;

    [ TRAFO3D_1 backtrafoNormalVec3D
        :   inNormal
        : & localNormal
        ];

    [ TRAFO3D_0 backtrafoNormalVec3D
        : & localNormal
        :   outNormal
        ];
}

- (void) transformPnt3D
        : (const Pnt3D *) inPoint
        : (Pnt3D *) outPoint
{
    Pnt3D  localPoint;

    [ TRAFO3D_1 transformPnt3D
        :   inPoint
        : & localPoint
        ];

    [ TRAFO3D_0 transformPnt3D
        : & localPoint
        :   outPoint
        ];
}

- (void) transformVec3D
        : (const Vec3D *) inVector
        : (Vec3D *) outVector
{
    Vec3D  localVector;

    [ TRAFO3D_1 transformVec3D
        :   inVector
        : & localVector
        ];

    [ TRAFO3D_0 transformVec3D
        : & localVector
        :   outVector
        ];
}

- (void) transformRay3D
        : (const Ray3D *) inRay
        : (Ray3D *) outRay
{
    Ray3D  localRay;

    [ TRAFO3D_1 transformRay3D
        :   inRay
        : & localRay
        ];

    [ TRAFO3D_0 transformRay3D
        : & localRay
        :   outRay
        ];
}

- (void) transformNormalVec3D
        : (const Vec3D *) inNormal
        : (Vec3D *) outNormal
{
    Vec3D  localNormal;

    [ TRAFO3D_1 transformNormalVec3D
        :   inNormal
        : & localNormal
        ];

    [ TRAFO3D_0 transformNormalVec3D
        : & localNormal
        :   outNormal
        ];
}

- (void) getHTrafo3Ds
        : (HTrafo3D *) outForward
        : (HTrafo3D *) outBackward
{
    HTrafo3D  oneForward;
    HTrafo3D  oneBackward;
    HTrafo3D  twoForward;
    HTrafo3D  twoBackward;

    [ TRAFO3D_0 getHTrafo3Ds
        : & oneForward
        : & oneBackward
        ];

    [ TRAFO3D_1 getHTrafo3Ds
        : & twoForward
        : & twoBackward
        ];

    trafo3d_hh_mul_h(&oneForward, &twoForward, outForward);
    trafo3d_hh_mul_h(&twoBackward, &oneBackward, outBackward);
}

- (ArNode <ArpTrafo3D> *) reduceToSingleHTrafo3D
{
    return
        [ TRAFO3D_0 mul
            :   TRAFO3D_1
            ];
}

- (ArNode <ArpTrafo3D> *) mul
        : (ArNode <ArpTrafo3D> *) other
{
    HTrafo3D selfForward;
    HTrafo3D selfBackward;
    HTrafo3D otherForward;
    HTrafo3D otherBackward;
    HTrafo3D newForward;
    HTrafo3D newBackward;

    [ self  getHTrafo3Ds :&selfForward  :&selfBackward  ];
    [ other getHTrafo3Ds :&otherForward :&otherBackward ];

    trafo3d_hh_mul_h(&selfForward, &otherForward, &newForward);
    trafo3d_hh_mul_h(&otherBackward, &selfBackward, &newBackward);

    return
        create_arnhtrafo3d(
              art_gv,
            & newForward,
            & newBackward
            );
}

@end

ArnTrafo3DPair * create_arntrafo3dpair(
        ART_GV    * art_gv,
        ArNodeRef    nodeRef0,
        ArNodeRef    nodeRef1
        )
{
    return
        [ ALLOC_INIT_OBJECT(ArnTrafo3DPair)
            :   nodeRef0
            :   nodeRef1
            ];
}

// ===========================================================================
