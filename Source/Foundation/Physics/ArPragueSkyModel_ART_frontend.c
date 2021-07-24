/* ===========================================================================

    Copyright (c) 1996-2018 The ART Development Team
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

#define ART_MODULE_NAME     ArPragueSkyModel_ART_frontend

#include "ColourAndLightSubsystem.h"
#include "ArPragueSkyModel_ART_frontend.h"
#include "ArPragueSkyModel.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY
ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

void arpragueskymodel_polarised_light_sample(
              ART_GV                 * art_gv,
        const ArPragueSkyModelState  * state,
        const double                   sunTheta,
        const double                   sunPhi,
        const Vec3D                  * viewDirection,
        const double                   viewTheta,
        const double                   viewGamma,
        const double                   altitude,
        const double                   turbidity,
        const double                   albedo,
        const double                   wavelength,
        const ArSpectralSample       * unpolarisedRadiance,
              ArLightSample          * result
        )
{
    Vec3D  sunDir;

    XC(sunDir) =     cos( sunPhi )
                   * cos( sunTheta );
    YC(sunDir) =     sin( sunPhi )
                   * cos( sunTheta );
    ZC(sunDir) =     sin( sunTheta );

    Vec3D  base1;

    vec3d_dv_mul_v( -1.0, viewDirection, & base1 );

    Vec3D  base2;

    vec3d_vv_cross_v( & base1, & sunDir, & base2 );

    Vec3D  base3;

    vec3d_vv_cross_v( & base1, & base2, & base3 );

    double  dop =
        arpragueskymodel_polarisation(
              state,
              viewTheta,
              viewGamma,
              sunTheta,
              altitude,
              turbidity,
              albedo,
              wavelength
            );

    ArReferenceFrame  referenceFrame;

    ARREFFRAME_RF_I( referenceFrame, 0 ) = base2;
    ARREFFRAME_RF_I( referenceFrame, 1 ) = base3;

    vec3d_norm_v( & ARREFFRAME_RF_I( referenceFrame, 0 ) );
    vec3d_norm_v( & ARREFFRAME_RF_I( referenceFrame, 1 ) );

    ArStokesVectorSample  svs;

    ARSVS_I( svs, 0 ) = *unpolarisedRadiance;
    ARSVS_I( svs, 1 ) = SPS4(0.0);
    SPS_CI(ARSVS_I( svs, 1 ), 0) = dop * SPS_CI(*unpolarisedRadiance,0);
    ARSVS_I( svs, 2 ) = SPS4(0.0);
    ARSVS_I( svs, 3 ) = SPS4(0.0);

    arlightsample_s_rf_init_polarised_l(
          art_gv,
        & svs,
        & referenceFrame,
          result
        );
}

void arpragueskymodel_polarised_light_sample_halfhero(
              ART_GV                 * art_gv,
        const ArPragueSkyModelState  * state,
        const double                   sunTheta,
        const double                   sunPhi,
        const Vec3D                  * viewDirection,
        const double                   viewTheta,
        const double                   viewGamma,
        const double                   altitude,
        const double                   turbidity,
        const ArSpectralSample       * albedo,
        const ArWavelength           * wavelength,
        const ArSpectralSample       * unpolarisedRadiance,
              ArLightSample          * result
        )
{
    Vec3D  sunDir;

    XC(sunDir) =     cos( sunPhi )
                   * cos( sunTheta );
    YC(sunDir) =     sin( sunPhi )
                   * cos( sunTheta );
    ZC(sunDir) =     sin( sunTheta );

    Vec3D  base1;

    vec3d_dv_mul_v( -1.0, viewDirection, & base1 );

    Vec3D  base2;

    vec3d_vv_cross_v( & base1, & sunDir, & base2 );

    Vec3D  base3;

    vec3d_vv_cross_v( & base1, & base2, & base3 );

    ArSpectralSample dop;
    for (unsigned int i = 0; i < HERO_SAMPLES_TO_SPLAT; ++i)
    {
        SPS_CI(dop, i) =
        arpragueskymodel_polarisation(
              state,
              viewTheta,
              viewGamma,
              sunTheta,
              altitude,
              turbidity,
              SPS_CI(*albedo,i),
              NANO_FROM_UNIT(ARWL_WI(*wavelength,i))
            );
    }

    ArSpectralSample linearPolarisation;
    sps_ss_mul_s(
          art_gv,
        & dop,
          unpolarisedRadiance,
        & linearPolarisation
        );

    ArReferenceFrame  referenceFrame;

    ARREFFRAME_RF_I( referenceFrame, 0 ) = base2;
    ARREFFRAME_RF_I( referenceFrame, 1 ) = base3;

    vec3d_norm_v( & ARREFFRAME_RF_I( referenceFrame, 0 ) );
    vec3d_norm_v( & ARREFFRAME_RF_I( referenceFrame, 1 ) );

    ArStokesVectorSample  svs;

    ARSVS_I( svs, 0 ) = *unpolarisedRadiance;
    ARSVS_I( svs, 1 ) = linearPolarisation;
    ARSVS_I( svs, 2 ) = SPS4(0.0);
    ARSVS_I( svs, 3 ) = SPS4(0.0);

    arlightsample_s_rf_init_polarised_l(
          art_gv,
        & svs,
        & referenceFrame,
          result
        );
}

void arpragueskymodel_polarised_light_sample_hero(
              ART_GV                 * art_gv,
        const ArPragueSkyModelState  * state,
        const double                   sunTheta,
        const double                   sunPhi,
        const Vec3D                  * viewDirection,
        const double                   viewTheta,
        const double                   viewGamma,
        const double                   altitude,
        const double                   turbidity,
        const double                   albedo,
        const ArWavelength           * wavelength,
        const ArSpectralSample       * unpolarisedRadiance,
              ArLightSample          * result
        )
{
    Vec3D  sunDir;

    XC(sunDir) =     cos( sunPhi )
                   * cos( sunTheta );
    YC(sunDir) =     sin( sunPhi )
                   * cos( sunTheta );
    ZC(sunDir) =     sin( sunTheta );

    Vec3D  base1;

    vec3d_dv_mul_v( -1.0, viewDirection, & base1 );

    Vec3D  base2;

    vec3d_vv_cross_v( & base1, & sunDir, & base2 );

    Vec3D  base3;

    vec3d_vv_cross_v( & base1, & base2, & base3 );

    ArSpectralSample dop;
    arpragueskymodel_polarisation_hero(
          art_gv,
          state,
          viewTheta,
          viewGamma,
          sunTheta,
          altitude,
          turbidity,
          albedo,
          wavelength,
        & dop
        );

    ArSpectralSample linearPolarisation;
    sps_ss_mul_s(
          art_gv,
        & dop,
          unpolarisedRadiance,
        & linearPolarisation
        );

    ArReferenceFrame  referenceFrame;

    ARREFFRAME_RF_I( referenceFrame, 0 ) = base2;
    ARREFFRAME_RF_I( referenceFrame, 1 ) = base3;

    vec3d_norm_v( & ARREFFRAME_RF_I( referenceFrame, 0 ) );
    vec3d_norm_v( & ARREFFRAME_RF_I( referenceFrame, 1 ) );

    ArStokesVectorSample  svs;

    ARSVS_I( svs, 0 ) = *unpolarisedRadiance;
    ARSVS_I( svs, 1 ) = linearPolarisation;
    ARSVS_I( svs, 2 ) = *SS_ZERO;
    ARSVS_I( svs, 3 ) = *SS_ZERO;

    arlightsample_s_rf_init_polarised_l(
          art_gv,
        & svs,
        & referenceFrame,
          result
        );
}

void arpragueskymodel_spc(
        ART_GV                       * art_gv,
        const ArPragueSkyModelState  * state,
        const double                   theta,
        const double                   gamma,
        const double                   shadow,
        const double                   zero,
        const double                   elevation,
        const double                   altitude,
        const double                   turbidity,
        const ArSpectrum             * groundAlbedo,
              ArSpectrum             * result
        )
{
    if (   ART_CURRENT_ISR == ardt_rgb
        || ART_CURRENT_ISR == ardt_rgb_polarisable )
    {
        ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
    }
    else
    {
        const unsigned int  num_channels = spc_channels( art_gv );

        for ( unsigned int i = 0; i < num_channels; i++ )
            spc_set_sid(
                  art_gv,
                  result,
                  i,
                  arpragueskymodel_radiance(
                        state,
                        theta,
                        gamma,
                        shadow,
                        zero,
                        elevation,
                        altitude,
                        turbidity,
                        spc_si(art_gv,groundAlbedo,i),
                        NANO_FROM_UNIT(spc_channel_center( art_gv, i ))
                      )
                );
    }
}

void arpragueskymodel_solar_spc(
        ART_GV                       * art_gv,
        const ArPragueSkyModelState  * state,
        const double                   theta,
        const double                   gamma,
        const double                   shadow,
        const double                   zero,
        const double                   elevation,
        const double                   altitude,
        const double                   turbidity,
        const ArSpectrum             * groundAlbedo,
              ArSpectrum             * result
        )
{
    if (   ART_CURRENT_ISR == ardt_rgb
        || ART_CURRENT_ISR == ardt_rgb_polarisable )
    {
        ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
    }
    else
    {
        const unsigned int  num_channels = spc_channels( art_gv );

        for ( unsigned int i = 0; i < num_channels; i++ )
            spc_set_sid(
                  art_gv,
                  result,
                  i,
                  arpragueskymodel_solar_radiance(
                        state,
                        theta,
                        gamma,
                        shadow,
                        zero,
                        elevation,
                        altitude,
                        turbidity,
                        spc_si(art_gv,groundAlbedo,i),
                        NANO_FROM_UNIT(spc_channel_center( art_gv, i ))
                        )
                );
    }
}

// ===========================================================================
