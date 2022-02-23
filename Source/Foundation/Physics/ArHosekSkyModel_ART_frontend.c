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

#define ART_MODULE_NAME     ArHosekSkyModel_ART_frontend

#include "ColourAndLightSubsystem.h"
#include "ArHosekSkyModel_ART_frontend.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY
ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

void arhosekskymodel_spc(
        ART_GV                 * art_gv,
        ArHosekSkyModelState  ** state,
        double                   theta,
        double                   gamma,
        ArSpectrum             * result
        )
{
    if (   ART_CURRENT_ISR == ardt_xyz
        || ART_CURRENT_ISR == ardt_xyz_polarisable )
    {
        ArCIEXYZ  xyz;

        ARCIEXYZ_X(xyz) =
            arhosek_tristim_skymodel_radiance(
                state[0],
                theta,
                gamma,
                0
              );

        ARCIEXYZ_Y(xyz) =
            arhosek_tristim_skymodel_radiance(
                state[1],
                theta,
                gamma,
                1
              );

        ARCIEXYZ_Z(xyz) =
            arhosek_tristim_skymodel_radiance(
                state[2],
                theta,
                gamma,
                2
              );

        xyz_to_spc( art_gv, & xyz, result );
    }
    else
    {
        const unsigned int  num_channels = spc_channels( art_gv );

        for ( unsigned int i = 0; i < num_channels; i++ )
            spc_set_sid(
                  art_gv,
                  result,
                  i,
                  arhosekskymodel_radiance(
                        state[i],
                        theta,
                        gamma,
                        NANO_FROM_UNIT(spc_channel_center( art_gv, i ))
                      )
                );
    }
}

void arhosekskymodel_solar_spc(
        ART_GV                 * art_gv,
        ArHosekSkyModelState  ** state,
        double                   theta,
        double                   gamma,
        ArSpectrum               * result
        )
{
    if (   ART_CURRENT_ISR == ardt_xyz
        || ART_CURRENT_ISR == ardt_xyz_polarisable )
    {
        ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
//        ArSpectrum46  * sr_s46;
//
//        sr_s46 = s46_alloc( art_gv );
//
//        int  num_channels = s46_channels( art_gv );
//
//        for ( unsigned int i = 0; i < num_channels; i++ )
//            s46_set_sid(
//                  art_gv,
//                  sr_s46,
//                  i,
//                  arhosekskymodel_solar_radiance(
//                        state[i],
//                        theta,
//                        gamma,
//                        NANO_FROM_UNIT(s46_channel_center( art_gv, i ))
//                        )
//                );
//
//        s46_to_spc( art_gv, sr_s46, result );
//
//        s46_free( art_gv, sr_s46 );
    }
    else
    {
        const unsigned int  num_channels = spc_channels( art_gv );

        for ( unsigned int i = 0; i < num_channels; i++ )
            spc_set_sid(
                  art_gv,
                  result,
                  i,
                  arhosekskymodel_solar_radiance(
                        state[i],
                        theta,
                        gamma,
                        NANO_FROM_UNIT(spc_channel_center( art_gv, i ))
                        )
                );
    }
}

double  arhosekskymodel_mono_sample(
              ART_GV                 * art_gv,
              ArHosekSkyModelState  ** state,
        const double                   theta,
        const double                   gamma,
        const double                   wavelength
        )
{
    if ( wavelength < s11_channel_center( art_gv, 0 ) )
    {
        return
            arhosekskymodel_radiance(
                state[0],
                theta,
                gamma,
                NANO_FROM_UNIT(s11_channel_center( art_gv, 0 ))
                );
    }
    
    if ( wavelength > s11_channel_center( art_gv, 10 ) )
    {
        return
            arhosekskymodel_radiance(
                state[10],
                theta,
                gamma,
                NANO_FROM_UNIT(s11_channel_center( art_gv, 10 ))
                );
    }
    
    int     i_a = 0, i_b = 1;  // low and high i for the S11 bands
    double  c_a, c_b;          // low and high S11 band centers
    double  ic_a = 1.0, ic_b = 0.0;

    c_a = s11_channel_center( art_gv, i_a );
    c_b = s11_channel_center( art_gv, i_b );
    
    for ( unsigned int j = 0; j < 11; j++ )
    {
        if (   wavelength >= c_a
            && wavelength <  c_b )
        {
            double  cw = c_b - c_a;
            ic_b = ( wavelength - c_a ) / cw;
            ic_a = 1.0 - ic_b;
            
            break;
        }
        
        i_a++; i_b++;
        c_a = c_b;
        c_b = s11_channel_center( art_gv, i_b );
    }
    
    double  sr_a, sr_b;  // results for low and high S11 bands

    sr_a =
        arhosekskymodel_radiance(
            state[i_a],
            theta,
            gamma,
            NANO_FROM_UNIT(c_a)
            );
    sr_b =
        arhosekskymodel_radiance(
            state[i_b],
            theta,
            gamma,
            NANO_FROM_UNIT(c_b)
            );

    return
        ic_a * sr_a + ic_b * sr_b;
}

double  arhosekskymodel_mono_solar_sample(
              ART_GV                 * art_gv,
              ArHosekSkyModelState  ** state,
        const double                   theta,
        const double                   gamma,
        const double                   wavelength
        )
{
    if ( wavelength < s11_channel_center( art_gv, 0 ) )
    {
        return
            arhosekskymodel_solar_radiance(
                state[0],
                theta,
                gamma,
                NANO_FROM_UNIT(s11_channel_center( art_gv, 0 ))
                );
    }
    
    if ( wavelength > s11_channel_center( art_gv, 10 ) )
    {
        return
            arhosekskymodel_solar_radiance(
                state[10],
                theta,
                gamma,
                NANO_FROM_UNIT(s11_channel_center( art_gv, 10 ))
                );
    }
   
    int     i_a = 0, i_b = 1;  // low and high i for the S11 bands
    double  c_a, c_b;          // low and high S11 band centers
    double  ic_a = 1.0, ic_b = 0.0;

    c_a = s11_channel_center( art_gv, i_a );
    c_b = s11_channel_center( art_gv, i_b );
    
    for ( unsigned int j = 0; j < 11; j++ )
    {
        if (   wavelength >= c_a
            && wavelength <  c_b )
        {
            double  cw = c_b - c_a;
            ic_b = ( wavelength - c_a ) / cw;
            ic_a = 1.0 - ic_b;
            
            break;
        }
        
        i_a++; i_b++;
        c_a = c_b;
        c_b = s11_channel_center( art_gv, i_b );
    }

    double  sr_a, sr_b;  // results for low and high S11 bands
 
    sr_a =
        arhosekskymodel_solar_radiance(
            state[i_a],
            theta,
            gamma,
            NANO_FROM_UNIT(c_a)
            );
   sr_b =
        arhosekskymodel_solar_radiance(
            state[i_b],
            theta,
            gamma,
            NANO_FROM_UNIT(c_b)
            );

    return
        ic_a * sr_a + ic_b * sr_b;
}

void  arhosekskymodel_sps(
              ART_GV                 * art_gv,
              ArHosekSkyModelState  ** state,
        const double                   theta,
        const double                   gamma,
        const ArWavelength           * wavelength,
              ArSpectralSample       * spectral_sample
        )
{
    if (   ART_CURRENT_ISR == ardt_xyz
        || ART_CURRENT_ISR == ardt_xyz_polarisable )
    {
        ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
    }
    else
    {
        for ( unsigned int i = 0; i < HERO_SAMPLES_TO_SPLAT; i++ )
        {
            SPS_CI( *spectral_sample, i ) =
                arhosekskymodel_mono_sample(
                    art_gv,
                    state,
                    theta,
                    gamma,
                    ARWL_WI(*wavelength,i)
                    );
        }
    }
}

void arhosekskymodel_solar_sps(
              ART_GV                 * art_gv,
              ArHosekSkyModelState  ** state,
        const double                   theta,
        const double                   gamma,
        const ArWavelength           * wavelength,
              ArSpectralSample       * spectral_sample
        )
{
    if (   ART_CURRENT_ISR == ardt_xyz
        || ART_CURRENT_ISR == ardt_xyz_polarisable )
    {
        ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
    }
    else
    {
        for ( unsigned int i = 0; i < HERO_SAMPLES_TO_SPLAT; i++ )
        {
            SPS_CI( *spectral_sample, i ) =
                arhosekskymodel_mono_solar_sample(
                    art_gv,
                    state,
                    theta,
                    gamma,
                    ARWL_WI(*wavelength,i)
                    );
        }
    }
}

// ===========================================================================
