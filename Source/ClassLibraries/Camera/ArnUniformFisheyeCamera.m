/* ===========================================================================

    Copyright (c) 1996-2021 The ART Development Team
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

#define ART_MODULE_NAME     ArnUniformFisheyeCamera

#import "ArnUniformFisheyeCamera.h"

#import "ART_Trafo.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    [ ArnUniformFisheyeCamera registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArnUniformFisheyeCamera

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnUniformFisheyeCamera)

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
        zoom =              1.0;             // meaningless -> ignored!
        twist =             0.0;
        near =              0.0;
        ratio =             1.0;
        [self _setupCamera];
    }
    
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
        zoom =              1.0; // ignore newZoom - meaningless!
        twist =             newTwist;
        near =              0.0; // ignore newNear
        ratio =             1.0; // ignore newRatio
        [self _setupCamera];
    }
    
    return self;
}

- copy
{
    ArnUniformFisheyeCamera  * copiedInstance = [ super copy ];

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
    ArnUniformFisheyeCamera  * copiedInstance =
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

- (void) setupForObject
        : (ArNode <ArpBBox> *) inObject
        : (ArcObject <ArpReporter> *) reporter
{
  //    Scale3D scale; // maybe
  //    double zscale; // maybe
    double alpha,beta;
    int autoDir = 0;
    ArNode <ArpTrafo3D> * trafo = NULL;
    Box3D bbox;

    if (vec3d_v_sqrlen(&ray.vector) == 0.0)     // autocenter ?
    {
        Vec3D diagonal;
        Pnt3D midPoint;

        [inObject getBBoxObjectspace :&bbox];

        vec3d_pp_sub_v(&bbox.max, &bbox.min, &diagonal);

        pnt3d_dv_mul_p_add_p(
              0.5,
            & diagonal,
            & bbox.min,
            & midPoint
            );

        vec3d_pp_sub_v(&midPoint, &ray.point, &ray.vector);
        if (vec3d_v_sqrlen(&ray.vector) == 0.0)
            ray.vector = VEC3D_X_UNIT;

        autoDir = 1;
    }

    vec3d_norm_v(&ray.vector);

    xscale = (double) XC(imageSize);
    yscale = (double) YC(imageSize);

    /*
    zscale = sqrt(xscale * xscale  + yscale * yscale);
    XC(scale) = 1.0 / zscale;
    YC(scale) = 1.0 / (ratio * zscale);
    ZC(scale) = 1.0;
    */

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
            HARD_NODE_REFERENCE(rotZ0_Node),
            HARD_NODE_REFERENCE(rotX0_Node),
            HARD_NODE_REFERENCE(rotZ1_Node),
            HARD_NODE_REFERENCE(translation_Node),
            ARNODEREF_NONE
            );

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

    if (autoDir)
    {
        [reporter beginAction:  "CAMERA SETUP"];
        [reporter printf:"automatic direction = " VEC3D_FORMAT("%f") "\n",
                    VEC3D_V_PRINTF(ray.vector)];
        [reporter endAction];
        [reporter printf:"\n"];
    }

    [trafo getHTrafo3Ds :&camera2world :&world2camera];

    RELEASE_OBJECT(trafo);
}

- (BOOL) getWorldspaceRay
        : (const Vec2D *) pixelCoordinates
        : (      ArcObject <ArpRandomGenerator> *) randomGenerator
        : (      ArReferenceFrame *) outReferenceFrame
        : (      Ray3D *) outRay
{
    Vec3D       getRayVector;

    /* make circular image area in center of image */
    /* Note: [xy]scale is half the image size */

    double radius  = ( xscale >= yscale ? yscale : xscale);
    double scaledx = (XC(*pixelCoordinates)-xscale)/radius;
    double scaledy = (YC(*pixelCoordinates)-yscale)/radius;
    double v       = sqrt(M_SQR(scaledx)+M_SQR(scaledy));

    if ( v > 1.0 )
    { // outside image area
        return NO;
    }

    /* Now scaledx, scaledy are certainly within circle */

    /* stereographic mapping: */

    outRay->point = ray.point;

    XC(getRayVector) = scaledx;
    YC(getRayVector) = scaledy;
    ZC(getRayVector) = tan((1-v)*(M_PI/2.0)) * v;
    
    vec3d_norm_v(&getRayVector);

    vec3d_v_htrafo3d_v(&getRayVector, &camera2world, &outRay->vector);

    if ( outReferenceFrame )
    {
        Vec3D  rotVector;
        Vec3D  ref1, ref0;

        XC(rotVector) =  YC(getRayVector);
        YC(rotVector) = -XC(getRayVector);
        ZC(rotVector) =  0.0;

        double  alpha = atan2(YC(rotVector), XC(rotVector));
        Rot3D   rot = ROT3D(outRay->vector, alpha + MATH_PI_DIV_2 );

        vec3d_norm_v(&rotVector);
        vec3d_v_htrafo3d_v(&rotVector, &camera2world, &ref1);
        vec3d_vv_cross_v(&outRay->vector, &ref1, &ref0);
        vec3d_norm_v(& ref1);
        vec3d_norm_v(& ref0);
        vec3d_v_rot_v(&ref1,&rot, &ARREFFRAME_RF_I( *outReferenceFrame, 0 ));
        vec3d_v_rot_v(&ref0,&rot, &ARREFFRAME_RF_I( *outReferenceFrame, 1 ));
    }

    return YES;
}

- (BOOL) getPixelCoordinates
        : (const Ray3D *) inputRay
        : (      Vec2D *) pixelCoordinates
{
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
    return NO;
}

- withNear
        : (double) newNear
{
    near = 0.0; // ignore  newNear;
    return self;
}

- withRatio
        : (double) newRatio
{
    ratio = 1.0; // ignore
    return self;
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

- withTwist
        : (double) newTwist
{
    twist = newTwist;
    return self;
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





