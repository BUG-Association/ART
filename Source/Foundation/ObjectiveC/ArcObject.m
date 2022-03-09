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

#define ART_MODULE_NAME     ArcObject

#include "ArcObject.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

#define ART_BASECLASS_IMPLEMENTATION

#ifdef ART_WITH_NSOBJECT

#import "ArcObject_derived_from_NSObject.m"

#else

#import "ArcObject_derived_from_Object.m"

#endif

#undef ART_BASECLASS_IMPLEMENTATION

ARLIST_IMPLEMENTATION_FOR_PTR_TYPE(ArcObject,obj)
ARLIST_IMPLEMENTATION_FOR_OBJECT_TYPE(ArcObject,obj)

#ifdef NEVERMORE
ARDYNARRAY_IMPLEMENTATION_FOR_TYPE_PTR(ArcObject,Obj,obj,o,0)
ARDYNARRAY_IMPLEMENTATION_FOR_OBJECT_TYPE(ArcObject,Obj,obj,o,0)
#endif

// ===========================================================================
