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

#define ART_MODULE_NAME     ArnUniformEnvironmentMaterial

#import "ArnUniformEnvironmentMaterial.h"

#import "SurfaceMaterialMacros.h"

ART_MODULE_INITIALISATION_FUNCTION
(
     (void) art_gv;
    [ ArnUniformEnvironmentMaterial  registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


/* ===========================================================================
    'ArnUniformEnvironmentMaterial'
=========================================================================== */

@implementation ArnUniformEnvironmentMaterial

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnUniformEnvironmentMaterial)

- (void) lightEmittedTowardsPointFromDirection
        : (ArcPointContext *) queryLocation_worldspace
        : (Vec3D *) queryDirection_worldspace
        : (ArSamplingRegion *) resultSamplingRegion
        : (ArLight *) resultLight
{
    (void) queryLocation_worldspace;
    (void) queryDirection_worldspace;
    
    if ( resultSamplingRegion ) *resultSamplingRegion = 0;

    ArSpectrum  * lightColour = spc_alloc( art_gv );

    [ SUB_COLOUR getSpectrum
        :   0
        :   lightColour
        ];

    arlight_s_init_unpolarised_l(
        art_gv,
        lightColour,
        resultLight
        );

    spc_free( art_gv, lightColour );

    arlight_d_mul_l(
        art_gv,
        parameter,
        resultLight
        );
}

- (void) spectralIntensityEmittedTowardsPointFromDirection
        : (ArcPointContext *) queryLocation_worldspace
        : (Vec3D *) queryDirection_worldspace
        : (ArSamplingRegion *) resultSamplingRegion
        : (ArSpectralIntensity *) resultIntensity
{
    (void) queryLocation_worldspace;
    (void) queryDirection_worldspace;
    
    if ( resultSamplingRegion ) *resultSamplingRegion = 0;

    ArSpectrum500 * lightColour = s500_alloc( art_gv );

    [ SUB_COLOUR getHiresSpectrum
        :   0
        :   lightColour
        ];

    arspectralintensity_s_init_i(
        art_gv,
        lightColour,
        resultIntensity
        );

    s500_free( art_gv, lightColour );

    arspectralintensity_d_mul_i(
        art_gv,
        parameter,
        resultIntensity
        );
}

- (void) lightSampleEmittedTowardsPointFromDirection
        : (ArcPointContext *) queryLocation_worldspace
        : (Vec3D *) queryDirection_worldspace
        : (const ArWavelength *) wavelength
        : (ArSamplingRegion *) resultSamplingRegion
        : (ArLightSample *) resultLightSample
{
    (void) queryLocation_worldspace;
    (void) queryDirection_worldspace;
    
    if ( resultSamplingRegion ) *resultSamplingRegion = 0;

    ArSpectralSample  lightColourSample;

    [ SUB_COLOUR getSpectralSample
        :   0
        :   wavelength
        : & lightColourSample
        ];

    arlightsample_s_init_unpolarised_l(
          art_gv,
        & lightColourSample,
          resultLightSample
        );

    arlightsample_d_mul_l(
          art_gv,
          parameter,
          resultLightSample
        );
}

@end

// ===========================================================================
