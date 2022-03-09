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

#include "ART_Foundation.h"

ART_MODULE_INTERFACE(ArnAssignmentTable)

#import "ArNode.h"
#import "ArpAssignmentTable.h"

typedef struct ArAssignment
{
    ArNode  * variable;
    ArNode  * value;
}
ArAssignment;

ARLIST_INTERFACE_FOR_TYPE(ArAssignment,arassignment)

typedef struct ArAssignmentTable
{
    ArAssignment   * array;
    unsigned int     size;
}
ArAssignmentTable;

@interface ArnAssignmentTable
        : ArNode
        < ArpConcreteClass, ArpCoding, ArpAssignmentTable >
{
    ArAssignmentTable  assignmentTable;
}

- (id) init
        : (ArAssignmentTable) newAssignmentTable
        ;

@end

ArAssignmentTable  arassignmenttable_copy(
        ArAssignmentTable  * table2copy
        );

ArAssignmentTable arassignmenttable_v(
        ART_GV   * art_gv,
        ArNode   * firstVariable,
        ArNode   * firstValue,
        va_list    argPtr
        );

ArAssignmentTable arassignmenttable(
        ART_GV  * art_gv,
        ArNode  * firstVariable,
        ArNode  * firstValue,
        ...
        );

// ===========================================================================
