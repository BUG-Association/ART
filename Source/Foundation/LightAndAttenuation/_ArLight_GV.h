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

#ifndef _ART_FOUNDATION_LIGHTANDATTENUATION_ARLIGHT_GV_H_
#define _ART_FOUNDATION_LIGHTANDATTENUATION_ARLIGHT_GV_H_

#include "ArLight.h"

#define ARLIGHT_GV  art_gv->arlight_gv

#define ARLIGHT_LCT_HAS_BEEN_INITIALISED_GV \
    ARLIGHT_GV->lct_has_been_initialised
#define ARLIGHT_UNPOLARISED_UNIT_GV \
    ARLIGHT_GV->light_unpolarised_unit
#define ARLIGHT_NONE_GV \
    ARLIGHT_GV->light_none

typedef struct ArLight_GV
{
    unsigned int  lct_has_been_initialised;

    struct ArLight  * light_unpolarised_unit;
    struct ArLight  * light_none;

    const char * (* _alf_lct_string)
    ( const ART_GV * );

    void * (* _alf_alloc)
    ( const ART_GV * );

    void (* _alf_free)
    ( const ART_GV *, void * );

    void * (* _alf_d_alloc_init_unpolarised)
    ( const ART_GV *, const double );

    void * (* _alf_l_alloc_init)
    ( const ART_GV *, const void * );

    const void * (* _alf_light_unpolarised_unit)
    ( const ART_GV * );

    const void * (* _alf_light_none)
    ( const ART_GV * );

    void (* _alf_d_init_unpolarised_l)
    ( const ART_GV *, const double, void * );

    void (* _alf_s_init_unpolarised_l)
    ( const ART_GV *, const ArSpectrum *, void * );

    void (* _alf_s_rf_init_polarised_l)
    ( const ART_GV *, const ArStokesVector *, const ArReferenceFrame *, void * );

    void (* _alf_l_init_s)
    ( const ART_GV *, const void *, ArSpectrum  * );

    void (* _alf_l_init_i)
    ( const ART_GV *, const void *, ArLightIntensity  * );

    void (* _alf_l_init_l)
    ( const ART_GV *, const void *, void * );

    unsigned int (* _alf_l_polarised)
    ( const ART_GV *, const void * );

    ArReferenceFrame const * (* _alf_l_refframe)
    ( const ART_GV *, const void * );

    void (* _alf_l_to_sv)
    ( const ART_GV *, const void *, ArStokesVector * );

    void (* _alf_ld_realign_to_coaxial_refframe_sv)
    ( const ART_GV *, const void *, const double,
      const ArReferenceFrame *, ArStokesVector * );

    void (* _alf_realign_to_coaxial_refframe_l)
    ( const ART_GV *, const ArReferenceFrame *, void * );

    void (* _alf_l_realign_to_coaxial_refframe_l)
    ( const ART_GV *, const void * , const ArReferenceFrame *, void * );

    unsigned int (* _alf_ll_equal)
    ( const ART_GV *, const void *, const void * );

    unsigned int (* _alf_lld_equal)
    ( const ART_GV *, const void *, const void *, const double );

    double (* _alf_l_norm)
    ( const ART_GV *, const void * );

    double (* _alf_l_norm_visible_range)
    ( const ART_GV *, const void * );

    void (* _alf_l_inv_l)
    ( const ART_GV *, const void *, void * );

    double (* _alf_l_max)
    ( const ART_GV *, const void * );

    void (* _alf_ll_max_l)
    ( const ART_GV *, const void *, const void *, void * );

    double (* _alf_l_min)
    ( const ART_GV *, const void * );

    void (* _alf_ll_min_l)
    ( const ART_GV *, const void *, const void *, void * );

    double (* _alf_ll_maxdiff)
    ( const ART_GV *, const void *, const void * );

    void (* _alf_i_add_l)
    ( const ART_GV *, const ArLightIntensity *, void * );

    void (* _alf_l_add_l)
    ( const ART_GV *, const void *, void * );

    void (* _alf_dl_sloppy_add_l)
    ( const ART_GV *, const double, const void *, void * );

    void (* _alf_wsd_sloppy_add_l)
    ( const ART_GV *, const void *, const void *, const void *, const double, void * );

    void (* _alf_l_atomic_add_l)
    ( const ART_GV *, const void *, void * );

    void (* _alf_ll_add_l)
    ( const ART_GV *, const void *, const void *, void * );

    void (* _alf_lld_sloppy_add_l)
    ( const ART_GV *, const void *, const void *, const double, void * );

    void (* _alf_ll_sub_l)
    ( const ART_GV *, const void *, const void *, void * );

    void (* _alf_d_mul_l)
    ( const ART_GV *, const double, void * );

    void (* _alf_dl_mul_l)
    ( const ART_GV *, const double, const void *, void * );

    void (* _alf_i_mul_l)
    ( const ART_GV *, const ArLightIntensity *, void * );

    void (* _alf_l_mul_l)
    ( const ART_GV *, const void *, void * );

    void (* _alf_ll_mul_l)
    ( const ART_GV *, const void *, const void *, void * );

    void (* _alf_d_div_l)
    ( const ART_GV *, const double, void * );

    void (* _alf_dl_div_l)
    ( const ART_GV *, const double, const void *, void * );

    void (* _alf_dl_pow_l)
    ( const ART_GV *, const double, const void *, void * );

    void (* _alf_ddl_clamp_l)
    ( const ART_GV *, const double, const double, const void *, void * );

    void (* _alf_dl_mul_add_l)
    ( const ART_GV *, const double, const void *, void * );

    void (* _alf_dld_mul_sloppy_add_l)
    ( const ART_GV *, const double, const void *, const double, void * );

    void (* _alf_dwsd_mul_sloppy_add_l)
    ( const ART_GV *, const double, const void *, const void *, const void *, const double, void * );

    void (* _alf_dll_interpol_l)
    ( const ART_GV *, const double, const void *, const void *, void * );

    unsigned int (* _alf_l_valid)
    ( const ART_GV *, const void * );

    unsigned int (* _alf_ll_collinear)
    ( const ART_GV *, const void *, const void * );

    void (* _alf_l_debugprintf)
    ( const ART_GV *, const void *);
}
ArLight_GV;

#endif /* _ART_FOUNDATION_LIGHTANDATTENUATION_ARLIGHT_GV_H_ */
/* ======================================================================== */
