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

#define ART_MODULE_NAME     ArRayTree

#import "ArRayTree.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

void arraytree_init(
        ArRayTree * ray_tree
        )
{
    ray_tree->object_hash = CRC32_INITIAL_VALUE;
    ray_tree->surface_hash = CRC32_INITIAL_VALUE;
    ray_tree->normal_sum = VEC3D(0,0,0);
}

BOOL arraytree_different(
        ArRayTree * rt0,
        ArRayTree * rt1,
        double sqr_eps
        )
{
    BOOL different = (rt0->object_hash != rt1->object_hash);

    if (different)
        return different;

    different = (rt0->surface_hash != rt1->surface_hash);

    if (different)
        return different;

    return vec3d_vv_sqrdist(&rt0->normal_sum, &rt1->normal_sum) > sqr_eps;
}

// ===========================================================================
