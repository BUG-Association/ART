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

ART_MODULE_INTERFACE(ArnCone)

#import "ArnShape.h"

@interface ArnCone
        : ArnShape
        < ArpConcreteClass, ArpHull, ArpExtremalCoordinates, ArpShape >
{
}

@end

@interface ArnConeQuadrangularMapping
        : ArNode
        < ArpConcreteClass, ArpMapping >
{
}

@end


ArnCone  * arncone_unbounded_singleton(
        ART_GV  * art_gv
        );

ArnCone  * arncone_bounded_singleton(
        ART_GV  * art_gv
        );

ArnCone  * arncone_bounded_sheet_singleton(
        ART_GV  * art_gv
        );

ArnCone  * arncone_bounded_surface_singleton(
        ART_GV  * art_gv
        );

#define ARNCONE_UNBOUNDED_SINGLETON \
        arncone_unbounded_singleton(art_gv)
#define ARNCONE_BOUNDED_SINGLETON \
        arncone_bounded_singleton(art_gv)
#define ARNCONE_BOUNDED_SHEET_SINGLETON \
        arncone_bounded_sheet_singleton(art_gv)
#define ARNCONE_BOUNDED_SURFACE_SINGLETON \
        arncone_bounded_surface_singleton(art_gv)

// ===========================================================================
