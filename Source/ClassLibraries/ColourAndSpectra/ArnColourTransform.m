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

#define ART_MODULE_NAME     ArnColourTransform

#import "ArnColourTransform.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnColourTransform registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArnColourTransform

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnColourTransform)

- (id) init
        : (ArRenderingIntent)  newRenderingIntent
{
    self = [ super init ];

    if ( self )
    {
        renderingIntent = newRenderingIntent;
    }
    
    return self;
}

- (void) setSourceAndDestinationColourspace
        : (ArColourSpace const *) source
        : (ArColourSpace const *) destination
{
    transformRef =
        cmsCreateTransform(
              ARCSR_PROFILE(source),
              TYPE_Lab_DBL,
              ARCSR_PROFILE(destination),
              TYPE_RGB_DBL,
              renderingIntent,
              0
              );
}

- (ArColourTransformRef) transformRef
{
    return transformRef;
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];

    [ coder codeUInt: ( ( unsigned int* ) & renderingIntent ) ];
}

@end

// ===========================================================================
