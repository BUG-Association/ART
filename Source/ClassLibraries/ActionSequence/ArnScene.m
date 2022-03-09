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

#define ART_MODULE_NAME     ArnScene

#import "ArnScene.h"

#import "ART_Shape.h"

#import "ART_SurfaceMaterial.h"
#import "ART_VolumeMaterial.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnScene registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

#define WORLD_SUBNODE \
ARNARY_SUBNODE_I(1)

@implementation ArnScene

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnScene)

- (ArNode <ArpCamera> *) camera
{
    return (ArNode <ArpCamera> *) ARNARY_SUBNODE_I(0);
}

- (id) setCamera
        : (ArNode <ArpCamera> *) newCamera
{
    ASSIGN_AS_HARD_NODE_REFERENCE_TO_NARY_SUBNODE( 0, newCamera );
    return self;
}

- (ArNode <ArpWorld> *) world
{
    return (ArNode <ArpWorld> *) ARNARY_SUBNODE_I(1);
}

- (id) setWorld
        : (ArNode <ArpWorld> *) newWorld
{
    ASSIGN_AS_HARD_NODE_REFERENCE_TO_NARY_SUBNODE( 1, newWorld );
    return self;
}

- (ArNode <ArpActionSequence> *) actionSequence
{
    return
        (ArNode <ArpActionSequence> *) ARNARY_SUBNODE_I(2);
}

- (id) setActionSequence
        : (ArNode <ArpActionSequence> *) newActionSequence
{
    ASSIGN_AS_HARD_NODE_REFERENCE_TO_NARY_SUBNODE(
        2,
        newActionSequence
        );

    return self;
}

- (ArNode *) allocBBoxes
{
    ASSIGN_AS_HARD_NODE_REFERENCE_TO_NARY_SUBNODE(
        1,
        [ WORLD_SUBNODE allocBBoxes ]
        );

    return self;
}

- (ArNode *) removeBBoxes
{
    ASSIGN_AS_HARD_NODE_REFERENCE_TO_NARY_SUBNODE(
        1,
        [ WORLD_SUBNODE removeBBoxes ]
        );

    return self;
}

- (void) initBBoxes
        : (ArnGraphTraversal *) traversal
        : (Box3D *) outBBox
{
    [ WORLD_SUBNODE initBBoxes
        :   traversal
        :   outBBox
        ];
}

- (void) getBBoxObjectspace
        : (ArnGraphTraversal *) traversal
        : (Box3D *) outBBox
{
    [ WORLD_SUBNODE getBBoxObjectspace
        :   traversal
        :   outBBox
        ];
}

- (void) shrinkToFitWithinBox
        : (Box3D *) inBBox
{
    [ WORLD_SUBNODE shrinkToFitWithinBox
        :    inBBox
        ];
}

- (ArNode *) optimiseBBoxes
        : (ArnGraphTraversal *) traversal
{
    ASSIGN_AS_HARD_NODE_REFERENCE_TO_NARY_SUBNODE(
        1,
        [ WORLD_SUBNODE optimiseBBoxes
            :   traversal
            ]
        );

    return self;
}

- (void) enlargeBBoxes
        : (const Vec3D *) epsilonVec
{
    [ WORLD_SUBNODE enlargeBBoxes
        :   epsilonVec
        ];
}

- (ArNode *) removeTopmostBBox
{
    ASSIGN_AS_HARD_NODE_REFERENCE_TO_NARY_SUBNODE(
        1,
        [ WORLD_SUBNODE removeTopmostBBox ]
        );

    return self;
}

- (ArNode *) clipToBox
        : (Box3D *) inBox
{
    ArNode  * result = [ WORLD_SUBNODE clipToBox :inBox ];

    if (result)
    {
        ASSIGN_AS_HARD_NODE_REFERENCE_TO_NARY_SUBNODE( 1, result );
        return self;
    }
    else
        return 0;
}

@end

// ===========================================================================
