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

#define ART_MODULE_NAME     ArcPointLightsource

#import "ArcPointLightsource.h"
#import "ArnLightsourceCollector.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#define REPORTER  \
((ArNode <ArpReporter>  *)lightsourceCollector->reporter)
#define ACTIVE_TRAFO        ((ArNode <ArpTrafo3D> *)ARTS_TRAFO(*traversalState))
#define CURRENT_SURFACE     ((ArNode <ArpSurfaceMaterial> *)ARTS_SURFACE_MATERIAL(*traversalState))

@implementation ArcPointLightsource

- (id) init
        : (ArNode <ArpShape> *) shapeRef
        : (ArTraversalState *) traversalState
        : (ArnLightsourceCollector *) lightsourceCollector
{
    Pnt3D  centroid;

    self =
        [ super init
            :   arlightsourcetype_point
            ];
    
    if ( self )
    {
        [ REPORTER beginAction
            :   "Creating a point lightsource representation"
            ];

        shape = shapeRef;

        spectralPower = arspectralintensity_alloc( art_gv );

        [ CURRENT_SURFACE calculateTotalEmittedSpectralLightAtPoint
            :   0
            :   0
            :   spectralPower
            ];

        [ shape getLocalCentroid
            :   traversalState
            : & centroid
            ];

        if ( ACTIVE_TRAFO )
            [ ACTIVE_TRAFO transformPnt3D
                : & centroid
                : & position
                ];
        else
            position = centroid;

        radiantPower =
            arspectralintensity_i_norm_visible_range(
                  art_gv,
                  spectralPower
                );

        [ REPORTER printf
            :   "Visible range radiant power of point light: %f [W]\n"
            ,   radiantPower
            ];

        [ REPORTER endAction ];
    }
    
    return self;
}

@end

// ===========================================================================
