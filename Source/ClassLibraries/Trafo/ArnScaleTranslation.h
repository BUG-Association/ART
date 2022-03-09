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

ART_MODULE_INTERFACE(ArnScaleTranslation)

#import "ART_Scenegraph.h"

#import "ArnTrafo.h"

/* ===========================================================================
    'ArnScale3D'
=========================================================================== */

@interface ArnScale3D
        : ArnTrafo
        < ArpConcreteClass, ArpCoding, ArpTrafo3D >
{
    Scale3D  scale;
    Scale3D  invScale;
}

- (id) init
        ;

- (id) init
        : (Scale3D) newScale
        ;

- (id) initFrom
        : (const Scale3D *) newScale
        ;

@end

/* ===========================================================================
    'ArnTranslation3D'
=========================================================================== */

@interface ArnTranslation3D
        : ArnTrafo
        < ArpConcreteClass, ArpCoding, ArpTrafo3D >
{
    Translation3D  translation;
}

- (id) init
        ;

- (id) init
        : (Translation3D) newTranslation
        ;

- (id) initFrom
        : (const Translation3D *) newTranslation
        ;

@end

/* ===========================================================================
    'ArnScale2D'
=========================================================================== */

@interface ArnScale2D
        : ArnTrafo
        < ArpConcreteClass, ArpCoding, ArpTrafo2D >
{
    Scale2D  scale;
    Scale2D  invScale;
}

- (id) init
        ;

- (id) init
        : (Scale2D) newScale
        ;

- (id) initFrom
        : (const Scale2D *) newScale
        ;

@end

/* ===========================================================================
    'ArnTranslation2D'
=========================================================================== */

@interface ArnTranslation2D
        : ArnTrafo
        < ArpConcreteClass, ArpCoding, ArpTrafo2D >
{
    Translation2D  translation;
}

- (id) init
        ;

- (id) init
        : (Translation2D) newTranslation
        ;

- (id) initFrom
        : (const Translation2D *) newTranslation
        ;

@end

/* ===========================================================================
    'ArnVarScale3D'
=========================================================================== */

@interface ArnVarScale3D
        : ArnTernaryTrafo
        < ArpConcreteClass, ArpVarTrafo3D >
{
}

- (id) init
        : (ArNode <ArpDoubleValues> *) xScale
        : (ArNode <ArpDoubleValues> *) yScale
        : (ArNode <ArpDoubleValues> *) zScale
        ;

@end

/* ===========================================================================
    'ArnVarTranslation3D'
=========================================================================== */

@interface ArnVarTranslation3D
        : ArnTernaryTrafo
        < ArpConcreteClass, ArpVarTrafo3D >
{
}

- (id) init
        : (ArNode <ArpDoubleValues> *) xTranslation
        : (ArNode <ArpDoubleValues> *) yTranslation
        : (ArNode <ArpDoubleValues> *) zTranslation
        ;

@end

// ===========================================================================
