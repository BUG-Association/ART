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
/**
 * @file ARM_ActionSequence.h
 * @brief Scene
 * @type Scene
 */

#import "ARM_Foundation.h"

ART_MODULE_INTERFACE(ARM_ActionSequence)

#import "ART_ActionSequence.h"
#import "ARM_Scenegraph.h"


/**
 * @brief Scene node
 * Creates a scene
 */
#define SCENE           ALLOC_OBJECT_AUTORELEASE(ArnScene)

@interface ArnScene ( ARM_Interface )

//   old interface, deprecated, will be removed

- (id) object
        :               (ArNode *) sceneGeometry
        camera:         (ArNode <ArpCamera> *) newCamera
        actionSequence: (ArNode <ArpActionSequence> *) newActionSequence
        ;

- (id) object
        :               (ArNode *) sceneGeometry
        skylight:       (ArNode <ArpSkyModel> *) newSkyModel
        camera:         (ArNode <ArpCamera> *) newCamera
        actionSequence: (ArNode <ArpActionSequence> *) newActionSequence
        ;

//   new interface

/**
 * @def [ SCENE sceneGeometry: camera: actionSequence: ]
 * @param sceneGeometry     Shape           Geometry contained in the scene.
 * @param camera            Camera          Camera in use for rendering the scene.
 * @param actionSequence    ActionSequence  The action sequence that need to be executed for rendering the scene.
 */
- (id) sceneGeometry
        :               (ArNode *) sceneGeometry
        camera:         (ArNode <ArpCamera> *) newCamera
        actionSequence: (ArNode <ArpActionSequence> *) newActionSequence
        ;

/**
 * @def [ SCENE sceneGeometry: skyModel: camera: actionSequence: ]
 * @param sceneGeometry     Shape           Geometry contained in the scene.
 * @param skyModel          SkyModel        Skymodel used for illuminating the scene.
 * @param camera            Camera          Camera in use for rendering the scene.
 * @param actionSequence    ActionSequence  The action sequence that need to be executed for rendering the scene.
 */
- (id) sceneGeometry
        :               (ArNode *) sceneGeometry
        skyModel:       (ArNode <ArpSkyModel> *) newSkyModel
        camera:         (ArNode <ArpCamera> *) newCamera
        actionSequence: (ArNode <ArpActionSequence> *) newActionSequence
        ;

//   Short variable names to avoid line breaks
/**
 * @def [ SCENE sceneGeometry: skyModel: camera: actionSequence: ]
 * @param sceneGeometry         Shape               Geometry contained in the scene.
 * @param environmentMaterial   EnvironmentMaterial Environment material to use.
 * @param camera                Camera              Camera in use for rendering the scene.
 * @param actionSequence        ActionSequence      The action sequence that need to be executed for rendering the scene.
 */
- (id) sceneGeometry
        :                    (ArNode *) sceneGeometry
        environmentMaterial: (ArNode <ArpEnvironmentMaterial> *) e
        camera:              (ArNode <ArpCamera> *) c
        actionSequence:      (ArNode <ArpActionSequence> *) a
        ;

//   This is the full interface to which all others refer
/**
 * @def [ SCENE
 * @fdef    sceneGeometry: environmentMaterial: worldVolumeMaterial
 * @fdef    camera: actionSequence: ]
 * @param sceneGeometry         Shape               Geometry contained in the scene.
 * @param environmentMaterial   EnvironmentMaterial Environment material to use.
 * @param worldVolumeMaterial   VolumeMaterial      Volume material to use to fill in the scene.
 * @param camera                Camera              Camera in use for rendering the scene.
 * @param actionSequence        ActionSequence      The action sequence that need to be executed for rendering the scene.
 */
- (id) sceneGeometry
        :                    (ArNode *) sceneGeometry
        environmentMaterial: (ArNode <ArpEnvironmentMaterial> *) e
        worldVolumeMaterial: (ArNode <ArpVolumeMaterial> *) v
        camera:              (ArNode <ArpCamera> *) c
        actionSequence:      (ArNode <ArpActionSequence> *) a
        ;

@end



#define NOP_ACTION      NOP_ACTION_SINGLETON

ArnActionSequence  * arnactionsequence(
        ART_GV              * art_gv,
        ArNode <ArpAction>  * firstAction,
        ...
        );

ArnActionSequence  * arnactionsequence_no_autorelease(
        ART_GV              * art_gv,
        ArNode <ArpAction>  * firstAction,
        ...
        );

ArnActionSequence  * arnactionsequence_message(
              ART_GV              * art_gv,
        const char                * newMessage,
              ArNode <ArpAction>  * firstAction,
              ...
        );

/**
 * @section Action Sequences
 * @type ActionSequence
 */

/**
 * @brief Action Sequence
 *
 * Creates an action sequence based on the sequence of action provided.
 * It needs to be ended by \verb?ACTION_SEQUENCE_END?.
 *
 * @def ACTION_SEQUENCE(action, ..., ACTION_SEQUENCE_END)
 *
 * @param action                Action    Action that need to be performed
 * @param ...                   Action    List of actions that need to be performed
 * @param ACTION_SEQUENCE_END   -         Mandatory termination marker for the action sequence
 */
#define ACTION_SEQUENCE(action...) \
    arnactionsequence( art_gv, ## action )

/**
 * @brief Named action Sequence
 *
 * Creates an action sequence based on the sequence of action provided.
 * It needs to be ended by \verb?ACTION_SEQUENCE_END?.
 *
 * @def NAMED_ACTION_SEQUENCE(message, action, ..., ACTION_SEQUENCE_END)
 *
 * @param message               char*     Message to display before executing the sequence
 * @param action                Action    Action that need to be performed
 * @param ...                   Action    List of actions that need to be performed
 * @param ACTION_SEQUENCE_END   -         Mandatory termination marker for the action sequence */
#define NAMED_ACTION_SEQUENCE(message,action...) \
    arnactionsequence_message( art_gv, message, ## action )


#define ACTION_SEQUENCE_END     NULL


#define ARNNODESTACK(__masterFilename) \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnNodeStack) \
        : (__masterFilename) \
        ]

// ===========================================================================
