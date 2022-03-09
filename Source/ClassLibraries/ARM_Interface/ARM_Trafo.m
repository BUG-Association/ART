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

#define ART_MODULE_NAME     ARM_Trafo

#import "ARM_Trafo.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

ArnScale2D * arnscale2d(
        ART_GV   * art_gv,
        Scale2D    newScale
        )
{
    return
        [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnScale2D)
            :   newScale
            ];
}

ArnScale3D * arnscale3d(
        ART_GV   * art_gv,
        Scale3D    newScale
        )
{
    return
        [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnScale3D)
            :   newScale
            ];
}

ArnTranslation2D * arntranslation2d(
        ART_GV         * art_gv,
        Translation2D    newTranslation
        )
{
    return
        [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnTranslation2D)
            :   newTranslation
            ];
}

ArnTranslation3D * arntranslation3d(
        ART_GV         * art_gv,
        Translation3D    newTranslation
        )
{
    return
        [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnTranslation3D)
            :   newTranslation
            ];
}


ArnVarScale3D * arnvarscale3d(
        ART_GV                    * art_gv,
        ArNode <ArpDoubleValues>  * x_scale,
        ArNode <ArpDoubleValues>  * y_scale,
        ArNode <ArpDoubleValues>  * z_scale
        )
{
    return
        [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnVarScale3D)
            :   x_scale
            :   y_scale
            :   z_scale
            ];
}

ArnVarTranslation3D * arnvartranslation3d(
        ART_GV                    * art_gv,
        ArNode <ArpDoubleValues>  * x_translation,
        ArNode <ArpDoubleValues>  * y_translation,
        ArNode <ArpDoubleValues>  * z_translation
        )
{
    return
        [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnVarTranslation3D)
            :   x_translation
            :   y_translation
            :   z_translation
            ];
}

ArnRotX3D * arnrotx3d(
        ART_GV  * art_gv,
        double    alpha
        )
{
    return
        [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnRotX3D)
            :  alpha ];
}

ArnRotY3D * arnroty3d(
        ART_GV  * art_gv,
        double    alpha
        )
{
    return
        [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnRotY3D)
            :  alpha ];
}

ArnRotZ3D * arnrotz3d(
        ART_GV  * art_gv,
        double    alpha
        )
{
    return
        [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnRotZ3D)
            :  alpha ];
}

ArnRot3D * arnrot3d(
        ART_GV  * art_gv,
        Rot3D     newRot
        )
{
    return
        [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnRot3D)
            :  newRot ];
}

ArnVarRotX3D * arnvarrotx3d(
        ART_GV  * art_gv,
        ArNode  * alpha
        )
{
    return
        [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnVarRotX3D)
            :  alpha ];
}

ArnVarRotY3D * arnvarroty3d(
        ART_GV  * art_gv,
        ArNode  * alpha
        )
{
    return
        [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnVarRotY3D)
            :  alpha ];
}

ArnVarRotZ3D * arnvarrotz3d(
        ART_GV  * art_gv,
        ArNode  * alpha
        )
{
    return
        [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnVarRotZ3D)
            :  alpha ];
}

ArnShearXY3D * arnshearxy3d(
        ART_GV     * art_gv,
        ShearXY3D    newShear
        )
{
    return
        [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnShearXY3D)
            :   newShear
            ];
}

ArnShearYZ3D * arnshearyz3d(
        ART_GV     * art_gv,
        ShearYZ3D    newShear
        )
{
    return
        [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnShearYZ3D)
            :   newShear
            ];
}

ArnShearZX3D * arnshearzx3d(
        ART_GV     * art_gv,
        ShearZX3D    newShear
        )
{
    return
        [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnShearZX3D)
            :   newShear
            ];
}

ArNode * arntrafopair(
        ART_GV  * art_gv,
        ArNode  * node0,
        ArNode  * node1
        )
{
    if (   [ node0 conformsToArProtocol: ARPROTOCOL(ArpTrafo2D) ]
        && [ node1 conformsToArProtocol: ARPROTOCOL(ArpTrafo2D) ] )
        return
            arntrafo2dpair(
                art_gv,
                (ArNode <ArpTrafo2D> *) node0,
                (ArNode <ArpTrafo2D> *) node1
                );

    if (   [ node0 conformsToArProtocol: ARPROTOCOL(ArpTrafo3D) ]
        && [ node1 conformsToArProtocol: ARPROTOCOL(ArpTrafo3D) ] )
        return
            arntrafo3dpair(
                art_gv,
                (ArNode <ArpTrafo3D> *) node0,
                (ArNode <ArpTrafo3D> *) node1
                );

    ART_ERRORHANDLING_FATAL_ERROR(
        "required transformation protocols not supported: %s / %s"
        ,  [ node0 cStringClassName ]
        ,  [ node1 cStringClassName ]
        );

    return 0;
}

ArnTrafo3DPair * arntrafo3dpair(
        ART_GV               * art_gv,
        ArNode <ArpTrafo3D>  * node0,
        ArNode <ArpTrafo3D>  * node1
        )
{
    return
        [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnTrafo3DPair)
            :   HARD_NODE_REFERENCE(node0)
            :   HARD_NODE_REFERENCE(node1)
            ];
}

ArnTrafo2DPair * arntrafo2dpair(
        ART_GV               * art_gv,
        ArNode <ArpTrafo2D>  * node0,
        ArNode <ArpTrafo2D>  * node1
        )
{
    return
            [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnTrafo2DPair)
                :   HARD_NODE_REFERENCE(node0)
                :   HARD_NODE_REFERENCE(node1)
                ];
}

// ===========================================================================
