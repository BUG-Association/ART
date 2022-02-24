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

ART_MODULE_INTERFACE(ArnExternal)

#import "ArNode.h"

/* --------------------------------------------------------------------------
    'ArnExternal'

        This node is used to load and/or instantiate objects which depend on
        external data sources. The actual instantiation could be done lazily
        (i.e. only when the external is accessed for the first time), but
        currently the native parser reads them at the same time as the
        regular scene graph is being parsed.

        Terminology:

        - The "external" is the scene graph object for which this type of
          node is being used as a stand-in.

        - The "ArnExternal node" is an instance of this class which serves
          as the connection to a particular external.

        - The "external file" is the file which contains the raw data used
          in the creation of the external. Depending on the type of
          external this data can be very diverse: from .art scene graph files
          (from which either parts can be selected, e.g. through the
          node table mechanism, or which can be used in their entirety) to
          binary heightfield data anything that is understood by an Arf...
          parser is possible.

        - The "parser of the external" is the Arf... parser object selected
          by the file probe to read the external in question.
------------------------------------------------------------------------aw- */


@interface ArnExternal
        : ArnBinary
        < ArpConcreteClass, ArpCoding >
{
    ArSymbol    externalFileName;
    ArSymbol    objectName;
    Protocol  * arProtocol0;
    Protocol  * arProtocol1;
}

/* --------------------------------------------------------------------------
    'init'
        Arguments for the various versions of "init":

        'newExternalFileName'

        Filename for the external file. The file itself can be of any type
        that is recognised by the ART file probe. The only constraint is
        semantical, namely that the parser for this type of file should
        return something that is use- and meaningful at the point where the
        external is eventually inserted.
        (in other words, that the object which will eventually be created
        makes sense at the place where the ArnExternal node is located)

        'newObjectName'

        The name for a named subnode of the external to be returned by the
        ArnExternal node. This parameter is only meaningful if the external
        is an ArnNodeTable that supports this kind of look-up; the typical
        use for this kind of functionality are library modules which contain
        large numbers of similar objects in one single external file.
        The parameter is ignored for all other types of externals.

        'newArProtocol'

        A protocol which the ArnExternal node will feign to support as long
        as the external that it is being used as a stand-in for has not been
        instantiated yet.

        'newAuxiliaryNode'

        This optional parameter is an arbitrary scene graph node which is
        presented to the parser of the external when the external is being
        read from file. This can be used to store optional object creation
        parameters which should - or can - not be placed in the external file
        itself.

        A typical example are the mesh operator sequences used to control the
        final appearance of heightfields. The heightfield external file only
        contains the raw elevation data - the level and type of subdivision
        (or lack thereof) is specified in native ArnMesh semantics in the
        ART scene file.
------------------------------------------------------------------------aw- */

- (id) init
        : (ArSymbol) newExternalFileName
        : (Protocol *) newArProtocol
        ;

- (id) init
        : (ArSymbol) newExternalFileName
        : (ArSymbol) newObjectName
        : (Protocol *) newArProtocol
        ;

- (id) init
        : (ArSymbol) newExternalFileName
        : (ArSymbol) newObjectName
        : (Protocol *) newArProtocol0
        : (Protocol *) newArProtocol1
        ;

- (id) initWithAuxiliaryNode
        : (ArSymbol) newExternalFileName
        : (ArNodeRef) newAuxiliary
        : (Protocol *) newArProtocol
        ;

- (BOOL) conformsToArProtocol
        : (Protocol *) protocol
        ;

- (ArSymbol) externalFileName
        ;

- (ArSymbol) objectName
        ;

- (ArNode *) auxiliaryNode
        ;

@end

@interface ArNode ( External )

- (ArNode *) removeExternals
        ;

@end

ARLIST_INTERFACE_FOR_OBJECT_TYPE(ArnExternal,external);

// ===========================================================================
