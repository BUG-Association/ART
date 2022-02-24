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

#define ART_MODULE_NAME     ArnScaleTranslation

#import "ArnHTrafo3D.h"

#import "ArnScaleTranslation.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnScale2D       registerWithRuntime ];
    [ ArnScale3D       registerWithRuntime ];
    [ ArnTranslation2D registerWithRuntime ];
    [ ArnTranslation3D registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


/* ===========================================================================
    'ArnScale3D'
=========================================================================== */

@implementation ArnScale3D

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnScale3D)

- (void) _setupScale3D
{
    if (   m_d_is_tiny( XC(scale) )
        || m_d_is_tiny( YC(scale) )
        || m_d_is_tiny( ZC(scale) ) )
        ART_ERRORHANDLING_WARNING(
            "ArnScale3D: singular scale"
            );

    invScale = SCALE3D_INV_SC(scale);
}

- (id) init
{
    self = [ super init ];

    if ( self )
    {
        scale    = SCALE3D( 1.0, 1.0, 1.0 );
        invScale = SCALE3D( 1.0, 1.0, 1.0 );
    }
    
    return self;
}

- (id) init
        : (Scale3D) newScale
{
    self = [ super init ];

    if ( self )
    {
        scale = newScale;

        [ self _setupScale3D ];
    }
    
    return self;
}

- (id) initFrom
        : (const Scale3D *) newScale
{
    self = [ super init ];

    if ( self )
    {
        scale = *newScale;

        [ self _setupScale3D ];
    }
    
    return self;
}

- (id) copy
{
    ArnScale3D  * copiedInstance = [ super copy ];

    copiedInstance->scale    = scale;
    copiedInstance->invScale = invScale;

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnScale3D  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    copiedInstance->scale    = scale;
    copiedInstance->invScale = invScale;

    return copiedInstance;
}

- (void) backtrafoPnt3D
        : (const Pnt3D *) inPoint
        : (Pnt3D *) outPoint
{
    pnt3d_p_scale_p(
        & invScale,
          inPoint,
          outPoint
        );
}

- (void) backtrafoVec3D
        : (const Vec3D *) inVector
        : (Vec3D *) outVector
{
    vec3d_v_scale_v(
        & invScale,
          inVector,
          outVector
        );
}

- (void) backtrafoRay3D
        : (const Ray3D *) inRay
        : (Ray3D *) outRay
{
    ray3d_r_scale_r(
        & invScale,
          inRay,
          outRay
        );
}

- (void) backtrafoNormalVec3D
        : (const Vec3D *) inNormal
        : (Vec3D *) outNormal
{
    vec3d_v_scale_v(
        & scale,
          inNormal,
          outNormal
        );
}

- (void) transformPnt3D
        : (const Pnt3D *) inPoint
        : (Pnt3D *) outPoint
{
    pnt3d_p_scale_p(
        & scale,
          inPoint,
          outPoint
        );
}

- (void) transformVec3D
        : (const Vec3D *) inVector
        : (Vec3D *) outVector
{
    vec3d_v_scale_v(
        & scale,
          inVector,
          outVector
        );
}

- (void) transformRay3D
        : (const Ray3D *) inRay
        : (Ray3D *) outRay
{
    ray3d_r_scale_r(
        & scale,
          inRay,
          outRay
        );
}

- (void) transformNormalVec3D
        : (const Vec3D *) inNormal
        : (Vec3D *) outNormal
{
    vec3d_v_scale_v(
        & invScale,
          inNormal,
          outNormal
        );
}

- (void) getHTrafo3Ds
        : (HTrafo3D *) outForward
        : (HTrafo3D *) outBackward
{
    trafo3d_sc_to_h(&scale, outForward);
    trafo3d_sc_inv_to_h(&scale, outBackward);
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

    trafo3d_sc_h_mul_h(
        & scale,
        & otherForward,
        & newForward
        );

    trafo3d_h_sc_mul_h(
        & otherBackward,
        & invScale,
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

    [ coder codeScale3D
        : & scale
        ];

    if ( [ coder isReading ] )
        [ self _setupScale3D ];
}

@end


/* ===========================================================================
    'ArnTranslation3D'
=========================================================================== */

@implementation ArnTranslation3D

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnTranslation3D)

- (id) init
{
    self = [ super init ];

    if ( self )
    {
        translation = TRANSLATION3D(0.0,0.0,0.0);
    }
    
    return self;
}

- (id) init
        : (Translation3D) newTranslation
{
    self = [ super init ];

    if ( self )
    {
        translation = newTranslation;
    }
    
    return self;
}

- (id) initFrom
        : (const Translation3D *) newTranslation
{
    translation = *newTranslation;

    return self;
}

- (id) copy
{
    ArnTranslation3D  * copiedInstance = [ super copy ];

    copiedInstance->translation = translation;

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnTranslation3D  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    copiedInstance->translation = translation;

    return copiedInstance;
}

- (void) backtrafoPnt3D
        : (const Pnt3D *) inPoint
        : (Pnt3D *) outPoint
{
    pnt3d_p_inv_translate_p(inPoint, &translation, outPoint);
}

- (void) backtrafoVec3D
        : (const Vec3D *) inVector
        : (Vec3D *) outVector
{
    *outVector = *inVector;
}

- (void) backtrafoRay3D
        : (const Ray3D *) inRay
        : (Ray3D *) outRay
{
    ray3d_r_inv_translate_r(inRay, &translation, outRay);
}

- (void) backtrafoNormalVec3D
        : (const Vec3D *) inNormal
        : (Vec3D *) outNormal
{
    *outNormal = *inNormal;
}

- (void) transformPnt3D
        : (const Pnt3D *) inPoint
        : (Pnt3D *) outPoint
{
    pnt3d_p_translate_p(inPoint, &translation, outPoint);
}

- (void) transformVec3D
        : (const Vec3D *) inVector
        : (Vec3D *) outVector
{
    *outVector = *inVector;
}

- (void) transformRay3D
        : (const Ray3D *) inRay
        : (Ray3D *) outRay
{
    ray3d_r_translate_r(inRay, &translation, outRay);
}

- (void) transformNormalVec3D
        : (const Vec3D *) inNormal
        : (Vec3D *) outNormal
{
    *outNormal = *inNormal;
}

- (void) getHTrafo3Ds
        : (HTrafo3D *) outForward
        : (HTrafo3D *) outBackward
{
    trafo3d_tr_to_h(&translation, outForward);
    trafo3d_tr_inv_to_h(&translation, outBackward);
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

    trafo3d_tr_h_mul_h(
        & translation,
        & otherForward,
        & newForward
        );

    trafo3d_h_tr_inv_mul_h(
        & otherBackward,
        & translation,
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

    [ coder codeTranslation3D
        : & translation
        ];
}

@end

/* ===========================================================================
    'ArnScale2D'
=========================================================================== */
@implementation ArnScale2D

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnScale2D)

- (void) _setupScale2D
{
    if (   m_d_is_tiny( XC(scale) )
        || m_d_is_tiny( YC(scale) ) )
        ART_ERRORHANDLING_WARNING(
            "ArnScale2D: singular scale"
            );

    invScale = SCALE2D_INV_SC(scale);
}

- (id) init
{
    self  = [ super init ];

    if ( self )
    {
        scale    = SCALE2D( 1.0, 1.0 );
        invScale = SCALE2D( 1.0, 1.0 );
    }
    
    return self;
}

- (id) init
        : (Scale2D) newScale
{
    self = [ super init ];

    if ( self )
    {
        scale = newScale;

        [ self _setupScale2D ];
    }
    
    return self;
}

- (id) initFrom
        : (const Scale2D *) newScale
{
    self = [ super init ];
    
    if ( self )
    {
        scale = *newScale;

        [ self _setupScale2D ];
    }
    
    return self;
}

- (id) copy
{
    ArnScale2D  * copiedInstance = [ super copy ];

    copiedInstance->scale    = scale;
    copiedInstance->invScale = invScale;

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnScale2D  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    copiedInstance->scale    = scale;
    copiedInstance->invScale = invScale;

    return copiedInstance;
}

- (void) backtrafoPnt2D
        : (const Pnt2D *) inPoint
        : (Pnt2D *) outPoint
{
    pnt2d_p_scale_p(
        & invScale,
          inPoint,
          outPoint
        );
}

- (void) backtrafoVec2D
        : (const Vec2D *) inVector
        : (Vec2D *) outVector
{
    vec2d_v_scale_v(
        & invScale,
          inVector,
          outVector
        );
}

- (void) backtrafoRay2D
        : (const Ray2D *) inRay
        : (Ray2D *) outRay
{
    ray2d_r_scale_r(
        & invScale,
          inRay,
          outRay
        );
}

- (void) backtrafoNormalVec2D
        : (const Vec2D *) inNormal
        : (Vec2D *) outNormal
{
    vec2d_v_scale_v(
        & scale,
          inNormal,
          outNormal
        );
}

- (void) transformPnt2D
        : (const Pnt2D *) inPoint
        : (Pnt2D *) outPoint
{
    pnt2d_p_scale_p(
        & scale,
          inPoint,
          outPoint
        );
}

- (void) transformVec2D
        : (const Vec2D *) inVector
        : (Vec2D *) outVector
{
    vec2d_v_scale_v(
        & scale,
          inVector,
          outVector
        );
}

- (void) transformRay2D
        : (const Ray2D *) inRay
        : (Ray2D *) outRay
{
    ray2d_r_scale_r(
        & scale,
          inRay,
          outRay
        );
}

- (void) transformNormalVec2D
        : (const Vec2D *) inNormal
        : (Vec2D *) outNormal
{
    vec2d_v_scale_v(
        & invScale,
          inNormal,
          outNormal
        );
}

- (void) getHTrafo2Ds
        : (HTrafo2D *) outForward
        : (HTrafo2D *) outBackward
{
    trafo2d_sc_to_h(&scale, outForward);
    trafo2d_sc_inv_to_h(&scale, outBackward);
}

- (ArNode <ArpTrafo2D> *) reduceToSingleHTrafo2D
{
    return self;
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code
        :   coder
        ];

    [ coder codeScale2D
        : & scale
        ];

    if ( [ coder isReading ] )
        [ self _setupScale2D ];
}

@end

/* ===========================================================================
    'ArnTranslation2D'
=========================================================================== */

@implementation ArnTranslation2D

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnTranslation2D)

- (id) init
{
    self = [ super init ];
    
    if ( self )
    {
        translation = TRANSLATION2D(0.0,0.0);
    }
    
    return self;
}

- (id) init
        : (Translation2D) newTranslation
{
    self = [ super init ];

    if ( self )
    {
        translation = newTranslation;
    }
    
    return self;
}

- (id) initFrom
        : (const Translation2D *) newTranslation
{
    translation = *newTranslation;

    return self;
}

- (id) copy
{
    ArnTranslation2D  * copiedInstance = [ super copy ];

    copiedInstance->translation = translation;

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnTranslation2D  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    copiedInstance->translation = translation;

    return copiedInstance;
}

- (void) backtrafoPnt2D
        : (const Pnt2D *) inPoint
        : (Pnt2D *) outPoint
{
    pnt2d_p_inv_translate_p(inPoint, &translation, outPoint);
}

- (void) backtrafoVec2D
        : (const Vec2D *) inVector
        : (Vec2D *) outVector
{
    *outVector = *inVector;
}

- (void) backtrafoRay2D
        : (const Ray2D *) inRay
        : (Ray2D *) outRay
{
    ray2d_r_inv_translate_r(inRay, &translation, outRay);
}

- (void) backtrafoNormalVec2D
        : (const Vec2D *) inNormal
        : (Vec2D *) outNormal
{
    *outNormal = *inNormal;
}

- (void) transformPnt2D
        : (const Pnt2D *) inPoint
        : (Pnt2D *) outPoint
{
    pnt2d_p_translate_p(inPoint, &translation, outPoint);
}

- (void) transformVec2D
        : (const Vec2D *) inVector
        : (Vec2D *) outVector
{
    *outVector = *inVector;
}

- (void) transformRay2D
        : (const Ray2D *) inRay
        : (Ray2D *) outRay
{
    ray2d_r_translate_r(inRay, &translation, outRay);
}

- (void) transformNormalVec2D
        : (const Vec2D *) inNormal
        : (Vec2D *) outNormal
{
    *outNormal = *inNormal;
}

- (void) getHTrafo2Ds
        : (HTrafo2D *) outForward
        : (HTrafo2D *) outBackward
{
    trafo2d_tr_to_h(&translation, outForward);
    trafo2d_tr_inv_to_h(&translation, outBackward);
}

- (ArNode <ArpTrafo2D> *) reduceToSingleHTrafo2D
{
    return self;
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code
        :   coder
        ];

    [ coder codeTranslation2D
        : & translation
        ];
}

@end

#define VARIABLE_EXPRESSION_FOR_XC \
((ArNode <ArpDoubleValues> *) ARNODEREF_POINTER(subnodeRefArray[0]))
#define VARIABLE_EXPRESSION_FOR_YC \
((ArNode <ArpDoubleValues> *) ARNODEREF_POINTER(subnodeRefArray[1]))
#define VARIABLE_EXPRESSION_FOR_ZC \
((ArNode <ArpDoubleValues> *) ARNODEREF_POINTER(subnodeRefArray[2]))


/* ===========================================================================
    'ArnVarScale3D'
=========================================================================== */

@implementation ArnVarScale3D

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnVarScale3D)

- (void) _checkVarScale3D
{
}

- (id) init
        : (ArNode <ArpDoubleValues> *) xScale
        : (ArNode <ArpDoubleValues> *) yScale
        : (ArNode <ArpDoubleValues> *) zScale
{
    self =
        [ super init
            :   HARD_NODE_REFERENCE(xScale)
            :   HARD_NODE_REFERENCE(yScale)
            :   HARD_NODE_REFERENCE(zScale)
            ];
    
    if ( self )
    {
        [ self _checkVarScale3D ];
    }
    
    return self;
}

- (ArNode <ArpTrafo3D> *) evaluateToFixedTrafo3D
        : (ArcObject <ArpEvaluationEnvironment> *) env
{
    Scale3D  newScale;

    [ VARIABLE_EXPRESSION_FOR_XC getDoubleValue
        :   env
        : & SCALE3D_XC( newScale )
        ];

    [ VARIABLE_EXPRESSION_FOR_YC getDoubleValue
        :   env
        : & SCALE3D_YC( newScale )
        ];

    [ VARIABLE_EXPRESSION_FOR_ZC getDoubleValue
        :   env
        : & SCALE3D_ZC( newScale )
        ];

    return
        [ ALLOC_INIT_OBJECT(ArnScale3D)
            :   newScale
            ];
}

@end

/* ===========================================================================
    'ArnVarTranslation3D'
=========================================================================== */
@implementation ArnVarTranslation3D

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnVarTranslation3D)

- (void) _checkVarTranslation3D
{
}

- (id) init
        : (ArNode <ArpDoubleValues> *) xTranslation
        : (ArNode <ArpDoubleValues> *) yTranslation
        : (ArNode <ArpDoubleValues> *) zTranslation
{
    self =
        [ super init
            :   HARD_NODE_REFERENCE(xTranslation)
            :   HARD_NODE_REFERENCE(yTranslation)
            :   HARD_NODE_REFERENCE(zTranslation)
            ];
    
    if ( self )
    {
        [ self _checkVarTranslation3D ];
    }
    
    return self;
}

- (ArNode <ArpTrafo3D> *) evaluateToFixedTrafo3D
        : (ArcObject <ArpEvaluationEnvironment> *) env
{
    Translation3D  newTranslation;

    [ VARIABLE_EXPRESSION_FOR_XC getDoubleValue
        :   env
        : & TRANSLATION3D_XC( newTranslation )
        ];

    [ VARIABLE_EXPRESSION_FOR_YC getDoubleValue
        :   env
        : & TRANSLATION3D_YC( newTranslation )
        ];

    [ VARIABLE_EXPRESSION_FOR_ZC getDoubleValue
        :   env
        : & TRANSLATION3D_ZC( newTranslation )
        ];

    return
        [ ALLOC_INIT_OBJECT(ArnTranslation3D)
            :   newTranslation
            ];
}

@end

// ===========================================================================
