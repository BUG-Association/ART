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

#define ART_MODULE_NAME     ArSpectralSample

#include "ArSpectralSample.h"

#include "SpectralDatatype_ImplementationMacros.h"

typedef struct ArSpectralSample_GV
{
    ArSpectralSample  sps_zero;
    ArSpectralSample  sps_unit;
    unsigned int      hero_samples_to_splat;
}
ArSpectralSample_GV;

#define ARSPECTRALSAMPLE_GV   art_gv->arspectralsample_gv

#define ARSPECTRALSAMPLE_GV_ZERO    ARSPECTRALSAMPLE_GV->sps_zero
#define ARSPECTRALSAMPLE_GV_UNIT    ARSPECTRALSAMPLE_GV->sps_unit
#define ARSPECTRALSAMPLE_GV_HSTS    ARSPECTRALSAMPLE_GV->hero_samples_to_splat

ART_MODULE_INITIALISATION_FUNCTION
(
    ARSPECTRALSAMPLE_GV = ALLOC(ArSpectralSample_GV);

    ARSPECTRALSAMPLE_GV_HSTS = 4;
 
    sps_d_init_s( art_gv, 0.0, & ARSPECTRALSAMPLE_GV_ZERO );
    sps_d_init_s( art_gv, 1.0, & ARSPECTRALSAMPLE_GV_UNIT );
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    FREE( ARSPECTRALSAMPLE_GV );
)


#define _ISR_CHANNELS           HERO_SAMPLES_TO_SPLAT
#define _ISR_C(_s0)             SPS_C(_s0)
#define _ISR_CI(_s0,_i)         SPS_CI(_s0,_i)

CANONICAL_IMPLEMENTATION_FOR_ISR_AND_HERO( ArSpectralSample, arspectralsample, SPS, sps, c4, s )

ADDITIONAL_IMPLEMENTATION_FOR_SPECTRAL_ISR_AND_HERO( ArSpectralSample, SPS, sps, c4, s )

#undef _ISR_CHANNELS
#undef _ISR_C
#undef _ISR_CI

unsigned int art_hero_samples_to_splat(
        const ART_GV  * art_gv
        )
{
    return  ARSPECTRALSAMPLE_GV_HSTS;
}

void art_set_hero_samples_to_splat(
              ART_GV        * art_gv,
        const unsigned int    new_hero_samples_to_splat
        )
{
    ARSPECTRALSAMPLE_GV_HSTS = new_hero_samples_to_splat;
}

void sps_sw_init_s(
        const ART_GV            * art_gv,
        const ArSpectrum        * s0,
        const ArWavelength      * w0,
              ArSpectralSample  * sr
        )
{
    ASSERT_VALID_SPECTRUM( s0 )
    ASSERT_VALID_WAVELENGTH( w0 )
    ASSERT_ALLOCATED_SPECTRUMTYPE( sps, ArSpectralSample, sr)

    unsigned int last_channel_idx = spc_channels( art_gv ) - 1;
    double range_start = spc_channel_lower_bound( art_gv, 0 );
    double range_end = spc_channel_lower_bound( art_gv, last_channel_idx )
                        + spc_channel_width( art_gv, last_channel_idx );
    double channel_width = spc_channel_width( art_gv, 0 );

    for ( int i = 0; i < 4; i++ )
    {
        double  wavelength = ARWL_WI( *w0,i );

        if ( wavelength < range_start )
        {
            SPS_CI( *sr, i ) = 0.0;
            continue;
        }

        if ( wavelength > range_end )
        {
            SPS_CI( *sr, i ) = 0.0;
            continue;
        }
        
        unsigned int  channel_idx =
            (unsigned int)( ( wavelength - range_start ) / channel_width );

        SPS_CI( *sr, i ) = spc_si( art_gv, s0, channel_idx );
    }

    ASSERT_VALID_SPECTRUMTYPE( sps, ArSpectralSample, sr, s)
}

void sps_s500w_init_s(
        const ART_GV            * art_gv,
        const ArSpectrum500     * s0,
        const ArWavelength      * w0,
              ArSpectralSample  * sr
        )
{
    ASSERT_VALID_WAVELENGTH( w0 )
    ASSERT_ALLOCATED_SPECTRUMTYPE( sps, ArSpectralSample, sr)
    
    unsigned int last_channel_idx = s500_channels( art_gv ) - 1;
    double range_start = s500_channel_lower_bound( art_gv, 0 );
    double range_end =   s500_channel_lower_bound( art_gv, last_channel_idx )
                       + s500_channel_width( art_gv, last_channel_idx );
    double channel_width = s500_channel_width( art_gv, 0 );

    for ( int i = 0; i < 4; i++ )
    {
        double  wavelength = ARWL_WI( *w0,i );

        if ( wavelength < range_start )
        {
            SPS_CI( *sr, i ) = 0.0;
            continue;
        }

        if ( wavelength > range_end )
        {
            SPS_CI( *sr, i ) = 0.0;
            continue;
        }
        
        unsigned int  channel_idx =
            (unsigned int)( ( wavelength - range_start ) / channel_width );

        SPS_CI( *sr, i ) = s500_si( art_gv, s0, channel_idx );
    }

    ASSERT_VALID_SPECTRUMTYPE( sps, ArSpectralSample, sr, s)
}

ArSpectralSample const * sps_one(
        const ART_GV  * art_gv
        )
{
    return & ARSPECTRALSAMPLE_GV_UNIT;
}

ArSpectralSample const * sps_zero(
        const ART_GV  * art_gv
        )
{
    return & ARSPECTRALSAMPLE_GV_ZERO;
}

ArSpectralSample * sps_alloc(
        const ART_GV  * art_gv
        )
{
    (void) art_gv;
    
    ArSpectralSample  * sr = ALLOC(ArSpectralSample);

    return sr;
}

ArSpectralSample * sps_d_alloc_init(
        const ART_GV  * art_gv,
        const double    d0
        )
{
    ArSpectralSample  * sr = sps_alloc( art_gv );

    sps_d_init_s( art_gv, d0, sr );

    return sr;
}

ArSpectralSample * sps_s_alloc_init(
        const ART_GV            * art_gv,
        const ArSpectralSample  * s0
        )
{
    (void) art_gv;
    
    ASSERT_VALID_SPECTRUMTYPE( sps, ArSpectralSample,  s0, s )

    ArSpectralSample  * hr = ALLOC( ArSpectralSample );

    ASSERT_ALLOCATED_SPECTRUMTYPE( hs, ArSpectralSample,  hr )

    if ( hr )
        memcpy( hr, s0, sizeof( ArSpectralSample ) );
    else
        ART_ERRORHANDLING_FATAL_ERROR( "ArSpectralSample pointer is NULL, cannot copy its contents!" );

    INITIALISE_ISR_ASSERTION_DATA( hr )

    ASSERT_VALID_SPECTRUMTYPE( sps, ArSpectralSample,  hr, s )

    return hr;
}

void sps_splatting_data_from_current_ISR_s(
        const ART_GV                         * art_gv,
              ArSpectralSampleSplattingData  * sr
        )
{
    int  numChannels = spc_channels( art_gv );

    sr->numChannels = (double) numChannels;
    
    sr->lastChannelIndex = numChannels - 1;
    
    sr->spectralRangeStart = spc_channel_lower_bound( art_gv, 0 );
    sr->spectralRangeEnd = spc_channel_lower_bound( art_gv, numChannels );
    
    sr->channelWidth = spc_channel_width( art_gv, 0 );

    unsigned int  s500_last_channel_idx = s500_channels( art_gv ) - 1;
    
    sr->s500_range_start = NANO_FROM_UNIT(s500_channel_lower_bound( art_gv, 0 ));
    
    sr->s500_range_end = NANO_FROM_UNIT(
          s500_channel_lower_bound( art_gv, s500_last_channel_idx )
        + s500_channel_width( art_gv, s500_last_channel_idx ));

    int  channelsEqual = 1;

    for ( int i = 1; i < numChannels; i++ )
    {
        if ( sr->channelWidth != spc_channel_width( art_gv, i ))
        {
            channelsEqual = 0;
            break;
        }
    }

    int current_spc_channel = 0;

    for ( int i = 0; i < 500; i++ )
    {
        double  current_wl =
            s500_channel_center(art_gv, i);

        sr->splatFactor0[i] = 1.0;
        sr->splatFactor1[i] = 0.0;

        if ( current_spc_channel == numChannels )
        {
            sr->splatChannel0[i] = INVALID_SPLAT_CHANNEL;
            sr->splatChannel1[i] = INVALID_SPLAT_CHANNEL;
        }
        else
        {
            if (  current_wl
                < spc_channel_lower_bound( art_gv, current_spc_channel ) )
            {
                sr->splatChannel0[i] = INVALID_SPLAT_CHANNEL;
                sr->splatChannel1[i] = INVALID_SPLAT_CHANNEL;
            }
            else
            {
                if (  current_wl
                    > (  spc_channel_lower_bound( art_gv, current_spc_channel )
                       + spc_channel_width( art_gv, current_spc_channel ) ) )
                {
                    current_spc_channel++;
                }
                
                if ( current_spc_channel == numChannels )
                {
                    sr->splatChannel0[i] = INVALID_SPLAT_CHANNEL;
                    sr->splatChannel1[i] = INVALID_SPLAT_CHANNEL;
                }
                else
                {
                    sr->splatChannel0[i] = current_spc_channel;
                    
                    if (  s500_channel_center(art_gv, i)
                        > spc_channel_center( art_gv, current_spc_channel ) )
                    {
                        sr->splatChannel1[i] = current_spc_channel+1;
                    }
                    else
                    {
                        sr->splatChannel1[i] = current_spc_channel-1;
                    }
                    
                    if (   sr->splatChannel1[i] < 0
                        || sr->splatChannel1[i] == numChannels )
                    {
                        sr->splatChannel1[i] = INVALID_SPLAT_CHANNEL;
                    }
                    else
                    {
                        double  channel_center =
                            spc_channel_center( art_gv, sr->splatChannel0[i] );
                        
                        double  channel_width =
                            spc_channel_width( art_gv, sr->splatChannel0[i] );

                        double  wl_dist = M_ABS(current_wl-channel_center);
                        
                        //   We only use bi-linear sample splatting iff
                        //   all channels are of equal width
                        
                        if (channelsEqual)
                        {
                            sr->splatFactor0[i] =
                                  (channel_width - wl_dist)
                                / channel_width;
                            sr->splatFactor1[i] = 1.-sr->splatFactor0[i];
                        }
                    }
                }
            }
        }

        if ( sr->splatChannel0[i] != INVALID_SPLAT_CHANNEL )
        {
            sr->splatFactor0[i] *=
                spc_channel_weight(art_gv, sr->splatChannel0[i]) * numChannels;// * 0.01;
        }

        if ( sr->splatChannel1[i] != INVALID_SPLAT_CHANNEL )
        {
            sr->splatFactor1[i] *=
                spc_channel_weight(art_gv, sr->splatChannel1[i]) * numChannels;// * 0.01;
        }
    }
    
//    debugprintf("\n")
//    for ( int i = 0; i < 500; i++ )
//    {
//        debugprintf("%2d ",sr->splatChannel0[i])
//    }
//    debugprintf("\n")
//    for ( int i = 0; i < 500; i++ )
//    {
//        debugprintf("%2d ",sr->splatChannel1[i])
//    }
//    debugprintf("\n")
//    for ( int i = 0; i < 500; i++ )
//    {
//        debugprintf("%5.3f ",sr->splatFactor0[i])
//    }
//    debugprintf("\n----\n")
//    for ( int i = 0; i < 500; i++ )
//    {
//        debugprintf("%5.3f ",sr->splatFactor1[i])
//    }
//    debugprintf("\n----\n")
//    for ( int i = 0; i < 500; i++ )
//    {
//        debugprintf("%5.3f ",sr->splatFactor1[i]+sr->splatFactor0[i])
//    }
//    debugprintf("\n")
}


/* ======================================================================== */
