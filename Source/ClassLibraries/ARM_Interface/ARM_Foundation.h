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
 * @file ARM_Foundation.h
 * @brief Foundation
 * @type Foundation
 */

#import "ART_Foundation.h"

ART_MODULE_INTERFACE(ARM_Foundation)

@class ArNode;

//   This typedef is mainly here to make the porting of .arm files that were
//   used with ART 1.x easier. ARM files can and should use "id" as
//   variable type for all nodes they instantiate.

typedef ArNode *  ArObj;

/**
 * @section Random generators
 * @type RandomGenerator
 */

/**
 * @def HALTON_SEQUENCE
 * @brief Halton sequence
 */
#define HALTON_SEQUENCE             arrandomvaluegeneration_halton

/**
 * @def RANDOM_SEQUENCE
 * @brief Random sequence
 */
#define RANDOM_SEQUENCE             arrandomvaluegeneration_nr2

/**
 * @def EFFICIENT_SEQUENCE
 * @brief Efficient sequence
 */
#define EFFICIENT_SEQUENCE          arrandomvaluegeneration_efficient

/**
 * @def PSEUDORANDOM_SEQUENCE
 * @brief Pseudorandom sequence
 */
#define PSEUDORANDOM_SEQUENCE       HALTON_SEQUENCE

/**
 * @section Data structures
 */

    /**
     * @brief Point 2D
     * @type Pnt2D
     * Creates a 2D point.
     * @def PNT2D(x, y)
     * @param x double  Value for x axis.
     * @param y double  Value for y axis.
     */

    /**
     * @brief Vector 2D
     * @type Vec2D
     * Creates a 2D vector.
     * @def VEC2D(x, y)
     * @param x double  Value for x axis.
     * @param y double  Value for y axis.
     *
     * @def IVEC2D(x, y)
     * @param x int     Value fox x axis.
     * @param y int     Value for y axis.
     */


    /**
     * @brief Point 3D
     * @type Pnt3D
     * Creates a 3D point.
     * @def PNT3D(x, y, z)
     * @param x double  Value for x axis.
     * @param y double  Value for y axis.
     * @param z double  Value for z axis.
     */

    /**
     * @brief Vector 3D
     * @type Vec3D
     * Creates a 3D vector.
     * @def VEC3D(x, y, z)
     * @param x double  Value for x axis.
     * @param y double  Value for y axis.
     * @param z double  Value for z axis.
     */

    /**
     * @brief Ray 3D
     * @type Ray3D
     * Creates a 3D ray.
     * @def RAY3D(point, direction)
     * @param point     Pnt3D   Origin of the ray.
     * @param direciton Vec3D   Direction of the ray.
     */


/* ---------------------------------------------------------------------------

    Important note on the ..._AUTORELEASE version of the constructor macros:
    ------------------------------------------------------------------------

    Code within the ART libraries should NEVER use the ..._AUTORELEASE
    version of any of the constructor macros. They are solely intended for
    use in ARM scene description files, and using them in regular ART
    library routines will almost certainly cause memory management issues.

    The logic behind this is as follows: while all ART threads have an
    autorelease pool, it should not be used outside the processing of ARM
    files. For performance reasons, these pools are only guaranteed to be
    deallocated once the thread in question terminates. This might lead to
    huge memory requirements if you autorelease things inside, say, a
    rendering loop.

------------------------------------------------------------------------aw- */

#define AUTORELEASE_OBJECT(_object) \
    [ ((id)(_object)) autorelease ]

#define ALLOC_OBJECT_AUTORELEASE(_class) \
    ( (_class *) AUTORELEASE_OBJECT(ALLOC_OBJECT(_class)) )

#define ALLOC_INIT_OBJECT_AUTORELEASE(_class) \
    ALLOC_OBJECT_AUTORELEASE(_class) init

#ifndef REPORTER
#define REPORTER \
    ALLOC_INIT_OBJECT_AUTORELEASE(ArcReporter)
#endif
#define FILE_REPORTER \
    ALLOC_INIT_OBJECT_AUTORELEASE(ArcFileReporter)
#define SHORT_FILE_REPORTER \
    ALLOC_INIT_OBJECT_AUTORELEASE(ArcShortFileReporter)

#define FLAG_OPTION \
    ALLOC_INIT_OBJECT_AUTORELEASE(ArcOptionFlag)
#define STRING_OPTION \
    ALLOC_INIT_OBJECT_AUTORELEASE(ArcOptionString)
#define INTEGER_OPTION \
    ALLOC_INIT_OBJECT_AUTORELEASE(ArcOptionInteger)
#define FLOAT_OPTION \
    ALLOC_INIT_OBJECT_AUTORELEASE(ArcOptionDouble)
#define OPTION_CATEGORY_TITLE \
    ALLOC_INIT_OBJECT_AUTORELEASE(ArcOptionCategoryTitle)

// ===========================================================================
