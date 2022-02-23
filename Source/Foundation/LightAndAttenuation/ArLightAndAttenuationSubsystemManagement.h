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

#ifndef _ART_FOUNDATION_LIGHTANDATTENUATION_SUBSYSTEM_MANAGEMENT_H_
#define _ART_FOUNDATION_LIGHTANDATTENUATION_SUBSYSTEM_MANAGEMENT_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArLightAndAttenuationSubsystemManagement)

void art_initialise_light_and_attenuation_subsystem(
        ART_GV  * art_gv
        );

void art_shutdown_light_and_attenuation_subsystem(
        ART_GV  * art_gv
        );

void arlightandattenuation_printf_current_subsystem_state(
        const ART_GV  * art_gv
        );

void arlightandattenuation_switch_to_plain(
        ART_GV  * art_gv
        );

void arlightandattenuation_switch_to_polarisable(
        ART_GV  * art_gv
        );

unsigned int arlightandattenuation_support_polarisation(
        const ART_GV  * art_gv
        );

#define LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE \
        arlightandattenuation_support_polarisation(art_gv)

#define INITIALISE_LIGHT_AND_ATTENUATION_MODULE(_name) \
_name ##_initialise_light_and_attenuation_subsystem( art_gv );

#define ART_LIGHT_AND_ATTENUATION_MODULE_INTERFACE(_name) \
void _name ##_initialise_light_and_attenuation_subsystem( \
        ART_GV  * art_gv \
        ); \
void _name ##_shutdown_light_and_attenuation_subsystem( \
        ART_GV  * art_gv \
        );

#define _ART_LIGHT_AND_ATTENUATION_MODULE_INIT_FUNCTION_HEADER_WITH_NAME( _name ) \
void _name##_initialise_light_and_attenuation_subsystem( \
        ART_GV  * art_gv \
        )
#define _ART_LIGHT_AND_ATTENUATION_MODULE_INIT_FUNCTION_HEADER( _name ) \
     _ART_LIGHT_AND_ATTENUATION_MODULE_INIT_FUNCTION_HEADER_WITH_NAME( _name )
#define ART_LIGHT_AND_ATTENUATION_MODULE_INIT_FUNCTION_HEADER \
     _ART_LIGHT_AND_ATTENUATION_MODULE_INIT_FUNCTION_HEADER( ART_MODULE_NAME )

#define ART_LIGHT_AND_ATTENUATION_MODULE_INITIALISATION_FUNCTION( _code ) \
ART_LIGHT_AND_ATTENUATION_MODULE_INIT_FUNCTION_HEADER \
{ \
    _code \
}

#define SHUTDOWN_LIGHT_AND_ATTENUATION_MODULE(_name) \
_name ##_shutdown_light_and_attenuation_subsystem( art_gv );

#define _ART_LIGHT_AND_ATTENUATION_MODULE_SD_FUNCTION_HEADER_WITH_NAME( _name ) \
void _name##_shutdown_light_and_attenuation_subsystem( \
        ART_GV  * art_gv \
        )
#define _ART_LIGHT_AND_ATTENUATION_MODULE_SD_FUNCTION_HEADER( _name ) \
     _ART_LIGHT_AND_ATTENUATION_MODULE_SD_FUNCTION_HEADER_WITH_NAME( _name )
#define ART_LIGHT_AND_ATTENUATION_MODULE_SD_FUNCTION_HEADER \
     _ART_LIGHT_AND_ATTENUATION_MODULE_SD_FUNCTION_HEADER( ART_MODULE_NAME )

#define ART_LIGHT_AND_ATTENUATION_MODULE_SHUTDOWN_FUNCTION( _code ) \
ART_LIGHT_AND_ATTENUATION_MODULE_SD_FUNCTION_HEADER \
{ \
    _code \
}

#define ART_LIGHT_AND_ATTENUATION_MODULE_NO_SHUTDOWN_FUNCTION_NECESSARY \
ART_LIGHT_AND_ATTENUATION_MODULE_SHUTDOWN_FUNCTION( (void) art_gv; )

#endif /* _ART_FOUNDATION_LIGHTANDATTENUATION_SUBSYSTEM_MANAGEMENT_H_ */
/* ======================================================================== */
