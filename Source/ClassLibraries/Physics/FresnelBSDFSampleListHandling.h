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

ART_MODULE_INTERFACE(FresnelBSDFSampleListHandling)

#import "ART_Scenegraph.h"

#import "Fresnel.h"

//  these are the new functions that will remain
//  most of the others will be scrapped
//  (eventually, there will be hero versions of these functions as well)

void add_fresnel_reflection_sample_to_bsdfsamplelist(
              ART_GV                         * art_gv,
              struct ArBSDFSampleFreelists   * bsdfsampleFreelists,
        const ArcIntersection                * incomingDirectionAndLocation,
        const ArPathDirection                  pathDirection,
        const ArWavelength                   * wavelength,
              struct ArBSDFSampleList        * bsdfsampleList
        );

int add_fresnel_refraction_sample_to_bsdfsamplelist(
              ART_GV                         * art_gv,
              struct ArBSDFSampleFreelists   * bsdfsampleFreelists,
        const ArcIntersection                * incomingDirectionAndLocation,
        const ArPathDirection                  pathDirection,
        const ArWavelength                   * wavelength,
              struct ArBSDFSampleList        * bsdfsampleList
        );

int add_birefringent_fresnel_refraction_samples_to_bsdfsamplelist(
              ART_GV                         * art_gv,
              struct ArBSDFSampleFreelists   * bsdfsampleFreelists,
        const ArcIntersection                * incomingDirectionAndLocation,
        const ArPathDirection                  pathDirection,
        const ArWavelength                   * wavelength,
              struct ArBSDFSampleList        * bsdfsampleList
        );


/* ---------------------------------------------------------------------------
    'fresnel_absorbance'

    Computes the absorbance (i.e. "1 minus whatever is being reflected")
    for a given incoming direction on a smooth Fresnel interface.
--------------------------------------------------------------------------- */

void fresnel_absorbance(
              ART_GV           * art_gv,
        const ArcIntersection  * incomingDirectionAndLocation,
        const ArPathDirection    pathDirection,
              ArAttenuation    * fr
        );

void fresnel_absorbance_sample(
              ART_GV                 * art_gv,
        const ArcSurfacePoint        * absorbanceLocation,
        const ArDirectionCosine      * outgoingDirection,
        const ArPathDirection          pathDirection,
        const ArWavelength           * wavelength,
              ArAttenuationSample    * fr
        );
// ===========================================================================
