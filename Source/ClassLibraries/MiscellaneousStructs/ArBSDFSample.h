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

#include "ART_Foundation.h"

ART_MODULE_INTERFACE(ArBSDFSample)

#import "ArDifferentialGeometry.h"
#import "ArDirectionCosine.h"
#import "ArPDFValue.h"


/* ---------------------------------------------------------------------------

    'ArBSDFSampleType' enum

        This enum is used to describe the properties of a given BSDF sample.
        Some of this information is used by the ray tracer, and some by the
        path tracer to handle certain special cases of BSDFs more efficiently.
        The meaning of the entries is as follows:

        'arbsdfsample_general':    Generic BSDF sample, no special
                                   properties.

        'arbsdfsample_refraction': Refraction sample. If this bit is not
                                   set, we are looking at a reflection
                                   sample.

        'arbsdfsample_specular':   Perfectly specular (== mirror) reflection.

        'arbsdfsample_glossy':     Glossy reflection.

        'arbsdfsample_monochrome': Monochrome BSDF sample; used on
                                   dispersive interfaces

------------------------------------------------------------------------aw- */

typedef enum ArBSDFSampleType
{
    arbsdfsample_general    = 0x00,
    arbsdfsample_refraction = 0x01,
    arbsdfsample_specular   = 0x02,
    arbsdfsample_glossy     = 0x04,
    arbsdfsample_monochrome = 0x08,
}
ArBSDFSampleType;

/* ---------------------------------------------------------------------------

    'ArBSDFSample' struct

        Describes an individual reflection/refraction of an incident ray from
        a surface, and provides facilities for being part of a BSDF sample
        list.

        The concept behind BSDF evaluation is that the surface model
        used to describe the appearance of a given object may be aggregate
        in nature, and therefore may return a set of n separate reflections
        even if the user only requested a single reflection. Another case
        where this occcurs are realistically modelled transparent interfaces:
        these always generate at least one reflection and one refraction,
        and it is up to the rendering algorithm to choose one (or both) of
        them for further evaluation.

        All such individual reflections are gathered into a linked list which
        is referred to as the 'BSDF sample list' (BSL); each reflection
        contains a pointer to the next entry in the list. BSLs are managed by
        a set of functions which can be found further on in this header; one
        should never directly enter or delete entries in a BSL.

        If an individual reflection can be meaningfully subdivided into
        wavelength-dependent components with different outgoing directions
        (as it is e.g. the case at dispersive interfaces in crystal or glass
        objects) it may also contain a 'component BSDF list' (CBSL).
        This CBSL is a complete BSDF sample list that contains all different
        spectral components which the surface model generated; in addition to
        that, a reflection which contains such a CBSL is assumed to be the
        aggregate of the components in this list.

        This leaves the choice of whether to use the wavelength-dependent
        information to the renderer; it can either follow one (or all) of
        the wavelength-dependent components, or just use the average
        information contained in the parent reflection and ignore the
        components.

        'next':                  Pointer to the next reflection in the
                                 reflection list.

        'componentList':         Pointer to the component reflection list for
                                 dispersive reflections.

        'sampleProbability':     Value of the parent PDF for this reflection
                                 direction. This is specific to the BRDF which
                                 generates the reflection, and is filled in by
                                 the surface model code at the time the
                                 reflection is created.

        'selectionProbability':  The relative selection probability of this
                                 reflection in the context of all other hits
                                 in a given hitlist (i.e. "given the sum of
                                 all generation probabilities in a BSL,
                                 how many percent does this one contribute?").
                                 This field can only be filled in by a
                                 hitlist post-processing function - such as
                                 'arbsdfsamplelist_compute_percentiles' -
                                 after the complete hitlist has been
                                 assembled.

        'percentile':            Selection probability percentile within the
                                 hitlist for the given hit. This is
                                 precomputed at the same time as the
                                 previous field, so that a hit can be
                                 selected by choosing a single random value
                                 and iterating through the hitlist until
                                 percentile >= random value. Used by the
                                 path tracers.

        'attenuation':           Attenuation to which incident light is
                                 subjected when it is reflected in the direction
                                 given by 'dir'.

        'averageAttenuation':    TODO: describe

        'dir':                   Outgoing direction of the reflection.

        'type':                  Type of reflection; see the enum description
                                 above for possible values.

------------------------------------------------------------------------aw- */

typedef struct ArBSDFSample
{
    struct ArBSDFSample  * next;
    ArPDFValue             probability;
    double                 selectionProbability;
    double                 percentile;
    ArAttenuationSample  * attenuation;
    ArDirectionCosine      directionCosine;
    ArBSDFSampleType       type;
}
ArBSDFSample;

//   main accessor macros

#define ARBSDFSAMPLE_NEXT(__s)              (__s).next

#define ARBSDFSAMPLE_ATTENUATION(__s)       (__s).attenuation
#define ARBSDFSAMPLE_DIRECTION_COSINE(__s)  (__s).directionCosine
#define ARBSDFSAMPLE_TYPE(__s)              (__s).type

//   probability accessor macros

#define ARBSDFSAMPLE_PROBABILITY(__s)       (__s).probability
#define ARBSDFSAMPLE_SELECTION_PROBABILITY(__s) \
                                            (__s).selectionProbability
#define ARBSDFSAMPLE_PERCENTILE(__s)        (__s).percentile

//   hitdirection component accessor macros

#define ARBSDFSAMPLE_VECTOR(__s) \
    ARDIRECTIONCOSINE_VECTOR(ARBSDFSAMPLE_DIRECTION_COSINE(__s))
#define ARBSDFSAMPLE_COSINE(__s) \
    ARDIRECTIONCOSINE_COSINE(ARBSDFSAMPLE_DIRECTION_COSINE(__s))

//   prefabricated type queries

#define ARBSDFSAMPLE_IS_SPECULAR_REFRACTION(__refl) \
    (    ( ARBSDFSAMPLE_TYPE(__refl) & arbsdfsample_specular ) \
      && ( ARBSDFSAMPLE_TYPE(__refl) & arbsdfsample_refraction ) )
#define ARBSDFSAMPLE_IS_SPECULAR_REFLECTION(__refl) \
    (      ( ARBSDFSAMPLE_TYPE(__refl) & arbsdfsample_specular ) \
      && ! ( ARBSDFSAMPLE_TYPE(__refl) & arbsdfsample_refraction ) )
#define ARBSDFSAMPLE_IS_GLOSSY(__refl) \
    ( ARBSDFSAMPLE_TYPE(__refl) & arbsdfsample_glossy )

int arbsdfsample_s_valid(
        ART_GV        * art_gv,
        ArBSDFSample  * sample
        );


/* ---------------------------------------------------------------------------

    'ArBSDFSampleList' struct

        Wrapper struct for reflection lists. At the moment this just contains
        a pointer to the first reflection, but might include flags - such as
        information on whether the list has already been processed using one
        of the post-processing functions provided below - in the future.

        Similar to the RL entries themselves, reflection lists should only be
        manipulated (created, expanded, deleted) through the functions
        provided below.

-----------------------------------------------------------------------aw-- */

typedef struct ArBSDFSampleList
{
    struct ArBSDFSample * head;
}
ArBSDFSampleList;


#define ARBSDFSAMPLELIST_EMPTY          ((ArBSDFSampleList){ 0 })
#define ARBSDFSAMPLELIST_HEAD(__rl)     (__rl).head

/* ---------------------------------------------------------------------------

    'ArBSDFSampleFreelists' struct

        Since reflections and (especially in the case of dispersive
        interfaces) whole reflection lists are dynamically created and
        destroyed at a very high rate during rendering, all manipulations
        require the presence of a freelist data structure. Such freelists
        maintain a list of preallocated structures which are re-used on
        demand; this avoids dynamic allocation of each and every instance of
        reflections and RLs, which apart from a speed increase also
        considerably lessens heap fragmentation.

        For reflections and RLs we provide a special structure which
        contains the two freelists needed in this context packaged together;
        the main reason for this is probably just code aesthetics...  :-)

-----------------------------------------------------------------------aw-- */

typedef struct ArBSDFSampleFreelists
{
    ArFreelist  bsdfSampleListFreelist;
    ArFreelist  bsdfSampleFreelist;
}
ArBSDFSampleFreelists;

#define ARBSDFSAMPLELIST_FREELIST(__sfl)    (__sfl).bsdfSampleListFreelist
#define ARBSDFSAMPLE_FREELIST(__sfl)        (__sfl).bsdfSampleFreelist

void arbsdfsamplefreelists_alloc(
        ART_GV                 * art_gv,
        ArBSDFSampleFreelists  * sfl,
        int                      initialSize
        );

void arbsdfsamplefreelists_free_contents(
        ArBSDFSampleFreelists  * sfl
        );

ArBSDFSampleList * arbsdfsamplelist_alloc(
        ArBSDFSampleFreelists  * sfl
        );

void arbsdfsamplelist_fast_free(
        ArBSDFSampleList       * sl,
        ArBSDFSampleFreelists  * sfl
        );

void arbsdfsamplelist_free_contents(
        ArBSDFSampleList       * sl,
        ArBSDFSampleFreelists  * sfl
        );

ArBSDFSample * arbsdfsample_alloc(
        ART_GV  * art_gv
        );

void arbsdfsample_free(
        ART_GV        * art_gv,
        ArBSDFSample  * sample
        );

ArBSDFSample * arbsdfsample_alloc_from_freelist(
        ArBSDFSampleFreelists  * sfl
        );

void arbsdfsample_init(
        const ART_GV               * art_gv,
              ArBSDFSample         * s,
        const Vec3D                * vector,
              double                 cosine,
        const ArAttenuationSample  * attenuationSample
        );

void arbsdfsample_fast_free_to_freelist(
        ArBSDFSample           * s,
        ArBSDFSampleFreelists  * sfl
        );

void arbsdfsample_free_to_freelist(
        ArBSDFSample           * s,
        ArBSDFSampleFreelists  * sfl
        );

void arbsdfsamplelist_push(
        ArBSDFSampleList  * l,
        ArBSDFSample      * s
        );

ArBSDFSample * arbsdfsample_pop(
        ArBSDFSample           * s,
        ArBSDFSampleFreelists  * sfl
        );


/* ---------------------------------------------------------------------------

    'arbsdfsamplelist_add_and_filter_special'
    'arbsdfsamplelist_add_special'

        The idea behind these two functions is that a reflection list might
        contain a number of different perfect reflections or refractions,
        i.e. reflections with 'arbsdfsample_specular_reflection' or
        'arbsdfsample_specular_refraction' flags set.

        Aggregate surface models (such as ArnGeneralSurfaceMaterial or
        ArnMappedSurfaceMaterial) are the source for such reflection lists with
        duplicates, and ought to combine all these "special" reflections into
        two distinct entities (one refraction, and one reflection) for reasons
        of efficiency. The first function extracts any such reflections from
        a given list and adds them to a "scratchpad" data structure, and
        the second adds this scratchpad to a given destination list.

        The constant ARBSDFSAMPLE_NUMBER_OF_SPECIAL_CASES is currently set
        to 2; if any other "extractable" special cases are added in the
        future (unlikely but possible) changing of this value - and the line
        below it - would be all that is needed to adapt the client code to
        this change.

------------------------------------------------------------------------aw- */

#define ARBSDFSAMPLE_NUMBER_OF_SPECIAL_CASES    2
#define ARBSDFSAMPLE_EMPTY_SPECIAL_SCRATCHPAD   { 0, 0 }

void arbsdfsamplelist_add_and_filter_special(
        ART_GV                  * art_gv,
        ArBSDFSampleList        * srcList,
        double                    weight,
        ArBSDFSampleList        * dstList,
        ArBSDFSample           ** specialArray,
        ArBSDFSampleFreelists   * rfl
        );

void arbsdfsamplelist_add_special(
        ArBSDFSampleList   * list,
        ArBSDFSample      ** sp
        );


/* ---------------------------------------------------------------------------

    'arbsdfsamplelist_compute_percentiles'

        Computes and inserts the correct percentile generation probability
        values into the elements of a reflection list. This can only be done
        after the entire reflection list has been assembled, so a
        post-processing function like this is needed.

        Note that this kind of post-processing is only needed in certain
        circumstances; a plain Whitted raytracer does not need it, and a path
        tracer only if it does not allow forking of paths at interfaces which
        return more than one reflection. See the source of ArnPathTracer for
        details.

------------------------------------------------------------------------aw- */

void arbsdfsamplelist_compute_percentiles(
        ART_GV            * art_gv,
        ArBSDFSampleList  * list
        );


// ===========================================================================

