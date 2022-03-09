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

#define ART_MODULE_NAME     FresnelBSDFSampleListHandling

#import "FresnelBSDFSampleListHandling.h"
#import "SurfaceMaterialMacros.h"
#import "Fresnel.h"
#include "FoundationAssertionMacros.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


void fresnel_absorbance(
              ART_GV           * art_gv,
        const ArcIntersection  * incomingDirectionAndLocation,
        const ArPathDirection    pathDirection,
              ArAttenuation    * fr
        )
{
    ASSERT_ALLOCATED_ATTENUATION( fr )

    if ( ARCPHASEINTERFACE_HAS_REALVALUED_IOR(
           *ARCSURFACEPOINT_PHASEINTERFACE(incomingDirectionAndLocation) ) )
        fresnel_absorbance_realvalued_IOR(
            art_gv,
            incomingDirectionAndLocation,
            pathDirection,
            fr
            );
    else
        fresnel_absorbance_complex_IOR(
            art_gv,
            incomingDirectionAndLocation,
            pathDirection,
            fr
            );

    ASSERT_VALID_ATTENUATION( fr )
}

void fresnel_absorbance_sample(
              ART_GV                 * art_gv,
        const ArcSurfacePoint        * absorbanceLocation,
        const ArDirectionCosine      * outgoingDirection,
        const ArPathDirection          pathDirection,
        const ArWavelength           * wavelength,
              ArAttenuationSample    * fr
        )
{
    ASSERT_ALLOCATED_ATTENUATION_SAMPLE( fr )

    if ( ARCPHASEINTERFACE_HAS_REALVALUED_IOR(
           *ARCSURFACEPOINT_PHASEINTERFACE(absorbanceLocation) ) )
        fresnel_absorbance_sample_realvalued_IOR(
            art_gv,
            absorbanceLocation,
            outgoingDirection,
            pathDirection,
            wavelength,
            fr
            );
    else
        fresnel_absorbance_sample_complex_IOR(
            art_gv,
            absorbanceLocation,
            outgoingDirection,
            pathDirection,
            wavelength,
            fr
            );

    ASSERT_VALID_ATTENUATION_SAMPLE( fr )
}

void add_fresnel_reflection_sample_to_bsdfsamplelist(
              ART_GV                         * art_gv,
              struct ArBSDFSampleFreelists   * bsdfsampleFreelists,
        const ArcIntersection                * incomingDirectionAndLocation,
        const ArPathDirection                  pathDirection,
        const ArWavelength                   * wavelength,
              struct ArBSDFSampleList        * bsdfsampleList
        )
{
    ArBSDFSample  * bsdfsample =
        arbsdfsample_alloc_from_freelist( bsdfsampleFreelists );

    ASSERT_ALLOCATED_BSDFSAMPLE( bsdfsample )

    ardirectioncosine_reflect(
          art_gv,
        & INCOMING_DIRECTION_COSINE_WORLDSPACE,
        & SURFACE_NORMAL_WORLDSPACE,
        & ARBSDFSAMPLE_DIRECTION_COSINE(*bsdfsample)
        );

    ARBSDFSAMPLE_TYPE(*bsdfsample)        = arbsdfsample_specular;
    ARBSDFSAMPLE_PROBABILITY(*bsdfsample) = ARPDFVALUE_UNIT_DIRAC;

    if ( ARCPHASEINTERFACE_HAS_REALVALUED_IOR(
           *ARCSURFACEPOINT_PHASEINTERFACE(incomingDirectionAndLocation) ) )
        fresnel_reflective_attenuation_sample_dielectric(
              art_gv,
              incomingDirectionAndLocation,
              pathDirection,
              wavelength,
              ARBSDFSAMPLE_ATTENUATION(*bsdfsample)
            );
    else
        fresnel_reflective_attenuation_sample_conductor(
              art_gv,
              incomingDirectionAndLocation,
              pathDirection,
              wavelength,
              ARBSDFSAMPLE_ATTENUATION(*bsdfsample)
            );

    ASSERT_VALID_BSDFSAMPLE( bsdfsample )

    arbsdfsamplelist_push(bsdfsampleList, bsdfsample);
}

int add_fresnel_refraction_sample_to_bsdfsamplelist(
              ART_GV                         * art_gv,
              struct ArBSDFSampleFreelists   * bsdfsampleFreelists,
        const ArcIntersection                * incomingDirectionAndLocation,
        const ArPathDirection                  pathDirection,
        const ArWavelength                   * wavelength,
              struct ArBSDFSampleList        * bsdfsampleList
        )
{
    //   Step zero: we check whether the dot NT thing is positive
    
    double  inverseIOR =
        ARCPHASEINTERFACE_INV_IOR_AT_WAVELENGTH(
            *ARCINTERSECTION_PHASEINTERFACE(incomingDirectionAndLocation),
            ARWL_WI(*wavelength,0)
            );
    
    double  squaredMinusDotNT =
              1.0
            -     M_SQR( inverseIOR )
                * ( 1.0 - M_SQR( INCOMING_COSINE_WORLDSPACE ) );

    if ( squaredMinusDotNT < 0.0 )
        return 0;

    double  dotNT = -sqrt( squaredMinusDotNT );

    ArBSDFSample  * bsdfsample =
        arbsdfsample_alloc_from_freelist( bsdfsampleFreelists );

    ardirectioncosine_refract(
          art_gv,
        & INCOMING_DIRECTION_COSINE_WORLDSPACE,
        & SURFACE_NORMAL_WORLDSPACE,
          inverseIOR,
          dotNT,
        & ARBSDFSAMPLE_DIRECTION_COSINE(*bsdfsample)
        );

    ARBSDFSAMPLE_TYPE(*bsdfsample)        = arbsdfsample_specular;
    ARBSDFSAMPLE_PROBABILITY(*bsdfsample) = ARPDFVALUE_UNIT_DIRAC;

    fresnel_refractive_attenuation_single_sample(
          art_gv,
          incomingDirectionAndLocation,
          pathDirection,
        & ARBSDFSAMPLE_DIRECTION_COSINE(*bsdfsample),
          wavelength,
          ARBSDFSAMPLE_ATTENUATION(*bsdfsample)
        );

    ASSERT_VALID_BSDFSAMPLE( bsdfsample )

    arbsdfsamplelist_push(bsdfsampleList, bsdfsample);

    return 1;
}

int add_birefringent_fresnel_refraction_samples_to_bsdfsamplelist(
              ART_GV                         * art_gv,
              struct ArBSDFSampleFreelists   * bsdfsampleFreelists,
        const ArcIntersection                * incomingDirectionAndLocation,
        const ArPathDirection                  pathDirection,
        const ArWavelength                   * wavelength,
              struct ArBSDFSampleList        * bsdfsampleList
        )
{
    (void) art_gv;
    (void) bsdfsampleFreelists;
    (void) incomingDirectionAndLocation;
    (void) pathDirection;
    (void) wavelength;
    (void) bsdfsampleList;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR;
    return 0;
}

// ===========================================================================
