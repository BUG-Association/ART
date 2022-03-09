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

//   key comparison interface macros

#define ARTABLE_INTERFACE_KEYTYPE_COMPARISON_BY_OPERAND( \
        _KeyType, \
        _keytype  \
        ) \
\
int artable_compare_##_keytype( \
        const _KeyType  key1, \
        const _KeyType  key2  \
        );

#define ARTABLE_INTERFACE_PTR_KEYTYPE_COMPARISON_BY_OPERAND( \
        _KeyType, \
        _keytype  \
        ) \
\
int artable_compare_##_keytype##ptr( \
        const _KeyType  * key1, \
        const _KeyType  * key2  \
        );

#define ARTABLE_INTERFACE_KEYTYPE_COMPARISON_BY_FUNCTION( \
        _KeyType, \
        _keytype, \
        _comparisonFunction \
        ) \
\
int artable_compare_##_keytype( \
        const _KeyType  key1, \
        const _KeyType  key2  \
        );

#define ARTABLE_INTERFACE_PTR_KEYTYPE_COMPARISON_BY_FUNCTION( \
        _KeyType, \
        _keytype, \
        _comparisonFunction \
        ) \
\
int compare_##_keytype##ptr( \
        const _KeyType  * key1, \
        const _KeyType  * key2  \
        );


//   hash function interface macros


#define ARTABLE_INTERFACE_DIRECT_HASHING_FOR_KEYTYPE( \
        _KeyType, \
        _keytype  \
        ) \
\
unsigned long get_##_keytype##_hash( \
        const _KeyType  dataToHash \
        );

#define ARTABLE_INTERFACE_DIRECT_HASHING_FOR_PTR_KEYTYPE( \
        _KeyType, \
        _keytype  \
        ) \
\
unsigned long get_##_keytype##ptr_hash( \
        const _KeyType  * ptrDataToHash \
        );

#define ARTABLE_INTERFACE_HASHING_FUNCTION_FOR_KEYTYPE( \
        _KeyType, \
        _keytype, \
        _hashingFunc \
        ) \
\
unsigned long get_##_keytype##_hash( \
        const _KeyType  dataToHash \
        );

#define ARTABLE_INTERFACE_HASHING_FUNCTION_FOR_PTR_KEYTYPE( \
        _KeyType, \
        _keytype, \
        _HashingFunc \
        ) \
\
unsigned long get_##_keytype##ptr_hash( \
        const _KeyType        * ptrDataToHash \
        );


//   Generic ArTable interface definition used by the various forms that
//   the user gets to see


#define _ARTABLE_INTERFACE_FOR_TYPE( \
        _KeyTypeName, \
        _KeyType, \
        _keytype, \
        _DataTypeName, \
        _DataType, \
        _datatype \
        ) \
\
typedef struct _KeyTypeName##_DataTypeName##HashEntry \
{ \
    ArHashEntry  entry; \
    _KeyType     key; \
    _DataType    data; \
} \
_KeyTypeName##_DataTypeName##HashEntry; \
\
_DataType * artable_get_##_datatype##_with_key_##_keytype( \
              ArTable   * table, \
        const _KeyType    key \
        ); \
\
_DataType * artable_add_##_datatype##_with_key_##_keytype( \
        ArTable    * table,  \
        _DataType    newData, \
        _KeyType     key \
        ); \
\
_DataType * artable_add_##_datatype( \
        ArTable         * table, \
        _DataType       newData \
        ); \
\
void artable_remove_##_datatype##_with_key_##_keytype( \
        ArTable         * table, \
        _KeyType        removeKey \
        ); \
\
void artable_remove_##_datatype( \
        ArTable         * table, \
        _DataType       removeData \
        ); \
\
_KeyType * artable_iterate_##_datatype##_with_key_##_keytype( \
        ArTable     * table, \
        _KeyType    * previousKey \
        );


//   Table for types that are their own index (i.e. where the hash value
//   is computed directly from the object that is stored in the table)


#define ARTABLE_INTERFACE_FOR_TYPE( \
        _DataType, \
        _datatype \
        ) \
\
_DataType get_key_##_datatype##_from_data_##_datatype( \
        _DataType  data \
        ); \
\
_ARTABLE_INTERFACE_FOR_TYPE( \
        _DataType, \
        _DataType, \
        _datatype, \
        _DataType, \
        _DataType, \
        _datatype ) \
\
_DataType * artable_get_##_datatype##_with_key( \
              ArTable   * table, \
        const _DataType   key \
        );


//   Structure tables

#define _ARTABLE_DSINTERFACE_FOR_TYPE( \
    _DataTypeName, \
    _DataType, \
    _datatype, \
    _KeyTypeName, \
    _KeyType, \
    _keytype ) \
\
typedef struct _KeyTypeName##_DataTypeName##HashEntry \
{ \
    ArHashEntry  entry; \
    _KeyType     key; \
    _DataType    data; \
} \
_KeyTypeName##_DataTypeName##HashEntry;

#define ARTABLE_DSINTERFACE_FOR_TYPE( \
    _DataType,  \
    _datatype,  \
    _KeyType,   \
    _keytype )  \
\
_ARTABLE_DSINTERFACE_FOR_TYPE( \
    _datatype,  \
    _DataType,  \
    _datatype,  \
    _keytype,   \
    _KeyType,   \
    _keytype )  \


#define ARTABLE_INTERFACE_FOR_STRUCTURE_WITH_INDEX_FIELD( \
        _DataType, \
        _datatype, \
        _KeyType,  \
        _keytype,  \
        _KeyField  \
        ) \
\
_KeyType get_key_##_keytype##_from_data_##_datatype( \
        _DataType  data \
        ); \
\
_ARTABLE_INTERFACE_FOR_TYPE( \
        _KeyType,  \
        _KeyType,  \
        _keytype,  \
        _DataType, \
        _DataType, \
        _datatype )

#define ARTABLE_INTERFACE_FOR_STRUCTURE_WITH_PTR_INDEX_FIELD( \
        _DataType, \
        _datatype, \
        _KeyType, \
        _keytype, \
        _KeyField \
        ) \
\
_KeyType * get_key_##_keytype##ptr_from_data_##_datatype( \
        _DataType  data \
        ); \
\
_ARTABLE_INTERFACE_FOR_TYPE( \
        _KeyType##Ptr, \
        _KeyType *, \
        _keytype##ptr, \
        _DataType, \
        _DataType, \
        _datatype \
        )

#define ARTABLE_INTERFACE_FOR_STRUCTURE_PTR_WITH_INDEX_FIELD( \
        _DataType, \
        _datatype, \
        _KeyType, \
        _keytype, \
        _KeyField \
        ) \
\
_KeyType get_key_##_keytype##_from_data_##_datatype##ptr( \
        _DataType  * data \
        ); \
\
_ARTABLE_INTERFACE_FOR_TYPE( \
        _KeyType, \
        _KeyType, \
        _keytype, \
        _DataType##Ptr, \
        _DataType *, \
        _datatype##ptr \
        )

#define ARTABLE_INTERFACE_FOR_STRUCTURE_PTR_WITH_PTR_INDEX_FIELD( \
        _DataType, \
        _datatype, \
        _KeyType, \
        _keytype, \
        _KeyField \
        ) \
\
_KeyType * get_key_##_keytype##ptr_from_data_##_datatype##ptr( \
        _DataType  * data \
        ); \
\
_ARTABLE_INTERFACE_FOR_TYPE( \
        _KeyType##Ptr, \
        _KeyType *, \
        _keytype##ptr, \
        _DataType##Ptr, \
        _DataType *, \
        _datatype##ptr \
        )

//   Table for pointer types with a pointer key


#define ARTABLE_INTERFACE_FOR_PTR_TYPE_WITH_PTR_KEY( \
        _DataType, \
        _datatype, \
        _KeyType,  \
        _keytype   \
        )          \
\
_KeyType get_key_##_keytype##_from_data_##_datatype( \
        _DataType  data \
        ); \
\
_ARTABLE_INTERFACE_FOR_TYPE( \
        _KeyType,  \
        _KeyType,  \
        _keytype,  \
        _DataType, \
        _DataType, \
        _datatype  \
        ) \
\
_DataType * artable_get_##_datatype##_with_key( \
        ArTable         * table, \
        const _DataType   key \
        );


//   Special case: symbol-indexed structures


#define ARTABLE_INTERFACE_FOR_STRUCTURE_WITH_ARSYMBOL_INDEX_FIELD( \
        _DataType, \
        _datatype, \
        _KeyField \
        ) \
\
ARTABLE_INTERFACE_FOR_STRUCTURE_WITH_INDEX_FIELD( \
        _DataType, \
        _datatype, \
        ArSymbol, \
        symbol, \
        _KeyField ) \
\
_DataType * artable_get_##_datatype##_with_key( \
              ArTable   * table, \
        const ArSymbol    key \
        ); \
\
void artable_remove_##_datatype##_with_key( \
              ArTable   * table, \
        const ArSymbol    keyToRemove \
        ); \
\
ArSymbol * artable_iterate_##_datatype##_with_key( \
              ArTable   * table, \
              ArSymbol  * previousKey \
        );
