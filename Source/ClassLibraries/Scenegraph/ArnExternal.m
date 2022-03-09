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

#define ART_MODULE_NAME     ArnExternal

#include "ART_Foundation.h"
#import "ArnVisitor.h"
#import "ArnExternal.h"
#import "ArnNamedNodeSet.h"

#ifdef ART_4_OPENSTEP
#import <Foundation/NSInvocation.h>
#endif

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnExternal registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


ARLIST_IMPLEMENTATION_FOR_OBJECT_TYPE(ArnExternal,external);


@interface ArnVisitor ( External )

- (ArNode *) removeExternals
        : (ArNode *) node
        : (void *) unused
        ;

@end

@implementation ArnVisitor ( External )

- (ArNode *) removeExternals
        : (ArNode *) node
        : (void *) unused
{
    if ( [ node isMemberOfClass: [ ArnExternal class ] ] )
    {
        id result =
            [ (ArnExternal *)node subnodeWithIndex
                :   0
                ];

//        if ( result != node )
//        {
//            debugprintf("\nRelease External\n")
//            RELEASE_OBJECT(node);
//        }

        return result;
    }
    else
    {
        return node;
    }
}

@end

@implementation ArNode ( External )

#define EXTERNAL_REF    subnodeRefArray[0]
#define AUXILIARY_REF   subnodeRefArray[1]

#define EXTERNAL        (ArNode *)ARNODEREF_POINTER(EXTERNAL_REF)
#define AUXILIARY       (ArNode *)ARNODEREF_POINTER(AUXILIARY_REF)


- (ArNode *) removeExternals
{
    ArnVisitor * visitor = [ ALLOC_INIT_OBJECT(ArnVisitor) ];

    ArNode  * result =
        [ visitor modifyPreOrder
            :   arvisitmode_full_dag_with_attributes
            :   self
            :   @selector(removeExternals::)
            :   0
            ];

    RELEASE_OBJECT(visitor);

    return result;
}

@end

@implementation ArnExternal

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnExternal)

- (id) _getFileNamed
        : (char *) fileName
{
    ArFiletypeMatch  filetypeMatch;

    id returnFile =
        [ ArcFileProbe filetypeObjectForFilename
            :   art_gv
            :   fileName
            :   YES
            :   YES
            : & filetypeMatch
            ];

    if (! returnFile)
        ART_ERRORHANDLING_FATAL_ERROR(
            "could not read file \"%s\""
            ,   fileName
            );

    switch (filetypeMatch)
    {
        case arfiletypematch_impossible:
        {
            ART_ERRORHANDLING_FATAL_ERROR(
                "no parser for \"%s\" found"
                ,   fileName
                );
            break;
        }
        case arfiletypematch_weak:
        {
            ART_ERRORHANDLING_WARNING(
                "only unsafe parser for \"%s\" available"
                ,   fileName
                );
            break;
        }
        default:
            break;
    }

    return returnFile;
}

- (void) _loadExternal
{
    ArString  complete_path_to_external = NULL;

    BOOL  external_found =
        full_path_for_filename(
            & complete_path_to_external,
              [ self externalFileName ],
              ART_INCLUDE_PATHS
            );

    if ( ! external_found )
    {
        ART_ERRORHANDLING_FATAL_ERROR(
            "could not locate external file %s",
            [ self externalFileName ]
            );
    }

    ArNode   * auxiliaryNode    = [ self auxiliaryNode ];

    ArNode <ArpFiletype, ArpParser> * nativeFile =
        [ self _getFileNamed: complete_path_to_external ];

    FREE( complete_path_to_external );

    ArNode  * externalContent;

    ArList  externalList = ARLIST_EMPTY;

    if ( auxiliaryNode )
        [ nativeFile parseFileGetExternalsWithAuxiliaryNode
            : & externalContent
            : & externalList
            :   auxiliaryNode
            ];
    else
        [ nativeFile parseFileGetExternals
            : & externalContent
            : & externalList
            ];

    RELEASE_OBJECT(nativeFile);

    ArNode  * externalContentNode;

    if (    [ externalContent isMemberOfClass
                :   [ ArnNamedNodeSet class ]
                ]
          && objectName != 0 )
    {
        externalContentNode =
            [ (ArnNamedNodeSet *)externalContent
                nodeWithName
                :   objectName
                ];
    }
    else
    {
        externalContentNode = externalContent;
    }

    //   MASKING_INSTANCE_ID is a "magic" value for the instance ID which
    //   prevents the node in question from being collected during scene
    //   graph linearisation.

    [ externalContentNode setInstanceID
        :   MASKING_INSTANCE_ID
        ];

    [ self setSubnodeRefWithIndex
        :   0
        :   HARD_NODE_REFERENCE(externalContentNode)
        ];
}

- (id) init
        : (ArSymbol) newExternalFileName
        : (Protocol *) newArProtocol
{
    return
        [ self init
            :   newExternalFileName
            :   0
            :   newArProtocol
            :   0
            ];
}

- (id) init
        : (ArSymbol) newExternalFileName
        : (ArSymbol) newObjectName
        : (Protocol *) newArProtocol
{
    return
        [ self init
            :   newExternalFileName
            :   newObjectName
            :   newArProtocol
            :   0
            ];
}

- (id) init
        : (ArSymbol) newExternalFileName
        : (ArSymbol) newObjectName
        : (Protocol *) newArProtocol0
        : (Protocol *) newArProtocol1
{
    self =
        [ super init
            :   ARNODEREF_NONE
            :   ARNODEREF_NONE
            ];
    
    if ( self )
    {
        externalFileName = newExternalFileName;
        objectName       = newObjectName;
        arProtocol0      = newArProtocol0;
        arProtocol1      = newArProtocol1;
    }
    
    return self;
}

- (id) initWithAuxiliaryNode
        : (ArSymbol) newExternalFileName
        : (ArNodeRef) newAuxiliary
        : (Protocol *) newArProtocol
{
    self =
        [ super init
            :   ARNODEREF_NONE
            :   newAuxiliary
            ];
    
    if ( self )
    {
        externalFileName = newExternalFileName;
        arProtocol0      = newArProtocol;
        arProtocol1      = 0;
    }
    
    return self;
}

- (id) copy
{
    ArnExternal  * copiedInstance = [ super copy ];

    copiedInstance->externalFileName = externalFileName;
    copiedInstance->objectName       = objectName;
    copiedInstance->arProtocol0      = arProtocol0;
    copiedInstance->arProtocol1      = arProtocol1;

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnExternal  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    copiedInstance->externalFileName = externalFileName;
    copiedInstance->objectName       = objectName;
    copiedInstance->arProtocol0      = arProtocol0;
    copiedInstance->arProtocol1      = arProtocol1;

    return copiedInstance;
}

- (BOOL) conformsToArProtocol
        : (Protocol *) protocol
{
    if ( EXTERNAL )
        return [ EXTERNAL conformsToArProtocol: protocol ];
    else
    {
        if (   ( arProtocol0 && arProtocol0 == protocol )
            || ( arProtocol1 && arProtocol1 == protocol ) )
            return YES;
        else
            return NO;
    }
}

#ifdef ART_WITH_NSOBJECT

- (NSMethodSignature *) methodSignatureForSelector
        : (SEL) aSelector
{
    NSMethodSignature  * sig = 0;

    if ( ! EXTERNAL )
        [ self _loadExternal ];

    if ( EXTERNAL )
    {
        sig =
            [ EXTERNAL methodSignatureForSelector
                :   aSelector
                ];
    }

    return sig;
}

- (void) forwardInvocation:(NSInvocation *)anInvocation
{
    if ( ! EXTERNAL )
    {
        [ self _loadExternal ];
    }

    if ( EXTERNAL )
        [ anInvocation invokeWithTarget: EXTERNAL ];
    else
        [ self doesNotRecognizeSelector: [ anInvocation selector ] ];
}

#else

- (ART_RETURN_TYPE) forward
        : (SEL) aSelector
        : (ART_ARGUMENT_LIST) argFrame
{
    if ( EXTERNAL )
        return [ EXTERNAL performv: aSelector : argFrame ];

    [ self doesNotRecognize: aSelector ];

    return (ART_RETURN_TYPE) 0;
}

#endif

- (ArSymbol) externalFileName
{
    return externalFileName;
}

- (ArSymbol) objectName
{
    return objectName;
}

- (ArNode *) auxiliaryNode
{
    return AUXILIARY;
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    if ( [ coder isReading ] )
    {
        //   No special treatment necessary during reading.

        [ super code: coder ];
    }
    else
    {
        //   If we are writing the ArnExternal node to disk we make
        //   sure that any instantiated externals are not written
        //   along with it.

        ArNodeRef  temp;

        temp = EXTERNAL_REF;
        EXTERNAL_REF = ARNODEREF_NONE;

        [ super code: coder ];

        EXTERNAL_REF = temp;
    }

    [ coder codeSymbol:   & externalFileName ];
    [ coder codeSymbol:   & objectName ];
    [ coder codeProtocol: & arProtocol0 ];
    [ coder codeProtocol: & arProtocol1 ];
}

@end

// ===========================================================================
