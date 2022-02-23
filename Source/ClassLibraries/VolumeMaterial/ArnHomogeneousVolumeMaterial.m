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

#define ART_MODULE_NAME     ArnHomogeneousVolumeMaterial

#import "ArnHomogeneousVolumeMaterial.h"

#define IOR_COLOURNODE                                          \
    (( ArNode <ArpSpectrum> *)   [self subnodeWithIndex : 0])
#define ABSORPTION_COLOURNODE                                   \
    (( ArNode <ArpSpectrum> *)   [self subnodeWithIndex : 1])

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnHomogeneousVolumeMaterial registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

@implementation ArnHomogeneousVolumeMaterial

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnHomogeneousVolumeMaterial)
ARPVOLUME_MATERIAL_HOMOGENEOUS_IMPLEMENTATION

- (id) copy
{
    ArnHomogeneousVolumeMaterial  * copiedInstance = [ super copy ];

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnHomogeneousVolumeMaterial  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    return copiedInstance;
}

- (id) init
        : (ArNode <ArpSpectrum>      *) newIOR
        : (ArNode <ArpSpectrum>      *) newAbsorption
        : (ArNode <ArpPhaseFunction> *) newPhaseFunction
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

- (void) crossSection
        : (const Pnt3D            *) pointWorldspace
        : (const ArWavelength     *) wavelength
        : (      ArPathDirection   ) pathDirection
        : (      ArSpectralSample *) crossSection
{
    ArSpectralSample absorption;

    [ self absorptionCoefficient
        :   pointWorldspace
        :   wavelength
        : & absorption
        ];
    
    [ self scatteringCoefficient
        :   pointWorldspace
        :   wavelength
        :   pathDirection
        :   crossSection
        ];
    
    // absorption + scattering
    sps_s_add_s(
          art_gv,
        & absorption,
          crossSection
        );
}

- (void) absorptionCoefficient
        : (const Pnt3D            *) pointWorldspace
        : (const ArWavelength     *) wavelength
        : (      ArSpectralSample *) absorptionCoefficient
{
    [ ABSORPTION_COLOURNODE getSpectralSample
        :   0
        :   wavelength
        :   absorptionCoefficient
        ];
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];
}


@end
// ===========================================================================
