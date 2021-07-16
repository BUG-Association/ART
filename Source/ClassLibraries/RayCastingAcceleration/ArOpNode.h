/* ===========================================================================

 Copyright (c) 1996-2021 The ART Development Team
 ------------------------------------------------

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

#ifndef AR_OP_NODE
#define AR_OP_NODE

#include "ART_Foundation.h"
#include "ArIntersectionList.h"

typedef struct ArOpNode
{
    int            superNodeID;
    int            numberOfSubNodes;
    unsigned long  data;

    void (*intersectFunction)
        (int, struct ArOpNode*, ArnRayCaster*, ArIntersectionList*);
}
ArOpNode;

void intersect_sub(int, ArOpNode*, ArnRayCaster*, ArIntersectionList*);
void intersect_and(int, ArOpNode*, ArnRayCaster*, ArIntersectionList*);
void intersect_or(int, ArOpNode*, ArnRayCaster*, ArIntersectionList*);
void intersect_union(int, ArOpNode*, ArnRayCaster*, ArIntersectionList*);
void intersect_leaf(int, ArOpNode*, ArnRayCaster*, ArIntersectionList*);

void setActive(int, BOOL*, ArOpNode*);
void setInactive(int, BOOL*, ArOpNode*);


#endif
