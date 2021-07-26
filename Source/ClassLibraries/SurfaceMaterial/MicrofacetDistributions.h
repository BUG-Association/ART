/* ===========================================================================

 Copyright (c) 1996-2021 The ART Development Team
 -------------------------------------------

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

ART_MODULE_INTERFACE(MicrofacetDistributions)

#import "ART_Scenegraph.h"

/* ===========================================================================
    'ArnBlinnMicrofacetDistribution'
 
    Blinn's Microfacet distribution, which in Blinn's original work
    (Siggraph 1977) is designated D1.

    The roughness (for Blinn's Microfacet distribution) is
    controlled by the expression for beta, describing the
    average roughness angle (>0 (smooth)...pi/2 (rough)).

    Following Blinn's original paper (Siggraph 1977), this parameter
    describes the angle where the distribution of microfacets falls off
    to 1/2.
========================================================================mm= */

@interface ArnBlinnMicrofacetDistribution
        : ArnUnary <ArpConcreteClass, ArpMicrofacetDistribution >
{
}

- (id) init
        : (ArNode *) expressionForBeta
        ;

@end

