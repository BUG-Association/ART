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

#define ART_MODULE_NAME     ArnHNSVolumeMaterial

#import "ArnHNSVolumeMaterial.h"

typedef struct ArnHNSVolumeMaterial_GV
{
    ArSpectrum  * absorptionCoefficient;
}
ArnHNSVolumeMaterial_GV;

#define ARN_HNS_VOLUME_MATERIAL_GV   art_gv->arnhnsvolumematerial_gv

#define ABSORPTION_COEFFICIENT_GV  \
    ARN_HNS_VOLUME_MATERIAL_GV->absorptionCoefficient

#define IOR_COLOURNODE          \
    (( ArNode <ArpSpectrum> *) [self subnodeWithIndex : 0])
#define EXTINCTION_COLOURNODE   \
    (( ArNode <ArpSpectrum> *) [self subnodeWithIndex : 1])

#define ABSORPTION_COEFFICIENT_LAMBDA(_lambda) \
    ( ( 4.0 * MATH_PI ) / (_lambda) )

ART_MODULE_INITIALISATION_FUNCTION
(
    [ ArnHNSVolumeMaterial registerWithRuntime ];

    ARN_HNS_VOLUME_MATERIAL_GV =
        ALLOC(ArnHNSVolumeMaterial_GV);

    ABSORPTION_COEFFICIENT_GV  = 0;
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    if ( ABSORPTION_COEFFICIENT_GV )
    {
        spc_free( art_gv, ABSORPTION_COEFFICIENT_GV );
        ABSORPTION_COEFFICIENT_GV = 0;
    }

    FREE( ARN_HNS_VOLUME_MATERIAL_GV );
)

ART_SPECTRUM_MODULE_INITIALISATION_FUNCTION
(
    if ( ! ABSORPTION_COEFFICIENT_GV )
    {
        ABSORPTION_COEFFICIENT_GV = spc_alloc( art_gv );

        int  numChannels = spc_channels( art_gv );

        for ( int i = 0; i < numChannels; i++ )
        {
            double  lambda = spc_channel_center( art_gv, i );
            double  alpha  = ABSORPTION_COEFFICIENT_LAMBDA(lambda);

            spc_set_sid(
                art_gv,
                ABSORPTION_COEFFICIENT_GV,
                i,
                alpha
                );
        }
    }

)

ART_SPECTRUM_MODULE_SHUTDOWN_FUNCTION
(
    if ( ABSORPTION_COEFFICIENT_GV )
    {
        spc_free( art_gv, ABSORPTION_COEFFICIENT_GV );
        ABSORPTION_COEFFICIENT_GV = 0;
    }
)

ArSpectrum const * art_absorptionCoefficient(
        const ART_GV  * art_gv
        )
{
    return ABSORPTION_COEFFICIENT_GV;
}

@implementation ArnHNSVolumeMaterial

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnHNSVolumeMaterial)
ARPVOLUME_MATERIAL_CLOSED_FORM_ONLY_IMPLEMENTATION

- (id) copy
{
    ArnHNSVolumeMaterial  * copiedInstance = [ super copy ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnHNSVolumeMaterial  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- (void) _precomputeMaterial
{
    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
        IOR_COLOURNODE,
        ArpSpectrum
        );

    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
        EXTINCTION_COLOURNODE,
        ArpSpectrum
        );

    ArSpectrum  *extinction = spc_alloc( art_gv );

    [ EXTINCTION_COLOURNODE getSpectrum
        :   0
        :   extinction
        ];

    int  numChannels = spc_channels( art_gv );

    opaque = YES;
    clear  = YES;

    for ( int i = 0; i < numChannels; i++ )
    {
        double  channelValue = spc_si( art_gv, extinction, i );

        //   If any single channel value is nonzero, the material
        //   is not entirely clear.

        if ( channelValue > 0.0 )
            clear = NO;

        //   If any single channel value is smaller than 1.0, the
        //   material is not opaque.

        if ( channelValue < 1.0 )
            opaque = NO;
    }

    spc_free(art_gv, extinction);
}

- (void) _setupMaterial
{
    [ self _precomputeMaterial ];
}

- (id) init
        : (ArNode <ArpSpectrum> *) newIOR
        : (ArNode <ArpSpectrum> *) newExtinction
{
    ArNodeRefDynArray  array;

    array = arnoderefdynarray_init( 0 );

    arnoderefdynarray_push(
        & array,
          HARD_NODE_REFERENCE( newIOR )
        );

    arnoderefdynarray_push(
        & array,
          HARD_NODE_REFERENCE( newExtinction )
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
    return opaque;
}

- (BOOL) isClear
{
    return clear;
}

- (BOOL) isLuminous
{
    return NO;
}

- (BOOL) isVolumetricMaterial
{
    return NO;
}

- (void) closedFormEmissionAndExtinctionSample
        : (const Ray3D               *) rayWorldspace
        : (const double               ) distance
        : (const ArPathDirection      ) pathDirection
        : (const ArWavelength        *) wavelength
        : (      ArAttenuationSample *) attenuation_r
        : (      ArLightSample       *) light_r
{
    ArSpectralSample extinctionSample;
    
    [ EXTINCTION_COLOURNODE getSpectralSample
        :   0
        :   wavelength
        : & extinctionSample
        ];
    
    ArSpectralSample transmittanceSample;

    if(distance >= MATH_HUGE_DOUBLE)
    {
        // for each of the wavelengths
        for(int i = 0; i < 4; ++i)
            SPS_CI(transmittanceSample, i) =        // non-clear medium acts as a blocker over inifinite distance
                SPS_CI(extinctionSample, i) > 0.0 ? 0.0 : 1.0;
                                                          // clear medium acts as free transmission
    }
    else
    {
        ArSpectralSample absorption;
        
        // extinctionSample * ABSORPTION_COEFFICIENT_LAMBDA(*wavelength)
        c4_cc_div_c(
            & ARWL_C(*wavelength),
            & SPS_C(extinctionSample),
            & SPS_C(absorption)
            );
            
        sps_d_mul_s(
              art_gv,
              4.0 * MATH_PI,
            & absorption
            );
        
        // exp(-absorption * distance)
        sps_ds_mul_s(
              art_gv,
              distance,
            & absorption,
            & transmittanceSample
            );
        
        sps_negexp_s(
              art_gv,
            & transmittanceSample
            );
    }
    
    ARATTENUATIONSAMPLE_V_PD_C_INIT_NONPOLARISING_A(
        & RAY3D_V(*rayWorldspace),
          pathDirection,
        & transmittanceSample,
          attenuation_r
        );

    ARLIGHTSAMPLE_INIT_AS_NONE(light_r);
}

- (void) code
    : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];

    if ( [ coder isReading ] )
        [ self _setupMaterial ];
}


@end
// ===========================================================================
