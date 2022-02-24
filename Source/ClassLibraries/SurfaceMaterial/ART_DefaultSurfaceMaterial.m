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

#define ART_MODULE_NAME      ART_DefaultSurfaceMaterial

#import "ART_DefaultSurfaceMaterial.h"

#import "ART_ColourAndSpectra.h"

#import "ArnLambertSurfaceMaterial.h"
#import "ArnLambertEmissiveSurfaceMaterialConst.h"

typedef struct ART_DefaultSurfaceMaterial_GV
{
    ArNode <ArpSurfaceMaterial>  * surfaceMaterial;
}
ART_DefaultSurfaceMaterial_GV;

#define ART_DEFAULTSURFACEMATERIAL_GV   art_gv->art_defaultsurfacematerial_gv
#define ART_DEFAULT_SURFACE_MATERIAL_GV  ART_DEFAULTSURFACEMATERIAL_GV->surfaceMaterial

ART_MODULE_INITIALISATION_FUNCTION
(
    ART_DEFAULTSURFACEMATERIAL_GV = ALLOC(ART_DefaultSurfaceMaterial_GV);

    ART_DEFAULT_SURFACE_MATERIAL_GV = 0;
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    RELEASE_OBJECT( ART_DEFAULT_SURFACE_MATERIAL_GV );

    FREE( ART_DEFAULTSURFACEMATERIAL_GV );
)

ART_SPECTRUM_MODULE_INITIALISATION_FUNCTION
(
    if ( ! ART_DEFAULT_SURFACE_MATERIAL_GV )
        ARNODE_SINGLETON(
            ART_DEFAULT_SURFACE_MATERIAL_GV,
            ART_DEFAULT_SURFACE_MATERIAL,
            [ ALLOC_INIT_OBJECT(ArnLambertSurfaceMaterial)
                :   ARNGREY_050
                ]
            );
    else
        [ ART_DEFAULT_SURFACE_MATERIAL_GV reinitialiseAfterISRChange ];
)

ART_SPECTRUM_MODULE_SHUTDOWN_FUNCTION
(
    if ( ART_DEFAULT_SURFACE_MATERIAL_GV )
        [ ART_DEFAULT_SURFACE_MATERIAL_GV prepareForISRChange ];
)


ArNode <ArpSurfaceMaterial> * art_default_surface_material(
        ART_GV  * art_gv
        )
{
    return
        ART_DEFAULT_SURFACE_MATERIAL_GV;
}

// ===========================================================================
