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

ART_MODULE_INTERFACE(ArcObject)

#import "ART_Foundation_Datastructures.h"

#import "ArpObject.h"


//   Uncomment the following line if - and only if - you want TONS of
//   debug output printfs - one for each release and retain operation.

//#define RELEASE_RETAIN_DEBUG_OUTPUT

//   Uncomment the following line if - and only if - you want TONS of
//   debug output printfs - one for each alloc and dealloc operation.
//   This second debug printf is more specific, and more geared towards
//   finding memory leaks.

//#define ALLOC_DEALLOC_DEBUG_OUTPUT


//   Comment out the following line to use the native, inbuilt NSObject
//   retain/release calls within ART. This is slower (and on Linux, probably
//   not threadsafe!), but potentially more reliable. One reason to activate
//   this if you suspect that someone, somewhere is accidentally calling
//   "native" retain/release together with the ART-specific methods (which is
//   of course guaranteed to send everything over the edge).

#define USE_PROPRIETARY_ARCOBJECT_RETAIN_RELEASE


/* ---------------------------------------------------------------------------

    'ArcObject' class
    -----------------

    This class is the base class of all object instances used in ART, and
    apart from a few minor additions (which are defined in the ArpObject
    protocol) behaves exactly like the Openstep/GNUStep/Cocoa base class
    'NSObject'.

    In the interest of readability and clarity the two different derivations
    (short in the case of 'NSObject' already being the base class, longer if
    we have to retro-fit 'Object' to behave like NSObject) were placed in
    separate source files, and are included as needed.

------------------------------------------------------------------------aw- */

#define ART_BASECLASS_INTERFACE

#ifdef ART_WITH_NSOBJECT

#import "ArcObject_derived_from_NSObject.h"

#else

#import "ArcObject_derived_from_Object.h"

#endif

#undef ART_BASECLASS_INTERFACE


/* ---------------------------------------------------------------------------

    Macros for object handling
    --------------------------

    These are just shorthand notation for some very common object operations,
    and do exactly what you expect them to do. Some are not necessary in the
    strict sense of the word, since they are really just plain wrappers around
    core ObjC/NSObject functionality; RETAIN_OBJECT is one example. Such
    macros are introduced nonetheless, so that all low-level object creation
    and management (release/retain) operations use similar-looking macros
    throughout ART.

    Note that the consistent use of the "constructor macros" supplied here
    allows us to pass the "art_gv" global variable environment to any newly
    created objects without having to explicitly mention this in the
    calling code.

------------------------------------------------------------------------aw- */

#ifdef RELEASE_RETAIN_DEBUG_OUTPUT

//   Debug versions of several macros - explanations of their normal
//   functionality are given further down where the non-debug versions
//   of the macros are defined.

#define ALLOC_OBJECT(_class) \
    ( (_class *) (printf("CRT "#_class" 0->1\n"),fflush(stdout), \
    [ [ _class alloc ] init_ART_GV: art_gv ]) )

#define ALLOC_OBJECT_AGV(_agv,_class) \
    ( (_class *) (printf("CRX "#_class" 0->1\n"),fflush(stdout), \
    [ [ _class alloc ] init_ART_GV: _agv ]) )

#define ALLOC_OBJECT_BY_CLASS(_class,_protocol) \
    (id <_protocol>)(printf("CRC "#_class" 0->1\n"),fflush(stdout), \
    [(ArcObject*)((id <_protocol>)NSAllocateObject( _class, 0, 0 )) \
    init_ART_GV: art_gv ] )

#ifdef USE_PROPRIETARY_ARCOBJECT_RETAIN_RELEASE

#define RETAIN_OBJECT(_object) \
    ((id) \
        ( \
            ( \
                (_object) == 0 \
                ? \
                0 \
                : \
                ( \
                    printf( \
                        "rEt %s %p %lu/%u->" \
                        ,   [ ((id)(_object)) cStringClassName ] \
                        ,   (_object) \
                        ,   (unsigned long)[ ((id)(_object)) retainCount ] \
                        ,   ((ArcObject*)(_object))->art_retain_counter \
                        ), \
                    ((ArcObject*)(_object))->art_retain_counter++, \
                    printf( \
                        "%lu/%u\n" \
                        ,   (unsigned long)[ ((id)(_object)) retainCount ] \
                        ,   ((ArcObject*)(_object))->art_retain_counter \
                        ) \
                ) \
            ), \
            (_object) \
        ) \
    )

#define RELEASE_OBJECT(_object) \
    ((id) ( \
    ( \
        (_object) == 0 \
        ? \
        0 \
        : \
        ( \
            printf( \
                "macro REL %s %p %lu/%u->" \
                ,   [ ((id)(_object)) cStringClassName ] \
                ,   (_object) \
                ,   (unsigned long)[ ((id)(_object)) retainCount ] \
                ,   ((ArcObject*)(_object))->art_retain_counter \
                ), \
            ((ArcObject*)(_object))->art_retain_counter, \
            ( \
                printf( \
                    "%lu/%u\n" \
                    ,   (unsigned long)[ ((id)(_object)) retainCount ] \
                    ,   ((ArcObject*)(_object))->art_retain_counter \
                    ), \
                ((ArcObject*)(_object))->art_retain_counter == 0 \
                ? \
                    [ ((id)(_object)) release ] \
                : \
                    0 \
            ) \
        ) \
    ), \
    NULL \
    ) )

#endif  // USE_PROPRIETARY_ARCOBJECT_RETAIN_RELEASE

#else  //  RELEASE_RETAIN_DEBUG_OUTPUT not defined

//   Normal, non-debug versions of macros follow.

//   ALLOC_OBJECT
//   Equivalent to [ class alloc ], except that the created object is
//   transparently provided with a pointer to the ubiquitous art_gv struct.

#define ALLOC_OBJECT(_class) \
    ( (_class *) [ [ _class alloc ] init_ART_GV: art_gv ] )

//   ALLOC_OBJECT_AGV
//   Exactly the same as ALLOC_OBJECT, except that the caller gets to supply
//   their own ART_GV variable. This is useful in all cases where there is no
//   implicit "art_gv" variable, such as in class methods (!).

#define ALLOC_OBJECT_AGV(_agv,_class) \
    ( (_class *) [ [ _class alloc ] init_ART_GV: _agv ] )

//   ALLOC_OBJECT_BY_CLASS
//   Creates an instance of a class that is given by a char* name only. This
//   is needed e.g. to construct the objects that are in a scene graph when
//   it is being read from file. The parser reads the class name as string,
//   and then calls this macro to create an instance.

#define ALLOC_OBJECT_BY_CLASS(_class,_protocol) \
    (id <_protocol>)([(ArcObject*)((id <_protocol>)NSAllocateObject( _class, 0, 0 )) init_ART_GV: art_gv ] )

#endif //  RELEASE_RETAIN_DEBUG_OUTPUT

//   RETAIN_OBJECT
//   Calls "retain" on the supplied object.

#ifndef USE_PROPRIETARY_ARCOBJECT_RETAIN_RELEASE

#define RETAIN_OBJECT(_object) \
     [ ((id)(_object)) retain ]

#else

#ifndef RELEASE_RETAIN_DEBUG_OUTPUT

#define RETAIN_OBJECT(_object) \
    ((id) ( \
    ( \
        (_object) == 0 \
        ? \
            0 \
        : \
            ((ArcObject*)(_object))->art_retain_counter++ \
    ), \
    (_object) \
    ) )

#endif

#endif

//   RETAIN_COUNT_OF_OBJECT
//   Returns the retain count of the supplied object.

//   The cast to (unsigned int) is questionable - but so is making this
//   a 64 bit int on 64 bit systems. Once your retain count runs over 32 bits,
//   you have design issues anyway...

#define RETAIN_COUNT_OF_OBJECT(_object) \
     (unsigned int)[ ((id)(_object)) retainCount ]

//   RELEASE_OBJECT
//   Calls "release" on the supplied object. Note that this macro
//   intentionally returns NULL, so that the caller can set any instance
//   variables that point to object instances to NULL by calling this macro:
//
//   instanceVar = RELEASE_OBJECT(previousContent); // <- instanceVar = NULL

#ifndef USE_PROPRIETARY_ARCOBJECT_RETAIN_RELEASE

#define RELEASE_OBJECT(_object) \
    ( (id) ( [ ((id)(_object)) release ], NULL ) )

#else

#ifndef RELEASE_RETAIN_DEBUG_OUTPUT

#define RELEASE_OBJECT_RETURN_ID(_object) \
    ((id) ( \
    ( \
        (_object) == 0 \
        ? \
        ((void) NULL) \
        : \
        ((void)(\
            ((ArcObject*)(_object))->art_retain_counter--, \
            ( \
                ((ArcObject*)(_object))->art_retain_counter == 0 \
                ? \
                    ((void)[ ((id)(_object)) release ]) \
                : \
                    ((void) NULL) \
            )) \
        ) \
    ), \
    NULL \
    ) )

#define RELEASE_OBJECT(_object) ((void)RELEASE_OBJECT_RETURN_ID(_object))

#endif

#endif

//   ALLOC_INIT_OBJECT
//   Equivalent to "[ class alloc ] init" - note the missing outer brackets!
//   This is used to create objects (who are, amongst other things, provided
//   with the "magic" art_gv pointer), and then call "init" on them.
//
//   Typical usage would be:
//
//   id  foo =
//       [  ALLOC_INIT_OBJECT(someClass)
//           :   initArgument0
//           :   initArgument1
//           ];

#define ALLOC_INIT_OBJECT(_class) \
    ALLOC_OBJECT(_class) init

//   ALLOC_INIT_OBJECT_BY_CLASS
//   Same goal and usage as ALLOC_INIT_OBJECT, except that the class name
//   is provided as char *.

#define ALLOC_INIT_OBJECT_BY_CLASS(_class,_protocol) \
    ALLOC_OBJECT_BY_CLASS(_class,_protocol) init

//   ALLOC_INIT_OBJECT_BY_CLASS
//   Same goal and usage as ALLOC_INIT_OBJECT, except that a user-specified
//   ART_GV struct is supplied (this is e.g. necessary within class methods).

#define ALLOC_INIT_OBJECT_AGV(_agv,_class) \
    ALLOC_OBJECT_AGV(_agv,_class) init


/* ---------------------------------------------------------------------------

    'ArList', 'Ar..DynArray' and 'ArTable' object retention policy
    ----------------------------------------------------------------

    (Note that this discussion does NOT apply to the various forms of
    'Ar..Array', which are just a front-end to plain C arrays, and which
    should not be used to store objects.)

    The basic list, dynarray and table data structures used in ART
    all behave in a similar fashion when dealing with reference counted
    object instances, i.e. when they are instantiated via the
    '..._FOR_OBJECT_TYPE' interface/implementation macros defined below.

    The policy is that anything that is added to such a data structure is
    retained by it (i.e. [ newAddition retain ] is called by the data
    structure add code), but everything that is being removed from it is NOT
    released. This means that e.g. the reference count of an object which
    is taken from a dynarray via 'pop' is NOT decremented by the stack code.

    Of course, [ object release ] is called for each element in one of
    these storage data structures, if and when the entire data structure is
    disposed of via 'free'. It is also called on any object previously stored
    in a particular slot, if it is directly overwritten with a new entry,
    e.g. by calling arobjdynarray_set_i() to directly set the i-th slot in
    a dynarray to a new value.

    This convention might seem a bit illogical at first, but has its reasons.

    One is pragmatical - we do not use autorelease pools in ART for
    performance and compatibility reasons. And without 'autorelease', which
    keeps an object with a reference counter of zero alive until the entire
    autorelease pool is freed (as opposed to 'release', which immediately
    destroys any object that reaches zero) it would not be possible to
    'pop' an object that is only 'retained' by the stack itself. Such an
    object would reach a retain count of zero before any new owner could get
    hold of it, if the stack were to 'release' every instance it contains
    when performing a 'pop' operation.

    The second reason is that this convention is not all that counter-
    intuitive anyway. It makes sense to assume that anything one removes from
    such a data structure implicitly belongs to the caller, i.e. that 'retain'
    has already been called for the instance one takes from the data
    structure.

    Otherwise, if one did not want to 'retain' the object, why would one
    remove it from the storage data structure in the first place?

    All one has to do is keep in mind that any object which is removed from
    a list, stack, array or table has to be 'released' after use.

    IMPORTANT:
    ==========

    Note that the discussion above only applies to objects that are actually
    being *removed* from the data structure in question, e.g. via a 'pop'
    operation.

    Any object instance that you just *reference* in such a way that the
    actual entry remains in the data structure - e.g. by directly accessing
    an ArNodeArray element, or an ArTable entry that you found by searching
    for its key - has to be explicitly 'retained' by the caller (in addition
    to of course later being 'released' by the caller, once he no longer
    needs it).

------------------------------------------------------------------------aw- */


//   Definition of a special version of the Ar..List interface/implementation
//   macros that work for retain/release managed objects.

#define ARLIST_INTERFACE_FOR_OBJECT_TYPE(_Type,_type) \
    _ARLIST_INTERFACE_FOR_TYPE(_Type,_type,_Type *)

#define ARLIST_IMPLEMENTATION_FOR_OBJECT_TYPE(_Type,_type) \
    _ARLIST_IMPLEMENTATION_FOR_TYPE( \
        _Type, _type, _Type *, \
        RETAIN_OBJECT, , RELEASE_OBJECT_RETURN_ID )


//   Interface definition of ArList functionality for ArcObjects.
//
//   The first, 'pointer only version' is the one to use for lists
//   that contain weak references (i.e. ones that are not reference
//   counted), and the second one retains and releases its payload
//   objects (see note above for an exact description of the functionality).

ARLIST_INTERFACE_FOR_PTR_TYPE(ArcObject,obj)
ARLIST_INTERFACE_FOR_OBJECT_TYPE(ArcObject,obj)

#ifdef NEVERMORE
//   Definition of a special version of the Ar..DynArray
//   interface/implementation macros that work for retain/release managed
//   objects.

#define ARDYNARRAY_INTERFACE_FOR_OBJECT_TYPE( _Type,_SType,_type,_TYPE ) \
    _ARDYNARRAY_INTERFACE_FOR_TYPE( _SType, _Type *, _type, _TYPE )

#define ARDYNARRAY_IMPLEMENTATION_FOR_OBJECT_TYPE( \
        _Type, _SType, _type, _TYPE, _unusedValue ) \
    _ARDYNARRAY_IMPLEMENTATION_FOR_MANAGED_TYPE( \
        _Type *, _SType, _type, _TYPE, _unusedValue, \
        RETAIN_OBJECT, RETAIN_COUNT_OF_OBJECT, , RELEASE_OBJECT ) \
    _ARDYNARRAY_VA_ARG_IMPLEMENTATION_FOR_TYPE( \
        _Type *, _Type *, _SType, _type ) \



//   Interface definition of Ar..Stack functionality for ArcObjects.
//
//   The first, 'pointer only version' is the one to use for stacks
//   that contain weak references (i.e. ones that are not reference
//   counted), and the second one retains and releases its payload
//   objects (see note above for an exact description of the functionality).

ARDYNARRAY_INTERFACE_FOR_TYPE_PTR(ArcObject,Obj,obj,o)
ARDYNARRAY_INTERFACE_FOR_OBJECT_TYPE(ArcObject,Obj,obj,o)
#endif
// ===========================================================================
