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

ART_MODULE_INTERFACE(ArcRandomGenerator)

/* ---------------------------------------------------------------------------

    OVERALL NOTICE
    ==============

    See the comments in ArpRandomGenerator.h for an explanation of the
    functionality found in the classes which are defined here!

--------------------------------------------------------------------------- */


#import "ArpRandomGenerator.h"


/* ---------------------------------------------------------------------------

    ArcRandomGenerator class
    ------------------------

    This superclass to the other random number generators implements just one
    method, a class method which returns a new random generator instance of
    appropriate type to the caller. This functionality is provided to
    keep the "if type = foo ..." cascade in one place and to faciliate
    changes to the actual class selection mechanism.

    Use the provided macro for a call like

    newRandGen = ARCRANDOMGENERATOR_NEW(type, max_samples, reporter);

    whenever you want to instantiate a random generator object of variable
    type. (reporter can be NULL, if you don't want anything to be printed in
    the log file.)

    Note: as you can easily figure out, there is actually no compelling
          technical reason why the empty shell of the ArcRandomGenerator
          class is a superclass of the real generator classes.
          This was done purely for aesthetical reasons w/r to OOP; it does
          no harm but looks nice...  :-)

--------------------------------------------------------------------------- */


#define ARCRANDOMGENERATOR_NEW(__type,__samples, __reporter) \
newARTRandomGenerator( \
    art_gv, \
    (__type), \
    (__samples),  \
    (__reporter) \
    )

@interface ArcRandomGenerator
        : ArcObject
{
    ArcObject <ArpReporter>  * reporter;
}

- (id) initWithReporter
        : (ArcObject <ArpReporter> *) newReporter
        ;

@end

ArcRandomGenerator <ArpRandomGenerator> * newARTRandomGenerator(
        ART_GV                   * new_art_gv,
        ArRandomValueGeneration    type,
        int                        newMaxNumberOfSamples,
        ArcObject <ArpReporter>  * newReporter
        );

/* ---------------------------------------------------------------------------

    ArcNR0RandomGenerator, ArcNR1RandomGenerator, ArcNR2RandomGenerator,
    ArcFastRandomGenerator, ArcKnuthRandomGenerator classes
    --------------------------------------------------------------------

    Encapsulated states of various random generators from the book
    "Numerical Recipes in C".

--------------------------------------------------------------------------- */


@interface ArcNR0RandomGenerator
        : ArcRandomGenerator
        < ArpRandomGenerator >
{
    ArRandomNR0  nr0State;
}

@end

@interface ArcNR1RandomGenerator
        : ArcRandomGenerator
        < ArpRandomGenerator >
{
    ArRandomNR1  nr1State;
}

@end

@interface ArcNR2RandomGenerator
        : ArcRandomGenerator
        < ArpRandomGenerator >
{
    ArRandomNR2  nr2State;
}

@end

/* ---------------------------------------------------------------------------

    ArcHaltonRandomGenerator class
    ------------------------------

    Encapsulated state of Halton type pseudorandom generators.

    This class is derived from ArcNR2RandomGenerator because it will
    provide NR2 numbers instead of very high order Halton sequences.
    (GZ:correct?)
------------------------------------------------------------------aw/gz---- */

@interface ArcHaltonRandomGenerator
        : ArcNR2RandomGenerator
        < ArpRandomGenerator >
{
    int       maxPrime, currentPrime;
    double  * haltonState;
    double  * inverseBase;
}

- (id) init
        : (unsigned int) newMaxNumberOfSamples
        ;

- (id) initWithReporter
        : (ArcObject <ArpReporter> *) newReporter
        : (unsigned int) newMaxNumberOfSamples
        ;

@end

/* ---------------------------------------------------------------------------

   ArcGaussianRandomGenerator
   --------------------------

   A gaussian random number generator with median 0 and variance 1

----------------------------------------------------------------------------*/

@interface ArcGaussianRandomGenerator
        : ArcRandomGenerator <ArpRandomGenerator>
{
    int     iset;
    double  gset;

    id <ArpRandomGenerator>  uniform_generator;
}

@end


/* ---------------------------------------------------------------------------

   ArcEfficientRandomGenerator
   ----------------------------

   Random Number Generator following Eurographics 2002 paper
   "Efficient Multidimensional Sampling" by Thomas Kollig, Alexander Keller

   Note that (t, m, s)-nets should best be used entirely, so call this
   sequence with a newTmsNetSize of 128/256/512, if possible, or
   slightly lower (125/250/500 as obvious candidates), NOT slightly
   higher (150/300/1200)!

   Background:
   -----------

   The original paper assumes (without telling the reader) that the
   (t, m, s) net is used in its entirety.  In case only e.g. 1/4 of
   the requested numbers are used, this results in samples that come
   from a contiguous region of 1/4 of the domain, because the X values
   are generated sequentially along the X axis.  THIS IS TRUE EVEN
   WHEN THE VALUES ARE SCRAMBLED!  To avoid this, we use an array with
   indices which get shuffled (not scrambled), where we simply use
   Knuth's Shuffling algorithm here. The shuffling is redone each time
   reInitializeWith: is called.  Scrambling only leads to the
   generation of different (t, m, s) nets from a few (0, 1) sequences,
   however, only entire blocks of size 2^-n may get exchanged,
   recursively, so in a series 0..7 you can never exchange e.g. just 2
   and 6.

   The structs ArEffRNGseq1State and ArEffRNGseq2State hold data for
   1-dimensional and 2-dimensional sequences.

   UInt32 r  holds random scramble value for X coordinate
   UInt32 s  holds random scramble value for RI_LP (Y coordinate)
   UInt32 i  current index
   UInt32 *shuffledIndices  points to array of size tmsNetSize,
                            so that in case less than tmsNetSize samples are
                            taken,
                            the values still come from all over the domain
   The following are only needed for debugging. For an optimized build
   (when ? ;-),
   we will omit any action with these. Define NDEBUG_RANDOM_GENERATOR to
   omit them. When the appropriate code is put to compilable state, this
   macro can be replaced with the usual system-wide NDEBUG one.
   UInt32 max    maximum index (never larger than tmsNetSize)
   UInt32 total  total count of numbers from this series (over all pixels)

---------------------------------------------------------------------gz-ip--*/

//#define NDEBUG_RANDOM_GENERATOR

typedef struct ArEffRNGseq1State {
  UInt32 r;
  UInt32 i;
  UInt32 *shuffledIndices;
#ifndef NDEBUG_RANDOM_GENERATOR
  UInt32 max;
  UInt32 total;
#endif
}ArEffRNGseq1State ;

typedef struct ArEffRNGseq2State {
  UInt32 r;
  UInt32 s;
  UInt32 i;
  UInt32 *shuffledIndices;
#ifndef NDEBUG_RANDOM_GENERATOR
  UInt32 max;
  UInt32 total;
#endif
}
ArEffRNGseq2State;

// (0, m, 2) net will have ARCEFFICIENTRANDOMGENERATOR_DEFAULTNETSIZE
// entries if not requested with different size.
#define ARCEFFICIENTRANDOMGENERATOR_DEFAULTNETSIZE 64
// Initially, the INITIALNUMBEROFSEQUENCES of 1-dim and 2-dim sequences
//each are created. On demand,
// these numbers are doubled automagically.
#define ARCEFFICIENTRANDOMGENERATOR_INITIALNUMBEROFSEQUENCES 32

@interface ArcEfficientRandomGenerator
        : ArcRandomGenerator
        < ArpRandomGenerator >
{
  unsigned short rseed48[3]; // keeps seed for drand48() type functions.
  UInt32 tmsNetSize;
  ArEffRNGseq1State *state1; // "autogrowing" arrays
  ArEffRNGseq2State *state2;
  UInt32 current1, current2; // series indices into state1, state2
  UInt32 size1, size2;       // sizes of state1, state2
#ifndef NDEBUG_RANDOM_GENERATOR
  UInt32 maxused1, maxused2; // index of highest used series
#endif
}

// default constructor: newTmsNetSize should be a power of 2, 64 seems
// reasonable, if not, will be increased to next larger power of 2.
- (id) init
        : (unsigned int) newTmsNetSize
        ;

- (id) initWithReporter
        : (ArcObject <ArpReporter> *) newReporter
        : (unsigned int) newTmsNetSize
        ;


// in case sequences are used up, create some new ones.
// (will be called internally)
- (id) _createMoreSequencesSingle: (BOOL) moreSingle
                      Double: (BOOL) moreDouble

  ;
// shuffle TMS net access sequence to avoid problems with partially
// called nets
- (id) _shuffleIndices;

@end

// ===========================================================================
