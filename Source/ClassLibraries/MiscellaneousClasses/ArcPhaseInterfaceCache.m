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

#define ART_MODULE_NAME     ArcPhaseInterfaceCache

#import "ArcPhaseInterfaceCache.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

//   A table of ArcPhaseInterfaceIsotropic pointers

//   We need a key datastructure, as we will be searching for entries
//   based on the two material pointers. The material IORs would be
//   more thorough (in the sense that using them as key would weed out all
//   duplicates), but also much slower to check for. So pointers it is.

typedef struct ArPhaseInterfaceCacheKey
{
    ArNode <ArpVolumeMaterial>  * fromMaterial;
    ArNode <ArpVolumeMaterial>  * intoMaterial;
}
ArPhaseInterfaceCacheKey;

//   Entries are identical iff the two material pointers match.

int compare_phase_interface_cache_key(
        const ArPhaseInterfaceCacheKey  * entry1,
        const ArPhaseInterfaceCacheKey  * entry2
        )
{
    if (    entry1->fromMaterial == entry2->fromMaterial
         && entry1->intoMaterial == entry2->intoMaterial )
        return 0;
    else
        return -1;
}

//   Hash value: bitwise XOR of the two material pointers. Probably as
//   good as it gets, in this case.

unsigned long phase_interface_cache_key_hash(
        const ArPhaseInterfaceCacheKey  * entryToHash
        )
{
    return
           (unsigned long) entryToHash->fromMaterial
        ^ ~(unsigned long) entryToHash->intoMaterial;
}

ARTABLE_IMPLEMENTATION_KEYTYPE_COMPARISON_BY_FUNCTION(
        ArPhaseInterfaceCacheKey *,
        phase_interface_cache_key,
        compare_phase_interface_cache_key
        )

ARTABLE_IMPLEMENTATION_HASHING_FUNCTION_FOR_KEYTYPE(
        ArPhaseInterfaceCacheKey *,
        phase_interface_cache_key,
        phase_interface_cache_key_hash
        )

ARTABLE_DSINTERFACE_FOR_TYPE(
        ArcPhaseInterfaceIsotropic *,
        phase_interface,
        ArPhaseInterfaceCacheKey *,
        phase_interface_cache_key
        )

ARTABLE_IMPLEMENTATION_FOR_TYPE(
        ArcPhaseInterfaceIsotropic *,
        phase_interface,
        ArPhaseInterfaceCacheKey *,
        phase_interface_cache_key
        )

// What you see here is a manual re-implementation of stuff the
// ARTABLE_ macros should provide. Until they are fixed,
// the manual implementation has to remain in place.

// TL;DR; artable_get_phase_interface_with_key provided by the macros is
//        buggy, artable_get_phase_interface_with_key2 is a fixed version

typedef struct ArPhaseInterfaceCacheHashEntry
{
    ArHashEntry                   entry;
    ArPhaseInterfaceCacheKey      key;
    ArcPhaseInterfaceIsotropic  * data;
}
ArPhaseInterfaceCacheHashEntry;


ArPhaseInterfaceCacheKey * artable_iterate_phase_interface_with_key(
        ArTable                   * table,
        ArPhaseInterfaceCacheKey  * previousKey
        )
{
    ArHashEntry * hashEntry = NULL;

    if ( previousKey )
    {
        unsigned long hash =
            phase_interface_cache_key_hash( previousKey );

        hashEntry =
            arhashtable_find_hash(
                (ArHashTable *) table,
                hash,
                ((void*)0)
                );

        int isFinished = 0;

        while ( isFinished == 0 && hashEntry )
        {
            ArPhaseInterfaceCacheHashEntry
                * typedHashEntry =
                STRUCT_POINTER(
                    ArPhaseInterfaceCacheHashEntry,
                    hashEntry,
                    entry
                    );

            ArPhaseInterfaceCacheKey * storedKey = &typedHashEntry->key;

            if ( artable_compare_phase_interface_cache_key(previousKey,storedKey) == 0 )
            {
                isFinished = 1;
            }
            else
            {
                hashEntry =
                    arhashtable_find_hash(
                        (ArHashTable *) table,
                        hash,
                        hashEntry
                        );
            }
        }
    }

    hashEntry =
        arhashtable_next_entry(
            (ArHashTable *) table,
            hashEntry
            );

    if ( hashEntry )
    {
        ArPhaseInterfaceCacheHashEntry
            * typedHashEntry =
            STRUCT_POINTER(
                ArPhaseInterfaceCacheHashEntry,
                hashEntry,
                entry
                );

        if ( typedHashEntry )
        {
            ArPhaseInterfaceCacheKey
                * storedKey = & typedHashEntry->key;

            if ( storedKey )
            {
                return storedKey;
            }
        }

        hashEntry = arhashtable_next_entry( (ArHashTable *) table, hashEntry );

    }

    return NULL;
}

ArcPhaseInterfaceIsotropic ** artable_get_phase_interface_with_key2(
          ArTable                   * table,
    const ArPhaseInterfaceCacheKey  * key
    )
{
    unsigned long hash = phase_interface_cache_key_hash(key);

    ArHashEntry * hashEntry =
        arhashtable_find_hash( (ArHashTable *) table, hash, ((void*)0) );

    while ( hashEntry )
    {
        ArPhaseInterfaceCacheHashEntry
            * typedHashEntry = (ArPhaseInterfaceCacheHashEntry*)hashEntry;

        if ( artable_compare_phase_interface_cache_key(key,&typedHashEntry->key) == 0 )
        {
            return &(typedHashEntry->data);
        }

        hashEntry = arhashtable_find_hash( (ArHashTable *) table, hash, hashEntry );
    }

    return 0;
}

void artable_add_phase_interface_with_key2(
    ArTable                    * table,
    ArcPhaseInterfaceIsotropic * newData,
    ArPhaseInterfaceCacheKey   * key
    )
{
    ArcPhaseInterfaceIsotropic ** existingEntry =
        artable_get_phase_interface_with_key2( table, key );

    if ( existingEntry )
    {
        RELEASE_OBJECT(*existingEntry);
        (*existingEntry) = newData;
        return;
    }
    else
    {
        ArPhaseInterfaceCacheHashEntry * hashEntry =
            ALLOC(ArPhaseInterfaceCacheHashEntry);

        hashEntry->entry.hash = phase_interface_cache_key_hash(key);
        hashEntry->key = *key;
        hashEntry->data = newData;

        arhashtable_add_entry( table, (ArHashEntry*) hashEntry );

        return;
    }
}

@implementation ArcPhaseInterfaceCache

- (id) init
{
    self = [ super init ];

    if ( self )
    {
        artable_init( & cache );
    }
    
    return self;
}

- (ArcPhaseInterfaceIsotropic *) getPhaseInterfaceForMaterials
    : (ArNode <ArpVolumeMaterial> *) fromMaterial
    : (ArNode <ArpVolumeMaterial> *) intoMaterial
{
    //   Assemble the key

    ArPhaseInterfaceCacheKey  key;

    key.fromMaterial = fromMaterial;
    key.intoMaterial = intoMaterial;

    if ( fromMaterial == 0 && intoMaterial == 0 )
        ART_ERRORHANDLING_FATAL_ERROR("double null");

    //   Table lookup if we find an entry

    ArcPhaseInterfaceIsotropic  ** interface_ptr =
        artable_get_phase_interface_with_key2(
            & cache,
            & key
            );

    ArcPhaseInterfaceIsotropic  * interface = NULL;

    //   If a non-null pointer was returned, use its contents (!)

    if ( interface_ptr )
    {
        interface = *interface_ptr;
    }
    else
    {
        //   And if nothing was found, create a new cache entry

        
//   #define the following to see debug printfs for all phase interfaces
//   which get created. As each rendering thread has its own cache, run
//   artist -j1 to see meaningful output

#ifdef PHASEINTERFACE_DEBUG_OUTPUT
        ArSpectrum  *temp = spc_alloc( art_gv );

        double  fromIOR, intoIOR;
        
        [ (( ArNode <ArpSpectrum> *) [fromMaterial subnodeWithIndex : 0])
            getSpectrum
            :   0
            :   temp
            ];
        
        fromIOR =
            spc_sd_value_at_wavelength(
                  art_gv,
                  temp,
                  550 NM
                );

        [ (( ArNode <ArpSpectrum> *) [intoMaterial subnodeWithIndex : 0])
            getSpectrum
            :   0
            :   temp
            ];
        
        intoIOR =
            spc_sd_value_at_wavelength(
                  art_gv,
                  temp,
                  550 NM
                );

        spc_free(art_gv, temp);
#endif

        interface =
            [ ALLOC_INIT_OBJECT(ArcPhaseInterfaceIsotropic)
                :   fromMaterial
                :   intoMaterial
                ];

#ifdef PHASEINTERFACE_DEBUG_OUTPUT
        debugprintf("\nphase interface: created %f / %f\n",fromIOR,intoIOR)
        debugprintf("                %p / %p - %p\n",fromMaterial,intoMaterial,interface)
#endif

        artable_add_phase_interface_with_key2(
            & cache,
              interface,
            & key
            );
    }

    return interface;
}

- (void) dealloc
{
    ArPhaseInterfaceCacheKey  * previous_key_to_free = NULL;

    do
    {
        ArPhaseInterfaceCacheKey  * key_to_free;

        key_to_free =
            artable_iterate_phase_interface_with_key(
                & cache,
                  previous_key_to_free
                );

        if ( key_to_free )
        {
            ArcPhaseInterfaceIsotropic  ** phaseInterface_ptr =
                artable_get_phase_interface_with_key2(
                    & cache,
                      key_to_free
                    );

            if ( *phaseInterface_ptr )
                RELEASE_OBJECT(*phaseInterface_ptr);
        }

        previous_key_to_free = key_to_free;

    } while ( previous_key_to_free );

    artable_free_contents( & cache );

    [ super dealloc ];
}



@end

// ===========================================================================
