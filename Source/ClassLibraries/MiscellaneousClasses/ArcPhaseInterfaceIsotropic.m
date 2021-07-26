/* ===========================================================================

    Copyright (c) 1996-2021 The ART Development Team
    -------------------------------------------

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

#define ART_MODULE_NAME     ArcPhaseInterfaceIsotropic

#import "ArcPhaseInterfaceIsotropic.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

@implementation ArcPhaseInterfaceIsotropic

#define IOR_SPECTRUM_FROM \
    (( ArNode <ArpSpectrum> *) [fromMaterial subnodeWithIndex : 0])
#define EXTINCTION_SPECTRUM_FROM \
    (( ArNode <ArpSpectrum> *) [fromMaterial subnodeWithIndex : 1])

#define IOR_SPECTRUM_INTO \
    (( ArNode <ArpSpectrum> *) [intoMaterial subnodeWithIndex : 0])
#define EXTINCTION_SPECTRUM_INTO \
    (( ArNode <ArpSpectrum> *) [intoMaterial subnodeWithIndex : 1])

- (id) init
        : (ArNode <ArpVolumeMaterial> *) fromMaterial
        : (ArNode <ArpVolumeMaterial> *) intoMaterial
{
    self = [ super init ];
    
    if ( self )
    {
        mFrom = fromMaterial;
        mInto = intoMaterial;
        ARCPHASEINTERFACE_EXACT_IOR(*self) = spc_alloc( art_gv );
        ARCPHASEINTERFACE_EXACT_INV_IOR(*self) = spc_alloc( art_gv );
        ARCPHASEINTERFACE_EXACT_EXTINCTION_FROM(*self) = spc_alloc( art_gv );
        ARCPHASEINTERFACE_EXACT_EXTINCTION_INTO(*self) = spc_alloc( art_gv );
        ARCPHASEINTERFACE_HIRES_IOR(*self) = s500_alloc( art_gv );
        ARCPHASEINTERFACE_HIRES_INV_IOR(*self) = s500_alloc( art_gv );
        ARCPHASEINTERFACE_HIRES_EXTINCTION_FROM(*self) = s500_alloc( art_gv );
        ARCPHASEINTERFACE_HIRES_EXTINCTION_INTO(*self) = s500_alloc( art_gv );

        ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
            IOR_SPECTRUM_FROM,
            ArpSpectrum
            );

        ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
            EXTINCTION_SPECTRUM_FROM,
            ArpSpectrum
            );

        ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
            IOR_SPECTRUM_INTO,
            ArpSpectrum
            );

        ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
            EXTINCTION_SPECTRUM_INTO,
            ArpSpectrum
            );

        ArSpectrum *temp1 = spc_alloc( art_gv );
        ArSpectrum *temp2 = spc_alloc( art_gv );
        
        // First we calculate the IOR nt/ni for each wavelengh
        // and its inverse (ni/nt)

        [ IOR_SPECTRUM_FROM getSpectrum
            :   0
            :   temp1
            ];

        [ IOR_SPECTRUM_INTO getSpectrum
            :   0
            :   temp2
            ];

        spc_ss_div_s(
            art_gv,
            temp1,
            temp2,
            ARCPHASEINTERFACE_EXACT_IOR(*self)
            );

        spc_ss_div_s(
            art_gv,
            temp2,
            temp1,
            ARCPHASEINTERFACE_EXACT_INV_IOR(*self)
            );

        for ( int i = 0; i < 500 ; i++ )
        {
            ArWavelength  lambda;
            
            arwavelength_d_init_w(
                  art_gv,
                  s500_channel_center(art_gv, i),
                & lambda
                );

            ArSpectralSample temp1Sample, temp2Sample;

            [ IOR_SPECTRUM_FROM getSpectralSample
                :   0
                : & lambda
                : & temp1Sample
                ];

            [ IOR_SPECTRUM_INTO getSpectralSample
                :   0
                : & lambda
                : & temp2Sample
                ];

            ArSpectralSample iorSample, invIorSample;
            
            sps_ss_div_s(
                  art_gv,
                & temp1Sample,
                & temp2Sample,
                & iorSample
                );
            
            sps_ss_div_s(
                  art_gv,
                & temp2Sample,
                & temp1Sample,
                & invIorSample
                );

            s500_set_sid(
                  art_gv,
                  ARCPHASEINTERFACE_HIRES_IOR(*self),
                  i,
                  SPS_CI(iorSample, 0)
                );
            
            s500_set_sid(
                  art_gv,
                  ARCPHASEINTERFACE_HIRES_INV_IOR(*self),
                  i,
                  SPS_CI(invIorSample, 0)
                );
        }

        // For e.g. non-dispersive rendering we are only interested in
        // the average IOR

        ARCPHASEINTERFACE_AVG_IOR(*self) =
            spc_s_avg(
                art_gv,
                ARCPHASEINTERFACE_EXACT_IOR(*self)
                );

        ARCPHASEINTERFACE_AVG_INV_IOR(*self) =
            spc_s_avg(
                art_gv,
                ARCPHASEINTERFACE_EXACT_INV_IOR(*self)
                );

        ARCPHASEINTERFACE_AVG_IOR_I(*self) =
            spc_s_avg(
                art_gv,
                temp1
                );

        ARCPHASEINTERFACE_AVG_IOR_T(*self) =
            spc_s_avg(
                art_gv,
                temp2
                );

        ASSERT_NONNEGATIVE_FINITE_DOUBLE( ARCPHASEINTERFACE_AVG_IOR_I(*self) );
        ASSERT_NONNEGATIVE_FINITE_DOUBLE( ARCPHASEINTERFACE_AVG_IOR_T(*self) );

        // We do the same for the extinction coefficient

        [ EXTINCTION_SPECTRUM_FROM getSpectrum
            :   0
            :   ARCPHASEINTERFACE_EXACT_EXTINCTION_FROM(*self)
            ];

        [ EXTINCTION_SPECTRUM_INTO getSpectrum
            :   0
            :   ARCPHASEINTERFACE_EXACT_EXTINCTION_INTO(*self)
            ];

        for ( int i = 0; i < 500 ; i++ )
        {
            ArWavelength  lambda;
            
            arwavelength_d_init_w(
                  art_gv,
                  s500_channel_center(art_gv, i),
                & lambda
                );

            ArSpectralSample fromSample, intoSample;

            [ EXTINCTION_SPECTRUM_FROM getSpectralSample
                :   0
                : & lambda
                : & fromSample
                ];
            
            [ EXTINCTION_SPECTRUM_INTO getSpectralSample
                :   0
                : & lambda
                : & intoSample
                ];

            s500_set_sid(
                  art_gv,
                  ARCPHASEINTERFACE_HIRES_EXTINCTION_FROM(*self),
                  i,
                  SPS_CI(fromSample, 0)
                );
            
            s500_set_sid(
                  art_gv,
                  ARCPHASEINTERFACE_HIRES_EXTINCTION_INTO(*self),
                  i,
                  SPS_CI(intoSample, 0)
                );
        }

        ARCPHASEINTERFACE_AVG_EXT_FROM(*self) =
            spc_s_avg(
                art_gv,
                ARCPHASEINTERFACE_EXACT_EXTINCTION_FROM(*self)
                );

        ARCPHASEINTERFACE_AVG_EXT_INTO(*self) =
            spc_s_avg(
                art_gv,
                ARCPHASEINTERFACE_EXACT_EXTINCTION_INTO(*self)
                );

        ASSERT_NONNEGATIVE_FINITE_DOUBLE( ARCPHASEINTERFACE_AVG_EXT_FROM(*self) );
        ASSERT_NONNEGATIVE_FINITE_DOUBLE( ARCPHASEINTERFACE_AVG_EXT_INTO(*self) );

        // Finally we need to set the type of the phase interface

        ARCPHASEINTERFACE_TYPE(*self) = arphaseinterface_simple;

        for ( unsigned int i = 0; i < spc_channels( art_gv ); i++ )
        {

            double delta_IOR =
                  ARCPHASEINTERFACE_AVG_IOR_T(*self)
                - spc_si( art_gv, temp2, i );

            if ( delta_IOR > 0.001 )
                ARCPHASEINTERFACE_TYPE(*self) |= arphaseinterface_spectral;

            if ( spc_si(
                    art_gv,
                    ARCPHASEINTERFACE_EXACT_EXTINCTION_INTO(*self),
                    i
                    )
                 > 0.0 )
                ARCPHASEINTERFACE_TYPE(*self) |= arphaseinterface_complex;
        }

        spc_free(art_gv, temp1);
        spc_free(art_gv, temp2);
    }
    
    return self;

}

- (void) dealloc
{
    spc_free(art_gv, ARCPHASEINTERFACE_EXACT_IOR(*self));
    spc_free(art_gv, ARCPHASEINTERFACE_EXACT_INV_IOR(*self));
    spc_free(art_gv, ARCPHASEINTERFACE_EXACT_EXTINCTION_FROM(*self));
    spc_free(art_gv, ARCPHASEINTERFACE_EXACT_EXTINCTION_INTO(*self));

    s500_free(art_gv, ARCPHASEINTERFACE_HIRES_IOR(*self));
    s500_free(art_gv, ARCPHASEINTERFACE_HIRES_INV_IOR(*self));
    s500_free(art_gv, ARCPHASEINTERFACE_HIRES_EXTINCTION_FROM(*self));
    s500_free(art_gv, ARCPHASEINTERFACE_HIRES_EXTINCTION_INTO(*self));

     [ super dealloc ];
}



@end

// ===========================================================================
