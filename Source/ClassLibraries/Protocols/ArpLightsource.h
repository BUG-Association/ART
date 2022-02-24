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

ART_MODULE_INTERFACE(ArpLightsource)

#import "ART_MiscellaneousStructs.h"

/* ===========================================================================


    Lightsources in ART
    ===================

    Several layers of abstraction are used to deal with lightsources in ART
    in a way that is compatible with the notions of as many different
    rendering paradigms as possible. This text attempts to explain the
    ideas behind this scheme; all related implementation files refer to this
    text for a high-level explanation of their content.


    The Basic Idea
    --------------

    In order to keep their design free from data structures which are
    specific to any given rendering algorithm, ART scenegraphs do not
    contain explicit lightsource objects. The only way to specify a
    luminaire is to attach an emissive surface attribute to an object.

    The idea behind this concept is that the permanent, stored description
    of the scene should be as physically correct as possible, and that one
    should only construct some fitting intermediate representation for each
    of these emissive objects at rendering time. This allows for a clean
    treatment of different rendering methods insofar as each one can get
    optimal support from their intermediary lightsource object of choice.

    Consequently, one of the preliminary actions of any rendering pass is
    to walk through the scenegraph, identify all emissive objects, and to
    create a collection of suitable temporary lightsource objects which
    can be constructed for each of these emitters.


    Lightsource Subsystem Protocols
    -------------------------------

    ArpLightsource

    The individual classes which are used to represent the various types
    of luminaire (such as skylights, area lightsources, point lightsources)
    conform to this protocol.


    ArpLightsourceCollection

    This interface is used for classes which act as collections of all
    lightsources in a scene. The initial luminaire indentification pass
    over the scenegraph places an object which conforms to this protocol
    back on the stack.


========================================================================aw= */


typedef enum ArLightsourceType
{
    arlightsourcetype_point           = 0x00,
    arlightsourcetype_area            = 0x01,
    arlightsourcetype_infinitesphere  = 0x02,
    arlightsourcetype_ambient         = 0x03,
    arlightsourcetype_complex_skydome = 0x04
}
ArLightsourceType;

@class ArNode;
@protocol ArpShape;

@protocol ArpLightsource

- (ArNode <ArpShape> *) shape
        ;

- (double) area
        ;

- (unsigned int) numberOfPatches
        ;

- (ArLightsourceType) lightsourceType
        ;

- (ArSpectralIntensity *) spectralPower
        : (const Pnt3D *) queryLocationWorldspace
        ;

- (double) radiantPower
        : (const Pnt3D *) queryLocationWorldspace
        ;

- (void) setPercentOfOverallRadiantPower
        : (const Pnt3D *) queryLocationWorldspace
        : (double) newPercentOfOverallRadiantPower
        ;

- (double) selectionProbabilityOfRegion
        : ( ArSamplingRegion *)  lightsourceSamplingRegion
        ;

- (double) selectionProbabilityOfRegion
        : ( ArSamplingRegion *)  lightsourceSamplingRegion
        : (const Pnt3D *) queryLocationWorldspace
        ;

/* ---------------------------------------------------------------------------
    'getSampleProbability'
    Some lightsources (e.g. skydomes) provide the probability of sampling
    a certain region; this information is used by the path tracer with
    light source sampling
--------------------------------------------------------------------------- */

- (BOOL) getSampleProbability
        : ( ArSamplingRegion *)  samplingRegion
        : ( double * ) probability
        ;

@end


@protocol ArpLightsourceCollection

- (void) addLightsource
        : (ArcObject <ArpLightsource> *) newLightsource
        : (ArcObject <ArpReporter> *) reporter
        ;

- (unsigned int) overallNumberOfPatches
        ;

- (unsigned int) numberOfLights
        ;

- (double) overallArea
        ;

- (double) overallRadiantPower
        ;

- (ArSpectrum500 *) overallSpectralPower
        ;

- (void) prepareForUse
        : (ArcObject <ArpReporter> *) reporter
        ;

- (double) selectionProbabilityOfSource
        : ( ArNode *)               emissiveObject
        : ( ArSamplingRegion *)     samplingRegionOnEmissiveObject
        : ( id <ArpLightsource> *)  lightsource
        ;

- (double) selectionProbabilityOfSource
        : (const Pnt3D *)           queryLocationWorldspace
        : ( ArNode *)               emissiveObject
        : ( ArSamplingRegion *)     samplingRegionOnEmissiveObject
        : ( id <ArpLightsource> *)  lightsource
        ;

@end

// ===========================================================================
