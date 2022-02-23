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

/*
Note: this was used to build ART with the old gcc ObjC runtime which still
      had "Object" as its baseclass. GNUStep now also uses NSObject, but
      we still keep this for the time being in case someone publishes
      an ObjC runtime based on Object again.
*/

#ifdef ART_BASECLASS_INTERFACE

@interface ArcObject
        : Object
        < ArpObject >
{
}

- retain
        ;

- (oneway void) release
        ;

- (id) autorelease
        ;

- (BOOL) isKindOfClass
        : aClassObject
        ;

- (BOOL) isMemberOfClass
        : aClassObject
        ;

+ (BOOL) instancesRespondToSelector
        : (SEL) aSelector
        ;

- (BOOL) respondsToSelector
        : (SEL) aSelector
        ;

- performSelector
        : (SEL) aSelector
        ;

- performSelector : (SEL) aSelector
        withObject: anObject
        ;

- performSelector : (SEL) aSelector
        withObject: anObject1
        withObject: anObject2
        ;

- (IMP) methodForSelector
        : (SEL) aSelector
        ;

@end

#endif // ART_BASECLASS_INTERFACE

// ===========================================================================
