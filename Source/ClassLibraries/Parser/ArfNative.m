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

#define ART_MODULE_NAME     ArfNative

#import <ctype.h>
#import <stdlib.h>
#import <string.h>

#import "ArfNative.h"
#import "ArcBinaryCoder.h"
#import "ArcObjCCoder.h"
#import "ArnTriangleMesh.h"

static const char * arfnativebinary_magic_string =      "ART binary";
static const char * arfnativebinary_short_class_name =  "native ART binary";
static const char * arfnativebinary_long_class_name =
                            "Advanced Rendering Toolkit binary";

static const char * arfnativeobjectivec_magic_string =  "// ART - Objective-C";
static const char * arfnativeobjectivec_short_class_name = "native ART ObjC";
static const char * arfnativeobjectivec_long_class_name =
                            "Advanced Rendering Toolkit Objective C";

ART_MODULE_INITIALISATION_FUNCTION
(
    [ ArfNativeBinary registerWithFileProbe
        :   art_gv
        ];

    [ ArfNativeObjectiveC registerWithFileProbe
        :   art_gv
        ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


TYPEDEF_ARHASH(ArNode *, entry, node, ArHashNodeEntry);

typedef struct ArNodeHashEntry
{
    ArHashEntry    entry;
    ArNode       * node;
}
ArNodeHashEntry;

#define ARNODEHASHTABLE_FIND_ENTRY(__table,__key) \
    STRUCT_POINTER( ArNodeHashEntry, \
                     arhashtable_find_hash( \
                         (__table), \
                         (unsigned long)(__key), 0 ), \
                         entry );

#define ARNODEHASHTABLEENTRY_HASHENTRY(__nht)  (__nht).entry
#define ARNODEHASHTABLEENTRY_NODE(__nht)       (__nht).node

#define ARNHTE_NODE      ARNODEHASHTABLEENTRY_NODE

@implementation ArfNative

ARPFILE_DEFAULT_IMPLEMENTATION(
    ArfNative,
    arfiletypecapabilites_read | arfiletypecapabilites_write
    );

ARPPARSER_AUXLIARY_NODE_DEFAULT_IMPLEMENTATION

+ (ArFiletypeMatch) matchWithStream
        : (ArcObject <ArpStream> *) stream
{
    ART__VIRTUAL_METHOD__EXIT_WITH_ERROR

    return arfiletypematch_impossible;
}

+ (const char**) extensions
{
    ART__VIRTUAL_METHOD__EXIT_WITH_ERROR

    return 0;
}

- (id) initWithFile: (ArcFile *) newFile
{
    file = newFile;

    return self;
}

- (void) dealloc
{
    RELEASE_OBJECT(file);

    [ super dealloc ];
}

+ (const char*) magicString
{
    ART__VIRTUAL_METHOD__EXIT_WITH_ERROR

    return 0;
}

- (const char*) shortClassName
{
    ART__VIRTUAL_METHOD__EXIT_WITH_ERROR

    return 0;
}

- (const char*) longClassName
{
    ART__VIRTUAL_METHOD__EXIT_WITH_ERROR

    return 0;
}

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

- (void) parseFile
        : (ArNode **) objectPtr
{
    ArList  externalList = ARLIST_EMPTY;

    //   The call to parseFileGetExternals performs the actual parsing of the
    //   file in question, i.e. it instances all nodes that are directly
    //   encoded in it.

    [ self parseFileGetExternals
        :   objectPtr
        : & externalList
        ];

    //   The only thing that is deferred until after the entire file has been
    //   read is the resolving of the ArnExternal nodes (i.e. those nodes
    //   which refer to entites which are encoded in separate files). The reason
    //   for this is that the easiest way to keep track of duplicate external
    //   references is to do this only after all externals have been
    //   collected in a list.

    //   The following hashtable is used to keep track of which externals
    //   have already been read from file; the purpose of this is to avoid
    //   re-reading them if multiple references to them exist in a scene file.

    ArHashTable  hashtableOfAlreadyLoadedExternals;

    arhashtable_init( & hashtableOfAlreadyLoadedExternals, 0 );

    //   We count the externals and then treat all of them in a loop

    const unsigned int  numberOfExternals = arlist_length( & externalList );

    for ( unsigned int i = 0; i < numberOfExternals; i++ )
    {
        //   Popping the external from the list also destroys the list entry,
        //   so there is no list left to free once we're through with them

        ArnExternal  * external;

        arlist_pop_external_from_head(
            & externalList,
            & external
            );

        //   Extraction of information from the ArnExternal

        ArString  complete_path_to_external;

        full_path_for_filename(
            & complete_path_to_external,
              [ external externalFileName ],
              ART_INCLUDE_PATHS
            );

        ArSymbol   externalFileName =
            arsymbol( art_gv, complete_path_to_external );
        ArSymbol   objectName       = [ external objectName ];
        ArNode   * auxiliaryNode    = [ external auxiliaryNode ];

        //   We try to find an entry in the table of externals which have
        //   already been read from disk that matches the one we are looking
        //   for; the key on which this search is based is the filename
        //   of the external file.

        ArNodeHashEntry  * hashtableEntry =
            ARNODEHASHTABLE_FIND_ENTRY(
                & hashtableOfAlreadyLoadedExternals,
                  externalFileName
                  );

        //   If no entry could be found we parse the contents of the external
        //   file in question, and create a hashtable entry for it.

        if (! hashtableEntry )
        {
            hashtableEntry = ALLOC(ArNodeHashEntry);
            hashtableEntry->entry.hash = (unsigned long)externalFileName;

            arhashtable_add_entry(
                & hashtableOfAlreadyLoadedExternals,
                  FIELD_POINTER( hashtableEntry, entry )
                );

            ArNode <ArpFiletype, ArpParser>  * nativeFile =
                [ self _getFileNamed: externalFileName ];

            if (   [ nativeFile conformsToProtocol: ARPROTOCOL(ArpImageFile) ]
                && [ external conformsToArProtocol: ARPROTOCOL(ArpShape) ] )
            {
                ARNHTE_NODE( *hashtableEntry ) =
                    arntrianglemesh_heightfield_from_image(
                          art_gv,
                          arshape_solid,
                          (ArNode <ArpImageFile> *) nativeFile
                        );
            }
            else
            {
                if ( auxiliaryNode )
                    [ nativeFile parseFileGetExternalsWithAuxiliaryNode
                        : & ARNHTE_NODE( *hashtableEntry )
                        : & externalList
                        :   auxiliaryNode
                        ];
                else
                    [ nativeFile parseFileGetExternals
                        : & ARNHTE_NODE( *hashtableEntry )
                        : & externalList
                        ];
            }

            RELEASE_OBJECT(nativeFile);
        }

        ArNode  * externalContentNode;

        if (    [ ARNHTE_NODE( *hashtableEntry )
                    isMemberOfClass
                    :   [ArnNamedNodeSet class]
                    ]
              && objectName != 0 )
        {
            externalContentNode =
                [(ArnNamedNodeSet *)ARNHTE_NODE( *hashtableEntry )
                    nodeWithName
                    :   objectName
                    ];
        }
        else
        {
            externalContentNode = ARNHTE_NODE( *hashtableEntry );
        }

        [ external setSubnodeRefWithIndex
            :   0
            :   HARD_NODE_REFERENCE(externalContentNode)
            ];
    }

    //   HIER: hashtable versaften

            //FIXME!!!!!!!!!
            /*hashEntry = STRUCT_POINTER(ArNodeHashEntry,
                   arhashtable_next_entry(&moduleTable, 0),
                   entry);


            //Free the remaining Nodes in the ArnNamedNodeSet since we got the node
            //we were interested in

            while (hashEntry)
            {
                ArNodeHashEntry * nextNode =
                    STRUCT_POINTER(ArNodeHashEntry,
                                   arhashtable_next_entry(&moduleTable,
                                                FIELD_POINTER(hashEntry,
                                                entry)),
                                   entry);

                RELEASE_OBJECT(hashEntry->node);
                arhashtable_del_entry(&moduleTable,FIELD_POINTER(hashEntry,entry));
                FREE(hashEntry);
                hashEntry = nextNode;
            }*/
}

- (void) parseFileGetExternals
        : (ArNode **) objectPtr
        : (ArList *) externals
{
    if (([file open :arfile_read] & arstream_invalid) == 0)
        [self parseStreamGetExternals :objectPtr :file :externals];
    else
        ART_ERRORHANDLING_FATAL_ERROR(
            "could not open file '%s'"
            ,   [ file name ]
            );

    [ file close ];
}

- (void) parseStream
        : (ArNode **) objectPtr
        : (ArcObject <ArpStream> *) stream
{
    [self parseStreamGetExternals :objectPtr :stream :0];
}

- (void) parseStreamGetExternals
        : (ArNode **) objectPtr
        : (ArcObject <ArpStream> *) stream
        : (ArList *) externals
{
    ART__VIRTUAL_METHOD__EXIT_WITH_ERROR
}

@end

const char * arfnativebinary_exts[] =
    { "arb", 0 };

@implementation ArfNativeBinary

ARPFILE_DEFAULT_IMPLEMENTATION(
    ArfNativeBinary,
    arfiletypecapabilites_read | arfiletypecapabilites_write
    )

+ (ArFiletypeMatch) matchWithStream
        : (ArcObject <ArpStream> *) stream
{
    return
        [ ArcBinaryReadingCoder matchWithStream
            :   stream
            ];
}

+ (const char **) extensions
{
    return arfnativebinary_exts;
}

+ (const char*) magicString
{
    return arfnativebinary_magic_string;
}

- (const char*) shortClassName
{
    return arfnativebinary_short_class_name;
}

- (const char*) longClassName
{
    return arfnativebinary_long_class_name;
}

- (void) parseStreamGetExternals
        : (ArNode **) objectPtr
        : (ArcObject <ArpStream> *) stream
        : (ArList *) externals
{
    ArcBinaryReadingCoder * coder =
        [ ALLOC_INIT_OBJECT(ArcBinaryReadingCoder)
            :   stream ];

    [ coder codeObject
        :   objectPtr
        :   externals
        ];

    RELEASE_OBJECT(coder);
}

@end

const char * arfnativeobjectivec_exts[] =
    { "art", 0 };

@implementation ArfNativeObjectiveC

ARPFILE_DEFAULT_IMPLEMENTATION(
    ArfNativeObjectiveC,
    arfiletypecapabilites_read | arfiletypecapabilites_write
    )

+ (ArFiletypeMatch) matchWithStream
        : (ArcObject <ArpStream> *) stream
{
    return
        [ ArcObjCReadingCoder matchWithStream
            :   stream
            ];
}

+ (const char **) extensions
{
    return arfnativeobjectivec_exts;
}

+ (const char*) magicString
{
    return arfnativeobjectivec_magic_string;
}

- (const char*) shortClassName
{
    return arfnativeobjectivec_short_class_name;
}

- (const char*) longClassName
{
    return arfnativeobjectivec_long_class_name;
}

- (void) parseStreamGetExternals
        : (ArNode **) objectPtr
        : (ArcObject <ArpStream> *) stream
        : (ArList *) externals
{
    ArcObjCReadingCoder * coder =
        [ ALLOC_INIT_OBJECT(ArcObjCReadingCoder)
            :   stream
            ];

    [ coder codeObject
        :   objectPtr
        :   externals
        ];

    RELEASE_OBJECT(coder);
}

@end

// ===========================================================================
