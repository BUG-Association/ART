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

#define ART_MODULE_NAME      ART_DefaultEnvironmentMaterial

#import "ART_DefaultEnvironmentMaterial.h"

#import "ART_ColourAndSpectra.h"

#import "ArnUniformEnvironmentMaterial.h"

typedef struct ART_DefaultEnvironmentMaterial_GV
{
    ArNode <ArpEnvironmentMaterial>  * environment_material;
}
ART_DefaultEnvironmentMaterial_GV;

#define ART_DEFAULTENVIRONMENTMATERIAL_GV   art_gv->art_defaultenvironmentmaterial_gv
#define ART_DEFAULT_ENVIRONMENT_MATERIAL_GV  ART_DEFAULTENVIRONMENTMATERIAL_GV->environment_material

ART_MODULE_INITIALISATION_FUNCTION
(
    ART_DEFAULTENVIRONMENTMATERIAL_GV = \
        ALLOC(ART_DefaultEnvironmentMaterial_GV);

    ART_DEFAULT_ENVIRONMENT_MATERIAL_GV = 0;
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    RELEASE_OBJECT( ART_DEFAULT_ENVIRONMENT_MATERIAL_GV );

    FREE( ART_DEFAULTENVIRONMENTMATERIAL_GV );
)

ART_SPECTRUM_MODULE_INITIALISATION_FUNCTION
(
    if ( ! ART_DEFAULT_ENVIRONMENT_MATERIAL_GV )
        ARNODE_SINGLETON(
            ART_DEFAULT_ENVIRONMENT_MATERIAL_GV,
            ART_DEFAULT_ENVIRONMENT_MATERIAL,
            [ ALLOC_INIT_OBJECT(ArnUniformEnvironmentMaterial)
                :   ARNGREY_100
                :   1.5
                ]
            );
    else
        [ ART_DEFAULT_ENVIRONMENT_MATERIAL_GV reinitialiseAfterISRChange ];
)

ART_SPECTRUM_MODULE_SHUTDOWN_FUNCTION
(
    if ( ART_DEFAULT_ENVIRONMENT_MATERIAL_GV )
        [ ART_DEFAULT_ENVIRONMENT_MATERIAL_GV prepareForISRChange ];
)


ArNode <ArpEnvironmentMaterial> * art_default_environment_material(
        ART_GV  * art_gv
        )
{
    return
        ART_DEFAULT_ENVIRONMENT_MATERIAL_GV;
}

// ===========================================================================
