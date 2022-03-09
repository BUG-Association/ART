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

#define ART_MODULE_NAME     ART_PathAndDirectoryTools

#include "ART_PathAndDirectoryTools.h"

#include "ART_SystemDatatypes.h"
#include "ART_ErrorHandling.h"

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <sys/stat.h>

#if ! defined( MINGW )
#include <wordexp.h>
#endif

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


void create_envvar_assignment_string(
        ArString  * assignment_string_to_create,
        ArString    varname,
        ArString    assignment
        )
{
    size_t  varname_strlen = strlen( varname );
    size_t  assignment_strlen = strlen( assignment );

    size_t  assignment_string_length =
          varname_strlen
        + 1                     // <- =
        + assignment_strlen
        + 1;                    // <- NULL

    *assignment_string_to_create =
        ALLOC_ARRAY( char, assignment_string_length );

    memcpy(
        *assignment_string_to_create,
        varname,
        varname_strlen * sizeof(char)
    );

    (*assignment_string_to_create)[varname_strlen] = '=';

    memcpy(
        *assignment_string_to_create + varname_strlen + 1,
        assignment,
        assignment_strlen * sizeof(char)
    );

    (*assignment_string_to_create)[assignment_string_length-1] = '\0';
}

void create_pathlist_from_stringarray(
        ArString            * pathlist_to_create,
        ArConstStringArray    stringarray
        )
{
    if ( stringarray )
    {
        size_t  j = 0;
        size_t  combined_pathlist_length = 0;

        while ( stringarray[j] != 0 )
        {
            combined_pathlist_length +=
                strlen( stringarray[j] ) + 1; // <- for the separating :
            j++;
        }
        
        //   The above logic works for all cases except empty stringarrays

        if ( combined_pathlist_length == 0 )
            combined_pathlist_length = 1;

        *pathlist_to_create = ALLOC_ARRAY( char, combined_pathlist_length );
        
        j = 0;

        int  current_insertion_index = 0;

        while ( stringarray[j] != 0 )
        {
            size_t  length_of_current_path = strlen( stringarray[j] );

            // Sanity check to avoid buffer overflow
            if ( length_of_current_path >= combined_pathlist_length - current_insertion_index )
            {
                ART_ERRORHANDLING_FATAL_ERROR("String buffer overflow avoided");
            }
            
            memcpy(
                *pathlist_to_create + current_insertion_index,
                stringarray[j],
                length_of_current_path * sizeof(char)
                );

            (*pathlist_to_create)[current_insertion_index + length_of_current_path] = ':';

            current_insertion_index += length_of_current_path + 1;

            j++;
        }

        (*pathlist_to_create)[combined_pathlist_length-1] = '\0';
    }
    else
    {
        *pathlist_to_create = ALLOC_ARRAY( char, 1 );
        (*pathlist_to_create)[0] = '\0';
    }
}

int wordexp_s_copy_s(const char * in_path, char ** out_path)
{
#if defined( MINGW )
    arstring_s_copy_s(in_path, out_path);
#else
    const int max_attempts_count = 20;
    wordexp_t we;
    memset(&we, 0, sizeof(we));

    for (int attempt = 0; attempt < max_attempts_count; attempt++)
    {
        wordexp_t  we;
        memset(&we, 0, sizeof(we));

        int wordexp_result =
            wordexp(
                in_path,
                & we,
                WRDE_UNDEF
                );

        if ((wordexp_result == 0) && (we.we_wordc > 0) && (we.we_wordv[0] != NULL))
        {
            *out_path = 0;

            arstring_s_copy_s(
                we.we_wordv[0],
                out_path
                );

            wordfree(&we);
            return 1;
        }
        else
            wordfree(&we);
    }

    ART_ERRORHANDLING_FATAL_ERROR(
        "\nwordexp() failed to expand \"%s\"\n",
        in_path
        );

    return 0;
#endif
}

size_t add_paths_from_pathlist_to_stringarray(
        ArConstString  list_of_dirs_to_add,
        const size_t   next_free_slot_in_stringarray,
        ArStringArray  stringarray_to_add_to
        )
{
    //   This processes a colon-separated list of directories in a path
    //   string, and adds each of them to the FTS-compatible path array
    //   provided by the caller. The path array is assumed to have enough
    //   storage for all elements this function might want to enter, so
    //   no checks are done for path array overflows.

    if ( ! list_of_dirs_to_add || strlen(list_of_dirs_to_add) == 0 )
        return 0;

    size_t  addedDirs = 0;
    size_t  currentIndex = next_free_slot_in_stringarray;
    size_t  pathBegin = 0;
    size_t  length_of_dirs_to_add = strlen( list_of_dirs_to_add );

    char  * char_buffer = ALLOC_ARRAY(char, length_of_dirs_to_add + 1);

    while ( length_of_dirs_to_add != pathBegin )
    {
        char  * startOfNextDirectory =
            strchr( list_of_dirs_to_add + pathBegin, ':' );

        size_t  pathlen;

        if ( startOfNextDirectory )
        {
            pathlen = (size_t) (
                  startOfNextDirectory
                - ( list_of_dirs_to_add + pathBegin )
                );
        }
        else
        {
            pathlen = strlen( list_of_dirs_to_add + pathBegin );
        }

        // Sanity check to avoid buffer overflow
        if ( pathlen > length_of_dirs_to_add )
        {
            ART_ERRORHANDLING_FATAL_ERROR("String buffer overflow avoided");
        }
            
        strncpy(
            char_buffer,
            list_of_dirs_to_add + pathBegin,
            pathlen
            );

        strncpy(
              char_buffer + pathlen,
              "\0",
              1
              );

        if (wordexp_s_copy_s(char_buffer, &stringarray_to_add_to[currentIndex]))
        {
            currentIndex++;
            addedDirs++;
        }

        if ( startOfNextDirectory )
            pathBegin = (size_t) (startOfNextDirectory - list_of_dirs_to_add + 1);
        else
            pathBegin = length_of_dirs_to_add;
    }

    FREE( char_buffer );

    return addedDirs;
}

void  create_string_from_first_existing_path_in_pathlist(
        ArString       * string_to_create,
        ArConstString    pathlist
        )
{
    if ( ! pathlist || strlen(pathlist) == 0 )
        return;

    size_t  subPathStartLocation = 0;
    size_t  length_of_entire_pathlist = strlen( pathlist );

    char  * char_buffer =
        ALLOC_ARRAY(char, length_of_entire_pathlist + 1);

    while ( length_of_entire_pathlist >= subPathStartLocation )
    {
        char  * startOfNextDirectory =
            strchr( pathlist + subPathStartLocation, ':' );

        size_t  pathlen;

        if ( startOfNextDirectory )
            pathlen = (size_t)(
                  startOfNextDirectory
                - ( pathlist + subPathStartLocation ));
        else
            pathlen = strlen( pathlist + subPathStartLocation );

        // Sanity check to avoid buffer overflow
        if ( pathlen > length_of_entire_pathlist)
        {
            ART_ERRORHANDLING_FATAL_ERROR("String buffer overflow avoided");
        }
        
        strncpy(
            char_buffer,
            pathlist + subPathStartLocation,
            pathlen
            );

        strncpy(
              char_buffer + pathlen,
              "\0",
              1
              );

        char * expanded_path = NULL;
        if (wordexp_s_copy_s(char_buffer, &expanded_path))
        {
            struct stat  st;

            int stat_result = stat( expanded_path, & st );

            if ( stat_result != -1 )
            {
                *string_to_create = expanded_path;

                break;
            }
            else
                FREE_ARRAY(expanded_path);
        }

        if ( startOfNextDirectory )
            subPathStartLocation = (size_t) (startOfNextDirectory - pathlist + 1);
        else
            subPathStartLocation = (size_t) pathlist;
    }

    FREE( char_buffer );
}

size_t  count_paths_in_pathlist(
        ArConstString  pathlist
        )
{
    //   This counts the colon-separated directories in a path listing.

    if ( ! pathlist )
        return 0;

    size_t pathBegin = 0;

    size_t numberOfDirs = 0;

    while ( strlen( pathlist ) != pathBegin )
    {
        char  * startOfNextDirectory =
            strchr( pathlist + pathBegin, ':' );

        if ( startOfNextDirectory )
            pathBegin = (size_t) (startOfNextDirectory - pathlist + 1);
        else
            pathBegin = strlen( pathlist );

        numberOfDirs++;
    }

    return numberOfDirs;
}

void  create_stringarray_from_pathlists(
        ArStringArray  * pathlist_to_create,
        ArConstString    first_pathlist,
        ...
        )
{
    if ( first_pathlist )
    {
        //   We count the number of distinct entries in the two sources of
        //   path names. This is a safe upper bound for the char ** array
        //   size, and has to be done before the actual pathlist can be
        //   allocated.

        const char  * current_pathlist = first_pathlist;
        size_t        possible_number_of_paths = 0;
        va_list       arg_ptr;

        va_start( arg_ptr, first_pathlist );

        while ( current_pathlist )
        {
            possible_number_of_paths +=
                count_paths_in_pathlist( current_pathlist );
            current_pathlist = va_arg( arg_ptr, char * );
        }

        va_end( arg_ptr );

        *pathlist_to_create =
            ALLOC_ARRAY( char *, possible_number_of_paths + 1 );
            // <- + 1 for zero

        (*pathlist_to_create)[0] = 0;

        //   We then add the paths from the two sources to the array.
        //   Since the add function might eliminate duplicates, the total
        //   number might be less than the upper bound computed earlier.

        size_t  actual_number_of_paths = 0;
        current_pathlist = first_pathlist;
        va_start( arg_ptr, first_pathlist );

        while ( current_pathlist )
        {
            actual_number_of_paths +=
                add_paths_from_pathlist_to_stringarray(
                    current_pathlist,
                    actual_number_of_paths,
                    *pathlist_to_create
                    );

            current_pathlist = va_arg( arg_ptr, char * );
        }

        va_end( arg_ptr );

        //   Manually inserted final zero just beyond the final entry.

        (*pathlist_to_create)[actual_number_of_paths] = 0;
    }
    else
    {
        *pathlist_to_create = 0;
    }

}

void  create_stringarray_from_two_spliced_pathlists(
        ArStringArray  * pathlist_to_create,
        ArConstString    prefix_pathlist,
        ArConstString    main_pathlist
        )
{
    if ( prefix_pathlist || main_pathlist )
    {
        //   The number of possible pathlist entries is the number of prefixes
        //   times the number of entries in the main pathlist.

        size_t  possible_number_of_paths =
              count_paths_in_pathlist( prefix_pathlist )
            * count_paths_in_pathlist( main_pathlist );

        *pathlist_to_create =
            ALLOC_ARRAY( char *, possible_number_of_paths + 1 );
            // <- + 1 for zero

        ArStringArray  prefix_path_stringarray;

        create_stringarray_from_pathlists(
            & prefix_path_stringarray,
              prefix_pathlist,
              NULL
            );

        ArStringArray  main_path_stringarray;

        create_stringarray_from_pathlists(
            & main_path_stringarray,
              main_pathlist,
              NULL
            );

        int  actual_number_of_paths = 0;

        if ( arstringarray_len(prefix_path_stringarray) > 0 )
        {
            int  i = 0;

            while ( prefix_path_stringarray[i] )
            {
                if ( arstringarray_len(main_path_stringarray) > 0 )
                {
                    int  j = 0;

                    while ( main_path_stringarray[j] )
                    {
                        ArString  newPath;

                        arstring_scs_copy_and_add_component_s(
                              prefix_path_stringarray[i],
                              '/',
                              main_path_stringarray[j],
                            & newPath
                            );

                        struct stat  st;

                        if ( stat( newPath, & st ) != -1 )
                        {
                            (*pathlist_to_create)[actual_number_of_paths] = newPath;
                            actual_number_of_paths++;
                        }
                        else
                            FREE(newPath);

                        j++;
                    }
                }
                else
                    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR;
                    //   an empty main array should also be handled

                i++;
            }
        }
        else
            ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR;
            //   an empty prefix array should also be handled

        //   Manually inserted final zero just beyond the final entry.

        (*pathlist_to_create)[actual_number_of_paths] = 0;

        arstringarray_free( & prefix_path_stringarray );
        arstringarray_free( & main_path_stringarray );
    }
    else
    {
        *pathlist_to_create = 0;
    }
}

int full_path_for_filename(
        ArString            * full_path,
        ArConstString         filename,
        ArConstStringArray    pathlist
        )
{
    int  found = 0;

    if ( pathlist && filename )
    {
        int  j = 0;

        while ( ! found && pathlist[j] )
        {
            arstring_scs_copy_and_add_component_s(
                pathlist[j],
                '/',
                filename,
                full_path
                );

            struct stat  st;

            if ( stat( *full_path, & st ) != -1 )
            {
                found = 1;
            }
            else
            {
                if ( *full_path )
                    FREE( *full_path );
            }

            j++;
        }
    }

    return found;
}

int filename_has_extension(
        ArConstString  filename,
        ArConstString  extension
        )
{
    size_t  filenameLength  = strlen( filename );
    size_t  extensionLength = strlen( extension );

    if ( strncmp(
            filename + filenameLength - extensionLength,
            extension,
            extensionLength
            ) )
        return 0;
    else
        return 1;
}

/* ======================================================================== */
