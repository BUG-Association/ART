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

#define ART_MODULE_NAME     ArString

#include "ArString.h"

#include "ART_SystemDatatypes.h"
#include "ART_ErrorHandling.h"

#include <string.h>
#include <stdio.h>

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


void arstring_s_copy_s(
        ArConstString    originalString,
        ArString       * newString
        )
{
    if ( originalString )
    {
        unsigned int  originalStringLength = strlen( originalString );

        *newString = ALLOC_ARRAY( char, originalStringLength + 1 );

        if ( ! *newString )
            ART_ERRORHANDLING_FATAL_ERROR(
                "memory allocation for string copy failed"
                );

        memcpy(
            *newString, 
            originalString, 
            (originalStringLength + 1) * sizeof(char)
            );
    }
    else {
        *newString = NULL;
    }
}

void arstring_sc_copy_without_rightmost_component_s(
        ArConstString    originalString,
        const char       separatorChar,
        ArString       * newString
        )
{
    if ( originalString )
    {
        char  * rightmostSeparatorLocation =
            strrchr( originalString, separatorChar );

        if ( rightmostSeparatorLocation != NULL )
        {
            unsigned int  newStringLength =
                  rightmostSeparatorLocation
                - originalString
                + 1;                          // <- for the terminating zero

            *newString = ALLOC_ARRAY( char, newStringLength );

            strncpy(
                *newString,
                originalString,
                newStringLength - 1
                );

            strcpy(
                *newString + newStringLength - 1,
                "\0"
                );
        }
        else
            arstring_s_copy_s(
                originalString,
                newString
                );
    }
    else
        *newString = 0;
}

void arstring_p_copy_without_extension_p(
        ArConstString    originalPathString,
        ArString       * newPathString
        )
{
    arstring_sc_copy_without_rightmost_component_s(
        originalPathString,
        '.',
        newPathString
        );
}

void arstring_sc_copy_rightmost_component_s(
        ArConstString    originalString,
        const char       separatorChar,
        ArString       * newString
        )
{
    if ( originalString )
    {
        char  * rightmostSeparatorLocation =
            strrchr( originalString, separatorChar );

        if ( rightmostSeparatorLocation != NULL )
        {
            unsigned int  newStringLength =
                  strlen(rightmostSeparatorLocation)
                - 1         // <- for the separator char
                + 1;        // <- for the terminating zero

            *newString = ALLOC_ARRAY( char, newStringLength );

            strncpy(
                *newString,
                rightmostSeparatorLocation + 1,
                newStringLength - 1
                );

            strcpy(
                *newString + newStringLength - 1,
                "\0"
                );
        }
        else
            arstring_s_copy_s(
                originalString,
                newString
                );
    }
    else
        *newString = 0;
}

void arstring_p_copy_filename_s(
        ArConstString    originalFullPathString,
        ArString       * newFilenameString
        )
{
    arstring_sc_copy_rightmost_component_s(
        originalFullPathString,
        '/',
        newFilenameString
        );
}

void arstring_cs_add_component_s(
        const char       separatorChar,
        ArConstString    extensionString,
        ArString       * resultString
        )
{
    if ( extensionString )
    {
        ArString  tempString = 0;

        if ( *resultString )
        {
            arstring_scs_copy_and_add_component_s(
                 *resultString,
                  separatorChar,
                  extensionString,
                & tempString
                );

            FREE( *resultString );
        }

        *resultString = tempString;
    }
}

void arstring_scs_copy_and_add_component_s(
        ArConstString    originalString,
        const char       separatorChar,
        ArConstString    componentString,
        ArString       * newString
        )
{
    unsigned int  originalStringLength =
        strlen( originalString );

    unsigned int  componentStringLength =
        strlen( componentString );

    unsigned int  newStringLength =
          originalStringLength
        + 1                           // <- for the separating dot
        + componentStringLength
        + 1;                          // <- for the terminating zero

    *newString = ALLOC_ARRAY( char, newStringLength );

    memcpy(
        *newString,
        originalString,
        originalStringLength * sizeof(char)
        );

    (*newString)[originalStringLength] = separatorChar;

    memcpy(
        *newString + originalStringLength + 1,
        componentString,
        componentStringLength * sizeof(char)
        );

    (*newString)[originalStringLength + 1 + componentStringLength] = '\0';
}

void arstring_pe_copy_add_extension_p(
        ArConstString    originalPathString,
        ArConstString    extensionString,
        ArString       * resultPathString
        )
{

    arstring_scs_copy_and_add_component_s(
        originalPathString,
        '.',
        extensionString,
        resultPathString
        );
}

void arstring_pd_copy_add_directory_p(
        ArConstString    originalPathString,
        ArConstString    directoryString,
        ArString       * resultPathString
        )
{

    arstring_scs_copy_and_add_component_s(
        originalPathString,
        ':',
        directoryString,
        resultPathString
        );
}

void arstring_d_add_directory_p(
        ArConstString    directoryString,
        ArString       * resultPathString
        )
{
    arstring_cs_add_component_s(
        ':',
        directoryString,
        resultPathString
        );
}

void arstring_e_add_extension_p(
        ArConstString    extensionString,
        ArString       * resultPathString
        )
{
    arstring_cs_add_component_s(
        '.',
        extensionString,
        resultPathString
        );
}

#define  arstring_sss_copy_swap_extension_s\
    arstring_sss_copy_replace_from_right_s

void arstring_sss_copy_replace_from_right_s(
        ArConstString    originalString,
        ArConstString    originalRightEndOfString,
        ArConstString    newRightEndOfString,
        ArString       * newString
        )
{
    unsigned int  originalStringLength =
        strlen( originalString );

    unsigned int  originalRightEndOfStringLength =
        strlen( originalRightEndOfString );

    unsigned int  newRightEndOfStringLength =
        strlen( newRightEndOfString );

    unsigned int  newStringLength =
          originalStringLength
        - originalRightEndOfStringLength
        + newRightEndOfStringLength
        + 1;                          // <- for the terminating zero

    *newString = ALLOC_ARRAY( char, newStringLength );

    memcpy(
        *newString,
        originalString,
        (newStringLength - originalRightEndOfStringLength) * sizeof(char)
    );

    memcpy(
        *newString + originalStringLength - originalRightEndOfStringLength,
        newRightEndOfString,
        newRightEndOfStringLength * sizeof(char)
    );

    (*newString)[newStringLength - 1] = '\0';
}

void arstring_psss_copy_add_tag_swap_extension_p(
        ArConstString    originalPathString,
        ArConstString    originalExtensionString,
        ArConstString    newTagString,
        ArConstString    newExtensionString,
        ArString       * newPathString
        )
{
    unsigned int  originalPathStringLength =
        strlen( originalPathString );

    unsigned int  originalExtensionStringLength =
        strlen( originalExtensionString );

    unsigned int  newTagStringLength =
        strlen( newTagString );

    unsigned int  newExtensionStringLength =
        strlen( newExtensionString );

    unsigned int  newPathStringLength =
          originalPathStringLength
        - originalExtensionStringLength
        + newTagStringLength
        + 1                           // <- for the separating period
        + newExtensionStringLength
        + 1;                          // <- for the terminating zero

    *newPathString = ALLOC_ARRAY( char, newPathStringLength );

    strncpy(
        *newPathString,
        originalPathString,
        originalPathStringLength - originalExtensionStringLength
        );

    strcpy(
        *newPathString + originalPathStringLength
        - originalExtensionStringLength,
        newTagString
        );

    strcpy(
        *newPathString + originalPathStringLength
        - originalExtensionStringLength + newTagStringLength,
        "."
        );

    strcpy(
        *newPathString + originalPathStringLength
        - originalExtensionStringLength + newTagStringLength + 1,
        newExtensionString
        );

    strcpy(
        *newPathString + originalPathStringLength
        - originalExtensionStringLength + newTagStringLength + 1
        + newExtensionStringLength,
        "\0"
        );
}


/* ======================================================================== */
