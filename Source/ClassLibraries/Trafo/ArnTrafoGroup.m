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

#define ART_MODULE_NAME     ArnTrafoGroup

#import "ArnTrafoGroup.h"

#import "ArnHTrafo3D.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnTrafoGroup registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


ArnTrafoGroup * arntrafogroup(
        ART_GV    * art_gv,
        ArNodeRef    firstNodeRef,
        ...
        )
{
    ArNodeRefDynArray  array;

    ARNODEREF_DYNARRAY_FROM_VA_ARGS(array, firstNodeRef);

    id  result =
        [ ALLOC_INIT_OBJECT(ArnTrafoGroup)
            : & array
            ];

    arnoderefdynarray_free_contents( & array );

    return result;
}

#define TRAFO_I(_i)    \
((id)ARNODEREF_POINTER(arnoderefdynarray_i(&subnodeRefArray,_i)))
#define TRAFO2D_I(_i)  \
((ArNode <ArpTrafo2D> *)ARNODEREF_POINTER(arnoderefdynarray_i(&subnodeRefArray,_i)))
#define TRAFO3D_I(_i)  \
((ArNode <ArpTrafo3D> *)ARNODEREF_POINTER(arnoderefdynarray_i(&subnodeRefArray,_i)))

@implementation ArnTrafoGroup

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnTrafoGroup)

- (id) init
        : (ArNodeRefDynArray *) newSubnodeArray
{
    self =
        [ super init
            :   newSubnodeArray
            ];

    if ( self )
    {
        unsigned int  numberOfSubnodes =
            arnoderefdynarray_size( & subnodeRefArray );

        unsigned int  allSubnodesConformToProtocol = YES;

        for ( unsigned int i = 0; i < numberOfSubnodes; i++)
            if ( ! [ TRAFO_I( i ) conformsToArProtocol
                       :   ARPROTOCOL(ArpTrafo2D)
                       ] )
                allSubnodesConformToProtocol = NO;

        if ( allSubnodesConformToProtocol )
            return self;

        allSubnodesConformToProtocol = YES;

        for ( unsigned int i = 0; i < numberOfSubnodes; i++)
            if ( ! [ TRAFO_I( i ) conformsToArProtocol
                       :   ARPROTOCOL(ArpTrafo3D)
                       ] )
                allSubnodesConformToProtocol = NO;

        if ( allSubnodesConformToProtocol )
            return self;

        ART_ERRORHANDLING_FATAL_ERROR(
            "none of the required transformation protocols is supported"
            );
    }
    
    return 0;
}

- (void) backtrafoPnt3D
        : (const Pnt3D *) inPoint
        : (Pnt3D *) outPoint
{
    Pnt3D    localPoint;
    Pnt3D  * point[2] = { & localPoint, outPoint };

    unsigned int  numberOfSubnodes =
        arnoderefdynarray_size( & subnodeRefArray );

    int  toggle = numberOfSubnodes & 1;

    const Pnt3D  * pointOne = inPoint;

    for ( int i = numberOfSubnodes - 1; i >= 0; i--)
    {
        Pnt3D  * pointTwo = point[toggle];

        [ TRAFO3D_I(i) backtrafoPnt3D
            :   pointOne
            :   pointTwo
            ];

        pointOne = pointTwo;
        toggle   = 1 - toggle;
    }
}

- (void) backtrafoVec3D
        : (const Vec3D *) inVector
        : (Vec3D *) outVector
{
    Vec3D    localVector;
    Vec3D  * vector[2] = { & localVector, outVector };

    unsigned int  numberOfSubnodes =
        arnoderefdynarray_size( & subnodeRefArray );

    int  toggle = numberOfSubnodes & 1;

    const Vec3D  * vectorOne = inVector;

    for ( int i = numberOfSubnodes - 1; i >= 0; i--)
    {
        Vec3D  * vectorTwo = vector[toggle];

        [ TRAFO3D_I(i) backtrafoVec3D
            :   vectorOne
            :   vectorTwo
            ];

        vectorOne = vectorTwo;
        toggle   = 1 - toggle;
    }
}

- (void) backtrafoRay3D
        : (const Ray3D *) inRay
        : (Ray3D *) outRay
{
    Ray3D    localRay;
    Ray3D  * ray[2] = { & localRay, outRay };

    unsigned int  numberOfSubnodes =
        arnoderefdynarray_size( & subnodeRefArray );

    int  toggle = numberOfSubnodes & 1;

    const Ray3D  * rayOne = inRay;

    for ( int i = numberOfSubnodes - 1; i >= 0; i--)
    {
        Ray3D  * rayTwo = ray[toggle];

        [ TRAFO3D_I(i) backtrafoRay3D
            :   rayOne
            :   rayTwo
            ];

        rayOne = rayTwo;
        toggle   = 1 - toggle;
    }
}

- (void) backtrafoNormalVec3D
        : (const Vec3D *) inNormal
        : (Vec3D *) outNormal
{
    Vec3D    localNormal;
    Vec3D  * normal[2] = { & localNormal, outNormal };

    unsigned int  numberOfSubnodes =
        arnoderefdynarray_size( & subnodeRefArray );

    int  toggle = numberOfSubnodes & 1;

    const Vec3D  * normalOne = inNormal;

    for ( int i = numberOfSubnodes - 1; i >= 0; i--)
    {
        Vec3D  * normalTwo = normal[toggle];

        [ TRAFO3D_I(i) backtrafoNormalVec3D
            :   normalOne
            :   normalTwo
            ];

        normalOne = normalTwo;
        toggle   = 1 - toggle;
    }
}

- (void) transformPnt3D
        : (const Pnt3D *) inPoint
        : (Pnt3D *) outPoint
{
    Pnt3D    localPoint;
    Pnt3D  * point[2] = { & localPoint, outPoint };

    unsigned int  numberOfSubnodes =
        arnoderefdynarray_size( & subnodeRefArray );

    int  toggle = numberOfSubnodes & 1;

    const Pnt3D  * pointOne = inPoint;

    for ( int i = numberOfSubnodes - 1; i >= 0; i--)
    {
        Pnt3D  * pointTwo = point[toggle];

        [ TRAFO3D_I(i) transformPnt3D
            :   pointOne
            :   pointTwo
            ];

        pointOne = pointTwo;
        toggle   = 1 - toggle;
    }
}

- (void) transformVec3D
        : (const Vec3D *) inVector
        : (Vec3D *) outVector
{
    Vec3D    localVector;
    Vec3D  * vector[2] = { & localVector, outVector };

    unsigned int  numberOfSubnodes =
        arnoderefdynarray_size( & subnodeRefArray );

    int  toggle = numberOfSubnodes & 1;

    const Vec3D  * vectorOne = inVector;

    for ( int i = numberOfSubnodes - 1; i >= 0; i--)
    {
        Vec3D  * vectorTwo = vector[toggle];

        [ TRAFO3D_I(i) transformVec3D
            :   vectorOne
            :   vectorTwo
            ];

        vectorOne = vectorTwo;
        toggle   = 1 - toggle;
    }
}

- (void) transformRay3D
        : (const Ray3D *) inRay
        : (Ray3D *) outRay
{
    Ray3D    localRay;
    Ray3D  * ray[2] = { & localRay, outRay };

    unsigned int  numberOfSubnodes =
        arnoderefdynarray_size( & subnodeRefArray );

    int  toggle = numberOfSubnodes & 1;

    const Ray3D  * rayOne = inRay;

    for ( int i = numberOfSubnodes - 1; i >= 0; i--)
    {
        Ray3D  * rayTwo = ray[toggle];

        [ TRAFO3D_I(i) transformRay3D
            :   rayOne
            :   rayTwo
            ];

        rayOne = rayTwo;
        toggle   = 1 - toggle;
    }
}

- (void) transformNormalVec3D
        : (const Vec3D *) inNormal
        : (Vec3D *) outNormal
{
    Vec3D    localNormal;
    Vec3D  * normal[2] = { & localNormal, outNormal };

    unsigned int  numberOfSubnodes =
        arnoderefdynarray_size( & subnodeRefArray );

    int  toggle = numberOfSubnodes & 1;

    const Vec3D  * normalOne = inNormal;

    for ( int i = numberOfSubnodes - 1; i >= 0; i--)
    {
        Vec3D  * normalTwo = normal[toggle];

        [ TRAFO3D_I(i) transformNormalVec3D
            :   normalOne
            :   normalTwo
            ];

        normalOne = normalTwo;
        toggle   = 1 - toggle;
    }
}

- (void) getHTrafo3Ds
        : (HTrafo3D *) outForward
        : (HTrafo3D *) outBackward
{
    HTrafo3D  localForward;
    HTrafo3D  localBackward;

    HTrafo3D * forward[2]  = { outForward,  & localForward };
    HTrafo3D * backward[2] = { outBackward, & localBackward };

    unsigned int  numberOfSubnodes =
        arnoderefdynarray_size( & subnodeRefArray );

    int  one = numberOfSubnodes & 1;

    *forward[one]  = HTRAFO3D_UNIT;
    *backward[one] = HTRAFO3D_UNIT;

    for ( unsigned int i = 0; i < numberOfSubnodes; i++ )
    {
        HTrafo3D  singleForward;
        HTrafo3D  singleBackward;

        int  two = 1 - one;

        [ TRAFO3D_I(i) getHTrafo3Ds
            : & singleForward
            : & singleBackward
            ];

        trafo3d_hh_mul_h(
              forward[one],
            & singleForward,
              forward[two]
            );

        trafo3d_hh_mul_h(
            & singleBackward,
              backward[one],
              backward[two]
            );

        one = two;
    }
}

- (ArNode <ArpTrafo3D> *) reduceToSingleHTrafo3D
{
    HTrafo3D  forward;
    HTrafo3D  backward;

    [ self getHTrafo3Ds
        : & forward
        : & backward
        ];

    return
        [ ALLOC_INIT_OBJECT(ArnHTrafo3D)
            :   forward
            :   backward
            ];
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

- (void) backtrafoPnt2D
        : (const Pnt2D *) inPoint
        : (Pnt2D *) outPoint
{
    Pnt2D    localPoint;
    Pnt2D  * point[2] = { & localPoint, outPoint };

    unsigned int  numberOfSubnodes =
        arnoderefdynarray_size( & subnodeRefArray );

    int  toggle = numberOfSubnodes & 1;

    const Pnt2D  * pointOne = inPoint;

    for ( int i = numberOfSubnodes - 1; i >= 0; i--)
    {
        Pnt2D  * pointTwo = point[toggle];

        [ TRAFO2D_I(i) backtrafoPnt2D
            :   pointOne
            :   pointTwo
            ];

        pointOne = pointTwo;
        toggle   = 1 - toggle;
    }
}

- (void) backtrafoVec2D
        : (const Vec2D *) inVector
        : (Vec2D *) outVector
{
    Vec2D    localVector;
    Vec2D  * vector[2] = { & localVector, outVector };

    unsigned int  numberOfSubnodes =
        arnoderefdynarray_size( & subnodeRefArray );

    int  toggle = numberOfSubnodes & 1;

    const Vec2D  * vectorOne = inVector;

    for ( int i = numberOfSubnodes - 1; i >= 0; i--)
    {
        Vec2D  * vectorTwo = vector[toggle];

        [ TRAFO2D_I(i) backtrafoVec2D
            :   vectorOne
            :   vectorTwo
            ];

        vectorOne = vectorTwo;
        toggle   = 1 - toggle;
    }
}

- (void) backtrafoRay2D
        : (const Ray2D *) inRay
        : (Ray2D *) outRay
{
    Ray2D    localRay;
    Ray2D  * ray[2] = { & localRay, outRay };

    unsigned int  numberOfSubnodes =
        arnoderefdynarray_size( & subnodeRefArray );

    int  toggle = numberOfSubnodes & 1;

    const Ray2D  * rayOne = inRay;

    for ( int i = numberOfSubnodes - 1; i >= 0; i--)
    {
        Ray2D  * rayTwo = ray[toggle];

        [ TRAFO2D_I(i) backtrafoRay2D
            :   rayOne
            :   rayTwo
            ];

        rayOne = rayTwo;
        toggle   = 1 - toggle;
    }
}

- (void) backtrafoNormalVec2D
        : (const Vec2D *) inNormal
        : (Vec2D *) outNormal
{
    Vec2D    localNormal;
    Vec2D  * normal[2] = { & localNormal, outNormal };

    unsigned int  numberOfSubnodes =
        arnoderefdynarray_size( & subnodeRefArray );

    int  toggle = numberOfSubnodes & 1;

    const Vec2D  * normalOne = inNormal;

    for ( int i = numberOfSubnodes - 1; i >= 0; i--)
    {
        Vec2D  * normalTwo = normal[toggle];

        [ TRAFO2D_I(i) backtrafoNormalVec2D
            :   normalOne
            :   normalTwo
            ];

        normalOne = normalTwo;
        toggle   = 1 - toggle;
    }
}

- (void) transformPnt2D
        : (const Pnt2D *) inPoint
        : (Pnt2D *) outPoint
{
    Pnt2D    localPoint;
    Pnt2D  * point[2] = { & localPoint, outPoint };

    unsigned int  numberOfSubnodes =
        arnoderefdynarray_size( & subnodeRefArray );

    int  toggle = numberOfSubnodes & 1;

    const Pnt2D  * pointOne = inPoint;

    for ( int i = numberOfSubnodes - 1; i >= 0; i--)
    {
        Pnt2D  * pointTwo = point[toggle];

        [ TRAFO2D_I(i) transformPnt2D
            :   pointOne
            :   pointTwo
            ];

        pointOne = pointTwo;
        toggle   = 1 - toggle;
    }
}

- (void) transformVec2D
        : (const Vec2D *) inVector
        : (Vec2D *) outVector
{
    Vec2D    localVector;
    Vec2D  * vector[2] = { & localVector, outVector };

    unsigned int  numberOfSubnodes =
        arnoderefdynarray_size( & subnodeRefArray );

    int  toggle = numberOfSubnodes & 1;

    const Vec2D  * vectorOne = inVector;

    for ( int i = numberOfSubnodes - 1; i >= 0; i--)
    {
        Vec2D  * vectorTwo = vector[toggle];

        [ TRAFO2D_I(i) transformVec2D
            :   vectorOne
            :   vectorTwo
            ];

        vectorOne = vectorTwo;
        toggle   = 1 - toggle;
    }
}

- (void) transformRay2D
        : (const Ray2D *) inRay
        : (Ray2D *) outRay
{
    Ray2D    localRay;
    Ray2D  * ray[2] = { & localRay, outRay };

    unsigned int  numberOfSubnodes =
        arnoderefdynarray_size( & subnodeRefArray );

    int  toggle = numberOfSubnodes & 1;

    const Ray2D  * rayOne = inRay;

    for ( int i = numberOfSubnodes - 1; i >= 0; i--)
    {
        Ray2D  * rayTwo = ray[toggle];

        [ TRAFO2D_I(i) transformRay2D
            :   rayOne
            :   rayTwo
            ];

        rayOne = rayTwo;
        toggle   = 1 - toggle;
    }
}

- (void) transformNormalVec2D
        : (const Vec2D *) inNormal
        : (Vec2D *) outNormal
{
    Vec2D    localNormal;
    Vec2D  * normal[2] = { & localNormal, outNormal };

    unsigned int  numberOfSubnodes =
        arnoderefdynarray_size( & subnodeRefArray );

    int  toggle = numberOfSubnodes & 1;

    const Vec2D  * normalOne = inNormal;

    for ( int i = numberOfSubnodes - 1; i >= 0; i--)
    {
        Vec2D  * normalTwo = normal[toggle];

        [ TRAFO2D_I(i) transformNormalVec2D
            :   normalOne
            :   normalTwo
            ];

        normalOne = normalTwo;
        toggle   = 1 - toggle;
    }
}

- (void) getHTrafo2Ds
        : (HTrafo2D *) outForward
        : (HTrafo2D *) outBackward
{
    HTrafo2D  localForward;
    HTrafo2D  localBackward;

    HTrafo2D * forward[2]  = { outForward,  & localForward };
    HTrafo2D * backward[2] = { outBackward, & localBackward };

    unsigned int  numberOfSubnodes =
        arnoderefdynarray_size( & subnodeRefArray );

    int  one = numberOfSubnodes & 1;

    *forward[one]  = HTRAFO2D_UNIT;
    *backward[one] = HTRAFO2D_UNIT;

    for ( unsigned int i = 0; i < numberOfSubnodes; i++ )
    {
        HTrafo2D  singleForward;
        HTrafo2D  singleBackward;

        int  two = 1 - one;

        [ TRAFO2D_I(i) getHTrafo2Ds
            : & singleForward
            : & singleBackward
            ];

        trafo2d_hh_mul_h(
              forward[one],
            & singleForward,
              forward[two]
            );

        trafo2d_hh_mul_h(
            & singleBackward,
              backward[one],
              backward[two]
            );

        one = two;
    }
}

@end

// ===========================================================================
