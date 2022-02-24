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

#ifndef _ART_FOUNDATION_SYSTEM_ARSTRING_H_
#define _ART_FOUNDATION_SYSTEM_ARSTRING_H_

#include "ART_ModuleManagement.h"

ART_MODULE_INTERFACE(ArString)

#define ART_STRING_BOLD    "\033[1m"
#define ART_STRING_NORMAL  "\033[m"

#ifdef ART_WITHOUT_COLOURED_TERMINAL_OUTPUT

#define ART_STRINGCOL_PURPLE   ""
#define ART_STRINGCOL_MAGENTA  ""
#define ART_STRINGCOL_BLUE     ""
#define ART_STRINGCOL_CYAN     ""
#define ART_STRINGCOL_GREEN    ""
#define ART_STRINGCOL_YELLOW   ""
#define ART_STRINGCOL_ORANGE   ""
#define ART_STRINGCOL_RED      ""
#define ART_STRINGCOL_NORMAL   ""

#define ART_STRINGBGCOL_BLACK    ""
#define ART_STRINGBGCOL_RED      ""
#define ART_STRINGBGCOL_GREEN    ""
#define ART_STRINGBGCOL_ORANGE   ""
#define ART_STRINGBGCOL_BLUE     ""
#define ART_STRINGBGCOL_MAGENTA  ""
#define ART_STRINGBGCOL_CYAN     ""
#define ART_STRINGBGCOL_WHITE    ""
#define ART_STRINGBGCOL_NORMAL   ""

#else

#define ART_STRINGCOL_PURPLE   "\033[0;35m"
#define ART_STRINGCOL_MAGENTA  "\033[1;35m"
#define ART_STRINGCOL_BLUE     "\033[1;34m"
#define ART_STRINGCOL_CYAN     "\033[1;36m"
#define ART_STRINGCOL_GREEN    "\033[1;32m"
#define ART_STRINGCOL_YELLOW   "\033[1;33m"
#define ART_STRINGCOL_ORANGE   "\033[0;33m"
#define ART_STRINGCOL_RED      "\033[1;31m"
#define ART_STRINGCOL_NORMAL   "\033[0;m"

#define ART_STRINGBGCOL_BLACK    "\033[40m"
#define ART_STRINGBGCOL_RED      "\033[41m"
#define ART_STRINGBGCOL_GREEN    "\033[42m"
#define ART_STRINGBGCOL_ORANGE   "\033[43m"
#define ART_STRINGBGCOL_BLUE     "\033[44m"
#define ART_STRINGBGCOL_MAGENTA  "\033[45m"
#define ART_STRINGBGCOL_CYAN     "\033[46m"
#define ART_STRINGBGCOL_WHITE    "\033[47m"
#define ART_STRINGBGCOL_NORMAL   "\033[0;m"

#endif

/* ---------------------------------------------------------------------------
    'ArString' typedef
    'ArConstString' typedef

    'ArString' is of course synonymous with 'char *', and only typedef'ed so
    that string code becomes more legible. The semantics are exactly the same
    as for the zero-terminated standard C strings.

    The separate 'ArConstString' typedef is caused by the way C evaluates
    'const' expressions. In particular, by the fact that 'const ArString' is
    *not* the same as 'ArConstString' with respect to the "const-ness" of the
    argument, and *not* equivalent to 'const char*'.
------------------------------------------------------------------------aw- */

typedef char * ArString;
typedef const char * ArConstString;


/* ---------------------------------------------------------------------------
    'arstring_s_copy_s'
    A version of strcpy() for ArStrings.
------------------------------------------------------------------------aw- */

void arstring_s_copy_s(
        ArConstString    originalString,
        ArString       * newString
        );


/* ---------------------------------------------------------------------------
    'arstring_p_copy_filename_s'
        Starting at the end and going left from there, this function copies
        everything from the supplied full path string until the first path
        separator ('/') is found.

        This functionality amounts to copying the filename off the end of a
        given path.
------------------------------------------------------------------------aw- */

void arstring_sc_copy_rightmost_component_s(
        ArConstString    originalString,
        const char       separatorChar,
        ArString       * newString
        );

void arstring_p_copy_filename_s(
        ArConstString    originalFullPathString,
        ArString       * newFilenameString
        );


/* ---------------------------------------------------------------------------
    'arstring_p_copy_without_extension_p'
        Starting at the end and going left from there, this function strips
        everything off a given path until it encounters the first separator
        (i.e. the first period). If no period is found in the entire original
        path string, the new path is an identical copy of the old one.

        This functionality amounts to stripping an arbitrary extension off
        the end of a given path.
------------------------------------------------------------------------aw- */

void arstring_sc_copy_without_rightmost_component_s(
        ArConstString    originalString,
        const char       separatorChar,
        ArString       * newString
        );

void arstring_p_copy_without_extension_p(
        ArConstString    originalPathString,
        ArString       * newPathString
        );


/* ---------------------------------------------------------------------------
    'arstring_e_add_extension_p'
        Adds the specified string to the end of the given char array.
------------------------------------------------------------------------aw- */

void arstring_cs_add_component_s(
        const char       separatorChar,
        ArConstString    extensionString,
        ArString       * resultString
        );


/* ---------------------------------------------------------------------------
    'arstring_pe_copy_add_extension_p'
        Adds an extension at the end of a given char array.
------------------------------------------------------------------------aw- */

void arstring_scs_copy_and_add_component_s(
        ArConstString    originalString,
        const char       separatorChar,
        ArConstString    componentString,
        ArString       * newString
        );



/* ---------------------------------------------------------------------------
    'path_add_directory'
        Adds a new directory at the end of an existing path string.

        The directory has to be specified without the colon which is put
        between it and the rest of the path (e.g. as "~", not as ":~").
------------------------------------------------------------------------aw- */

void arstring_d_add_directory_p(
        ArConstString    directoryString,
        ArString       * resultPathString
        );

void arstring_e_add_extension_p(
        ArConstString    extensionString,
        ArString       * resultPathString
        );

void arstring_pd_copy_add_directory_p(
        ArConstString    originalPathString,
        ArConstString    directoryString,
        ArString       * resultPathString
        );


/* ---------------------------------------------------------------------------
    'arstring_pe_copy_add_extension_p'
        Adds an extension at the end of a path string, but does nothing if
        this extension is already at the end of the path (!).

        The extension has to be specified without the period which is put
        between it and the rest of the path (e.g. as "png", not as ".png").
------------------------------------------------------------------------aw- */

void arstring_pe_copy_add_extension_p(
        ArConstString    originalPathString,
        ArConstString    extensionString,
        ArString       * resultPathString
        );

/* ---------------------------------------------------------------------------
    'path_copy_swap_extension'
        Replaces the original extension at the end of a path string with
        a new extension.

        The extensions have to be specified without the periods which are put
        between them and the rest of the path (e.g. as "png", not as ".png").
------------------------------------------------------------------------aw- */

void arstring_sss_copy_replace_from_right_s(
        ArConstString    originalString,
        ArConstString    originalRightEndOfString,
        ArConstString    newRightEndOfString,
        ArString       * newString
        );

#define arstring_pss_copy_swap_extension_p \
    arstring_sss_copy_replace_from_right_s

/* ---------------------------------------------------------------------------
    'arstring_psss_copy_add_tag_swap_extension_p'
        Adds a tag, and replaces the original extension at the end of a path
        string with a new extension.

        As far as this function is concerned a "tag" is a period-separated
        additional string which is inserted in a filename between the actual
        filename and the real extension - as e.g. in "MyImage.new.tiff", where
        "new" would be a tag used to differentiate it from the original
        "MyImage.tiff".

        Similar to above, both the extensions and the tag have to be specified
        without any of the periods which are put between them and the rest of
        the path.
------------------------------------------------------------------------aw- */

void arstring_psss_copy_add_tag_swap_extension_p(
        ArConstString    originalPathString,
        ArConstString    originalExtensionString,
        ArConstString    newTagString,
        ArConstString    newExtensionString,
        ArString       * newPathString
        );


#endif /* _ART_FOUNDATION_SYSTEM_ARSTRING_H_ */
/* ======================================================================== */
