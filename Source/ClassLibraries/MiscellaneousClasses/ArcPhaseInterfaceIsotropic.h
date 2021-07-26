/* ===========================================================================

    Copyright (c) 1996-2021 The ART Development Team
    -------------------------------------------

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

#ifndef _ARCPHASEINTERFACEISOTROPIC_H_
#define _ARCPHASEINTERFACEISOTROPIC_H_

#include "ART_Foundation.h"
#import "ArpNode.h"
#import "ArpSpectrum.h"
#import "ArpVolumeMaterial.h"

ART_MODULE_INTERFACE(ArcPhaseInterfaceIsotropic)

@class ArNode;

/* ---------------------------------------------------------------------------

     ArcPhaseInterfaceIsotropicType enum
     ---------------------

     Describes the properties of an ArcPhaseInterfaceIsotropic.

     - "simple": none of the special properties applies to this refraction
     - "variable": not used at the moment; should be used for materials
        that have a varying index of refraction (e.g. hot air)
     - "spectral": the index or refraction (IOR) varies with wavelength and
        ought to be evaluated accordingly
     - "complex": a nonzero extinction value is present, i.e. the material
        we are looking at is a conductor and hence opaque

 ------------------------------------------------------------------------aw- */

typedef enum ArcPhaseInterfaceType
{
    arphaseinterface_simple    = 0x0,
    arphaseinterface_variable  = 0x1,
    arphaseinterface_spectral  = 0x2,
    arphaseinterface_complex   = 0x4
}
ArcPhaseInterfaceType;

/* ---------------------------------------------------------------------------

     accessor macros for ArcPhaseInterfaceIsotropic

 ------------------------------------------------------------------------aw- */

#define ARCPHASEINTERFACE_EXACT_IOR(__rf)               (__rf).exactIOR
#define ARCPHASEINTERFACE_EXACT_INV_IOR(__rf)           (__rf).exactInverseIOR
#define ARCPHASEINTERFACE_HIRES_IOR(__rf)               (__rf).hiresIOR
#define ARCPHASEINTERFACE_HIRES_INV_IOR(__rf)           (__rf).hiresInverseIOR
#define ARCPHASEINTERFACE_AVG_IOR(__rf)                 (__rf).avgIOR
#define ARCPHASEINTERFACE_AVG_INV_IOR(__rf)             (__rf).avgInverseIOR
#define ARCPHASEINTERFACE_AVG_IOR_I(__rf)               (__rf).avgIORI
#define ARCPHASEINTERFACE_AVG_IOR_T(__rf)               (__rf).avgIORT
#define ARCPHASEINTERFACE_EXACT_EXTINCTION_FROM(__rf)   (__rf).exactExtinctionCoefficientFrom
#define ARCPHASEINTERFACE_EXACT_EXTINCTION_INTO(__rf)   (__rf).exactExtinctionCoefficientInto
#define ARCPHASEINTERFACE_HIRES_EXTINCTION_FROM(__rf)   (__rf).hiresExtinctionCoefficientFrom
#define ARCPHASEINTERFACE_HIRES_EXTINCTION_INTO(__rf)   (__rf).hiresExtinctionCoefficientInto
#define ARCPHASEINTERFACE_AVG_EXT_FROM(__rf)            (__rf).avgExtinctionCoefficientFrom
#define ARCPHASEINTERFACE_AVG_EXT_INTO(__rf)            (__rf).avgExtinctionCoefficientInto
#define ARCPHASEINTERFACE_TYPE(__rf)                    (__rf).type

#define ARCPHASEINTERFACE_IOR_AT_WAVELENGTH(__pi,__wl) \
    s500_sd_value_at_wavelength( \
        art_gv, \
        ARCPHASEINTERFACE_HIRES_IOR(__pi), \
        (__wl) \
        )
#define ARCPHASEINTERFACE_IOR_INIT_S(__pi,__wl,__sr) \
    sps_s500w_init_s( \
        art_gv, \
        ARCPHASEINTERFACE_HIRES_IOR(__pi), \
        (__wl), \
        (__sr) \
        )

#define ARCPHASEINTERFACE_INV_IOR_AT_WAVELENGTH(__pi,__wl) \
    s500_sd_value_at_wavelength( \
        art_gv, \
        ARCPHASEINTERFACE_HIRES_INV_IOR(__pi), \
        (__wl) \
        )
#define ARCPHASEINTERFACE_INV_IOR_INIT_S(__pi,__wl,__sr) \
    sps_s500w_init_s( \
        art_gv, \
        ARCPHASEINTERFACE_HIRES_INV_IOR(__pi), \
        (__wl), \
        (__sr) \
        )

#define ARCPHASEINTERFACE_EXTINCTION_INTO_AT_WAVELENGTH(__pi,__wl) \
    s500_sd_value_at_wavelength( \
        art_gv, \
        ARCPHASEINTERFACE_HIRES_EXTINCTION_INTO(__pi), \
        (__wl) \
        )
#define ARCPHASEINTERFACE_EXTINCTION_INTO_INIT_S(__pi,__wl,__sr) \
    sps_s500w_init_s( \
        art_gv, \
        ARCPHASEINTERFACE_HIRES_EXTINCTION_INTO(__pi), \
        (__wl), \
        (__sr) \
        )

#define ARCPHASEINTERFACE_EXTINCTION_FROM_AT_WAVELENGTH(__pi,__wl) \
    s500_sd_value_at_wavelength( \
        art_gv, \
        ARCPHASEINTERFACE_HIRES_EXTINCTION_FROM(__pi), \
        (__wl) \
        )
#define ARCPHASEINTERFACE_EXTINCTION_FROM_INIT_S(__pi,__wl,__sr) \
    sps_s500w_init_s( \
        art_gv, \
        ARCPHASEINTERFACE_HIRES_EXTINCTION_FROM(__pi), \
        (__wl), \
        (__sr) \
        )


#define ARCPHASEINTERFACE_HAS_COMPLEX_IOR(__rf) \
    (ARCPHASEINTERFACE_TYPE(__rf) & arphaseinterface_complex)
#define ARCPHASEINTERFACE_HAS_REALVALUED_IOR(__rf) \
    (!(ARCPHASEINTERFACE_TYPE(__rf) & arphaseinterface_complex))
#define ARCPHASEINTERFACE_HAS_IOR_FOR_EACH_WAVELENGTH(__rf) \
    (ARCPHASEINTERFACE_TYPE(__rf) & arphaseinterface_spectral)

#define ARCPHASEINTERFACE_IS_ISOTROPIC(__rf) \
    (ARCPHASEINTERFACE_MATERIALCLASS(__rf) & arphaseinterface_isotropic)
#define ARCPHASEINTERFACE_IS_UNIAXIAL(__rf) \
    (ARCPHASEINTERFACE_MATERIALCLASS(__rf) & arphaseinterface_uniaxial)
#define ARCPHASEINTERFACE_IS_BIAXIAL(__rf) \
    (ARCPHASEINTERFACE_MATERIALCLASS(__rf) & arphaseinterface_biaxial)


/* ---------------------------------------------------------------------------

     ArcPhaseInterfaceIsotropic class
     -------------------

     This class is used to store precomputed values for the index of
     refraction at a particular phase interface.

     The idea behind this is that various algorithms need potentially costly
     evaluations and/or variations of the IOR (such as its inverse), and
     that - even for large numbers of different objects - there usually are
     not very many distinct combinations of material densities. For example,
     even though there may be thousands of glass spheres in a scene the IOR
     for the glass-air interface is the same for all of them and can be
     precomputed and re-used.

     The main components of this class are:

     - the average IOR and its inverse as a single double value each
     - the exact IOR (for each wavelength) and its inverse
     - the extinction and its inverse as a single colour value each (note
       that the extinction coefficient is the complex part of the IOR)
     - a type field to speed up processing of refraction structs by certain
       key characteristics (such as whether the IOR varies with wavelength -
       see above for a list)

 ------------------------------------------------------------------------aw- */

@interface ArcPhaseInterfaceIsotropic
    : ArcObject
{

@public

    ArSpectrum    * exactIOR;
    ArSpectrum    * exactInverseIOR;
    ArSpectrum500 * hiresIOR;
    ArSpectrum500 * hiresInverseIOR;
    double          avgIOR;
    double          avgInverseIOR;
    double          avgIORI;
    double          avgIORT;
    ArSpectrum    * exactExtinctionCoefficientFrom;
    ArSpectrum    * exactExtinctionCoefficientInto;
    ArSpectrum500 * hiresExtinctionCoefficientFrom;
    ArSpectrum500 * hiresExtinctionCoefficientInto;
    double          avgExtinctionCoefficientFrom;
    double          avgExtinctionCoefficientInto;
    unsigned int    type;
    //   These pointers will eventually be removed, as their presence violates
    //   ObjC design rules
    id mFrom;
    id mInto;

}


- (id) init
        : (ArNode <ArpVolumeMaterial> *) fromMaterial
        : (ArNode <ArpVolumeMaterial> *) intoMaterial
        ;

@end

#endif // _ARCPHASEINTERFACEISOTROPIC_H_

// ===========================================================================
