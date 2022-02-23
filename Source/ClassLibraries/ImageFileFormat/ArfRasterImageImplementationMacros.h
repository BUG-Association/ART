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

/* --------------------------------------------------------------------------
    'ARFRASTERIMAGE_DEFAULT_IMPLEMENTATION' macro

    Implements those methods from 'ArpImageFile' which are always the same
    for derived classes; see any of these for an example of its use.

    Note that this macro assumes the existence of some appropriately named
    static variables in the calling module, namely the array of char *
    arf<name>_extension for the valid filetype extensions, and the long and
    short class names.

    The split into 'ARFRASTERIMAGE_DEFAULT_NCC_IMPLEMENTATION' and
    'ARFRASTERIMAGE_DEFAULT_STRING_IMPLEMENTATION' is for those classes
    (like e.g. ArfTIFF) which have to implement a more sophisticated
    'nativeContentClass' method while retaining the common string
    implementations.
-----------------------------------------------------------------------aw- */

#define ARFRASTERIMAGE_DEFAULT_NCC_IMPLEMENTATION(__contentDataType) \
\
- (Class) nativeContentClass \
{ \
    return [ Arn##__contentDataType##Image class ]; \
}

#define ARFRASTERIMAGE_DEFAULT_STRING_IMPLEMENTATION(__name) \
\
+ (const char**) extensions \
{ \
    return arf##__name##_extension; \
} \
\
+ (const char *) fileExtension \
{ \
    return arf##__name##_extension[0]; \
} \
\
- (const char *) fileExtension \
{ \
    return arf##__name##_extension[0]; \
} \
\
- (const char*) shortClassName \
{ \
    return arf##__name##_short_class_name; \
} \
\
- (const char*) longClassName \
{ \
    return arf##__name##_long_class_name; \
}

#define ARFRASTERIMAGE_DEFAULT_IMPLEMENTATION(__contentDataType,__name) \
\
ARFRASTERIMAGE_DEFAULT_NCC_IMPLEMENTATION(__contentDataType) \
ARFRASTERIMAGE_DEFAULT_STRING_IMPLEMENTATION(__name)


// ===========================================================================
