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

#define ART_MODULE_NAME     ART_Parse_File

#import "ART_Parse_File.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


ArNode * _art_parse_file(
              ART_GV  * art_gv,
        const char    * filename,
        const BOOL      includeLibraryPathInSearch,
        const BOOL      createNewFileIfNeeded
        )
{
    ArFiletypeMatch  filetypeMatch;

    id <ArpFiletype> nativeFileypeObject =
        [ ArcFileProbe filetypeObjectForFilename
            :   art_gv
            :   filename
            :   includeLibraryPathInSearch
            :   createNewFileIfNeeded
            : & filetypeMatch
            ];

    switch ( filetypeMatch )
    {
        case arfiletypematch_failure:
            ART_ERRORHANDLING_FATAL_ERROR(
                "file '%s' not found"
                ,   filename
                );
            break;

        case arfiletypematch_impossible:
            ART_ERRORHANDLING_FATAL_ERROR(
                "no matching parser found for file '%s'"
                ,   filename
                );
            break;

        case arfiletypematch_weak:
            ART_ERRORHANDLING_WARNING(
                "only sub-optimal parser available for file '%s' - "
                "file read may yield unsatisfactory results"
                ,   filename
                );
            break;

        default:
            break;
    }

    ArNode  * resultNode;

    [ (ArcObject <ArpParser> *)nativeFileypeObject parseFile
        : & resultNode
        ];

    RELEASE_OBJECT(nativeFileypeObject);

    return resultNode;
}

ArNode * art_parse_file(
              ART_GV  * art_gv,
        const char    * filename,
        const BOOL      includeLibraryPathInSearch
        )
{
    return
        _art_parse_file(
            art_gv,
            filename,
            includeLibraryPathInSearch,
            YES
            );
}

ArNode * art_parse_existing_file(
              ART_GV  * art_gv,
        const char    * filename,
        const BOOL      includeLibraryPathInSearch
        )
{
    return
        _art_parse_file(
            art_gv,
            filename,
            includeLibraryPathInSearch,
            NO
            );
}

// ===========================================================================
