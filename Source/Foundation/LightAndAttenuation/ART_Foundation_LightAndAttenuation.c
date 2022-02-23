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

#define ART_LIBRARY_NAME     ART_Foundation_LightAndAttenuation

#include "ART_Foundation_LightAndAttenuation.h"



/* ---------------------------------------------------------------------------

    IMPORTANT
    =========

    The order in which the various "MODULE_INITIALISATIONs" are performed is
    *NOT* arbitrary. Putting these e.g. in alphabetical order would break the
    system, so just don't do that please...

------------------------------------------------------------------------aw- */


ART_LIBRARY_INITIALISATION_FUNCTION
(
    ART_PERFORM_MODULE_INITIALISATION( ArReferenceFrame )
    
    ART_PERFORM_MODULE_INITIALISATION( ArStokesVector )
    ART_PERFORM_MODULE_INITIALISATION( ArStokesVectorSample )

    ART_PERFORM_MODULE_INITIALISATION( ArMuellerMatrix )
    ART_PERFORM_MODULE_INITIALISATION( ArMuellerMatrixSample )

    ART_PERFORM_MODULE_INITIALISATION( ArCrosstalk500 )
    ART_PERFORM_MODULE_INITIALISATION( ArCrosstalk )
    ART_PERFORM_MODULE_INITIALISATION( ArCrosstalkSample )

    ART_PERFORM_MODULE_INITIALISATION( ArLightIntensity )
    ART_PERFORM_MODULE_INITIALISATION( ArLightIntensitySample )
 
    ART_PERFORM_MODULE_INITIALISATION( ArPlainLight )
    ART_PERFORM_MODULE_INITIALISATION( ArPlainLightSample )
 
    ART_PERFORM_MODULE_INITIALISATION( ArSVLight )
    ART_PERFORM_MODULE_INITIALISATION( ArSVLightSample )
 
    ART_PERFORM_MODULE_INITIALISATION( ArLight )
    ART_PERFORM_MODULE_INITIALISATION( ArLightSample )
 
    ART_PERFORM_MODULE_INITIALISATION( ArLightAlpha )
    ART_PERFORM_MODULE_INITIALISATION( ArLightAlphaSample )
 
    ART_PERFORM_MODULE_INITIALISATION( ArPlainDirectAttenuation )
    ART_PERFORM_MODULE_INITIALISATION( ArPlainDirectAttenuationSample )

    ART_PERFORM_MODULE_INITIALISATION( ArMMDirectAttenuation )
    ART_PERFORM_MODULE_INITIALISATION( ArMMDirectAttenuationSample )

    ART_PERFORM_MODULE_INITIALISATION( ArDirectAttenuation )
    ART_PERFORM_MODULE_INITIALISATION( ArDirectAttenuationSample )

    ART_PERFORM_MODULE_INITIALISATION( ArAttenuation )
    ART_PERFORM_MODULE_INITIALISATION( ArAttenuationSample )

    ART_PERFORM_MODULE_INITIALISATION( ArLightAndAttenuationSubsystemManagement )
)

ART_AUTOMATIC_LIBRARY_SHUTDOWN_FUNCTION


/* ======================================================================== */
