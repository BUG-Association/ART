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

#define ART_MODULE_NAME     ArnSphereVolumeMaterial

#import "ArnSphereVolumeMaterial.h"

#define OUTER_ABSORPTION_COLOURNODE          \
    (( ArNode <ArpSpectrum> *) [self subnodeWithIndex : 0])
#define SPHERE_ABSORPTION_COLOURNODE   \
    (( ArNode <ArpSpectrum> *) [self subnodeWithIndex : 1])

#define ABSORPTION_COEFFICIENT_LAMBDA(_lambda) \
    ( ( 4.0 * MATH_PI ) / (_lambda) )

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnSphereVolumeMaterial registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArnSphereVolumeMaterial

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnSphereVolumeMaterial)
ARPVOLUME_MATERIAL_ABSORPTION_ONLY

- (id) copy
{
    ArnSphereVolumeMaterial  * copiedInstance = [ super copy ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnSphereVolumeMaterial  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- (void) _setupMaterial
{
}

- (id) init
        : (Pnt3D                 ) newSphereCenter
        : (double                ) newSphereSize
        : (ArNode <ArpSpectrum> *) outerAbsorbCrossSection
        : (ArNode <ArpSpectrum> *) sphereAbsorbCrossSection
{
    sphereCenter = newSphereCenter;
    sphereSize = newSphereSize;
    

    ArNodeRefDynArray  array;

    array = arnoderefdynarray_init( 0 );

    arnoderefdynarray_push(
        & array,
          HARD_NODE_REFERENCE( outerAbsorbCrossSection )
        );

    arnoderefdynarray_push(
        & array,
          HARD_NODE_REFERENCE( sphereAbsorbCrossSection )
        );

    self =
        [ super init
            : & array
            ];

    arnoderefdynarray_free_contents( & array );

    if ( self )
    {
        [ self _setupMaterial ];
    }
    
    return self;
}

- (BOOL) isOpaque
{
    return NO;
}

- (BOOL) isClear
{
    return NO;
}

- (BOOL) isLuminous
{
    return NO;
}

- (BOOL) isHomogeneous
{
    return NO;
}

- (void) crossSection
        : (const Pnt3D            *) pointWorldspace
        : (const ArWavelength     *) wavelength
        : (      ArPathDirection   ) pathDirection
        : (      ArSpectralSample *) crossSection
{
    (void) pathDirection;
    
    [ self absorptionCoefficient
        :   pointWorldspace
        :   wavelength
        :   crossSection
        ];
}

- (void) maxCrossSectionForRay
        : (const Ray3D            *) rayWorldspace
        : (const ArWavelength     *) wavelength
        : (      ArPathDirection   ) pathDirection
        : (      ArSpectralSample *) maxCrossSection
{
    (void) pathDirection;
    
    [ self maxAbsorptionCoefficientForRay
        :   rayWorldspace
        :   wavelength
        :   maxCrossSection
        ];
}

- (void) absorptionCoefficient
        : (const Pnt3D            *) pointWorldspace
        : (const ArWavelength     *) wavelength
        : (      ArSpectralSample *) absorptionCoefficient
{
    Vec3D distanceToCenter;
    
    vec3d_pp_sub_v(
          pointWorldspace,
        & sphereCenter,
        & distanceToCenter
        );
    
    ArNode <ArpSpectrum> * node =
        (vec3d_v_sqrlen(& distanceToCenter) <= M_SQR(sphereSize) ?
                SPHERE_ABSORPTION_COLOURNODE :
                OUTER_ABSORPTION_COLOURNODE);
    
    [ node getSpectralSample
        :   0
        :   wavelength
        :   absorptionCoefficient
        ];
}

- (void) maxAbsorptionCoefficientForRay
        : (const Ray3D            *) rayWorldspace
        : (const ArWavelength     *) wavelength
        : (      ArSpectralSample *) maxAbsorptionCoefficient
{
    (void) rayWorldspace;
    
    // TODO : more clever stuff with the ray
    ArSpectralSample outerSample, sphereSample;
    
    [ OUTER_ABSORPTION_COLOURNODE getSpectralSample
        :   0
        :   wavelength
        : & outerSample
        ];
    
    [ SPHERE_ABSORPTION_COLOURNODE getSpectralSample
        :   0
        :   wavelength
        : & sphereSample
        ];
    
    sps_ss_max_s(
          art_gv,
        & outerSample,
        & sphereSample,
          maxAbsorptionCoefficient
        );
}

- (BOOL) rayIntersect
        : (const Ray3D  *) ray_worldspace
        : (      double *) near
        : (      double *) far
{
    (void) ray_worldspace;
    (void) near;
    (void) far;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
    
    return NO;
}


- (BOOL) providesClosedFormEmissionAndExtinction
{
    return NO;
}


- (void) closedFormEmissionAndExtinctionSample
        : (const Ray3D               *) ray_worldspace
        : (const double               ) distance
        : (const ArPathDirection      ) pathDirection
        : (const ArWavelength        *) wavelength
        : (      ArAttenuationSample *) attenuation_r
        : (      ArLightSample       *) light_r
{
    (void) ray_worldspace;
    (void) distance;
    (void) pathDirection;
    (void) wavelength;
    (void) attenuation_r;
    (void) light_r;
    
    ART_ERRORHANDLING_FATAL_ERROR(
        "This volume does not provide a closed form for emission / extinction"
        );
}


- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];

    [ coder codePnt3D: & sphereCenter ];
    [ coder codeDouble: & sphereSize ];

    if ( [ coder isReading ] )
        [ self _setupMaterial ];
}

@end
// ===========================================================================
