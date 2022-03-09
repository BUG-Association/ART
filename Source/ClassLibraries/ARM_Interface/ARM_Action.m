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

#define ART_MODULE_NAME     ARM_Action

#import "ARM_Action.h"

#import "ARM_RayCasting.h"
#import "ARM_ActionSequence.h"
#import "ARM_Action.h"

typedef struct ARM_Actions_GV
{
    ArNode <ArpAction>      * scenegraph_bounding_box_insertion;
    ArNode <ArpAction>      * scenegraph_raycasting_optimisations;
}
ARM_Actions_GV;

#define SCENEGRAPH_INSERT_BOUNDING_BOXES_GV \
    art_gv->ar2m_actions_gv->scenegraph_bounding_box_insertion
#define CREATE_STANDARD_RAYCASTING_ACCELERATION_STRUCTURE_GV \
    art_gv->ar2m_actions_gv->scenegraph_raycasting_optimisations

typedef struct ARM_ScenegraphActions_GV
{
    ArNode <ArpAction>  * nop_action_singleton;
    ArNode <ArpAction>  * remove_externals_action_singleton;
    ArNode <ArpAction>  * read_extra_data_action_singleton;
    ArNode <ArpAction>  * setup_node_data_action_singleton;
    ArNode <ArpAction>  * convert_to_tree_action_singleton;
    ArNode <ArpAction>  * combine_attributes_action_singleton;
    ArNode <ArpAction>  * print_csg_tree_action_singleton;
    ArNode <ArpAction>  * alloc_bboxes_action_singleton;
    ArNode <ArpAction>  * init_bboxes_action_singleton;
    ArNode <ArpAction>  * shrink_bboxes_action_singleton;
    ArNode <ArpAction>  * optimise_bboxes_action_singleton;
}
ARM_ScenegraphActions_GV;

#define NOP_ACTION_SINGLETON_GV \
    art_gv->ar2m_scenegraphactions_gv->nop_action_singleton
#define SCENEGRAPH_REMOVE_EXTERNALS_GV \
    art_gv->ar2m_scenegraphactions_gv->remove_externals_action_singleton
#define SCENEGRAPH_READ_EXTRA_DATA_GV \
    art_gv->ar2m_scenegraphactions_gv->read_extra_data_action_singleton
#define SCENEGRAPH_SETUP_DATA_GV \
    art_gv->ar2m_scenegraphactions_gv->setup_node_data_action_singleton
#define SCENEGRAPH_CREATE_FLATTENED_COPY_GV \
    art_gv->ar2m_scenegraphactions_gv->convert_to_tree_action_singleton
#define SCENEGRAPH_COMBINE_ATTRIBUTES_GV \
    art_gv->ar2m_scenegraphactions_gv->combine_attributes_action_singleton
#define SCENEGRAPH_PRINT_CSG_TREE_GV \
    art_gv->ar2m_scenegraphactions_gv->print_csg_tree_action_singleton
#define SCENEGRAPH_ALLOC_BBOXES_GV \
    art_gv->ar2m_scenegraphactions_gv->alloc_bboxes_action_singleton
#define SCENEGRAPH_INIT_BBOXES_GV \
    art_gv->ar2m_scenegraphactions_gv->init_bboxes_action_singleton
#define SCENEGRAPH_SHRINK_BBOXES_GV \
    art_gv->ar2m_scenegraphactions_gv->shrink_bboxes_action_singleton
#define SCENEGRAPH_OPTIMISE_BBOXES_GV \
    art_gv->ar2m_scenegraphactions_gv->optimise_bboxes_action_singleton

ART_MODULE_INITIALISATION_FUNCTION
(
    ARM_Actions_GV  * ar2m_objects_gv;

    ar2m_objects_gv = ALLOC(ARM_Actions_GV);

    art_gv->ar2m_actions_gv = ar2m_objects_gv;

    SCENEGRAPH_INSERT_BOUNDING_BOXES_GV = 0;
    CREATE_STANDARD_RAYCASTING_ACCELERATION_STRUCTURE_GV = 0;

    ARNODE_SINGLETON_CREATOR(SCENEGRAPH_INSERT_BOUNDING_BOXES);
    ARNODE_SINGLETON_CREATOR(CREATE_STANDARD_RAYCASTING_ACCELERATION_STRUCTURE);
 
    // Scene graph actions initialisation
    ARM_ScenegraphActions_GV  * ar2m_scenegraphactions_gv;

    ar2m_scenegraphactions_gv = ALLOC(ARM_ScenegraphActions_GV);

    art_gv->ar2m_scenegraphactions_gv = ar2m_scenegraphactions_gv;

    NOP_ACTION_SINGLETON_GV = 0;
    SCENEGRAPH_REMOVE_EXTERNALS_GV = 0;
    SCENEGRAPH_READ_EXTRA_DATA_GV = 0;
    SCENEGRAPH_SETUP_DATA_GV = 0;
    SCENEGRAPH_CREATE_FLATTENED_COPY_GV = 0;
    SCENEGRAPH_COMBINE_ATTRIBUTES_GV = 0;
    SCENEGRAPH_PRINT_CSG_TREE_GV = 0;
    SCENEGRAPH_ALLOC_BBOXES_GV = 0;
    SCENEGRAPH_INIT_BBOXES_GV = 0;
    SCENEGRAPH_SHRINK_BBOXES_GV = 0;
    SCENEGRAPH_OPTIMISE_BBOXES_GV = 0;
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    if ( SCENEGRAPH_INSERT_BOUNDING_BOXES_GV )
        RELEASE_OBJECT( SCENEGRAPH_INSERT_BOUNDING_BOXES_GV );
    if ( CREATE_STANDARD_RAYCASTING_ACCELERATION_STRUCTURE_GV )
        RELEASE_OBJECT( CREATE_STANDARD_RAYCASTING_ACCELERATION_STRUCTURE_GV );

    FREE( art_gv->ar2m_actions_gv );
 
    // Scene graph actions release
    if ( NOP_ACTION_SINGLETON_GV )
        RELEASE_OBJECT( NOP_ACTION_SINGLETON_GV );
    if ( SCENEGRAPH_REMOVE_EXTERNALS_GV )
        RELEASE_OBJECT( SCENEGRAPH_REMOVE_EXTERNALS_GV );
    if ( SCENEGRAPH_READ_EXTRA_DATA_GV )
        RELEASE_OBJECT( SCENEGRAPH_READ_EXTRA_DATA_GV );
    if ( SCENEGRAPH_SETUP_DATA_GV )
        RELEASE_OBJECT( SCENEGRAPH_SETUP_DATA_GV );
    if ( SCENEGRAPH_CREATE_FLATTENED_COPY_GV )
        RELEASE_OBJECT( SCENEGRAPH_CREATE_FLATTENED_COPY_GV );
    if ( SCENEGRAPH_COMBINE_ATTRIBUTES_GV )
        RELEASE_OBJECT( SCENEGRAPH_COMBINE_ATTRIBUTES_GV );
    if ( SCENEGRAPH_PRINT_CSG_TREE_GV )
        RELEASE_OBJECT( SCENEGRAPH_PRINT_CSG_TREE_GV );
    if ( SCENEGRAPH_ALLOC_BBOXES_GV )
        RELEASE_OBJECT( SCENEGRAPH_ALLOC_BBOXES_GV );
    if ( SCENEGRAPH_INIT_BBOXES_GV )
        RELEASE_OBJECT( SCENEGRAPH_INIT_BBOXES_GV );
    if ( SCENEGRAPH_SHRINK_BBOXES_GV )
        RELEASE_OBJECT( SCENEGRAPH_SHRINK_BBOXES_GV );
    if ( SCENEGRAPH_OPTIMISE_BBOXES_GV )
        RELEASE_OBJECT( SCENEGRAPH_OPTIMISE_BBOXES_GV );

    FREE( art_gv->ar2m_scenegraphactions_gv );
)

#define SCENEGRAPH_ENLARGE_BBOXES(_eps) \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnEnlargeBBoxesAction) \
        :   (_eps) \
        ]

ArNode <ArpAction> * scenegraph_bounding_box_insertion(
        ART_GV  * art_gv
        )
{
    if ( ! SCENEGRAPH_INSERT_BOUNDING_BOXES_GV )
    {
        id  result =
            arnactionsequence_message(
                art_gv,
                "bounding box creation",
                SCENEGRAPH_ALLOC_BBOXES,
                SCENEGRAPH_INIT_BBOXES,
                SCENEGRAPH_SHRINK_BBOXES,
                SCENEGRAPH_ENLARGE_BBOXES(0.0001),
                SCENEGRAPH_OPTIMISE_BBOXES,
                ACTION_SEQUENCE_END
                );

        ARNODE_SINGLETON(
            SCENEGRAPH_INSERT_BOUNDING_BOXES_GV,
            SCENEGRAPH_INSERT_BOUNDING_BOXES,
            RETAIN_OBJECT(result)
            );
    }

    return
        SCENEGRAPH_INSERT_BOUNDING_BOXES_GV;
}

#define WITH_NEW_RSA

ArNode <ArpAction> * scenegraph_raycasting_optimisations_create(
        ART_GV  * art_gv
        )
{
    return
        arnactionsequence_message(
            art_gv,
            "optimising scene graph for raycasting",

            SCENEGRAPH_REMOVE_EXTERNALS,

            SCENEGRAPH_READ_EXTRA_DATA,

            SCENEGRAPH_SETUP_DATA,

            SCENEGRAPH_CREATE_FLATTENED_COPY,

            SCENEGRAPH_COMBINE_ATTRIBUTES,

//            SCENEGRAPH_PRINT_CSG_TREE,

            SCENEGRAPH_INSERT_BOUNDING_BOXES,
#ifdef WITH_NEW_RSA

            [ ALLOC_INIT_OBJECT(ArnCollectLeafNodeBBoxesAction) ],

            [ ALLOC_INIT_OBJECT(ArnCreateBSPTreeAction) ],
#endif
            ACTION_SEQUENCE_END
            );
}

//   insert this into an action sequence to output the graph
//          [ ALLOC_INIT_OBJECT(ArnSaveScenegraphAction)
//              :   "debug_scenegraph.art"
//              ],

ArNode <ArpAction> * scenegraph_raycasting_optimisations(
        ART_GV  * art_gv
        )
{
    if ( ! CREATE_STANDARD_RAYCASTING_ACCELERATION_STRUCTURE_GV )
    {
        id  scenegraph_opts =
            scenegraph_raycasting_optimisations_create(
                art_gv
                );

        ARNODE_SINGLETON(
            CREATE_STANDARD_RAYCASTING_ACCELERATION_STRUCTURE_GV,
            CREATE_STANDARD_RAYCASTING_ACCELERATION_STRUCTURE,
            RETAIN_OBJECT(scenegraph_opts)
            );
    }

    return
        CREATE_STANDARD_RAYCASTING_ACCELERATION_STRUCTURE_GV;
}


ArNode <ArpAction> * nop_action_singleton(
        ART_GV  * art_gv
        )
{
    if ( ! NOP_ACTION_SINGLETON_GV )
        ARNODE_SINGLETON(
            NOP_ACTION_SINGLETON_GV,
            NOP_ACTION_SINGLETON,
            [ ALLOC_INIT_OBJECT(ArnNOPAction) ]
            );

    return NOP_ACTION_SINGLETON_GV;
}

ArNode <ArpAction> * remove_externals_action_singleton(
        ART_GV  * art_gv
        )
{
    if ( ! SCENEGRAPH_REMOVE_EXTERNALS_GV )
        ARNODE_SINGLETON(
            SCENEGRAPH_REMOVE_EXTERNALS_GV,
            SCENEGRAPH_REMOVE_EXTERNALS,
            [ ALLOC_INIT_OBJECT(ArnRemoveExternalsAction) ]
            );

    return SCENEGRAPH_REMOVE_EXTERNALS_GV;
}

ArNode <ArpAction> * read_extra_data_action_singleton(
        ART_GV  * art_gv
        )
{
    if ( ! SCENEGRAPH_READ_EXTRA_DATA_GV )
        ARNODE_SINGLETON(
            SCENEGRAPH_READ_EXTRA_DATA_GV,
            SCENEGRAPH_READ_EXTRA_DATA,
            [ ALLOC_INIT_OBJECT(ArnReadExtraDataAction) ]
            );

    return SCENEGRAPH_READ_EXTRA_DATA_GV;
}

ArNode <ArpAction> * setup_node_data_action_singleton(
        ART_GV  * art_gv
        )
{
    if ( ! SCENEGRAPH_SETUP_DATA_GV )
        ARNODE_SINGLETON(
            SCENEGRAPH_SETUP_DATA_GV,
            SCENEGRAPH_SETUP_DATA,
            [ ALLOC_INIT_OBJECT(ArnSetupNodeDataAction) ]
            );

    return SCENEGRAPH_SETUP_DATA_GV;
}

ArNode <ArpAction> * convert_to_tree_action_singleton(
        ART_GV  * art_gv
        )
{
    if ( ! SCENEGRAPH_CREATE_FLATTENED_COPY_GV )
        ARNODE_SINGLETON(
            SCENEGRAPH_CREATE_FLATTENED_COPY_GV,
            SCENEGRAPH_CREATE_FLATTENED_COPY,
            [ ALLOC_INIT_OBJECT(ArnConvertToTreeAction) ]
            );

    return SCENEGRAPH_CREATE_FLATTENED_COPY_GV;
}

ArNode <ArpAction> * combine_attributes_action_singleton(
        ART_GV  * art_gv
        )
{
    if ( ! SCENEGRAPH_COMBINE_ATTRIBUTES_GV )
        ARNODE_SINGLETON(
            SCENEGRAPH_COMBINE_ATTRIBUTES_GV,
            SCENEGRAPH_COMBINE_ATTRIBUTES,
            [ ALLOC_INIT_OBJECT(ArnCombineAttributesAction) ]
            );

    return SCENEGRAPH_COMBINE_ATTRIBUTES_GV;
}

ArNode <ArpAction> * combine_print_csg_tree_singleton(
        ART_GV  * art_gv
        )
{
    if ( ! SCENEGRAPH_PRINT_CSG_TREE_GV )
        ARNODE_SINGLETON(
            SCENEGRAPH_PRINT_CSG_TREE_GV,
            SCENEGRAPH_PRINT_CSG_TREE,
            [ ALLOC_INIT_OBJECT(ArnPrintCSGTreeAction) ]
            );

    return SCENEGRAPH_PRINT_CSG_TREE_GV;
}

ArNode <ArpAction> * alloc_bboxes_action_singleton(
        ART_GV  * art_gv
        )
{
    if ( ! SCENEGRAPH_ALLOC_BBOXES_GV )
        ARNODE_SINGLETON(
            SCENEGRAPH_ALLOC_BBOXES_GV,
            SCENEGRAPH_ALLOC_BBOXES,
            [ ALLOC_INIT_OBJECT(ArnAllocBBoxesAction) ]
            );

    return SCENEGRAPH_ALLOC_BBOXES_GV;
}

ArNode <ArpAction> * init_bboxes_action_singleton(
        ART_GV  * art_gv
        )
{
    if ( ! SCENEGRAPH_INIT_BBOXES_GV )
        ARNODE_SINGLETON(
            SCENEGRAPH_INIT_BBOXES_GV,
            SCENEGRAPH_INIT_BBOXES,
            [ ALLOC_INIT_OBJECT(ArnInitBBoxesAction) ]
            );

    return SCENEGRAPH_INIT_BBOXES_GV;
}

ArNode <ArpAction> * shrink_bboxes_action_singleton(
        ART_GV  * art_gv
        )
{
    if ( ! SCENEGRAPH_SHRINK_BBOXES_GV )
        ARNODE_SINGLETON(
            SCENEGRAPH_SHRINK_BBOXES_GV,
            SCENEGRAPH_SHRINK_BBOXES,
            [ ALLOC_INIT_OBJECT(ArnShrinkBBoxesAction) ]
            );

    return SCENEGRAPH_SHRINK_BBOXES_GV;
}

ArNode <ArpAction> * optimise_bboxes_action_singleton(
        ART_GV  * art_gv
        )
{
    if ( ! SCENEGRAPH_OPTIMISE_BBOXES_GV )
        ARNODE_SINGLETON(
            SCENEGRAPH_OPTIMISE_BBOXES_GV,
            SCENEGRAPH_OPTIMISE_BBOXES,
            [ ALLOC_INIT_OBJECT(ArnOptimiseBBoxesAction) ]
            );

    return SCENEGRAPH_OPTIMISE_BBOXES_GV;
}

// ===========================================================================

