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

#ifndef _ARNSKYDOME_H_
#define _ARNSKYDOME_H_

#include "ART_Foundation.h"
#import  "ART_Scenegraph.h"

ART_MODULE_INTERFACE(ArnSkyModel)

/*
    This superclass of the other sky dome model classes does not so much
    provide shared functionality, but rather shared data that is common to all
    of them.

    Sky dome nodes are ternary nodes. The subnodes are as follows:

    - subnode 0: the sky dome trafo (rotation of the skydome)
    - subnode 1: an infinite sphere instance (the ray intersection object)
    - subnode 2: the ground albedo colour

    Not all of the instance variables are used on all models: for instance, 
    the Preetham sky model does not use the ground albedo node, and the
    Hosek model cannot supply polarised output.
*/

#define SKYMODEL_TRAFO_SUBNODE \
    ((ArNode <ArpTrafo3D>*) ARNQUATERNARY_SUBNODE_2)

#define SKYMODEL_GROUND_ALBEDO_SUBNODE \
    ((ArNode <ArpSpectrum>*) ARNQUATERNARY_SUBNODE_3)

#define SKYMODEL_DEFAULT_SUN_RADIUS     0.251 DEGREES


@interface ArnSkyModel
        : ArnQuaternary
        < ArpConcreteClass, ArpCoding, ArpSkyModel >
{
    double        solarRadius;
    double        solarElevation;
    double        solarAzimuth;
    double        solarSurfaceTemperature;
    double        solarIntensityFactor;

    double        atmosphericTurbidity;

    BOOL          polarisedOutput;
    
    //   Ground albedo in our current ISR

    ArSpectrum  * groundAlbedo;

}

- (id) init
        : (double) newSolarRadius
        : (double) newSolarElevation
        : (double) newSolarAzimuth
        : (double) newSolarSurfaceTemperature
        : (double) newSolarIntensityFactor
        : (double) newAtmosphericTurbidity
        : (BOOL) newPolarisedOutput
        : (ArNode <ArpSpectrum> *) newGroundAlbedo
        : (ArNode <ArpTrafo3D> *) newTrafo
        ;

@end

#endif // _ARNSKYDOME_H_

// ===========================================================================
