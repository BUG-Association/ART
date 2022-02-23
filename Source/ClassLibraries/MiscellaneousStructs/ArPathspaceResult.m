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

#define ART_MODULE_NAME     ArPathspaceResult

#import "ArPathspaceResult.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


ArPathspaceResult * arpathspaceresult_alloc(
        ART_GV  * art_gv
        )
{
    ArPathspaceResult  * result = ALLOC(ArPathspaceResult);

    ARPATHSPACERESULT_LIGHTALPHASAMPLE(*result) =
        arlightalphasample_alloc( art_gv );

    ARPATHSPACERESULT_NEXT(*result) = NULL;

    return result;
}

void arpathspaceresult_free(
        ART_GV             * art_gv,
        ArPathspaceResult  * result
        )
{
    arlightalphasample_free(
          art_gv,
          ARPATHSPACERESULT_LIGHTALPHASAMPLE(*result)
        );

    FREE(result);
}

void arpathspaceresult_free_to_freelist(
        ART_GV             * art_gv,
        ArFreelist         * freelist,
        ArPathspaceResult  * result
        )
{
    if ( ARPATHSPACERESULT_NEXT(*result) )
    {
        arpathspaceresult_free_to_freelist(
              art_gv,
              freelist,
              ARPATHSPACERESULT_NEXT(*result)
            );
        
        ARPATHSPACERESULT_NEXT(*result) = NULL;
    }

    arfreelist_push(freelist, result);
}

void arpathspaceresult_push(
        ART_GV             * art_gv,
        ArPathspaceResult  * result,
        ArPathspaceResult  * new_result
        )
{
    (void) art_gv;
    
    ARPATHSPACERESULT_NEXT(*new_result) =
        ARPATHSPACERESULT_NEXT(*result);
    ARPATHSPACERESULT_NEXT(*result) =
        ARPATHSPACERESULT_NEXT(*new_result);
}

int arpathspaceresult_r_valid(
        ART_GV             * art_gv,
        ArPathspaceResult  * result
        )
{
    int  valid = true;
    
    if ( ! arlightalphasample_l_valid(
               art_gv,
               ARPATHSPACERESULT_LIGHTALPHASAMPLE( *result )
               ))
        valid = false;
    
    if ( ARPATHSPACERESULT_TYPE(*result) == arpathspaceresult_lightpath )
    {
        if ( ! ray3d_r_valid( & ARPATHSPACERESULT_RAY( *result ) ))
            valid = false;
    }
    
    return valid;
}


// ===========================================================================
