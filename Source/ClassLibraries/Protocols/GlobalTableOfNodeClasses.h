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

ART_MODULE_INTERFACE(GlobalTableOfNodeClasses)

#import <pthread.h>

extern pthread_mutex_t       globalTableOfNodeClasses_mutex;
extern ArClassDynArray     globalTableOfNodeClasses;

#define TOTAL_NUMBER_OF_CONCRETE_CLASSES \
    arclassdynarray_size( & globalTableOfNodeClasses )

#define CLASS_NAME_OF_CONCRETE_CLASS_I(__i) \
    [ arclassdynarray_i( & globalTableOfNodeClasses, i) cStringClassName ]

#define ARPCONCRETECLASS_RUNTIME_IMPLEMENTATION(_name) \
\
unsigned long  _name##_globalClassNumber; \
\
+ (void) registerWithRuntime \
{ \
    [ _name self ]; \
    pthread_mutex_lock( & globalTableOfNodeClasses_mutex ); \
    RUNTIME_REGISTER_CLASS( _name ); \
    _name##_globalClassNumber = \
        arclassdynarray_size( & globalTableOfNodeClasses ); \
    arclassdynarray_push( \
        & globalTableOfNodeClasses, \
          (Class) [ _name class ] ); \
    pthread_mutex_unlock( & globalTableOfNodeClasses_mutex ); \
} \
\
+ (unsigned long) globalClassNumber \
{ \
    return _name##_globalClassNumber; \
} \
\
- (unsigned long) globalClassNumber \
{ \
    return _name##_globalClassNumber; \
} \

#define ARPCONCRETECLASS_DEFAULT_INTERFACE(_name) \
extern unsigned long  _name##_globalClassNumber; \

#define ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(_name) \
        ARPCONCRETECLASS_RUNTIME_IMPLEMENTATION(_name) \

// ===========================================================================
