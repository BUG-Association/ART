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

#define ARTABLE_IMPLEMENTATION_KEYTYPE_COMPARISON_BY_FUNCTION( \
        _KeyType, \
        _keytype, \
        _comparisonFunction \
        ) \
\
int artable_compare_##_keytype( \
        const _KeyType  key1, \
        const _KeyType  key2  \
        ) \
{ \
    return _comparisonFunction( key1, key2 ); \
}

#define ARTABLE_IMPLEMENTATION_HASHING_FUNCTION_FOR_KEYTYPE( \
        _KeyType, \
        _keytype, \
        _hashingFunc \
        ) \
\
unsigned long get_##_keytype##_hash( \
        const _KeyType  dataToHash \
        ) \
{ \
    return _hashingFunc(dataToHash); \
}

#define _ARTABLE_IMPLEMENTATION_FOR_TYPE( \
    _DataTypeName, \
    _DataType, \
    _datatype, \
    _KeyTypeName, \
    _KeyType, \
    _keytype ) \
\
_DataType * artable_get_##_datatype##_with_key_##_keytype( \
        ArTable         * table, \
        const _KeyType    key \
        ) \
{ \
    unsigned long hash = get_##_keytype##_hash(key); \
    ArHashEntry  * hashEntry = \
        arhashtable_find_hash( (ArHashTable *) table, hash, NULL ); \
    \
    while ( hashEntry ) \
    { \
        _KeyTypeName##_DataTypeName##HashEntry * typedHashEntry = \
            STRUCT_POINTER( \
                _KeyTypeName##_DataTypeName##HashEntry, \
                hashEntry, \
                entry ); \
        _KeyType  storedKey = typedHashEntry->key; \
        \
        if ( artable_compare_##_keytype(key,storedKey) == 0 ) \
        { \
            return &(typedHashEntry->data); \
        } \
        hashEntry = \
            arhashtable_find_hash( \
                (ArHashTable *) table, \
                hash, \
                hashEntry ); \
    } \
    return 0; \
} \
 \
_DataType * artable_add_##_datatype##_with_key_##_keytype( \
        ArTable    * table,  \
        _DataType    newData, \
        _KeyType     key \
        ) \
{ \
    /* check if key is already present */ \
    _DataType  * existingEntry = \
        artable_get_##_datatype##_with_key_##_keytype( table, key ); \
    \
    if ( existingEntry ) \
    { \
        (*existingEntry) = newData; \
        return existingEntry; \
    } \
    else \
    { \
        _KeyTypeName##_DataTypeName##HashEntry  * hashEntry = \
            ALLOC(_KeyTypeName##_DataTypeName##HashEntry); \
        hashEntry->entry.hash   = get_##_keytype##_hash(key); \
        hashEntry->key          = key; \
        hashEntry->data         = newData; \
        arhashtable_add_entry( table, (ArHashEntry*) hashEntry ); \
        return & hashEntry->data; \
    } \
} \
\
_DataType * artable_add_##_datatype( \
    ArTable         * table, \
    _DataType       newData \
    ) \
{ \
    /* getting key from implicit or explicit defined key-field with type _KeyType in newData structure */ \
    _KeyType key = get_key_##_keytype##_from_data_##_datatype(newData); \
    return artable_add_##_datatype##_with_key_##_keytype( table, newData, key ); \
} \
\
\
void artable_remove_##_datatype##_with_key_##_keytype( \
        ArTable         * table, \
        _KeyType        removeKey \
        ) \
{ \
    unsigned long hash = get_##_keytype##_hash(removeKey); \
    ArHashEntry     * hashEntry = arhashtable_find_hash( (ArHashTable *) table, hash, NULL ); \
        while ( hashEntry ) \
        { \
                _KeyTypeName##_DataTypeName##HashEntry * typedHashEntry = \
                        STRUCT_POINTER(_KeyTypeName##_DataTypeName##HashEntry,hashEntry,entry ); \
        _KeyType    storedKey   = typedHashEntry->key; \
        if ( artable_compare_##_keytype(removeKey,storedKey) == 0 ) \
        { \
            /* remove */ \
                        arhashtable_del_entry( (ArHashTable *) table, hashEntry); \
                        FREE(typedHashEntry); \
                        hashEntry = NULL; \
        } \
        else \
        { \
            hashEntry = arhashtable_find_hash( (ArHashTable *) table, hash, hashEntry ); \
        } \
        } \
} \
 \
void artable_remove_##_datatype( \
    ArTable         * table, \
    _DataType       removeData \
    ) \
{ \
    /* getting key from implicit or explicit defined key-field with type _KeyType in newData structure */ \
    _KeyType key = get_key_##_keytype##_from_data_##_datatype(removeData); \
    artable_remove_##_datatype##_with_key_##_keytype( table, key ); \
} \
\
_KeyType * artable_iterate_##_datatype##_with_key_##_keytype( \
    ArTable   * table, \
    _KeyType  * previousKey \
    ) \
{ \
    ArHashEntry * hashEntry = NULL; \
\
    if ( *previousKey ) \
    { \
        /* search for the entry with the previous key */ \
        /* we need the actual entry to take one step further */ \
\
        unsigned long hash = get_##_keytype##_hash( *previousKey ); \
\
        hashEntry = \
            arhashtable_find_hash( \
                (ArHashTable *) table, \
                hash, \
                NULL \
                ); \
\
        int isFinished = 0; \
\
        while ( isFinished == 0 && hashEntry ) \
        { \
            _KeyTypeName##_DataTypeName##HashEntry * typedHashEntry = \
                STRUCT_POINTER(_KeyTypeName##_DataTypeName##HashEntry,hashEntry,entry ); \
\
            _KeyType    storedKey   = typedHashEntry->key; \
\
            if ( artable_compare_##_keytype(*previousKey,storedKey) == 0 ) \
            { \
                isFinished = 1; \
            } \
            else \
            { \
                hashEntry = \
                    arhashtable_find_hash( \
                        (ArHashTable *) table, \
                        hash, \
                        hashEntry \
                        ); \
            } \
        } \
    } \
\
    /* we move to the next entry in the table */ \
\
    hashEntry = arhashtable_next_entry( (ArHashTable *) table, hashEntry ); \
\
    if ( hashEntry ) \
    { \
        _KeyTypeName##_DataTypeName##HashEntry * typedHashEntry = \
            STRUCT_POINTER(_KeyTypeName##_DataTypeName##HashEntry,hashEntry,entry); \
\
        if ( typedHashEntry ) \
        { \
            _KeyType * storedKey = &(typedHashEntry->key); \
            if ( storedKey ) \
            { \
                return storedKey; \
            } \
        } \
\
        hashEntry = \
            arhashtable_next_entry( \
                (ArHashTable *) table, \
                hashEntry \
                ); \
    } \
    return NULL; \
}

#define ARTABLE_IMPLEMENTATION_FOR_TYPE( \
    _DataType,  \
    _datatype,  \
    _KeyType,   \
    _keytype )  \
\
_KeyType get_key_##_keytype##_from_data_##_datatype( \
        _DataType  data \
        ) \
{ \
    _KeyType  key; \
    key = ( _KeyType ) data; \
    return key; \
} \
 \
_ARTABLE_IMPLEMENTATION_FOR_TYPE( \
    _datatype, \
    _DataType,    \
    _datatype, \
    _keytype,  \
    _KeyType,     \
    _keytype )  \
\
_DataType * artable_get_##_datatype##_with_key( \
        ArTable         * table, \
        const _KeyType    key \
        ) \
{ \
    return \
        artable_get_##_datatype##_with_key_##_keytype( \
            table, \
            key \
            ); \
} \
\
_DataType * artable_add_##_datatype##_with_key( \
    ArTable    * table, \
    _DataType    newData, \
    _KeyType     newKey \
    ) \
{ \
    return \
        artable_add_##_datatype##_with_key_##_keytype( \
            table, \
            newData, \
            newKey \
            ); \
}
/* \
\
_KeyType * artable_iterate_##_datatype##_with_key( \
    ArTable   * table, \
    _KeyType  * newKey \
    ) \
{ \
    return \
        artable_iterate_##_datatype##_with_key_##_keytype( \
            table, \
            newKey \
            ); \
}
*/

#define ARTABLE_COMPARISON_BY_OPERAND_FOR_KEYTYPE_INTERFACE(_KeyType,_keytype) \
int artable_compare_##_keytype( \
        const _KeyType  key1, \
        const _KeyType  key2  \
        );

#define ARTABLE_COMPARISON_BY_OPERAND_FOR_KEYTYPE_IMPLEMENTATION(_KeyType,_keytype) \
int artable_compare_##_keytype( \
        const _KeyType  key1, \
        const _KeyType  key2  \
        ) \
{ \
    if ( key1 < key2 ) \
        return -1; \
    if ( key1 > key2 ) \
        return 1; \
    return 0; \
}

#define ARTABLE_COMPARISON_BY_OPERAND_FOR_PTR_KEYTYPE(_KeyType,_keytype) \
int artable_compare_##_keytype##ptr( \
    const _KeyType  * key1, \
    const _KeyType  * key2  \
    ) \
{ \
    if ( key1 == key2 ) \
        return 0; \
    return -1; \
}

#define ARTABLE_COMPARISON_BY_FUNCTION_FOR_PTR_KEYTYPE( \
    _KeyType, \
    _keytype, \
    _comparisonFunction) \
int compare_##_keytype##ptr( \
    const _KeyType  * key1, \
    const _KeyType  * key2  \
    ) \
{ \
    return _comparisonFunction( key1, key2 ); \
}

// =================================================
// HASHING function

#define DEFINE_STANDARD_HASHING_FOR_TYPE(_KeyType,_keytype) \
unsigned long get_##_keytype##_hash( \
        const _KeyType  dataToHash \
        ) \
{ \
    return (unsigned long) & dataToHash; \
}

#define DEFINE_STANDARD_HASHING_FOR_PTR_TYPE(_KeyType,_keytype) \
unsigned long get_##_keytype##ptr_hash( \
        const _KeyType  * ptrDataToHash \
        ) \
{ \
    return (unsigned long) ptrDataToHash; \
}

#define DEFINE_HASHING_FOR_PTR_TYPE(_KeyType,_keytype,_HashingFunc) \
unsigned long get_##_keytype##ptr_hash( \
        const _KeyType        * ptrDataToHash \
        ) \
{ \
        return _HashingFunc( ptrDataToHash ); \
}

// =================================================
// TABLE IMPLEMENTATION function



/*============================
   STANDARD IMPLEMENTATION
   ============================*/

#define DEFINE_ARTABLE_FOR_TYPE( \
        _DataType, \
        _datatype ) \
_DataType get_key_##_datatype##_from_data_##_datatype( \
    _DataType  data \
    ) \
{ \
    return data; \
} \
\
_DEFINE_ARTABLE_IMPLEMENTATION_FOR_TYPE( \
    _DataType, \
    _DataType, \
    _datatype, \
    _DataType, \
    _DataType, \
    _datatype ) \
\
_DataType * artable_get_##_datatype##_with_key( \
        ArTable         * table, \
        const _DataType   key \
        ) \
{ \
    return artable_get_##_datatype##_with_key_##_datatype( table, key ); \
} \
\
_DataType * artable_add_##_datatype_with_key( \
    ArTable         * table, \
    _DataType       newData, \
    _KeyType        newKey \
    ) \
{ \
    return artable_add_##_datatype##_with_key_##_keytype( table, newData, newKey ); \
} \
\
_DataType * artable_iterate_##_datatype_with_key( \
    ArTable         * table, \
    _DataType       data, \
    _KeyType        key \
    ) \
{ \
    return artable_iterate_##_datatype##_with_key_##_keytype( table, data, key ); \
}


#define DEFINE_ARTABLE_FOR_TYPE_WITH_KEY( \
        _DataType, \
        _datatype, \
        _KeyType, \
        _keytype ) \
_KeyType get_key_##_keytype##_from_data_##_datatype( \
    _DataType           data \
    ) \
{ \
    _KeyType * key; \
    key = ( _KeyType * ) &data; \
    return *key; \
} \
 \
_DEFINE_ARTABLE_IMPLEMENTATION_FOR_TYPE( \
    _KeyType, \
    _KeyType, \
    _keytype, \
    _DataType, \
    _DataType, \
    _datatype ) \
\
_DataType * artable_get_##_datatype##_with_key( \
        ArTable         * table, \
        const _DataType   key \
        ) \
{ \
    return artable_get_##_datatype##_with_key_##_keytype( table, key ); \
}

#define DEFINE_STANDARD_ARTABLE_IMPLEMENTATION_FOR_PTR_KEYTYPE_DATATYPE(_KeyType,_keytype,_DataType,_datatype) \
_KeyType * get_key_##_keytype##ptr_from_data_##_datatype( \
    _DataType           data \
    ) \
{ \
    _KeyType ** key; \
    key = ( _KeyType ** ) &data; \
    return *key; \
} \
 \
_DEFINE_ARTABLE_IMPLEMENTATION_FOR_TYPE(_KeyType##Ptr,_KeyType *,_keytype##ptr,_DataType,_DataType,_datatype)

#define DEFINE_STANDARD_ARTABLE_IMPLEMENTATION_FOR_KEYTYPE_PTR_DATATYPE(_KeyType,_keytype,_DataType,_datatype) \
_KeyType get_key_##_keytype##_from_data_##_datatype##ptr( \
    _DataType           * data \
    ) \
{ \
    _KeyType * key; \
    key = ( _KeyType * ) data; \
    return *key; \
} \
 \
_DEFINE_ARTABLE_IMPLEMENTATION_FOR_TYPE(_KeyType,_KeyType,_keytype,_DataType##Ptr,_DataType *,_datatype##ptr)



/*=====================================
   CUSTOM KEY-FIELD  IMPLEMENTATION
   =====================================*/

#define ARTABLE_IMPLEMENTATION_FOR_STRUCTURE_WITH_INDEX_FIELD( \
    _DataType,  \
    _datatype,  \
    _KeyType,   \
    _keytype,   \
    _KeyField ) \
\
_KeyType get_key_##_keytype##_from_data_##_datatype( \
    _DataType  data \
    ) \
{ \
    _KeyType  key; \
    key = ( _KeyType ) data._KeyField; \
    return key; \
} \
\
_ARTABLE_IMPLEMENTATION_FOR_TYPE( \
        _DataType, \
        _DataType, \
        _datatype, \
        _KeyType,  \
        _KeyType,  \
        _keytype \
        )


#define ARTABLE_IMPLEMENTATION_FOR_STRUCTURE_WITH_ARSYMBOL_INDEX_FIELD( \
        _DataType, \
        _datatype, \
        _KeyField \
        ) \
\
ARTABLE_IMPLEMENTATION_FOR_STRUCTURE_WITH_INDEX_FIELD( \
        _DataType, \
        _datatype, \
        ArSymbol, \
        symbol, \
        _KeyField ) \
\
_DataType * artable_get_##_datatype##_with_key( \
              ArTable   * table, \
        const ArSymbol    key \
        ) \
{ \
    return artable_get_##_datatype##_with_key_symbol( table, key ); \
} \
\
void artable_remove_##_datatype##_with_key( \
              ArTable   * table, \
        const ArSymbol    keyToRemove \
        ) \
{ \
    artable_remove_##_datatype##_with_key_symbol( table, keyToRemove ); \
} \
\
ArSymbol * artable_iterate_##_datatype##_with_key( \
              ArTable   * table, \
              ArSymbol  * previousKey \
        ) \
{ \
    return artable_iterate_##_datatype##_with_key_symbol( table, previousKey ); \
}
