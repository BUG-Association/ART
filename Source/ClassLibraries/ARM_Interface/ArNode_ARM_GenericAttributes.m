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

#define ART_MODULE_NAME     ArNode_ARM_GenericAttributes

#import "ArNode_ARM_GenericAttributes.h"
#import "ArNode_ARM_SpecificAttributes.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArNode ( ARM_GenericAttributes )

static ArNode * apply_attributes(
        ART_GV         * art_gv,
        ArNode         * node,
        unsigned int     n,
        ArNode        ** attr
        )
{
    //   The idea behind the variable t is that one wants to group trafo
    //   operations that occur next to each other in an attribute application
    //   list.

    unsigned int  t = 0;

    for ( unsigned int i = 0; i < n; i++ )
    {
        if ( [ attr[i] conformsToArProtocol: ARPROTOCOL(ArpSurfaceMaterial) ] )
        {
            node =
                [ (ArNode *) node surfaceMaterial
                    :   attr[i]
                    ];

            continue;
        }

        if ( [ attr[i] conformsToArProtocol: ARPROTOCOL(ArpEnvironmentMaterial) ] )
        {
            node =
                [ (ArNode *) node environmentMaterial
                    :   attr[i]
                    ];

            continue;
        }

        if ( [ attr[i] conformsToArProtocol: ARPROTOCOL(ArpVolumeMaterial) ] )
        {
            node =
                [ (ArNode *) node volumeMaterial
                    :   attr[i]
                    ];

            continue;
        }

        if ( [ attr[i] conformsToArProtocol: ARPROTOCOL(ArpVertices) ] )
        {
            node =
                [ (ArNode *) node vertices
                    :   attr[i]
                    ];

            continue;
        }

        if ( [ attr[i] conformsToArProtocol: ARPROTOCOL(ArpVarTrafo3D) ] )
        {
            node =
                [ ALLOC_INIT_OBJECT_AUTORELEASE(AraVarTrafo3D)
                    :   HARD_NODE_REFERENCE(node)
                    :   HARD_NODE_REFERENCE(attr[i])
                    ];

            continue;
        }

        if ( [ attr[i] conformsToArProtocol: ARPROTOCOL(ArpTrafo3D) ] )
        {
            t++;
        }
        else
            ART_ERRORHANDLING_FATAL_ERROR(
                "attribute of unknown type"
                );

        if (     i == n-1
            || ! [ attr[i+1] conformsToArProtocol : ARPROTOCOL(ArpTrafo3D) ] )
        {
            switch (t)
            {
                case 1:
                {
                    node =
                        [ (ArNode *) node trafo
                            :   attr[i]
                            ];

                    break;
                }
                case 2:
                {
                    node =
                        [ (ArNode *) node trafo
                            :   attr[i-1]
                            :   attr[i]
                            ];

                    break;
                }
                case 3:
                {
                    node =
                        [ (ArNode *) node trafo
                            :   attr[i-2]
                            :   attr[i-1]
                            :   attr[i]
                            ];

                    break;
                }
                case 4:
                {
                    node =
                        [ (ArNode *) node trafo
                            :   attr[i-3]
                            :   attr[i-2]
                            :   attr[i-1]
                            :   attr[i]
                            ];

                    break;
                }
                case 5:
                {
                    node =
                        [ (ArNode *) node trafo
                            :   attr[i-4]
                            :   attr[i-3]
                            :   attr[i-2]
                            :   attr[i-1]
                            :   attr[i]
                            ];

                    break;
                }
                case 6:
                {
                    node =
                        [ (ArNode *) node trafo
                            :   attr[i-5]
                            :   attr[i-4]
                            :   attr[i-3]
                            :   attr[i-2]
                            :   attr[i-1]
                            :   attr[i]
                            ];

                    break;
                }
                case 7:
                {
                    node =
                        [ (ArNode *) node trafo
                            :   attr[i-6]
                            :   attr[i-5]
                            :   attr[i-4]
                            :   attr[i-3]
                            :   attr[i-2]
                            :   attr[i-1]
                            :   attr[i]
                            ];

                    break;
                }
                case 8:
                {
                    node =
                        [ (ArNode *) node trafo
                            :   attr[i-7]
                            :   attr[i-6]
                            :   attr[i-5]
                            :   attr[i-4]
                            :   attr[i-3]
                            :   attr[i-2]
                            :   attr[i-1]
                            :   attr[i]
                            ];

                    break;
                }
            }
            t = 0;
        }
    }
    return node;
}

- (ArNode *) apply
        : (ArNode *) attr0
{
    ArNode  * attr[1] = { attr0 };
    return apply_attributes(art_gv,self, 1, attr);
}

- (ArNode *) apply
        : (ArNode *) attr0
        : (ArNode *) attr1
{
    ArNode  * attr[2] = { attr0,attr1 };
    return apply_attributes(art_gv,self, 2, attr);
}

- (ArNode *) apply
        : (ArNode *) attr0
        : (ArNode *) attr1
        : (ArNode *) attr2
{
    ArNode  * attr[3] = { attr0,attr1,attr2 };
    return apply_attributes(art_gv,self, 3, attr);
}

- (ArNode *) apply
        : (ArNode *) attr0
        : (ArNode *) attr1
        : (ArNode *) attr2
        : (ArNode *) attr3
{
    ArNode  * attr[4] = { attr0,attr1,attr2,attr3 };
    return apply_attributes(art_gv,self, 4, attr);
}

- (ArNode *) apply
        : (ArNode *) attr0
        : (ArNode *) attr1
        : (ArNode *) attr2
        : (ArNode *) attr3
        : (ArNode *) attr4
{
    ArNode  * attr[5] = { attr0,attr1,attr2,attr3,attr4 };
    return apply_attributes(art_gv,self, 5, attr);
}

- (ArNode *) apply
        : (ArNode *) attr0
        : (ArNode *) attr1
        : (ArNode *) attr2
        : (ArNode *) attr3
        : (ArNode *) attr4
        : (ArNode *) attr5
{
    ArNode  * attr[6] = { attr0,attr1,attr2,attr3,attr4,attr5 };
    return apply_attributes(art_gv,self, 6, attr);
}

- (ArNode *) apply
        : (ArNode *) attr0
        : (ArNode *) attr1
        : (ArNode *) attr2
        : (ArNode *) attr3
        : (ArNode *) attr4
        : (ArNode *) attr5
        : (ArNode *) attr6
{
    ArNode  * attr[7] = { attr0,attr1,attr2,attr3,attr4,attr5,attr6 };
    return apply_attributes(art_gv,self, 7, attr);
}

- (ArNode *) apply
        : (ArNode *) attr0
        : (ArNode *) attr1
        : (ArNode *) attr2
        : (ArNode *) attr3
        : (ArNode *) attr4
        : (ArNode *) attr5
        : (ArNode *) attr6
        : (ArNode *) attr7
{
    ArNode  * attr[8] = { attr0,attr1,attr2,attr3,attr4,attr5,attr6,attr7 };
    return apply_attributes(art_gv,self, 8, attr);
}

- (ArNode *) apply
        : (ArNode *) attr0
        : (ArNode *) attr1
        : (ArNode *) attr2
        : (ArNode *) attr3
        : (ArNode *) attr4
        : (ArNode *) attr5
        : (ArNode *) attr6
        : (ArNode *) attr7
        : (ArNode *) attr8
{
    ArNode  * attr[9] = { attr0,attr1,attr2,attr3,attr4,attr5,attr6,attr7,
                         attr8 };
    return apply_attributes(art_gv,self, 9, attr);
}

- (ArNode *) apply
        : (ArNode *) attr0
        : (ArNode *) attr1
        : (ArNode *) attr2
        : (ArNode *) attr3
        : (ArNode *) attr4
        : (ArNode *) attr5
        : (ArNode *) attr6
        : (ArNode *) attr7
        : (ArNode *) attr8
        : (ArNode *) attr9
{
    ArNode  * attr[10] = { attr0,attr1,attr2,attr3,attr4,attr5,attr6,attr7,
                          attr8,attr9 };
    return apply_attributes(art_gv,self, 10, attr);
}

@end


// ===========================================================================
