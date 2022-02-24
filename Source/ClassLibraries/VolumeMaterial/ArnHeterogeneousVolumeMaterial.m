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

#define ART_MODULE_NAME     ArnHeterogeneousVolumeMaterial

#import "ArnHeterogeneousVolumeMaterial.h"

#define IOR_COLOURNODE                                          \
    (( ArNode <ArpSpectrum> *)  [self subnodeWithIndex : 0])
#define ABSORPTION_COLOURNODE                                   \
    (( ArNode <ArpSpectrum> *)  [self subnodeWithIndex : 1])
#define DENSITY_VOLUMENODE                                      \
    (( ArNode <ArpVolume> *)    [self subnodeWithIndex : 2])

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnHeterogeneousVolumeMaterial registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArnHeterogeneousVolumeMaterial

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnHeterogeneousVolumeMaterial)

- (void) _scatteringMult
        : (const ArWavelength     *) wavelength
        : (      ArPathDirection   ) pathDirection
        : (      ArSpectralSample *) scatteringCoefficient
{
    [ super scatteringCoefficient
        :   NULL
        :   wavelength
        :   pathDirection
        :   scatteringCoefficient
        ];
}

- (id) copy
{
    ArnHeterogeneousVolumeMaterial  * copiedInstance = [ super copy ];

    copiedInstance->scale = scale;
    
    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnHeterogeneousVolumeMaterial  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    copiedInstance->scale = scale;
    
    return copiedInstance;
}

- (id) init
        : (ArNode <ArpSpectrum>      *) newIOR
        : (ArNode <ArpSpectrum>      *) newAbsorption
        : (ArNode <ArpPhaseFunction> *) newPhaseFunction
        : (ArNode <ArpVolume>        *) newDensityVolume
{
    ArNodeRefDynArray  array;

    array = arnoderefdynarray_init( 0 );

    arnoderefdynarray_push(
        & array,
          HARD_NODE_REFERENCE( newIOR )
        );
    
    arnoderefdynarray_push(
        & array,
          HARD_NODE_REFERENCE( newAbsorption )
        );
    
    arnoderefdynarray_push(
        & array,
          HARD_NODE_REFERENCE( newDensityVolume )
        );

    scale = 1.0;
    
    self =
        [ super init
            : & array
            :   newPhaseFunction
            ];

    arnoderefdynarray_free_contents( & array );
    
    return self;
}

- (id) init
        : (ArNode <ArpSpectrum>      *) newIOR
        : (ArNode <ArpSpectrum>      *) newAbsorption
        : (ArNode <ArpPhaseFunction> *) newPhaseFunction
        : (ArNode <ArpVolume>        *) newDensityVolume
        : (double                     ) newScaleDensity
{
    ArNodeRefDynArray  array;
    
    array = arnoderefdynarray_init( 0 );
    
    arnoderefdynarray_push(
        & array,
          HARD_NODE_REFERENCE( newIOR )
        );
    
    arnoderefdynarray_push(
        & array,
          HARD_NODE_REFERENCE( newAbsorption )
        );
    
    arnoderefdynarray_push(
        & array,
          HARD_NODE_REFERENCE( newDensityVolume )
        );

    scale = newScaleDensity;
    
    self =
        [ super init
            : & array
            :   newPhaseFunction
            ];

    arnoderefdynarray_free_contents( & array );

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

- (void) _crossSectionForDensity
        : (const ArWavelength     *) wavelength
        : (      ArPathDirection   ) pathDirection
        : (const double           *) density
        : (      ArSpectralSample *) transmittance
{
    ArSpectralSample absorption;
    
    [ ABSORPTION_COLOURNODE getSpectralSample
        :   0
        :   wavelength
        : & absorption
        ];
    
    [ self _scatteringMult
        :   wavelength
        :   pathDirection
        :   transmittance
        ];
    
    sps_s_add_s(art_gv, &absorption, transmittance);
    sps_d_mul_s(art_gv, *density, transmittance);
}

- (void) crossSection
        : (const Pnt3D            *) pointWorldspace
        : (const ArWavelength     *) wavelength
        : (      ArPathDirection   ) pathDirection
        : (      ArSpectralSample *) crossSection
{
    double density = 0;
    [ DENSITY_VOLUMENODE lookup :pointWorldspace :&density ];
    density *= scale;

    [ self _crossSectionForDensity
        :   wavelength
        :   pathDirection
        : & density
        :   crossSection
        ];
}

- (void) maxCrossSectionForRay
        : (const Ray3D            *) rayWorldspace
        : (const ArWavelength     *) wavelength
        : (      ArPathDirection   ) pathDirection
        : (      ArSpectralSample *) crossSection
{
    double maxDensity = 0;
    [ DENSITY_VOLUMENODE max :&maxDensity ];
    maxDensity *= scale;
    
    [ self _crossSectionForDensity
        :   wavelength
        :   pathDirection
        : & maxDensity
        :   crossSection
        ];
}

- (void) scatteringCoefficient
        : (const Pnt3D            *) pointWorldspace
        : (const ArWavelength     *) wavelength
        : (      ArPathDirection   ) pathDirection
        : (      ArSpectralSample *) scatteringCoefficient
{
    double density = 0;
    [ DENSITY_VOLUMENODE lookup :pointWorldspace :&density ];
    density *= scale;
    
    [ self _scatteringMult
         :  wavelength
         :  pathDirection
         :  scatteringCoefficient
         ];
    
    sps_d_mul_s(
          art_gv,
          density,
          scatteringCoefficient
        );
}

- (void) maxScatteringCoefficientForRay
        : (const Ray3D            *) rayWorldspace
        : (const ArWavelength     *) wavelength
        : (      ArPathDirection   ) pathDirection
        : (      ArSpectralSample *) maxScatteringCoefficient
{
    double maxDensity = 0;
    [ DENSITY_VOLUMENODE max :&maxDensity ];
    maxDensity *= scale;
    
    [ self _scatteringMult
         :  wavelength
         :  pathDirection
         :  maxScatteringCoefficient
     ];
    
    sps_d_mul_s(
          art_gv,
          maxDensity,
          maxScatteringCoefficient
        );
}

- (void) _absorptionForDensity
        : (const ArWavelength     *) wavelength
        : (const double           *) density
        : (      ArSpectralSample *) absorption
{
    [ ABSORPTION_COLOURNODE getSpectralSample
         :   0
         :   wavelength
         :   absorption
         ];
    
    sps_d_mul_s(art_gv, *density, absorption);
}

- (void) absorptionCoefficient
        : (const Pnt3D            *) pointWorldspace
        : (const ArWavelength     *) wavelength
        : (      ArSpectralSample *) absorptionCoefficient
{
    double density = 0;
    [ DENSITY_VOLUMENODE lookup :pointWorldspace :&density ];
    density *= scale;
    
    [ self _absorptionForDensity
        :   wavelength
        : & density
        :   absorptionCoefficient
        ];
}

- (void) maxAbsorptionCoefficientForRay
        : (const Ray3D            *) rayWorldspace
        : (const ArWavelength     *) wavelength
        : (      ArSpectralSample *) maxAbsorptionCoefficient
{
    double maxDensity = 0;
    [ DENSITY_VOLUMENODE max :&maxDensity ];
    maxDensity *= scale;
    
    [ self _absorptionForDensity
        :   wavelength
        : & maxDensity
        :   maxAbsorptionCoefficient
        ];
}

- (BOOL) rayIntersect
        : (const Ray3D *) rayWorldspace
        : (     double *) near
        : (     double *) far
{
    return
        [ DENSITY_VOLUMENODE rayIntersect
            :   rayWorldspace
            :   near
            :   far
            ];
}

- (BOOL) calculatePhaseFunctionSample
        : (      ArcRayEndpoint *)                incomingDirectionAndLocation
        : (      ArPathDirection)                 pathDirection
        : (      ArBSDFSampleGenerationContext *) context
        : (const ArWavelength *)                  incomingWavelength
        : (      ArWavelength *)                  sampledWavelength
        : (      Vec3D *)                         sampledDirection
        : (      ArPDFValue *)                    sampleProbability
        : (      ArPDFValue *)                    reverseSampleProbability
        : (      ArAttenuationSample *)           attenuationSample
{
    
    BOOL retVal =
        [ super calculatePhaseFunctionSample
            :   incomingDirectionAndLocation
            :   pathDirection
            :   context
            :   incomingWavelength
            :   sampledWavelength
            :   sampledDirection
            :   sampleProbability
            :   reverseSampleProbability
            :   attenuationSample
            ];
    
    double density, maxDensity;
    
    if ( retVal ) {
        [ DENSITY_VOLUMENODE lookup
            : & ARCRAYENDPOINT_WORLDSPACE_POINT(incomingDirectionAndLocation)
            : & density
            ];
        
        [ DENSITY_VOLUMENODE max
            : & maxDensity
            ];
        
        arattenuationsample_d_mul_a(
              art_gv,
              scale * density / maxDensity,
              attenuationSample
            );
    }
    
    return retVal;
}

- (BOOL) evaluatePhaseFunction
        : (      ArcRayEndpoint *)                incomingDirectionAndLocation
        : (const Vec3D *)                         outgoingDirection
        : (      ArPathDirection)                 pathDirection
        : (      ArBSDFSampleGenerationContext *) context
        : (const ArWavelength *)                  incomingWavelength
        : (const ArWavelength *)                  outgoingWavelength
        : (      ArPDFValue *)                    sampleProbability
        : (      ArPDFValue *)                    reverseSampleProbability
        : (      ArAttenuationSample *)           attenuationSample
{
    BOOL retVal =
        [ super evaluatePhaseFunction
            :   incomingDirectionAndLocation
            :   outgoingDirection
            :   pathDirection
            :   context
            :   incomingWavelength
            :   outgoingWavelength
            :   sampleProbability
            :   reverseSampleProbability
            :   attenuationSample
            ];
    
    double density, maxDensity;
    
    if ( retVal ) {
        [ DENSITY_VOLUMENODE lookup
            : & ARCRAYENDPOINT_WORLDSPACE_POINT(incomingDirectionAndLocation)
            : & density
            ];
        
        [ DENSITY_VOLUMENODE max
            : & maxDensity
            ];
        
        arattenuationsample_d_mul_a(
              art_gv,
              scale * density / maxDensity,
              attenuationSample
            );
    }
    
    return retVal;
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];

    [ coder codeDouble: &scale];
}

@end
// ===========================================================================
