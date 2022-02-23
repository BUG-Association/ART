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

ART_MODULE_INTERFACE(ArnColourSpace)

#import "ART_Scenegraph.h"

/* ---------------------------------------------------------------------------
    'ArnColourSpace' class

    Wrapper node around an ArColourSpaceRef. Note that writing and reading
    such nodes from .arm/.arb files is problematical for all but in-built
    colour spaces, since all this wrapper node "remembers" is the name
    of the colourspace struct it contains. If this colour space has not been
    created by the time it is retrieved from file (e.g. because the colour
    space in question was created from an ICC profile which is no longer
    available) its creation will fail.

    This is not a mis-feature, however, since the correct way to include an
    external data source - such as an ICC file - in an ART scene graph is to
    use an ArnExternal reference to the original file, instead of directly
    saving the wrapper node which is created once the external is resolved.

    SHORT VERSION OF THIS: you will probably never instantiate an
    ArnColourSpace node yourself; all you should do is use externals which
    reference ICC files instead.
------------------------------------------------------------------------aw- */

@interface ArnColourSpace
        : ArNode < ArpConcreteClass, ArpColourSpace >
{
    ArColourSpaceRef  colourSpaceRef;
}

- (id) init
        : (ArColourSpaceRef) newColourSpaceRef
        ;

@end


/* ---------------------------------------------------------------------------
    'ArnDefaultRGBColourSpace' class

    Wrapper node which returns a colourspace ref to the default RGB space
    in use at the time of calling.
------------------------------------------------------------------------aw- */

@interface ArnDefaultRGBColourSpace
        : ArNode < ArpConcreteClass, ArpColourSpace >
{
}

@end


ArnDefaultRGBColourSpace  * arndefault_rgb_colourspace_singleton(
        ART_GV  * art_gv
        );

#define DEFAULT_RGB_COLOURSPACE \
    arndefault_rgb_colourspace_singleton(art_gv)

// ===========================================================================
