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

#define ART_MODULE_NAME     ArnThinLensCamera

#import "ArnThinLensCamera.h"

#import "ART_Trafo.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnThinLensCamera registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArnThinLensCamera

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnThinLensCamera)

- (void) _setupCamera
{
    realAperture = aperture;

    if ( realAperture < 1.0 )
    {
        realAperture = 1.0;

        ART_ERRORHANDLING_WARNING(
            "desired camera aperture too large - value re-set to 1.0"
            );
    }

    if ( realAperture == 1.4 )
        realAperture = MATH_SQRT_2;

    if ( realAperture == 2.8 )
        realAperture = 2.0 * MATH_SQRT_2;

    if ( realAperture == 5.6 )
        realAperture = 4.0 * MATH_SQRT_2;

    if ( realAperture == 11.0 )
        realAperture = 8.0 * MATH_SQRT_2;

    if ( realAperture == 22.0 )
        realAperture = 16.0 * MATH_SQRT_2;

    if ( realAperture == 45.0 )
        realAperture = 32.0 * MATH_SQRT_2;

    if ( realAperture == 90.0 )
        realAperture = 64.0 * MATH_SQRT_2;

    if ( realAperture > 1024.0 )
    {
        realAperture = 1024.0;
        ART_ERRORHANDLING_WARNING(
            "desired camera aperture too small - value re-set to 1024.0"
            );
    }

    XC(cameraSpaceFilmOffset) = XC(filmResolution) / 2.0;
    YC(cameraSpaceFilmOffset) = YC(filmResolution) / 2.0;

    imageScale =
        sqrt(  XC(filmResolution) * XC(filmResolution)
             + YC(filmResolution) * YC(filmResolution) );

    worldSpaceFilmDiagonal =
        sqrt(  XC(worldSpaceFilmSize) * XC(worldSpaceFilmSize)
             + YC(worldSpaceFilmSize) * YC(worldSpaceFilmSize) );

    double  zoom =   worldSpaceLensFocalLength
                   / worldSpaceFilmDiagonal;

    imageScale *= zoom;

    Scale3D  scaleA;

    XC(scaleA) = 1.0 / imageScale;
    YC(scaleA) = 1.0 / imageScale;
    ZC(scaleA) = 1.0;

    Scale3D  scaleB;

    XC(scaleB) = worldSpaceCameraFocalDistance;
    YC(scaleB) = worldSpaceCameraFocalDistance;
    ZC(scaleB) = worldSpaceCameraFocalDistance;

    ArNode <ArpTrafo3D> * trafo = NULL;

    double  alpha;

    if (   XC(worldSpaceViewingDirection) == 0.0
        && YC(worldSpaceViewingDirection) == 0.0 )
        alpha = M_SIGN( ZC(worldSpaceViewingDirection) ) * MATH_PI_DIV_2;
    else
    {
        alpha = sqrt(  M_SQR( XC(worldSpaceViewingDirection) )
                     + M_SQR( YC(worldSpaceViewingDirection) ) );

        alpha = atan2( ZC(worldSpaceViewingDirection), alpha );
    }

    double  beta;

    if ( XC(worldSpaceViewingDirection) == 0.0 )
        beta = M_SIGN( YC(worldSpaceViewingDirection) ) * MATH_PI_DIV_2;
    else
        beta = atan2( YC(worldSpaceViewingDirection),
                      XC(worldSpaceViewingDirection) );

    ArnScale3D * scaleA_Node =
        [ ALLOC_INIT_OBJECT(ArnScale3D)
            :   scaleA
            ];

    ArnScale3D * scaleB_Node =
        [ ALLOC_INIT_OBJECT(ArnScale3D)
            :   scaleB
            ];

    ArnRotZ3D * rotZ0_Node =
        [ ALLOC_INIT_OBJECT(ArnRotZ3D)
            :   worldSpaceCameraRotation
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
            :   TRANSLATION3D_P( worldSpaceEyePoint )
            ];

    trafo =
        arntrafogroup(
            art_gv,
            HARD_NODE_REFERENCE(scaleA_Node),
            HARD_NODE_REFERENCE(scaleB_Node),
            HARD_NODE_REFERENCE(rotZ0_Node),
            HARD_NODE_REFERENCE(rotX0_Node),
            HARD_NODE_REFERENCE(rotZ1_Node),
            HARD_NODE_REFERENCE(translation_Node),
            ARNODEREF_NONE
            );

    RELEASE_OBJECT(scaleA_Node);
    RELEASE_OBJECT(scaleB_Node);
    RELEASE_OBJECT(rotZ0_Node);
    RELEASE_OBJECT(rotX0_Node);
    RELEASE_OBJECT(rotZ1_Node);
    RELEASE_OBJECT(translation_Node);

    [ trafo getHTrafo3Ds
        : & camera2world
        : & world2camera
        ];

    RELEASE_OBJECT(trafo);
}

- (id) copy
{
    ArnThinLensCamera  * copiedInstance = [ super copy ];

    copiedInstance->worldSpaceFilmSize = worldSpaceFilmSize;
    copiedInstance->filmResolution = filmResolution;
    copiedInstance->worldSpaceLensFocalLength = worldSpaceCameraFocalDistance;
    copiedInstance->aperture = aperture;
    copiedInstance->worldSpaceEyePoint = worldSpaceEyePoint;
    copiedInstance->worldSpaceViewingDirection = worldSpaceViewingDirection;
    copiedInstance->worldSpaceCameraFocalDistance = worldSpaceCameraFocalDistance;
    copiedInstance->worldSpaceCameraRotation = worldSpaceCameraRotation;

    copiedInstance->cameraSpaceFilmOffset = cameraSpaceFilmOffset;
    copiedInstance->worldSpaceFilmDiagonal = worldSpaceFilmDiagonal;
    copiedInstance->imageScale = imageScale;
    copiedInstance->realAperture = realAperture;
    copiedInstance->world2camera = world2camera;
    copiedInstance->camera2world = camera2world;

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnThinLensCamera  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    copiedInstance->worldSpaceFilmSize = worldSpaceFilmSize;
    copiedInstance->filmResolution = filmResolution;
    copiedInstance->worldSpaceLensFocalLength = worldSpaceCameraFocalDistance;
    copiedInstance->aperture = aperture;
    copiedInstance->worldSpaceEyePoint = worldSpaceEyePoint;
    copiedInstance->worldSpaceViewingDirection = worldSpaceViewingDirection;
    copiedInstance->worldSpaceCameraFocalDistance = worldSpaceCameraFocalDistance;
    copiedInstance->worldSpaceCameraRotation = worldSpaceCameraRotation;

    copiedInstance->cameraSpaceFilmOffset = cameraSpaceFilmOffset;
    copiedInstance->worldSpaceFilmDiagonal = worldSpaceFilmDiagonal;
    copiedInstance->imageScale = imageScale;
    copiedInstance->realAperture = realAperture;
    copiedInstance->world2camera = world2camera;
    copiedInstance->camera2world = camera2world;

    return copiedInstance;
}

- (id) filmSize                  : (Vec2D) newFilmSize
{
    [ super init ];

    worldSpaceFilmSize            = newFilmSize;

    //   Default values so that the setup routine may be
    //   safely called, even on an otherwise uninitialised
    //   camera.

    filmResolution                = IVEC2D( 700, 480 );
    worldSpaceLensFocalLength     = 50 MM;
    aperture                      = 5.6;
    worldSpaceEyePoint            = PNT3D( 10.0, 0.0, 0.0 );
    worldSpaceViewingDirection    = VEC3D( -1.0, 0, 0 );
    worldSpaceCameraFocalDistance = 10;
    worldSpaceCameraRotation      = 0.0;

    return self;
}

- (id) filmResolution
                            : (IVec2D) newFilmResolution
        lensFocalLength     : (double) newFocalLength
        aperture            : (double) newAperture
        eyePoint            : (Pnt3D)  newEyePoint
        cameraFocusPoint    : (Pnt3D)  newFocusPoint
{
    return [ self
        filmResolution  : newFilmResolution
        lensFocalLength : newFocalLength
        aperture        : newAperture
        eyePoint        : newEyePoint
        cameraFocusPoint: newFocusPoint
        cameraRotation  : 0.0 ];
}

- (id) filmResolution
                            : (IVec2D) newFilmResolution
        lensFocalLength     : (double) newFocalLength
        aperture            : (double) newAperture
        eyePoint            : (Pnt3D)  newEyePoint
        cameraFocusPoint    : (Pnt3D)  newFocusPoint
        cameraRotation      : (double) newCameraRotation
{
    filmResolution                = newFilmResolution;
    worldSpaceLensFocalLength     = newFocalLength;
    aperture                      = newAperture;
    worldSpaceEyePoint            = newEyePoint;
    vec3d_pp_sub_v(
        & newFocusPoint,
        & newEyePoint,
        & worldSpaceViewingDirection );
    worldSpaceCameraFocalDistance = vec3d_v_len( & worldSpaceViewingDirection );
    worldSpaceCameraRotation      = newCameraRotation;

    [ self _setupCamera ];

    return self;
}

- (id) filmResolution
                            : (IVec2D) newFilmResolution
        lensFocalLength     : (double) newFocalLength
        aperture            : (double) newAperture
        eyePoint            : (Pnt3D)  newEyePoint
        viewingDirection    : (Vec3D)  newViewingDirection
        cameraFocalDistance : (double) newFocalDistance
{
    return [ self
        filmResolution     : newFilmResolution
        lensFocalLength    : newFocalLength
        aperture           : newAperture
        eyePoint           : newEyePoint
        viewingDirection   : newViewingDirection
        cameraFocalDistance: newFocalDistance
        cameraRotation     : 0.0 ];
}

- (id) filmResolution
                            : (IVec2D) newFilmResolution
        lensFocalLength     : (double) newFocalLength
        aperture            : (double) newAperture
        eyePoint            : (Pnt3D)  newEyePoint
        viewingDirection    : (Vec3D)  newViewingDirection
        cameraFocalDistance : (double) newFocalDistance
        cameraRotation      : (double) newCameraRotation
{
    filmResolution                = newFilmResolution;
    worldSpaceLensFocalLength     = newFocalLength;
    aperture                      = newAperture;
    worldSpaceEyePoint            = newEyePoint;
    worldSpaceViewingDirection    = newViewingDirection;
    worldSpaceCameraFocalDistance = newFocalDistance;
    worldSpaceCameraRotation      = newCameraRotation;

    [ self _setupCamera ];

    return self;
}

- (id) init
        : (Vec2D)  newFilmSize
        : (IVec2D) newFilmResolution
        : (double) newFocalLength
        : (double) newAperture
        : (Pnt3D)  newEyePoint
        : (Vec3D)  newViewingDirection
        : (double) newFocalDistance
        : (double) newCameraRotation
{
    self = [ super init ];

    if ( self )
    {
        worldSpaceFilmSize            = newFilmSize;
        filmResolution                = newFilmResolution;
        worldSpaceLensFocalLength     = newFocalLength;
        aperture                      = newAperture;
        worldSpaceEyePoint            = newEyePoint;
        worldSpaceViewingDirection    = newViewingDirection;
        worldSpaceCameraFocalDistance = newFocalDistance;
        worldSpaceCameraRotation      = newCameraRotation;

        [ self _setupCamera ];
    }
    
    return self;
}

/*
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

*/

- (void) getImageSize
        :(IVec2D *) outImageSize
{
    *outImageSize = filmResolution;
}

- (void) setImageSize
        : (const IVec2D *) inImageSize
{
    filmResolution = *inImageSize;

    [ self _setupCamera ];
}

- (void) setupForObject
        : (ArNode <ArpBBox> *) inObject
        : (ArcObject <ArpReporter> *) reporter
{
    (void) inObject;
    (void) reporter;
    //   Unlike some other cameras, we do not need to do anything object-specific in
    //   this setup routine, so nothing happens here. (aw)
}

- (const Pnt3D *) eye
{
    return & worldSpaceEyePoint;
}

- (void) setEye
        : (const Pnt3D *) newEye
{
    worldSpaceEyePoint = *newEye;
}

- (double) near
{
    return 0.0;
}

- (BOOL) getWorldspaceRay
        : (const Vec2D *) pixelCoordinates
        : (      ArcObject <ArpRandomGenerator> *) randomGenerator
        : (      ArReferenceFrame *) outReferenceFrame
        : (      Ray3D *) worldSpaceViewingRay
{
    (void) outReferenceFrame;
    
    double  x0, x1;

    do
    {
        [ randomGenerator getValuesFromNewSequences
            : & x0
            : & x1 ];
    }
    while ( ( M_SQR(x0) + M_SQR(x1) ) > 1.0 );

    double  apertureScale =
          ( worldSpaceLensFocalLength * worldSpaceCameraFocalDistance * imageScale )
        / ( realAperture );

    Pnt3D  cameraSpaceRayStart =
        PNT3D( ( x0 - 0.5 ) * apertureScale,
               ( x1 - 0.5 ) * apertureScale, 0.0 );

    Pnt3D  cameraSpaceRayEnd;

    XC(cameraSpaceRayEnd) = XC(*pixelCoordinates) - XC(cameraSpaceFilmOffset);
    YC(cameraSpaceRayEnd) = XC(*pixelCoordinates) - YC(cameraSpaceFilmOffset);
    ZC(cameraSpaceRayEnd) = 1.0;

    Vec3D  cameraSpaceRayVector;

    vec3d_pp_sub_v(
        & cameraSpaceRayEnd,
        & cameraSpaceRayStart,
        & cameraSpaceRayVector );

    vec3d_v_htrafo3d_v(
        & cameraSpaceRayVector,
        & camera2world,
        & RAY3D_VECTOR(*worldSpaceViewingRay) );

    pnt3d_p_htrafo3d_p(
        & cameraSpaceRayStart,
        & camera2world,
        & RAY3D_POINT(*worldSpaceViewingRay) );

#ifdef WITH_POLARISATION
    //   This does not work, and will have to be fixed during the renovation
    //   of the polarisation-aware renderer
    Vec3D  auxRayVector;
    Vec3D  auxWorldVector;

    XC(auxRayVector) = x - xscale;
    YC(auxRayVector) = y - 1.0 - yscale;
    ZC(auxRayVector) = 1.0;
    vec3d_v_htrafo3d_v( & auxRayVector, & camera2world, & auxWorldVector );
    vec3d_norm_v(&auxWorldVector);

    if ( outReferenceFrame )
    {
        Vec3D   upVector;

        XC(upVector) = XC(outRay->vector) - XC(auxWorldVector);
        YC(upVector) = YC(outRay->vector) - YC(auxWorldVector);
        ZC(upVector) = ZC(outRay->vector) - ZC(auxWorldVector);

        vec3d_vv_cross_v( & outRay->vector,
                        & upVector,
                        & ARREFFRAME_RF_I( *outReferenceFrame, 0 ) );
        vec3d_vv_cross_v( & ARREFFRAME_RF_I( *outReferenceFrame, 0 ),
                        & outRay->vector,
                        & ARREFFRAME_RF_I( *outReferenceFrame, 1 ) );
        vec3d_norm_v(& ARREFFRAME_RF_I( *outReferenceFrame, 0 ));
        vec3d_norm_v(& ARREFFRAME_RF_I( *outReferenceFrame, 1 ));
    }
#endif // WITH_POLARISATION

    return YES;
}

- (BOOL) getPixelCoordinates
        : (const Ray3D *) inputRay
        : (      Vec2D *) pixelCoordinates
{
    (void) inputRay;
    (void) pixelCoordinates;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
    return NO;
}

- (double) getZoom
{
    return 1.0;
}


- (double) getTwist
{
    return 0.0;
}


- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];

    [ coder codeVec2D : & worldSpaceFilmSize ];
    [ coder codeIVec2D: & filmResolution ];
    [ coder codeDouble: & worldSpaceLensFocalLength ];
    [ coder codeDouble: & aperture ];
    [ coder codePnt3D : & worldSpaceEyePoint ];
    [ coder codeVec3D : & worldSpaceViewingDirection ];
    [ coder codeDouble: & worldSpaceCameraFocalDistance ];
    [ coder codeDouble: & worldSpaceCameraRotation ];

    if ( [ coder isReading ] )
        [ self _setupCamera ];
}

@end

// ===========================================================================
