/* ===========================================================================

    Copyright (c) 1996-2017 The ART Development Team
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

#ifndef _ARPRAGUESKYMODEL_ART_FRONTEND_H_
#define _ARPRAGUESKYMODEL_ART_FRONTEND_H_

#include "ART_Foundation_ColourAndSpectra.h"
#include "ART_Foundation_LightAndAttenuation.h"
#include "ArPragueSkyModel.h"

ART_MODULE_INTERFACE(ArPragueSkyModel_ART_frontend)

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
        );

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
        );

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
        );

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
        );

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
        );

#endif // _ARPRAGUESKYMODEL_ART_FRONTEND_H_

// ===========================================================================
