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
#define TERRESTRIAL_SOLAR_RADIUS    ( ( 0.5334 DEGREES ) / 2.0 )
#endif

#ifndef ALLOC
#define ALLOC(_struct)              ((_struct *)malloc(sizeof(_struct)))
#endif

#ifdef ARPRAGUESKYMODEL_USE_NEW

const int elevations = 24;
const int altitudes = 21;
const int albedos = 4;
const double elevation_vals[] = { -0.049742, -0.017453, -0.000175, 0.000000, 0.017453, 0.049742, 0.091281, 0.140499, 0.196350, 0.258134, 0.325329, 0.397411, 0.474206, 0.555364, 0.640710, 0.730071, 0.823097, 0.919963, 1.020319, 1.123992, 1.230981, 1.341111, 1.454557, 1.570796 };
const double altitude_vals[] = { 0, 1.875, 15, 50.625, 120, 234.38, 405, 643.12, 960, 1366.9, 1875, 2495.6, 3240, 4119.4, 5145, 6328.1, 7680, 9211.9, 10935, 12861, 15000 };
const int tensor_components = 10;
const int tensor_components_pol = 4;

#define transsvdrank 32
const int transmittance_coefs = 5;
const int transmittance_altitudes = 86;
int total_transmittance_values = (86 * 101 * transsvdrank) + (transsvdrank * 231);
const double planet_radius = 6378000.0;

ArPragueSkyModelState  * arpragueskymodelstate_alloc_init(
        const char  * library_path
        )
{
    ArPragueSkyModelState  * state = ALLOC(ArPragueSkyModelState);

        // Radiance

	// Read breaks
	// Breaks file structure:
	// sun_nbreaks (1 * int), zenith_nbreaks (1 * int), emph_nbreaks (1 * int), sun_breaks (sun_nbreaks * double), zenith_breaks (zenith_nbreaks * double), emph_breaks (emph_nbreaks * double)

	char breaks_filename[1024];
	sprintf(breaks_filename, "%s/SkyModel/new_breaks_c.dat", library_path);
	FILE* breaks_handle = fopen(breaks_filename, "rb");
	fread(&state->sun_nbreaks, sizeof(int), 1, breaks_handle);
	fread(&state->zenith_nbreaks, sizeof(int), 1, breaks_handle);
	fread(&state->emph_nbreaks, sizeof(int), 1, breaks_handle);

	state->sun_breaks = ALLOC_ARRAY(double, state->sun_nbreaks);
	state->zenith_breaks = ALLOC_ARRAY(double, state->zenith_nbreaks);
	state->emph_breaks = ALLOC_ARRAY(double, state->emph_nbreaks);

	fread(state->sun_breaks, sizeof(double), state->sun_nbreaks, breaks_handle);
	fread(state->zenith_breaks, sizeof(double), state->zenith_nbreaks, breaks_handle);
	fread(state->emph_breaks, sizeof(double), state->emph_nbreaks, breaks_handle);

    	fclose(breaks_handle);

	// Calculate offsets and strides

	state->sun_offset = 0;
	state->sun_stride = 2 * state->sun_nbreaks - 2 + 2 * state->zenith_nbreaks - 2;

	state->zenith_offset = state->sun_offset + 2 * state->sun_nbreaks - 2;
	state->zenith_stride = state->sun_stride;

	state->emph_offset = state->sun_offset + tensor_components * state->sun_stride;

	state->total_coefs_single_config = state->emph_offset + 2 * state->emph_nbreaks - 2; // this is for one specific configuration
	state->total_coefs_all_configs = state->total_coefs_single_config * elevations * altitudes * albedos;

	// Read coefficients

    for (int wl = 0; wl < 11; ++wl)
    {
        // Radiance file structure:
	// [[[[ sun_coefs ((2 * state->sun_nbreaks - 2) * double), zenith_coefs ((2 * state->zenith_nbreaks - 2) * double) ] * tensor_components, emph_coefs ((2 * state->emph_nbreaks - 2) * double) ] * elevations ] * altitudes ] * albedos

	char filename[1024];

        sprintf(filename, "%s/SkyModel/new_params_t%d_wl%d.dat", library_path, 5, wl+1);

        FILE* handle = fopen(filename, "rb");

        state->radiance_dataset[wl] = ALLOC_ARRAY(double, state->total_coefs_all_configs);

        size_t s = fread(state->radiance_dataset[wl], sizeof(double), state->total_coefs_all_configs, handle);
        fclose(handle);
    }

    // Polarisation

	// Read breaks
	// Breaks file structure:
	// sun_nbreaks_pol (1 * int), zenith_nbreaks_pol (1 * int), sun_breaks_pol (sun_nbreaks_pol * double), zenith_breaks_pol (zenith_nbreaks_pol * double)

	sprintf(breaks_filename, "%s/SkyModel/new_breaks_c_pol.dat", library_path);
	breaks_handle = fopen(breaks_filename, "rb");
	fread(&state->sun_nbreaks_pol, sizeof(int), 1, breaks_handle);
	fread(&state->zenith_nbreaks_pol, sizeof(int), 1, breaks_handle);

	state->sun_breaks_pol = ALLOC_ARRAY(double, state->sun_nbreaks_pol);
	state->zenith_breaks_pol = ALLOC_ARRAY(double, state->zenith_nbreaks_pol);

	fread(state->sun_breaks_pol, sizeof(double), state->sun_nbreaks_pol, breaks_handle);
	fread(state->zenith_breaks_pol, sizeof(double), state->zenith_nbreaks_pol, breaks_handle);

    	fclose(breaks_handle);

	// Calculate offsets and strides

	state->sun_offset_pol = 0;
	state->sun_stride_pol = 2 * state->sun_nbreaks_pol - 2 + 2 * state->zenith_nbreaks_pol - 2;

	state->zenith_offset_pol = state->sun_offset_pol + 2 * state->sun_nbreaks_pol - 2;
	state->zenith_stride_pol = state->sun_stride_pol;

	state->total_coefs_single_config_pol = state->sun_offset_pol + tensor_components_pol * state->sun_stride_pol; // this is for one specific configuration
	state->total_coefs_all_configs_pol = state->total_coefs_single_config_pol * elevations * altitudes * albedos;

	// Read coefficients

    for (int wl = 0; wl < 11; ++wl)
    {
        // Polarisation file structure:
	// [[[[ sun_coefs ((2 * state->sun_nbreaks_pol - 2) * double), zenith_coefs ((2 * state->zenith_nbreaks_pol - 2) * double) ] * tensor_components_pol ] * elevations ] * altitudes ] * albedos

	char filename[1024];

        sprintf(filename, "%s/SkyModel/new_params_t%d_wl%d_pol.dat", library_path, 5, wl+1);

        FILE* handle = fopen(filename, "rb");

        state->polarisation_dataset[wl] = ALLOC_ARRAY(double, state->total_coefs_all_configs_pol);

        size_t s = fread(state->polarisation_dataset[wl], sizeof(double), state->total_coefs_all_configs_pol, handle);
        fclose(handle);
    }


    // Transmittance

    char filenametransmittance[1024];
    
    sprintf(filenametransmittance, "%s/SkyModel/SVDFit32.dat", library_path);
    
    FILE* handletrans = fopen(filenametransmittance, "rb");
    
    state->transmission_dataset = ALLOC_ARRAY(float, total_transmittance_values);
    
    fread(state->transmission_dataset, sizeof(float), total_transmittance_values, handletrans);
    
    fclose(handletrans);

    return state;
}

void arpragueskymodelstate_free(
        ArPragueSkyModelState  * state
        )
{
    free(state->sun_breaks);
    free(state->zenith_breaks);
    free(state->emph_breaks);
    free(state->sun_breaks_pol);
    free(state->zenith_breaks_pol);

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
    //Pnt3D  centerOfTheEarth = PNT3D(0,0,0);

    Vec3D  directionToPlanetCenter;

    vec3d_pp_sub_v(
        & centerOfTheEarth,
          viewpoint,
        & directionToPlanetCenter
        );

    *altitudeOfViewpoint =
        fabs( vec3d_v_len( & directionToPlanetCenter) ) - PLANET_RADIUS;

    // Altitude correction?
    /*double C_a = fabs(vec3d_v_len( & directionToPlanetCenter)) - PSM_PLANET_RADIUS_SQR / fabs(vec3d_v_len( & directionToPlanetCenter));
    double a = sqrt( vec3d_v_len( & directionToPlanetCenter) * vec3d_v_len( & directionToPlanetCenter) - PSM_PLANET_RADIUS_SQR );
    double cosCorrectionAngle = a > 0.0 ? C_a / a : 0.0;
    // assuming the sun is infinitely far away
    *solarElevationAtViewpoint = groundLevelSolarElevationAtOrigin + acos(1. - cosCorrectionAngle);*/
    *solarElevationAtViewpoint = groundLevelSolarElevationAtOrigin; // no correction

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
    // Altitude correction

    Pnt3D  centerOfTheEarth = PNT3D(0,0,-PLANET_RADIUS);
    //Pnt3D  centerOfTheEarth = PNT3D(0,0,0);
    Vec3D  directionToPlanetCenter2;
    vec3d_pp_sub_v(
        & centerOfTheEarth,
          viewpoint,
        & directionToPlanetCenter2
        );
    const double C_a = fabs(vec3d_v_len( & directionToPlanetCenter2)) - PSM_PLANET_RADIUS_SQR / fabs(vec3d_v_len( & directionToPlanetCenter2));
    const double a = sqrt( vec3d_v_len( & directionToPlanetCenter2) * vec3d_v_len( & directionToPlanetCenter2) - PSM_PLANET_RADIUS_SQR );
    const double cosCorrectionAngle = a > 0.0 ? C_a / a : 0.0;    
    const Vec3D corr = VEC3D(0,0,cosCorrectionAngle);
    Vec3D correctView;
    vec3d_vv_add_v(
        & corr,
          viewDirection,
        & correctView
        );
    Vec3D correctViewN;
    vec3d_v_norm_v( & correctView, & correctViewN );    

    Vec3D directionToPlanetCenter;
    arpragueskymodel_compute_altitude_and_elevation(
          viewpoint,
          groundLevelSolarElevationAtOrigin,
          groundLevelSolarAzimuthAtOrigin,
          solarElevationAtViewpoint,
          altitudeOfViewpoint,
        & directionToPlanetCenter
        );

    // Sun angle (gamma) - no correction

    Vec3D  sunDirection;
    XC(sunDirection) =   cos( groundLevelSolarAzimuthAtOrigin )
                       * cos( groundLevelSolarElevationAtOrigin );
                       //* cos( *solarElevationAtViewpoint );
    YC(sunDirection) =   sin( groundLevelSolarAzimuthAtOrigin )
                       * cos( groundLevelSolarElevationAtOrigin );
                       //* cos( *solarElevationAtViewpoint );
    ZC(sunDirection) =   sin( groundLevelSolarElevationAtOrigin );
                         //sin( *solarElevationAtViewpoint );

    double  dotProductSun =
        vec3d_vv_dot(
              viewDirection,
            & sunDirection
            );

    *gamma = acos(dotProductSun);


    // Shadow angle - requires correction

    /*Vec3D  zeroDirection;
    XC(zeroDirection) =   cos( groundLevelSolarAzimuthAtOrigin );
    YC(zeroDirection) =   sin( groundLevelSolarAzimuthAtOrigin );
    ZC(zeroDirection) =   0.0;

    double  dotProductZero  =
        vec3d_vv_dot(
            //& correctViewN,
              viewDirection,
            & zeroDirection
            );

    Vec3D  zeroToView;
    vec3d_vv_sub_v(
        //& correctViewN,
          viewDirection,
        & zeroDirection,
        & zeroToView
        );

    double  len = sqrt(M_SQR(XC(zeroToView)) + M_SQR(YC(zeroToView)));

    *shadow = atan2(-ZC(zeroToView), len);
    *zero   = acos(dotProductZero);*/

    Vec3D  shadowDirection;
    const double shadow_angle = groundLevelSolarElevationAtOrigin + MATH_PI * 0.5;
    const double rotation = groundLevelSolarAzimuthAtOrigin - MATH_PI * 0.5;
    XC(shadowDirection) = 0.0;
    YC(shadowDirection) = cos(shadow_angle);
    ZC(shadowDirection) = sin(shadow_angle);
    const double shadow_x = XC(shadowDirection);
    const double shadow_y = YC(shadowDirection);
    XC(shadowDirection) = shadow_x * cos(rotation) - shadow_y * sin(rotation);
    YC(shadowDirection) = shadow_x * sin(rotation) + shadow_y * cos(rotation);
    const double  dotProductShadow  =
        vec3d_vv_dot(
            & correctViewN,
            & shadowDirection
            );
    *shadow = acos(dotProductShadow);

    // Zenith angle (theta) - corrected version stored in otherwise unused zero angle

    double  cosThetaCor =
        vec3d_vv_dot(
            & correctViewN,
            & directionToPlanetCenter
            );

    *zero  = acos(cosThetaCor); 

    // Zenith angle (theta) - uncorrected version goes outside
     
    Vec3D  viewDirNorm;
    vec3d_v_norm_v(   viewDirection, & viewDirNorm );

    double  cosTheta =
        vec3d_vv_dot(
            & viewDirNorm,
            & directionToPlanetCenter
            );

    *theta  = acos(cosTheta); 

#ifdef NEVERMORE
debugprintf("\n" )
debugprintf("Point   : " PNT3D_FORMAT("%f") "\n",PNT3D_P_PRINTF(*viewpoint) )
debugprintf("ViewDir : " VEC3D_FORMAT("%f") "\n",VEC3D_V_PRINTF(*viewDirection) )
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

/*double* control_params_single_elevation(
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

  if (factor < 1e-6 || elevation_low >= (elevations - 1))
  {
    copy_params(control_params_low, result, state->total_coefs_single_config);
    return;
  }

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

  if (factor < 1e-6 || altitude_low >= (altitudes - 1))
  {
    control_params_single_altitude(
      state,
      elevation,
      altitude_low,
      turbidity,
      albedo,
      wavelength,
      result
      );
    return;
  }

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
  //int turbidity_low = (int)turbidity;
  //const double factor = turbidity - (double)turbidity_low;

  //double control_params_low[state->total_coefs_single_config];
  control_params_single_turbidity(
    state,
    elevation,
    altitude,
    0,           // we don't care about turbidity now
    albedo,
    wavelength,
    result);

  /*double control_params_high[state->total_coefs_single_config];
  control_params_single_turbidity(
    state,
    elevation,
    altitude,
    turbidity_low+1,
    albedo,
    wavelength,
    control_params_high);

  lerp_control_params(control_params_low, control_params_high, result, factor, state->total_coefs_single_config);*//*
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

  if (factor < 1e-6 || albedo_low >= (albedos - 1)) {
    control_params_single_albedo(
      state,
      elevation,
      altitude,
      turbidity,
      albedo_low,
      wavelength,
      result);
    return;
  }

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
  const double factor = wavelength - (double)wavelength_low;

  if (factor < 1e-6 || wavelength_low >= 10) {
    control_params_single_wavelength(
      state,
      elevation,
      altitude,
      turbidity,
      albedo,
      wavelength_low,
      result); 
    return;
  }

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
}*/

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
  const double* sc = coefs + 2 * segment; // segment coefs
  return sc[0] * x + sc[1];
}

double* control_params_single_config(
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

double reconstruct(
  const ArPragueSkyModelState  * state,
  double                         gamma,
  double                         alpha,
  double                         zero,
  double*                        control_params
)
{
  double res = 0;
  for (int t = 0; t < tensor_components; ++t) {
	const double sun_val_t = eval_pp(gamma, state->sun_nbreaks, state->sun_breaks, control_params + state->sun_offset + t * state->sun_stride);
	const double zenith_val_t = eval_pp(alpha, state->zenith_nbreaks, state->zenith_breaks, control_params + state->zenith_offset + t * state->zenith_stride);
	res += sun_val_t * zenith_val_t;
  }
  const double emph_val_t = eval_pp(zero, state->emph_nbreaks, state->emph_breaks, control_params + state->emph_offset);
  res *= emph_val_t;

  return M_MAX(res,0.);
}

double interpolate_elevation(
  const ArPragueSkyModelState  * state,
  double                  elevation,
  int                     altitude,
  int                     turbidity,
  int                     albedo,
  int                     wavelength,
  double                  gamma,
  double                  alpha,
  double                  zero
)
{
  const int elevation_low = (int)elevation;
  const double factor = elevation - (double)elevation_low;

  double* control_params_low = control_params_single_config(
    state,
    elevation_low,
    altitude,
    turbidity,
    albedo,
    wavelength);

  double res_low = reconstruct(
    state,
    gamma,
    alpha,
    zero,
    control_params_low);    

  if (factor < 1e-6 || elevation_low >= (elevations - 1))
  {
    return res_low;
  }

  double* control_params_high = control_params_single_config(
    state,
    elevation_low+1,
    altitude,
    turbidity,
    albedo,
    wavelength);

  double res_high = reconstruct(
    state,
    gamma,
    alpha,
    zero,
    control_params_high); 

  return lerp(res_low, res_high, factor);
}

double interpolate_altitude(
  const ArPragueSkyModelState  * state,
  double                  elevation,
  double                  altitude,
  int                     turbidity,
  int                     albedo,
  int                     wavelength,
  double                  gamma,
  double                  alpha,
  double                  zero
)
{
  const int altitude_low = (int)altitude;
  const double factor = altitude - (double)altitude_low;

  double res_low = interpolate_elevation(
    state,
    elevation,
    altitude_low,
    turbidity,
    albedo,
    wavelength,
    gamma,
    alpha,
    zero);    

  if (factor < 1e-6 || altitude_low >= (altitudes - 1))
  {
    return res_low;
  }

  double res_high = interpolate_elevation(
    state,
    elevation,
    altitude_low + 1,
    turbidity,
    albedo,
    wavelength,
    gamma,
    alpha,
    zero); 

  return lerp(res_low, res_high, factor);
}

double interpolate_turbidity(
  const ArPragueSkyModelState  * state,
  double                  elevation,
  double                  altitude,
  double                  turbidity,
  int                     albedo,
  int                     wavelength,
  double                  gamma,
  double                  alpha,
  double                  zero
)
{
  // Ignore turbidity

  return interpolate_altitude(
    state,
    elevation,
    altitude,
    (int)turbidity,
    albedo,
    wavelength,
    gamma,
    alpha,
    zero);
}

double interpolate_albedo(
  const ArPragueSkyModelState  * state,
  double                  elevation,
  double                  altitude,
  double                  turbidity,
  double                  albedo,
  int                     wavelength,
  double                  gamma,
  double                  alpha,
  double                  zero
)
{
  const int albedo_low = (int)albedo;
  const double factor = albedo - (double)albedo_low;

  double res_low = interpolate_turbidity(
    state,
    elevation,
    altitude,
    turbidity,
    albedo_low,
    wavelength,
    gamma,
    alpha,
    zero);    

  if (factor < 1e-6 || albedo_low >= (albedos - 1))
  {
    return res_low;
  }

  double res_high = interpolate_turbidity(
    state,
    elevation,
    altitude,
    turbidity,
    albedo_low + 1,
    wavelength,
    gamma,
    alpha,
    zero); 

  return lerp(res_low, res_high, factor);
}

double interpolate_wavelength(
  const ArPragueSkyModelState  * state,
  double                  elevation,
  double                  altitude,
  double                  turbidity,
  double                  albedo,
  double                  wavelength,
  double                  gamma,
  double                  alpha,
  double                  zero
)
{
  // Don't interpolate, use the bin it belongs to

  return interpolate_albedo(
    state,
    elevation,
    altitude,
    turbidity,
    albedo,
    (int)wavelength,
    gamma,
    alpha,
    zero);
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
  // Translate parameter values to indices

  double altitude_control;
  if (altitude < altitude_vals[0])
  {
     altitude_control = 0;
  }
  else if (altitude > altitude_vals[altitudes - 1])
  {
     altitude_control = altitudes - 1;
  }
  else
  {
     for (int a = 1; a < altitudes; ++a)
     {
	double val = altitude_vals[a];
        if (fabs(altitude - val) < 1e-6)
        {
           altitude_control = a;
           break;
        }
	else if (altitude < val)
        {
	   altitude_control = a - ((val - altitude) / (val - altitude_vals[a - 1]));
           break;
        }
     }
  }

  double elevation_control;
  if (elevation < elevation_vals[0])
  {
     elevation_control = 0;
  }
  else if (elevation > elevation_vals[elevations - 1])
  {
     elevation_control = elevations - 1;
  }
  else
  {
     for (int e = 1; e < elevations; ++e)
     {
        double val = elevation_vals[e];
        if (fabs(elevation - val) < 1e-6)
        {
           elevation_control = e;
           break;
        }
	else if (elevation < val)
        {
	   elevation_control = e - ((val - elevation) / (val - elevation_vals[e - 1]));
           break;
        }
     }
  }

  const double albedo_control = albedo * (albedos - 1);

  const double channel = (wavelength - 320.0) / 40.0;
  if ( channel >= 11. || channel < 0.) return 0.; 

  // Get params corresponding to the indices, reconstruct result and interpolate

  const double alpha = elevation < 0 ? shadow : zero;
  return interpolate_wavelength(
    state,
    elevation_control,
    altitude_control,
    turbidity,
    albedo_control,
    channel,
    gamma,
    alpha,
    zero);

  /*const int albedo_low = (int)albedo_control;
  //const double factor = albedo_control - (double)albedo_low;
  const int elevation_low = (int)elevation_control;
  const double factor = elevation_control - (double)elevation_low;*/  
  
  /*double* control_params_low = state->radiance_dataset[(int)channel] 
    + state->total_coefs_single_config * ((int)elevation_control + elevations*((int)altitude_control) + elevations*altitudes*albedo_low);

  double* control_params_high;
  if (factor < 1e-6 || albedo_low >= (albedos - 1)) {
     control_params_high = control_params_low;
  } else {
     control_params_high = state->radiance_dataset[(int)channel] 
    + state->total_coefs_single_config * ((int)elevation_control + elevations*((int)altitude_control) + elevations*altitudes*(albedo_low + 1));
  }

  double control_params_interpolated[state->total_coefs_single_config];
  lerp_control_params(control_params_low, control_params_high, control_params_interpolated, factor, state->total_coefs_single_config);*/

  // Interpolate params
  /*double control_params_interpolated[state->total_coefs_single_config];
  control_params(
    state,
    elevation_control,
    altitude_control,
    turbidity,
    albedo_control,
    channel,
    control_params_interpolated);

  const double alpha = elevation < 0 ? shadow : zero;
  double res = 0;
  for (int t = 0; t < tensor_components; ++t) {
	const double sun_val_t = eval_pp(gamma, state->sun_nbreaks, state->sun_breaks, control_params_interpolated + state->sun_offset + t * state->sun_stride);
	const double zenith_val_t = eval_pp(alpha, state->zenith_nbreaks, state->zenith_breaks, control_params_interpolated + state->zenith_offset + t * state->zenith_stride);
	res += sun_val_t * zenith_val_t;
  }
  double emph_val_t = eval_pp(zero, state->emph_nbreaks, state->emph_breaks, control_params_interpolated + state->emph_offset);
  res *= emph_val_t;

  res = M_MAX(res,0.);*/


  // No interpolation

  // Compute radiance from the params for the lower albedo

  /*const double alpha = elevation < 0 ? shadow : zero;

  double* control_params_interpolated = state->radiance_dataset[(int)channel] 
    + state->total_coefs_single_config * (elevation_low + elevations*((int)altitude_control) + elevations*altitudes*(albedo_low));
  
  double res = 0;
  for (int t = 0; t < tensor_components; ++t) {
	const double sun_val_t = eval_pp(gamma, state->sun_nbreaks, state->sun_breaks, control_params_interpolated + state->sun_offset + t * state->sun_stride);
	const double zenith_val_t = eval_pp(alpha, state->zenith_nbreaks, state->zenith_breaks, control_params_interpolated + state->zenith_offset + t * state->zenith_stride);
	res += sun_val_t * zenith_val_t;
  }
  double emph_val_t = eval_pp(zero, state->emph_nbreaks, state->emph_breaks, control_params_interpolated + state->emph_offset);
  res *= emph_val_t;

  res = M_MAX(res,0.);*/

  /*if (factor < 1e-6 || albedo_low >= (albedos - 1)) {
     return res;
  }*/

  // Compute radiance from the params for the higher albedo

  /*control_params_interpolated = state->radiance_dataset[(int)channel] 
    + state->total_coefs_single_config * (elevation_low + 1 + elevations*((int)altitude_control) + elevations*altitudes*(albedo_low));  

  double res2 = 0;
  for (int t = 0; t < tensor_components; ++t) {
	const double sun_val_t = eval_pp(gamma, state->sun_nbreaks, state->sun_breaks, control_params_interpolated + state->sun_offset + t * state->sun_stride);
	const double zenith_val_t = eval_pp(alpha, state->zenith_nbreaks, state->zenith_breaks, control_params_interpolated + state->zenith_offset + t * state->zenith_stride);
	res2 += sun_val_t * zenith_val_t;
  }
  emph_val_t = eval_pp(zero, state->emph_nbreaks, state->emph_breaks, control_params_interpolated + state->emph_offset);
  res2 *= emph_val_t;

  res2 = M_MAX(res2,0.);*/

  //debugprintf("elc1: %d, elc2: %d", (int)elevation_control, ((int)(elevation_control)+1));
  //debugprintf("res1: %f, res2: %f, factor: %f", res, res2, factor);

  //return lerp(res, res2, factor);
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

const double SunRadStartWL = 310;
const double SunRadIncrementWL = 1;
const double SunRadTable[] =
{
9829.41, 10184., 10262.6, 10375.7, 10276., 10179.3, 10156.6, 10750.7, 11134., 11463.6, 11860.4, 12246.2, 12524.4, 12780., 13187.4, 13632.4, 13985.9, 13658.3, 13377.4, 13358.3, 13239., 13119.8, 13096.2, 13184., 13243.5, 13018.4, 12990.4, 13159.1, 13230.8, 13258.6, 13209.9, 13343.2, 13404.8, 13305.4, 13496.3, 13979.1, 14153.8, 14188.4, 14122.7, 13825.4, 14033.3, 13914.1, 13837.4, 14117.2, 13982.3, 13864.5, 14118.4, 14545.7, 15029.3, 15615.3, 15923.5, 16134.8, 16574.5, 16509., 16336.5, 16146.6, 15965.1, 15798.6, 15899.8, 16125.4, 15854.3, 15986.7, 15739.7, 15319.1, 15121.5, 15220.2, 15041.2, 14917.7, 14487.8, 14011., 14165.7, 14189.5, 14540.7, 14797.5, 14641.5, 14761.6, 15153.7, 14791.8, 14907.6, 15667.4, 16313.5, 16917., 17570.5, 18758.1, 20250.6, 21048.1, 21626.1, 22811.6, 23577.2, 23982.6, 24062.1, 23917.9, 23914.1, 23923.2, 24052.6, 24228.6, 24360.8, 24629.6, 24774.8, 24648.3, 24666.5, 24938.6, 24926.3, 24693.1, 24613.5, 24631.7, 24569.8, 24391.5, 24245.7, 24084.4, 23713.7, 22985.4, 22766.6, 22818.9, 22834.3, 22737.9, 22791.6, 23086.3, 23377.7, 23461., 23935.5, 24661.7, 25086.9, 25520.1, 25824.3, 26198., 26350.2, 26375.4, 26731.2, 27250.4, 27616., 28145.3, 28405.9, 28406.8, 28466.2, 28521.5, 28783.8, 29025.1, 29082.6, 29081.3, 29043.1, 28918.9, 28871.6, 29049., 29152.5, 29163.2, 29143.4, 28962.7, 28847.9, 28854., 28808.7, 28624.1, 28544.2, 28461.4, 28411.1, 28478., 28469.8, 28513.3, 28586.5, 28628.6, 28751.5, 28948.9, 29051., 29049.6, 29061.7, 28945.7, 28672.8, 28241.5, 27903.2, 27737., 27590.9, 27505.6, 27270.2, 27076.2, 26929.1, 27018.2, 27206.8, 27677.2, 27939.9, 27923.9, 27899.2, 27725.4, 27608.4, 27599.4, 27614.6, 27432.4, 27460.4, 27392.4, 27272., 27299.1, 27266.8, 27386.5, 27595.9, 27586.9, 27504.8, 27480.6, 27329.8, 26968.4, 26676.3, 26344.7, 26182.5, 26026.3, 25900.3, 25842.9, 25885.4, 25986.5, 26034.5, 26063.5, 26216.9, 26511.4, 26672.7, 26828.5, 26901.8, 26861.5, 26865.4, 26774.2, 26855.8, 27087.1, 27181.3, 27183.1, 27059.8, 26834.9, 26724.3, 26759.6, 26725.9, 26724.6, 26634.5, 26618.5, 26560.1, 26518.7, 26595.3, 26703.2, 26712.7, 26733.9, 26744.3, 26764.4, 26753.2, 26692.7, 26682.7, 26588.1, 26478., 26433.7, 26380.7, 26372.9, 26343.3, 26274.7, 26162.3, 26160.5, 26210., 26251.2, 26297.9, 26228.9, 26222.3, 26269.7, 26295.6, 26317.9, 26357.5, 26376.1, 26342.4, 26303.5, 26276.7, 26349.2, 26390., 26371.6, 26346.7, 26327.6, 26274.2, 26247.3, 26228.7, 26152.1, 25910.3, 25833.2, 25746.5, 25654.3, 25562., 25458.8, 25438., 25399.1, 25324.3, 25350., 25514., 25464.9, 25398.5, 25295.2, 25270.2, 25268.4, 25240.6, 25184.9, 25149.6, 25123.9, 25080.3, 25027.9, 25012.3, 24977.9, 24852.6, 24756.4, 24663.5, 24483.6, 24398.6, 24362.6, 24325.1, 24341.7, 24288.7, 24284.2, 24257.3, 24178.8, 24097.6, 24175.6, 24175.7, 24139.7, 24088.1, 23983.2, 23902.7, 23822.4, 23796.2, 23796.9, 23814.5, 23765.5, 23703., 23642., 23592.6, 23552., 23514.6, 23473.5, 23431., 23389.3, 23340., 23275.1, 23187.3, 23069.5, 22967., 22925.3, 22908.9, 22882.5, 22825., 22715.4, 22535.5, 22267.1, 22029.4, 21941.6, 21919.5, 21878.8, 21825.6, 21766., 21728.9, 21743.2, 21827.1, 21998.7, 22159.4, 22210., 22187.2, 22127.2, 22056.2, 22000.2, 21945.9, 21880.2, 21817.1, 21770.3, 21724.3, 21663.2, 21603.3, 21560.4, 21519.8, 21466.2, 21401.6, 21327.7, 21254.2, 21190.7, 21133.6, 21079.3, 21024., 20963.7, 20905.5, 20856.6, 20816.6, 20785.2, 20746.7, 20685.3, 20617.8, 20561.1, 20500.4, 20421.2, 20333.4, 20247., 20175.3, 20131.4, 20103.2, 20078.5, 20046.8, 19997.2, 19952.9, 19937.2, 19930.8, 19914.4, 19880.8, 19823., 19753.8, 19685.9, 19615.3, 19537.5, 19456.8, 19377.6, 19309.4, 19261.9, 19228., 19200.5, 19179.5, 19164.8, 19153.1, 19140.6, 19129.2, 19120.6, 19104.5, 19070.6, 19023.9, 18969.3, 18911.4, 18855., 18798.6, 18740.8, 18672.7, 18585.2, 18501., 18442.4, 18397.5, 18353.9, 18313.2, 18276.8, 18248.3, 18231.2, 18224., 18225.4, 18220.1, 18192.6, 18155.1, 18119.8, 18081.6, 18035.6, 17987.4, 17942.8, 17901.7, 17864.2, 17831.1, 17802.9, 17771.5, 17728.6, 17669.7, 17590.1, 17509.5, 17447.4, 17396., 17347.4, 17300.3, 17253.2, 17206.1, 17159., 17127.6, 17127.6, 17133.6, 17120.4, 17097.2, 17073.3, 17043.7, 17003.4, 16966.3, 16946.3, 16930.9, 16907.7, 16882.7, 16862., 16837.8, 16802.1, 16759.2, 16713.6, 16661.8, 16600.8, 16542.6, 16499.4, 16458.7, 16408., 16360.6, 16329.5, 16307.4, 16286.7, 16264.9, 16239.6, 16207.8, 16166.8, 16118.2, 16064., 16011.2, 15966.9, 15931.9, 15906.9, 15889.1, 15875.5, 15861.2, 15841.3, 15813.1, 15774.2, 15728.8, 15681.4, 15630., 15572.9, 15516.5, 15467.2, 15423., 15381.6, 15354.4, 15353., 15357.3, 15347.3, 15320.2, 15273.1, 15222., 15183.1, 15149.6, 15114.6, 15076.8, 15034.6, 14992.9
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
    /*const int low_wl = (wavelength - 320.0 ) / 40.0;

    if ( low_wl < 0 || low_wl >= 11 )
        return 0.0f;

    double result = psm_originalSolarRadianceTable[low_wl];

    double interp = fmod((wavelength - 320.0 ) / 40.0, 1.0);
    if ( interp > 1e-6 && low_wl < 10 )
    {
        result = ( 1.0 - interp ) * result;
        result += interp * psm_originalSolarRadianceTable[low_wl+1];
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
    
    return result * tau;*/

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

    /*const int wl = (int)(wavelength - SunRadStartWL);

    if ( wl < 0 || wl >= 521 )
        return 0.0f;

    double result = SunRadTable[wl];

    double  tau =
        arpragueskymodel_tau(
            state,
            theta,
            altitude,
            turbidity,
            wavelength,
            MATH_HUGE_DOUBLE
            );
    
    return result * tau;*/
}

int arpragueskymodel_circleBounds2D(
	double x_v,
	double y_v,
	double y_c,
	double radius,
	double *d
)
{
	double qa = (x_v * x_v) + (y_v * y_v);
	double qb = 2.0 * y_c * y_v;
	double qc = (y_c * y_c) - (radius * radius);
	double n = (qb * qb) - (4.0 * qa * qc);
	if (n <= 0)
	{
		return 0;
	}
	float d1;
	float d2;
	n = sqrt(n);
	d1 = (-qb + n) / (2.0 * qa);
	d2 = (-qb - n) / (2.0 * qa);
	*d = (d1 > 0 && d2 > 0) ? (d1 < d2 ? d1 : d2) : (d1 > d2 ? d1 : d2); // It fits in one line.
	if (*d <= 0)
	{
		return 0;
	}
	return 1;
}

void arpragueskymodel_scaleAD(
	double x_p,
	double y_p,
	double *a,
	double *d
)
{
	double n;
	n = sqrt((x_p * x_p) + (y_p * y_p));
	*a = n - planet_radius;
	*a = *a > 0 ? *a : 0;
	*a = pow(*a / 100000.0, 1.0 / 3.0);
	*d = acos(y_p / n) * planet_radius;
	*d = *d / 1571524.413613; // Maximum distance to the edge of the atmosphere in the transmittance model
	*d = pow(*d, 0.25);
	*d = *d > 1.0 ? 1.0 : *d;
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
	double n;
	if (arpragueskymodel_circleBounds2D(x_v, y_v, y_c, planet_radius, &n) == 1) // Check for planet intersection
	{
		if (n <= distance) // We do intersect the planet so return a and d at the surface
		{
			double x_p = x_v * n;
			double y_p = (y_v * n) + planet_radius + altitude;
			arpragueskymodel_scaleAD(x_p, y_p, a, d);
			return;
		}
	}
	if (arpragueskymodel_circleBounds2D(x_v, y_v, y_c, atmo_edge, &n) == 0)
	{
		// Then we have a problem!
		// Return something, but this should never happen so long as the camera is inside the atmosphere
		// Which it should be in this work
		*a = 0;
		*d = 0;
		return;
	}
	double distance_corrected = n;
	// Use the smaller of the distances
	distance_corrected = distance < distance_corrected ? distance : distance_corrected;
	// Points in world space
	double x_p = x_v * distance_corrected;
	double y_p = (y_v * distance_corrected) + planet_radius + altitude;
	arpragueskymodel_scaleAD(x_p, y_p, a, d);
}

float *arpragueskymodel_transmittanceCoefsIndex(const ArPragueSkyModelState  * state,
	int altitude,
	int wavelength
)
{
	return &state->transmission_dataset[(transsvdrank * 86 * 101) + (((altitude * 11) + wavelength) * transsvdrank)];
}

void arpragueskymodel_transmittanceInterpolateWaveLength(
	const ArPragueSkyModelState  * state,
	int altitude,
	int wavelength_low,
	int wavelength_inc,
	double wavelength_w,
	float *coefficients
)
{
	float *wll = arpragueskymodel_transmittanceCoefsIndex(state, altitude, wavelength_low);
	float *wlu = arpragueskymodel_transmittanceCoefsIndex(state, altitude, wavelength_low + wavelength_inc);
	double iw = 1.0 - wavelength_w;
	for (int i = 0; i < transsvdrank; i++)
	{
		coefficients[i] = (wll[i] * iw) + (wlu[i] * wavelength_w);
	}
}

void arpragueskymodel_transmittanceInterpolateAltitude(
	const ArPragueSkyModelState  * state,
	int altitude_low,
	int altitude_inc,
	double altitude_weight,
	int wavelength_low,
	int wavelength_inc,
	double wavelength_w,
	float *coefficients
)
{
	float au[transsvdrank];
	float al[transsvdrank];
	arpragueskymodel_transmittanceInterpolateWaveLength(state, altitude_low, wavelength_low, wavelength_inc, wavelength_w, al);
	arpragueskymodel_transmittanceInterpolateWaveLength(state, altitude_low + altitude_inc, wavelength_low, wavelength_inc, wavelength_w, au);
	double iw = 1.0 - altitude_weight;
	for (int i = 0; i < transsvdrank; i++)
	{
		coefficients[i] = (al[i] * iw) + (au[i] * altitude_weight);
	}
}

double arpragueskymodel_calc_transmittance_svd(const ArPragueSkyModelState  * state, double a, double d, float *interpolated_coefficients)
{
	float t[4] = { 0.0, 0.0, 0.0, 0.0 };
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
	for (int al = aa; al <= aa + aainc; al++)
	{
		for (int dl = da; dl <= da + dainc; dl++)
		{
			for (int i = 0; i < transsvdrank; i++)
			{
				t[index] = t[index] + (state->transmission_dataset[(((dl * 86) + al) * transsvdrank) + i] * interpolated_coefficients[i]);
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
	t[0] = t[0] * t[0] * t[0];
	return t[0];
}

double cbrt(double x)
{
	return pow(x, 1.0 / 3.0);
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

	const double wavelength_norm = (wavelength - 340.0) / 40.0;
	if (wavelength_norm > 10. || wavelength_norm < 0.)
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
	float interpolatedCoefs[transsvdrank];
	arpragueskymodel_transmittanceInterpolateAltitude(state, altitude_low, altitude_inc, altitude_factor, wavelength_low, wavelength_inc, wavelength_factor, interpolatedCoefs);

	// Evaluate basis
	double trans = arpragueskymodel_calc_transmittance_svd(state, a, d, interpolatedCoefs);

	return trans;
}

double* control_params_single_config_pol(
  const ArPragueSkyModelState  * state,
  int                     elevation,
  int                     altitude,
  int                     turbidity,
  int                     albedo,
  int                     wavelength
)
{
  // turbidity is completely ignored for now
  return state->polarisation_dataset[wavelength] + state->total_coefs_single_config_pol * (elevation + elevations*altitude + elevations*altitudes*albedo);
}

double reconstruct_pol(
  const ArPragueSkyModelState  * state,
  double                         gamma,
  double                         alpha,
  double*                        control_params
)
{
  double res = 0;
  for (int t = 0; t < tensor_components_pol; ++t) {
	const double sun_val_t = eval_pp(gamma, state->sun_nbreaks_pol, state->sun_breaks_pol, control_params + state->sun_offset_pol + t * state->sun_stride_pol);
	const double zenith_val_t = eval_pp(alpha, state->zenith_nbreaks_pol, state->zenith_breaks_pol, control_params + state->zenith_offset_pol + t * state->zenith_stride_pol);
	res += sun_val_t * zenith_val_t;
  }

  return res;
}

double interpolate_elevation_pol(
  const ArPragueSkyModelState  * state,
  double                  elevation,
  int                     altitude,
  int                     turbidity,
  int                     albedo,
  int                     wavelength,
  double                  gamma,
  double                  alpha
)
{
  const int elevation_low = (int)elevation;
  const double factor = elevation - (double)elevation_low;

  double* control_params_low = control_params_single_config_pol(
    state,
    elevation_low,
    altitude,
    turbidity,
    albedo,
    wavelength);

  double res_low = reconstruct_pol(
    state,
    gamma,
    alpha,
    control_params_low);    

  if (factor < 1e-6 || elevation_low >= (elevations - 1))
  {
    return res_low;
  }

  double* control_params_high = control_params_single_config_pol(
    state,
    elevation_low+1,
    altitude,
    turbidity,
    albedo,
    wavelength);

  double res_high = reconstruct_pol(
    state,
    gamma,
    alpha,
    control_params_high); 

  return lerp(res_low, res_high, factor);
}

double interpolate_altitude_pol(
  const ArPragueSkyModelState  * state,
  double                  elevation,
  double                  altitude,
  int                     turbidity,
  int                     albedo,
  int                     wavelength,
  double                  gamma,
  double                  alpha
)
{
  const int altitude_low = (int)altitude;
  const double factor = altitude - (double)altitude_low;

  double res_low = interpolate_elevation_pol(
    state,
    elevation,
    altitude_low,
    turbidity,
    albedo,
    wavelength,
    gamma,
    alpha);    

  if (factor < 1e-6 || altitude_low >= (altitudes - 1))
  {
    return res_low;
  }

  double res_high = interpolate_elevation_pol(
    state,
    elevation,
    altitude_low + 1,
    turbidity,
    albedo,
    wavelength,
    gamma,
    alpha); 

  return lerp(res_low, res_high, factor);
}

double interpolate_turbidity_pol(
  const ArPragueSkyModelState  * state,
  double                  elevation,
  double                  altitude,
  double                  turbidity,
  int                     albedo,
  int                     wavelength,
  double                  gamma,
  double                  alpha
)
{
  // Ignore turbidity

  return interpolate_altitude_pol(
    state,
    elevation,
    altitude,
    (int)turbidity,
    albedo,
    wavelength,
    gamma,
    alpha);
}

double interpolate_albedo_pol(
  const ArPragueSkyModelState  * state,
  double                  elevation,
  double                  altitude,
  double                  turbidity,
  double                  albedo,
  int                     wavelength,
  double                  gamma,
  double                  alpha
)
{
  const int albedo_low = (int)albedo;
  const double factor = albedo - (double)albedo_low;

  double res_low = interpolate_turbidity_pol(
    state,
    elevation,
    altitude,
    turbidity,
    albedo_low,
    wavelength,
    gamma,
    alpha);    

  if (factor < 1e-6 || albedo_low >= (albedos - 1))
  {
    return res_low;
  }

  double res_high = interpolate_turbidity_pol(
    state,
    elevation,
    altitude,
    turbidity,
    albedo_low + 1,
    wavelength,
    gamma,
    alpha); 

  return lerp(res_low, res_high, factor);
}

double interpolate_wavelength_pol(
  const ArPragueSkyModelState  * state,
  double                  elevation,
  double                  altitude,
  double                  turbidity,
  double                  albedo,
  double                  wavelength,
  double                  gamma,
  double                  alpha
)
{
  // Don't interpolate, use the bin it belongs to

  return interpolate_albedo_pol(
    state,
    elevation,
    altitude,
    turbidity,
    albedo,
    (int)wavelength,
    gamma,
    alpha);
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
  // Translate parameter values to indices

  double altitude_control;
  if (altitude < altitude_vals[0])
  {
     altitude_control = 0;
  }
  else if (altitude > altitude_vals[altitudes - 1])
  {
     altitude_control = altitudes - 1;
  }
  else
  {
     for (int a = 1; a < altitudes; ++a)
     {
	double val = altitude_vals[a];
        if (fabs(altitude - val) < 1e-6)
        {
           altitude_control = a;
           break;
        }
	else if (altitude < val)
        {
	   altitude_control = a - ((val - altitude) / (val - altitude_vals[a - 1]));
           break;
        }
     }
  }

  double elevation_control;
  if (elevation < elevation_vals[0])
  {
     elevation_control = 0;
  }
  else if (elevation > elevation_vals[elevations - 1])
  {
     elevation_control = elevations - 1;
  }
  else
  {
     for (int e = 1; e < elevations; ++e)
     {
        double val = elevation_vals[e];
        if (fabs(elevation - val) < 1e-6)
        {
           elevation_control = e;
           break;
        }
	else if (elevation < val)
        {
	   elevation_control = e - ((val - elevation) / (val - elevation_vals[e - 1]));
           break;
        }
     }
  }

  const double albedo_control = albedo * (albedos - 1);

  const double channel = (wavelength - 320.0) / 40.0;
  if ( channel >= 11. || channel < 0.) return 0.; 

  // Get params corresponding to the indices, reconstruct result and interpolate

  return -interpolate_wavelength_pol(
    state,
    elevation_control,
    altitude_control,
    turbidity,
    albedo_control,
    channel,
    gamma,
    theta);
}


#else

const double background_breaks[] = {0, 1.745329e-01, 3.490659e-01, 5.235988e-01, 6.981317e-01, 8.726646e-01, 1.047198e+00, 1.221730e+00, 1.396263e+00, 1.466077e+00, 1.483530e+00, 1.500983e+00, 1.518436e+00, 1.535890e+00, 1.553343e+00, 1.562070e+00, 1.570796e+00, 1.579523e+00, 1.588250e+00, 1.605703e+00, 1.623156e+00, 1.640609e+00, 1.658063e+00, 1.675516e+00, 1.745329e+00, 1.867502e+00, 2.094395e+00, 2.617994e+00, 3.141593e+00};
const double solar_max_breaks[] = {0, 1.745329e-02, 5.235988e-02, 1.047198e-01, 1.570796e-01, 2.094395e-01, 2.617994e-01, 3.141593e-01, 3.665191e-01, 4.188790e-01, 4.712389e-01, 5.235988e-01, 5.759587e-01, 6.283185e-01, 6.806784e-01, 7.330383e-01, 7.853982e-01, 8.726646e-01, 9.599311e-01, 1.047198e+00, 1.134464e+00, 1.221730e+00, 1.308997e+00, 1.396263e+00, 1.483530e+00, 1.570796e+00, 3.141593e+00};
const double solar_ratio_breaks[] = {0, 3.490659e-01, 6.981317e-01, 8.726646e-01, 1.047198e+00, 1.221730e+00, 1.256637e+00, 1.291544e+00, 1.326450e+00, 1.361357e+00, 1.396263e+00, 1.431170e+00, 1.466077e+00, 1.500983e+00, 1.535890e+00, 1.570796e+00, 1.605703e+00, 1.640609e+00, 1.675516e+00, 1.710423e+00, 1.745329e+00, 1.780236e+00, 1.815142e+00, 1.850049e+00, 1.884956e+00, 1.919862e+00, 2.094395e+00, 3.141593e+00};
const double backglow_vertical_breaks[] = {0, 1.570796e+00, 2.094395e+00, 2.617994e+00, 2.967060e+00, 3.141593e+00};
const double backglow_ratio_breaks[] = {0, 5.235988e-01, 1.047198e+00, 1.274090e+00, 1.396263e+00, 1.466077e+00, 1.518436e+00, 1.535890e+00, 1.553343e+00, 1.562070e+00, 1.570796e+00, 1.579523e+00, 1.588250e+00, 1.605703e+00, 1.623156e+00, 1.675516e+00, 1.745329e+00, 1.867502e+00, 2.094395e+00, 2.617994e+00, 3.141593e+00};
const double frontglow_vertical_breaks[] = {0, 1.745329e-02, 5.235988e-02, 1.047198e-01, 1.570796e-01, 2.094395e-01, 2.617994e-01, 3.141593e-01, 3.665191e-01, 4.188790e-01, 4.712389e-01, 5.235988e-01, 5.759587e-01, 6.283185e-01, 6.806784e-01, 7.330383e-01, 7.853982e-01, 8.726646e-01, 9.599311e-01, 1.047198e+00, 2.094395e+00, 3.141593e+00};
const double frontglow_ratio_breaks[] = {0, 1.745329e-01, 3.490659e-01, 5.235988e-01, 6.981317e-01, 8.726646e-01, 1.047198e+00, 1.221730e+00, 1.256637e+00, 1.291544e+00, 1.326450e+00, 1.361357e+00, 1.396263e+00, 1.431170e+00, 1.466077e+00, 1.500983e+00, 1.535890e+00, 1.553343e+00, 1.562070e+00, 1.570796e+00, 1.579523e+00, 1.588250e+00, 1.605703e+00, 1.640609e+00, 1.675516e+00, 1.710423e+00, 1.745329e+00, 1.780236e+00, 1.815142e+00, 1.850049e+00, 1.884956e+00, 1.919862e+00, 2.094395e+00, 2.268928e+00, 2.443461e+00, 2.617994e+00, 2.792527e+00, 2.967060e+00, 3.141593e+00};
const double shadow_breaks[] = {-1.570796e+00, -2.617994e-01, -1.745329e-01, -1.396263e-01, -1.047198e-01, -8.726646e-02, -6.981317e-02, -5.235988e-02, -4.363323e-02, -3.490659e-02, -2.617994e-02, -1.745329e-02, -8.726646e-03, 0, 8.726646e-03, 1.745329e-02, 2.617994e-02, 3.490659e-02, 4.363323e-02, 5.235988e-02, 6.108652e-02, 6.981317e-02, 7.853982e-02, 8.726646e-02, 1.047198e-01, 1.221730e-01, 1.396263e-01, 1.570796e-01, 1.745329e-01, 2.094395e-01, 2.443461e-01, 2.792527e-01, 3.141593e-01, 3.490659e-01, 1.570796e+00};
const double shadow_v_breaks[] = {0, 1.047198e+00, 2.094395e+00, 2.443461e+00, 2.792527e+00, 3.141593e+00};
const double shadow_h_breaks[] = {0, 6.981317e-01, 1.396263e+00, 1.483530e+00, 1.500983e+00, 1.518436e+00, 1.535890e+00, 1.553343e+00, 1.570796e+00, 1.588250e+00, 1.605703e+00, 1.623156e+00, 1.640609e+00, 1.658063e+00, 1.745329e+00, 2.443461e+00, 3.141593e+00};

const int background_nbreaks = 28;
const int solar_max_nbreaks = 26;
const int solar_ratio_nbreaks = 27;
const int backglow_vertical_nbreaks = 5;
const int backglow_ratio_nbreaks = 20;
const int frontglow_vertical_nbreaks = 21;
const int frontglow_ratio_nbreaks = 38;
const int shadow_nbreaks = 34;
const int shadow_v_nbreaks = 5;
const int shadow_h_nbreaks = 16;

const int background_offset = 0;
const int solar_max_offset = background_offset + 4 * background_nbreaks;
const int solar_ratio_offset = solar_max_offset + 4 * solar_max_nbreaks;
const int backglow_vertical_offset = solar_ratio_offset + 4 * solar_ratio_nbreaks;
const int backglow_ratio_offset = backglow_vertical_offset + 4 * backglow_vertical_nbreaks;
const int frontglow_vertical_offset = backglow_ratio_offset + 4 * backglow_ratio_nbreaks;
const int frontglow_ratio_offset = frontglow_vertical_offset + 4 * frontglow_vertical_nbreaks;
const int shadow_offset = frontglow_ratio_offset + 4 * frontglow_ratio_nbreaks;
const int shadow_v_offset = shadow_offset + 4 * shadow_nbreaks;
const int shadow_h_offset = shadow_v_offset + 4 * shadow_v_nbreaks;
const int total_coefs_single_config = shadow_h_offset + 4 * shadow_h_nbreaks; // this is for one specific configuration

const int elevations = 31;
const int altitudes = 21;
const int albedos = 2;

const int total_coefs_all_configs = total_coefs_single_config * elevations * altitudes * albedos;

const double polarisation_max_breaks[] = {0, 5.235988e-01, 1.047198e+00, 1.570796e+00, 2.094395e+00, 2.617994e+00, 3.141593e+00};
const double polarisation_ratio_breaks[] = {0, 7.853982e-01, 1.396263e+00, 1.483530e+00, 1.553343e+00, 1.570796e+00, 1.588250e+00, 1.658063e+00, 1.745329e+00, 2.356194e+00, 3.141593e+00};

const int polarisation_max_nbreaks = 6;
const int polarisation_ratio_nbreaks = 10;

const int polarisation_max_offset = 0;
const int polarisation_ratio_offset = polarisation_max_offset + 4 * polarisation_max_nbreaks;
const int total_polarisation_coefs_single_config = polarisation_ratio_offset + 4 * polarisation_ratio_nbreaks; // this is for one specific configuration

const int total_polarisation_coefs_all_configs = total_polarisation_coefs_single_config * elevations * altitudes * albedos;

const int transmittance_angles = 360;
const int transmittance_onionlayers = 31;
const int total_transmittance_values = 11 * 11 * altitudes * transmittance_onionlayers * transmittance_angles;
const double planet_radius = 6378000.0;

ArPragueSkyModelState  * arpragueskymodelstate_alloc_init(
        const char  * library_path
        )
{
    ArPragueSkyModelState  * state = ALLOC(ArPragueSkyModelState);

    for (int wl = 0; wl < 11; ++wl)
    {
        char filename[1024];
        
        sprintf(filename, "%s/SkyModel/params_t%d_wl%d.dat", library_path, 4, wl+1);
        
        FILE* handle = fopen(filename, "rb");
        
        state->radiance_dataset[wl] = ALLOC_ARRAY(double,total_coefs_all_configs);
        
        fread(state->radiance_dataset[wl], sizeof(double), total_coefs_all_configs, handle);
        
        fclose(handle);
        
        sprintf(filename, "%s/SkyModel/polarisation_params_t%d_wl%d.dat", library_path, 4, wl+1);
        
        handle = fopen(filename, "rb");
        
        state->polarisation_dataset[wl] = (double*)malloc(sizeof(double) * total_polarisation_coefs_all_configs);
        fread(state->polarisation_dataset[wl], sizeof(double), total_polarisation_coefs_all_configs, handle);
        fclose(handle);

    }
    
    // Read in transmittance
    char filenametransmittance[1024];
    
    sprintf(filenametransmittance, "%s/SkyModel/Transmittance.dat", library_path);
    
    FILE* handletrans = fopen(filenametransmittance, "rb");
    
    state->transmission_dataset = ALLOC_ARRAY(float, total_transmittance_values);
    
    fread(state->transmission_dataset, sizeof(double), total_transmittance_values, handletrans);
    
    fclose(handletrans);
    // Done

    return state;
}

void arpragueskymodelstate_free(
        ArPragueSkyModelState  * state
        )
{
    for (int wl = 1; wl < 11; ++wl)
    {
        free(state->radiance_dataset[wl]);
        free(state->polarisation_dataset[wl]);
    }

    free(state->transmission_dataset);
    
    FREE(state);
}

#define PLANET_RADIUS       6378000.0 METER

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
    Vec3D  dirToPCNorm;
    
    vec3d_v_norm_v(   viewDirection, & viewDirNorm );
    vec3d_v_norm_v( & directionToPlanetCenter,  & dirToPCNorm );
    
    double  cosTheta =
        vec3d_vv_dot(
            & viewDirNorm,
            & dirToPCNorm
            );

    *theta  = acos(cosTheta);
    *gamma  = acos(dotProductSun);
    *shadow = atan2(-ZC(zeroToView), len);
    *zero   = acos(dotProductZero);
    
    *altitudeOfViewpoint = M_MAX( *altitudeOfViewpoint, 0.0 );
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
  return state->radiance_dataset[wavelength] + total_coefs_single_config * (elevation + elevations*altitude + elevations*altitudes*albedo);
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

  lerp_control_params(control_params_low, control_params_high, result, factor, total_coefs_single_config);
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

  double control_params_low[total_coefs_single_config];
  control_params_single_altitude(
    state,
    elevation,
    altitude_low,
    turbidity,
    albedo,
    wavelength,
    control_params_low
    );

  double control_params_high[total_coefs_single_config];
  control_params_single_altitude(
    state,
    elevation,
    altitude_low+1,
    turbidity,
    albedo,
    wavelength,
    control_params_high);

  lerp_control_params(control_params_low, control_params_high, result, factor, total_coefs_single_config);
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

  double control_params_low[total_coefs_single_config];
  control_params_single_turbidity(
    state,
    elevation,
    altitude,
    turbidity_low,
    albedo,
    wavelength,
    control_params_low);

  double control_params_high[total_coefs_single_config];
  control_params_single_turbidity(
    state,
    elevation,
    altitude,
    turbidity_low+1,
    albedo,
    wavelength,
    control_params_high);

  lerp_control_params(control_params_low, control_params_high, result, factor, total_coefs_single_config);
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

  double control_params_low[total_coefs_single_config];
  control_params_single_albedo(
    state,
    elevation,
    altitude,
    turbidity,
    albedo_low,
    wavelength,
    control_params_low);

  double control_params_high[total_coefs_single_config];
  control_params_single_albedo(
    state,
    elevation,
    altitude,
    turbidity,
    albedo_low+1,
    wavelength,
    control_params_high);

  lerp_control_params(control_params_low, control_params_high, result, factor, total_coefs_single_config);
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

  double control_params_low[total_coefs_single_config];
  control_params_single_wavelength(
    state,
    elevation,
    altitude,
    turbidity,
    albedo,
    wavelength_low,
    control_params_low);

  double control_params_high[total_coefs_single_config];
  control_params_single_wavelength(
    state,
    elevation,
    altitude,
    turbidity,
    albedo,
    wavelength_low+1,
    control_params_high);

  lerp_control_params(control_params_low, control_params_high, result, factor, total_coefs_single_config);
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
    
  double control_params_interpolated[total_coefs_single_config];
  control_params(
    state,
    elevation_control,
    altitude_control,
    turbidity,
    albedo,
    channel,
    control_params_interpolated);
  
  // coefs->value comes here

  const double background_val = eval_pp(theta, background_nbreaks, background_breaks, control_params_interpolated + background_offset);
  const double solar_max_val = eval_pp(gamma, solar_max_nbreaks, solar_max_breaks, control_params_interpolated + solar_max_offset);
  const double solar_ratio_val = eval_pp(theta, solar_ratio_nbreaks, solar_ratio_breaks, control_params_interpolated + solar_ratio_offset);
  const double backglow_vertical_val = eval_pp(zero, backglow_vertical_nbreaks, backglow_vertical_breaks, control_params_interpolated + backglow_vertical_offset);
  const double backglow_ratio_val = eval_pp(theta, backglow_ratio_nbreaks, backglow_ratio_breaks, control_params_interpolated + backglow_ratio_offset);
  const double frontglow_vertical_val = eval_pp(zero, frontglow_vertical_nbreaks, frontglow_vertical_breaks, control_params_interpolated + frontglow_vertical_offset);
  const double frontglow_ratio_val = eval_pp(theta, frontglow_ratio_nbreaks, frontglow_ratio_breaks, control_params_interpolated + frontglow_ratio_offset);
  const double shadow_val = eval_pp(shadow, shadow_nbreaks, shadow_breaks, control_params_interpolated + shadow_offset);
  const double shadow_v_val = eval_pp(zero, shadow_v_nbreaks, shadow_v_breaks, control_params_interpolated + shadow_v_offset);
  const double shadow_h_val = eval_pp(theta, shadow_h_nbreaks, shadow_h_breaks, control_params_interpolated + shadow_h_offset);

  const double res = 
    background_val
    + solar_max_val * solar_ratio_val
    + backglow_vertical_val * backglow_ratio_val
    + frontglow_vertical_val * frontglow_ratio_val
    - shadow_val * shadow_v_val * shadow_h_val;

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

const double onion_layers[] = {0.041152, 1.3169, 10, 42.1399, 128.6008, 320, 691.6461, 1348.4774, 2430, 4115.2263, 6627.6132, 10240, 15279.5473, 22132.6749, 31250, 43151.2757, 58430.3292, 77760, 101897.0782, 131687.2428, 168070, 212083.6214, 264870.0823, 327680, 401877.572, 488945.5144, 590490, 708245.5967, 844080.2058, 1000000, 1600000};

double arpragueskymodel_computeAngleOnOnionLayer(
        const double                    onion_layer_distance,
        const double                    altitude,
        const double                    altitude_hitpoint,
        double                          *onion_layer_altitude,
        double                          *distance_earth_mover
        )
{
    if (altitude_hitpoint > (altitude + onion_layer_distance))
    {
        *onion_layer_altitude = onion_layer_distance;
        *distance_earth_mover = 0;
        return 0;
    }
    if (altitude_hitpoint < (altitude - onion_layer_distance))
    {
        *onion_layer_altitude = -onion_layer_distance;
        *distance_earth_mover = 0;
        return MATH_PI;
    }
    *onion_layer_altitude = altitude_hitpoint;
    double Y = planet_radius + altitude;
    double A = planet_radius + altitude_hitpoint;
    Y = ((Y * Y) + (A * A) - (onion_layer_distance * onion_layer_distance)) / (2.0 * Y);
    double yn = Y / (planet_radius + altitude_hitpoint);
    Y = Y - (planet_radius + altitude);
    Y = Y / onion_layer_distance;
    double theta = acos(Y);
    *distance_earth_mover = acos(yn) * planet_radius;
    return theta;
}

void arpragueskymodel_computeAnglesAndWeights(
        const double                    altitude,
        const double                    distance,
        const double                    theta,
        const int                       onion_layer_upperindex,
        const int                       onion_layer_lowerindex,
        double                          *theta_upper,
        double                          *theta_lower,
        double                          *weight_upper,
        double                          *weight_lower
        )
{
    if (onion_layer_upperindex == onion_layer_lowerindex)
    {
        *theta_upper = theta;
        *theta_lower = theta;
        *weight_upper = 0.5;
        *weight_lower = 0.5;
        return;
    }
    // Compute Altitude
    //double ap = calcAltitude(altitude, distance, theta);
    double x;
    double y;
    x = sin(theta) * distance;
    y = (cos(theta) * distance) + planet_radius + altitude;
    double N = sqrt((x * x) + (y * y));
    // Altitude of hit point
    double ap = N - planet_radius;
    // Clamp to planet surface
    ap = ap < 0 ? 0 : ap;
    // Compute earth mover distance from camera
    double thetaap = asin(x / (planet_radius + ap)) * planet_radius;
    // Compute upper onion layer parameters
    double au;
    double emu;
    *theta_upper = arpragueskymodel_computeAngleOnOnionLayer(onion_layers[onion_layer_upperindex], altitude, ap, &au, &emu);
    // Compute lower onion layer parameters
    double al;
    double eml;
    *theta_lower = arpragueskymodel_computeAngleOnOnionLayer(onion_layers[onion_layer_lowerindex], altitude, ap, &al, &eml);
    // Calculate weights
    double dau = fabs(au - ap);
    double dal = fabs(al - ap);
    double dtu = fabs(emu - thetaap);
    double dtl = fabs(eml - thetaap);
    double wu1 = sqrt((dau * dau) + (dtu * dtu));
    double wl1 = sqrt((dal * dal) + (dtl * dtl));
    *weight_upper = wl1 / (wu1 + wl1);
    *weight_lower = 1.0 - *weight_upper;
}


double arpragueskymodel_transmittanceAngle(
        const int                       index,
        const double                    theta,
        const float *                   table
        )
{
    int itheta = (int)floor((theta / MATH_PI) * (double)transmittance_angles);
    int ithetainc = itheta < (transmittance_angles - 1) ? 1 : 0;

    double t1 = table[index + itheta];

    double t2 = table[index + itheta + ithetainc];

    double w = ((theta / MATH_PI) * (double)transmittance_angles) - (double)itheta;
    return ((t1 * (1.0 - w)) + (t2 * w));
}

double arpragueskymodel_calcOnionLayers(
        const int                       index,
        const double                    altitude,
        const double                    distance,
        const double                    theta,
        const int                       onion_layer_indexupper,
        const int                       onion_layer_indexlower,
        const float                     *table
        )
{
    double theta_upper;
    double theta_lower;
    double weightupper;
    double weightlower;
    // Compute onion layer interpolation
    //arhosekskymodel_transmittance_calcInterpolationValues(distance, onion_layers, NUM_COMPUTED_ONIONLAYERS, &indexupper, &indexlower, &weightupper, &weightlower);

    arpragueskymodel_computeAnglesAndWeights(altitude, distance, theta, onion_layer_indexupper, onion_layer_indexlower, &theta_upper, &theta_lower, &weightupper, &weightlower);

    double tl = arpragueskymodel_transmittanceAngle(index + (onion_layer_indexlower * transmittance_angles), theta_lower, table);

    double tu = arpragueskymodel_transmittanceAngle(index + (onion_layer_indexupper * transmittance_angles), theta_upper, table);

    // Interpolate in log domain. Epsilon is added to prevent log(0)
    double tauu = -log(tu + 0.0000000001);
    double taul = -log(tl + 0.0000000001);
    double t = (tauu * weightupper) + (taul * weightlower);
    return t;
}

const double computed_altitudes[] = {0.000, 1.875, 15.000, 50.625, 120.000, 234.375, 405.000, 643.125, 960.000, 1366.875, 1875.000, 2495.625, 3240.000, 4119.375, 5145.000, 6328.125, 7680.000, 9211.876, 10934.999, 12860.624, 15000.000};

double arpragueskymodel_calcAltitude(
        const int                   index,
        const double                distance,
        const double                theta,
        const float *               table,
        const int                   altitude_indexupper,
        const int                   altitude_indexlower,
        const float                 altitude_weight,
        const int                   onion_layer_indexupper,
        const int                   onion_layer_indexlower
        )
{
    double weightupper;
    double weightlower;
    double altitude_u = computed_altitudes[altitude_indexupper];
    double altitude_l = computed_altitudes[altitude_indexlower];
    double t1 = arpragueskymodel_calcOnionLayers(index + (altitude_indexupper * transmittance_onionlayers * transmittance_angles), altitude_u, distance, theta, onion_layer_indexupper, onion_layer_indexlower, table) * (1 - altitude_weight);
    double t2 = arpragueskymodel_calcOnionLayers(index + (altitude_indexlower * transmittance_onionlayers * transmittance_angles), altitude_l, distance, theta, onion_layer_indexupper, onion_layer_indexlower, table) *  altitude_weight;
    return (t1 + t2);
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
    
    int onion_layer_lower;
    int onion_layer_inc;
    if (distance > 1000000)
    {
        onion_layer_lower = 30;
        onion_layer_inc = 0;
    } else
    {
        double onion_layer_norm = (pow(distance / 1000000, 0.2) * (transmittance_onionlayers - 1)) - 1;
        onion_layer_lower = (int)onion_layer_norm;
        onion_layer_inc = onion_layer_lower < (transmittance_onionlayers - 1) ? 1 : 0;
    }
    
    int index = turbidity_low * 11 * altitudes * transmittance_onionlayers * transmittance_angles;
    int indexwl = index + (wavelength_low * altitudes * transmittance_onionlayers * transmittance_angles);
    
    double t1 = arpragueskymodel_calcAltitude(indexwl, distance, theta, state->transmission_dataset, altitude_low, altitude_low + altitude_inc, altitude_factor, onion_layer_lower + onion_layer_inc, onion_layer_lower);
    
    indexwl = index + ((wavelength_low + wavelength_inc) * altitudes * transmittance_onionlayers * transmittance_angles);
    
    double t2 = arpragueskymodel_calcAltitude(indexwl, distance, theta, state->transmission_dataset, altitude_low, altitude_low + altitude_inc, altitude_factor, onion_layer_lower + onion_layer_inc, onion_layer_lower);
    
    double tt1 = (t1 * (1.0f - wavelength_factor)) + (t2 * wavelength_factor);
    
    index = (turbidity_low + turbidity_inc) * 11 * altitudes * transmittance_onionlayers * transmittance_angles;
    
    indexwl = index + (wavelength_low * altitudes * transmittance_onionlayers * transmittance_angles);
    
    t1 = arpragueskymodel_calcAltitude(indexwl, distance, theta, state->transmission_dataset, altitude_low, altitude_low + altitude_inc, altitude_factor, onion_layer_lower + onion_layer_inc, onion_layer_lower);
    
    indexwl = index + ((wavelength_low + wavelength_inc) * altitudes * transmittance_onionlayers * transmittance_angles);
    
    t2 = arpragueskymodel_calcAltitude(indexwl, distance, theta, state->transmission_dataset, altitude_low, altitude_low + altitude_inc, altitude_factor, onion_layer_lower + onion_layer_inc, onion_layer_lower);
    
    double tt2 = (t1 * (1.0f - wavelength_factor)) + (t2 * wavelength_factor);
    
    double t = (tt1 * (1.0f - turbidity_factor)) + (tt2 * turbidity_factor);

    return exp(-t);
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

#endif // ARPRAGUESKYMODEL_USE_NEW