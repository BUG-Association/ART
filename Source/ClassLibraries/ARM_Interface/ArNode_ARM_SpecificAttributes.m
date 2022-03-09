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

#define ART_MODULE_NAME     ArNode_ARM_SpecificAttributes

#import "ARM_Trafo.h"
#import "ArNode_ARM_SpecificAttributes.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArNode ( ARM_SpecificAttributes )

- (id) surfaceMaterial
        : (ArNode *) newSurfaceMaterial
{
    if ( [ newSurfaceMaterial conformsToArProtocol
            : ARPROTOCOL(ArpSurfaceMaterial) ] )
    {
        id  result =
            [ ALLOC_INIT_OBJECT_AUTORELEASE(AraSurfaceMaterial)
                :   HARD_NODE_REFERENCE(self)
                :   HARD_NODE_REFERENCE(newSurfaceMaterial)
                ];

        return result;
    }

    ART_ERRORHANDLING_FATAL_ERROR(
        "surface material attribute node does not support the required "
        "ArpSurfaceMaterial protocol"
        );

    return 0;
}

- (id) environmentMaterial
        : (ArNode *) newEnvironmentMaterial
{
    if ( [ newEnvironmentMaterial conformsToArProtocol
            : ARPROTOCOL(ArpEnvironmentMaterial) ] )
    {
        id  result =
            [ ALLOC_INIT_OBJECT_AUTORELEASE(AraEnvironmentMaterial)
                :   HARD_NODE_REFERENCE(self)
                :   HARD_NODE_REFERENCE(newEnvironmentMaterial)
                ];

        return result;
    }

    ART_ERRORHANDLING_FATAL_ERROR(
        "environment material attribute node does not support the required "
        "ArpEnvironmentMaterial protocol"
        );

    return 0;
}

- (id) volumeMaterial
        : (ArNode *) newVolumeMaterial
{
    if ( [ newVolumeMaterial conformsToArProtocol
            : ARPROTOCOL(ArpVolumeMaterial) ] )
    {
        id  result =
            [ ALLOC_INIT_OBJECT_AUTORELEASE(AraVolumeMaterial)
                :   HARD_NODE_REFERENCE(self)
                :   HARD_NODE_REFERENCE(newVolumeMaterial)
                ];

        return result;
    }

    ART_ERRORHANDLING_FATAL_ERROR(
        "volume material attribute node does not support the required "
        "ArpVolumeMaterial protocol"
        );

    return 0;
}

- (id) vertices
        : (ArNode *) newSetOfVertices
{
    if ( [ newSetOfVertices conformsToArProtocol: ARPROTOCOL(ArpVertices) ] )
    {
        id  result =
            [ ALLOC_INIT_OBJECT_AUTORELEASE(AraVertices)
                :   HARD_NODE_REFERENCE(self)
                :   HARD_NODE_REFERENCE(newSetOfVertices)
                ];

        return result;
    }

    ART_ERRORHANDLING_FATAL_ERROR(
        "vertex set attribute node does not support the required "
        "ArpVertices protocol"
        );

    return 0;
}

- (id) trafo
        : (ArNode *) newTrafo
{
    if ( [ newTrafo conformsToArProtocol: ARPROTOCOL(ArpTrafo2D) ] )
    {
        id  result =
            [ ALLOC_INIT_OBJECT_AUTORELEASE(AraTrafo2D)
                :   HARD_NODE_REFERENCE(self)
                :   HARD_NODE_REFERENCE(newTrafo)
                ];

        return result;
    }

    if ( [ newTrafo conformsToArProtocol: ARPROTOCOL(ArpTrafo3D) ] )
    {
        id  result =
            [ ALLOC_INIT_OBJECT_AUTORELEASE(AraTrafo3D)
                :   HARD_NODE_REFERENCE(self)
                :   HARD_NODE_REFERENCE(newTrafo)
                ];

        return result;
    }

    if ( [ newTrafo conformsToArProtocol: ARPROTOCOL(ArpVarTrafo3D) ] )
    {
        id  result =
            [ ALLOC_INIT_OBJECT_AUTORELEASE(AraVarTrafo3D)
                :   HARD_NODE_REFERENCE(self)
                :   HARD_NODE_REFERENCE(newTrafo)
                ];

        return result;
    }

    ART_ERRORHANDLING_FATAL_ERROR(
        "required transformation protocol not supported"
        );

    return 0;
}

- (id) trafo
        : (ArNode *) newTrafo0
        : (ArNode *) newTrafo1
{
    return
        [ self trafo
            :   arntrafopair(
                    art_gv,
                    newTrafo0,
                    newTrafo1
                    )
            ];
}

- (id) trafo
        : (ArNode *) newTrafo0
        : (ArNode *) newTrafo1
        : (ArNode *) newTrafo2
{
    return
        [ self trafo
            :   arntrafopair(
                    art_gv,
                    arntrafopair(
                        art_gv,
                        newTrafo0,
                        newTrafo1
                        ),
                    newTrafo2
                    )
            ];
}

- (id) trafo
        : (ArNode *) newTrafo0
        : (ArNode *) newTrafo1
        : (ArNode *) newTrafo2
        : (ArNode *) newTrafo3
{
    return
        [ self trafo
            :   arntrafopair(
                    art_gv,
                    arntrafopair(
                        art_gv,
                        newTrafo0,
                        newTrafo1
                        ),
                    arntrafopair(
                        art_gv,
                        newTrafo2,
                        newTrafo3
                        )
                    )
            ];
}

- (id) trafo
        : (ArNode *) newTrafo0
        : (ArNode *) newTrafo1
        : (ArNode *) newTrafo2
        : (ArNode *) newTrafo3
        : (ArNode *) newTrafo4
{
    return
        [ self trafo
            :   arntrafopair(
                    art_gv,
                    arntrafopair(
                        art_gv,
                        arntrafopair(
                            art_gv,
                            newTrafo0,
                            newTrafo1
                            ),
                        arntrafopair(
                            art_gv,
                            newTrafo2,
                            newTrafo3
                            )
                        ),
                    newTrafo4
                    )
            ];
}

- (id) trafo
        : (ArNode *) newTrafo0
        : (ArNode *) newTrafo1
        : (ArNode *) newTrafo2
        : (ArNode *) newTrafo3
        : (ArNode *) newTrafo4
        : (ArNode *) newTrafo5
{
    return
        [ self trafo
            :   arntrafopair(
                    art_gv,
                    arntrafopair(
                        art_gv,
                        arntrafopair(
                            art_gv,
                            newTrafo0,
                            newTrafo1
                            ),
                        arntrafopair(
                            art_gv,
                            newTrafo2,
                            newTrafo3
                            )
                        ),
                    arntrafopair(
                        art_gv,
                        newTrafo4,
                        newTrafo5
                        )

                    )
            ];
}

- (id) trafo
        : (ArNode *) newTrafo0
        : (ArNode *) newTrafo1
        : (ArNode *) newTrafo2
        : (ArNode *) newTrafo3
        : (ArNode *) newTrafo4
        : (ArNode *) newTrafo5
        : (ArNode *) newTrafo6
{
    return
        [ self trafo
            :   arntrafopair(
                    art_gv,
                    arntrafopair(
                        art_gv,
                        arntrafopair(
                            art_gv,
                            newTrafo0,
                            newTrafo1
                            ),
                        arntrafopair(
                            art_gv,
                            newTrafo2,
                            newTrafo3
                            )
                        ),
                    arntrafopair(
                        art_gv,
                        arntrafopair(
                            art_gv,
                            newTrafo4,
                            newTrafo5
                            ),
                        newTrafo6
                        )

                    )
            ];
}

- (id) trafo
        : (ArNode *) newTrafo0
        : (ArNode *) newTrafo1
        : (ArNode *) newTrafo2
        : (ArNode *) newTrafo3
        : (ArNode *) newTrafo4
        : (ArNode *) newTrafo5
        : (ArNode *) newTrafo6
        : (ArNode *) newTrafo7
{
    return
        [ self trafo
            :   arntrafopair(
                    art_gv,
                    arntrafopair(
                        art_gv,
                        arntrafopair(
                            art_gv,
                            newTrafo0,
                            newTrafo1
                            ),
                        arntrafopair(
                            art_gv,
                            newTrafo2,
                            newTrafo3
                            )
                        ),
                    arntrafopair(
                        art_gv,
                        arntrafopair(
                            art_gv,
                            newTrafo4,
                            newTrafo5
                            ),
                        arntrafopair(
                            art_gv,
                            newTrafo6,
                            newTrafo7
                            )
                        )

                    )
            ];
}

@end

// ===========================================================================
