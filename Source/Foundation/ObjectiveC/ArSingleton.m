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

#define ART_MODULE_NAME     ArSingleton

#import "ArSingleton.h"

#import "ART_Foundation_Datastructures.h"


typedef struct ArSingleton
{
    ArHashEntry      name_entry;
    ArHashEntry      object_entry;
    ArObjectCreator  object_creator;
}
ArSingleton;


typedef struct ArSingleton_GV
{
    pthread_mutex_t  mutex;
    ArHashTable      name_map;
    ArHashTable      object_map;
}
ArSingleton_GV;

#define ARSINGLETON_GV          art_gv->arsingleton_gv
#define ARSINGLETON_MUTEX       ARSINGLETON_GV->mutex
#define ARSINGLETON_NAME_MAP    ARSINGLETON_GV->name_map
#define ARSINGLETON_OBJECT_MAP  ARSINGLETON_GV->object_map

ART_MODULE_INITIALISATION_FUNCTION
(
    ARSINGLETON_GV = ALLOC(ArSingleton_GV);

    pthread_mutex_init( & ARSINGLETON_MUTEX, NULL );

    arhashtable_init( & ARSINGLETON_NAME_MAP, 255);
    arhashtable_init( & ARSINGLETON_OBJECT_MAP, 255);
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    arhashtable_free( & ARSINGLETON_NAME_MAP );
    arhashtable_free( & ARSINGLETON_OBJECT_MAP );

    pthread_mutex_destroy( & ARSINGLETON_MUTEX );

    FREE( ARSINGLETON_GV );
)

#define SINGLETON_FROM_NAMEENTRY(_ne)   \
    STRUCT_POINTER(ArSingleton,(_ne),name_entry)
#define SINGLETON_FROM_OBJECTENTRY(_oe) \
    STRUCT_POINTER(ArSingleton,(_oe),object_entry)

void arsingleton_register(
        ART_GV    * art_gv,
        ArSymbol    name,
        id          object
        )
{
    ArSingleton * singleton = ALLOC(ArSingleton);

    pthread_mutex_lock( & ARSINGLETON_MUTEX );

    singleton->name_entry.hash = (unsigned long) name;
    arhashtable_add_entry( & ARSINGLETON_NAME_MAP, &singleton->name_entry);

    singleton->object_entry.hash = (unsigned long) object;
    arhashtable_add_entry( & ARSINGLETON_OBJECT_MAP, &singleton->object_entry);

    singleton->object_creator = 0;

    pthread_mutex_unlock( & ARSINGLETON_MUTEX );
}

void arsingleton_register_creator(
        ART_GV           * art_gv,
        ArSymbol           name,
        ArObjectCreator    object_creator
        )
{
    ArSingleton * singleton = ALLOC(ArSingleton);

    pthread_mutex_lock( & ARSINGLETON_MUTEX );

    singleton->name_entry.hash = (unsigned long) name;
    arhashtable_add_entry(&ARSINGLETON_NAME_MAP, &singleton->name_entry);

    singleton->object_entry.hash = 0;

    singleton->object_creator = object_creator;

    pthread_mutex_unlock( & ARSINGLETON_MUTEX );
}

ArSymbol arsingleton_name_of_object(
        const ART_GV  * art_gv,
        const id        object
        )
{
    ArSingleton * singleton =
        SINGLETON_FROM_OBJECTENTRY(
            arhashtable_find_hash(
                & ARSINGLETON_OBJECT_MAP,
                  (unsigned long)object,
                  0
                )
            );

    if (singleton)
        return (ArSymbol)singleton->name_entry.hash;
    else
        return 0;
}

/* ---------------------------------------------------------------------------
    'object_is_a_singleton'
        Returns a non-zero value if the supplied object is a singleton object.
--------------------------------------------------------------------------- */
unsigned int object_is_a_singleton(
        const ART_GV  * art_gv,
        const id        object
        )
{
    return
        arhashtable_find_hash(
            & ARSINGLETON_OBJECT_MAP,
              (unsigned long)object,
              0
            ) != 0;
}

/* ---------------------------------------------------------------------------
    'arsingleton_has_a_creator_function'
--------------------------------------------------------------------------- */
ArObjectCreator arsingleton_has_a_creator_function(
        const ART_GV  * art_gv,
        const id        object
        )
{
    ArSingleton * singleton =
        SINGLETON_FROM_OBJECTENTRY(
            arhashtable_find_hash(
                & ARSINGLETON_OBJECT_MAP,
                  (unsigned long)object,
                  0
                )
            );

    if (singleton)
        return singleton->object_creator;
    else
        return 0;
}

//#define SINGLETON_DEBUG_PRINTF

/* ---------------------------------------------------------------------------
    'arsingleton_of_name'
        Returns the singleton object of a given name or 0 if there is no singleton
        with this name.
--------------------------------------------------------------------------- */
id arsingleton_of_name(
              ART_GV    * art_gv,
        const ArSymbol    name
        )
{
    ArSingleton * singleton =
        SINGLETON_FROM_NAMEENTRY(arhashtable_find_hash(&ARSINGLETON_NAME_MAP,
                                                  (unsigned long)name, 0));
#ifdef SINGLETON_DEBUG_PRINTF
    printf("\n singleton= %s \n",name);fflush(stdout);
#endif
    if (singleton)
    {
        ArcObject  * object = (ArcObject *)singleton->object_entry.hash;
#ifdef SINGLETON_DEBUG_PRINTF
        printf("\n S object= %s \n",[ object cStringClassName ]);fflush(stdout);
#endif
        if (! object)
        {
            object = singleton->object_creator(art_gv);
#ifdef SINGLETON_DEBUG_PRINTF
            printf("\n C object= %s 0x%x\n",[ object cStringClassName ],object);fflush(stdout);
#endif
            singleton->object_entry.hash = (unsigned long)object;

            arhashtable_add_entry(&ARSINGLETON_OBJECT_MAP,
                                  &singleton->object_entry);
        }
        return object;
    }
    else
        return 0;
}

// ===========================================================================
