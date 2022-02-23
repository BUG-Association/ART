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

#import "ART_Foundation_Datastructures.h"
#import "ArcObject.h"

void arcobject_initialise()
{
}

#ifdef ART_WITH_NSOBJECT

@implementation NSObject ( ART_ObjC )

- (id)copyWithZone:(NSZone *)zone
{
    id copy = NSCopyObject(self, 0, zone);
    return copy;
}

+ (const char *) cStringClassName
{
    return [ [ self description ] cString ];
}

- (const char *) cStringClassName
{
    return [ [ [ self class ] description ] cString ];
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

- (IMP) methodFor
        : (SEL) aSelector
{
    return [ self methodForSelector: aSelector ];
}

@end

#else // ! ART_WITH_NSOBJECT

id NSAllocateObject(Class _class)
{
    return class_create_instance( _class, 0 );
}

@implementation ArcObject

- dealloc
{
    [ self free ];

    return 0;
}

- (BOOL) isKindOfClass
        : aClassObject
{
    return [ self isKindOf: aClassObject ];
}

- (BOOL) isMemberOfClass
        : aClassObject
{
    return [self isMemberOf: aClassObject];
}

+ (BOOL) instancesRespondToSelector
        : (SEL) aSelector
{
    return [ [ self class ] instancesRespondTo: aSelector];
}

- (BOOL) respondsToSelector
        : (SEL) aSelector
{
    return [self respondsTo: aSelector];
}

- performSelector
        : (SEL) aSelector
{
    return [ self perform: aSelector ];
}

- performSelector : (SEL) aSelector
        withObject: anObject
{
    return
        [ self perform: aSelector
                  with: anObject ];
}

- performSelector : (SEL) aSelector
        withObject: anObject1
        withObject: anObject2
{
    return
        [ self perform: aSelector
                  with: object1
                  with: object2 ];
}

- (IMP) methodForSelector
        : (SEL) aSelector
{
    return
        [ self methodFor: aSelector ]
}

+ (BOOL) conformsToArProtocol
        : (Protocol *) protocol
{
    return [ [ self class ] conformsTo: protocol ];
}

- (BOOL) conformsToArProtocol
        : (Protocol *) protocol
{
    return [ self conformsTo: protocol ];
}

+ (const char *) cStringClassName
{
    return [ self name ];
}

- (const char *) cStringClassName
{
    return [ self name ];
}

@end

#endif // ART_WITH_NSOBJECT

#endif // ART_BASECLASS_IMPLEMENTATION

// ===========================================================================
