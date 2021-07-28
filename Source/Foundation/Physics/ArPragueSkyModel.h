/*
This source is published under the following 3-clause BSD license.

Copyright (c) 2016 <anonymous authors of SIGGRAPH paper submision 0155>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * None of the names of the contributors may be used to endorse or promote
      products derived from this software without specific prior written
      permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


/* ============================================================================

1.0   January 17th, 2017
      Initial release.

============================================================================ */

#ifndef _ARPRAGUESKYMODEL_H_
#define _ARPRAGUESKYMODEL_H_

#define PSM_SUN_RADIUS                          0.2667 DEGREES
#define PSM_PLANET_RADIUS                       6378000.0
#define PSM_PLANET_RADIUS_SQR                   PSM_PLANET_RADIUS * PSM_PLANET_RADIUS

#define PSM_MIN_ALTITUDE                              0.0
#define PSM_MAX_ALTITUDE                          15000.0
#define PSM_LIGHTCOLLECTION_VERTICAL_STEPSIZE       250.0
#define PSM_ARRAYSIZE                                61 // = PSM_MAX_ALTITUDE / PSM_LIGHTCOLLECTION_VERTICAL_STEPSIZE + 1

#define ARPRAGUESKYMODEL_USE_NEW

#include "ART_Foundation_Geometry.h"
#include "ART_Foundation_ColourAndSpectra.h"

#ifdef ARPRAGUESKYMODEL_USE_NEW

//   This computes the canonical angles of the model from
//   a normalised view vector and solar elevation.

void arpragueskymodel_compute_angles(
        const Pnt3D   * viewpoint,
        const Vec3D   * viewDirection,
        const double    groundLevelSolarElevationAtOrigin,
        const double    groundLevelSolarAzimuthAtOrigin,
              double  * solarElevationAtViewpoint,
              double  * altitudeOfViewpoint,
              double  * theta,
              double  * gamma,
              double  * shadow,
              double  * zero
        );

//   One blob of floats for each wavelength and task

typedef struct ArPragueSkyModelState
{
	// Radiance metadata

	int turbidities;
	double * turbidity_vals;

	int albedos;
	double * albedo_vals;

	int altitudes;
	double * altitude_vals;

	int elevations;
	double * elevation_vals;

	int channels;
	double channel_start;
	double channel_width;

	int tensor_components;

	int sun_nbreaks;
	int sun_offset;
	int sun_stride;
	double * sun_breaks;

	int zenith_nbreaks;
	int zenith_offset;
	int zenith_stride;
	double * zenith_breaks;

	int emph_nbreaks;
	int emph_offset;
	double * emph_breaks;

	int total_coefs_single_config; // this is for one specific configuration
	int total_coefs_all_configs;
	int total_configs;

	// Radiance data

	double * radiance_dataset;



    // Tranmittance metadata

	int     trans_n_a;
	int     trans_n_d;
	int     trans_turbidities;
	int     trans_altitudes;
	int     trans_rank;
	float * transmission_altitudes;
	float * transmission_turbities;

    // Tranmittance data

	float * transmission_dataset_U;
	float * transmission_dataset_V;



    // Polarisation metadata

	int tensor_components_pol;
     
    int sun_nbreaks_pol;
	int sun_offset_pol;
	int sun_stride_pol;
	double * sun_breaks_pol;

	int zenith_nbreaks_pol;
	int zenith_offset_pol;
	int zenith_stride_pol;
	double * zenith_breaks_pol;

	int total_coefs_single_config_pol; // this is for one specific configuration
	int total_coefs_all_configs_pol;

	// Polarisation data

	double * polarisation_dataset;
}
ArPragueSkyModelState;

ArPragueSkyModelState  * arpragueskymodelstate_alloc_init(
        const char  * library_path
        );

void arpragueskymodelstate_free(
        ArPragueSkyModelState  * state
        );

//   theta  - zenith angle
//   gamma  - sun angle
//   shadow - angle from the shadow point, which is further 90 degrees above the sun
//   zero   - angle from the zero point, which lies at the horizon below the sun
//   altitude
//   wavelength

double arpragueskymodel_radiance(
        const ArPragueSkyModelState  * state,
        const double                   theta,
        const double                   gamma,
        const double                   shadow,
        const double                   zero,
        const double                   elevation,
        const double                   altitude,
        const double                   turbidity,
        const double                   albedo,
        const double                   wavelength
        );

void arpragueskymodel_radiance_hero(
        const ART_GV                 * art_gv,
        const ArPragueSkyModelState  * state,
        const double                   gamma,
        const double                   shadow,
        const double                   zero,
        const double                   elevation,
        const double                   altitude,
        const double                   turbidity,
        const double                   albedo,
        const ArWavelength           * wavelength,
              ArSpectralSample       * result
        );

/* ----------------------------------------------------------------------------

    arpragueskymodel_solar_radiance
    ---------------------------

    This computes transmittance between a point at 'altitude' and infinity in
    the direction 'theta' at a wavelength 'wavelength'.

---------------------------------------------------------------------------- */


double arpragueskymodel_solar_radiance(
        const ArPragueSkyModelState  * state,
        const double                   theta,
        const double                   gamma,
        const double                   shadow,
        const double                   zero,
        const double                   elevation,
        const double                   altitude,
        const double                   turbidity,
        const double                   albedo,
        const double                   wavelength
        );

double arpragueskymodel_polarisation(
        const ArPragueSkyModelState  * state,
        const double                   theta,
        const double                   gamma,
        const double                   elevation,
        const double                   altitude,
        const double                   turbidity,
        const double                   albedo,
        const double                   wavelength
        );

void arpragueskymodel_polarisation_hero(
        const ART_GV                 * art_gv,
        const ArPragueSkyModelState  * state,
        const double                   theta,
        const double                   gamma,
        const double                   elevation,
        const double                   altitude,
        const double                   turbidity,
        const double                   albedo,
        const ArWavelength           * wavelength,
              ArSpectralSample       * result
        );

/* ----------------------------------------------------------------------------

    arpragueskymodel_tau
    ------------------------------

    This computes transmittance between a point at 'altitude' and infinity in
    the direction 'theta' at a wavelength 'wavelength'.

---------------------------------------------------------------------------- */

void arpragueskymodel_toAD(
	double theta,
	double distance,
	double altitude,
	double *a,
	double *d
);

double arpragueskymodel_tau(
        const ArPragueSkyModelState  * state,
        const double                   theta,
        const double                   altitude,
        const double                   turbidity,
        const double                   wavelength,
        const double                   distance
        );

void arpragueskymodel_tau_hero(
        const ART_GV                 * art_gv,
        const ArPragueSkyModelState  * state,
        const double                   theta,
        const double                   altitude,
        const double                   turbidity,
        const ArWavelength           * wavelength,
        const double                   distance,
              ArSpectralSample       * result
        );

#else

//   This computes the canonical angles of the model from
//   a normalised view vector and solar elevation.

void arpragueskymodel_compute_angles(
        const Pnt3D   * viewpoint,
        const Vec3D   * viewDirection,
        const double    groundLevelSolarElevationAtOrigin,
        const double    groundLevelSolarAzimuthAtOrigin,
              double  * solarElevationAtViewpoint,
              double  * altitudeOfViewpoint,
              double  * theta,
              double  * gamma,
              double  * shadow,
              double  * zero
        );

//   One blob of floats for each wavelength and task

typedef struct ArPragueSkyModelState
{
    double  * radiance_dataset[11];
    double  * polarisation_dataset[11];
    float   * transmission_dataset;
}
ArPragueSkyModelState;

ArPragueSkyModelState  * arpragueskymodelstate_alloc_init(
        const char  * library_path
        );

void arpragueskymodelstate_free(
        ArPragueSkyModelState  * state
        );

//   theta  - zenith angle
//   gamma  - sun angle
//   shadow - angle from the shadow point, which is further 90 degrees above the sun
//   zero   - angle from the zero point, which lies at the horizon below the sun
//   altitude
//   wavelength

double arpragueskymodel_radiance(
        const ArPragueSkyModelState  * state,
        const double                   theta,
        const double                   gamma,
        const double                   shadow,
        const double                   zero,
        const double                   elevation,
        const double                   altitude,
        const double                   turbidity,
        const double                   albedo,
        const double                   wavelength
        );

/* ----------------------------------------------------------------------------

    arpragueskymodel_solar_radiance
    ---------------------------

    This computes transmittance between a point at 'altitude' and infinity in
    the direction 'theta' at a wavelength 'wavelength'.

---------------------------------------------------------------------------- */


double arpragueskymodel_solar_radiance(
        const ArPragueSkyModelState  * state,
        const double                   theta,
        const double                   gamma,
        const double                   shadow,
        const double                   zero,
        const double                   elevation,
        const double                   altitude,
        const double                   turbidity,
        const double                   albedo,
        const double                   wavelength
        );

double arpragueskymodel_polarisation(
        const ArPragueSkyModelState  * state,
        const double                   theta,
        const double                   gamma,
        const double                   elevation,
        const double                   altitude,
        const double                   turbidity,
        const double                   albedo,
        const double                   wavelength
        );

/* ----------------------------------------------------------------------------

    arpragueskymodel_tau
    ------------------------------

    This computes transmittance between a point at 'altitude' and infinity in
    the direction 'theta' at a wavelength 'wavelength'.

---------------------------------------------------------------------------- */

double arpragueskymodel_tau(
        const ArPragueSkyModelState  * state,
        const double                   theta,
        const double                   altitude,
        const double                   turbidity,
        const double                   wavelength,
        const double                   distance
        );

#endif // ARPRAGUESKYMODEL_USE_NEW

#endif // _ARPRAGUESKYMODEL_H_
