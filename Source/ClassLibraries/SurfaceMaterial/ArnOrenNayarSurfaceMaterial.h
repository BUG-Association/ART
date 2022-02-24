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

ART_MODULE_INTERFACE(ArnOrenNayarSurfaceMaterial)

#import "ART_Scenegraph.h"

//#import "ArnSurfaceMaterialConst.h"

/* ===========================================================================
    'ArnOrenNayarSurfaceMaterial'

        Microfacet-based diffuse surface reflectance according to
        the model of Oren and Nayar published at SIGGRAPH 1996.
        This class provides an implementation of the full
        (i.e. not the abbreviated version frequently also found in
        literature) model "straight from the original paper" with
        no internal speed-ups, or improvements of its realism.
        The reflective colour of the surface is determined the colour
        parameter, while the roughness is controlled by the sigma
        (i.e. roughness) parameter.

        The factor c1 found in the formulas is constant for all
        evaluations; since it only depends on sigma we can pre-
        compute and store it in the case the sigma value is constant.

        The surface is initialisd with a colour value which
        describes the non-directional object reflectance and the
        sigma (roughness) parameter given in radians. Useful values for
        sigma are between 0 (perfectly smooth diffuse surface, no
        retroreflection, completely equivalent to a Lambertian
        surface) to about 0.5-0.7 radians (a very rough diffuse surface;
        strong retroreflection).
=========================================================================== */
@interface ArnOrenNayarSurfaceMaterial
    : ArnBinary
        < ArpCoding, ArpConcreteClass, ArpSurfaceMaterial >
{
    ArSurfaceType   additionalSurfaceProperties;

    // (Potentially) precomputed data based on sigma parameter
    BOOL            isSigmaConstant;
    double          sigma2;
    double          c1;
}

- (id) init
        : (ArNode <ArpNode>*) newColour
        : (ArNode <ArpNode>*) newSigma
        ;

@end

// ===========================================================================
