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

#define ART_MODULE_NAME     ArnLightsourceCollection

#import "ArnLightsourceCollection.h"

//   Mutexes which are used to make instances re-entrant

typedef struct ArnLightsourceCollection_GV
{
    pthread_mutex_t  mutex_0;
    pthread_mutex_t  mutex_1;
}
ArnLightsourceCollection_GV;

ART_MODULE_INITIALISATION_FUNCTION
(
    [ ArnLightsourceCollection registerWithRuntime ];

    ArnLightsourceCollection_GV  * arnlightsourcecollection_gv;

    arnlightsourcecollection_gv = ALLOC(ArnLightsourceCollection_GV);

    pthread_mutex_init( & arnlightsourcecollection_gv->mutex_0, NULL );
    pthread_mutex_init( & arnlightsourcecollection_gv->mutex_1, NULL );

    art_gv->arnlightsourcecollection_gv = arnlightsourcecollection_gv;
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    pthread_mutex_destroy( & art_gv->arnlightsourcecollection_gv->mutex_0 );
    pthread_mutex_destroy( & art_gv->arnlightsourcecollection_gv->mutex_1 );

    FREE( art_gv->arnlightsourcecollection_gv );
)

//   These macros are used around passages in the code which might be called
//   from multiple threads concurrently.

//   Note that this class is one of the rare cases in ART where there is
//   a legitimate case for multiple threads altering the state of a single
//   object in a concurrent fashion; assembly of the light source collection
//   is a task where the computational effort is invested in the preparation
//   of the individual lightsource proxy objects (this can be done in parallel
//   by several threads - each thread prepares one proxy at a time), which
//   are just entered in this collection once they are ready (minimal overhead).

//   mutex 0 is no longer being used, will be removed

#define LOCK_IVAR_MUTEX     \
    pthread_mutex_lock( & art_gv->arnlightsourcecollection_gv->mutex_0 )
#define UNLOCK_IVAR_MUTEX   \
    pthread_mutex_unlock( & art_gv->arnlightsourcecollection_gv->mutex_0 )
#define LOCK_ADDITION_MUTEX      \
    pthread_mutex_lock( & art_gv->arnlightsourcecollection_gv->mutex_1 )
#define UNLOCK_ADDITION_MUTEX    \
    pthread_mutex_unlock( & art_gv->arnlightsourcecollection_gv->mutex_1 )

@implementation ArnLightsourceCollection

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnLightsourceCollection)

- (id) init
        : (ArcObject <ArpSampling2D> *) newSampler2D
        : (double) newResolution
{
    self = [ super init ];

    if ( self )
    {
        numberOfSlots  = 4;
        numberOfActualSlots = numberOfSlots * PSM_ARRAYSIZE;
        numberOfLights = 0;
        light = ALLOC_ARRAY( ArLightsourceEntry, numberOfActualSlots );

        overallArea            = 0.0;
        overallNumberOfPatches = 0;

    for ( int a = 0; a < PSM_ARRAYSIZE; a++ )
    {
        altitude[a] =
            PSM_MIN_ALTITUDE + a * PSM_LIGHTCOLLECTION_VERTICAL_STEPSIZE;

        overallRadiantPower[a]  = 0.0;

        overallSpectralPower[a] = arspectralintensity_alloc(art_gv);
        overallLowresSpectralPower[a] = arlightintensity_alloc(art_gv);

        arspectralintensity_d_init_i(
            art_gv,
            0.0,
            overallSpectralPower[a]
            );
        arlightintensity_d_init_i(
            art_gv,
            0.0,
            overallLowresSpectralPower[a]
            );
    }

    complexSkydomePresent  = NO;
    }

    return self;
}

- (void) addLightsource
        : (ArcObject <ArpLightsource> *)newLightsource
        : (ArcObject <ArpReporter> *) reporter
{
    //   as soon as a single light is added, we assume we own them all

    ownsLights = YES;

    /* -------------------------------------------------------------------

        Adding a new lightsource part 0: we don't insert the source if
        it has zero energy (something which can easily happen if an
        otherwise valid lightsource is subtracted by CSG operations - it's
        still in the scenegraph and cannot be easily culled earlier).

    ----------------------------------------------------------------aw- */

    BOOL  newLightHasAtLeastOneNonzeroPowerValue = NO;

    for ( int a = 0; a < PSM_ARRAYSIZE; a++ )
    {
        Pnt3D  point =
            PNT3D( 0.0, 0.0, altitude[a] );

        double  power =
            [ newLightsource radiantPower
                : & point
                ];

        if ( power > 0.0 )
        {
            newLightHasAtLeastOneNonzeroPowerValue = YES;
            break;
        }
    }

    if ( newLightHasAtLeastOneNonzeroPowerValue )
    {
        LOCK_ADDITION_MUTEX;

        int  newLightIndex = numberOfLights;
        
        overallArea            += [ newLightsource area ];
        overallNumberOfPatches += [ newLightsource numberOfPatches ];

        /* -------------------------------------------------------------------

            Adding a new lightsource part 1: enlarging the light source
            reference array, if this is necessary)

        ----------------------------------------------------------------aw- */

        if ( numberOfLights >= numberOfSlots )
        {
            numberOfSlots *= 2;

            numberOfActualSlots = numberOfSlots * PSM_ARRAYSIZE;

            light =
                REALLOC_ARRAY(
                    light,
                    ArLightsourceEntry,
                    numberOfActualSlots
                    );
        }

        numberOfLights++;

        for ( int a = 0; a < PSM_ARRAYSIZE; a++ )
        {
            LSC_LIGHT(a,newLightIndex).source = newLightsource;
            
            if ( [ newLightsource lightsourceType ] == arlightsourcetype_complex_skydome )
            {
                LSC_LIGHT(a,newLightIndex).complexSkydome = YES;
                complexSkydomePresent  = YES;
            }
            else
            {
                LSC_LIGHT(a,newLightIndex).complexSkydome = NO;
            }

            Pnt3D  point =
                PNT3D( 0.0, 0.0, altitude[a] );
        
            LSC_LIGHT(a,newLightIndex).radiantPower =
                [ newLightsource radiantPower
                    : & point
                    ];

            overallRadiantPower[a] += LSC_LIGHT(a,newLightIndex).radiantPower;

            LSC_LIGHT(a,newLightIndex).spectralPower =
                arspectralintensity_alloc(art_gv);
            LSC_LIGHT(a,newLightIndex).percentOfOverallSpectralPower =
                arspectralintensity_alloc(art_gv);
            LSC_LIGHT(a,newLightIndex).overallSpectralPowerPercentile =
                arspectralintensity_alloc(art_gv);
            
            arspectralintensity_i_init_i(
                art_gv,
                [ newLightsource spectralPower
                    : & point
                    ],
                LSC_LIGHT(a,newLightIndex).spectralPower
                );

            arspectralintensity_i_add_i(
                art_gv,
                LSC_LIGHT(a,newLightIndex).spectralPower,
                overallSpectralPower[a]
                );
            arspectralintensity_to_arlightintensity(
                art_gv,
                overallSpectralPower[a],
                overallLowresSpectralPower[a]
                );

            if ( overallRadiantPower[a] > 0.0 )
            {
                for ( unsigned int i = 0; i < numberOfLights; i++ )
                {
                    LSC_LIGHT(a,i).percentOfOverallRadiantPower =
                         LSC_LIGHT(a,i).radiantPower / overallRadiantPower[a];

                    [ LSC_LIGHT(a,i).source setPercentOfOverallRadiantPower
                        : & point
                        :   LSC_LIGHT(a,i).percentOfOverallRadiantPower
                        ];

                    arspectralintensity_ii_div_i(
                        art_gv,
                        overallSpectralPower[a],
                        LSC_LIGHT(a,i).spectralPower,
                        LSC_LIGHT(a,i).percentOfOverallSpectralPower
                        );
                }

                if ( numberOfLights > 1 )
                {
                    BOOL swap;

                    do
                    {
                        swap = NO;

                        for ( unsigned int i = 0; i < numberOfLights - 1; i++ )
                        {
                            if (   LSC_LIGHT(a,i  ).percentOfOverallRadiantPower
                                 < LSC_LIGHT(a,i+1).percentOfOverallRadiantPower )
                            {
                                ArLightsourceEntry  temp;

                                temp = LSC_LIGHT(a,i);
                                LSC_LIGHT(a,i) = LSC_LIGHT(a,i+1);
                                LSC_LIGHT(a,i+1) = temp;

                                swap = YES;
                            }
                        }
                    }
                    while( swap );
                }

                LSC_LIGHT(a,0).overallRadiantPowerPercentile =
                    LSC_LIGHT(a,0).percentOfOverallRadiantPower;
                arspectralintensity_i_init_i(
                    art_gv,
                    LSC_LIGHT(a,0).percentOfOverallSpectralPower,
                    LSC_LIGHT(a,0).overallSpectralPowerPercentile
                    );

                for ( unsigned int i = 1; i < numberOfLights; i++ )
                {
                    LSC_LIGHT(a,i).overallRadiantPowerPercentile =
                          LSC_LIGHT(a,i).percentOfOverallRadiantPower
                        + LSC_LIGHT(a,i-1).overallRadiantPowerPercentile;
                    arspectralintensity_ii_add_i(
                        art_gv,
                        LSC_LIGHT(a,i).percentOfOverallSpectralPower,
                        LSC_LIGHT(a,i-1).overallSpectralPowerPercentile,
                        LSC_LIGHT(a,i).overallSpectralPowerPercentile
                        );
                }
            }
        }

        UNLOCK_ADDITION_MUTEX;
    }
}

- (ArSpectralIntensity *) overallSpectralPower
{
    return overallSpectralPower[0];
}

- (double) overallRadiantPower
{
    return overallRadiantPower[0];
}

- (double) overallArea
{
    return overallArea;
}

- (unsigned int) numberOfLights
{
    return numberOfLights;
}

- (unsigned int) overallNumberOfPatches
{
    return overallNumberOfPatches;
}

- (void) prepareForUse
        : (ArcObject <ArpReporter> *) reporter
{
    [ reporter beginAction
        :   "Preparing lightsource collection for use"
        ];

    [ reporter printf
        :   "Number of individual lightsources: %d\n"
        ,   numberOfLights ];
    [ reporter printf
        :   "Overall number of lightsource patches: %d\n"
        ,   overallNumberOfPatches ];
    [ reporter printf
        :   "Overall surface area of all lightsources: %f\n"
        ,   overallArea ];
    [ reporter printf
        :   "Overall radiant power of all lightsources: %f\n"
        ,   overallRadiantPower ];

    [ reporter endAction ];
}

- (double) selectionProbabilityOfSource
        : ( ArNode *)               emissiveObject
        : ( ArSamplingRegion *)     samplingRegionOnEmissiveObject
        : ( id <ArpLightsource> *)  lightsource
{
    unsigned int  i = 0;

    while (   i < ( numberOfLights - 1 )
           && (id)[ light[i].source shape ] != (id)emissiveObject )
        i++;

    *lightsource = light[i].source;

    return
        [ light[i].source selectionProbabilityOfRegion
            :   samplingRegionOnEmissiveObject
            ];
}

- (double) selectionProbabilityOfSource
        : (const Pnt3D *)           queryLocationWorldspace
        : ( ArNode *)               emissiveObject
        : ( ArSamplingRegion *)     samplingRegionOnEmissiveObject
        : ( id <ArpLightsource> *)  lightsource
{
    unsigned int  i = 0;

    while (   i < ( numberOfLights - 1 )
           && (id)[ light[i].source shape ] != (id)emissiveObject )
        i++;

    *lightsource = light[i].source;

    return
        [ light[i].source selectionProbabilityOfRegion
            :   samplingRegionOnEmissiveObject
            :   queryLocationWorldspace
            ];
}

- (void) dealloc
{
    if ( ownsLights && light )
    {
        //   We release the source for altitude 0
        for ( unsigned int i = 0; i < numberOfLights; i++ )
            RELEASE_OBJECT(LSC_LIGHT(0,i).source);
    }

    FREE_ARRAY( light );

    [ super dealloc ];
}

- (void) _useLightsourcesOfOtherLSC
        : (ArnLightsourceCollection *) otherLSC
{
    ownsLights     = NO;
    numberOfSlots  = otherLSC->numberOfSlots;
    numberOfActualSlots  = otherLSC->numberOfActualSlots;
    numberOfLights = otherLSC->numberOfLights;
    light          = ALLOC_ARRAY( ArLightsourceEntry, numberOfActualSlots );

    for ( unsigned int i = 0; i < numberOfActualSlots; i++ )
        light[i] = otherLSC->light[i];
}

- (void) _copyLightsourcesOfOtherLSC
        : (ArnLightsourceCollection *) otherLSC
{
ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
    ownsLights     = YES;
    numberOfSlots  = otherLSC->numberOfSlots;
    numberOfLights = otherLSC->numberOfLights;
    light          = ALLOC_ARRAY( ArLightsourceEntry, numberOfSlots );

    for ( unsigned int i = 0; i < numberOfLights; i++ )
    {
        light[i].source = [ ((ArcObject *)otherLSC->light[i].source) copy ];
        light[i].percentOfOverallRadiantPower =
            otherLSC->light[i].percentOfOverallRadiantPower;
        light[i].overallRadiantPowerPercentile =
            otherLSC->light[i].overallRadiantPowerPercentile;
    }
}

- (id) copy
{
    ArnLightsourceCollection  * copiedInstance = [ super copy ];

    copiedInstance->overallArea = overallArea;
    copiedInstance->overallNumberOfPatches = overallNumberOfPatches;
    
    for ( int a = 0; a < PSM_ARRAYSIZE; a++ )
    {
        copiedInstance->overallRadiantPower[a] = overallRadiantPower[a];
        copiedInstance->overallSpectralPower[a] = arspectralintensity_alloc( art_gv );
        copiedInstance->overallLowresSpectralPower[a] = arlightintensity_alloc( art_gv );

        arspectralintensity_i_init_i(
              art_gv,
              overallSpectralPower[a],
              copiedInstance->overallSpectralPower[a]
            );
        arlightintensity_i_init_i(
            art_gv,
            overallLowresSpectralPower[a],
            copiedInstance->overallLowresSpectralPower[a]
            );
    }
    
    //   Shallow copy! At this stage, there is no point in duplicating light
    //   source lists. If, at some later stage, light source lists become in
    //   any way dynamic and/or thread-specific, this has to change to
    //   use _copyLightsources instead!

    [ copiedInstance _useLightsourcesOfOtherLSC
        :   self
        ];

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnLightsourceCollection  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

@end

// ===========================================================================
