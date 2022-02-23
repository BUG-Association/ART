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

#define ART_MODULE_NAME ArnVolumeDataConst

#import "ArnVolumeDataConst.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
     [ ArnVolumeDataConst registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

@implementation ArnVolumeDataConst

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnVolumeDataConst)

- (id) copy
{
    ArnVolumeDataConst  * copiedInstance = [ super copy ];

    copiedInstance->_value = _value;
    copiedInstance->_min = _min;
    copiedInstance->_max = _max;
    copiedInstance->_isBounded = _isBounded;

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnVolumeDataConst  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    copiedInstance->_value = _value;
    copiedInstance->_min = _min;
    copiedInstance->_max = _max;
    copiedInstance->_isBounded = _isBounded;

    return copiedInstance;
}


// TODO:
//  - Use a proper spectrum instead of a single double
//  - More ARTistic implementation way 
    
- (id) init
        : (double) value
{
    ArNodeRefDynArray array = arnoderefdynarray_init(0);
    self = [ super init : &array];
    arnoderefdynarray_free_contents(&array);

    _value = value;
    _isBounded = NO;
    
    return self;
}

- (id) init
// TODO    : (ArNode <ArpSpectrum> *) value
        : (double) value
        : (Pnt3D) min
        : (Pnt3D) max
{
    ArNodeRefDynArray array = arnoderefdynarray_init(0);
    self = [ super init : &array];
    arnoderefdynarray_free_contents(&array);
    
    if ( self )
    {
        _value = value;
        _isBounded = YES;
        
        _min = min;
        _max = max;
    }
    
    return self;
}

- (void) max
    : (double *) value
{
    *value = _value;
}

- (BOOL) rayIntersect
        : (const Ray3D*) ray
        : (double *) near
        : (double *) far
{
    // Should not be copied pasted from GridVolume.
    // TODO: Make a superclass with those functions
    *near = -MATH_HUGE_DOUBLE;
    *far  =  MATH_HUGE_DOUBLE;
    
    if (!_isBounded) return YES;
    
    const double minP[3] = { XC(_min), YC(_min), ZC(_min) };
    const double maxP[3] = { XC(_max), YC(_max), ZC(_max) };
    
    const double origin[3]    = { XC(ray->point) , YC(ray->point) , ZC(ray->point)  };
    const double direction[3] = { XC(ray->vector), YC(ray->vector), ZC(ray->vector) };
    
    for (int i = 0; i < 3; i++) {
        const double minVal = minP[i], maxVal = maxP[i];
        
        if (direction[i] == 0) {
            // The ray is parallel to the planes
            if (origin[i] < minVal || origin[i] > maxVal) {
                return NO;
            }
        } else {
            // Calculate intersection distances
            double t1 = (minVal - origin[i]) / direction[i];
            double t2 = (maxVal - origin[i]) / direction[i];
            
            if (t1 > t2) {
                // swap t1, t2
                double tmp = t1; t1 = t2; t2 = tmp;
            }
            
            *near = MAX(t1, *near);
            *far  = MIN(t2, *far);
            
            if (!(*near <= *far)) {
                return NO;
            }
        }
    }
    
    return YES;
}

- (BOOL) isInside
       : (const Pnt3D *) point_wordspace
{
    if (_isBounded) {
        return
            XC(*point_wordspace) >= XC(_min) && XC(*point_wordspace) < XC(_max) &&
            YC(*point_wordspace) >= YC(_min) && YC(*point_wordspace) < YC(_max) &&
            ZC(*point_wordspace) >= ZC(_min) && ZC(*point_wordspace) < ZC(_max);
    }
    
    return YES;
}

- (void) lookup
        : (const Pnt3D *) point_wordspace
        : (     double *) value
{
    if (!_isBounded || [self isInside:point_wordspace]) {
        *value = _value;
    } else {
        *value = 0;
    }
}

- (int) dimensions
{
    // TODO
    return 1;
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];

    [ coder codeDouble: & _value ];
    [ coder codePnt3D: & _min ];
    [ coder codePnt3D: & _max ];
    [ coder codeBOOL: & _isBounded ];
}

@end
    
// ===========================================================================
