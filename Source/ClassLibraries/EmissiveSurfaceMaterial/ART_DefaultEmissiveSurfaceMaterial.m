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

#define ART_MODULE_NAME      ART_DefaultEmissiveSurfaceMaterial

#import "ART_DefaultEmissiveSurfaceMaterial.h"

#import "ART_ColourAndSpectra.h"

#import "ArnLambertSurfaceMaterial.h"
#import "ArnLambertEmissiveSurfaceMaterialConst.h"

typedef struct ART_DefaultEmissiveSurfaceMaterial_GV
{
    ArNode <ArpSurfaceMaterial>  * emitter;
}
ART_DefaultEmissiveSurfaceMaterial_GV;

#define ART_DEFAULTEMISSIVESURFACEMATERIAL_GV   art_gv->art_defaultemissivesurfacematerial_gv
#define ART_DEFAULT_EMISSIVE_SURFACE_MATERIAL_GV  ART_DEFAULTEMISSIVESURFACEMATERIAL_GV->emitter

ART_MODULE_INITIALISATION_FUNCTION
(
    ART_DEFAULTEMISSIVESURFACEMATERIAL_GV = \
        ALLOC(ART_DefaultEmissiveSurfaceMaterial_GV);

    ART_DEFAULT_EMISSIVE_SURFACE_MATERIAL_GV = 0;
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    RELEASE_OBJECT( ART_DEFAULT_EMISSIVE_SURFACE_MATERIAL_GV );

    FREE( ART_DEFAULTEMISSIVESURFACEMATERIAL_GV );
)

ART_SPECTRUM_MODULE_INITIALISATION_FUNCTION
(
    if ( ! ART_DEFAULT_EMISSIVE_SURFACE_MATERIAL_GV )
        ARNODE_SINGLETON(
            ART_DEFAULT_EMISSIVE_SURFACE_MATERIAL_GV,
            ART_DEFAULT_EMISSIVE_SURFACE_MATERIAL,
            [ ALLOC_INIT_OBJECT(ArnLambertEmissiveSurfaceMaterialConst)
                :   ARNGREY_100
                :   1.5
                ]
            );
    else
        [ ART_DEFAULT_EMISSIVE_SURFACE_MATERIAL_GV reinitialiseAfterISRChange ];
)

ART_SPECTRUM_MODULE_SHUTDOWN_FUNCTION
(
    if ( ART_DEFAULT_EMISSIVE_SURFACE_MATERIAL_GV )
        [ ART_DEFAULT_EMISSIVE_SURFACE_MATERIAL_GV prepareForISRChange ];
)


ArNode <ArpSurfaceMaterial> * art_default_emissive_surface_material(
        ART_GV  * art_gv
        )
{
    return
        ART_DEFAULT_EMISSIVE_SURFACE_MATERIAL_GV;
}

// ===========================================================================
