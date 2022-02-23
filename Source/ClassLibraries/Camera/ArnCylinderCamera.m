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

#define ART_MODULE_NAME     ArnCylinderCamera

#import "ArnCylinderCamera.h"

#import "ART_Trafo.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnCylinderCamera registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArnCylinderCamera

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnCylinderCamera)

- (void) _setupCamera
{
    near2world = NULL;
}

- (id) init
        : (Ray3D) newRay
        : (IVec2D) newImageSize
        : (double) newZoom
{
    (void) newZoom;
    
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

- (id) init
        : (Ray3D) newRay
        : (IVec2D) newImageSize
        : (double) newZoom
        : (double) newTwist
        : (double) newNear
        : (double) newRatio
{
    (void) newNear;
    (void) newZoom;
    
    self = [super init];
    
    if ( self )
    {
        ray =               newRay;
        imageSize =         newImageSize;
        zoom =              1.0; // ignored
        twist =             newTwist;
        near =              0.0; // ignored
        ratio =             newRatio;
        [self _setupCamera];
    }
    
    return self;
}

- (void) setupForObject
        : (ArNode <ArpBBox> *) inObject
        : (ArcObject <ArpReporter> *) reporter
{
//    Scale3D scale;
//    double zscale;
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

    [trafo getHTrafo3Ds :&camera2world :&world2camera];

    xscale *= 0.5;
    yscale *= 0.5;

    if (autoDir)
    {
        [ reporter beginAction
            :   "CAMERA SETUP"
            ];

        [reporter printf:"automatic direction = " VEC3D_FORMAT("%f") "\n",
                    VEC3D_V_PRINTF(ray.vector)];
        [reporter endAction];
        [reporter printf:"\n"];
    }

    [trafo getHTrafo3Ds :&camera2world :&world2camera];

/*
    if (! m_d_is_tiny(near))
    {
        Trafo3D near2camera;
        near2world = ALLOC(Trafo3D);

        XC(scale) = near;
        YC(scale) = near;
        ZC(scale) = 0.0;

        g3d_sc_to_tr(&scale, &near2camera);
        trafo3d_hh_mul_h(&near2camera, &camera2world, near2world);

        pnt3d_rd_eval_p(&ray, near, &ray.point);
    }
*/

    RELEASE_OBJECT(trafo);
}

- (BOOL) getRay
        : (double) x
        : (double) y
        : (ArcObject <ArpRandomGenerator> *)  randomGenerator
        : (ArReferenceFrame *) outReferenceFrame
        : (Ray3D *) outRay
{
    (void) randomGenerator;
    (void) outReferenceFrame;
    
    Vec3D       getRayVector;

    double scaledx= (x - xscale)/(2*xscale) * MATH_2_MUL_PI;

    XC(getRayVector) = sin( scaledx );
    YC(getRayVector) = (y - yscale) * MATH_PI / xscale  / ratio;
    ZC(getRayVector) = cos( scaledx );

    vec3d_v_htrafo3d_v(&getRayVector, &camera2world, &outRay->vector);

    if (near2world)
    {
        Vec3D nearVector;
        vec3d_v_htrafo3d_v(&getRayVector, near2world, &nearVector);
        pnt3d_vp_add_p(
            & nearVector,
            & ray.point,
            & outRay->point
            );
    }
    else
        outRay->point = ray.point;

    return YES;
}

- (id) withNear
        : (double) newNear
{
    (void) newNear;
    near = 0.0; // ignore  newNear;
    return self;
}

@end

// ===========================================================================


