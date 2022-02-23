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

#define ART_MODULE_NAME     ArcFileProbe

#include "ART_Foundation_Geometry.h"

#import "ArcFileProbe.h"
#include <fts.h>
#include <string.h>

typedef struct ArFiletypeClassesForExtension
{
    ArSymbol  extension;
    ArList    filetypeClassList;
}
ArFiletypeClassesForExtension;

#define ARFTC4E_EXTENSION(__f)                  (__f).extension
#define ARFTC4E_FILETYPECLASSLIST(__f)          (__f).filetypeClassList

ARTABLE_INTERFACE_FOR_STRUCTURE_WITH_ARSYMBOL_INDEX_FIELD(
        ArFiletypeClassesForExtension,
        ftc4e,
        extension
        )

ARTABLE_IMPLEMENTATION_FOR_STRUCTURE_WITH_ARSYMBOL_INDEX_FIELD(
        ArFiletypeClassesForExtension,
        ftc4e,
        extension
        )

typedef struct ArcFileProbe_GV
{
    pthread_mutex_t    ftc4e_mutex;
    ArTable          * ftc4e_table;
}
ArcFileProbe_GV;

#define ARCFILEPROBE_GV         art_gv->arcfileprobe_gv
#define ARCFILEPROBE_MUTEX      ARCFILEPROBE_GV->ftc4e_mutex
#define ARCFILEPROBE_TABLE      ARCFILEPROBE_GV->ftc4e_table

#define ARCFILEPROBE_NGV        new_art_gv->arcfileprobe_gv
#define ARCFILEPROBE_MUTEX_NGV  ARCFILEPROBE_NGV->ftc4e_mutex
#define ARCFILEPROBE_TABLE_NGV  ARCFILEPROBE_NGV->ftc4e_table

ART_MODULE_INITIALISATION_FUNCTION
(
    ARCFILEPROBE_GV = ALLOC(ArcFileProbe_GV);

    pthread_mutex_init( & ARCFILEPROBE_MUTEX, NULL );
    ARCFILEPROBE_TABLE = artable_alloc_init_n( 17 );
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    artable_free( ARCFILEPROBE_TABLE );
    pthread_mutex_destroy( & ARCFILEPROBE_MUTEX );

    FREE( ARCFILEPROBE_GV );
)


@implementation ArcFileProbe

+ (void) registerFiletypeClass
        : (ART_GV *) new_art_gv
        : (Class) newFiletypeClass
        : (const char **) extensions
{
    if  ( extensions )
    {
        for ( int i = 0; extensions[i] != 0; i++ )
        {
            ArSymbol  extension = arsymbol( new_art_gv, extensions[i] );

            ArFiletypeClassesForExtension  * ftc4e = NULL;

            pthread_mutex_lock(
                & ARCFILEPROBE_MUTEX_NGV
                );

            ftc4e =
                artable_get_ftc4e_with_key(
                    ARCFILEPROBE_TABLE_NGV,
                    extension
                    );

            if ( ! ftc4e )
            {
                ftc4e = ALLOC( ArFiletypeClassesForExtension );

                ARFTC4E_EXTENSION(*ftc4e) = arsymbol( new_art_gv, extensions[i] );
                ARFTC4E_FILETYPECLASSLIST(*ftc4e) = ARLIST_EMPTY;

                ftc4e =
                    artable_add_ftc4e(
                          ARCFILEPROBE_TABLE_NGV,
                        * ftc4e
                        );
            }

            arlist_add_class( & ARFTC4E_FILETYPECLASSLIST(*ftc4e), newFiletypeClass );

            pthread_mutex_unlock(
                & ARCFILEPROBE_MUTEX_NGV
                );
        }
    }
    else
        ART_ERRORHANDLING_WARNING(
            "the filetype for class '%s' was registered with an empty extension list"
            ,   [ newFiletypeClass cStringClassName ]
            );

}

//   Uncomment the following #define to get diagnostic output as to which
//   filenames are actually used by the path search logic.  (aw)

//#define DEBUG_OUTPUT_PATHNAMES

+ (ArcObject <ArpFiletype> *) filetypeObjectForFilename
        : (ART_GV *) new_art_gv
        : (const char *) filename
        : (BOOL) searchLibraryPath
        : (BOOL) createNewFileIfNeeded
        : (ArFiletypeMatch *) filetypeMatch
{
    return
        [ ArcFileProbe filetypeObjectForFilenameWithProtocol
            :   new_art_gv
            :   filename
            :   0
            :   searchLibraryPath
            :   createNewFileIfNeeded
            :   filetypeMatch
            ];
}

void art_find_file_on_paths(
        const char  ** paths,
        const char   * file_to_find,
              char  ** complete_path_to_file
        )
{
    if ( arstringarray_len( (ArStringArray) paths ) == 0 ) return;

    FTS  * fts_struct;

    fts_struct =
        fts_open(
            (char **)paths,
            FTS_COMFOLLOW | FTS_LOGICAL,
            0
            );

    if ( ! fts_struct ) return;

    FTSENT  * fts_entry;

    while ( (fts_entry = fts_read( fts_struct )) )
    {
        if ( ! strcmp( fts_entry->fts_name, file_to_find) )
        {
            arstring_s_copy_s(
                fts_entry->fts_path,
                complete_path_to_file
                );

            fts_close( fts_struct );

            return;
        }
    }

    fts_close( fts_struct );
}

+ (ArcObject <ArpFiletype> *) filetypeObjectForFilenameWithProtocol
        : (ART_GV *) new_art_gv
        : (const char *) filename
        : (Protocol *) arProtocol
        : (BOOL) searchLibraryPath
        : (BOOL) createNewFileIfNeeded
        : (ArFiletypeMatch *) filetypeMatch
{
    const char  * dot = strrchr(filename, '.');

    // GZ: only at this point: try all dirs in $libraryPath
    // Added a flag to this method to allow/disallow
    // $libraryPath search.  It will return a filename if it finds
    // one in the $libraryPath. Theoretically, if a file in this
    // search path is named identical to the relative path
    // (filename), this would be returned also if you are writing,
    // so you could lose data. The flag setting NO would inhibit this, as you
    // disallow finding a file of that name along $libraryPath

    BOOL  fileExists = NO;

    // will hold one of directory names listed in $libraryPath,
    // plus filename; can be at most as long as $libraryPath/filename.
    char  * completeFilename = NULL;

    // ":"-separated string of directories
    const char ** libraryPath = art_ev_resource_paths( new_art_gv );

    // holds single directory name listed in $libraryPath, and
    // can be at most as long as $libraryPath.
//    char  * pathEntry = NULL;

#ifdef DEBUG_OUTPUT_PATHNAMES

    if ( libraryPath == NULL )
        printf(
            "$ART_LIB_PATH not defined. Only current directory will be searched.\n"
            );
    else
    {
        int j = 0;

        while ( libraryPath[j] != 0 )
        {
            printf(
                "Library path %d is '%s'\n"
                ,   j
                ,   libraryPath[j]
                );

            j++;
        }

        if ( searchLibraryPath == NO )
            printf(
                "Exact filename match required for '%s'\n"
                ,   filename
                );
        else
            printf(
                "Library path will be searched for '%s'\n"
                ,   filename
                );

        if ( createNewFileIfNeeded )
            printf(
                "A new file of that name will be created if needed\n"
                );
        else
            printf(
                "Will only read existing files, exit with error if none found\n"
                );
    }
#endif

    if (   ( libraryPath == NULL )
        || ( searchLibraryPath == NO ) )
    {
        completeFilename = (char *) filename;
        fileExists = YES; // we must assume this. (like old behaviour)
        // alternate, TBD: err("no $libraryPath found"); ?
    }
    else
    {
        art_find_file_on_paths(
              libraryPath,
              filename,
            & completeFilename
            );
        /*
        completeFilename = ALLOC_ARRAY_ZERO( char, strlen(libraryPath) + strlen(filename) + 2 );
        pathEntry = ALLOC_ARRAY_ZERO( char, strlen(libraryPath) + 1 );

        // verify immediate file existence
        strcpy(completeFilename, filename);
        fileExists= [ArcFile exists: completeFilename];
        // if not found, search along $libraryPath
        int pathBegin=0;

        while ( ! fileExists && (strlen(libraryPath) != pathBegin))
        {
            char  * startOfNextPathPart = strchr(libraryPath+pathBegin, ':');

            if (startOfNextPathPart)
            {
                // extract start of libraryPath
                strncpy(
                    pathEntry,
                    libraryPath + pathBegin,
                    startOfNextPathPart - libraryPath - pathBegin );

                pathEntry[ startOfNextPathPart - libraryPath - pathBegin ] = '\0';

                pathBegin = startOfNextPathPart - libraryPath + 1;
            }
            else // no further ':' in libraryPath
            {
                // assume rest of libraryPath as path
                strcpy(pathEntry, libraryPath+pathBegin);
                pathBegin+=strlen(libraryPath+pathBegin);
            }

#ifdef DEBUG_OUTPUT_PATHNAMES
            printf("Searching directory %s\n", pathEntry);
#endif
            // build complete filename

            strcpy(completeFilename, pathEntry);
            completeFilename[strlen(pathEntry)]='/';
            strcpy(completeFilename+strlen(pathEntry)+1, filename);

            // verify file existence
            fileExists = [ ArcFile exists: completeFilename ];
        }
        */

        if ( ! completeFilename )
        {
            if ( [ ArcFile exists: filename ] )
                completeFilename=(char *)filename;
        }

        if ( ! completeFilename )
        {
            completeFilename=(char *)filename;

            if ( ! createNewFileIfNeeded )
            {
                if ( searchLibraryPath )
                    ART_ERRORHANDLING_FATAL_ERROR(
                        "file %s not found in search path",
                        completeFilename
                        );
                else
                    ART_ERRORHANDLING_FATAL_ERROR(
                        "file %s not found in working directory",
                        completeFilename
                        );
            }

            #ifdef DEBUG_OUTPUT_PATHNAMES
            printf("File not found in path. Apparently you want to write a new file: '%s'\n", completeFilename );
            #endif
        }
    }
    /* -----------------------------------------------------------------------
    now completeFilename is the name of the valid file.
    Either the immediately given, or the first file found via libraryPath is taken.
    If we have found no file on libraryPath, we assume it does not exist and
    assume filename, as this was the previous behavior.
    ------------------------------------------------------------------gz--- */
    #ifdef DEBUG_OUTPUT_PATHNAMES
    printf("complete filename is now: '%s'\n", completeFilename ); // DEBUG
    #endif

    Class      filetypeClass = Nil;

    ArcFile  * file =
        [ ArcFile new
            :   new_art_gv
            :   completeFilename
            ];

    if ( dot && dot[1] )
    {
        ArSymbol  extension = arsymbol( new_art_gv, dot + 1 );

        ArFiletypeClassesForExtension  * ftc4e;

        ftc4e =
            artable_get_ftc4e_with_key(
                ARCFILEPROBE_TABLE_NGV,
                extension
                );

        if ( ! ftc4e )
            ART_ERRORHANDLING_FATAL_ERROR(
                "no filetype class found for extension '%s'"
                ,   extension
                );

        ArListEntry  * currentListEntry = ARLIST_HEAD( ARFTC4E_FILETYPECLASSLIST(*ftc4e) );

        Class  currentClass = arlistentry_class( currentListEntry );

        //   We try to open the file this is all about

        if ( [ file open: arfile_read ] & arstream_invalid )
        {
            //   If it does not exist we assume that the user wishes to create a new file of
            //   the type associated with the extension he used.

            //   Note that the first filetype class capable of writing files with the specified
            //   extension found in the list is used in this case, and all others are ignored.

            while (    ! ARCFILETYPE_IS_CAPABLE_OF_WRITING( currentClass )
                    && ( currentListEntry = ARLISTENTRY_NEXT( *currentListEntry ) ) );
            {
                currentClass = arlistentry_class( currentListEntry );
            }

            if ( ! ARCFILETYPE_IS_CAPABLE_OF_WRITING( currentClass ) )
                ART_ERRORHANDLING_FATAL_ERROR(
                    "no filetype class capable of writing found for new file "
                    "with extension '%s'"
                    ,   extension
                    );

            *filetypeMatch = arfiletypematch_exact;
            filetypeClass = currentClass;
        }
        else
        {
            //   If the file already exists we let all the filetypes that list this
            //   extension have a look at it.

            //   This implicitly selects the first

            do
            {
                currentClass = arlistentry_class( currentListEntry );
                *filetypeMatch = [ currentClass matchWithStream: file ];
                filetypeClass = currentClass;
            }
            while (    ( currentListEntry = ARLISTENTRY_NEXT( *currentListEntry ) )
                    && ( *filetypeMatch !=  arfiletypematch_exact ) );

            [ file close ];

            if ( *filetypeMatch ==  arfiletypematch_impossible )
                ART_ERRORHANDLING_FATAL_ERROR(
                    "no filetype class found for file with extension '%s'"
                    ,   extension
                    );
        }
    }
    else
    {
        ART_ERRORHANDLING_FATAL_ERROR(
            "cannot identify type of file '%s' without filename extension"
            ,   [ file name ]
            );
    }

    if ( filetypeClass )
    {
        #ifdef DEBUG_OUTPUT_PATHNAMES
        printf("Class to handle this file: %s\n", [ filetypeClass cStringClassName ]);

        printf("Filename: %s\n", [ file name ]);
        printf("Filetype class: %s\n", [ filetypeClass cStringClassName ]);
        #endif

        return [ [ [ filetypeClass alloc] init_ART_GV: new_art_gv ]
                 initWithFile: file ];
    }
    else
    {
        ART_ERRORHANDLING_FATAL_ERROR(
            "file '%s' not supported by ART"
            ,    [ file name ]
            );

        return 0;
    }
}

@end

// ===========================================================================
