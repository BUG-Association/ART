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

#ifndef _ARHOSEK_SKYMODEL_ART_FRONTEND_H_
#define _ARHOSEK_SKYMODEL_ART_FRONTEND_H_

#include "ART_Foundation_ColourAndSpectra.h"
#include "ART_Foundation_LightAndAttenuation.h"
#include "ArHosekSkyModel.h"

ART_MODULE_INTERFACE(ArHosekSkyModel_ART_frontend)

void arhosekskymodel_spc(
        ART_GV                 * art_gv,
        ArHosekSkyModelState  ** state,
        double                   theta,
        double                   gamma,
        ArSpectrum               * result
        );

void arhosekskymodel_solar_spc(
        ART_GV                 * art_gv,
        ArHosekSkyModelState  ** state,
        double                   theta,
        double                   gamma,
        ArSpectrum               * result
        );

void  arhosekskymodel_sps(
              ART_GV                 * art_gv,
              ArHosekSkyModelState  ** state,
        const double                   theta,
        const double                   gamma,
        const ArWavelength           * wavelength,
              ArSpectralSample       * spectral_sample
        );

void arhosekskymodel_solar_sps(
              ART_GV                 * art_gv,
              ArHosekSkyModelState  ** state,
        const double                   theta,
        const double                   gamma,
        const ArWavelength           * wavelength,
              ArSpectralSample       * spectral_sample
        );

#endif // _ARHOSEK_SKYMODEL_ART_FRONTEND_H_

// ===========================================================================
