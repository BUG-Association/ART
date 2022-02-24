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

#define ART_MODULE_NAME     ARM_ActionSequence

#import "ARM_ActionSequence.h"

#import "ART_SurfaceMaterial.h"
#import "ART_VolumeMaterial.h"
#import "ARM_Shape.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

@implementation ArnScene ( ARM_Interface )

- (id) object
        :               (ArNode *) newWorld
        camera:         (ArNode <ArpCamera> *) newCamera
        actionSequence: (ArNode <ArpActionSequence> *) newActionSequence
{
    ART_ERRORHANDLING_WARNING(
        "\nDeprecated ARM object constructor method: \n\n"
        "[ ArnScene object: camera: actionSequence: ]\n\n"
        "will be removed in future ART releases. Use\n\n"
        "[ ArnScene sceneGeometry: camera: actionSequence: ]\n\n"
        "instead.\n"
        );

    return
        [ self
            sceneGeometry:   newWorld
            camera:          newCamera
            actionSequence:  newActionSequence
            ];
}

- (id) object
        :               (ArNode *) newWorld
        skylight:       (ArNode <ArpSkyModel> *) newSkyModel
        camera:         (ArNode <ArpCamera> *) newCamera
        actionSequence: (ArNode <ArpActionSequence> *) newActionSequence
{
    ART_ERRORHANDLING_WARNING(
        "\nDeprecated ARM object constructor method: \n\n"
        "[ ArnScene object: skylight: camera: actionSequence: ]\n\n"
        "will be removed in future ART releases. Use\n\n"
        "[ ArnScene sceneGeometry: skyModel: camera: actionSequence: ]\n\n"
        "instead.\n"
        );

    return
        [ self
            sceneGeometry:   newWorld
            skyModel:        newSkyModel
            camera:          newCamera
            actionSequence:  newActionSequence
            ];
}

- (id) sceneGeometry
        :               (ArNode *) sceneGeometry
        camera:         (ArNode <ArpCamera> *) newCamera
        actionSequence: (ArNode <ArpActionSequence> *) newActionSequence
{
    return
        [ self
            sceneGeometry:       sceneGeometry
            environmentMaterial: NULL
            worldVolumeMaterial: ART_DEFAULT_VOLUME_MATERIAL_AIR
            camera:              newCamera
            actionSequence:      newActionSequence
            ];
}

- (id) sceneGeometry
        :               (ArNode *) sceneGeometry
        skyModel:       (ArNode <ArpSkyModel> *) newSkyModel
        camera:         (ArNode <ArpCamera> *) newCamera
        actionSequence: (ArNode <ArpActionSequence> *) newActionSequence
{
    ArNode <ArpVolumeMaterial>  * atmosphericMaterial = NULL;

    if ( [ newSkyModel servesAsVolumeMaterial ] )
        atmosphericMaterial = (ArNode <ArpVolumeMaterial> *) newSkyModel;
    else
        atmosphericMaterial = ART_DEFAULT_VOLUME_MATERIAL_AIR;

    return
        [ self
            sceneGeometry:       sceneGeometry
            environmentMaterial: (ArNode <ArpEnvironmentMaterial> *) newSkyModel
            worldVolumeMaterial: atmosphericMaterial
            camera:              newCamera
            actionSequence:      newActionSequence
            ];
}

- (id) sceneGeometry
        :                    (ArNode *) sceneGeometry
        environmentMaterial: (ArNode <ArpEnvironmentMaterial> *) newEnvironmentMaterial
        camera:              (ArNode <ArpCamera> *) newCamera
        actionSequence:      (ArNode <ArpActionSequence> *) newActionSequence
{
    return
        [ self
            sceneGeometry:       sceneGeometry
            environmentMaterial: newEnvironmentMaterial
            worldVolumeMaterial: ART_DEFAULT_VOLUME_MATERIAL_AIR
            camera:              newCamera
            actionSequence:      newActionSequence
            ];
}

- (id) sceneGeometry
        :                    (ArNode *) sceneGeometry
        environmentMaterial: (ArNode <ArpEnvironmentMaterial> *) newEnvironmentMaterial
        worldVolumeMaterial: (ArNode <ArpVolumeMaterial> *) newWorldVolumeMaterial
        camera:              (ArNode <ArpCamera> *) newCamera
        actionSequence:      (ArNode <ArpActionSequence> *) newActionSequence
{
    if ( [ sceneGeometry isMemberOfClass: [ AraWorld class ] ] )
        ART_ERRORHANDLING_FATAL_ERROR(
            "AraWorld instances should not be directly constructed in an "
            "ARM file."
            );

    if ( newEnvironmentMaterial )
        sceneGeometry =
            [ sceneGeometry or
                :   ARNINFSPHERE_SINGLETON
                ];


    AraWorld  * newWorld =
        [ ALLOC_INIT_OBJECT_AUTORELEASE(AraWorld)
            :   HARD_NODE_REFERENCE(sceneGeometry)
            :   HARD_NODE_REFERENCE(newWorldVolumeMaterial)
            :   HARD_NODE_REFERENCE(ART_DEFAULT_VOLUME_MATERIAL_OBJECT)
            :   HARD_NODE_REFERENCE(ART_DEFAULT_SURFACE_MATERIAL)
            :   HARD_NODE_REFERENCE(newEnvironmentMaterial)
            ];


    ArNodeRefDynArray  dynarray =
        arnoderefdynarray(
            3,
            HARD_NODE_REFERENCE(newCamera),
            HARD_NODE_REFERENCE(newWorld),
            HARD_NODE_REFERENCE(newActionSequence)
            );

    [ self init
        : & dynarray
        ];

    arnoderefdynarray_free_contents( & dynarray );

    return self;
}

@end

ArnActionSequence  * arnactionsequence(
        ART_GV              * art_gv,
        ArNode <ArpAction>  * firstAction,
        ...
        )
{
    ArNodeRefDynArray  actions;
    
    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK( firstAction, ArpAction );
    
    ARNODEREF_DYNARRAY_FROM_HARD_PTR_VA_ARGS( actions, (ArNode *)firstAction );

    ArnActionSequence  * actionsequence =
        [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnActionSequence)
            : & actions
            :   0
            ];

    arnoderefdynarray_free_contents( & actions );

    return actionsequence;
}

ArnActionSequence  * arnactionsequence_no_autorelease(
        ART_GV              * art_gv,
        ArNode <ArpAction>  * firstAction,
        ...
        )
{
    ArNodeRefDynArray  actions;
    
    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK( firstAction, ArpAction );
    
    ARNODEREF_DYNARRAY_FROM_HARD_PTR_VA_ARGS( actions, (ArNode *)firstAction );

    ArnActionSequence  * actionsequence =
        [ ALLOC_INIT_OBJECT(ArnActionSequence)
            : & actions
            :   0
            ];

    arnoderefdynarray_free_contents( & actions );

    return actionsequence;
}

ArnActionSequence  * arnactionsequence_message(
              ART_GV              * art_gv,
        const char                * newMessage,
              ArNode <ArpAction>  * firstAction,
              ...
        )
{
    ArNodeRefDynArray  actions;
    
    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK( firstAction, ArpAction );
    
    ARNODEREF_DYNARRAY_FROM_HARD_PTR_VA_ARGS( actions, (ArNode *)firstAction );

    ArnActionSequence  * actionsequence =
        [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnActionSequence)
            : & actions
            :   arsymbol( art_gv, newMessage )
            ];

    arnoderefdynarray_free_contents( & actions );

    return actionsequence;
}


// ===========================================================================
