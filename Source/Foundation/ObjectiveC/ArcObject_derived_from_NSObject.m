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

#ifdef ART_BASECLASS_IMPLEMENTATION

@implementation ArcObject

- (id)copyWithZone:(NSZone *)zone
{
    id copy = [ [ [self class] alloc ] init_ART_GV: art_gv ];

    return copy;
}

+ (const char *) cStringClassName
{
    return
        [ [ self description ] cStringUsingEncoding
            :   NSASCIIStringEncoding ];
}

- (const char *) cStringClassName
{
    return
        [ [ [ self class ] description ] cStringUsingEncoding
            :   NSASCIIStringEncoding ];
}

+ (BOOL) conformsToArProtocol
        : (Protocol *) protocol
{
    return [ [ self class ] conformsToProtocol: protocol ];
}

- (BOOL) conformsToArProtocol
        : (Protocol *) protocol
{
    return [ self conformsToProtocol: protocol ];
}

#ifdef ALLOC_DEALLOC_DEBUG_OUTPUT

- (void) dealloc
{
    printf("(%s %p dealloc ",[self cStringClassName],self);

    [ super dealloc ];

    printf("- done)");
}

#endif // ALLOC_DEALLOC_DEBUG_OUTPUT

- (id) init_ART_GV
        : (ART_GV *) new_art_gv
{
#ifdef USE_PROPRIETARY_ARCOBJECT_RETAIN_RELEASE
    art_retain_counter = 1;
#endif

    art_gv = new_art_gv;

#ifdef ALLOC_DEALLOC_DEBUG_OUTPUT
    printf("(%s %p init)",[self cStringClassName], self);
#endif

    return self;
}

#ifdef RELEASE_RETAIN_DEBUG_OUTPUT

- (id) retain
{
    printf(
#ifdef USE_PROPRIETARY_ARCOBJECT_RETAIN_RELEASE
        "ret %s %p %lu/%u->"
#else
        "ret %s %p %lu->"
#endif
        ,   [ self cStringClassName ]
        ,   self
        ,   (unsigned long)[ self retainCount ]
#ifdef USE_PROPRIETARY_ARCOBJECT_RETAIN_RELEASE
        ,   art_retain_counter
#endif
        );

    fflush(stdout);

#ifdef USE_PROPRIETARY_ARCOBJECT_RETAIN_RELEASE
    art_retain_counter++;
#else
    [ super retain ];
#endif

    printf(
#ifdef USE_PROPRIETARY_ARCOBJECT_RETAIN_RELEASE
        "%lu/%u\n"
#else
        "%lu\n"
#endif
        ,   (unsigned long)[ self retainCount ]
#ifdef USE_PROPRIETARY_ARCOBJECT_RETAIN_RELEASE
        ,   art_retain_counter
#endif
        );

    fflush(stdout);

    return self;
}

#ifdef USE_PROPRIETARY_ARCOBJECT_RETAIN_RELEASE

- (oneway void) release
{
    printf("method REL %p ",self);

    printf("%s %lu/%u->"
        ,   [ self cStringClassName ]
        ,   (unsigned long)[ self retainCount ]
        ,   art_retain_counter
        );

    if ( art_retain_counter <= 1 )
        [ super release ];

    printf("%lu/%u\n"
        ,   (unsigned long)[ self retainCount ]
        ,   art_retain_counter
        );

    fflush(stdout);
}

#else // USE_PROPRIETARY_ARCOBJECT_RETAIN_RELEASE

- (oneway void) release
{
    int  i = [ self retainCount ];

    printf("method REL %u ",self);

    printf("%s 0x%x->"
        ,   [ self cStringClassName ]
        ,   [ self retainCount ]
        );

    [ super release ];

    if ( i == 1 )
        printf("0\n");
    else
        printf("%u\n"
            ,   [ self retainCount ]
            );

    fflush(stdout);
}

#endif // USE_PROPRIETARY_ARCOBJECT_RETAIN_RELEASE

#else

#ifdef USE_PROPRIETARY_ARCOBJECT_RETAIN_RELEASE

- (id) retain
{
    art_retain_counter++;

    return self;
}

- (oneway void) release
{
    if ( art_retain_counter <= 1 )
        [ super release ];
    else
        art_retain_counter--;
}

#endif // USE_PROPRIETARY_ARCOBJECT_RETAIN_RELEASE

#endif

@end

#endif // ART_BASECLASS_IMPLEMENTATION

// ===========================================================================
