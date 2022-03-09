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

ART_MODULE_INTERFACE(ArSingleton)

#import "ArcObject.h"

typedef id (*ArObjectCreator)(ART_GV *);

/* ---------------------------------------------------------------------------
    'arsingleton_name_of_object'
        Returns the singleton name of the supplied object. If the supplied
        object is not a singleton object, 0 is returned.
--------------------------------------------------------------------------- */
ArSymbol arsingleton_name_of_object(
        const ART_GV  * art_gv,
        const id        object
        );

/* ---------------------------------------------------------------------------
    'arsingleton'
        Returns a non-zero value if the supplied object is a singleton object.
--------------------------------------------------------------------------- */
unsigned int object_is_a_singleton(
        const ART_GV  * art_gv,
        const id        object
        );

/* ---------------------------------------------------------------------------
    'arsingleton_has_a_creator_function'
--------------------------------------------------------------------------- */
ArObjectCreator arsingleton_has_a_creator_function(
        const ART_GV  * art_gv,
        const id        object
        );

/* ---------------------------------------------------------------------------
    'arsingleton_of_name'
        Returns the singleton object of a given name or 0 if there is no
        singleton with this name.
--------------------------------------------------------------------------- */
id arsingleton_of_name(
              ART_GV    * art_gv,
        const ArSymbol    name
        );


/* ---------------------------------------------------------------------------
    'arsingleton_register'
        Registers an object as a singleton object.
--------------------------------------------------------------------------- */
void arsingleton_register(
        ART_GV    * art_gv,
        ArSymbol    name,
        id          object
        );

/* ---------------------------------------------------------------------------
    'arsingleton_register_creator'
        Registers a function to lazily create a singleton object.
--------------------------------------------------------------------------- */
void arsingleton_register_creator(
        ART_GV           * art_gv,
        ArSymbol           name,
        ArObjectCreator    object_creator
        );

#define ARSINGLETON(_name) \
    (arsingleton_of_name(art_gv,arsymbol(art_gv,#_name)))

// ===========================================================================
