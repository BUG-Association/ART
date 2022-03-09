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

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArpObject)

#import "ObjC_Runtime.h"

/* ---------------------------------------------------------------------------

    'ArpObject' protocol

    conformsToArProtocol
        For some derived classes of 'ArNode', protocol conformance is
        dynamically defined on a per instance basis, so that two instances of
        the same class might not conform to the same protocols.  In order to
        account for that without changing the semantics of the standard
        'conformsToProtocol' method, this method has been introduced. The
        default implementation is to just use 'conformsToProtocol'.

    cStringClassName
        A convenient shorthand to obtain a string that contains the class
        name; this is mainly used for debug printouts and the like.

--------------------------------------------------------------------------- */


@protocol ArpObject <NSObject>

+ (BOOL) conformsToArProtocol
        : (Protocol *) protocol
        ;

- (BOOL) conformsToArProtocol
        : (Protocol *) protocol
        ;

+ (const char *) cStringClassName
        ;

- (const char *) cStringClassName
        ;

@end

// ===========================================================================
