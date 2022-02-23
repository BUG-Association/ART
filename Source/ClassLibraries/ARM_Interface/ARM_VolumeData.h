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
 * @file ARM_VolumeData.h
 * @brief Volumes
 * @type Volume
 */

#import "ARM_Foundation.h"

ART_MODULE_INTERFACE(ARM_VolumeData)

#import "ART_VolumeData.h"

/**
 * @brief Constant bounded volume
 *
 * A constant volume filling the whole world
 *
 * @def CONST_VOLUME(value)
 *
 * @param value      double  The density value for the volume.
 */
#define CONST_VOLUME(_value)                            \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnVolumeDataConst) \
        : (_value)                                      \
    ]

/**
 * @brief Constant bounded volume
 *
 * A constant volume within a specific area
 *
 * @artist Medium.arm -DHETEROGENEOUS -DUSE_NON_FLUO -DREGULAR_SCATTERING=0.8 -DALG_MIS -DPLAIN_BOX -DFLOATING_CUBE
 *
 * @def CONST_VOLUME_BOUNDED(value, ptMin, ptMax)
 *
 * @param value      double  The density value for the volume.
 * @param ptMin      Pnt3D   One of the boundaries of the volume.
 * @param ptMax      Pnt3D   The second boundary of the volume.
 *
 */
#define CONST_VOLUME_BOUNDED(_value, _ptMin, _ptMax)    \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnVolumeDataConst) \
        : (_value)                                      \
        : (_ptMin)                                      \
        : (_ptMax)                                      \
    ]

/**
 * @brief Grid Volume
 *
 * A volume where data are stored in a VOL file. The specification of VOL files
 * is available in Mitsuba documenation.
 *
 * @artist Medium.arm -DHETEROGENEOUS -DUSE_NON_FLUO -DREGULAR_SCATTERING=0.8 -DALG_MIS -DPLAIN_BOX -DHETERO_SCALE=20
 *
 * @def GRID_VOLUME(filename)
 *
 * @param filename      char*   The path where the volume is.
 *
 */
#define GRID_VOLUME(_filename)                     \
    ART_PARSE_EXISTING_FILE(                       \
        (_filename),                               \
        ArnVolumeDataGrid                          \
    )


// ===========================================================================
