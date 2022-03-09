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

#define ART_MODULE_NAME     ArnAssignmentTable

#include "ART_Foundation.h"
#import <stdarg.h>

#import "ArnAssignmentTable.h"
#import "ArpValue.h"
#import "ArnVisitor.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnAssignmentTable registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


ARLIST_IMPLEMENTATION_FOR_TYPE(ArAssignment,arassignment)

ArAssignmentTable arassignmenttable_v(
        ART_GV   * art_gv,
        ArNode   * firstVariable,
        ArNode   * firstValue,
        va_list    argPtr
        )
{
    (void) art_gv;
    
    ArList  assignmentList = ARLIST_EMPTY;

    ArAssignment  assignment;

    assignment.variable = firstVariable;
    assignment.value    = firstValue;

    while (   assignment.variable
           && assignment.value )
    {
        arlist_add_arassignment_at_tail(
            & assignmentList,
              assignment );

        assignment.variable = va_arg( argPtr, ArNode * );
        assignment.value    = va_arg( argPtr, ArNode * );
    }

    ArAssignmentTable  assignmentTable;

    assignmentTable.size = arlist_length( & assignmentList );

    if ( assignmentTable.size > 0 )
    {
        assignmentTable.array =
            ALLOC_ARRAY( ArAssignment, assignmentTable.size );

        for ( unsigned int i = 1; i < assignmentTable.size; i++ )
            arlist_pop_arassignment_from_head(
                & assignmentList,
                & assignmentTable.array[i]
                );
    }
    else
        assignmentTable.array = 0;

    return assignmentTable;
}

ArAssignmentTable arassignmenttable(
        ART_GV  * art_gv,
        ArNode  * firstVariable,
        ArNode  * firstValue,
        ...
        )
{
    va_list argPtr;

    va_start( argPtr, firstValue );

    ArAssignmentTable  assignmentTable =
        arassignmenttable_v(
            art_gv,
            firstVariable,
            firstValue,
            argPtr );

    va_end(argPtr);

    return assignmentTable;
}

ArAssignmentTable  arassignmenttable_copy(
        ArAssignmentTable  * table2copy
        )
{
    ArAssignmentTable  assignmentTable;

    assignmentTable.size = table2copy->size;

    if ( assignmentTable.size > 0 )
    {
        assignmentTable.array =
            ALLOC_ARRAY( ArAssignment, assignmentTable.size );

        for ( unsigned int i = 1; i < assignmentTable.size; i++ )
        {
            assignmentTable.array[i].variable =
                table2copy->array[i].variable;

            assignmentTable.array[i].value =
                table2copy->array[i].value;
        }
    }
    else
        assignmentTable.array = 0;

    return assignmentTable;
}

@implementation ArnAssignmentTable

#define ASSIGNMENT_VARIABLE   (ArNode <ArpVariable> *)assignment->variable
#define ASSIGNMENT_VALUE      (ArNode <ArpValues> *)assignment->value

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnAssignmentTable)

- (void) _allocAssignmentTable
{
    assignmentTable.array =
        ALLOC_ARRAY( ArAssignment, assignmentTable.size );
}

- (id) init
        : (ArAssignmentTable) newAssignmentTable
{
    self = [ super init ];
    
    if ( self )
    {
        assignmentTable = newAssignmentTable;
    }
    
    return self;
}

- (id) copy
{
    ArnAssignmentTable  * copiedInstance = [ super copy ];

    copiedInstance->assignmentTable =
        arassignmenttable_copy( & assignmentTable );

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnAssignmentTable  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    copiedInstance->assignmentTable =
        arassignmenttable_copy( & assignmentTable );

    return copiedInstance;
}

- (void) addContentsToEvalEnv
        : (ArcObject <ArpEvaluationEnvironment> *) evalEnv
{
    ArAssignment  * assignment;

    FOR_EACH( assignment, assignmentTable )
        [ ASSIGNMENT_VARIABLE addValueToEvalEnv
            :   ASSIGNMENT_VALUE
            :   evalEnv
            ];
}

- (void) delContentsFromEvalEnv
        : (ArcObject <ArpEvaluationEnvironment> *) evalEnv
{
    ArAssignment  * assignment;

    FOR_EACH( assignment, assignmentTable )
        [ ASSIGNMENT_VARIABLE delValueFromEvalEnv
            :   evalEnv
            ];
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];

    [ coder codeTableBegin
        :   "arassignmenttable"
        : & assignmentTable.size ];

    if ( [ coder isReading ] )
        [ self _allocAssignmentTable ];

    ArAssignment  * assignment;

    FOR_EACH( assignment, assignmentTable )
    {
        [ coder codeSubnode: & assignment->variable ];
        [ coder codeSubnode: & assignment->value ];
    }

    [ coder codeTableEnd ];
}

- (void) dealloc
{
    ArAssignment  * assignment;

    FOR_EACH( assignment, assignmentTable )
    {
        RELEASE_OBJECT( assignment->variable );
        RELEASE_OBJECT( assignment->value );
    }

    FREE_ARRAY( assignmentTable.array );

    [ super dealloc ];
}

@end

@implementation ArnAssignmentTable ( Visiting )

- (void) visit
        : (ArnVisitor *) visitor
{
    if ( [ visitor wantsToInteractWith: self ] )
    {
        if ( arnvisitor_visit_preorder(visitor) )
            [ super visit
                :   visitor ];

        if ( arnvisitor_visit_subnodes(visitor, self) )
        {
            if ( arnvisitor_visit_dag(visitor) )
            {
                ArAssignment  * assignment;

                FOR_EACH( assignment, assignmentTable )
                {
                    [ assignment->variable visit
                        :   visitor ];
                    [ assignment->value visit
                        :   visitor ];
                }
            }
        }

        if ( arnvisitor_visit_postorder(visitor) )
            [ super visit
                :   visitor ];
    }
}


- (ArNode *) modify
        : (ArnVisitor *) visitor
{
    if ( [ visitor wantsToInteractWith: self ] )
    {
        ArNode  * result = NULL;

        if ( arnvisitor_visit_preorder(visitor) )
            result =
                [ super modify
                    :   visitor ];

        if ( arnvisitor_visit_subnodes(visitor, self) )
        {
            ArAssignment * assignment;

            FOR_EACH( assignment, assignmentTable )
            {
                if ( assignment->variable )
                    assignment->variable =
                        [ assignment->variable modify
                            :   visitor ];

                if ( assignment->value )
                    assignment->value =
                        [ assignment->value modify
                            :   visitor ];

            }
        }

        if ( arnvisitor_visit_postorder(visitor) )
            result =
                [ super modify
                    :   visitor ];

        return result;
    }

    return self;
}

@end

// ===========================================================================
