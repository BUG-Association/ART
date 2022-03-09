/* ===========================================================================

    Copyright (c) 1996-2022 The ART Development Team
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

#include "ART_Foundation.h"

#import "ArpPath.h"

ART_MODULE_INTERFACE(ArcSimplePath)

// I'm not sure if this should be a generic ArcObject or an ArNode that could
// be used in the scenegraph. Right now, I'm using it as a simple generic
// object, so it's that, but it's likely that it would be wiser to have it
// directly in the scenegraph.

@interface ArcSimplePath
        : ArcObject
        < ArpPath >
{
@public
    double startTime;
    Vec3D startTranslation;
    
    double endTime;
    Vec3D endTranslation;
}
    

- (void) setStartTranslation
        : (double) timePoint
        : (Vec3D) translation
        ;

- (void) setEndTranslation
        : (double) timePoint
        : (Vec3D) translation
        ;

- (id) init
        : (double) startTime
        : (Vec3D ) startTranslation
        : (double) endTime
        : (Vec3D ) endTranslation
        ;

@end

// ===========================================================================
