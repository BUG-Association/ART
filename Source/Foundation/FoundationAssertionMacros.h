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

#ifndef _ART_FOUNDATION_ASSERTION_MACROS_H_
#define _ART_FOUNDATION_ASSERTION_MACROS_H_

//#define WITH_RSA_STATISTICS

/* ---------------------------------------------------------------------------

    Light and Attenuation assertion macros

        By #defining the FOUNDATION_ASSERTIONS flag, you can switch on
        assertions and checks in the ART Foundation library. The validity
        of many input and output parameters is checked (currently, mainly in
        the colour, attenuation and light subsystems), and if the renderer is
        in polarisation mode, the alignment of the coordinate systems
        associated with each light and attenuation operand is also verified.

        CAVEAT:
        =======

        Be aware that switching these mechanisms on incurs a considerable
        performance hit, so this should never be done, except during
        debugging runs!

------------------------------------------------------------------------aw- */


//#define FOUNDATION_ASSERTIONS


//   The functionality of each assertion is briefly explained in the section
//   with the dummy definitions that are normally used, if the flag is not set.
//   The actual assertion macros are further down, after the #else of the
//   #ifndef.

#ifndef FOUNDATION_ASSERTIONS  //   dummy definitions follow

//   Assertions for numbers

//   valid x          not a NaN or similar denormalised number
//   nonzero x        (x < 0) || (x > 0)
//   nonnegative x    x >= 0
//   positive x       x > 0
//   unit range x     0 <= x <= 1
//   range (l, h)     l <= x <= h
//   random           0 <= x <  1

//   Only for float data types: the additional qualifier 'finite' tests whether
//   the number in question is != inf. This is a separate, explicit check since
//   IEEE float arithmetic mostly behaves as expected in the presence of
//   infinities, so their occurrence is not necessarily a pathological case
//   that has to be caught via an exception.

#define ASSERT_VALID_DOUBLE(__d)
#define ASSERT_VALID_FINITE_DOUBLE(__d)
#define ASSERT_POSITIVE_DOUBLE(__d)
#define ASSERT_POSITIVE_FINITE_DOUBLE(__d)
#define ASSERT_NONNEGATIVE_DOUBLE(__d)
#define ASSERT_DOUBLE_EQUAL_TO(__d1, __d2, __radius)
#define ASSERT_DOUBLE_LARGER_THAN_OR_EQUAL_TO(__d, __l)
#define ASSERT_DOUBLE_LARGER_THAN(__d, __l)
#define ASSERT_DOUBLE_LESS_THAN_OR_EQUAL_TO(__d, __h)
#define ASSERT_DOUBLE_LESS_THAN(__d, __h)
#define ASSERT_NONNEGATIVE_FINITE_DOUBLE(__d)
#define ASSERT_NONZERO_DOUBLE(__d)
#define ASSERT_NONZERO_FINITE_DOUBLE(__d)
#define ASSERT_UNIT_RANGE_DOUBLE(__d)
#define ASSERT_DOUBLE_WITHIN_RANGE(__d,__l,__h)
#define ASSERT_RANDOM_DOUBLE(__d)

#define ASSERT_POSITIVE_INTEGER(__i)
#define ASSERT_NONNEGATIVE_INTEGER(__i)
#define ASSERT_INTEGER_WITHIN_RANGE(__i,__l,__h)
#define ASSERT_INTEGER_IS_EQUAL_TO(__i,__v)

//   Point and vector validity and equivalence assertions

//   valid point:  all components are valid doubles (see above)
//   valid vector: all components are valid doubles (see above),
//                 and the vector is also not (0,0,0)
//   valid ray:    both point and vector are valid
//   valid normalised ray: same, with normalised vector

//   allocated = currently check if nonzero pointer, should be checked
//               against a list of allocated instances

#define ASSERT_ALLOCATED_PNT3D(__r0)
#define ASSERT_ALLOCATED_VEC3D(__r0)
#define ASSERT_ALLOCATED_RAY3D(__r0)

#define ASSERT_VALID_PNT3D(__p0)
#define ASSERT_VALID_VEC3D(__v0)
#define ASSERT_VALID_RAY3D(__r0)

#define ASSERT_VALID_NORMALIZED_VEC3D(__v0)
#define ASSERT_VALID_NORMALIZED_RAY3D(__r0)

#define ASSERT_IDENTICAL_VEC3D(__v0,__v1)

//   The following macro has to be placed in the declaration of all
//   ISR structures, so that the assertion mechanisms work for
//   polarised light. In particular, each ISR instance has to be able to
//   "remember" whether it should tolerate values < 0.0. Normal colours and
//   spectra don't, but Stokes Vector components have to. In normal operation,
//   this flag is not needed, so it is only compiled in if the assertions
//   are active.

#define ISR_ASSERTION_DATA

//   The following macro has to be placed in the allocation function of
//   all ISR structs. It sets the assertion mechanism to the default case,
//   i.e. negative values being invalid.

#define INITIALISE_ISR_ASSERTION_DATA(__c)

//   The following macro sets a particular colour value to allow negative
//   values during assertion checks.

#define COLOUR_ASSERTIONS_ALLOW_NEGATIVE_VALUES(__c)

//   Colour existence and validity assertions

#define ASSERT_ALLOCATED_RGBA(__r)
#define ASSERT_VALID_RGBA(__r)
#define ASSERT_ALLOCATED_SPECTRUMTYPE(__ts,__t,__c)
#define ASSERT_NONZERO_COLOURTYPE(__ts,__t,__c,__s)
#define ASSERT_VALID_SPECTRUMTYPE(__ts,__t,__c,__s)
#define ASSERT_ALLOCATED_SPECTRUM(__c)
#define ASSERT_VALID_SPECTRUM(__c)

//   Spectral data type assertions

#define ASSERT_ALLOCATED_PSS(__c)
#define ASSERT_VALID_PSS(__c)
#define ASSERT_ALLOCATED_RSS(__c)
#define ASSERT_VALID_RSS(__c)
#define ASSERT_ALLOCATED_RSS2D(__c)
#define ASSERT_VALID_RSS2D(__c)

//   Assertions for spectral samples.

#define ASSERT_ALLOCATED_HERO_VALUES(__v)
#define ASSERT_VALID_HERO_VALUES(__v)
#define ASSERT_ALLOCATED_SPECTRAL_HERO_SAMPLE(__s)
#define ASSERT_VALID_SPECTRAL_HERO_SAMPLE(__s)
#define ASSERT_ALLOCATED_SPECTRAL_SAMPLE(__s)
#define ASSERT_VALID_SPECTRAL_SAMPLE(__s)

//   Assertions for wavelength data structures.

#define ASSERT_ALLOCATED_WAVELENGTH(__w)
#define ASSERT_ALLOCATED_HERO_WAVELENGTHS(__w)
#define ASSERT_VALID_WAVELENGTH(__w)
#define ASSERT_VALID_HERO_WAVELENGTHS(__w)

//   Assertions for Stokes Vectors.

#define ASSERT_ALLOCATED_STOKES_VECTOR(__sv)
#define ASSERT_ALLOCATED_STOKES_VECTOR_SAMPLE(__s)
#define ASSERT_ALLOCATED_STOKES_VECTOR_HERO_SAMPLE(__s)
#define ASSERT_VALID_STOKES_VECTOR(__sv)
#define ASSERT_VALID_STOKES_VECTOR_SAMPLE(__s)
#define ASSERT_VALID_STOKES_VECTOR_HERO_SAMPLE(__s)

//   Assertions for Müller matrics.

#define ASSERT_ALLOCATED_MUELLER_MATRIX(__mm)
#define ASSERT_ALLOCATED_MUELLER_MATRIX_SAMPLE(__mm)
#define ASSERT_ALLOCATED_MUELLER_MATRIX_HERO_SAMPLE(__mm)
#define ASSERT_VALID_MUELLER_MATRIX(__mm)
#define ASSERT_VALID_MUELLER_MATRIX_SAMPLE(__mm)
#define ASSERT_VALID_MUELLER_MATRIX_HERO_SAMPLE(__mm)

//   Assertions for polarisation reference frames.

#define ASSERT_ALLOCATED_REFERENCE_FRAME(__rf)
#define ASSERT_VALID_REFERENCE_FRAME(__rf)

//   Internal functions to be used in routines that require storage
//   of reference frames for assertion purposes.

//   Stores the reference frame of the given entity in a temp variable.
//   Only one such storage command may be used in each function!

#define STORE_LIGHT_REFERENCE_FRAME(__l)
#define STORE_LIGHT_SAMPLE_REFERENCE_FRAME(__l)
#define STORE_LIGHT_HERO_SAMPLE_REFERENCE_FRAME(__l)

#define STORE_ATTENUATION_REFERENCE_FRAMES(__f)
#define STORE_ATTENUATION_SAMPLE_REFERENCE_FRAMES(__f)
#define STORE_ATTENUATION_HERO_SAMPLE_REFERENCE_FRAMES(__f)

//   Light existence and validity assertions.

//   Generic ArLight

#define ASSERT_ALLOCATED_LIGHT(__l)
#define ASSERT_ALLOCATED_LIGHT_SAMPLE(__l)
#define ASSERT_ALLOCATED_LIGHT_HERO_SAMPLE(__l)
#define ASSERT_VALID_LIGHT(__l)
#define ASSERT_VALID_LIGHT_SAMPLE(__l)
#define ASSERT_VALID_LIGHT_HERO_SAMPLE(__l)

//   ArSVLight

#define ASSERT_ALLOCATED_SV_LIGHT(__l)
#define ASSERT_ALLOCATED_SV_LIGHT_SAMPLE(__)
#define ASSERT_ALLOCATED_SV_LIGHT_HERO_SAMPLE(__)
#define ASSERT_VALID_SV_LIGHT(__l)
#define ASSERT_VALID_SV_LIGHT_SAMPLE(__l)
#define ASSERT_VALID_SV_LIGHT_HERO_SAMPLE(__l)

//   ArLightAlpha

#define ASSERT_ALLOCATED_LIGHTALPHA(__l)
#define ASSERT_ALLOCATED_LIGHTALPHA_SAMPLE(__l)
#define ASSERT_ALLOCATED_LIGHTALPHA_HERO_SAMPLE(__l)
#define ASSERT_VALID_LIGHTALPHA(__l)
#define ASSERT_VALID_LIGHTALPHA_SAMPLE(__l)
#define ASSERT_VALID_LIGHTALPHA_HERO_SAMPLE(__l)

//   ArLightIntensity

#define ASSERT_ALLOCATED_LIGHT_INTENSITY(__li)
#define ASSERT_ALLOCATED_LIGHT_INTENSITY_SAMPLE(__l)
#define ASSERT_ALLOCATED_LIGHT_INTENSITY_HERO_SAMPLE(__l)
#define ASSERT_VALID_LIGHT_INTENSITY(__li)
#define ASSERT_VALID_LIGHT_INTENSITY_SAMPLE(__l)
#define ASSERT_VALID_LIGHT_INTENSITY_HERO_SAMPLE(__l)

//   Crosstalk existence and validity assertions.

#define ASSERT_ALLOCATED_CROSSTALK(__cx)
#define ASSERT_ALLOCATED_CROSSTALK_SAMPLE(__x)
#define ASSERT_ALLOCATED_CROSSTALK_HERO_SAMPLE(__x)
#define ASSERT_ALLOCATED_CROSSTALK500(__x)

#define ASSERT_VALID_CROSSTALK(__cx)
#define ASSERT_VALID_CROSSTALK_SAMPLE(__x)
#define ASSERT_VALID_CROSSTALK_HERO_SAMPLE(__x)
#define ASSERT_VALID_CROSSTALK500(__cx)

//   Attenuation existence and validity assertions.

#define ASSERT_ALLOCATED_ATTENUATION(__f)
#define ASSERT_ALLOCATED_ATTENUATION_SAMPLE(__f)
#define ASSERT_ALLOCATED_ATTENUATION_HERO_SAMPLE(__f)

#define ASSERT_VALID_ATTENUATION(__f)
#define ASSERT_VALID_ATTENUATION_SAMPLE(__f)
#define ASSERT_VALID_ATTENUATION_HERO_SAMPLE(__f)

//   Direct MM attenuation existence and validity assertions.

#define ASSERT_ALLOCATED_MM_DIRECT_ATTENUATION(__f)
#define ASSERT_ALLOCATED_MM_DIRECT_ATTENUATION_SAMPLE(__a)
#define ASSERT_ALLOCATED_MM_DIRECT_ATTENUATION_HERO_SAMPLE(__a)

#define ASSERT_VALID_MM_DIRECT_ATTENUATION(__f)
#define ASSERT_VALID_MM_DIRECT_ATTENUATION_SAMPLE(__a)
#define ASSERT_VALID_MM_DIRECT_ATTENUATION_HERO_SAMPLE(__a)

//   BSDF sample existence and validity assertions.

#define ASSERT_ALLOCATED_BSDFSAMPLE(__s)
#define ASSERT_ALLOCATED_SINGLE_WAVELENGTH_BSDFSAMPLE(__s)
#define ASSERT_ALLOCATED_HERO_SPECTRAL_SAMPLE_BSDFSAMPLE(__s)
#define ASSERT_VALID_BSDFSAMPLE(__s)
#define ASSERT_VALID_SINGLE_WAVELENGTH_BSDFSAMPLE(__s)
#define ASSERT_VALID_HERO_BSDFSAMPLE(__s)

//   The following assertions check whether the reference frames used
//   for polarised light, and polarising attenuations, are collinear.

//   Basic coaxiality test for directly provided reference frames.

#define ASSERT_COAXIAL_REFERENCE_FRAMES_RR(__r0,__r1,__d0)

//   Asserts whether a directly given reference frame, and light struct
//   are coaxial. The _SVL version operates directly on ArSVLight, instead
//   of ArLight, and is used for internal checks within that module.

#define ASSERT_COAXIAL_REFERENCE_FRAMES_RL(__r0,__l0,__d0)
#define ASSERT_COAXIAL_SAMPLE_REFERENCE_FRAMES_RL(__r0,__l0,__d0)
#define ASSERT_COAXIAL_SAMPLE_REFERENCE_FRAMES_VL(__v0,__l0,__d0)
#define ASSERT_COAXIAL_HERO_SAMPLE_REFERENCE_FRAMES_RL(__r0,__l0,__d0)

#define ASSERT_COAXIAL_REFERENCE_FRAMES_R_SVL(__r0,__l0,__d0)
#define ASSERT_COAXIAL_SAMPLE_REFERENCE_FRAMES_R_SVL(__r0,__l0,__d0)
#define ASSERT_COAXIAL_HERO_SAMPLE_REFERENCE_FRAMES_R_SVL(__r0,__l0,__d0)

//   Asserts whether a directly given reference frame, and the reference
//   frame of a given light struct are identical.

#define ASSERT_IDENTICAL_REFERENCE_FRAMES_RL(__r0,__l0)
#define ASSERT_IDENTICAL_SAMPLE_REFERENCE_FRAMES_RL(__r0,__l0)
#define ASSERT_IDENTICAL_HERO_SAMPLE_REFERENCE_FRAMES_RL(__r0,__l0)

//   Asserts whether the stored reference frame (see above), and the reference
//   frame of a given light struct are identical.

#define ASSERT_IDENTICAL_REFERENCE_FRAMES_SL(__l0)
#define ASSERT_IDENTICAL_SAMPLE_REFERENCE_FRAMES_SL(__l0)
#define ASSERT_IDENTICAL_HERO_SAMPLE_REFERENCE_FRAMES_SL(__l0)

//   Asserts whether two light structs have collinear reference frames.

#define ASSERT_COAXIAL_REFERENCE_FRAMES_LL(__l0,__l1,__d0)
#define ASSERT_COAXIAL_SAMPLE_REFERENCE_FRAMES_LL(__l0,__l1,__d0)
#define ASSERT_COAXIAL_HERO_SAMPLE_REFERENCE_FRAMES_LL(__l0,__l1,__d0)

//   Asserts whether the coordinate system of a reference frame, and the
//   *exit* coordinate system of an attenuation match.

#define ASSERT_COAXIAL_EXIT_REFERENCE_FRAME_RA(__r0,__a0,__d0)
#define ASSERT_COAXIAL_SAMPLE_EXIT_REFERENCE_FRAME_RA(__r0,__a0,__d0)
#define ASSERT_COAXIAL_HERO_SAMPLE_EXIT_REFERENCE_FRAME_RA(__r0,__a0,__d0)

//   Asserts whether the coordinate system of a light struct, and the
//   *entry* coordinate system of an attenuation match.

#define ASSERT_COAXIAL_ENTRY_REFERENCE_FRAME_LA(__l0,__a0,__d0)
#define ASSERT_COAXIAL_SAMPLE_ENTRY_REFERENCE_FRAME_LA(__l0,__a0,__d0)
#define ASSERT_COAXIAL_HERO_SAMPLE_ENTRY_REFERENCE_FRAME_LA(__l0,__a0,__d0)

//   Asserts whether the coordinate system of a light struct, and the
//   *exit* coordinate system of an attenuation match.

#define ASSERT_COAXIAL_EXIT_REFERENCE_FRAME_LA(__l0,__a0,__d0)
#define ASSERT_COAXIAL_SAMPLE_EXIT_REFERENCE_FRAME_LA(__l0,__a0,__d0)
#define ASSERT_COAXIAL_HERO_SAMPLE_EXIT_REFERENCE_FRAME_LA(__l0,__a0,__d0)

//   Asserts whether the coordinate system of a light struct and a given
//   ray match.

#define ASSERT_COAXIAL_REFERENCE_FRAME_L_WITH_EYE_RAY(__l0,__r0,__d0)
#define ASSERT_COAXIAL_SAMPLE_REFERENCE_FRAME_L_WITH_EYE_RAY(__l0,__r0,__d0)
#define ASSERT_COAXIAL_HERO_SAMPLE_REFERENCE_FRAME_L_WITH_EYE_RAY(__l0,__r0,__d0)

//   Asserts whether the entry coordinate systems of two attenuations are
//   coaxial.

#define ASSERT_COAXIAL_ENTRY_REFERENCE_FRAME_AA(__a0,__a1,__d0)
#define ASSERT_COAXIAL_SAMPLE_ENTRY_REFERENCE_FRAME_AA(__a0,__a1,__d0)
#define ASSERT_COAXIAL_HERO_SAMPLE_ENTRY_REFERENCE_FRAME_AA(__a0,__a1,__d0)

//   Asserts whether the entry coordinate systems of two MM direct attenuations
//   are coaxial.

#define ASSERT_COAXIAL_ENTRY_REFERENCE_FRAME_MM_DA_AA(__a0,__a1,__d0)
#define ASSERT_COAXIAL_SAMPLE_ENTRY_REFERENCE_FRAME_MM_DA_AA(__a0,__a1,__d0)
#define ASSERT_COAXIAL_HERO_SAMPLE_ENTRY_REFERENCE_FRAME_MM_DA_AA(__a0,__a1,__d0)

//   Asserts whether the exit coordinate systems of two attenuations are
//   coaxial.

#define ASSERT_COAXIAL_EXIT_REFERENCE_FRAME_AA(__a0,__a1,__d0)
#define ASSERT_COAXIAL_SAMPLE_EXIT_REFERENCE_FRAME_AA(__a0,__a1,__d0)
#define ASSERT_COAXIAL_HERO_SAMPLE_EXIT_REFERENCE_FRAME_AA(__a0,__a1,__d0)

//   Asserts whether the exit coordinate systems of two MM direct attenuations
//   are coaxial.

#define ASSERT_COAXIAL_EXIT_REFERENCE_FRAME_MM_DA_AA(__a0,__a1,__d0)
#define ASSERT_COAXIAL_SAMPLE_EXIT_REFERENCE_FRAME_MM_DA_AA(__a0,__a1,__d0)
#define ASSERT_COAXIAL_HERO_SAMPLE_EXIT_REFERENCE_FRAME_MM_DA_AA(__a0,__a1,__d0)

//   Asserts whether the entry and exit coordinate systems of two
//   attenuations match. This is the check you have to make before
//   concatenating them.

#define ASSERT_REFERENCE_FRAME_COMPATIBILITY_AA(__a0,__a1,__d0)
#define ASSERT_SAMPLE_REFERENCE_FRAME_COMPATIBILITY_AA(__a0,__a1,__d0)
#define ASSERT_HERO_SAMPLE_REFERENCE_FRAME_COMPATIBILITY_AA(__a0,__a1,__d0)

//   Asserts whether the entry and exit coordinate systems of two
//   attenuations are identical. This is the check you have to make
//   before e.g. adding their influence, or interpolating between
//   two attenuations (actually, before any operation that involves
//   two attenuations, *except* concatenation).

#define ASSERT_IDENTICAL_REFERENCE_FRAMES_AA(__a0,__a1)
#define ASSERT_IDENTICAL_SAMPLE_REFERENCE_FRAMES_AA(__a0,__a1)
#define ASSERT_IDENTICAL_HERO_SAMPLE_REFERENCE_FRAMES_AA(__a0,__a1)

//   Same as above, except with the stored debug frame.

#define ASSERT_IDENTICAL_REFERENCE_FRAMES_SA(__a0)
#define ASSERT_IDENTICAL_SAMPLE_REFERENCE_FRAMES_SA(__a0)
#define ASSERT_IDENTICAL_HERO_SAMPLE_REFERENCE_FRAMES_SA(__a0)

//   Assertions for gathering results

#define ASSERT_ALLOCATED_GATHERING_RESULT(__r)
#define ASSERT_VALID_GATHERING_RESULT(__r)

//   Assertions for ObjC instances

#define ASSERT_VALID_ARCOBJECT_INSTANCE(__instance)
#define ASSERT_EXACT_CLASS_MEMBERSHIP(__instance,__classname)
#define ASSERT_CLASS_OR_SUBCLASS_MEMBERSHIP(__instance,__classname)
#define ASSERT_PROTOCOL_SUPPORT(__instance,__arprotocol)

//   Safety checks for code sections that must not be run in one particular mode

#define ASSERT_LIGHT_SUBSYSTEM_IN_POLARISATION_MODE
#define ASSERT_LIGHT_SUBSYSTEM_NOT_IN_POLARISATION_MODE

#define ASSERT_INTEGER_WITHIN_ISR_CHANNEL_RANGE(__i)

//   Checks graph traversal contents for protocol conformance

#define ASSERT_VALID_ARNGRAPHTRAVERSAL(__t)

//  Arbitrary pointer parameter and variable existence assertions

#define ASSERT_ALLOCATED_GENERIC_ARGUMENT(__f, __t)
#define ASSERT_ALLOCATED_VARIABLE(__f, __t) \

#else

#include "ArLightAndAttenuationSubsystemManagement.h"

//   *****   the *real* assertion macros follow   *****

#define ASSERT_NON_NAN_DOUBLE(__d) \
    if ( m_d_isNaN( __d ) ) \
        ART_ERRORHANDLING_FATAL_ERROR( \
            "NaN double argument '" #__d "' %f in %s", \
            (__d), __func__ \
            );

#define ASSERT_NON_INF_DOUBLE(__d) \
    if ( m_d_isInf( __d ) ) \
        ART_ERRORHANDLING_FATAL_ERROR( \
            "infinite double argument '" #__d "' %f in %s", \
            (__d), __func__ \
            );

#define ASSERT_VALID_DOUBLE(__d) \
{ \
    ASSERT_NON_NAN_DOUBLE(__d) \
}

#define ASSERT_VALID_FINITE_DOUBLE(__d) \
{ \
    ASSERT_NON_INF_DOUBLE(__d) \
    ASSERT_VALID_DOUBLE(__d) \
}

#define ASSERT_POSITIVE_DOUBLE(__d) \
{ \
    ASSERT_VALID_DOUBLE(__d) \
    if ( (__d) <= 0.0 ) \
        ART_ERRORHANDLING_FATAL_ERROR( \
            "double argument is not positive '" #__d "' %f in %s", \
            (__d), __func__ \
            ); \
}

#define ASSERT_POSITIVE_FINITE_DOUBLE(__d) \
{ \
    ASSERT_NON_INF_DOUBLE(__d) \
    ASSERT_POSITIVE_DOUBLE(__d) \
}

#define ASSERT_NONNEGATIVE_DOUBLE(__d) \
{ \
    ASSERT_VALID_DOUBLE(__d) \
    if ( (__d) < 0.0 ) \
        ART_ERRORHANDLING_FATAL_ERROR( \
            "negative double argument '" #__d "' %f in %s", \
            (__d), __func__ \
            ); \
}

#define ASSERT_DOUBLE_EQUAL_TO(__d1, __d2, __radius) \
{ \
    ASSERT_VALID_DOUBLE(__d1) \
    ASSERT_VALID_DOUBLE(__d2) \
    ASSERT_VALID_DOUBLE(__radius) \
    if ( (fabs((__d1) - (__d2)) > (__radius)) ) \
        ART_ERRORHANDLING_FATAL_ERROR( \
            "double argument '" #__d1 "' %.10f is not equal to '" #__d2 "' %.10f (within tolerance radius %.10f) in %s", \
            (__d1), (__d2), (__radius), __func__ \
            ); \
}

#define ASSERT_DOUBLE_LARGER_THAN_OR_EQUAL_TO(__d, __l) \
{ \
    ASSERT_VALID_DOUBLE(__d) \
    ASSERT_VALID_DOUBLE(__l) \
    if ( (__d) < (__l) ) \
        ART_ERRORHANDLING_FATAL_ERROR( \
            "double argument '" #__d "' %f is not larger than or equal to %f in %s", \
            (__d), (__l), __func__ \
            ); \
}

#define ASSERT_DOUBLE_LARGER_THAN(__d, __l) \
{ \
    ASSERT_VALID_DOUBLE(__d) \
    ASSERT_VALID_DOUBLE(__l) \
    if ( (__d) <= (__l) ) \
        ART_ERRORHANDLING_FATAL_ERROR( \
        "double argument '" #__d "' %f is not larger than %f in %s", \
        (__d), (__l), __func__ \
        ); \
}

#define ASSERT_DOUBLE_LESS_THAN_OR_EQUAL_TO(__d, __h) \
{ \
    ASSERT_VALID_DOUBLE(__d) \
    ASSERT_VALID_DOUBLE(__h) \
    if ( (__d) > (__h) ) \
        ART_ERRORHANDLING_FATAL_ERROR( \
            "double argument '" #__d "' %f is not less than or equal to %f in %s", \
            (__d), (__h), __func__ \
            ); \
}

#define ASSERT_DOUBLE_LESS_THAN(__d, __h) \
{ \
    ASSERT_VALID_DOUBLE(__d) \
    ASSERT_VALID_DOUBLE(__h) \
    if ( (__d) >= (__h) ) \
        ART_ERRORHANDLING_FATAL_ERROR( \
        "double argument '" #__d "' %f is not less than %f in %s", \
        (__d), (__h), __func__ \
        ); \
}

#define ASSERT_NONNEGATIVE_FINITE_DOUBLE(__d) \
{ \
    ASSERT_NON_INF_DOUBLE(__d) \
    ASSERT_NONNEGATIVE_DOUBLE(__d) \
}

#define ASSERT_NONZERO_DOUBLE(__d) \
{ \
    ASSERT_VALID_DOUBLE(__d) \
    if ( (__d) == 0.0 ) \
        ART_ERRORHANDLING_FATAL_ERROR( \
            "zero double argument '" #__d "' %f in %s", \
            (__d), __func__ \
            ); \
}

#define ASSERT_NONZERO_FINITE_DOUBLE(__d) \
{ \
    ASSERT_NON_INF_DOUBLE(__d) \
    ASSERT_NONZERO_DOUBLE(__d) \
}

#define ASSERT_UNIT_RANGE_DOUBLE(__d) \
{ \
    ASSERT_VALID_DOUBLE(__d) \
    if ( (__d) < 0.0 || (__d) > 1.0 ) \
        ART_ERRORHANDLING_FATAL_ERROR( \
            "double argument '" #__d "' %f not in range 0..1 in %s", \
            (__d), __func__ \
            ); \
}

#define ASSERT_RANDOM_DOUBLE(__d) \
{ \
    ASSERT_VALID_DOUBLE(__d) \
    if ( (__d) < 0.0 || (__d) >= 1.0 ) \
        ART_ERRORHANDLING_FATAL_ERROR( \
            "double argument '" #__d "' %f not in range [0,1) in %s", \
            (__d), __func__ \
            ); \
}

#define ASSERT_DOUBLE_WITHIN_RANGE(__d,__l,__h) \
{ \
    ASSERT_VALID_DOUBLE(__d) \
    if ( (__d) < (__l) || (__d) > (__h) ) \
        ART_ERRORHANDLING_FATAL_ERROR( \
            "double argument '" #__d "' %f not in range %f..%f in %s", \
            (__d), (__l), (__h), __func__ \
            ); \
}

#define ASSERT_POSITIVE_INTEGER(__i) \
if ( (__i) <= 0 ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "integer argument '" #__i "' %d not positive in %s", \
        (__i), __func__ \
        );

#define ASSERT_NONNEGATIVE_INTEGER(__i) \
if ( (__i) < 0 ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "negative integer argument '" #__i "' %d in %s", \
        (__i), __func__ \
        );

#define ASSERT_INTEGER_WITHIN_RANGE(__i,__l,__h) \
{ \
    if ( (__i) < (__l) || (__i) > (__h) ) \
        ART_ERRORHANDLING_FATAL_ERROR( \
            "integer argument '" #__i "' %d not in range %d..%d in %s", \
            (__i), (__l), (__h), __func__ \
            ); \
}

#define ASSERT_INTEGER_IS_EQUAL_TO(__i,__v) \
{ \
    if ( (__i) != (__v) ) \
        ART_ERRORHANDLING_FATAL_ERROR( \
            "integer argument '" #__i "' %d not equal to %d in %s", \
            (__i), (__v), __func__ \
            ); \
}

#define ASSERT_ALLOCATED_PNT3D(__r0) \
if ( ! (__r0) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "unallocated point '" #__r0 "' in %s", \
        __func__ \
        );

#define ASSERT_ALLOCATED_VEC3D(__r0) \
if ( ! (__r0) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "unallocated vector '" #__r0 "' in %s", \
        __func__ \
        );

#define ASSERT_ALLOCATED_RAY3D(__r0) \
if ( ! (__r0) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "unallocated ray '" #__r0 "' in %s", \
        __func__ \
        );


#define ASSERT_VALID_PNT3D(__p0) \
if ( ! pnt3d_p_valid( & (__p0) ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid point '" #__p0 "' " \
        PNT3D_FORMAT("% 5.3f") " in %s", \
        PNT3D_P_PRINTF(__p0), __func__ \
        );

#define ASSERT_VALID_VEC3D(__v0) \
if ( ! vec3d_v_valid( & (__v0) ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid vector '" #__v0 "' " \
        VEC3D_FORMAT("% 5.3f") " in %s", \
        VEC3D_V_PRINTF(__v0), __func__ \
        );

#define ASSERT_VALID_RAY3D(__r0) \
if ( ! pnt3d_p_valid( & RAY3D_P(__r0) ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid point in ray '" #__r0 "' " \
        PNT3D_FORMAT("% 5.3f") " in %s", \
        PNT3D_P_PRINTF(RAY3D_P(__r0)), __func__ \
        ); \
if ( ! vec3d_v_valid( & RAY3D_V(__r0) ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid vector in ray '" #__r0 "' " \
        VEC3D_FORMAT("% 5.3f") " in %s", \
        VEC3D_V_PRINTF(RAY3D_V(__r0)), __func__ \
        );

#define ASSERT_VALID_NORMALIZED_VEC3D(__v0) \
if ( ! vec3d_v_valid( & (__v0) ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid vector '" #__v0 "' " \
        VEC3D_FORMAT("% 5.3f") " in %s", \
        VEC3D_V_PRINTF(__v0), __func__ \
        );\
else if ( vec3d_v_len( & (__v0) ) < 0.9999 || vec3d_v_len( & (__v0) ) > 1.0001 ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "non-normalized vector '" #__v0 "' " \
        VEC3D_FORMAT("% 5.3f") " in %s", \
        VEC3D_V_PRINTF(__v0), __func__ \
        );

#define ASSERT_VALID_NORMALIZED_RAY3D(__r0) \
if ( ! pnt3d_p_valid( & RAY3D_P(__r0) ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid point in ray '" #__r0 "' " \
        PNT3D_FORMAT("% 5.3f") " in %s", \
        PNT3D_P_PRINTF(RAY3D_P(__r0)), __func__ \
        ); \
if ( ! vec3d_v_valid( & (RAY3D_V(__r0)) ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid vector in ray '" #__r0 "' " \
        VEC3D_FORMAT("% 5.3f") " in %s", \
        VEC3D_V_PRINTF(RAY3D_V(__r0)), __func__ \
        );\
else if ( vec3d_v_len( & (RAY3D_V(__r0)) ) < 0.9999 || vec3d_v_len( & (RAY3D_V(__r0)) ) > 1.0001 ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "non-normalized vector in ray '" #__r0 "' " \
        VEC3D_FORMAT("% 5.3f") " in %s", \
        VEC3D_V_PRINTF(RAY3D_V(__r0)), __func__ \
        );

#define ASSERT_IDENTICAL_VEC3D(__v0,__v1) \
if ( ! vec3d_vv_equal( & (__v0), & (__v1) ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "non-identical vectors '" #__v0 "' and '" #__v1 "' " \
        VEC3D_FORMAT("% 5.3f") " " VEC3D_FORMAT("% 5.3f") " in %s", \
        VEC3D_V_PRINTF(__v0), VEC3D_V_PRINTF(__v1), __func__ \
        );

#define ISR_ASSERTION_DATA    int  assertion_data;

#define INITIALISE_ISR_ASSERTION_DATA(__c) \
        (__c)->assertion_data = 0;

#define COLOUR_ASSERTIONS_ALLOW_NEGATIVE_VALUES(__c)

#define ASSERT_ALLOCATED_SPECTRUMTYPE(__ts,__t,__c) \
if ( (__c) == 0 ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "unallocated " #__t " argument '" #__c "' %p in %s", \
        (__c), __func__ \
        );

#define ASSERT_VALID_SPECTRUMTYPE(__ts,__t,__c,__s) \
if ( ! __ts##_##__s##_valid( art_gv, (__c) ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid " #__t " argument '" #__c "' %p in %s", \
        (__c), __func__ \
        );

#define ASSERT_NONZERO_COLOURTYPE(__ts,__t,__c,__s) \
if ( ! __ts##_##__s##_all_components_nonzero( art_gv, (__c) ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "zero components in " #__t " argument '" #__c "' %p in %s", \
        (__c), __func__ \
        );

#define ASSERT_ALLOCATED_SPECTRUM(__c) \
if ( (__c) == 0 ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "unallocated ArSpectrum argument '" #__c "' %p in %s", \
        (__c), __func__ \
        );

#define ASSERT_VALID_SPECTRUM(__c) \
if ( ! spc_s_valid( art_gv, (__c) ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid ArSpectrum argument '" #__c "' %p in %s", \
        (__c), __func__ \
        );


#define ASSERT_ALLOCATED_RGBA(__s) \
if ( (__s) == 0 ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "unallocated ArRGBA argument '" #__s "' %p in %s", \
        (__s), __func__ \
        );

#define ASSERT_VALID_RGBA(__s) \
if ( ! rgba_s_valid( art_gv, (__s) ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid ArRGBA argument '" #__s "' %p in %s", \
        (__s), __func__ \
        );


//   Spectral data type assertions


#define ASSERT_ALLOCATED_PSS(__s) \
if ( (__s) == 0 ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "unallocated ArPSSpectrum argument '" #__s "' %p in %s", \
        (__s), __func__ \
        );

#define ASSERT_VALID_PSS(__s) \
if ( ! pss_s_valid( art_gv, (__s) ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid ArPSSpectrum argument '" #__s "' %p in %s", \
        (__s), __func__ \
        );

#define ASSERT_ALLOCATED_RSS(__s) \
if ( (__s) == 0 ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "unallocated ArPSSpectrum argument '" #__s "' %p in %s", \
        (__s), __func__ \
        );

#define ASSERT_VALID_RSS(__s) \
if ( ! rss_s_valid( art_gv, (__s) ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid ArRSSpectrum argument '" #__s "' %p in %s", \
        (__s), __func__ \
        );

#define ASSERT_ALLOCATED_RSS2D(__s) \
if ( (__s) == 0 ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "unallocated ArPSSpectrum2D argument '" #__s "' %p in %s", \
        (__s), __func__ \
        );

#define ASSERT_VALID_RSS2D(__s) \
if ( ! rss2d_s_valid( art_gv, (__s) ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid ArRSSpectrum2D argument '" #__s "' %p in %s", \
        (__s), __func__ \
        );


//   Assertions for spectral samples.


#define ASSERT_ALLOCATED_HERO_VALUES(__l) \
if ( (__l) == 0 ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "unallocated ArHeroValues argument '" #__l "' %p in %s", \
        (__l), __func__ \
);

#define ASSERT_ALLOCATED_SPECTRAL_SAMPLE(__l) \
if ( (__l) == 0 ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "unallocated ArSpectralSample argument '" #__l "' %p in %s", \
        (__l), __func__ \
);

#define ASSERT_ALLOCATED_SPECTRAL_HERO_SAMPLE(__l) \
if ( (__l) == 0 ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "unallocated ArSpectralHeroSample argument '" #__l "' %p in %s", \
        (__l), __func__ \
);

#define ASSERT_VALID_HERO_VALUES(__l) \
if ( ! arherovalues_v_valid( art_gv, (__l) ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid ArHeroValues argument '" #__l "' %p in %s", \
        (__l), __func__ \
);

#define ASSERT_VALID_SPECTRAL_SAMPLE(__l) \
if ( ! sps_s_valid( art_gv, (__l) ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid ArSpectralSample argument '" #__l "' %p in %s", \
        (__l), __func__ \
);

#define ASSERT_VALID_SPECTRAL_HERO_SAMPLE(__l) \
if ( ! hs_h_valid( art_gv, (__l) ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid ArSpectralHeroSample argument '" #__l "' %p in %s", \
        (__l), __func__ \
);


//   Assertions for wavelength data structures.


#define ASSERT_ALLOCATED_WAVELENGTH(__c) \
if ( (__c) == 0 ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "unallocated ArWavelentgth argument '" #__c "' %p in %s", \
        (__c), __func__ \
        );

#define ASSERT_ALLOCATED_HERO_WAVELENGTHS(__c) \
if ( (__c) == 0 ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "unallocated ArHeroWavelentgths argument '" #__c "' %p in %s", \
        (__c), __func__ \
        );

#define ASSERT_VALID_WAVELENGTH(__c) \
if ( ! arwavelength_w_valid( art_gv, (__c) ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid ArWavelentgth argument '" #__c "' %p in %s", \
        (__c), __func__ \
        );

#define ASSERT_VALID_HERO_WAVELENGTHS(__c) \
if ( ! arherowavelengths_w_valid( art_gv, (__c) ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid ArHeroWavelentgths argument '" #__c "' %p in %s", \
        (__c), __func__ \
        );

//   Assertions for Stokes Vectors.


#define ASSERT_ALLOCATED_STOKES_VECTOR(__sv) \
if ( (__sv) == 0 ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "unallocated ArStokesVectorSample argument '" #__sv "' %p in %s", \
        (__sv), __func__ \
        );

#define ASSERT_ALLOCATED_STOKES_VECTOR_SAMPLE(__sv) \
if ( (__sv) == 0 ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "unallocated ArStokesVectorSample argument '" #__sv "' %p in %s", \
        (__sv), __func__ \
        );

#define ASSERT_ALLOCATED_STOKES_VECTOR_HERO_SAMPLE(__sv) \
if ( (__sv) == 0 ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "unallocated ArStokesVectorHeroSample argument '" #__sv "' %p in %s", \
        (__sv), __func__ \
        );

#define ASSERT_VALID_STOKES_VECTOR(__sv) \
if ( ! arstokesvector_sv_valid(art_gv, (__sv) ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid ArStokesVectorSample '" #__sv "' %p in %s", \
        (__sv), __func__ \
        );

#define ASSERT_VALID_STOKES_VECTOR_SAMPLE(__sv) \
if ( ! arstokesvectorsample_sv_valid(art_gv, (__sv) ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid ArStokesVectorSample '" #__sv "' %p in %s", \
        (__sv), __func__ \
        );

#define ASSERT_VALID_STOKES_VECTOR_HERO_SAMPLE(__sv) \
if ( ! arstokesvectorherosample_sv_valid(art_gv, (__sv) ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid ArStokesVectorHeroSample '" #__sv "' %p in %s", \
        (__sv), __func__ \
        );


//   Assertions for Müller matrics.


#define ASSERT_ALLOCATED_MUELLER_MATRIX(__mm) \
if ( (__mm) == 0 ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "unallocated ArMuellerMatrix argument '" #__mm "' %p in %s", \
        (__mm), __func__ \
        );

#define ASSERT_ALLOCATED_MUELLER_MATRIX_SAMPLE(__mm) \
if ( (__mm) == 0 ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "unallocated ArMuellerMatrixSample argument '" #__mm "' %p in %s", \
        (__mm), __func__ \
        );

#define ASSERT_ALLOCATED_MUELLER_MATRIX_HERO_SAMPLE(__mm) \
if ( (__mm) == 0 ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "unallocated ArMuellerMatrixHeroSample argument '" #__mm "' %p in %s", \
        (__mm), __func__ \
        );

#define ASSERT_VALID_MUELLER_MATRIX(__mm) \
if ( ! armuellermatrix_m_valid( art_gv, (__mm), 1, 0 ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid ArMuellerMatrix argument '" #__mm "' %p in %s", \
        (__mm), __func__ \
        );

#define ASSERT_VALID_MUELLER_MATRIX_SAMPLE(__mm) \
if ( ! armuellermatrixsample_m_valid( art_gv, (__mm), 1, 0 ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid ArMuellerMatrixSample argument '" #__mm "' %p in %s", \
        (__mm), __func__ \
        );

#define ASSERT_VALID_MUELLER_MATRIX_HERO_SAMPLE(__mm) \
if ( ! armuellermatrixherosample_m_valid( art_gv, (__mm), 1, 0 ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid ArMuellerMatrixHeroSample argument '" #__mm "' %p in %s", \
        (__mm), __func__ \
        );


//   Assertions for polarisation reference frames.


#define ASSERT_ALLOCATED_REFERENCE_FRAME(__rf) \
if ( (__rf) == 0 ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "unallocated ArReferenceFrame argument '" #__rf "' %p in %s", \
        (__rf), __func__ \
        );

#define ASSERT_VALID_REFERENCE_FRAME(__rf) \
if ( ! arrefframe_rf_valid(art_gv, (__rf) ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid refframe '" #__rf "' %p in %s", \
        (__rf), __func__ \
        );


//   Internal functions to be used in routines that require storage
//   of reference frames for assertion purposes.

//   Stores the reference frame of the given entity in a temp variable.
//   Only one such storage command may be used in each function!

#define STORE_LIGHT_REFERENCE_FRAME(__l) \
ArReferenceFrame  __rf_light; \
__rf_light = * arlight_l_refframe(art_gv, __l);

#define STORE_LIGHT_SAMPLE_REFERENCE_FRAME(__l) \
ArReferenceFrame  __rf_light; \
__rf_light = * arlightsample_l_refframe(art_gv, __l);

#define STORE_LIGHT_HERO_SAMPLE_REFERENCE_FRAME(__l) \
ArReferenceFrame  __rf_light; \
__rf_light = * arlightherosample_l_refframe(art_gv, __l);

#define STORE_ATTENUATION_REFERENCE_FRAMES(__f) \
ArReferenceFrame  __rf_entry; \
ArReferenceFrame  __rf_exit; \
__rf_entry = * arattenuation_a_entry_rf(art_gv, __f); \
__rf_exit = * arattenuation_a_exit_rf(art_gv, __f);

#define STORE_ATTENUATION_SAMPLE_REFERENCE_FRAMES(__f) \
ArReferenceFrame  __rf_entry; \
ArReferenceFrame  __rf_exit; \
__rf_entry = * arattenuationsample_a_entry_rf(art_gv, __f); \
__rf_exit = * arattenuationsample_a_exit_rf(art_gv, __f);

#define STORE_ATTENUATION_HERO_SAMPLE_REFERENCE_FRAMES(__f) \
ArReferenceFrame  __rf_entry; \
ArReferenceFrame  __rf_exit; \
__rf_entry = * arattenuationherosample_a_entry_rf(art_gv, __f); \
__rf_exit = * arattenuationherosample_a_exit_rf(art_gv, __f);


//   Light existence and validity assertions.

//   Generic ArLight


#define ASSERT_ALLOCATED_LIGHT(__l) \
if ( (__l) == 0 ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "unallocated ArLight argument '" #__l "' %p in %s", \
        (__l), __func__ \
        );

#define ASSERT_ALLOCATED_LIGHT_SAMPLE(__l) \
if ( (__l) == 0 ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "unallocated ArLightSample argument '" #__l "' %p in %s", \
        (__l), __func__ \
);

#define ASSERT_ALLOCATED_LIGHT_HERO_SAMPLE(__l) \
if ( (__l) == 0 ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "unallocated ArLightHeroSample argument '" #__l "' %p in %s", \
        (__l), __func__ \
);

#define ASSERT_VALID_LIGHT(__l) \
if ( ! arlight_l_valid( art_gv, (__l) ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid ArLight argument '" #__l "' %p in %s", \
        (__l), __func__ \
        );

#define ASSERT_VALID_LIGHT_SAMPLE(__l) \
if ( ! arlightsample_l_valid( art_gv, (__l) ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid ArLightSample argument '" #__l "' %p in %s", \
        (__l), __func__ \
        );

#define ASSERT_VALID_LIGHT_HERO_SAMPLE(__l) \
if ( ! arlightherosample_l_valid( art_gv, (__l) ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid ArLightHeroSample argument '" #__l "' %p in %s", \
        (__l), __func__ \
        );


//   Light existence and validity assertions.

//   ArSVLight


#define ASSERT_ALLOCATED_SV_LIGHT(__l) \
if ( (__l) == 0 ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "unallocated ArSVLight argument '" #__l "' %p in %s", \
        (__l), __func__ \
        );

#define ASSERT_ALLOCATED_SV_LIGHT_SAMPLE(__l) \
if ( (__l) == 0 ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "unallocated ArSVLightSample argument '" #__l "' %p in %s", \
        (__l), __func__ \
        );

#define ASSERT_ALLOCATED_SV_LIGHT_HERO_SAMPLE(__l) \
if ( (__l) == 0 ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "unallocated ArSVLightHeroSample argument '" #__l "' %p in %s", \
        (__l), __func__ \
        );

#define ASSERT_VALID_SV_LIGHT(__l) \
if ( ! arsvlight_l_valid( art_gv, (__l) ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid ArSVLight argument '" #__l "' %p in %s", \
        (__l), __func__ \
        );

#define ASSERT_VALID_SV_LIGHT_SAMPLE(__l) \
if ( ! arsvlightsample_l_valid( art_gv, (__l) ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid ArSVLightSample argument '" #__l "' %p in %s", \
        (__l), __func__ \
        );

#define ASSERT_VALID_SV_LIGHT_HERO_SAMPLE(__l) \
if ( ! arsvlightherosample_l_valid( art_gv, (__l) ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid ArSVLightHeroSample argument '" #__l "' %p in %s", \
        (__l), __func__ \
        );


//   Light existence and validity assertions.

//   ArLightAlpha


#define ASSERT_ALLOCATED_LIGHTALPHA(__l) \
if ( (__l) == 0 ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "unallocated ArLightAlpha argument '" #__l "' %p in %s", \
        (__l), __func__ \
        );

#define ASSERT_ALLOCATED_LIGHTALPHA_SAMPLE(__l) \
if ( (__l) == 0 ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "unallocated ArLightAlphaSample argument '" #__l "' %p in %s", \
        (__l), __func__ \
        );

#define ASSERT_ALLOCATED_LIGHTALPHA_HERO_SAMPLE(__l) \
if ( (__l) == 0 ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "unallocated ArLightAlphaHeroSample argument '" #__l "' %p in %s", \
        (__l), __func__ \
        );

#define ASSERT_VALID_LIGHTALPHA(__l) \
if ( ! arlightalpha_l_valid( art_gv, (__l) ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid ArLightAlpha argument '" #__l "' %p in %s", \
        (__l), __func__ \
        );

#define ASSERT_VALID_LIGHTALPHA_SAMPLE(__l) \
if ( ! arlightalphasample_l_valid( art_gv, (__l) ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid ArLightAlphaSample argument '" #__l "' %p in %s", \
        (__l), __func__ \
        );

#define ASSERT_VALID_LIGHTALPHA_HERO_SAMPLE(__l) \
if ( ! arlightalphaherosample_l_valid( art_gv, (__l) ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid ArLightAlphaHeroSample argument '" #__l "' %p in %s", \
        (__l), __func__ \
        );


//   Light existence and validity assertions.

//   ArLightIntensity


#define ASSERT_ALLOCATED_LIGHT_INTENSITY(__li) \
if ( (__li) == 0 ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "unallocated ArLightIntensity argument '" #__li "' %p in %s", \
        (__li), __func__ \
        );

#define ASSERT_ALLOCATED_LIGHT_INTENSITY_SAMPLE(__l) \
if ( (__l) == 0 ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "unallocated ArLightIntensitySample argument '" #__l "' %p in %s", \
        (__l), __func__ \
        );

#define ASSERT_ALLOCATED_LIGHT_INTENSITY_HERO_SAMPLE(__l) \
if ( (__l) == 0 ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "unallocated ArLightIntensityHeroSample argument '" #__l "' %p in %s", \
        (__l), __func__ \
        );

#define ASSERT_VALID_LIGHT_INTENSITY(__li) \
if ( ! arlightintensity_i_valid( art_gv, (__li) ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid ArLightIntensity argument '" #__li "' %p in %s", \
        (__li), __func__ \
        );

#define ASSERT_VALID_LIGHT_INTENSITY_SAMPLE(__l) \
if ( ! arlightintensitysample_i_valid( art_gv, (__l) ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid ArLightIntensitySample argument '" #__l "' %p in %s", \
        (__l), __func__ \
        );

#define ASSERT_VALID_LIGHT_INTENSITY_HERO_SAMPLE(__l) \
if ( ! arlightintensityherosample_i_valid( art_gv, (__l) ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid ArLightIntensityHeroSample argument '" #__l "' %p in %s", \
        (__l), __func__ \
        );


//   Crosstalk existence and validity assertions.


#define ASSERT_ALLOCATED_CROSSTALK(__cx) \
if ( (__cx) == 0 ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "unallocated ArCrosstalk argument '" #__cx "' %p in %s", \
        (__cx), __func__ \
        );

#define ASSERT_ALLOCATED_CROSSTALK_SAMPLE(__cx) \
if ( (__cx) == 0 ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "unallocated ArCrosstalkSample argument '" #__cx "' %p in %s", \
        (__cx), __func__ \
        );

#define ASSERT_ALLOCATED_CROSSTALK_HERO_SAMPLE(__cx) \
if ( (__cx) == 0 ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "unallocated ArCrosstalkHeroSample argument '" #__cx "' %p in %s", \
        (__cx), __func__ \
        );

#define ASSERT_ALLOCATED_CROSSTALK500(__cx) \
if ( (__cx) == 0 ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "unallocated ArCrosstalk500 argument '" #__cx "' %p in %s", \
        (__cx), __func__ \
        );

#define ASSERT_VALID_CROSSTALK(__cx) \
if ( ! arcrosstalk_x_valid( art_gv, (__cx) ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid ArCrosstalk argument '" #__cx "' %p in %s", \
        (__cx), __func__ \
        );

#define ASSERT_VALID_CROSSTALK_SAMPLE(__cx) \
if ( ! arcrosstalksample_x_valid( art_gv, (__cx) ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid ArCrosstalkSample argument '" #__cx "' %p in %s", \
        (__cx), __func__ \
        );

#define ASSERT_VALID_CROSSTALK_HERO_SAMPLE(__cx) \
if ( ! arcrosstalkherosample_x_valid( art_gv, (__cx) ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid ArCrosstalkHeroSample argument '" #__cx "' %p in %s", \
        (__cx), __func__ \
        );

#define ASSERT_VALID_CROSSTALK500(__cx) \
if ( ! cx500_x_valid( art_gv, (__cx) ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid ArCrosstalk500 argument '" #__cx "' %p in %s", \
        (__cx), __func__ \
        );


//   Attenuation existence and validity assertions.


#define ASSERT_ALLOCATED_ATTENUATION(__f) \
if ( (__f) == 0 ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "unallocated ArAttenuation argument '" #__f "' %p in %s", \
        (__f), __func__ \
        );

#define ASSERT_ALLOCATED_ATTENUATION_SAMPLE(__f) \
if ( (__f) == 0 ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "unallocated ArAttenuationSample argument '" #__f "' %p in %s", \
        (__f), __func__ \
        );

#define ASSERT_ALLOCATED_ATTENUATION_HERO_SAMPLE(__f) \
if ( (__f) == 0 ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "unallocated ArAttenuationHeroSample argument '" #__f "' %p in %s", \
        (__f), __func__ \
        );

#define ASSERT_VALID_ATTENUATION(__f) \
if ( ! arattenuation_a_valid( art_gv, (__f) ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid ArAttenuation argument '" #__f "' %p in %s", \
        (__f), __func__ \
        );

#define ASSERT_VALID_ATTENUATION_SAMPLE(__f) \
if ( ! arattenuationsample_a_valid( art_gv, (__f) ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid ArAttenuationSample argument '" #__f "' %p in %s", \
        (__f), __func__ \
        );

#define ASSERT_VALID_ATTENUATION_HERO_SAMPLE(__f) \
if ( ! arattenuationherosample_a_valid( art_gv, (__f) ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid ArAttenuationHeroSample argument '" #__f "' %p in %s", \
        (__f), __func__ \
        );


//   Direct MM attenuation existence and validity assertions.


#define ASSERT_ALLOCATED_MM_DIRECT_ATTENUATION(__f) \
if ( (__f) == 0 ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "unallocated ArMMDirectAttenuation argument '" #__f "' %p in %s", \
        (__f), __func__ \
        );

#define ASSERT_ALLOCATED_MM_DIRECT_ATTENUATION_SAMPLE(__f) \
if ( (__f) == 0 ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "unallocated ArMMDirectAttenuationSample argument '" #__f "' %p in %s", \
        (__f), __func__ \
        );

#define ASSERT_ALLOCATED_MM_DIRECT_ATTENUATION_HERO_SAMPLE(__f) \
if ( (__f) == 0 ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "unallocated ArMMDirectAttenuationHeroSample argument '" #__f "' %p in %s", \
        (__f), __func__ \
        );

#define ASSERT_VALID_MM_DIRECT_ATTENUATION(__f) \
if ( ! armmdirectattenuation_a_valid( art_gv, (__f) ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid ArMMDirectAttenuation argument '" #__f "' %p in %s", \
        (__f), __func__ \
        );

#define ASSERT_VALID_MM_DIRECT_ATTENUATION_SAMPLE(__f) \
if ( ! armmdirectattenuationsample_a_valid( art_gv, (__f) ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid ArMMDirectAttenuationSample argument '" #__f "' %p in %s", \
        (__f), __func__ \
        );

#define ASSERT_VALID_MM_DIRECT_ATTENUATION_HERO_SAMPLE(__f) \
if ( ! armmdirectattenuationherosample_a_valid( art_gv, (__f) ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid ArMMDirectAttenuationHeroSample argument '" #__f "' %p in %s", \
        (__f), __func__ \
        );


//   BSDF sample existence and validity assertions.


#define ASSERT_ALLOCATED_BSDFSAMPLE(__s) \
if ( (__s) == 0 ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "unallocated ArFullLightBSDFSample argument '" #__s "' %p in %s", \
        (__s), __func__ \
        );

#define ASSERT_ALLOCATED_SINGLE_WAVELENGTH_BSDFSAMPLE(__s) \
if ( (__s) == 0 ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "unallocated ArSingleWavelengthBSDFSample argument '" #__s "' %p in %s", \
        (__s), __func__ \
        );

#define ASSERT_ALLOCATED_HERO_SPECTRAL_SAMPLE_BSDFSAMPLE(__s) \
if ( (__s) == 0 ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "unallocated ArHeroBSDFSample argument '" #__s "' %p in %s", \
        (__s), __func__ \
        );

#define ASSERT_VALID_BSDFSAMPLE(__s) \
if ( ! arbsdfsample_s_valid( art_gv, (__s) ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid ArFullLightBSDFSample argument '" #__s "' %p in %s", \
        (__s), __func__ \
        );

#define ASSERT_VALID_SINGLE_WAVELENGTH_BSDFSAMPLE(__s) \
if ( ! arsinglewavelengthbsdfsample_s_valid( art_gv, (__s) ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid ArSingleWavelengthBSDFSample argument '" #__s "' %p in %s", \
        (__s), __func__ \
        );

#define ASSERT_VALID_HERO_BSDFSAMPLE(__s) \
if ( ! arherobsdfsample_s_valid( art_gv, (__s) ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid ArHeroBSDFSample argument '" #__s "' %p in %s", \
        (__s), __func__ \
        );


//   The following assertions check whether the reference frames used
//   for polarised light, and polarising attenuations, are collinear.

//   Basic coaxiality test for directly provided reference frames.


#define ASSERT_COAXIAL_REFERENCE_FRAMES_RR(__r0,__r1,__d0) \
if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
{ \
    if ( ! arrefframe_rf_rf_d_coaxial( \
               art_gv, \
               (__r0), \
               (__r1),\
               (__d0) \
               ) \
        ) \
    { \
        ART_ERRORHANDLING_FATAL_ERROR( \
            "non-coaxial reference frames for '" #__r0 "' %p " \
            "and '" #__r1 "' %p in %s", \
            (__r0), (__r1), __func__ \
            ); \
    } \
}


//   Asserts whether a directly given reference frame, and light struct
//   are coaxial. The _SVL version operates directly on ArSVLight, instead
//   of ArLight, and is used for internal checks within that module.


#define ASSERT_COAXIAL_REFERENCE_FRAMES_RL(__r0,__l0,__d0) \
if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
{ \
    if ( arlight_l_polarised( art_gv, (__l0) ) ) \
    { \
        if ( ! arrefframe_rf_rf_d_coaxial( \
                   art_gv, \
                   (__r0), \
                   arlight_l_refframe(art_gv, (__l0) ),\
                   (__d0) \
                   ) \
            ) \
        { \
            ART_ERRORHANDLING_FATAL_ERROR( \
                "non-coaxial reference frames for '" #__r0 "' %p " \
                "and '" #__l0 "' %p in %s", \
                (__r0), (__l0), __func__ \
                ); \
        } \
    } \
}

#define ASSERT_COAXIAL_SAMPLE_REFERENCE_FRAMES_RL(__r0,__l0,__d0) \
if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
{ \
    if ( arlightsample_l_polarised( art_gv, (__l0) ) ) \
    { \
        if ( ! arrefframe_rf_rf_d_coaxial( \
                   art_gv, \
                   (__r0), \
                   arlightsample_l_refframe(art_gv, (__l0) ),\
                   (__d0) \
                   ) \
            ) \
        { \
            ART_ERRORHANDLING_FATAL_ERROR( \
                "non-coaxial reference frames for '" #__r0 "' %p " \
                "and '" #__l0 "' %p in %s", \
                (__r0), (__l0), __func__ \
                ); \
        } \
    } \
}

#define ASSERT_COAXIAL_SAMPLE_REFERENCE_FRAMES_VL(__v0,__l0,__d0) \
if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
{ \
    if ( arlightsample_l_polarised( art_gv, (__l0) ) ) \
    { \
        if ( ! arrefframe_rf_vd_coaxial( \
                   art_gv, \
                   arlightsample_l_refframe(art_gv, (__l0) ),\
                   (__v0), \
                   (__d0) \
                   ) \
            ) \
        { \
            ART_ERRORHANDLING_FATAL_ERROR( \
                "non-coaxial reference frames for '" #__v0 "' %p " \
                "and '" #__l0 "' %p in %s", \
                (__v0), (__l0), __func__ \
                ); \
        } \
    } \
}

#define ASSERT_COAXIAL_HERO_SAMPLE_REFERENCE_FRAMES_RL(__r0,__l0,__d0) \
if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
{ \
    if ( arlightherosample_l_polarised( art_gv, (__l0) ) ) \
    { \
        if ( ! arrefframe_rf_rf_d_coaxial( \
                   art_gv, \
                   (__r0), \
                   arlightherosample_l_refframe(art_gv, (__l0) ),\
                   (__d0) \
                   ) \
            ) \
        { \
            ART_ERRORHANDLING_FATAL_ERROR( \
                "non-coaxial reference frames for '" #__r0 "' %p " \
                "and '" #__l0 "' %p in %s", \
                (__r0), (__l0), __func__ \
                ); \
        } \
    } \
}

#define ASSERT_COAXIAL_REFERENCE_FRAMES_R_SVL(__r0,__l0,__d0) \
if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
{ \
    if ( arsvlight_l_polarised( art_gv, (__l0) ) \
       ) \
    { \
        if ( ! arrefframe_rf_rf_d_coaxial( \
                   art_gv, \
                   (__r0),\
                   arsvlight_l_refframe(art_gv, (__l0) ),\
                   (__d0)  \
                 ) \
            ) \
        { \
            ART_ERRORHANDLING_FATAL_ERROR( \
                "non-coaxial reference frames for '" #__r0 "' %p " \
                "and '" #__l0 "' %p in %s", \
                (__r0), (__l0), __func__ \
                ); \
        } \
    } \
}

#define ASSERT_COAXIAL_SAMPLE_REFERENCE_FRAMES_R_SVL(__r0,__l0,__d0) \
if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
{ \
    if ( arsvlightsample_l_polarised( art_gv, (__l0) ) \
       ) \
    { \
        if ( ! arrefframe_rf_rf_d_coaxial( \
                   art_gv, \
                   (__r0),\
                   arsvlightsample_l_refframe(art_gv, (__l0) ),\
                   (__d0)  \
                 ) \
            ) \
        { \
            ART_ERRORHANDLING_FATAL_ERROR( \
                "non-coaxial reference frames for '" #__r0 "' %p " \
                "and '" #__l0 "' %p in %s", \
                (__r0), (__l0), __func__ \
                ); \
        } \
    } \
}

#define ASSERT_COAXIAL_HERO_SAMPLE_REFERENCE_FRAMES_R_SVL(__r0,__l0,__d0) \
if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
{ \
    if ( arsvlightherosample_l_polarised( art_gv, (__l0) ) \
       ) \
    { \
        if ( ! arrefframe_rf_rf_d_coaxial( \
                   art_gv, \
                   (__r0),\
                   arsvlightherosample_l_refframe(art_gv, (__l0) ),\
                   (__d0)  \
                 ) \
            ) \
        { \
            ART_ERRORHANDLING_FATAL_ERROR( \
                "non-coaxial reference frames for '" #__r0 "' %p " \
                "and '" #__l0 "' %p in %s", \
                (__r0), (__l0), __func__ \
                ); \
        } \
    } \
}


//   Asserts whether a directly given reference frame, and the reference
//   frame of a given light struct are identical.


#define ASSERT_IDENTICAL_REFERENCE_FRAMES_RL(__r0,__l0) \
if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
{ \
    if ( arlight_l_polarised( art_gv, (__l0) ) ) \
    { \
        if ( ! arrefframe_rf_rf_equal( \
                   art_gv, \
                   (__r0), \
                   arlight_l_refframe(art_gv, (__l0) )\
                   ) \
            ) \
        { \
            ART_ERRORHANDLING_FATAL_ERROR( \
                "non-identical reference frames for '" #__r0 "' %p " \
                "and '" #__l0 "' %p in %s", \
                (__r0), (__l0), __func__ \
                ); \
        } \
    } \
}

#define ASSERT_IDENTICAL_SAMPLE_REFERENCE_FRAMES_RL(__r0,__l0) \
if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
{ \
    if ( arlightsample_l_polarised( art_gv, (__l0) ) ) \
    { \
        if ( ! arrefframe_rf_rf_equal( \
                   art_gv, \
                   (__r0), \
                   arlightsample_l_refframe(art_gv, (__l0) )\
                   ) \
            ) \
        { \
            ART_ERRORHANDLING_FATAL_ERROR( \
                "non-identical reference frames for '" #__r0 "' %p " \
                "and '" #__l0 "' %p in %s", \
                (__r0), (__l0), __func__ \
                ); \
        } \
    } \
}

#define ASSERT_IDENTICAL_HERO_SAMPLE_REFERENCE_FRAMES_RL(__r0,__l0) \
if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
{ \
    if ( arlightherosample_l_polarised( art_gv, (__l0) ) ) \
    { \
        if ( ! arrefframe_rf_rf_equal( \
                   art_gv, \
                   (__r0), \
                   arlightherosample_l_refframe(art_gv, (__l0) )\
                   ) \
            ) \
        { \
            ART_ERRORHANDLING_FATAL_ERROR( \
                "non-identical reference frames for '" #__r0 "' %p " \
                "and '" #__l0 "' %p in %s", \
                (__r0), (__l0), __func__ \
                ); \
        } \
    } \
}


//   Asserts whether the stored reference frame (see above), and the reference
//   frame of a given light struct are identical.


//   < these are missing / not yet implemented >


//   Asserts whether two light structs have collinear reference frames.


#define ASSERT_COAXIAL_REFERENCE_FRAMES_LL(__l0,__l1,__d0) \
if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
{ \
    if (    arlight_l_polarised( art_gv, (__l0) ) \
         && arlight_l_polarised( art_gv, (__l1) ) \
       ) \
    { \
        if ( ! arrefframe_rf_rf_d_coaxial( \
                   art_gv, \
                   arlight_l_refframe(art_gv, (__l0) ),\
                   arlight_l_refframe(art_gv, (__l1) ),\
                   (__d0) \
                   ) \
            ) \
        { \
            ART_ERRORHANDLING_FATAL_ERROR( \
                "non-coaxial reference frames for '" #__l0 "' %p " \
                "and '" #__l1 "' %p in %s", \
                (__l0), (__l1), __func__ \
                ); \
        } \
    } \
}

#define ASSERT_COAXIAL_SAMPLE_REFERENCE_FRAMES_LL(__l0,__l1,__d0) \
if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
{ \
    if (    arlightsample_l_polarised( art_gv, (__l0) ) \
         && arlightsample_l_polarised( art_gv, (__l1) ) \
       ) \
    { \
        if ( ! arrefframe_rf_rf_d_coaxial( \
                   art_gv, \
                   arlightsample_l_refframe(art_gv, (__l0) ),\
                   arlightsample_l_refframe(art_gv, (__l1) ),\
                   (__d0) \
                   ) \
            ) \
        { \
            ART_ERRORHANDLING_FATAL_ERROR( \
                "non-coaxial reference frames for '" #__l0 "' %p " \
                "and '" #__l1 "' %p in %s", \
                (__l0), (__l1), __func__ \
                ); \
        } \
    } \
}

#define ASSERT_COAXIAL_HERO_SAMPLE_REFERENCE_FRAMES_LL(__l0,__l1,__d0) \
if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
{ \
    if (    arlightherosample_l_polarised( art_gv, (__l0) ) \
         && arlightherosample_l_polarised( art_gv, (__l1) ) \
       ) \
    { \
        if ( ! arrefframe_rf_rf_d_coaxial( \
                   art_gv, \
                   arlightherosample_l_refframe(art_gv, (__l0) ),\
                   arlightherosample_l_refframe(art_gv, (__l1) ),\
                   (__d0) \
                   ) \
            ) \
        { \
            ART_ERRORHANDLING_FATAL_ERROR( \
                "non-coaxial reference frames for '" #__l0 "' %p " \
                "and '" #__l1 "' %p in %s", \
                (__l0), (__l1), __func__ \
                ); \
        } \
    } \
}


//   Asserts whether the coordinate system of a reference frame, and the
//   *exit* coordinate system of an attenuation match.


#define ASSERT_COAXIAL_EXIT_REFERENCE_FRAME_RA(__r0,__a0,__d0) \
if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
{ \
    if ( ! arrefframe_rf_rf_d_coaxial( \
               art_gv, \
               (__r0), \
               arattenuation_a_exit_rf(art_gv, (__a0) ),\
               (__d0) \
               ) \
        ) \
    { \
        ART_ERRORHANDLING_FATAL_ERROR( \
            "non-coaxial reference frames for '" #__r0 "' %p " \
            "and exit frame of '" #__a0 "' %p in %s", \
            (__r0), (__a0), __func__ \
            ); \
    } \
}

#define ASSERT_COAXIAL_SAMPLE_EXIT_REFERENCE_FRAME_RA(__r0,__a0,__d0) \
if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
{ \
    if ( ! arrefframe_rf_rf_d_coaxial( \
               art_gv, \
               (__r0), \
               arattenuationsample_a_exit_rf(art_gv, (__a0) ),\
               (__d0) \
               ) \
        ) \
    { \
        ART_ERRORHANDLING_FATAL_ERROR( \
            "non-coaxial reference frames for '" #__r0 "' %p " \
            "and exit frame of '" #__a0 "' %p in %s", \
            (__r0), (__a0), __func__ \
            ); \
    } \
}

#define ASSERT_COAXIAL_HERO_SAMPLE_EXIT_REFERENCE_FRAME_RA(__r0,__a0,__d0) \
if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
{ \
    if ( ! arrefframe_rf_rf_d_coaxial( \
               art_gv, \
               (__r0), \
               arattenuationherosample_a_exit_rf(art_gv, (__a0) ),\
               (__d0) \
               ) \
        ) \
    { \
        ART_ERRORHANDLING_FATAL_ERROR( \
            "non-coaxial reference frames for '" #__r0 "' %p " \
            "and exit frame of '" #__a0 "' %p in %s", \
            (__r0), (__a0), __func__ \
            ); \
    } \
}


//   Asserts whether the coordinate system of a light struct, and the
//   *entry* coordinate system of an attenuation match.


#define ASSERT_COAXIAL_ENTRY_REFERENCE_FRAME_LA(__l0,__a0,__d0) \
if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
{ \
    if (    arlight_l_polarised( art_gv, (__l0) ) \
         && arattenuation_a_polarising( art_gv, (__a0) ) ) \
    { \
        if ( ! arrefframe_rf_rf_d_coaxial( \
                   art_gv, \
                   arlight_l_refframe(art_gv, (__l0) ),\
                   arattenuation_a_entry_rf(art_gv, (__a0) ),\
                   (__d0)  \
                   ) \
            ) \
        { \
            if ( ! arrefframe_rf_rf_d_coaxial( \
                       art_gv, \
                       arlight_l_refframe(art_gv, (__l0) ),\
                       arattenuation_a_exit_rf(art_gv, (__a0) ),\
                       (__d0)  \
                       ) \
                ) \
                ART_ERRORHANDLING_FATAL_ERROR( \
                    "reference frame of '" #__l0 "' %p " \
                    "and entry reference frame of '" #__a0 "' " \
                    "%p in %s are not coaxial", \
                    (__l0), (__a0), __func__ \
                    ); \
            else \
                ART_ERRORHANDLING_FATAL_ERROR( \
                    "reference frame of '" #__l0 "' %p " \
                    "and entry reference frame of '" #__a0 "' " \
                    "%p in %s are not coaxial - but the exit frame is!", \
                    (__l0), (__a0), __func__ \
                    ); \
        } \
    } \
}

#define ASSERT_COAXIAL_SAMPLE_ENTRY_REFERENCE_FRAME_LA(__l0,__a0,__d0) \
if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
{ \
    if (    arlightsample_l_polarised( art_gv, (__l0) ) \
         && arattenuationsample_a_polarising( art_gv, (__a0) ) ) \
    { \
        if ( ! arrefframe_rf_rf_d_coaxial( \
                   art_gv, \
                   arlightsample_l_refframe(art_gv, (__l0) ),\
                   arattenuationsample_a_entry_rf(art_gv, (__a0) ),\
                   (__d0)  \
                   ) \
            ) \
        { \
            if ( ! arrefframe_rf_rf_d_coaxial( \
                       art_gv, \
                       arlightsample_l_refframe(art_gv, (__l0) ),\
                       arattenuationsample_a_exit_rf(art_gv, (__a0) ),\
                       (__d0)  \
                       ) \
                ) \
                ART_ERRORHANDLING_FATAL_ERROR( \
                    "reference frame of '" #__l0 "' %p " \
                    "and entry reference frame of '" #__a0 "' " \
                    "%p in %s are not coaxial", \
                    (__l0), (__a0), __func__ \
                    ); \
            else \
                ART_ERRORHANDLING_FATAL_ERROR( \
                    "reference frame of '" #__l0 "' %p " \
                    "and entry reference frame of '" #__a0 "' " \
                    "%p in %s are not coaxial - but the exit frame is!", \
                    (__l0), (__a0), __func__ \
                    ); \
        } \
    } \
}

#define ASSERT_COAXIAL_HERO_SAMPLE_ENTRY_REFERENCE_FRAME_LA(__l0,__a0,__d0) \
if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
{ \
    if (    arlightherosample_l_polarised( art_gv, (__l0) ) \
         && arattenuationherosample_a_polarising( art_gv, (__a0) ) ) \
    { \
        if ( ! arrefframe_rf_rf_d_coaxial( \
                   art_gv, \
                   arlightherosample_l_refframe(art_gv, (__l0) ),\
                   arattenuationherosample_a_entry_rf(art_gv, (__a0) ),\
                   (__d0)  \
                   ) \
            ) \
        { \
            if ( ! arrefframe_rf_rf_d_coaxial( \
                       art_gv, \
                       arlightherosample_l_refframe(art_gv, (__l0) ),\
                       arattenuationherosample_a_exit_rf(art_gv, (__a0) ),\
                       (__d0)  \
                       ) \
                ) \
                ART_ERRORHANDLING_FATAL_ERROR( \
                    "reference frame of '" #__l0 "' %p " \
                    "and entry reference frame of '" #__a0 "' " \
                    "%p in %s are not coaxial", \
                    (__l0), (__a0), __func__ \
                    ); \
            else \
                ART_ERRORHANDLING_FATAL_ERROR( \
                    "reference frame of '" #__l0 "' %p " \
                    "and entry reference frame of '" #__a0 "' " \
                    "%p in %s are not coaxial - but the exit frame is!", \
                    (__l0), (__a0), __func__ \
                    ); \
        } \
    } \
}


//   Asserts whether the coordinate system of a light struct, and the
//   *exit* coordinate system of an attenuation match.


#define ASSERT_COAXIAL_EXIT_REFERENCE_FRAME_LA(__l0,__a0,__d0) \
if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
{ \
    if (    arlight_l_polarised( art_gv, (__l0) ) \
         && arattenuation_a_polarising( art_gv, (__a0) ) ) \
    { \
        if ( ! arrefframe_rf_rf_d_coaxial( \
                   art_gv, \
                   arlight_l_refframe(art_gv, (__l0) ),\
                   arattenuation_a_exit_rf(art_gv, (__a0) ),\
                  (__d0)  \
                   ) \
            ) \
        { \
            ART_ERRORHANDLING_FATAL_ERROR( \
                "reference frame of '" #__l0 "' %p " \
                "and exit reference frame of '" #__a0 "' " \
                "%p in %s are not coaxial", \
                (__l0), (__a0), __func__ \
                ); \
        } \
    } \
}

#define ASSERT_COAXIAL_SAMPLE_EXIT_REFERENCE_FRAME_LA(__l0,__a0,__d0) \
if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
{ \
    if (    arlightsample_l_polarised( art_gv, (__l0) ) \
         && arattenuationsample_a_polarising( art_gv, (__a0) ) ) \
    { \
        if ( ! arrefframe_rf_rf_d_coaxial( \
                art_gv, \
                arlightsample_l_refframe(art_gv, (__l0) ),\
                arattenuationsample_a_exit_rf(art_gv, (__a0) ),\
                (__d0)  \
                ) \
            ) \
        { \
            ART_ERRORHANDLING_FATAL_ERROR( \
                "reference frame of '" #__l0 "' %p " \
                "and exit reference frame of '" #__a0 "' " \
                "%p in %s are not coaxial", \
                (__l0), (__a0), __func__ \
                ); \
        } \
    } \
}

#define ASSERT_COAXIAL_HERO_SAMPLE_EXIT_REFERENCE_FRAME_LA(__l0,__a0,__d0) \
if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
{ \
    if (    arlightherosample_l_polarised( art_gv, (__l0) ) \
         && arattenuationherosample_a_polarising( art_gv, (__a0) ) ) \
    { \
        if ( ! arrefframe_rf_rf_d_coaxial( \
                art_gv, \
                arlightherosample_l_refframe(art_gv, (__l0) ),\
                arattenuationherosample_a_exit_rf(art_gv, (__a0) ),\
                (__d0)  \
                ) \
            ) \
        { \
            ART_ERRORHANDLING_FATAL_ERROR( \
                "reference frame of '" #__l0 "' %p " \
                "and exit reference frame of '" #__a0 "' " \
                "%p in %s are not coaxial", \
                (__l0), (__a0), __func__ \
                ); \
        } \
    } \
}


//   Asserts whether the coordinate system of a light struct and a given
//   ray match.


#define ASSERT_COAXIAL_REFERENCE_FRAME_L_WITH_EYE_RAY(__l0,__r0,__d0) \
if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
{ \
    if ( arlight_l_polarised( art_gv, (__l0) ) ) \
    { \
        Vec3D  lightVec;\
\
        vec3d_dv_mul_v( -1.0, & RAY3D_V(*(__r0)), & lightVec ); \
\
        if ( ! arrefframe_rf_vd_coaxial( \
                     art_gv, \
                     arlight_l_refframe(art_gv, (__l0) ),\
                   & lightVec, \
                     (__d0) \
                   ) \
            ) \
        { \
            ART_ERRORHANDLING_FATAL_ERROR( \
                "light reference frame of '" #__l0 "' %p " \
                "not coaxial with eye ray '" #__r0 "' %p in %s", \
                (__l0), (__r0), __func__ \
                ); \
        } \
    } \
}

#define ASSERT_COAXIAL_SAMPLE_REFERENCE_FRAME_L_WITH_EYE_RAY(__l0,__r0,__d0) \
if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
{ \
    if ( arlightsample_l_polarised( art_gv, (__l0) ) ) \
    { \
        Vec3D  lightVec;\
\
        vec3d_dv_mul_v( -1.0, & RAY3D_V(*(__r0)), & lightVec ); \
\
        if ( ! arrefframe_rf_vd_coaxial( \
                    art_gv, \
                    arlightsample_l_refframe(art_gv, (__l0) ),\
                  & lightVec, \
                    (__d0) \
                  ) \
           ) \
        { \
           ART_ERRORHANDLING_FATAL_ERROR( \
               "light reference frame of '" #__l0 "' %p " \
               "not coaxial with eye ray '" #__r0 "' %p in %s", \
               (__l0), (__r0), __func__ \
               ); \
        } \
    } \
}

#define ASSERT_COAXIAL_HERO_SAMPLE_REFERENCE_FRAME_L_WITH_EYE_RAY(__l0,__r0,__d0) \
if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
{ \
    if ( arlightherosample_l_polarised( art_gv, (__l0) ) ) \
    { \
        Vec3D  lightVec;\
\
        vec3d_dv_mul_v( -1.0, & RAY3D_V(*(__r0)), & lightVec ); \
\
        if ( ! arrefframe_rf_vd_coaxial( \
                    art_gv, \
                    arlightherosample_l_refframe(art_gv, (__l0) ),\
                  & lightVec, \
                    (__d0) \
                  ) \
           ) \
        { \
           ART_ERRORHANDLING_FATAL_ERROR( \
               "light reference frame of '" #__l0 "' %p " \
               "not coaxial with eye ray '" #__r0 "' %p in %s", \
               (__l0), (__r0), __func__ \
               ); \
        } \
    } \
}


//   Asserts whether the entry coordinate systems of two attenuations are
//   coaxial.


#define ASSERT_COAXIAL_ENTRY_REFERENCE_FRAME_AA(__a0,__a1,__d0) \
if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
{ \
    if ( ! arrefframe_rf_rf_d_coaxial( \
               art_gv, \
               arattenuation_a_entry_rf(art_gv, (__a0) ),\
               arattenuation_a_entry_rf(art_gv, (__a1) ),\
               (__d0)  \
               ) \
        ) \
    { \
        ART_ERRORHANDLING_FATAL_ERROR( \
            "entry reference frames of '" #__a0 "' %p " \
            "and '" #__a1 "' %p in %s are not coaxial", \
            (__a0), (__a1), __func__ \
            ); \
    } \
}

#define ASSERT_COAXIAL_SAMPLE_ENTRY_REFERENCE_FRAME_AA(__a0,__a1,__d0) \
if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
{ \
    if ( ! arrefframe_rf_rf_d_coaxial( \
               art_gv, \
               arattenuationsample_a_entry_rf(art_gv, (__a0) ),\
               arattenuationsample_a_entry_rf(art_gv, (__a1) ),\
               (__d0)  \
               ) \
        ) \
    { \
        ART_ERRORHANDLING_FATAL_ERROR( \
            "entry reference frames of '" #__a0 "' %p " \
            "and '" #__a1 "' %p in %s are not coaxial", \
            (__a0), (__a1), __func__ \
            ); \
    } \
}

#define ASSERT_COAXIAL_HERO_SAMPLE_ENTRY_REFERENCE_FRAME_AA(__a0,__a1,__d0) \
if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
{ \
    if ( ! arrefframe_rf_rf_d_coaxial( \
               art_gv, \
               arattenuationherosample_a_entry_rf(art_gv, (__a0) ),\
               arattenuationherosample_a_entry_rf(art_gv, (__a1) ),\
               (__d0)  \
               ) \
        ) \
    { \
        ART_ERRORHANDLING_FATAL_ERROR( \
            "entry reference frames of '" #__a0 "' %p " \
            "and '" #__a1 "' %p in %s are not coaxial", \
            (__a0), (__a1), __func__ \
            ); \
    } \
}


//   Asserts whether the entry coordinate systems of two MM direct attenuations
//   are coaxial.


#define ASSERT_COAXIAL_ENTRY_REFERENCE_FRAME_MM_DA_AA(__a0,__a1,__d0) \
if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
{ \
    if ( ! arrefframe_rf_rf_d_coaxial( \
               art_gv, \
               armmdirectattenuation_a_entry_rf(art_gv, (__a0) ),\
               armmdirectattenuation_a_entry_rf(art_gv, (__a1) ),\
               (__d0)  \
               ) \
        ) \
    { \
        ART_ERRORHANDLING_FATAL_ERROR( \
            "entry reference frames of '" #__a0 "' %p " \
            "and '" #__a1 "' %p in %s are not coaxial", \
            (__a0), (__a1), __func__ \
            ); \
    } \
}

#define ASSERT_COAXIAL_SAMPLE_ENTRY_REFERENCE_FRAME_MM_DA_AA(__a0,__a1,__d0) \
if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
{ \
    if ( ! arrefframe_rf_rf_d_coaxial( \
               art_gv, \
               armmdirectattenuationsample_a_entry_rf(art_gv, (__a0) ),\
               armmdirectattenuationsample_a_entry_rf(art_gv, (__a1) ),\
               (__d0)  \
               ) \
        ) \
    { \
        ART_ERRORHANDLING_FATAL_ERROR( \
            "entry reference frames of '" #__a0 "' %p " \
            "and '" #__a1 "' %p in %s are not coaxial", \
            (__a0), (__a1), __func__ \
            ); \
    } \
}

#define ASSERT_COAXIAL_HERO_SAMPLE_ENTRY_REFERENCE_FRAME_MM_DA_AA(__a0,__a1,__d0) \
if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
{ \
    if ( ! arrefframe_rf_rf_d_coaxial( \
               art_gv, \
               armmdirectattenuationherosample_a_entry_rf(art_gv, (__a0) ),\
               armmdirectattenuationherosample_a_entry_rf(art_gv, (__a1) ),\
               (__d0)  \
               ) \
        ) \
    { \
        ART_ERRORHANDLING_FATAL_ERROR( \
            "entry reference frames of '" #__a0 "' %p " \
            "and '" #__a1 "' %p in %s are not coaxial", \
            (__a0), (__a1), __func__ \
            ); \
    } \
}


//   Asserts whether the exit coordinate systems of two attenuations are
//   coaxial.


#define ASSERT_COAXIAL_EXIT_REFERENCE_FRAME_AA(__a0,__a1,__d0) \
if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
{ \
    if ( ! arrefframe_rf_rf_d_coaxial( \
               art_gv, \
               arattenuation_a_exit_rf(art_gv, (__a0) ),\
               arattenuation_a_exit_rf(art_gv, (__a1) ),\
               (__d0) \
               ) \
        ) \
    { \
        ART_ERRORHANDLING_FATAL_ERROR( \
            "exit reference frames of '" #__a0 "' %p " \
            "and '" #__a1 "' %p in %s are not coaxial", \
            (__a0), (__a1), __func__ \
            ); \
    } \
}

#define ASSERT_COAXIAL_SAMPLE_EXIT_REFERENCE_FRAME_AA(__a0,__a1,__d0) \
if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
{ \
    if ( ! arrefframe_rf_rf_d_coaxial( \
               art_gv, \
               arattenuationsample_a_exit_rf(art_gv, (__a0) ),\
               arattenuationsample_a_exit_rf(art_gv, (__a1) ),\
               (__d0) \
               ) \
        ) \
    { \
        ART_ERRORHANDLING_FATAL_ERROR( \
            "exit reference frames of '" #__a0 "' %p " \
            "and '" #__a1 "' %p in %s are not coaxial", \
            (__a0), (__a1), __func__ \
            ); \
    } \
}

#define ASSERT_COAXIAL_HERO_SAMPLE_EXIT_REFERENCE_FRAME_AA(__a0,__a1,__d0) \
if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
{ \
    if ( ! arrefframe_rf_rf_d_coaxial( \
               art_gv, \
               arattenuationherosample_a_exit_rf(art_gv, (__a0) ),\
               arattenuationherosample_a_exit_rf(art_gv, (__a1) ),\
               (__d0) \
               ) \
        ) \
    { \
        ART_ERRORHANDLING_FATAL_ERROR( \
            "exit reference frames of '" #__a0 "' %p " \
            "and '" #__a1 "' %p in %s are not coaxial", \
            (__a0), (__a1), __func__ \
            ); \
    } \
}


//   Asserts whether the exit coordinate systems of two MM direct attenuations
//   are coaxial.


#define ASSERT_COAXIAL_EXIT_REFERENCE_FRAME_MM_DA_AA(__a0,__a1,__d0) \
if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
{ \
    if ( ! arrefframe_rf_rf_d_coaxial( \
               art_gv, \
               armmdirectattenuation_a_exit_rf(art_gv, (__a0) ),\
               armmdirectattenuation_a_exit_rf(art_gv, (__a1) ),\
               (__d0) \
               ) \
        ) \
    { \
        ART_ERRORHANDLING_FATAL_ERROR( \
            "exit reference frames of '" #__a0 "' %p " \
            "and '" #__a1 "' %p in %s are not coaxial", \
            (__a0), (__a1), __func__ \
            ); \
    } \
}

#define ASSERT_COAXIAL_SAMPLE_EXIT_REFERENCE_FRAME_MM_DA_AA(__a0,__a1,__d0) \
if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
{ \
    if ( ! arrefframe_rf_rf_d_coaxial( \
               art_gv, \
               armmdirectattenuationsample_a_exit_rf(art_gv, (__a0) ),\
               armmdirectattenuationsample_a_exit_rf(art_gv, (__a1) ),\
               (__d0) \
               ) \
        ) \
    { \
        ART_ERRORHANDLING_FATAL_ERROR( \
            "exit reference frames of '" #__a0 "' %p " \
            "and '" #__a1 "' %p in %s are not coaxial", \
            (__a0), (__a1), __func__ \
            ); \
    } \
}

#define ASSERT_COAXIAL_HERO_SAMPLE_EXIT_REFERENCE_FRAME_MM_DA_AA(__a0,__a1,__d0) \
if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
{ \
    if ( ! arrefframe_rf_rf_d_coaxial( \
               art_gv, \
               armmdirectattenuationherosample_a_exit_rf(art_gv, (__a0) ),\
               armmdirectattenuationherosample_a_exit_rf(art_gv, (__a1) ),\
               (__d0) \
               ) \
        ) \
    { \
        ART_ERRORHANDLING_FATAL_ERROR( \
            "exit reference frames of '" #__a0 "' %p " \
            "and '" #__a1 "' %p in %s are not coaxial", \
            (__a0), (__a1), __func__ \
            ); \
    } \
}


//   Asserts whether the entry and exit coordinate systems of two
//   attenuations match. This is the check you have to make before
//   concatenating them.


#define ASSERT_REFERENCE_FRAME_COMPATIBILITY_AA(__a0,__a1,__d0) \
if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
{ \
    if ( ! arrefframe_rf_rf_d_coaxial( \
               art_gv, \
               arattenuation_a_exit_rf(art_gv, (__a0) ),\
               arattenuation_a_entry_rf(art_gv, (__a1) ),\
               (__d0)  \
               ) \
        ) \
    { \
        ART_ERRORHANDLING_FATAL_ERROR( \
            "exit reference frame of '" #__a0 "' %p " \
            "is not coaxial with entry reference frame of " \
            "'" #__a1 "' %p in %s", \
            (__a0), (__a1), __func__ \
            ); \
    } \
}

#define ASSERT_SAMPLE_REFERENCE_FRAME_COMPATIBILITY_AA(__a0,__a1,__d0) \
if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
{ \
    if ( ! arrefframe_rf_rf_d_coaxial( \
               art_gv, \
               arattenuationsample_a_exit_rf(art_gv, (__a0) ),\
               arattenuationsample_a_entry_rf(art_gv, (__a1) ),\
               (__d0)  \
               ) \
        ) \
    { \
        ART_ERRORHANDLING_WARNING( \
            "original test for refframe coaxiality failed. We try the " \
            "reversed order of attenuators, as commutativity errors are " \
            "a common problem..." \
            ); \
        if ( arrefframe_rf_rf_d_coaxial( \
                   art_gv, \
                   arattenuationsample_a_exit_rf(art_gv, (__a1) ),\
                   arattenuationsample_a_entry_rf(art_gv, (__a0) ),\
                   (__d0)  \
                   ) \
            ) \
            ART_ERRORHANDLING_FATAL_ERROR( \
                "the exit reference frame of '" #__a0 "' %p " \
                "is not coaxial with entry reference frame of " \
                "'" #__a1 "' %p in %s. But if the attenuators were " \
                "combined in reverse order, the exit/entry refframes " \
                "would be coaxial!", \
                (__a0), (__a1), __func__ \
                ); \
        else \
            ART_ERRORHANDLING_FATAL_ERROR( \
                "the exit reference frame of '" #__a0 "' %p " \
                "is not coaxial with entry reference frame of " \
                "'" #__a1 "' %p in %s. Even if the attenuators were " \
                "combined in reverse order, the exit/entry refframes " \
                "would not be compatible.", \
                (__a0), (__a1), __func__ \
                ); \
    } \
}

#define ASSERT_HERO_SAMPLE_REFERENCE_FRAME_COMPATIBILITY_AA(__a0,__a1,__d0) \
if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
{ \
    if ( ! arrefframe_rf_rf_d_coaxial( \
               art_gv, \
               arattenuationherosample_a_exit_rf(art_gv, (__a0) ),\
               arattenuationherosample_a_entry_rf(art_gv, (__a1) ),\
               (__d0)  \
               ) \
        ) \
    { \
        ART_ERRORHANDLING_WARNING( \
            "original test for refframe coaxiality failed. We try the " \
            "reversed order of attenuators, as commutativity errors are " \
            "a common problem..." \
            ); \
        if ( arrefframe_rf_rf_d_coaxial( \
                   art_gv, \
                   arattenuationherosample_a_exit_rf(art_gv, (__a1) ),\
                   arattenuationherosample_a_entry_rf(art_gv, (__a0) ),\
                   (__d0)  \
                   ) \
            ) \
            ART_ERRORHANDLING_FATAL_ERROR( \
                "the exit reference frame of '" #__a0 "' %p " \
                "is not coaxial with entry reference frame of " \
                "'" #__a1 "' %p in %s. But if the attenuators were " \
                "combined in reverse order, the exit/entry refframes " \
                "would be coaxial!", \
                (__a0), (__a1), __func__ \
                ); \
        else \
            ART_ERRORHANDLING_FATAL_ERROR( \
                "the exit reference frame of '" #__a0 "' %p " \
                "is not coaxial with entry reference frame of " \
                "'" #__a1 "' %p in %s. Even if the attenuators were " \
                "combined in reverse order, the exit/entry refframes " \
                "would not be compatible.", \
                (__a0), (__a1), __func__ \
                ); \
    } \
}


//   Asserts whether the entry and exit coordinate systems of two
//   attenuations are identical. This is the check you have to make
//   before e.g. adding their influence, or interpolating between
//   two attenuations (actually, before any operation that involves
//   two attenuations, *except* concatenation).


#define ASSERT_IDENTICAL_REFERENCE_FRAMES_AA(__a0,__a1) \
if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
{ \
    if ( ! ( arrefframe_rf_rf_equal( \
                 art_gv, \
                 arattenuation_a_entry_rf(art_gv, (__a0) ),\
                 arattenuation_a_entry_rf(art_gv, (__a1) ) \
                 ) \
             && \
             arrefframe_rf_rf_equal( \
                 art_gv, \
                 arattenuation_a_exit_rf(art_gv, (__a0) ),\
                 arattenuation_a_exit_rf(art_gv, (__a1) ) \
                 ) \
            ) \
        ) \
    { \
        ART_ERRORHANDLING_FATAL_ERROR( \
            "reference frames of '" #__a0 "' %p " \
            "and '" #__a1 "' %p in %s are not identical", \
            (__a0), (__a1), __func__ \
            ); \
    } \
}

#define ASSERT_IDENTICAL_SAMPLE_REFERENCE_FRAMES_AA(__a0,__a1) \
if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
{ \
    if ( ! ( arrefframe_rf_rf_equal( \
                 art_gv, \
                 arattenuationsample_a_entry_rf(art_gv, (__a0) ),\
                 arattenuationsample_a_entry_rf(art_gv, (__a1) ) \
                 ) \
             && \
             arrefframe_rf_rf_equal( \
                 art_gv, \
                 arattenuationsample_a_exit_rf(art_gv, (__a0) ),\
                 arattenuationsample_a_exit_rf(art_gv, (__a1) ) \
                 ) \
            ) \
        ) \
    { \
        ART_ERRORHANDLING_FATAL_ERROR( \
            "reference frames of '" #__a0 "' %p " \
            "and '" #__a1 "' %p in %s are not identical", \
            (__a0), (__a1), __func__ \
            ); \
    } \
}

#define ASSERT_IDENTICAL_HERO_SAMPLE_REFERENCE_FRAMES_AA(__a0,__a1) \
if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
{ \
    if ( ! ( arrefframe_rf_rf_equal( \
                 art_gv, \
                 arattenuationherosample_a_entry_rf(art_gv, (__a0) ),\
                 arattenuationherosample_a_entry_rf(art_gv, (__a1) ) \
                 ) \
             && \
             arrefframe_rf_rf_equal( \
                 art_gv, \
                 arattenuationherosample_a_exit_rf(art_gv, (__a0) ),\
                 arattenuationherosample_a_exit_rf(art_gv, (__a1) ) \
                 ) \
            ) \
        ) \
    { \
        ART_ERRORHANDLING_FATAL_ERROR( \
            "reference frames of '" #__a0 "' %p " \
            "and '" #__a1 "' %p in %s are not identical", \
            (__a0), (__a1), __func__ \
            ); \
    } \
}


//   Same as above, except with the stored debug frame.


#define ASSERT_IDENTICAL_REFERENCE_FRAMES_SA(__a0) \
if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
{ \
    if ( ! ( arrefframe_rf_rf_equal( \
                   art_gv, \
                 & __rf_entry,\
                   arattenuation_a_entry_rf(art_gv, (__a0) ) \
                 ) \
             && \
             arrefframe_rf_rf_equal( \
                   art_gv, \
                 & __rf_exit,\
                   arattenuation_a_exit_rf(art_gv, (__a0) ) \
                 ) \
            ) \
        ) \
    { \
        ART_ERRORHANDLING_FATAL_ERROR( \
            "reference frames of '" #__a0 "' %p " \
            "and stored refframes in %s are not identical", \
            (__a0), __func__ \
            ); \
    } \
}

#define ASSERT_IDENTICAL_SAMPLE_REFERENCE_FRAMES_SA(__a0) \
if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
{ \
    if ( ! ( arrefframe_rf_rf_equal( \
                   art_gv, \
                 & __rf_entry,\
                   arattenuationsample_a_entry_rf(art_gv, (__a0) ) \
                 ) \
             && \
             arrefframe_rf_rf_equal( \
                   art_gv, \
                 & __rf_exit,\
                   arattenuationsample_a_exit_rf(art_gv, (__a0) ) \
                 ) \
            ) \
        ) \
    { \
        ART_ERRORHANDLING_FATAL_ERROR( \
            "reference frames of '" #__a0 "' %p " \
            "and stored refframes in %s are not identical", \
            (__a0), __func__ \
            ); \
    } \
}

#define ASSERT_IDENTICAL_HERO_SAMPLE_REFERENCE_FRAMES_SA(__a0) \
if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
{ \
    if ( ! ( arrefframe_rf_rf_equal( \
                   art_gv, \
                 & __rf_entry,\
                   arattenuationherosample_a_entry_rf(art_gv, (__a0) ) \
                 ) \
             && \
             arrefframe_rf_rf_equal( \
                   art_gv, \
                 & __rf_exit,\
                   arattenuationherosample_a_exit_rf(art_gv, (__a0) ) \
                 ) \
            ) \
        ) \
    { \
        ART_ERRORHANDLING_FATAL_ERROR( \
            "reference frames of '" #__a0 "' %p " \
            "and stored refframes in %s are not identical", \
            (__a0), __func__ \
            ); \
    } \
}


//   Assertions for gathering results

#define ASSERT_ALLOCATED_GATHERING_RESULT(__r) \
if ( (__r) == 0 ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "unallocated ArPathspaceResult argument '" #__r "' %p in %s", \
        (__r), __func__ \
        );

#define ASSERT_VALID_GATHERING_RESULT(__r) \
if ( ! arpathspaceresult_r_valid( art_gv, (__r) ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "invalid ArPathspaceResult argument '" #__r "' %p in %s", \
        (__r), __func__ \
        );

//   Assertions for ObjC instances


#define ASSERT_VALID_ARCOBJECT_INSTANCE(__instance)

#define ASSERT_EXACT_CLASS_MEMBERSHIP(__instance,__classname) \
do { \
    if (    (__instance) \
         && ! [(__instance) isMemberOfClass: [__classname class] ] \
       ) \
        ART_ERRORHANDLING_FATAL_ERROR( \
            "instance %s %p is not a member of class " #__classname \
            ,   [ (__instance) cStringClassName ] \
            ,   (__instance) \
            ); \
} \
while (0)

#define ASSERT_CLASS_OR_SUBCLASS_MEMBERSHIP(__instance,__classname) \
do { \
    if (    (__instance) \
         && ! [ [ (__instance) class ] isSubclassOfClass \
                    : [__classname class] ] \
       ) \
        ART_ERRORHANDLING_FATAL_ERROR( \
            "instance %s %p is not a member of class " #__classname \
            ", or one of its subclasses"\
            ,   [ (__instance) cStringClassName ] \
            ,   (__instance) \
            ); \
} \
while (0)

#define ASSERT_PROTOCOL_SUPPORT(__instance,__arprotocol) \
do { \
    if (     (__instance) \
        && ! [ (__instance) conformsToArProtocol: \
                 ARPROTOCOL(__arprotocol) ] ) \
        ART_ERRORHANDLING_FATAL_ERROR( \
            "instance %s %p does not conform to the " \
            #__arprotocol " protocol"\
            ,   [ (__instance) cStringClassName ] \
            ,   (__instance) \
            ); \
} \
while (0);


#define ASSERT_LIGHT_SUBSYSTEM_IN_POLARISATION_MODE \
do { \
    if ( ! LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
        ART_ERRORHANDLING_FATAL_ERROR( \
            "this code section must not be executed if ART is not in "\
            "polarisation mode" \
            ); \
} \
while (0);

#define ASSERT_LIGHT_SUBSYSTEM_NOT_IN_POLARISATION_MODE \
do { \
    if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE ) \
        ART_ERRORHANDLING_FATAL_ERROR( \
            "this code section must not be executed if ART is in "\
            "polarisation mode" \
            ); \
} \
while (0);

#define ASSERT_VALID_ARNGRAPHTRAVERSAL(__t) \
do { \
    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(ARNGT_VOLUME_MATERIAL(__t),ArpVolumeMaterial); \
    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(ARNGT_SURFACE_MATERIAL(__t),ArpSurfaceMaterial); \
    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(ARNGT_TRAFO(__t),ArpTrafo3D); \
    ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK(ARNGT_VERTICES(__t),ArpVertices); \
} \
while (0);

#define ASSERT_ALLOCATED_GENERIC_ARGUMENT(__f, __t) \
if ( (__f) == 0 ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "unallocated " #__t " argument '" #__f "' %p in %s", \
        (__f), __func__ \
        );

#define ASSERT_ALLOCATED_VARIABLE(__f, __t) \
if ( (__f) == 0 ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "unallocated " #__t " variable '" #__f "' %p in %s", \
        (__f), __func__ \
        );

#define ASSERT_INTEGER_WITHIN_ISR_CHANNEL_RANGE(__i) \
if ( (__i) < 0  || (__i) > spc_channels( art_gv ) ) \
    ART_ERRORHANDLING_FATAL_ERROR( \
        "ISR channel index '" #__i "' has value %d " \
        "outside ISR bounds (0-%d) in %s", \
        (__i), spc_channels( art_gv ), __func__ \
        );

#endif

#endif /* _ART_FOUNDATION_ASSERTION_MACROS_H_ */
/* ======================================================================== */
