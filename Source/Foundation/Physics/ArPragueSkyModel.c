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

1.0   January 15th, 2016
      Initial release.

============================================================================ */

#include "ArPragueSkyModel.h"
#include "ArPragueSkyModelData_Spectral.h"
#include "ArPragueSkyModelData_TransCoefficients.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//   Some macro definitions that occur elsewhere in ART, and that have to be
//   replicated to make this a stand-alone module.

#ifndef NULL
#define NULL                         NULL
#endif

#ifndef MATH_PI
#define MATH_PI                     3.141592653589793
#endif

#ifndef MATH_DEG_TO_RAD
#define MATH_DEG_TO_RAD             ( MATH_PI / 180.0 )
#endif

#ifndef MATH_RAD_TO_DEG
#define MATH_RAD_TO_DEG             ( 180.0 / MATH_PI )
#endif

#ifndef DEGREES
#define DEGREES                     * MATH_DEG_TO_RAD
#endif

#ifndef TERRESTRIAL_SOLAR_RADIUS
#define TERRESTRIAL_SOLAR_RADIUS    ( ( 0.51 DEGREES ) / 2.0 )
#endif

#ifndef ALLOC
#define ALLOC(_struct)              ((_struct *)malloc(sizeof(_struct)))
#endif

const int elevations = 31;
const int altitudes = 21;
const int albedos = 2;

const double polarisation_max_breaks[] = {0, 5.235988e-01, 1.047198e+00, 1.570796e+00, 2.094395e+00, 2.617994e+00, 3.141593e+00};
const double polarisation_ratio_breaks[] = {0, 7.853982e-01, 1.396263e+00, 1.483530e+00, 1.553343e+00, 1.570796e+00, 1.588250e+00, 1.658063e+00, 1.745329e+00, 2.356194e+00, 3.141593e+00};

const int polarisation_max_nbreaks = 6;
const int polarisation_ratio_nbreaks = 10;

const int polarisation_max_offset = 0;
int polarisation_ratio_offset;
int total_polarisation_coefs_single_config; // this is for one specific configuration

int total_polarisation_coefs_all_configs;

const int transmittance_coefs = 5;
const int transmittance_altitudes = 86;
int total_transmittance_values;
const double planet_radius = 6378000.0;

const int transsvdrank = 32;

ArPragueSkyModelState  * arpragueskymodelstate_alloc_init(
        const char  * library_path
        )
{
    ArPragueSkyModelState  * state = ALLOC(ArPragueSkyModelState);
	
	char breaks_filename[1024];
	sprintf(breaks_filename, "%s/SkyModel/breaks_c.dat", library_path);
	FILE* breaks_handle = fopen(breaks_filename, "rb");
	fread(&state->background_nbreaks, sizeof(int), 1, breaks_handle);
	fread(&state->solar_max_nbreaks, sizeof(int), 1, breaks_handle);
	fread(&state->solar_ratio_nbreaks, sizeof(int), 1, breaks_handle);
	fread(&state->backglow_vertical_nbreaks, sizeof(int), 1, breaks_handle);
	fread(&state->backglow_ratio_nbreaks, sizeof(int), 1, breaks_handle);
	fread(&state->frontglow_vertical_nbreaks, sizeof(int), 1, breaks_handle);
	fread(&state->frontglow_ratio_nbreaks, sizeof(int), 1, breaks_handle);
	fread(&state->shadow_nbreaks, sizeof(int), 1, breaks_handle);
	fread(&state->shadow_v_nbreaks, sizeof(int), 1, breaks_handle);
	
	state->background_breaks = ALLOC_ARRAY(double, state->background_nbreaks);
	state->solar_max_breaks = ALLOC_ARRAY(double, state->solar_max_nbreaks);
	state->solar_ratio_breaks = ALLOC_ARRAY(double, state->solar_ratio_nbreaks);
	state->backglow_vertical_breaks = ALLOC_ARRAY(double, state->backglow_vertical_nbreaks);
	state->backglow_ratio_breaks = ALLOC_ARRAY(double, state->backglow_ratio_nbreaks);
	state->frontglow_vertical_breaks = ALLOC_ARRAY(double, state->frontglow_vertical_nbreaks);
	state->frontglow_ratio_breaks = ALLOC_ARRAY(double, state->frontglow_ratio_nbreaks);
	state->shadow_breaks = ALLOC_ARRAY(double, state->shadow_nbreaks);
	state->shadow_v_breaks = ALLOC_ARRAY(double, state->shadow_v_nbreaks);
	
	fread(state->background_breaks, sizeof(double), state->background_nbreaks, breaks_handle);
	fread(state->solar_max_breaks, sizeof(double), state->solar_max_nbreaks, breaks_handle);
	fread(state->solar_ratio_breaks, sizeof(double), state->solar_ratio_nbreaks, breaks_handle);
	fread(state->backglow_vertical_breaks, sizeof(double), state->backglow_vertical_nbreaks, breaks_handle);
	fread(state->backglow_ratio_breaks, sizeof(double), state->backglow_ratio_nbreaks, breaks_handle);
	fread(state->frontglow_vertical_breaks, sizeof(double), state->frontglow_vertical_nbreaks, breaks_handle);
	fread(state->frontglow_ratio_breaks, sizeof(double), state->frontglow_ratio_nbreaks, breaks_handle);
	fread(state->shadow_breaks, sizeof(double), state->shadow_nbreaks, breaks_handle);
	fread(state->shadow_v_breaks, sizeof(double), state->shadow_v_nbreaks, breaks_handle);
    
    fclose(breaks_handle);

	// calculate offsets and breaks
	state->background_offset = 0;
	state->solar_max_offset = state->background_offset + 4 * state->background_nbreaks - 4;
	state->solar_ratio_offset = state->solar_max_offset + 4 * state->solar_max_nbreaks - 4;
	state->backglow_vertical_offset = state->solar_ratio_offset + 4 * state->solar_ratio_nbreaks - 4;
	state->backglow_ratio_offset = state->backglow_vertical_offset + 4 * state->backglow_vertical_nbreaks - 4;
	state->frontglow_vertical_offset = state->backglow_ratio_offset + 4 * state->backglow_ratio_nbreaks - 4;
	state->frontglow_ratio_offset = state->frontglow_vertical_offset + 4 * state->frontglow_vertical_nbreaks - 4;
	state->shadow_offset = state->frontglow_ratio_offset + 4 * state->frontglow_ratio_nbreaks - 4;
	state->shadow_v_offset = state->shadow_offset + 4 * state->shadow_nbreaks - 4;
	state->total_coefs_single_config = state->shadow_v_offset + 4 * state->shadow_v_nbreaks - 4; // this is for one specific configuration
	
	state->total_coefs_all_configs = state->total_coefs_single_config * elevations * altitudes * albedos;
	
	polarisation_ratio_offset = polarisation_max_offset + 4 * polarisation_max_nbreaks;
	total_polarisation_coefs_single_config = polarisation_ratio_offset + 4 * polarisation_ratio_nbreaks; // this is for one specific configuration
	total_polarisation_coefs_all_configs = total_polarisation_coefs_single_config * elevations * altitudes * albedos;
	total_transmittance_values = 11 * 11 * altitudes * transmittance_altitudes * transmittance_coefs;

    for (int wl = 0; wl < 11; ++wl)
    {
        char filename[1024];
        
        sprintf(filename, "%s/SkyModel/params_t%d_wl%d.dat", library_path, 4, wl+1);
        
        FILE* handle = fopen(filename, "rb");
        
        state->radiance_dataset[wl] = ALLOC_ARRAY(double,state->total_coefs_all_configs);
        
        size_t s = fread(state->radiance_dataset[wl], sizeof(double), state->total_coefs_all_configs, handle);
        fclose(handle);
        
        sprintf(filename, "%s/SkyModel/polarisation_params_t%d_wl%d.dat", library_path, 4, wl+1);
        
        handle = fopen(filename, "rb");
        
        state->polarisation_dataset[wl] = (double*)malloc(sizeof(double) * total_polarisation_coefs_all_configs);
        fread(state->polarisation_dataset[wl], sizeof(double), total_polarisation_coefs_all_configs, handle);
        fclose(handle);

    }
    
    // Read in transmittance
    char filenametransmittance[1024];
    
    sprintf(filenametransmittance, "%s/SkyModel/SVDFit32N.dat", library_path);
    
    FILE* handletrans = fopen(filenametransmittance, "rb");
    
    state->transmission_dataset = ALLOC_ARRAY(float, total_transmittance_values);
    
    fread(state->transmission_dataset, sizeof(float), total_transmittance_values, handletrans);
    
    fclose(handletrans);
    // Done

    return state;
}

void arpragueskymodelstate_free(
        ArPragueSkyModelState  * state
        )
{
    free(state->background_breaks);
	free(state->solar_max_breaks);
	free(state->solar_ratio_breaks);
	free(state->backglow_vertical_breaks);
	free(state->backglow_ratio_breaks);
	free(state->frontglow_vertical_breaks);
	free(state->frontglow_ratio_breaks);
	free(state->shadow_breaks);
	free(state->shadow_v_breaks);
	
	for (int wl = 1; wl < 11; ++wl)
    {
        free(state->radiance_dataset[wl]);
        free(state->polarisation_dataset[wl]);
    }

    free(state->transmission_dataset);
    
    FREE(state);
}

#define PLANET_RADIUS       6378000.0 METER

void arpragueskymodel_compute_altitude_and_elevation(
        const Pnt3D   * viewpoint,
        const double    groundLevelSolarElevationAtOrigin,
        const double    groundLevelSolarAzimuthAtOrigin,
              double  * solarElevationAtViewpoint,
              double  * altitudeOfViewpoint,
              Vec3D   * directionToPlanet
        )
{
    Pnt3D  centerOfTheEarth = PNT3D(0,0,-PLANET_RADIUS);

    Vec3D  directionToPlanetCenter;
    
    vec3d_pp_sub_v(
        & centerOfTheEarth,
          viewpoint,
        & directionToPlanetCenter
        );

    *altitudeOfViewpoint =
        fabs( vec3d_v_len( & directionToPlanetCenter) ) - PLANET_RADIUS;

#warning this needs to be actually computed!
    *solarElevationAtViewpoint = groundLevelSolarElevationAtOrigin;
    
    *altitudeOfViewpoint = M_MAX( *altitudeOfViewpoint, 0.0 );
    
    if(directionToPlanet)
        vec3d_v_norm_v( & directionToPlanetCenter, directionToPlanet );
}

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
        )
{
    Vec3D directionToPlanetCenter;
    arpragueskymodel_compute_altitude_and_elevation(
          viewpoint,
          groundLevelSolarElevationAtOrigin,
          groundLevelSolarAzimuthAtOrigin,
          solarElevationAtViewpoint,
          altitudeOfViewpoint,
        & directionToPlanetCenter
        );
    
    Vec3D  sunDirection;

    XC(sunDirection) =   cos( groundLevelSolarAzimuthAtOrigin )
                       * cos( *solarElevationAtViewpoint );
    YC(sunDirection) =   sin( groundLevelSolarAzimuthAtOrigin )
                       * cos( *solarElevationAtViewpoint );
    ZC(sunDirection) =   sin( *solarElevationAtViewpoint );

    double  dotProductSun =
        vec3d_vv_dot(
              viewDirection,
            & sunDirection
            );

    Vec3D  zeroDirection;

    XC(zeroDirection) =   cos( groundLevelSolarAzimuthAtOrigin );
    YC(zeroDirection) =   sin( groundLevelSolarAzimuthAtOrigin );
    ZC(zeroDirection) =   0.0;

    double  dotProductZero  =
        vec3d_vv_dot(
              viewDirection,
            & zeroDirection
            );
    
    Vec3D  zeroToView;
    
    vec3d_vv_sub_v(
          viewDirection,
        & zeroDirection,
        & zeroToView
        );
    
    double  len = sqrt(M_SQR(XC(zeroToView)) + M_SQR(YC(zeroToView)));

    Vec3D  viewDirNorm;
    vec3d_v_norm_v(   viewDirection, & viewDirNorm );
    
    double  cosTheta =
        vec3d_vv_dot(
            & viewDirNorm,
            & directionToPlanetCenter
            );

    *theta  = acos(cosTheta);
    *gamma  = acos(dotProductSun);
    *shadow = atan2(-ZC(zeroToView), len);
    *zero   = acos(dotProductZero);
    
#ifdef NEVERMORE
debugprintf("\n" )
debugprintf("Point   : " PNT3D_FORMAT("%f") "\n",PNT3D_P_PRINTF(*viewpoint) )
debugprintf("DirTC   : " VEC3D_FORMAT("%f") "\n",VEC3D_V_PRINTF(directionToPlanetCenter) )
debugprintf("Altitude: %f\n",*altitudeOfViewpoint )
debugprintf("Theta   : %f\n",*theta * MATH_RAD_TO_DEG)
debugprintf("Gamma   : %f\n",*gamma * MATH_RAD_TO_DEG)
#endif
}

double lerp(double from, double to, double factor)
{
  return (1.0 - factor) * from + factor * to;
}


void lerp_control_params(double* from, double* to, double* result, double factor, int total_coefs)
{
  for (int i = 0; i < total_coefs; ++i)
  {
    result[i] = lerp(from[i], to[i], factor);
  }
}

double* control_params_single_elevation(
  const ArPragueSkyModelState  * state,
  int                     elevation,
  int                     altitude,
  int                     turbidity,
  int                     albedo,
  int                     wavelength
)
{
  // turbidity is completely ignored for now
  return state->radiance_dataset[wavelength] + state->total_coefs_single_config * (elevation + elevations*altitude + elevations*altitudes*albedo);
}


void control_params_single_altitude(
  const ArPragueSkyModelState  * state,
  double                  elevation,
  int                     altitude,
  int                     turbidity,
  int                     albedo,
  int                     wavelength,
  double*                 result
)
{
  const int elevation_low = (int)elevation;
  const double factor = elevation - (double)elevation_low;

  double* control_params_low = control_params_single_elevation(
    state,
    elevation_low,
    altitude,
    turbidity,
    albedo,
    wavelength);

  double* control_params_high = control_params_single_elevation(
    state,
    elevation_low+1,
    altitude,
    turbidity,
    albedo,
    wavelength);

  lerp_control_params(control_params_low, control_params_high, result, factor, state->total_coefs_single_config);
}


void control_params_single_turbidity(
  const ArPragueSkyModelState  * state,
  double                  elevation,
  double                  altitude,
  int                     turbidity,
  int                     albedo,
  int                     wavelength,
  double*                 result
)
{
  const int altitude_low = (int)altitude;
  const double factor = altitude - (double)altitude_low;

  double control_params_low[state->total_coefs_single_config];
  control_params_single_altitude(
    state,
    elevation,
    altitude_low,
    turbidity,
    albedo,
    wavelength,
    control_params_low
    );

  double control_params_high[state->total_coefs_single_config];
  control_params_single_altitude(
    state,
    elevation,
    altitude_low+1,
    turbidity,
    albedo,
    wavelength,
    control_params_high);

  lerp_control_params(control_params_low, control_params_high, result, factor, state->total_coefs_single_config);
}


void control_params_single_albedo(
  const ArPragueSkyModelState  * state,
  double                  elevation,
  double                  altitude,
  double                  turbidity,
  int                     albedo,
  int                     wavelength,
  double*                 result
)
{
  int turbidity_low = (int)turbidity;
  const double factor = turbidity - (double)turbidity_low;
  --turbidity_low;

  double control_params_low[state->total_coefs_single_config];
  control_params_single_turbidity(
    state,
    elevation,
    altitude,
    turbidity_low,
    albedo,
    wavelength,
    control_params_low);

  double control_params_high[state->total_coefs_single_config];
  control_params_single_turbidity(
    state,
    elevation,
    altitude,
    turbidity_low+1,
    albedo,
    wavelength,
    control_params_high);

  lerp_control_params(control_params_low, control_params_high, result, factor, state->total_coefs_single_config);
}


void control_params_single_wavelength(
  const ArPragueSkyModelState  * state,
  double                  elevation,
  double                  altitude,
  double                  turbidity,
  double                  albedo,
  int                     wavelength,
  double*                 result
)
{
  const int albedo_low = (int)albedo;
  const double factor = albedo - (double)albedo_low;

  double control_params_low[state->total_coefs_single_config];
  control_params_single_albedo(
    state,
    elevation,
    altitude,
    turbidity,
    albedo_low,
    wavelength,
    control_params_low);

  double control_params_high[state->total_coefs_single_config];
  control_params_single_albedo(
    state,
    elevation,
    altitude,
    turbidity,
    albedo_low+1,
    wavelength,
    control_params_high);

  lerp_control_params(control_params_low, control_params_high, result, factor, state->total_coefs_single_config);
}


void control_params(
  const ArPragueSkyModelState  * state,
  double                  elevation,
  double                  altitude,
  double                  turbidity,
  double                  albedo,
  double                  wavelength,
  double*                 result
)
{
  const int wavelength_low = (int)wavelength;
  const double factor = wavelength - (double)wavelength;

  double control_params_low[state->total_coefs_single_config];
  control_params_single_wavelength(
    state,
    elevation,
    altitude,
    turbidity,
    albedo,
    wavelength_low,
    control_params_low);

  double control_params_high[state->total_coefs_single_config];
  control_params_single_wavelength(
    state,
    elevation,
    altitude,
    turbidity,
    albedo,
    wavelength_low+1,
    control_params_high);

  lerp_control_params(control_params_low, control_params_high, result, factor, state->total_coefs_single_config);
}

double eval_pp(double x, int nbreaks, const double* breaks, const double* coefs)
{
  // determine which segment
  int segment = 0;
  for (segment = 0; segment < nbreaks; ++segment)
  {
    if (breaks[segment+1] >= x)
      break;
  }

  x -= breaks[segment];
  const double* sc = coefs + 4 * segment; // segment coefs
  return sc[0] * x*x*x + sc[1] * x*x + sc[2] * x + sc[3];
}

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
        )
{
  //return M_MAX(shadow,0.);
  //return arpragueskymodel_polarisation(state, theta, gamma, elevation, altitude, turbidity, albedo, wavelength);
    
  const double altitude_control = 20.0 * cbrt(altitude / 15000.0);
  const double elevation_control = 20.0 * cbrt(2.0 * elevation / M_PI) + 10.0;

  const double channel = (wavelength-320.0) / 40.0;

    if ( channel >= 10. || channel < 0.)
        return 0.;
    
  double control_params_interpolated[state->total_coefs_single_config];
  control_params(
    state,
    elevation_control,
    altitude_control,
    turbidity,
    albedo,
    channel,
    control_params_interpolated);
  
  // coefs->value comes here

  const double background_val = eval_pp(theta, state->background_nbreaks, state->background_breaks, control_params_interpolated + state->background_offset);
  const double solar_max_val = eval_pp(gamma, state->solar_max_nbreaks, state->solar_max_breaks, control_params_interpolated + state->solar_max_offset);
  const double solar_ratio_val = eval_pp(theta, state->solar_ratio_nbreaks, state->solar_ratio_breaks, control_params_interpolated + state->solar_ratio_offset);
  const double backglow_vertical_val = eval_pp(zero, state->backglow_vertical_nbreaks, state->backglow_vertical_breaks, control_params_interpolated + state->backglow_vertical_offset);
  const double backglow_ratio_val = eval_pp(theta, state->backglow_ratio_nbreaks, state->backglow_ratio_breaks, control_params_interpolated + state->backglow_ratio_offset);
  const double frontglow_vertical_val = eval_pp(zero, state->frontglow_vertical_nbreaks, state->frontglow_vertical_breaks, control_params_interpolated + state->frontglow_vertical_offset);
  const double frontglow_ratio_val = eval_pp(theta, state->frontglow_ratio_nbreaks, state->frontglow_ratio_breaks, control_params_interpolated + state->frontglow_ratio_offset);
  const double shadow_val = eval_pp(shadow, state->shadow_nbreaks, state->shadow_breaks, control_params_interpolated + state->shadow_offset);
  const double shadow_v_val = eval_pp(zero, state->shadow_v_nbreaks, state->shadow_v_breaks, control_params_interpolated + state->shadow_v_offset);

  const double res = 
    ( background_val
    + solar_max_val * solar_ratio_val
    + backglow_vertical_val * backglow_ratio_val
    + frontglow_vertical_val * frontglow_ratio_val
    ) * ( 1 - shadow_val * shadow_v_val );

  return M_MAX(res,0.);
}

const double psm_originalSolarRadianceTable[] =
{
     7500.0,
    12500.0,
    21127.5,
    26760.5,
    30663.7,
    27825.0,
    25503.8,
    25134.2,
    23212.1,
    21526.7,
    19870.8
};

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
        )
{
    int low_wl = (wavelength - 320.0 ) / 40.0;

    if ( low_wl < 0 || low_wl >= 11 )
        return 0.0f;

    double interp = fmod((wavelength - 320.0 ) / 40.0, 1.0);

    double val_low =
        psm_originalSolarRadianceTable[low_wl];

    if ( interp < 1e-6 )
        return val_low;

    double result = ( 1.0 - interp ) * val_low;

    if ( low_wl+1 < 11 )
    {
        result +=
              interp
            * psm_originalSolarRadianceTable[low_wl+1];
    }

    double  tau =
        arpragueskymodel_tau(
            state,
            theta,
            altitude,
            turbidity,
            wavelength,
            MATH_HUGE_DOUBLE
            );
    
    return result * tau;
}

void arpragueskymodel_toAD(
                           double theta,
                           double distance,
                           double altitude,
                           double *a,
                           double *d
                           )
{
    // Ray circle intersection
    double x_v = sin(theta);
    double y_v = cos(theta);
    double x_c = 0;
    double y_c = planet_radius + altitude;
    double atmo_edge = planet_radius + 100000;
    double qa = (x_v * x_v) + (y_v * y_v);
    double qb = 2.0 * y_c * y_v;
    double qc = (y_c * y_c) - (atmo_edge * atmo_edge);
    double n = (qb * qb) - (4.0 * qa * qc);
    if (n <= 0)
    {
        // Then we have a problem!
        // Return something, but this should never happen so long as the camera is inside the atmosphere
        // Which it should be in this work
        *a = 0;
        *d = 0;
        return;
    }
    n = (-qb + sqrt(n)) / (2.0 * qa);
    double distance_corrected = n;
    // Use the smaller of the distances
    distance_corrected = distance < distance_corrected ? distance : distance_corrected;
    // Points in world space
    double x_p = x_v * distance_corrected;
    double y_p = (y_v * distance_corrected) + planet_radius + altitude;
    n = sqrt((x_p * x_p) + (y_p * y_p));
    *a = n - planet_radius;
    *a = *a > 0 ? *a : 0;
    *a = pow(*a / 100000.0, 1.0 / 3.0);
    *d = acos(y_p / n) * planet_radius;
    *d = *d / 1571524.413613;
    *d = pow(*d, 0.25);
    *d = *d > 1.0 ? 1.0 : *d;
}

float *arpragueskymodel_transmittanceCoefsIndex(const ArPragueSkyModelState  * state,
                                                int turbidity,
                                                int altitude,
                                                int wavelength
                                                )
{
    return &state->transmission_dataset[(transsvdrank * 86 * 101) + ((turbidity * 21 * 11) + (altitude * 11) + wavelength) * transsvdrank];
}

void arpragueskymodel_transmittanceInterpolateWaveLength(
                                            const ArPragueSkyModelState  * state,
                                            int turbidity,
                                            int altitude,
                                            int wavelength_low,
                                            int wavelength_inc,
                                            double wavelength_w,
                                            float *coefficients
                                            )
{
    float *wlu = arpragueskymodel_transmittanceCoefsIndex(state, turbidity, altitude, wavelength_low);
    float *wll = arpragueskymodel_transmittanceCoefsIndex(state, turbidity, altitude, wavelength_low + wavelength_inc);
    double iw = 1.0 - wavelength_w;
    for (int i = 0; i < 32; i++)
    {
        coefficients[i] = (wll[i] * iw) + (wlu[i] * wavelength_w);
    }
}

void arpragueskymodel_transmittanceInterpolateAltitude(
                                          const ArPragueSkyModelState  * state,
                                          int turbidity,
                                          int altitude_low,
                                          int altitude_inc,
                                          double altitude_weight,
                                          int wavelength_low,
                                          int wavelength_inc,
                                          double wavelength_w,
                                          float *coefficients
                                          )
{
    float au[32];
    float al[32];
    arpragueskymodel_transmittanceInterpolateWaveLength(state, turbidity, altitude_low, wavelength_low, wavelength_inc, wavelength_w, al);
    arpragueskymodel_transmittanceInterpolateWaveLength(state, turbidity, altitude_low + altitude_inc, wavelength_low, wavelength_inc, wavelength_w, au);
    double iw = 1.0 - altitude_weight;
    for (int i = 0; i < 32; i++)
    {
        coefficients[i] = (al[i] * iw) + (au[i] * altitude_weight);
    }
}

double arpragueskymodel_calc_transmittance_svd(const ArPragueSkyModelState  * state, double a, double d, float *interpolated_coefficients)
{
    float t[4] = {0.0, 0.0, 0.0, 0.0};
    int aa = (int)floor(a * 86.0);
    int da = (int)floor(d * 101.0);
    int aainc = 0;
    int dainc = 0;
    if (aa < 85)
    {
        aainc = 1;
    } else
    {
        aa = 85;
    }
    if (da < 100)
    {
        dainc = 1;
    } else
    {
        da = 100;
    }
    // TODO: Interpolate in actual space rather than pow space. This will make interpolation more accurate
    double wa = (a * 86.0) - (double)aa;
    double wd = (d * 101.0) - (double)da;
    int index = 0;
    for (int al = aa ; al <= aa + aainc; al++)
    {
        for (int dl = da ; dl <= da + dainc; dl++)
        {
            for (int i = 0; i < transsvdrank; i++)
            {
                t[index] = t[index] + (state->transmission_dataset[(i * 86 * 101) + (al * 101) + dl] * interpolated_coefficients[i]);
            }
            index++;
        }
    }
    if (dainc == 1)
    {
        t[0] = (t[0] * (1.0 - wd)) + (t[1] * wd);
        t[1] = (t[2] * (1.0 - wd)) + (t[3] * wd);
    }
    t[0] = t[0] < 0 ? 0 : t[0];
    t[0] = t[0] > 1.0 ? 1.0 : t[0];
    if (aainc == 1)
    {
        t[1] = t[1] < 0 ? 0 : t[1];
        t[1] = t[1] > 1.0 ? 1.0 : t[1];
        t[0] = (t[0] * (1.0 - wa)) + (t[1] * wa);
    }
    t[0] = t[0] * t[0];
    return t[0];
}

double arpragueskymodel_calcTransmittanceScale(
                                               const ArPragueSkyModelState  * state,
                                               int turbidity,
                                               int altitude,
                                               int wavelength,
                                               int a_l,
                                               int a_u,
                                               double a_w
                                               )
{
    float *svdscale = &state->transmission_dataset[(transsvdrank * 86 * 101) + (transsvdrank * 11 * 21 * 11) + (((turbidity * 21 * 11) + (altitude * 11) + (wavelength)) * 86)];
    return ((svdscale[a_l] * (1.0 - a_w)) + (svdscale[a_u] * a_w));
}

double arpragueskymodel_tau(
                            const ArPragueSkyModelState  * state,
                            const double                   theta,
                            const double                   altitude,
                            const double                   turbidity,
                            const double                   wavelength,
                            const double                   distance
                            )
{
    const int turbidity_low = (int)turbidity - 1;
    const double turbidity_factor = turbidity - (double)(turbidity_low + 1.0);
    const int turbidity_inc = turbidity_low < 10 ? 1 : 0;
    
    const double wavelength_norm = (wavelength - 340.0) / 40.0;
    if (wavelength_norm >= 10. || wavelength_norm < 0.)
        return 0.;
    const int wavelength_low = (int)wavelength_norm;
    const double wavelength_factor = wavelength_norm - (double)wavelength_low;
    const int wavelength_inc = wavelength_low < 10 ? 1 : 0;
    
    const double altitude_norm = 20.0 * cbrt(altitude / 15000.0);
    const int altitude_low = (int)altitude_norm;
    const double altitude_factor = altitude_norm - (double)altitude_low;
    const int altitude_inc = altitude_low < 20 ? 1 : 0;
    
    // Calculate normalized and non-linearly scaled position in the atmosphere
    double a;
    double d;
    arpragueskymodel_toAD(theta, distance, altitude, &a, &d);
    
    // Interpolate basis coefficients
    float interpolatedCoefs[32];
    float tu[32];
    float tl[32];
    arpragueskymodel_transmittanceInterpolateAltitude(state, turbidity_low, altitude_low, altitude_inc, altitude_factor, wavelength_low, wavelength_inc, wavelength_factor, tl);
    arpragueskymodel_transmittanceInterpolateAltitude(state, turbidity_low + turbidity_inc, altitude_low, altitude_inc, altitude_factor, wavelength_low, wavelength_inc, wavelength_factor, tu);
    
    double iw = 1.0 - turbidity_factor;
    for (int i = 0; i < 32; i++)
    {
        interpolatedCoefs[i] = (tl[i] * iw) + (tu[i] * turbidity_factor);
    }
    
    // Evaluate basis
    double trans = arpragueskymodel_calc_transmittance_svd(state, a, d, interpolatedCoefs);
    
    // Scale by normalization factor
    double N;
    int aa = (int)floor(a * 86.0);
    int aainc = 0;
    if (aa < 85)
    {
        aainc = 1;
    } else
    {
        aa = 85;
    }
    // TODO: Interpolate in actual space rather than pow space. This will make interpolation more accurate
    // Also, make less ugly!
    double wa = (a * 86.0) - (double)aa;
    double nt[2];
    double na[2];
    double nwl[2];
    nwl[0] = arpragueskymodel_calcTransmittanceScale(state, turbidity_low, altitude_low, wavelength_low, aa, aa + aainc, wa);
    nwl[1] = arpragueskymodel_calcTransmittanceScale(state, turbidity_low, altitude_low, wavelength_low + wavelength_inc, aa, aa + aainc, wa);
    na[0] = (nwl[0] * (1.0 - wavelength_factor)) + (nwl[1] * wavelength_factor);
    nwl[0] = arpragueskymodel_calcTransmittanceScale(state, turbidity_low, altitude_low + altitude_inc, wavelength_low, aa, aa + aainc, wa);
    nwl[1] = arpragueskymodel_calcTransmittanceScale(state, turbidity_low, altitude_low + altitude_inc, wavelength_low + wavelength_inc, aa, aa + aainc, wa);
    na[1] = (nwl[0] * (1.0 - wavelength_factor)) + (nwl[1] * wavelength_factor);
    nt[0] = (na[0] * (1.0 - altitude_factor)) + (na[1] * altitude_factor);
    
    nwl[0] = arpragueskymodel_calcTransmittanceScale(state, turbidity_low + turbidity_inc, altitude_low, wavelength_low, aa, aa + aainc, wa);
    nwl[1] = arpragueskymodel_calcTransmittanceScale(state, turbidity_low + turbidity_inc, altitude_low, wavelength_low + wavelength_inc, aa, aa + aainc, wa);
    na[0] = (nwl[0] * (1.0 - wavelength_factor)) + (nwl[1] * wavelength_factor);
    nwl[0] = arpragueskymodel_calcTransmittanceScale(state, turbidity_low + turbidity_inc, altitude_low + altitude_inc, wavelength_low, aa, aa + aainc, wa);
    nwl[1] = arpragueskymodel_calcTransmittanceScale(state, turbidity_low + turbidity_inc, altitude_low + altitude_inc, wavelength_low + wavelength_inc, aa, aa + aainc, wa);
    na[1] = (nwl[0] * (1.0 - wavelength_factor)) + (nwl[1] * wavelength_factor);
    nt[1] = (na[0] * (1.0 - altitude_factor)) + (na[1] * altitude_factor);
    
    N = (nt[0] * (1.0 - turbidity_factor)) + (nt[1] * turbidity_factor);
    
    N = N > 1.0 ? 1.0 : N;
    
    double ntrans = N * trans;
    
    return ntrans;
}

double* polarisation_control_params_single_elevation(
  const ArPragueSkyModelState  * state,
  int                     elevation,
  int                     altitude,
  int                     turbidity,
  int                     albedo,
  int                     wavelength
)
{
  // turbidity is completely ignored for now
  return state->polarisation_dataset[wavelength] + total_polarisation_coefs_single_config * (elevation + elevations*altitude + elevations*altitudes*albedo);
}


void polarisation_control_params_single_altitude(
  const ArPragueSkyModelState  * state,
  double                  elevation,
  int                     altitude,
  int                     turbidity,
  int                     albedo,
  int                     wavelength,
  double*                 result
)
{
  const int elevation_low = (int)elevation;
  const double factor = elevation - (double)elevation_low;

  double* control_params_low = polarisation_control_params_single_elevation(
    state,
    elevation_low,
    altitude,
    turbidity,
    albedo,
    wavelength);

  double* control_params_high = polarisation_control_params_single_elevation(
    state,
    elevation_low+1,
    altitude,
    turbidity,
    albedo,
    wavelength);

  lerp_control_params(control_params_low, control_params_high, result, factor, total_polarisation_coefs_single_config);
}


void polarisation_control_params_single_turbidity(
  const ArPragueSkyModelState  * state,
  double                  elevation,
  double                  altitude,
  int                     turbidity,
  int                     albedo,
  int                     wavelength,
  double*                 result
)
{
  const int altitude_low = (int)altitude;
  const double factor = altitude - (double)altitude_low;

  double control_params_low[total_polarisation_coefs_single_config];
  polarisation_control_params_single_altitude(
    state,
    elevation,
    altitude_low,
    turbidity,
    albedo,
    wavelength,
    control_params_low
  );

  double control_params_high[total_polarisation_coefs_single_config];
  polarisation_control_params_single_altitude(
    state,
    elevation,
    altitude_low+1,
    turbidity,
    albedo,
    wavelength,
    control_params_high);

  lerp_control_params(control_params_low, control_params_high, result, factor, total_polarisation_coefs_single_config);
}


void polarisation_control_params_single_albedo(
  const ArPragueSkyModelState  * state,
  double                  elevation,
  double                  altitude,
  double                  turbidity,
  int                     albedo,
  int                     wavelength,
  double*                 result
)
{
  int turbidity_low = (int)turbidity;
  const double factor = turbidity - (double)turbidity_low;
  --turbidity_low;

  double control_params_low[total_polarisation_coefs_single_config];
  polarisation_control_params_single_turbidity(
    state,
    elevation,
    altitude,
    turbidity_low,
    albedo,
    wavelength,
    control_params_low);

  double control_params_high[total_polarisation_coefs_single_config];
  polarisation_control_params_single_turbidity(
    state,
    elevation,
    altitude,
    turbidity_low+1,
    albedo,
    wavelength,
    control_params_high);

  lerp_control_params(control_params_low, control_params_high, result, factor, total_polarisation_coefs_single_config);
}


void polarisation_control_params_single_wavelength(
  const ArPragueSkyModelState  * state,
  double                  elevation,
  double                  altitude,
  double                  turbidity,
  double                  albedo,
  int                     wavelength,
  double*                 result
)
{
  const int albedo_low = (int)albedo;
  const double factor = albedo - (double)albedo_low;

  double control_params_low[total_polarisation_coefs_single_config];
  polarisation_control_params_single_albedo(
    state,
    elevation,
    altitude,
    turbidity,
    albedo_low,
    wavelength,
    control_params_low);

  double control_params_high[total_polarisation_coefs_single_config];
  polarisation_control_params_single_albedo(
    state,
    elevation,
    altitude,
    turbidity,
    albedo_low+1,
    wavelength,
    control_params_high);

  lerp_control_params(control_params_low, control_params_high, result, factor, total_polarisation_coefs_single_config);
}


void polarisation_control_params(
        const ArPragueSkyModelState  * state,
        const double                   elevation,
        const double                   altitude,
        const double                   turbidity,
        const double                   albedo,
        const double                   wavelength,
              double                 * result
        )
{
  const int wavelength_low = (int)wavelength;
  const double factor = wavelength - (double)wavelength;

  double control_params_low[total_polarisation_coefs_single_config];
  polarisation_control_params_single_wavelength(
    state,
    elevation,
    altitude,
    turbidity,
    albedo,
    wavelength_low,
    control_params_low);

  double control_params_high[total_polarisation_coefs_single_config];
  polarisation_control_params_single_wavelength(
    state,
    elevation,
    altitude,
    turbidity,
    albedo,
    wavelength_low+1,
    control_params_high);

  lerp_control_params(control_params_low, control_params_high, result, factor, total_polarisation_coefs_single_config);
}


double arpragueskymodel_polarisation(
        const ArPragueSkyModelState  * state,
        const double                   theta,
        const double                   gamma,
        const double                   elevation,
        const double                   altitude,
        const double                   turbidity,
        const double                   albedo,
        const double                   wavelength
        )
{
  const double altitude_control = 20.0 * cbrt(altitude / 15000.0);
  const double elevation_control = 20.0 * cbrt(2.0 * elevation / M_PI) + 10.0;

  const double channel = (wavelength-320.0) / 40.0;
  
  if ( channel >= 10. || channel < 0.)
    return 0.;

  double control_params_interpolated[total_polarisation_coefs_single_config];
  polarisation_control_params(
    state,
    elevation_control,
    altitude_control,
    turbidity,
    albedo,
    channel,
    control_params_interpolated);

  // coefs->value comes here

  const double polarisation_max_val = eval_pp(gamma, polarisation_max_nbreaks, polarisation_max_breaks, control_params_interpolated + polarisation_max_offset);
  const double polarisation_ratio_val = eval_pp(theta, polarisation_ratio_nbreaks, polarisation_ratio_breaks, control_params_interpolated + polarisation_ratio_offset);

  const double res = polarisation_max_val * polarisation_ratio_val;

  return res;
}
