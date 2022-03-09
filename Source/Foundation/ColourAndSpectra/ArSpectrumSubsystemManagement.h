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

#ifndef _ART_FOUNDATION_COLOURANDSPECTRA_ARSPECTRUM_SUBSYSTEM_MANAGEMENT_H_
#define _ART_FOUNDATION_COLOURANDSPECTRA_ARSPECTRUM_SUBSYSTEM_MANAGEMENT_H_

#include "ART_Foundation_System.h"

#include "ArDataType.h"

ART_MODULE_INTERFACE(ArSpectrumSubsystemManagement)

void art_foundation_set_isr(
        ART_GV          * art_gv,
        ArDataType    isr
        );

ArDataType  art_foundation_isr(
        const ART_GV  * art_gv
        );

void art_foundation_initialise_spectral_subsystem(
        ART_GV  * art_gv
        );

#define INITIALISE_COLOUR_MODULE(_name) \
_name ##_initialise_spectrum_subsystem( art_gv );

#define ART_SPECTRUM_MODULE_INTERFACE(_name) \
void _name ##_initialise_spectrum_subsystem( \
        ART_GV  * art_gv \
        ); \
\
void _name ##_shutdown_spectrum_subsystem( \
        ART_GV  * art_gv \
        );

#define _ART_SPECTRUM_MODULE_INIT_FUNCTION_HEADER_WITH_NAME( _name ) \
void _name ##_initialise_spectrum_subsystem( \
        ART_GV  * art_gv \
        )
#define _ART_SPECTRUM_MODULE_INIT_FUNCTION_HEADER( _name ) \
     _ART_SPECTRUM_MODULE_INIT_FUNCTION_HEADER_WITH_NAME( _name )
#define ART_SPECTRUM_MODULE_INIT_FUNCTION_HEADER \
     _ART_SPECTRUM_MODULE_INIT_FUNCTION_HEADER( ART_MODULE_NAME )

#define ART_SPECTRUM_MODULE_INITIALISATION_FUNCTION( _code ) \
ART_SPECTRUM_MODULE_INIT_FUNCTION_HEADER \
{ \
    _code \
}

#define SHUTDOWN_COLOUR_MODULE(_name) \
_name ##_shutdown_spectrum_subsystem( art_gv );

#define _ART_SPECTRUM_MODULE_SHUTDOWN_FUNCTION_HEADER_WITH_NAME( _name ) \
void _name ##_shutdown_spectrum_subsystem( \
        ART_GV  * art_gv \
        )
#define _ART_SPECTRUM_MODULE_SHUTDOWN_FUNCTION_HEADER( _name ) \
     _ART_SPECTRUM_MODULE_SHUTDOWN_FUNCTION_HEADER_WITH_NAME( _name )
#define ART_SPECTRUM_MODULE_SHUTDOWN_FUNCTION_HEADER \
     _ART_SPECTRUM_MODULE_SHUTDOWN_FUNCTION_HEADER( ART_MODULE_NAME )

#define ART_SPECTRUM_MODULE_SHUTDOWN_FUNCTION( _code ) \
ART_SPECTRUM_MODULE_SHUTDOWN_FUNCTION_HEADER \
{ \
    _code \
}

#define ART_NO_SPECTRUM_MODULE_SHUTDOWN_FUNCTION_NECESSARY \
ART_SPECTRUM_MODULE_SHUTDOWN_FUNCTION( )

#endif /* _ART_FOUNDATION_COLOURANDSPECTRA_ARSPECTRUM_SUBSYSTEM_MANAGEMENT_H_ */
/* ======================================================================== */
