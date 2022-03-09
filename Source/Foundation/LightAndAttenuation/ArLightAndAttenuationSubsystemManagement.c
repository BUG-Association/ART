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

#define ART_MODULE_NAME     ArLightAndAttenuationSubsystemManagement

#include "ArLightAndAttenuationSubsystemManagement.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#include "_ArSpectrum_GV.h"

#include "ArCrosstalk.h"
#include "ArSVLight.h"
#include "ArLight.h"
#include "ArLightAlpha.h"
#include "ArDirectAttenuation.h"
#include "ArPlainDirectAttenuation.h"
#include "ArMMDirectAttenuation.h"
#include "ArLight.h"
#include "ArAttenuation.h"

#include "_ArLight_GV.h"

#include "LightInterfaceMacros.h"

SWITCHING_INTERFACE_FOR_LCT(arplainlight)
SWITCHING_INTERFACE_FOR_LCT(arsvlight)
SWITCHING_INTERFACE_FOR_LCT(arplainlightsample)
SWITCHING_INTERFACE_FOR_LCT(arsvlightsample)

SWITCHING_INTERFACE_FOR_ACT(arplaindirectattenuation)
SWITCHING_INTERFACE_FOR_ACT(armmdirectattenuation)
SWITCHING_INTERFACE_FOR_ACT(arplaindirectattenuationsample)
SWITCHING_INTERFACE_FOR_ACT(armmdirectattenuationsample)

void art_initialise_light_and_attenuation_subsystem(
        ART_GV  * art_gv
        )
{
    INITIALISE_LIGHT_AND_ATTENUATION_MODULE( ArCrosstalk );
    INITIALISE_LIGHT_AND_ATTENUATION_MODULE( ArSVLight );
    INITIALISE_LIGHT_AND_ATTENUATION_MODULE( ArLightAlpha );
    INITIALISE_LIGHT_AND_ATTENUATION_MODULE( ArLightAlphaSample );
    INITIALISE_LIGHT_AND_ATTENUATION_MODULE( ArMMDirectAttenuation );
    INITIALISE_LIGHT_AND_ATTENUATION_MODULE( ArLight );
    INITIALISE_LIGHT_AND_ATTENUATION_MODULE( ArAttenuation );
}

void art_shutdown_light_and_attenuation_subsystem(
        ART_GV  * art_gv
        )
{
    SHUTDOWN_LIGHT_AND_ATTENUATION_MODULE( ArAttenuation );
    SHUTDOWN_LIGHT_AND_ATTENUATION_MODULE( ArLight );
    SHUTDOWN_LIGHT_AND_ATTENUATION_MODULE( ArMMDirectAttenuation );
    SHUTDOWN_LIGHT_AND_ATTENUATION_MODULE( ArLightAlpha );
    SHUTDOWN_LIGHT_AND_ATTENUATION_MODULE( ArLightAlphaSample );
    SHUTDOWN_LIGHT_AND_ATTENUATION_MODULE( ArSVLight );
    SHUTDOWN_LIGHT_AND_ATTENUATION_MODULE( ArCrosstalk );
}

void arlightandattenuation_printf_current_subsystem_state(
        const ART_GV  * art_gv
        )
{
    printf( "Status of the light and attenuation subsystem\n");
    printf( "---------------------------------------------\n");

    printf( "ArLight is set to      : ");
    if ( art_gv->arlight_gv->lct_has_been_initialised )
        printf( "%s\n", arlight_lct_string(art_gv) );
    else
        printf( "(subsystem not initialised yet)\n");

    printf( "ArAttenuation is set to: ");
    if ( art_gv->arlight_gv->lct_has_been_initialised )
        printf( "%s\n", ardirectattenuation_act_string(art_gv) );
    else
        printf( "(subsystem not initialised yet)\n");

    fflush(stdout);
}

void arlightandattenuation_switch_to_plain(
        ART_GV  * art_gv
        )
{
    art_shutdown_light_and_attenuation_subsystem( art_gv );

    switch_lct_to_arplainlight(
        art_gv
        );

    switch_lct_to_arplainlightsample(
        art_gv
        );

    switch_act_to_arplaindirectattenuation(
        art_gv
        );

    switch_act_to_arplaindirectattenuationsample(
        art_gv
        );

    art_initialise_light_and_attenuation_subsystem( art_gv );
}

void arlightandattenuation_switch_to_polarisable(
        ART_GV  * art_gv
        )
{
    art_shutdown_light_and_attenuation_subsystem( art_gv );

    switch_lct_to_arsvlight(
        art_gv
        );

    switch_lct_to_arsvlightsample(
        art_gv
        );

    switch_act_to_armmdirectattenuation(
        art_gv
        );

    switch_act_to_armmdirectattenuationsample(
        art_gv
        );

    art_initialise_light_and_attenuation_subsystem( art_gv );
}

unsigned int arlightandattenuation_support_polarisation(
        const ART_GV  * art_gv
        )
{
    return (art_gv->arspectrum_gv->current_isr & ardt_polarisable);
}

/* ======================================================================== */
