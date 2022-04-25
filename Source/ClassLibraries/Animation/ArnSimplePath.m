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

#define ART_MODULE_NAME     ArnSimplePath

#import "ArnSimplePath.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnSimplePath registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArnSimplePath

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnSimplePath)

- (void) setStartTranslation
        : (double) timePoint
        : (Vec3D)  translation
{
    startTime = timePoint;
    startTranslation = translation;
}

- (void) setEndTranslation
        : (double) timePoint
        : (Vec3D)  translation
{
    endTime = timePoint;
    endTranslation = translation;
}

- (id) init
        : (double) newStartTime
        : (Vec3D ) newStartTranslation
        : (double) newEndTime
        : (Vec3D ) newEndTranslation
{
    self = [ super init ];

    if (self)
    {
        self->startTime = newStartTime;
        self->startTranslation = newStartTranslation;
        self->endTime = newEndTime;
        self->endTranslation = newEndTranslation;
    }
    return self;
}

- (void) getTransform
        : (double    ) time
        : (HTrafo3D *) transform
{
    double transformInterval = endTime - startTime;
    double offsetTime = time - startTime;
    double unitTime = offsetTime / transformInterval;
    
    if (unitTime < 0) unitTime = 0;
    else if (unitTime > 1) unitTime = 1;
    
    Vec3D interpolation;
    vec3d_vv_sub_v(&endTranslation, &startTranslation, &interpolation);
    vec3d_dv_mul_v(unitTime, &interpolation, &interpolation);
    vec3d_vv_add_v(&startTranslation, &interpolation, &interpolation);
    
    *transform = HTRAFO3D_UNIT;
    XC(*transform) = XC(interpolation);
    YC(*transform) = YC(interpolation);
    ZC(*transform) = ZC(interpolation);
}

- (void) code
        : (ArcObject<ArpCoder> *) coder
{
    [ super code : coder ];

    [ coder codeDouble : &startTime ];
    [ coder codeVec3D  : &startTranslation ];
    [ coder codeDouble : &endTime ];
    [ coder codeVec3D  : &endTranslation ];
}
 
@end

// ===========================================================================
