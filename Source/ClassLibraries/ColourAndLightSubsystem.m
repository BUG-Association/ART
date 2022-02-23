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

#define ART_MODULE_NAME     ColourAndLightSubsystem

#include "ColourAndLightSubsystem.h"

#include "_ArSpectrum_GV.h"

typedef struct ArCLS_GV
{
    ArWavelengthSamplingData       spectralSamplingData;
    ArSpectralSampleSplattingData  spectralSplattingData;
}
ArCLS_GV;

#define ARCLS_GV                    art_gv->arcls_gv
#define ARCLS_SAMPLING_DATA         ARCLS_GV->spectralSamplingData
#define ARCLS_SPLATTING_DATA        ARCLS_GV->spectralSplattingData

ART_MODULE_INITIALISATION_FUNCTION
(
    ARCLS_GV = ALLOC(ArCLS_GV);
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    FREE( ARCLS_GV );
)


#include "ART_ColourAndSpectra.h"
#include "ART_Physics.h"
#include "ART_SkyModel.h"
#include "ART_SurfaceMaterial.h"
#include "ART_EmissiveSurfaceMaterial.h"
#include "ART_EnvironmentMaterial.h"
#include "ART_VolumeMaterial.h"

void art_initialise_spectrum_subsystem(
        ART_GV  * art_gv
        )
{
    INITIALISE_COLOUR_MODULE( ArnConstSpectrum );
    INITIALISE_COLOUR_MODULE( ArnPreethamSkyModel );
    INITIALISE_COLOUR_MODULE( ART_DefaultSurfaceMaterial );
    INITIALISE_COLOUR_MODULE( ART_DefaultEmissiveSurfaceMaterial );
    INITIALISE_COLOUR_MODULE( ART_DefaultEnvironmentMaterial );
    INITIALISE_COLOUR_MODULE( ART_DefaultVolumeMaterial );
    INITIALISE_COLOUR_MODULE( ArnHNSVolumeMaterial );
}

void art_shutdown_spectrum_subsystem(
        ART_GV  * art_gv
        )
{
    SHUTDOWN_COLOUR_MODULE( ArnConstSpectrum );
    SHUTDOWN_COLOUR_MODULE( ArnPreethamSkyModel );
    SHUTDOWN_COLOUR_MODULE( ART_DefaultSurfaceMaterial );
    SHUTDOWN_COLOUR_MODULE( ART_DefaultEmissiveSurfaceMaterial );
    SHUTDOWN_COLOUR_MODULE( ART_DefaultEnvironmentMaterial );
    SHUTDOWN_COLOUR_MODULE( ART_DefaultVolumeMaterial );
    SHUTDOWN_COLOUR_MODULE( ArnHNSVolumeMaterial );
}

void art_set_default_rgbspace_ref(
        ART_GV               * art_gv,
        ArColourSpace const  * newRef
        )
{
    art_shutdown_spectrum_subsystem( art_gv );

    set_default_rgbspace_ref ( art_gv, newRef );

    art_initialise_spectrum_subsystem( art_gv );
}

void art_set_isr(
        ART_GV          * art_gv,
        ArDataType    isr
        )
{
    art_shutdown_spectrum_subsystem( art_gv );

    art_foundation_set_isr( art_gv, isr );

    switch (isr)
    {
        case ardt_xyz:
        case ardt_spectrum8:
        case ardt_spectrum11:
        case ardt_spectrum18:
        case ardt_spectrum46:
        case ardt_spectrum500:
            arlightandattenuation_switch_to_plain( art_gv );
            break;

        case ardt_xyz_polarisable:
        case ardt_spectrum8_polarisable:
        case ardt_spectrum11_polarisable:
        case ardt_spectrum18_polarisable:
        case ardt_spectrum46_polarisable:
        case ardt_spectrum500_polarisable:
            arlightandattenuation_switch_to_polarisable( art_gv );
            break;

        default:
            ART_ERRORHANDLING_FATAL_ERROR(
                "invalid ISR"
                );
            break;
    }

    art_initialise_spectrum_subsystem( art_gv );
    
    //   Get sample splatting pre-computed data

    arwavelength_sampling_data_from_current_ISR_s(
          art_gv,
        & ARCLS_SAMPLING_DATA
        );

    sps_splatting_data_from_current_ISR_s(
          art_gv,
        & ARCLS_SPLATTING_DATA
        );
}

ArDataType  art_isr(
        ART_GV  * art_gv
        )
{
    return
        art_gv->arspectrum_gv->current_isr;
}

const ArWavelengthSamplingData  * art_isr_sampling_data(
        const ART_GV  * art_gv
        )
{
    return & ARCLS_SAMPLING_DATA;
}

const ArSpectralSampleSplattingData  * art_isr_splatting_data(
        const ART_GV  * art_gv
        )
{
    return & ARCLS_SPLATTING_DATA;
}

