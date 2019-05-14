#ifndef MERL_H_
#define MERL_H_

#include <Vec3D.h>

#define MERL_SAMPLING_RES_THETA_H 90
#define MERL_SAMPLING_RES_THETA_D 90
#define MERL_SAMPLING_RES_PHI_D   180

// Given a pair of incoming/outgoing angles, look up the BRDF.
void lookup_brdf_val(
    const double * brdf,
    const Vec3D  * localI,
	const Vec3D  * localO,
	      double * red_val,
	      double * green_val,
	      double * blue_val
	);

#endif // MERL_H_
