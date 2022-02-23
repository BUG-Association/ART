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

#ifndef _ARPVOLUMEINTEGRATOR_H_
#define _ARPVOLUMEINTEGRATOR_H_

#include "ART_Foundation.h"

ART_MODULE_INTERFACE(ArpVolumeIntegrator)

#import "ArNode.h"

@protocol ArpVolumeIntegrator <ArpNode>

/**
 * Sample a distance inside the volume
 * @param   volume
 * @param   ray
 * @param   wavelength
 * @param   pathDirection
 * @param   allowTermination
 * @param   distance                in and out, if estimation was stopped along
 *                                  the way, this holds the sampled distance
 * @param   transmittanceSample     not the true estimate, already divided by
 *                                  portions of PDF
 * @param   volumeProbability
 * @param   distanceProbability
 * @return  YES if the ray stopped into the volume, NO otherwise
 */
- (BOOL) sampleDistance
        : (      ArNode <ArpVolumeMaterial>     *) volume
        : (const Ray3D                          *) ray
        : (const ArWavelength                   *) wavelength
        : (      ArPathDirection                 ) pathDirection
        : (      double                         *) distance
        : (      ArSpectralSample               *) transmittanceSample
        : (      ArPDFValue                     *) volumeProbability
        : (      ArPDFValue                     *) distanceProbability
        : (      ArcObject <ArpRandomGenerator> *) randomGenerator
        ;

/**
 * Estimate the transmittance of a given volume
 * @param   volume
 * @param   ray
 * @param   wavelength
 * @param   pathDirection
 * @param   allowTermination
 * @param   distance                Distance to where to integrate
 * @param   transmittanceSample     Not the true estimate, already divided by
 *                                  portions of PDF
 * @param   volumeProbability
 * @param   distanceProbability
 */
- (void) estimateTransmittance
        : (      ArNode <ArpVolumeMaterial>     *) volume
        : (const Ray3D                          *) ray
        : (const ArWavelength                   *) wavelength
        : (      ArPathDirection                 ) pathDirection
        : (      double                         *) distance
        : (      ArSpectralSample               *) transmittanceSample
        : (      ArPDFValue                     *) volumeProbability
        : (      ArPDFValue                     *) distanceProbability
        : (      ArcObject <ArpRandomGenerator> *) randomGenerator
        ;

- (BOOL) homogeneousSupport
        ;

- (BOOL) heterogeneousSupport
        ;

@end

#endif // _ARPVOLUMEINTEGRATOR_H_

// ===========================================================================

