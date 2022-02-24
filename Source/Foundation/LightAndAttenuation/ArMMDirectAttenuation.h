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

#ifndef _ART_FOUNDATION_LIGHTANDATTENUATION_ARMMDIRECTATTENUATION_H_
#define _ART_FOUNDATION_LIGHTANDATTENUATION_ARMMDIRECTATTENUATION_H_

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArMMDirectAttenuation)

#include "ART_Foundation_ColourAndSpectra.h"

#include "ArLightAndAttenuationSubsystemManagement.h"

ART_LIGHT_AND_ATTENUATION_MODULE_INTERFACE(ArMMDirectAttenuation)

#include "ArStokesVector.h"
#include "ArMuellerMatrix.h"
#include "ArCrosstalk.h"
#include "ArReferenceFrame.h"
#include "ArSVLight.h"
#include "ArLightIntensity.h"

#include "AttenuationInterfaceMacros.h"

/* ---------------------------------------------------------------------------

    'ArMMProperty' enum

    Bit-field enum used to describe the properties of individual Mueller
    matrices for optimisation purposes. Encodes the following properties:

    - whether the MM is rotationally invariant
    - whether the MM is a depolariser

    Depolarisers are rotationally invariant, but not all rotationally invariant
    matrices are depolarisers.

    Rotationally invariant matrices do not have to be rotated prior to
    concatenation, which saves considerable computational effort.

    For some operations (such as multiplication with a scalar), depolarisers
    also only require that the element a(0,0) be treated, so this flag allows
    some additional optimisations.

------------------------------------------------------------------------aw- */


typedef enum ArMMProperty
{
    armmproperty_general_matrix         = 0x00,
    armmproperty_rotationally_invariant = 0x01,
    armmproperty_depolariser            = 0x02
}
ArMMProperty;


/* ---------------------------------------------------------------------------

    'ArMMDirectAttenuation' struct

    Describes the attenuation of light by a single, nonfluorescent optical
    element which optionally can act as a polariser.

    The struct consists of four elements:

    - an enum which contains information on the validity and nature of the
      attenuation (e.g. whether the refframes are valid, see above)
    - a Mueller matrix which stores the actual attenuation values
    - and two reference frames, one for the beginning of the
      interaction described by this element, and one for the end.

    If only collinear elements are concatenated one only needs a single
    refframe, but there are also light-matter interactions - such as
    reflections from surfaces - which include a change in direction for the
    light ray which they attenuate, so both the old and new reference system
    have to be stored. For arbitrarily long attenuation chains all one has
    to remember are the first and the last frame, so two entries are sufficent.

    The geometry of the reference frames is as follows:

    (light) -> entry frame -> (attenuation) -> exit frame -> (eye)

------------------------------------------------------------------------aw- */


typedef struct ArMMDirectAttenuation
{
    ArMMProperty        properties;
    ArMuellerMatrix   * muellerMatrix;
    ArReferenceFrame    referenceFrameEntry;
    ArReferenceFrame    referenceFrameExit;
}
ArMMDirectAttenuation;


#define ARMMDIRECTATTENUATION_PROPERTIES(__a) \
    (__a).properties

#define ARMMDIRECTATTENUATION_MM(__a)         (__a).muellerMatrix
#define ARMMDIRECTATTENUATION_MM_I(__a,__i)   \
    ARMUELLER_M_I(*ARMMDIRECTATTENUATION_MM(__a),(__i))
#define ARMMDIRECTATTENUATION_MM_II(__a,__row,__col)   \
    ARMUELLER_M_II(*ARMMDIRECTATTENUATION_MM(__a),(__row),(__col))

#define ARMMDIRECTATTENUATION(__a)            ARMMDIRECTATTENUATION_MM_I(__a,0)

#define ARMMDIRECTATTENUATION_MM_IC(__a,__i,__c) \
   spc_si(art_gv,ARMMDIRECTATTENUATION_MM_I(__a,__i),(__c))

#define ARMMDIRECTATTENUATION_ENTRY_REFFRAME(__a)  (__a).referenceFrameEntry
#define ARMMDIRECTATTENUATION_ENTRY_REFFRAME_I(__a,__i) \
    ARREFFRAME_RF_I(ARMMDIRECTATTENUATION_ENTRY_REFFRAME(__a),(__i))

#define ARMMDIRECTATTENUATION_EXIT_REFFRAME(__a)  (__a).referenceFrameExit
#define ARMMDIRECTATTENUATION_EXIT_REFFRAME_I(__a,__i) \
    ARREFFRAME_RF_I(ARMMDIRECTATTENUATION_EXIT_REFFRAME(__a),(__i))

#define ARMMDIRECTATTENUATION_IS_GENERAL_MATRIX(__a)  \
    ( (__a).properties == armmproperty_general_matrix )

#define ARMMDIRECTATTENUATION_IS_ROTATIONALLY_INVARIANT(__a)  \
    ( (__a).properties & armmproperty_rotationally_invariant )

#define ARMMDIRECTATTENUATION_IS_DEPOLARISER(__a)  \
    ( (__a).properties & armmproperty_depolariser )

#define ARMMDIRECTATTENUATION_HAS_FULL_MATRIX(__a) \
    ( ! ARMMDIRECTATTENUATION_IS_DEPOLARISER(__a) )

#define ARMMDIRECTATTENUATION_SET_PROPERTY_GENERAL_MATRIX(__a)  \
    ( (__a).properties = armmproperty_general_matrix )

#define ARMMDIRECTATTENUATION_SET_PROPERTY_ROTATIONALLY_INVARIANT(__a)  \
    ( (__a).properties = armmproperty_rotationally_invariant )

#define ARMMDIRECTATTENUATION_SET_PROPERTY_DEPOLARISER(__a)  \
    ( (__a).properties = \
          armmproperty_depolariser  \
        | armmproperty_rotationally_invariant )

//   The following are just short versions of the canonical struct
//   access macros. ART code is convoluted enough as it is, and these
//   short versions do improve readability a bit.


#define ARMMDA_PROPERTIES           ARMMDIRECTATTENUATION_PROPERTIES

#define ARMMDA_MATRIX               ARMMDIRECTATTENUATION_MM
#define ARMMDA_MATRIX_I             ARMMDIRECTATTENUATION_MM_I
#define ARMMDA_MATRIX_II            ARMMDIRECTATTENUATION_MM_II

#define ARMMDA_REFFRAME_ENTRY       ARMMDIRECTATTENUATION_ENTRY_REFFRAME
#define ARMMDA_REFFRAME_ENTRY_I     ARMMDIRECTATTENUATION_ENTRY_REFFRAME_I
#define ARMMDA_REFFRAME_EXIT        ARMMDIRECTATTENUATION_EXIT_REFFRAME
#define ARMMDA_REFFRAME_EXIT_I      ARMMDIRECTATTENUATION_EXIT_REFFRAME_I

#define ARMMDA_IS_ROTATIONALLY_INVARIANT \
    ARMMDIRECTATTENUATION_IS_ROTATIONALLY_INVARIANT
#define ARMMDA_IS_POLARISER         ARMMDIRECTATTENUATION_IS_GENERAL_MATRIX
#define ARMMDA_IS_DEPOLARISER       ARMMDIRECTATTENUATION_IS_DEPOLARISER

CANONICAL_INTERFACE_FOR_ACT(
    ArMMDirectAttenuation,
    armmdirectattenuation,
    a,
    ArSVLight
    )

ADDITIONAL_INTERFACE_FOR_ACT(
    ArMMDirectAttenuation,
    armmdirectattenuation,
    ArSVLight
    )

#endif /* _ART_FOUNDATION_LIGHTANDATTENUATION_ARMMDIRECTATTENUATION_H_ */

// ===========================================================================
