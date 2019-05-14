// Copyright 2005 Mitsubishi Electric Research Laboratories All Rights Reserved.

// Permission to use, copy and modify this software and its documentation without
// fee for educational, research and non-profit purposes, is hereby granted, provided
// that the above copyright notice and the following three paragraphs appear in all copies.

// To request permission to incorporate this software into commercial products contact:
// Vice President of Marketing and Business Development;
// Mitsubishi Electric Research Laboratories (MERL), 201 Broadway, Cambridge, MA 02139 or 
// <license@merl.com>.

// IN NO EVENT SHALL MERL BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL,
// OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE AND
// ITS DOCUMENTATION, EVEN IF MERL HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

// MERL SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED
// HEREUNDER IS ON AN "AS IS" BASIS, AND MERL HAS NO OBLIGATIONS TO PROVIDE MAINTENANCE, SUPPORT,
// UPDATES, ENHANCEMENTS OR MODIFICATIONS.


#include "merl.h"
#include <math.h>

#define RED_SCALE (1.0/1500.0)
#define GREEN_SCALE (1.15/1500.0)
#define BLUE_SCALE (1.66/1500.0)
#ifndef M_PI
#define M_PI	3.1415926535897932384626433832795
#endif


// Lookup theta_half index
// This is a non-linear mapping!
// In:  [0 .. pi/2]
// Out: [0 .. 89]
inline int theta_half_index(double theta_half)
{
	if (theta_half <= 0.0)
		return 0;
	double theta_half_deg = ((theta_half / (M_PI/2.0))*MERL_SAMPLING_RES_THETA_H);
	double temp = theta_half_deg*MERL_SAMPLING_RES_THETA_H;
	temp = sqrt(temp);
	int ret_val = (int)temp;
	if (ret_val < 0) ret_val = 0;
	if (ret_val >= MERL_SAMPLING_RES_THETA_H)
		ret_val = MERL_SAMPLING_RES_THETA_H-1;
	return ret_val;
}


// Lookup theta_diff index
// In:  [0 .. pi/2]
// Out: [0 .. 89]
inline int theta_diff_index(double theta_diff)
{
	int tmp = (int)(theta_diff / (M_PI * 0.5) * MERL_SAMPLING_RES_THETA_D);
	if (tmp < 0)
		return 0;
	else if (tmp < MERL_SAMPLING_RES_THETA_D - 1)
		return tmp;
	else
		return MERL_SAMPLING_RES_THETA_D - 1;
}

// Lookup phi_diff index
inline int phi_diff_index(double phi_diff)
{
	// Because of reciprocity, the BRDF is unchanged under
	// phi_diff -> phi_diff + M_PI
	if (phi_diff < 0.0)
		phi_diff += M_PI;

	// In: phi_diff in [0 .. pi]
	// Out: tmp in [0 .. 179]
	int tmp = (int)(phi_diff / M_PI * MERL_SAMPLING_RES_PHI_D);
	if (tmp < 0)	
		return 0;
	else if (tmp < MERL_SAMPLING_RES_PHI_D - 1)
		return tmp;
	else
		return MERL_SAMPLING_RES_PHI_D - 1;
}

void vec3d_vd_rot_v(
    const Vec3D  *vector,
	const Vec3D  *axis,
	      double  angle,
	      Vec3D  *out
	)
{
	double cos_ang = cos(angle);
	double sin_ang = sin(angle);

	vec3d_dv_mul_v(cos_ang, vector, out);
	double tmp = vec3d_vv_dot(axis, vector) * (1.0 - cos_ang);

	// out = out + tmp * axis
	vec3d_vvd_mul_add_v(out, axis, tmp, out);

	Vec3D cross;
	vec3d_vv_cross_v(axis, vector, &cross);

	// out = out + sin_ang * cross
	vec3d_vvd_mul_add_v(out, &cross, sin_ang, out);
}

// Given a pair of incoming/outgoing angles, look up the BRDF.
void lookup_brdf_val(
    const double * brdf,
    const Vec3D  * localI,
	const Vec3D  * localO,
	      double * red_val,
	      double * green_val,
	      double * blue_val
    )
{
	// Get half vector
	Vec3D localH;
	vec3d_vv_add_v(localI, localO, &localH);
	vec3d_norm_v(&localH);

	const double theta_h = acos(ZC(localH));
	const double phi_h = atan2(YC(localH), XC(localH));

	// Get Diff vector
	Vec3D normal = VEC3D_Z_UNIT;
	Vec3D binormal = VEC3D_Y_UNIT;
	Vec3D diff, temp;
	
	vec3d_vd_rot_v(localI, &normal, -phi_h, &temp);
	vec3d_vd_rot_v(&temp, &binormal, -theta_h, &diff);

	// Convert to halfangle / difference angle coordinates
	const double theta_d = acos(ZC(diff));
	const double phi_d = atan2(YC(diff), XC(diff));

	// Find index.
	// Note that phi_half is ignored, since isotropic BRDFs are assumed
	const int ind = phi_diff_index(phi_d) +
		MERL_SAMPLING_RES_PHI_D * (
		  theta_diff_index(theta_d) +
		  theta_half_index(theta_h) * MERL_SAMPLING_RES_THETA_D
		);

	const int offset = MERL_SAMPLING_RES_THETA_H*MERL_SAMPLING_RES_THETA_D*MERL_SAMPLING_RES_PHI_D;

	*red_val = brdf[ind] * RED_SCALE;
	*green_val = brdf[ind + offset] * GREEN_SCALE;
	*blue_val = brdf[ind + 2*offset] * BLUE_SCALE;
}
