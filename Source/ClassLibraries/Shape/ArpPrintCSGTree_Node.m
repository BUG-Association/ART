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

#define ART_MODULE_NAME     ArpPrintCSGTree_Node

#import "ArpPrintCSGTree_Node.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

#define HTRAFO3D_PLAIN_FORMAT(_form) \
            "("  _form "," _form "," _form "," \
                 _form "," _form "," _form "," \
                 _form "," _form "," _form "," \
                 _form "," _form "," _form ")"

#define DEBUGPRINTF_CURRENT_TRAFO \
    HTrafo3D  current_htrafo_fwd; \
    HTrafo3D  current_htrafo_bwd; \
\
    ArNode <ArpTrafo3D> *  trafo = \
        ARTS_TRAFO(*traversal_state); \
\
    [ trafo getHTrafo3Ds \
        : & current_htrafo_fwd \
        : & current_htrafo_bwd \
        ]; \
\
    debugprintf(HTRAFO3D_PLAIN_FORMAT("%f") " ",HTRAFO3D_H_PRINTF(current_htrafo_fwd))

@implementation ArnUnion (PrintCSGTree)

- (void) printCSGTree
        : (struct ArTraversalState *) traversal_state
{
    debugprintf("| CSG Union %u ", arnoderefdynarray_size(&subnodeRefArray));
}

@end

@implementation ArnCSGor (PrintCSGTree)

- (void) printCSGTree
        : (struct ArTraversalState *) traversal_state
{
    debugprintf("| CSG or ");
}

@end

@implementation ArnCSGand (PrintCSGTree)

- (void) printCSGTree
        : (struct ArTraversalState *) traversal_state
{
    debugprintf("| CSG and ");
}

@end

@implementation ArnCSGsub (PrintCSGTree)

- (void) printCSGTree
        : (struct ArTraversalState *) traversal_state
{
    debugprintf("| CSG sub ");
}

@end

@implementation ArnSphere (PrintCSGTree)

- (void) printCSGTree
        : (struct ArTraversalState *) traversal_state
{
    debugprintf("| Sphere ");

    DEBUGPRINTF_CURRENT_TRAFO;
}

@end

@implementation ArnParaboloid (PrintCSGTree)

- (void) printCSGTree
        : (struct ArTraversalState *) traversal_state
{
    debugprintf("| Paraboloid ");

    DEBUGPRINTF_CURRENT_TRAFO;
}

@end

@implementation ArnTorus (PrintCSGTree)

- (void) printCSGTree
        : (struct ArTraversalState *) traversal_state
{
    debugprintf("| Torus %f ",parameter);

    DEBUGPRINTF_CURRENT_TRAFO;
}

@end

@implementation ArnCube (PrintCSGTree)

- (void) printCSGTree
        : (struct ArTraversalState *) traversal_state
{
    debugprintf("| Cube ");

    DEBUGPRINTF_CURRENT_TRAFO;
}

@end

@implementation ArnCylinder (PrintCSGTree)

- (void) printCSGTree
        : (struct ArTraversalState *) traversal_state
{
    debugprintf("| Cylinder ");

    DEBUGPRINTF_CURRENT_TRAFO;
}

@end

@implementation ArnCone (PrintCSGTree)

- (void) printCSGTree
        : (struct ArTraversalState *) traversal_state
{
    debugprintf("| Cone ");

    DEBUGPRINTF_CURRENT_TRAFO;
}

@end

@implementation ArnVisitor (CSGPrint)

- (void) printCSGTree
        : ( ArNode < ArpPrintCSGTree >* ) node
{
    [ node printCSGTree
        : & self->state
        ];
}

@end

// ===========================================================================
