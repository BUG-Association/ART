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

#define ART_MODULE_NAME     ArnLightsourceCollector

#import "ArnLightsourceCollector.h"

#import "ART_Shape.h"

#import "ArcAmbientLightsource.h"
#import "ArcAreaLightsource.h"
#import "ArcInfiniteSphereLightsource.h"
#import "ArcPointLightsource.h"
#import "ArcSkydomeLightsource.h"
#import "ArcComplexSkydomeLightsource.h"
#import "ArnPreethamSkyModel.h"
#import "ArnHosekSkyModel.h"
#import "ArnPragueSkyModel.h"
#import "ArnLightsourceCollection.h"

#import "FoundationAssertionMacros.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnLightsourceCollector registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@interface ArnVisitor ( LightsourceCollection )

- (void) collectLightsources
        : (ArNode <ArpShape, ArpMapping> *) node
        : (ArnLightsourceCollector *) obj
        ;

@end

@implementation ArnVisitor ( LightsourceCollection )


/* -aw------------------------------------------------------------------------

    'collectLightsources'

    This visitor method is called for each node in the scene graph which is
    visited and does the actual work of creating the requested lightsource
    objects. It first checks whether the node in question is actually a
    shape, and if this is the case, whether the current surface type is
    actually emissive.

    If both these checks are positive, it does one of four things:

    Case 1: if the shape is an infinite sphere and the emitter surface is a
            skylight surface a skydome lightsource is created

    Case 2: if the shape is an infinite sphere and the surface is a generic
            emitter, it creates a normal infinte sphere lightsource

    Cases 3 and 4: for all other shapes, it inserts either an area
            lightsource (case 3) or a point lightsource (case 4), depending
            on the user specifications

--------------------------------------------------------------------------- */


#define AMBIENT_LIGHT  \
((ArNode <ArpSpectrum> *)ARNODEREF_POINTER(subnodeRefArray[1]))

#define SAMPLER_2D  \
((ArNode <ArpSampling2D> *)ARNODEREF_POINTER(subnodeRefArray[0]))

#define LIGHTCOLLECTION  \
((ArNode <ArpLightsourceCollection> *)obj->lightsourceCollection)

#define REPORTER  \
((ArcObject <ArpReporter> *)obj->reporter)

#import "ArnInfSphere.h"

- (void) collectLightsources
        : (ArNode <ArpShape, ArpMapping> *) node
        : (ArnLightsourceCollector *) obj
{
    if ( [ node conformsToArProtocol : ARPROTOCOL( ArpShape ) ] )
    {
        if ( [ node isMemberOfClass : [ ArnInfSphere class ] ] )
        {
            [ REPORTER beginAction
                :   "Processing environment map"
                ];

           if (   [ ARTS_ENVIRONMENT_MATERIAL(state) isMemberOfClass
                      :   [ ArnPreethamSkyModel class ] ]
               || [ ARTS_ENVIRONMENT_MATERIAL(state) isMemberOfClass
                      :   [ ArnHosekSkyModel class ] ] )

           {
                //  case 1
                ArcSkydomeLightsource  * skylight;

                skylight =
                    [ ALLOC_INIT_OBJECT( ArcSkydomeLightsource )
                        :   node
                        : & state
                        :   obj
                        ];

                [ LIGHTCOLLECTION addLightsource
                    :   skylight
                    :   REPORTER
                    ];
           }
           else
           {
                if ( [ ARTS_ENVIRONMENT_MATERIAL(state) isMemberOfClass
                     :   [ ArnPragueSkyModel class ] ] )
                {
                    //  case 2
                    
                    ArcComplexSkydomeLightsource  * skylight;

                    skylight =
                        [ ALLOC_INIT_OBJECT( ArcComplexSkydomeLightsource )
                            :   node
                            : & state
                            :   obj
                            ];

                    [ LIGHTCOLLECTION addLightsource
                        :   skylight
                        :   REPORTER
                        ];
                }
                else
                {
                    //  case 3

                    ArcInfiniteSphereLightsource  * infLight;

                    infLight =
                        [ ALLOC_INIT_OBJECT( ArcInfiniteSphereLightsource )
                            :   node
                            : & state
                            :   obj ];

                    [ LIGHTCOLLECTION addLightsource
                        :   infLight
                        :   REPORTER
                        ];
                }
           }

           [ REPORTER endAction ];

           return;
        }
        
        if ( [ ARTS_SURFACE_MATERIAL(state) globalSurfaceType ] & arsurface_emitter )
        {
            [ REPORTER beginAction
                :   "Processing emissive shape"
                ];

            [ REPORTER printf
                :   "Shape is of type %s \n"
                ,   [ node cStringClassName ]
                ];

            if ( obj->lightsourceType & arlightsourcetype_area )
            {
                //  case 3
                ArcAreaLightsource  * areaLight;

                areaLight =
                    [ ALLOC_INIT_OBJECT(ArcAreaLightsource)
                        :   node
                        : & state
                        :   obj
                        ];

                [ LIGHTCOLLECTION addLightsource
                    :   areaLight
                    :   REPORTER
                    ];
            }
            else
            {
                //  case 4
                ArcPointLightsource  * pointLight;

                pointLight =
                    [ ALLOC_INIT_OBJECT(ArcPointLightsource)
                        :   node
                        : & state
                        :   obj
                        ];

                [ LIGHTCOLLECTION addLightsource
                    :   pointLight
                    :   REPORTER
                    ];
            }

            [ REPORTER endAction ];

            return;
        }
    }
}

#undef REPORTER

@end

@implementation ArnLightsourceCollector

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnLightsourceCollector)
ARPACTION_DEFAULT_IMPLEMENTATION(ArnLightsourceCollector)

- (id) copy
{
    ArnLightsourceCollector  * copiedInstance = [ super copy ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnLightsourceCollector  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- (id) sampler2D
                   : (ArNode <ArpSampling2D> *) newSampler2D
        resolution : (double) newResolution
        type       : (ArLightsourceType) newLightsourceType
{
    return
        [ self init
            :   newSampler2D
            :   0
            :   0.0
            :   newResolution
            :   newLightsourceType
            ];
}

- (id) sampler2D
                        : (ArNode <ArpSampling2D> *) newSampler2D
        ambientLight    : (ArNode <ArpSpectrum> *) newAmbientLight
        ambientIntensity: (double) newAmbientIntensity
        resolution      : (double) newResolution
        type            : (ArLightsourceType) newLightsourceType
{
    return
        [ self init
            :   newSampler2D
            :   newAmbientLight
            :   newAmbientIntensity
            :   newResolution
            :   newLightsourceType
            ];
}

- (id) init
        : (ArNode <ArpSampling2D> *) newSampler2D
        : (ArNode <ArpSpectrum> *) newAmbientLight
        : (double) newAmbientIntensity
        : (double) newResolution
        : (ArLightsourceType) newLightsourceType
{
    self =
        [ super init
            :   HARD_NODE_REFERENCE(newSampler2D)
            :   HARD_NODE_REFERENCE(newAmbientLight)
            ];
    
    if ( self )
    {
        resolution       = newResolution;
        ambientIntensity = newAmbientIntensity;
        lightsourceType  = newLightsourceType;
    }
    
    return self;
}

- (ArNode <ArpSampling2D> *) sampler2D

{
    return SAMPLER_2D;
}

- (void) performOn
        : (ArNode <ArpNodeStack> *) nodeStack
{
    reporter = ART_GLOBAL_REPORTER;

    ArnVisitor  * visitor =
        [ ALLOC_INIT_OBJECT(ArnVisitor) ];

    ArNodeRef  world_Ref = [ nodeStack pop ];

    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(
        ARNODEREF_POINTER(world_Ref),
        ArpWorld
        );

    ArNode <ArpWorld>  * world =
        (ArNode <ArpWorld> *)ARNODEREF_POINTER(world_Ref);

    ASSERT_CLASS_OR_SUBCLASS_MEMBERSHIP(
        world,
        ArNode
        );

    ASSERT_PROTOCOL_SUPPORT(
        world,
        ArpWorld
        );

    lightsourceCollection =
        [ ALLOC_INIT_OBJECT(ArnLightsourceCollection)
            :   SAMPLER_2D
            :   resolution
            ];

    [ reporter beginTimedAction
        : "identifying and analysing lightsources"
        ];

    [ visitor visitPreOrder
        :   arvisitmode_geom_tree_with_attributes
        :   world
        :   @selector(collectLightsources::)
        :   self
        ];


    if (   ambientIntensity > 0.0
        && AMBIENT_LIGHT )
    {
        ArSpectralIntensity  * ambientLight;


        //   The subnodeArray entry 1 (a.k.a. ambient light) might be an
        //   external, which we have to resolve before use.
        //   We are not in the main scene graph here, so we have to do
        //   this by hand...

        ASSIGN_AS_HARD_NODE_REFERENCE_TO_SUBNODE_I(
            1,
            [ ARNBINARY_SUBNODE_1 removeExternals ]
            );

        ArSpectrum500 * ambientLightColour = s500_alloc( art_gv );

        [ AMBIENT_LIGHT getHiresSpectrum
            :   0
            :   ambientLightColour
            ];

        ambientLight = arspectralintensity_alloc( art_gv );

        arspectralintensity_s_init_i(
            art_gv,
            ambientLightColour,
            ambientLight
            );


        arspectralintensity_d_mul_i(
            art_gv,
            ambientIntensity,
            ambientLight
            );

        ArcAmbientLightsource  * ambientLightSource;

        ambientLightSource =
            [ ALLOC_INIT_OBJECT(ArcAmbientLightsource)
                :   ambientLight
                ];

        [ lightsourceCollection addLightsource
            :   ambientLightSource
            :   reporter
            ];
    }

    [ lightsourceCollection prepareForUse
        :   reporter
        ];

    [ reporter endAction ];
    [ reporter printf: "\n" ];

    [ nodeStack push
        :   HARD_NODE_REFERENCE(lightsourceCollection)
        ];

    [ nodeStack push
        :   world_Ref
        ];

    RELEASE_OBJECT( world );
    RELEASE_OBJECT( lightsourceCollection );
    RELEASE_OBJECT( visitor );
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];
    [ coder codeDouble: & ambientIntensity ];
    [ coder codeDouble: & resolution ];
    [ coder codeInt: ((int*)& lightsourceType) ]; // <- ???
}

@end

// ===========================================================================
