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

#define ART_MODULE_NAME      ART_DefaultVolumeMaterial

#import "ART_DefaultVolumeMaterial.h"

#import "ArnConstSpectrum.h"
#import "ArnHNSVolumeMaterial.h"

typedef struct ART_DefaultVolumeMaterial_GV
{
    ArNode <ArpVolumeMaterial>  * air;
    ArNode <ArpVolumeMaterial>  * object;
}
ART_DefaultVolumeMaterial_GV;

#define ART_DEFAULTMATERIAL_GV      art_gv->art_defaultvolumematerial_gv
#define ART_DEFAULT_VOLUME_MATERIAL_AIR_GV ART_DEFAULTMATERIAL_GV->air
#define ART_DEFAULT_VOLUME_MATERIAL_OBJ_GV ART_DEFAULTMATERIAL_GV->object


ART_MODULE_INITIALISATION_FUNCTION
(
    ART_DEFAULTMATERIAL_GV = ALLOC(ART_DefaultVolumeMaterial_GV);

    ART_DEFAULT_VOLUME_MATERIAL_AIR_GV = 0;
    ART_DEFAULT_VOLUME_MATERIAL_OBJ_GV = 0;
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    RELEASE_OBJECT( ART_DEFAULT_VOLUME_MATERIAL_AIR_GV );
    RELEASE_OBJECT( ART_DEFAULT_VOLUME_MATERIAL_OBJ_GV );

    FREE( ART_DEFAULTMATERIAL_GV );
)

ART_SPECTRUM_MODULE_INITIALISATION_FUNCTION
(
    if ( ! ART_DEFAULT_VOLUME_MATERIAL_AIR_GV )
        ARNODE_SINGLETON(
            ART_DEFAULT_VOLUME_MATERIAL_AIR_GV,
            ART_DEFAULT_VOLUME_MATERIAL_AIR,
            [ ALLOC_INIT_OBJECT(ArnHNSVolumeMaterial)
                : ARNGREY_100
                : ARNGREY_000
                ]
            );
    else
        [ ART_DEFAULT_VOLUME_MATERIAL_AIR_GV reinitialiseAfterISRChange ];

    if ( ! ART_DEFAULT_VOLUME_MATERIAL_OBJ_GV )
        ARNODE_SINGLETON(
            ART_DEFAULT_VOLUME_MATERIAL_OBJ_GV,
            ART_DEFAULT_VOLUME_MATERIAL_OBJECT,
            [ ALLOC_INIT_OBJECT(ArnHNSVolumeMaterial)
                : ARNGREY_100
                : ARNGREY_100
                ]
            );
    else
        [ ART_DEFAULT_VOLUME_MATERIAL_OBJ_GV reinitialiseAfterISRChange ];
)

ART_SPECTRUM_MODULE_SHUTDOWN_FUNCTION
(
    if ( ART_DEFAULT_VOLUME_MATERIAL_AIR_GV )
        [ ART_DEFAULT_VOLUME_MATERIAL_AIR_GV prepareForISRChange ];
    if ( ART_DEFAULT_VOLUME_MATERIAL_OBJ_GV )
        [ ART_DEFAULT_VOLUME_MATERIAL_OBJ_GV prepareForISRChange ];
)



ArNode <ArpVolumeMaterial> * art_default_volume_material_air(
        ART_GV  * art_gv
        )
{
    return
        ART_DEFAULT_VOLUME_MATERIAL_AIR_GV;
}

ArNode <ArpVolumeMaterial> * art_default_volume_material_object(
        ART_GV  * art_gv
        )
{
    return
        ART_DEFAULT_VOLUME_MATERIAL_OBJ_GV;
}


// ===========================================================================
