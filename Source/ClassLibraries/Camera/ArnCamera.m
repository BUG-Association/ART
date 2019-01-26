/* ===========================================================================

    Copyright (c) 1996-2019 The ART Development Team
    ------------------------------------------------

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

#define ART_MODULE_NAME     ArnCamera

#import "ArnCamera.h"

#import "ART_Trafo.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    [ ArnCamera registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArnCamera

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnCamera)

- (void) _setupCamera
{
    near2world = NULL;
}

- imageSize
                : (IVec2D) newImageSize
        ray     : (Ray3D) newRay
        zoom    : (double) newZoom
{
    return
    [self init
        : newRay
        : newImageSize
        : newZoom
    ];
}


- init
        : (Ray3D) newRay
        : (IVec2D) newImageSize
        : (double) newZoom
{
    self = [super init];
    
    if ( self )
    {
        ray =               newRay;
        imageSize =         newImageSize;
        zoom =              newZoom;
        twist =             0.0 DEGREES;
        near =              0.0;
        ratio =             1.0;
        [self _setupCamera];
    }
    
    return self;
}

- withTwist
        : (double) newTwist
{
    twist = newTwist;
    return self;
}

- withNear
        : (double) newNear
{
    near = newNear;
    return self;
}

- withRatio
        : (double) newRatio
{
    ratio = newRatio;
    return self;
}

- init
        : (Ray3D) newRay
        : (IVec2D) newImageSize
        : (double) newZoom
        : (double) newTwist
        : (double) newNear
        : (double) newRatio
{
    self = [super init];
    
    if ( self )
    {
        ray =               newRay;
        imageSize =         newImageSize;
        zoom =              newZoom;
        twist =             newTwist;
        near =              newNear;
        ratio =             newRatio;
        [self _setupCamera];
    }
    return self;
}

- copy
{
    ArnCamera  * copiedInstance = [ super copy ];

    copiedInstance->ray = ray;
    copiedInstance->imageSize = imageSize;
    copiedInstance->zoom = zoom;

    copiedInstance->twist = twist;
    copiedInstance->near = near;
    copiedInstance->ratio = ratio;

    copiedInstance->xscale = xscale;
    copiedInstance->yscale = yscale;

    copiedInstance->world2camera = world2camera;
    copiedInstance->camera2world = camera2world;

    if ( near2world )
    {
        copiedInstance->near2world = ALLOC(HTrafo3D);
        *copiedInstance->near2world = *near2world;
    }

    return copiedInstance;
}

- deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnCamera  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    copiedInstance->ray = ray;
    copiedInstance->imageSize = imageSize;
    copiedInstance->zoom = zoom;

    copiedInstance->twist = twist;
    copiedInstance->near = near;
    copiedInstance->ratio = ratio;

    copiedInstance->xscale = xscale;
    copiedInstance->yscale = yscale;

    copiedInstance->world2camera = world2camera;
    copiedInstance->camera2world = camera2world;

    if ( near2world )
    {
        copiedInstance->near2world = ALLOC(HTrafo3D);
        *copiedInstance->near2world = *near2world;
    }

    return copiedInstance;
}

- (void) getImageSize
        :(IVec2D *) outImageSize
{
    *outImageSize = imageSize;
}

- (void) setImageSize
        : (const IVec2D *) inImageSize
{
    imageSize = *inImageSize;
}

- (void) setupForObject
        : (ArNode <ArpBBox> *) inObject
        : (ArcObject <ArpReporter> *) reporter
{
    Scale3D scale;
    double zscale;
    double alpha,beta;
    int autoDir = 0;
    int autoZoom = 0;
    ArNode <ArpTrafo3D> * trafo = NULL;
    Box3D  bbox;

    if ( vec3d_v_sqrlen(&ray.vector) == 0.0 )   // autocenter ?
    {
        Vec3D  diagonal;
        Pnt3D  midPoint;

        [ inObject getBBoxObjectspace
            : & bbox
            ];

        vec3d_pp_sub_v(
            & bbox.max,
            & bbox.min,
            & diagonal
            );

        pnt3d_dv_mul_p_add_p(
              0.5,
            & diagonal,
            & bbox.min,
            & midPoint
            );

        vec3d_pp_sub_v(
            & midPoint,
            & ray.point,
            & ray.vector
            );

        if ( vec3d_v_sqrlen( & ray.vector ) == 0.0 )
            ray.vector = VEC3D_X_UNIT;

        autoDir = 1;
    }

    if ( zoom == 0.0 )                  // autozoom ?
    {
        zoom     = 1.0;
        autoZoom = 1;
    }

    vec3d_norm_v(&ray.vector);

    xscale = (double) XC(imageSize);
    yscale = (double) YC(imageSize);
    zscale = zoom * sqrt(xscale * xscale + yscale * yscale);
    XC(scale) = 1.0 / zscale;
    YC(scale) = 1.0 / (ratio * zscale);
    ZC(scale) = 1.0;

    if (XC(ray.vector) == 0.0 && YC(ray.vector) == 0.0)
        alpha = M_SIGN(ZC(ray.vector)) * MATH_PI_DIV_2;
    else
    {
        alpha = sqrt(M_SQR(XC(ray.vector)) + M_SQR(YC(ray.vector)));
        alpha = atan2(ZC(ray.vector),alpha);
    }
    if (XC(ray.vector) == 0.0)
        beta = M_SIGN(YC(ray.vector)) * MATH_PI_DIV_2;
    else
        beta = atan2(YC(ray.vector), XC(ray.vector));

    ArnScale3D * scale_Node =
        [ ALLOC_INIT_OBJECT(ArnScale3D)
            :   scale
            ];

    ArnRotZ3D * rotZ0_Node =
        [ ALLOC_INIT_OBJECT(ArnRotZ3D)
            :   twist
            ];

    ArnRotX3D * rotX0_Node =
        [ ALLOC_INIT_OBJECT(ArnRotX3D)
            :   alpha - MATH_PI_DIV_2
            ];

    ArnRotZ3D * rotZ1_Node =
        [ ALLOC_INIT_OBJECT(ArnRotZ3D)
            :   beta - MATH_PI_DIV_2
            ];

    ArnTranslation3D * translation_Node =
        [ ALLOC_INIT_OBJECT(ArnTranslation3D)
            :   TRANSLATION3D_P(ray.point)
            ];

    trafo =
        arntrafogroup(
            art_gv,
            HARD_NODE_REFERENCE(scale_Node),
            HARD_NODE_REFERENCE(rotZ0_Node),
            HARD_NODE_REFERENCE(rotX0_Node),
            HARD_NODE_REFERENCE(rotZ1_Node),
            HARD_NODE_REFERENCE(translation_Node),
            ARNODEREF_NONE
            );

    RELEASE_OBJECT(scale_Node);
    RELEASE_OBJECT(rotZ0_Node);
    RELEASE_OBJECT(rotX0_Node);
    RELEASE_OBJECT(rotZ1_Node);
    RELEASE_OBJECT(translation_Node);

    [ trafo getHTrafo3Ds
        : & camera2world
        : & world2camera
        ];

    xscale *= 0.5;
    yscale *= 0.5;

    if (autoZoom)
    {
        Pnt3D point_array[8];
        Box3D camera_box;
        Pnt3D camera_pnt;
        Scale3D scale;

        box3d_b_corners_pa(&bbox, point_array);

        camera_box = BOX3D_EMPTY;

        for (unsigned int i = 0; i < 8; i++)
        {
            pnt3d_p_htrafo3d_p(
                & point_array[i],
                & world2camera,
                & camera_pnt
                );

            if (ZC(camera_pnt) < MATH_TINY_DOUBLE)
            {
                ART_ERRORHANDLING_FATAL_ERROR(
                    "autozoom not possible"
                    );
            }
            XC(camera_pnt) /= ZC(camera_pnt);
            YC(camera_pnt) /= ZC(camera_pnt);

            box3d_p_add_b(&camera_pnt, &camera_box);
        }

        XC(scale) = YC(scale) =
            M_MAX(M_MAX(M_ABS(XC(camera_box.min)),M_ABS(XC(camera_box.max)))
                        / xscale,
                    M_MAX(M_ABS(YC(camera_box.min)),M_ABS(YC(camera_box.max)))
                        / yscale
                ) * 1.05;
        ZC(scale) = 1.0;

        trafo =
            [ [ ALLOC_INIT_OBJECT(ArnScale3D)
                : scale
                ] then
                : trafo
                ];

        zoom = 1.0 / XC(scale);

    }

    if (autoDir || autoZoom)
    {
        [ reporter beginAction
            :   "CAMERA SETUP"
            ];

        if (autoDir)
            [reporter printf:"automatic direction = " VEC3D_FORMAT("%f") "\n",
                    VEC3D_V_PRINTF(ray.vector)];
        if (autoZoom)
            [reporter printf:"automatic zoom      = %f\n", zoom];
        [reporter endAction];
        [reporter printf:"\n"];
    }

    [ trafo getHTrafo3Ds
        : & camera2world
        : & world2camera
        ];

    if (! m_d_is_tiny(near))
    {
        near2world = ALLOC(HTrafo3D);

        XC(scale) = near;
        YC(scale) = near;
        ZC(scale) = 0.0;

        HTrafo3D  near2camera;

        trafo3d_sc_to_h(
            & scale,
            & near2camera
            );

        trafo3d_hh_mul_h(
            & near2camera,
            & camera2world,
              near2world
            );

        pnt3d_dr_eval_p(
              near,
            & ray,
            & ray.point
            );
    }

    RELEASE_OBJECT(trafo);
}

- (const Pnt3D *) eye
{
    return &ray.point;
}

- (void) setEye
        : (const Pnt3D *) newEye
{
    RAY3D_POINT(ray) = *newEye;
}

- (double) near
{
    return near;
}

// Uncomment this to get debug printouts from getRay
// Generates huge amounts of bizarre output - use carefully!
//#define DEBUGPRINTF_ArnCamera_getRay

- (BOOL) getWorldspaceRay
        : (const Vec2D *) pixelCoordinates
        : (      ArcObject <ArpRandomGenerator> *) randomGenerator
        : (      ArReferenceFrame *) outReferenceFrame
        : (      Ray3D *) outRay
{
    Vec3D  mainRayVector;
    Vec3D  auxRayVector, auxWorldVector;

    XC(mainRayVector) = XC(*pixelCoordinates) - xscale;
    YC(mainRayVector) = YC(*pixelCoordinates) - yscale;
    ZC(mainRayVector) = 1.0;

    vec3d_v_htrafo3d_v( & mainRayVector, & camera2world, & outRay->vector);

    XC(auxRayVector) = XC(*pixelCoordinates) - xscale;
    YC(auxRayVector) = YC(*pixelCoordinates) - 1.0 - yscale;
    ZC(auxRayVector) = 1.0;
    vec3d_v_htrafo3d_v( & auxRayVector, & camera2world, & auxWorldVector );
    vec3d_norm_v(&auxWorldVector);

    if ( near2world )
    {
        Vec3D nearVector;
        vec3d_v_htrafo3d_v(&mainRayVector, near2world, &nearVector);
        pnt3d_vp_add_p(
            & nearVector,
            & ray.point,
            & outRay->point
            );
    }
    else
        outRay->point = ray.point;

    vec3d_norm_v( & RAY3D_V(*outRay) );
    
    /* -aw---------------------------------------------------------------

        If the system is compiled with polarisation capabilities we
        have to provide an initial spatial reference system for each
        ray. This pair of orthogonal vectors (the propagation vector
        can be derived as their crossproduct) is constructed as follows:

        - The vector defined by the tips of the current direction and
          the ray directon for the pixel "above" (y-1) is the initial
          "up/y-axis" direction.

        - The crossproduct of this temporary "up" vector and the
          ray direction is the final "across/x-axis" reference.

        - the crossproduct of the ray direction and the "across/x-axis"
          reference yields the final, orthogonal "up/y-axis" vector.

        All these computations are performed in world coordinates, since
        the reference systems are always maintained in this system.

        It should be noted that this way of computing the reference
        system is probably not perfect and might lead to perceptible
        distortions for extreme wideangle cameras.

    ------------------------------------------------------------------ */

    if ( outReferenceFrame )
    {
        Vec3D   upVector;
        Vec3D   negRayVector;

        vec3d_dv_mul_v( -1.0, & outRay->vector, & negRayVector );

        XC(upVector) = XC(negRayVector) - XC(auxWorldVector);
        YC(upVector) = YC(negRayVector) - YC(auxWorldVector);
        ZC(upVector) = ZC(negRayVector) - ZC(auxWorldVector);

        vec3d_vv_cross_v(
            & negRayVector,
            & upVector,
            & ARREFFRAME_RF_I( *outReferenceFrame, 0 )
            );

        vec3d_vv_cross_v(
            & negRayVector,
            & ARREFFRAME_RF_I( *outReferenceFrame, 0 ),
            & ARREFFRAME_RF_I( *outReferenceFrame, 1 )
            );

        vec3d_norm_v(& ARREFFRAME_RF_I( *outReferenceFrame, 0 ));
        vec3d_norm_v(& ARREFFRAME_RF_I( *outReferenceFrame, 1 ));

#ifdef DEBUGPRINTF_ArnCamera_getRay
        printf("\n");
        printf("Main  ray: " VEC3D_FORMAT("%6.4f") "\n\n",
               VEC3D_V_PRINTF( mainRayVector ) );
        printf("World ray: " VEC3D_FORMAT("%6.4f") "\n",
               VEC3D_V_PRINTF( outRay->vector ) );

        printf("Ref. 0   : " VEC3D_FORMAT("%6.4f") "\n",
               VEC3D_V_PRINTF(ARREFFRAME_RF_I( *outReferenceFrame, 0)));
        printf("Ref. 1   : " VEC3D_FORMAT("%6.4f") "\n",
               VEC3D_V_PRINTF(ARREFFRAME_RF_I( *outReferenceFrame, 1)));
        fflush(stdout);
#endif // DEBUGPRINTF_ArnCamera_getRay
    }

    ASSERT_VALID_NORMALIZED_RAY3D(*outRay);
    
    return YES;
}

- (BOOL) getPixelCoordinates
        : (const Ray3D *) inputRay
        : (      Vec2D *) pixelCoordinates
{
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
    return NO;
}

- (double) getZoom
{
    return zoom;
}


- (double) getTwist
{
    return twist;
}


- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [super code :coder];
    if ([coder isReading]) [self _setupCamera];

    [coder codeRay3D :&ray];
    [coder codeIVec2D :&imageSize];
    [coder codeDouble :&zoom];
    [coder codeDouble :&twist];
    [coder codeDouble :&near];
    [coder codeDouble :&ratio];
}

@end

// ===========================================================================
