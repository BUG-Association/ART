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

#define ART_MODULE_NAME     ArnValEnvConstantsPoint

#import "ArnValEnvConstantsPoint.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnValWorldSpaceCoords    registerWithRuntime ];
    [ ArnValObjectSpaceCoords   registerWithRuntime ];
    [ ArnValWorldSpaceNormal    registerWithRuntime ];
    [ ArnValObjectSpaceNormal   registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


/* ===========================================================================
    'ArnValWorldSpaceCoords'
 =========================================================================== */
@implementation ArnValWorldSpaceCoords

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnValWorldSpaceCoords)
ARPVALUES_STANDARD_VALUETYPE_IMPLEMENTATION(arvalue_pnt3d /*|
                                            arvalue_vec3d*/
                                            )
ARPVALUES_NULLARY_EVALENVTYPE_IMPLEMENTATION(arevalenv_point)

//- (unsigned int) getVec3DValues
//        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv
//      : (const unsigned int) numberOfValues
//      : (Vec3D *) outValues
//{
//    return 0;
//}

- (unsigned int) getPnt3DValues
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (const unsigned int) numberOfValues
        : (Pnt3D *) outValues
{
    // The caller is responsible for providing an environment of correct type
    ArcObject <ArpPointEvalEnv> * pointEvalEnv =
            (ArcObject <ArpPointEvalEnv> *) evalEnv;

    const Pnt3D * coords = [ pointEvalEnv getWorldSpaceCoords ];

    unsigned int i;
    unsigned int actualNumberOfValues = M_MIN(numberOfValues, ARPVALUES_MAX_VALUES);
    for (i = 0; i < actualNumberOfValues; i++)
        outValues[i] = *coords;
    return actualNumberOfValues;

//    if (numberOfValues > 1)
//    {
//      if ([environment getPnt3Ds :(unsigned int)self :4 :outValues+1] != 4)
//      {
//          Vec3D dxy[2];
//
//          if ([environment getVec3Ds
//                  :(unsigned int)ARNGLOBALHITPOINT_MASTER :2 :dxy] != 2)
//            // #TH# changed on 12-Dec-00 10:33:26
//            //                return 1;
//            {
//              const ArHitDirection * direction = [environment hitDirection];
//
//              g3d_vv_cross_v(&direction->vector, &geometry->normal, &dxy[0]);
//              g3d_norm_v(&dxy[0]);
//              // Ivo: Why do we throw away previous content of dxy[1]?!?
//              g3d_vv_cross_v(&geometry->normal, &dxy[0], &dxy[1]);
//              g3d_d_mul_v(geometry->pixelSize,  &dxy[0]);
//              g3d_d_mul_v(geometry->pixelSize/direction->cosine, &dxy[1]);
//
//                [environment addVec3Ds
//                    :(unsigned int)ARNGLOBALHITPOINT_MASTER :2 :dxy];
//            }
//
//          g3d_pv_add_p(&geometry->point, &dxy[0], &outValues[1]);
//          g3d_pv_add_p(&geometry->point, &dxy[1], &outValues[2]);
//          g3d_pv_sub_p(&geometry->point, &dxy[0], &outValues[3]);
//          g3d_pv_sub_p(&geometry->point, &dxy[1], &outValues[4]);
//          [environment addPnt3Ds  :(unsigned int)self :4 :outValues+1];
//      }
//      return 5;
//    }
//
//    return 1;
}

- (unsigned int) getPnt3DValue
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (Pnt3D *) outValue
{
    // The caller is responsible for providing an environment of correct type
    ArcObject <ArpPointEvalEnv> * pointEvalEnv =
            (ArcObject <ArpPointEvalEnv> *) evalEnv;

    const Pnt3D * coords = [ pointEvalEnv getWorldSpaceCoords ];
    *outValue = *coords;

    return 1;
}

- (unsigned int) getConstPnt3DValues
        : (const unsigned int) numberOfValues
        : (Pnt3D *) outValues
{
    (void) numberOfValues;
    (void) outValues;
    
    // Coordinates do not provide globally constant values
    return 0;
}

- (unsigned int) getConstPnt3DValue
        : (Pnt3D *) outValue
{
    (void) outValue;
    
    // Coordinates do not provide globally constant values
    return 0;
}

@end

///* ===========================================================================
//    'ArnTextureReferencePoint'
//=========================================================================== */
//@implementation ArnReferenceHitPoint
//
//ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnReferenceHitPoint)
//
//- (ArValueType) valueType
//      : (ArValueType) valueTypeMask
//{
//    return (arvalue_pnt3d|arvalue_vec3d|arpenvironment_reference_point)
//         & valueTypeMask;
//}
//
//- (unsigned int) getVec3DValues
//        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv
//      : (const unsigned int) numberOfValues
//      : (Vec3D *) outValues
//{
//    return 0;
//}
//
//- (unsigned int) getPnt3DValues
//        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv
//        : (const unsigned int) numberOfValues
//        : (Pnt3D *) outValues
//{
//    const ArHitGeometry * geometry = [environment hitGeometry];
//    ArNode <ArpTrafo3D> * textureTrafo = [environment textureTrafo];
//    if (textureTrafo)
//      [textureTrafo backtrafoPnt3D :&geometry->point :&outValues[0]];
//    else
//      outValues[0] = geometry->point;
//    if (numberOfValues > 1)
//    {
//      if ([environment getPnt3Ds :(unsigned int)self :4 :outValues+1] != 4)
//      {
//            Pnt3D globalPnt[5];
//            int i;
//          Vec3D dxy[2];
//
//          if ([environment getVec3Ds
//                  :(unsigned int)ARNGLOBALHITPOINT_MASTER :2 :dxy] != 2)
//            // #TH# changed on 12-Dec-00 10:33:26
//            //                return 1;
//            {
//              const ArHitDirection * direction = [environment hitDirection];
//
//              g3d_vv_cross_v(&direction->vector, &geometry->normal, &dxy[0]);
//              g3d_norm_v(&dxy[0]);
//              g3d_vv_cross_v(&geometry->normal, &dxy[0], &dxy[1]);
//              g3d_d_mul_v(geometry->pixelSize,  &dxy[0]);
//              g3d_d_mul_v(geometry->pixelSize/direction->cosine, &dxy[1]);
//
//                [environment addVec3Ds
//                    :(unsigned int)ARNGLOBALHITPOINT_MASTER :2 :dxy];
//
//            }
//
//          g3d_pv_add_p(&geometry->point, &dxy[0], &globalPnt[1]);
//          g3d_pv_add_p(&geometry->point, &dxy[1], &globalPnt[2]);
//          g3d_pv_sub_p(&geometry->point, &dxy[0], &globalPnt[3]);
//          g3d_pv_sub_p(&geometry->point, &dxy[1], &globalPnt[4]);
//
//          if (textureTrafo)
//          {
//              for (i = 1; i < 5; i++)
//                  [textureTrafo
//                      backtrafoPnt3D :&globalPnt[i] :&outValues[i]];
//          }
//          else
//          {
//              for (i = 1; i < 5; i++)
//                  outValues[i] = globalPnt[i];
//          }
//
//          [environment addPnt3Ds  :(unsigned int)self :4 :outValues+1];
//      }
//      return 5;
//    }
//    return 1;
//}
//
//@end

/* ===========================================================================
    'ArnValObjectSpaceCoords'
=========================================================================== */
@implementation ArnValObjectSpaceCoords

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnValObjectSpaceCoords)
ARPVALUES_STANDARD_VALUETYPE_IMPLEMENTATION(arvalue_pnt3d)
ARPVALUES_NULLARY_EVALENVTYPE_IMPLEMENTATION(arevalenv_point)

- (unsigned int) getPnt3DValues
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (const unsigned int) numberOfValues
        : (Pnt3D *) outValues
{
//    ArSurfacePoint3D localPoint;
//    ArTraversalState * traversalState;
//    unsigned int i;
//
//    [environment getHitInfo :&localPoint :0 :&traversalState :0];
//    outValues[0] = localPoint.coord;
//
//    if (numberOfValues > 1)
//    {
//        Pnt3D globalPnt[5];
//        numberOfValues = [ARNGLOBALHITPOINT_MASTER getPnt3DValues
//                                    :environment :numberOfValues :globalPnt];
//        for (i = 1; i < numberOfValues; i++)
//            [traversalState->trafo backtrafoPnt3D: &globalPnt[i]
//                             : &outValues[i]];
//    }
//    return numberOfValues;


    // The caller is responsible for providing an environment of correct type
    ArcObject <ArpPointEvalEnv> * pointEvalEnv =
            (ArcObject <ArpPointEvalEnv> *) evalEnv;

    const Pnt3D * coords = [ pointEvalEnv getObjectSpaceCoords ];

    unsigned int i;
    unsigned int actualNumberOfValues = M_MIN(numberOfValues, ARPVALUES_MAX_VALUES);
    for (i = 0; i < actualNumberOfValues; i++)
        outValues[i] = *coords;

    return actualNumberOfValues;
}

- (unsigned int) getPnt3DValue
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (Pnt3D *) outValue
{
    // The caller is responsible for providing an environment of correct type
    ArcObject <ArpPointEvalEnv> * pointEvalEnv =
            (ArcObject <ArpPointEvalEnv> *) evalEnv;

    const Pnt3D * coords = [ pointEvalEnv getObjectSpaceCoords ];
    *outValue = *coords;

    return 1;
}

- (unsigned int) getConstPnt3DValues
        : (const unsigned int) numberOfValues
        : (Pnt3D *) outValues
{
    (void) numberOfValues;
    (void) outValues;
    
    // Coordinates do not provide globally constant values
    return 0;
}

- (unsigned int) getConstPnt3DValue
        : (Pnt3D *) outValue
{
    (void) outValue;
    
    // Coordinates do not provide globally constant values
    return 0;
}

@end

/* ===========================================================================
    'ArnValTextureSpaceCoords'
=========================================================================== */
@implementation ArnValTextureSpaceCoords

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnValTextureSpaceCoords)
ARPVALUES_STANDARD_VALUETYPE_IMPLEMENTATION(arvalue_pnt2d)
ARPVALUES_NULLARY_EVALENVTYPE_IMPLEMENTATION(arevalenv_point)

- (unsigned int) getPnt2DValues
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (const unsigned int) numberOfValues
        : (Pnt2D *) outValues
{
    // The caller is responsible for providing an environment of correct type

    ArcObject <ArpSurfacePointEvalEnv> * pointEvalEnv =
            (ArcObject <ArpSurfacePointEvalEnv> *) evalEnv;

    if ( numberOfValues > 1 )
        ART_ERRORHANDLING_FATAL_ERROR(">1");
    
    *outValues = *[ pointEvalEnv getTextureCoords ];

    return 1;
//    unsigned int i;
//    numberOfValues = M_MIN(numberOfValues, ARPVALUES_MAX_VALUES);
//    for (i = 0; i < numberOfValues; i++)
//        outValues[i] = *coords;
//
//    return numberOfValues;
}

- (unsigned int) getPnt2DValue
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (Pnt2D *) outValue
{
    // The caller is responsible for providing an environment of correct type

    ArcObject <ArpSurfacePointEvalEnv> * pointEvalEnv =
            (ArcObject <ArpSurfacePointEvalEnv> *) evalEnv;
    
    *outValue = *[ pointEvalEnv getTextureCoords ];

    return 1;
}

- (unsigned int) getConstPnt2DValues
        : (const unsigned int) numberOfValues
        : (Pnt2D *) outValues
{
    (void) numberOfValues;
    (void) outValues;
    
    // Coordinates do not provide globally constant values
    return 0;
}

- (unsigned int) getConstPnt2DValue
        : (Pnt2D *) outValue
{
    (void) outValue;
    
    // Coordinates do not provide globally constant values
    return 0;
}

@end

/* ===========================================================================
    'ArnValWorldSpaceNormal'
=========================================================================== */
@implementation ArnValWorldSpaceNormal

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnValWorldSpaceNormal)
ARPVALUES_STANDARD_VALUETYPE_IMPLEMENTATION(arvalue_vec3d)
ARPVALUES_NULLARY_EVALENVTYPE_IMPLEMENTATION(arevalenv_surfacepoint)

- (unsigned int) getVec3DValues
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (const unsigned int) numberOfValues
        : (Vec3D *) outValues
{
    // The caller is responsible for providing an environment of correct type
    ArcObject <ArpSurfacePointEvalEnv> * surfacePointEvalEnv =
            (ArcObject <ArpSurfacePointEvalEnv> *) evalEnv;

    const Vec3D * normal = [ surfacePointEvalEnv getWorldSpaceNormal ];

    unsigned int i;
    unsigned int actualNumberOfValues = M_MIN(numberOfValues, ARPVALUES_MAX_VALUES);
    for (i = 0; i < actualNumberOfValues; i++)
        outValues[i] = *normal;

    return actualNumberOfValues;
}

- (unsigned int) getVec3DValue
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (Vec3D *) outValue
{
    // The caller is responsible for providing an environment of correct type
    ArcObject <ArpSurfacePointEvalEnv> * surfacePointEvalEnv =
            (ArcObject <ArpSurfacePointEvalEnv> *) evalEnv;

    const Vec3D * normal = [ surfacePointEvalEnv getWorldSpaceNormal ];
    *outValue = *normal;

    return 1;
}

- (unsigned int) getConstVec3DValues
        : (const unsigned int) numberOfValues
        : (Vec3D *) outValues
{
    (void) numberOfValues;
    (void) outValues;
    
    // Normals do not provide globally constant values
    return 0;
}

- (unsigned int) getConstVec3DValue
        : (Vec3D *) outValue
{
    (void) outValue;
    
    // Normals do not provide globally constant values
    return 0;
}

@end

/* ===========================================================================
    'ArnValObjectSpaceNormal'
=========================================================================== */
@implementation ArnValObjectSpaceNormal

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnValObjectSpaceNormal)
ARPVALUES_STANDARD_VALUETYPE_IMPLEMENTATION(arvalue_vec3d)
ARPVALUES_NULLARY_EVALENVTYPE_IMPLEMENTATION(arevalenv_surfacepoint)

- (unsigned int) getVec3DValues
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (const unsigned int) numberOfValues
        : (Vec3D *) outValues
{
    // The caller is responsible for providing an environment of correct type
    ArcObject <ArpSurfacePointEvalEnv> * surfacePointEvalEnv =
            (ArcObject <ArpSurfacePointEvalEnv> *) evalEnv;

    const Vec3D * normal = [ surfacePointEvalEnv getObjectSpaceNormal ];

    unsigned int i;
    unsigned int actualNumberOfValues = M_MIN(numberOfValues, ARPVALUES_MAX_VALUES);
    for (i = 0; i < actualNumberOfValues; i++)
        outValues[i] = *normal;

    return actualNumberOfValues;
}

- (unsigned int) getVec3DValue
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (Vec3D *) outValue
{
    // The caller is responsible for providing an environment of correct type
    ArcObject <ArpSurfacePointEvalEnv> * surfacePointEvalEnv =
            (ArcObject <ArpSurfacePointEvalEnv> *) evalEnv;

    const Vec3D * normal = [ surfacePointEvalEnv getObjectSpaceNormal ];
    *outValue = *normal;

    return 1;
}

- (unsigned int) getConstVec3DValues
        : (const unsigned int) numberOfValues
        : (Vec3D *) outValues
{
    (void) numberOfValues;
    (void) outValues;
    
    // Normals do not provide globally constant values
    return 0;
}

- (unsigned int) getConstVec3DValue
        : (Vec3D *) outValue
{
    (void) outValue;
    
    // Normals do not provide globally constant values
    return 0;
}

@end

///* ===========================================================================
//    'ArnHitFaceIndex'
//=========================================================================== */
//@implementation ArnHitFaceIndex
//
//ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnHitFaceIndex)
//
//- (ArValueType) valueType
//      : (ArValueType) valueTypeMask
//{
//    return arvalue_int & valueTypeMask;
//}
//
//- (unsigned int) getLongValues
//        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv
//      : (const unsigned int) numberOfValues
//      : (Long *) outValues
//{
//    ArMicrofacet facet;
//    [environment getHitInfo :0 :0 :0 :&facet];
//    outValues[0] = facet.shapeFace.face;
//    return 1;
//}
//
//@end
//
///* ===========================================================================
//    'ArnHitSheetIndex'
//=========================================================================== */
//@implementation ArnHitSheetIndex
//
//ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnHitSheetIndex)
//
//- (ArValueType) valueType
//      : (ArValueType) valueTypeMask
//{
//    return arvalue_int & valueTypeMask;
//}
//
//- (unsigned int) getLongValues
//        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv
//      : (const unsigned int) numberOfValues
//      : (Long *) outValues
//{
//    ArMicrofacet facet;
//    [environment getHitInfo :0 :0 :0 :&facet];
//    outValues[0] = facet.shapeFace.sheet;
//    return 1;
//}
//
//@end

// ===========================================================================
