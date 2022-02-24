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

ART_MODULE_INTERFACE(ArnGeneralSurfaceMaterial)

#import "ART_Scenegraph.h"

/* ===========================================================================
    'ArnGeneralSurfaceMaterial'
=========================================================================== */
@interface ArnGeneralSurfaceMaterial
        : ArnNary
        < ArpConcreteClass, ArpCoding, ArpSurfaceMaterial >
{
    ArSurfaceType           globalSurfaceType;

    BOOL                    noTransmission;
    double                  diffuseWeight;
    double                  specularWeight;
}

- (id) init
        : (ArNodeRefDynArray *) newSurfaceComponents
        ;

@end

//@interface ArnGeneralSurfaceMaterial ( Visting ) < ArpVisiting > @end

ArnGeneralSurfaceMaterial * arngeneralsurfacematerial(
        ART_GV  * art_gv,
        double    firstWeight,
        ...
        );

/* ===========================================================================
    'ArpGeneralSurfaceComponent'
=========================================================================== */
@protocol ArpGeneralSurfaceComponent

- (ArNode <ArpSurfaceMaterial>*) getSurface
        ;

- (double) getWeight
        ;

- (void) setWeight
        :   (double) newWeight
        ;

@end

/* ===========================================================================
    'ArnGeneralSurfaceComponent'
=========================================================================== */
@interface ArnGeneralSurfaceComponent
        : ArnUnary < ArpConcreteClass, ArpCoding, ArpGeneralSurfaceComponent >
{
    double  weight;
}

- (id) init
        : (ArNode <ArpSurfaceMaterial>*) newSurface
        : (double) newWeight
        ;

@end

// ===========================================================================
