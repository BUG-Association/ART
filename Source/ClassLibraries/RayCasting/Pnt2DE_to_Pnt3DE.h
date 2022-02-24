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

#include "ART_Foundation.h"

ART_MODULE_INTERFACE(Pnt2DE_to_Pnt3DE)

#import "ART_Scenegraph.h"

#import "ArnRayCaster.h"

/* ---------------------------------------------------------------------------

    'pnt2de_to_world_pnt3de'

    Transforms a surface point on a given shape from 2D parameterisation
    space on a shape - i.e. (u,v) coordinates on a given patch - to a point
    in 3D world coordinates, complete with face index and surface normal.

    No testing of the validity of the resulting 3D point is performed, so the
    returned point might or might not be removed by CSG operations in the
    actual scene.

------------------------------------------------------------------------aw- */

void pnt2de_to_world_pnt3de(
        const Pnt2DE               * inPoint2D,
              ArTraversalState     * traversalState,
              ArNode <ArpShape>    * shapeThePointLiesOn,
              ArNode <ArpMapping>  * shapeParameterisation,
              Pnt3DE               * outPoint3D
        );


/* ---------------------------------------------------------------------------

    'pnt2de_to_tested_world_pnt3de'

    Transforms a 2D surface point to 3D world coordinates in exactly the
    same way as 'pnt2de_to_world_pnt3de'.

    This function also performs a check whether the resulting point is
    actually valid, in the sense that is not removed by a CSG operation. This
    is done by traversing the entire scene graph from the top, and evaluating
    all CSG operations that might affect the point in question.

    This after-the-fact testing in object space is necessary since there
    is no way to identify points that are removed by CSG SUB and AND
    operations in 2D parameter space.

------------------------------------------------------------------------aw- */

void pnt2de_to_tested_world_pnt3de(
        const Pnt2DE               * inPoint2D,
              ArTraversalState     * traversalState,
              ArNode <ArpShape>    * shapeThePointLiesOn,
              ArNode <ArpMapping>  * shapeParameterisation,
              ArnRayCaster         * rayCaster,
              Pnt3DE               * outPoint3D,
              BOOL                 * outValid
        );

// ===========================================================================
