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

#ifndef _ARPRANDOMGENERATOR_H_
#define _ARPRANDOMGENERATOR_H_

#include "ART_Foundation.h"

ART_MODULE_INTERFACE(ArpRandomGenerator)

/* ---------------------------------------------------------------------------

    ArRandomValueGeneration enum

    This enumeration is used to determine two properties of random value
    generation, namely the used algorithm and the sampling strategy. The
    first is directly linked to the generator objects, and the second is used
    by the application that uses the values from the generator.

    Ideally, these two properties would perhaps use separate enums, but ART
    is baroque enough already, and the two *are* linked, so this stays the
    way it is until further notice (especially since one "random value
    generation" parameter in the renderers is plenty).

    a) Random number generation algorithm

    The first property (encoded in the 0x00.. part of the integer) determines
    the type of random generator to use. The key choice is between random
    and pseudorandom schemes. Although they are considerably less useful
    for rendering purposes than pseudorandom generators, there is a choice
    of three random methods (nr0-2) to one pseudo (halton). All the random
    generators are those defined in ArRandom.h and taken from the
    book "Numerical Recipes in C".

    Since the whole point of having the possibility to choose RGs is the
    ability to measure the performance and utility of different RGs in the
    context of a renderer, expect this list to grow in the future.

    Each entry in this list is characterized by one of the flags
    arrandomvaluegeneration_random or arrandomvaluegeneration_pseudorandom
    which can be used to check for the global type of generator algorithm.

    b) Sampling strategy

    The second property (encoded in the 0x0.00 part of the integer) offers
    the possibility to choose a sampling scheme. The possible values are
    "plain" (i.e. no special strategy), "stratified" (used in (t, m, s) nets
    in the "efficient" generator of Keller)
    and "systematic" (identical sampling sequences in all pixels, a.k.a. grid
    shifting, which is not being used either... 8-)

------------------------------------------------------------------------aw- */

typedef enum ArRandomValueGeneration
{
    arrandomvaluegeneration_random               = 0x1000,
    arrandomvaluegeneration_pseudorandom         = 0x2000,

    arrandomvaluegeneration_nr0                 = 0x1000,
    arrandomvaluegeneration_nr1                  = 0x1001,
    arrandomvaluegeneration_nr2                  = 0x1002,
    arrandomvaluegeneration_gaussian             = 0x1004,

    arrandomvaluegeneration_halton               = 0x2010,
    arrandomvaluegeneration_efficient            = 0x2102,

    arrandomvaluegeneration_plain_sampling       = 0x0000,
    arrandomvaluegeneration_stratified_sampling  = 0x0100,
    arrandomvaluegeneration_systematic_sampling  = 0x0200
}
ArRandomValueGeneration;


/* ---------------------------------------------------------------------------

    'ArSequenceID' typedef

    This is the data type used to describe individual random number sequences
    produced by the random generators in ART.

    Calling code should not manipulate instances of this type. It should only
    store them, and use them to request specific sequences from the
    generators - but NOT assume this type to be an integer!

------------------------------------------------------------------------aw- */

typedef UInt64 ArSequenceID;


/* ---------------------------------------------------------------------------

    'ArpRandomGenerator' protocol

    Describes the abilities of random generator objects. Its functionality
    is a bit hybrid since it has to be a front for both "genuine" random
    generators and pseudorandom sequences.

    The noticeable peculiarity which arises from this hybridization is the
    emphasis which is being put on the concept of a sequence. While genuine
    random values are - by definition - totally uncorrelated and can be used
    in any order for any purpose, the use of sequences of pseudorandom
    numbers is typically restricted in this respect.

    An example would be that one must not use the same Halton or
    (t, s) sequence for more than one decision during a singe ray traversal
    in a scene.

    Therefore the user has to be able to choose which generator sequence he
    wishes to use for the random value he is requesting. For genuine random
    generators, this choice can safely be ignored internally.

    However, during the rendering process, the specific sequence which is being
    requested usually does not matter as much as the fact that it is different
    from those which were used before. So as a convenience, the random
    generators are assumed to remember the sequences they have already used,
    and as their default behaviour offer to generate values from a
    "new" sequence.

    For those cases where longer sequences of random decisions of the same
    type have to be generated one can also "mark the current level" in the
    decision tree and return to it.

    The user still has the option of obtaining information on which sequence
    the random values he received belong to, or to explicitly generate random
    values from a specific sequence. However, these options are not very
    frequently used.

    For purposes of logging and debugging, all random generators implement
    reporter methods.

------------------------------------------------------------------------aw- */


@protocol ArpRandomGenerator

/* ---------------------------------------------------------------------------

    'getValuesFromNewSequences'

    Returns a pair of random values from two new sequences.

    For rendering purposes, one very often not only needs a single random
    value, but rather an uncorrelated pair of them, e.g. for 2D sampling in
    a pixel or similar tasks. Since certain pseudorandom generators are able
    to specifically generate superior sampling point sequences for
    2D distributions, and since having such a function also makes the calling
    code more concise, this function was added.

------------------------------------------------------------------------aw- */

- (void) getValuesFromNewSequences
        : (double *) value_0
        : (double *) value_1
        ;


/* ---------------------------------------------------------------------------

    'valueFromNewSequence'
    'valueFromNewSequence' (with ArSequenceID pointer argument)
    'valueFromSequence' (with ArSequenceID argument)

    Returns a single random value from a new sequence, or in the third case,
    specific sequence identified by sequence ID. The second form of
    this method is identical to the first, but also returns the sequence ID
    used for the generation of the random number in the integer argument.

------------------------------------------------------------------------aw- */

- (double) valueFromNewSequence
        ;

- (double) valueFromNewSequence
        : (ArSequenceID *) usedSequence
        ;

- (double) valueFromSequence
        : (ArSequenceID) sequenceToUse
        ;


/* ---------------------------------------------------------------------------

    'reInitializeWith'

    Initalizes the random generator on the basis of a specific 32-bit pattern.
    This is equivalent to specifying a random seed, and has an effect for all
    types of generators. The slightly non-obvious type of UInt32 is used
    because

    a) 32-bit hash values are a good way to derive generator offsets from
       pixel positions (via CRC32, or similar) and

    b) the various generators in ART all have different states (and hence
       "native" seed values), and some neutral compromise had to be taken.

------------------------------------------------------------------------aw- */

- (void) reInitializeWith
        : (UInt32) uInt32Value
        ;


/* ---------------------------------------------------------------------------

    'resetSequenceIDs'
    'currentSequenceID'
    'setCurrentSequenceID'

    As with every aspect of RG functionality that deals with sequence IDs,
    these methods only have an effect for pseudorandom generators:

    When doing e.g. path tracing with one of these, one has to avoid using
    the same sequence more than once during a path descent into the scene.
    However, one should use the same sequence of sequences (har! :-) for
    neighbouring rays.

    These methods tell the generator to either completely reset its internal
    reckoning of which sequences have been used, query the current sequence ID,
    or set it to a specific sequence ID.

------------------------------------------------------------------------aw- */

- (void) resetSequenceIDs
        ;

- (ArSequenceID) currentSequenceID
        ;

- (void) setCurrentSequenceID
        : (ArSequenceID) sequenceID
        ;


/* ---------------------------------------------------------------------------

    'reporter'
    'setReporter'

    Returns or sets the random generator's reporter object.

------------------------------------------------------------------------aw- */

- (ArcObject <ArpReporter> *) reporter
        ;

- (void) setReporter
        : (ArcObject <ArpReporter> *) newReporter
        ;

@end

#endif // _ARPRANDOMGENERATOR_H_

// ===========================================================================
