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

#define ART_MODULE_NAME     ArcRandomGenerator

#include <err.h>
#import "ArcRandomGenerator.h"
#import "ArcGruenschlossHalton.h"

// for YES/NO:

// Define the following to get tons of debug messages for the
// EfficientRandomGenerator.
// most of its reporter printf's have been taken out, as they apparently
// cause spurious
// console output in case option -v is not given to artist.
//#define FULL_DIAGNOSTICS

// this define removes most of the debug messages
// define RANDOM_GENERATOR_DEBUG_PRINTFS

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY



ArcRandomGenerator <ArpRandomGenerator> * newARTRandomGenerator(
        ART_GV                   * new_art_gv,
        ArRandomValueGeneration    type,
        int                        newMaxNumberOfSamples,
        ArcObject <ArpReporter>  * newReporter
        )
{
    ArcRandomGenerator <ArpRandomGenerator>  * new_generator = 0;

    if ( type == arrandomvaluegeneration_nr0 )
    {
        new_generator =
            (ArcRandomGenerator <ArpRandomGenerator> *)
            [ ALLOC_OBJECT_AGV(new_art_gv,ArcNR0RandomGenerator) initWithReporter
                :   newReporter
                ];

        return (ArcRandomGenerator <ArpRandomGenerator> *) new_generator;
    }

    if ( type == arrandomvaluegeneration_nr1 )
    {
        new_generator =
            [ ALLOC_OBJECT_AGV(new_art_gv,ArcNR1RandomGenerator) initWithReporter
                :   newReporter
                ];

        return (ArcRandomGenerator <ArpRandomGenerator> *) new_generator;
    }

    if ( type == arrandomvaluegeneration_nr2 )
    {
        new_generator =
            [ ALLOC_OBJECT_AGV(new_art_gv,ArcNR2RandomGenerator) initWithReporter
                :   newReporter
                ];

        return (ArcRandomGenerator <ArpRandomGenerator> *) new_generator;
    }

    if ( type == arrandomvaluegeneration_halton )
    {
        new_generator =
            [ ALLOC_OBJECT_AGV(new_art_gv,ArcHaltonRandomGenerator)
                initWithReporter
                :   newReporter
                :   newMaxNumberOfSamples
                ];

        return (ArcRandomGenerator <ArpRandomGenerator> *) new_generator;
    }

    if ( type == arrandomvaluegeneration_gaussian )
    {
        new_generator =
            [ ALLOC_OBJECT_AGV(new_art_gv,ArcGaussianRandomGenerator) initWithReporter
                :   newReporter
                ];

        return (ArcRandomGenerator <ArpRandomGenerator> *) new_generator;
    }

    if ( type == arrandomvaluegeneration_efficient )
    {
        new_generator =
            [ ALLOC_OBJECT_AGV(new_art_gv,ArcEfficientRandomGenerator) initWithReporter
                :   newReporter
                :   newMaxNumberOfSamples
                ];

        return (ArcRandomGenerator <ArpRandomGenerator> *) new_generator;
    }

    return 0;
}


@implementation ArcRandomGenerator

- (ArcObject <ArpReporter> *) reporter
{
    return reporter;
}

- (void) setReporter
        : (ArcObject <ArpReporter> *) newReporter
{
    reporter = newReporter;
}

- (id) initWithReporter
        : (ArcObject <ArpReporter> *) newReporter
{
    self = [ super init ];

    if ( self )
    {
        reporter = newReporter;

        [ reporter printf
            : "Init of abstract ArcRandomGenerator, "
            "you should see a note on the concrete RG in the next line.\n" ];
    }
    
    return self;
}

- (void) dealloc
{
#ifdef FULL_DIAGNOSTICS
    [ reporter printf
        :   "Deallocation of %s\n"
        ,   [ self cStringClassName ] ];
#endif
    [ super dealloc ];
}

@end

@implementation ArcNR0RandomGenerator

- (ArcObject <ArpReporter> *) reporter
{
    return
        [ super reporter ];
}

- (void) setReporter
        : (ArcObject <ArpReporter> *) newReporter
{
    [ super setReporter: newReporter ];
}

- (id) init
{
    return
        [ self initWithReporter
            :   NULL
            ];
}

- (id) initWithReporter
        : (ArcObject <ArpReporter> *) newReporter
{
    self = [ super init ];

    if ( self )
    {
        reporter = newReporter;

        [ reporter printf
            :   "Using %s\n"
            ,   [ self cStringClassName ]
            ];

        long l = (long) ((RANDOM_UNIFORM(art_gv)-0.5) * ART_ULONG_MAX);

        arrandom_nr0_seed( & nr0State, l );
    }
    
    return self;
}

- (void) getValuesFromNewSequences
        : (double *) a
        : (double *) b
{
    *a = [ self valueFromNewSequence ];
    *b = [ self valueFromNewSequence ];
}

- (double) valueFromNewSequence
{
    return arrandom_nr0_double( & nr0State );
}

- (double) valueFromNewSequence
        : (ArSequenceID *) usedSequence
{
    *usedSequence = 0;

    return [ self valueFromNewSequence ];
}

- (double) valueFromSequence
        : (ArSequenceID) sequenceToUse
{
    return [ self valueFromNewSequence ];
}

- (void) reInitializeWith
        : (UInt32) uInt32Value
{
    long l = ( ART_ULONG_MAX / ART_UINT32_MAX ) * uInt32Value;
    arrandom_nr0_seed( & nr0State, l );
}

- (void) resetSequenceIDs
{
}

- (ArSequenceID) currentSequenceID
{
    return 0;
}

- (void) setCurrentSequenceID
        : (ArSequenceID) sequenceID
{
}

@end

@implementation ArcNR1RandomGenerator

- (ArcObject <ArpReporter> *) reporter
{
    return
        [ super reporter ];
}

- (void) setReporter
        : (ArcObject <ArpReporter> *) newReporter
{
    [ super setReporter: newReporter ];
}

- (id) init
{
    return
        [ self initWithReporter
            :   NULL
            ];
}

- (id) initWithReporter
        : (ArcObject <ArpReporter> *) newReporter
{
    self = [ super init ];
    
    if ( self )
    {
        reporter = newReporter;

        [ reporter printf
            :   "Using %s\n"
            ,   [ self cStringClassName ]
            ];

        long l = (long) ((RANDOM_UNIFORM(art_gv)-0.5) * ART_ULONG_MAX);

        arrandom_nr1_seed( & nr1State, l );
    }
    
    return self;
}

- (void) getValuesFromNewSequences
        : (double *) a
        : (double *) b
{
    *a = [ self valueFromNewSequence ];
    *b = [ self valueFromNewSequence ];
}

- (double) valueFromNewSequence
{
    return arrandom_nr1_double( & nr1State );
}

- (double) valueFromNewSequence
        : (ArSequenceID *) usedSequence
{
    *usedSequence = 0;

    return [ self valueFromNewSequence ];
}

- (double) valueFromSequence
        : (ArSequenceID) sequenceToUse
{
    return [ self valueFromNewSequence ];
}

- (void) reInitializeWith
        : (UInt32) uInt32Value
{
    long l = ( ART_ULONG_MAX / ART_UINT32_MAX ) * uInt32Value;
    arrandom_nr1_seed( & nr1State, l );
}

- (void) resetSequenceIDs
{
}

- (ArSequenceID) currentSequenceID
{
    return 0;
}

- (void) setCurrentSequenceID
        : (ArSequenceID) sequenceID
{
}

@end

@implementation ArcNR2RandomGenerator

- (ArcObject <ArpReporter> *) reporter
{
    return
        [ super reporter ];
}

- (void) setReporter
        : (ArcObject <ArpReporter> *) newReporter
{
    [ super setReporter: newReporter ];
}

- (id) init
{
    return
        [ self initWithReporter
            :   NULL
            ];
}

- (id) initWithReporter
        : (ArcObject <ArpReporter> *) newReporter
{
    self = [ super init ];

    if ( self )
    {
        reporter = newReporter;

        [ reporter printf
            :   "Using %s\n"
            ,   [ self cStringClassName ]
            ];

        long l = (long) ((RANDOM_UNIFORM(art_gv)-0.5) * ART_ULONG_MAX);

        arrandom_nr2_seed( & nr2State, l );
    }
    
    return self;
}

- (void) getValuesFromNewSequences
        : (double *) a
        : (double *) b
{
    *a = [ self valueFromNewSequence ];
    *b = [ self valueFromNewSequence ];
}

- (double) valueFromNewSequence
{
    return arrandom_nr2_double( & nr2State );
}

- (double) valueFromNewSequence
        : (ArSequenceID *) usedSequence
{
    *usedSequence = 0;

    return [ self valueFromNewSequence ];
}

- (double) valueFromSequence
        : (ArSequenceID) sequenceToUse
{
    return [ self valueFromNewSequence ];
}

- (void) reInitializeWith
        : (UInt32) uInt32Value
{
    long l = ( ART_ULONG_MAX / ART_UINT32_MAX ) * uInt32Value;
    arrandom_nr2_seed( & nr2State, l );
}

- (void) resetSequenceIDs
{
}

- (ArSequenceID) currentSequenceID
{
    return 0;
}

- (void) setCurrentSequenceID
        : (ArSequenceID) sequenceID
{
}

@end

@implementation ArcHaltonRandomGenerator

- (id) init
{
    return
        [ self initWithReporter
            :   NULL
            :   0
            ];
}

- (id) init
        : (unsigned int) newMaxNumberOfSamples
{
    return
        [ self initWithReporter
            :   NULL
            :   newMaxNumberOfSamples
            ];
}

- (id) initWithReporter
        : (ArcObject <ArpReporter> *) newReporter
        : (unsigned int) newMaxNumberOfSamples
{
    self =
        [ super initWithReporter
            :   newReporter
            ];
    
    if ( self )
    {
        maxPrime = 0;
        currentPrime = -1;

        do {}
        while ( prime_number( ++maxPrime ) < newMaxNumberOfSamples );

        maxPrime--;
        
        maxPrime = M_MIN( 42, maxPrime );

        if ( maxPrime > 0 )
        {
            [ reporter printf
                :   "ArcHaltonRandomGenerator: using %d prime numbers\n"
                ,   maxPrime ];
            haltonState = ALLOC_ARRAY( double, maxPrime );
            inverseBase = ALLOC_ARRAY( double, maxPrime );

            //   The state of the Halton generators is initialised
            //   via the normal ART system random generator, which
            //   is in turn dependent on the chosen global random seed.
            //   This is why there is no reference to the global
            //   randseed in here.

            for ( unsigned int i = 0; (int) i < maxPrime; i++ )
            {
                int  prime;

                prime = prime_number( i );
                inverseBase[ i ] = 1.0 / prime;
                haltonState[ i ] = RANDOM_UNIFORM( art_gv );
            }
        }
    }
    
    return self;
}

- (void) getValuesFromNewSequences
        : (double *) a
        : (double *) b
{
    *a = [ self valueFromNewSequence ];
    *b = [ self valueFromNewSequence ];
}

- (void) getValuesForDoubleSequence
        : (int) i
        : (double *) a
        : (double *) b
{
    *a = [ self valueFromSequence: i     ];
    *b = [ self valueFromSequence: i + 1 ];
}

- (double) valueFromNewSequence
{
    ArSequenceID  sequenceID;

    return [ self valueFromNewSequence: & sequenceID ];
}

- (double) valueFromNewSequence
        : (ArSequenceID *) usedSequence
{
    double  d;

    *usedSequence = ++currentPrime;

    d = [ self valueFromSequence: currentPrime ];

    return d;
}

- (double) valueFromSequence
        : (ArSequenceID) sequenceToUse
{
    double  quasiRnd = 0.0;

    if ( (int) sequenceToUse >= maxPrime )
        return [ super valueFromNewSequence ];

    quasiRnd = quasi_halton( inverseBase[ sequenceToUse ] ,
                             haltonState[ sequenceToUse ] );
    haltonState[ sequenceToUse ] = quasiRnd;

    return  quasiRnd;
}

- (void) reInitializeWith
        : (UInt32) uInt32Value
{
    int  i;

    [ super reInitializeWith: uInt32Value ];

    for ( i = 0; i < maxPrime; i++ )
        haltonState[ i ] = [ super valueFromNewSequence ];
}

- (void) resetSequenceIDs
{
    currentPrime = -1;
}

- (ArSequenceID) currentSequenceID
{
  return currentPrime;
}

- (void) setCurrentSequenceID
        : (ArSequenceID) sequenceID
{
    currentPrime = (int) sequenceID;
}

// repeated just for protocol completeness.
- (ArcObject <ArpReporter> *) reporter
{
  return reporter;
}
- (void) setReporter
        : (ArcObject <ArpReporter> *) newReporter
{
  reporter=newReporter;
}

- (void) dealloc
{
    FREE_ARRAY(haltonState);
    FREE_ARRAY(inverseBase);

    [ super dealloc ];
}

@end

@implementation ArcGaussianRandomGenerator

- (id) init
{
    return
        [ self initWithReporter
            :   NULL
            ];
}

- (id) initWithReporter
        : (ArcObject <ArpReporter> *) newReporter
{
    self = [ super init ];

    if ( self )
    {
      /* at the moment we always use a NR2 random number generator since
       * I do not want to break the overall initalization routine for
       * the generation of random number generators. A class for Transformation
       * number generators taking the uniform number generator as parameter
       * would definitely be more general */

        uniform_generator =
            ARCRANDOMGENERATOR_NEW(arrandomvaluegeneration_nr2, 16, newReporter);

        iset = 0;

        reporter = newReporter;

        [ reporter printf
            :   "Using %s\n"
            ,   [ self cStringClassName ]
            ];
    }
    
    return self;
}

- (void) getValuesFromNewSequences
        : (double *) a
        : (double *) b
{
    *a = [ self valueFromNewSequence ];
    *b = [ self valueFromNewSequence ];
}

- (void) getValuesForDoubleSequence
        : (int) i
        : (double *) a
        : (double *) b
{
    *a = [ self valueFromSequence: i     ];
    *b = [ self valueFromSequence: i + 1 ];
}


- (double) valueFromNewSequence
{
    double fac, rsq, v1, v2;

    if (iset == 0) {
      do {
          v1 = 2.0 * [uniform_generator valueFromNewSequence] - 1.0;
          v2 = 2.0 * [uniform_generator valueFromNewSequence] - 1.0;
          rsq = v1 * v1 + v2 * v2;
      } while(rsq >= 1.0 || rsq == 0.0);

      fac = sqrt(-2.0 * log(rsq) / rsq);

      gset = v1 * fac;
      iset = 1;

      return v2 * fac;

    } else {
        iset = 0;
        return gset;
    }

}

- (double) valueFromNewSequence
        : (ArSequenceID *) usedSequence
{
    *usedSequence = 0;

    return
        [ self valueFromNewSequence ];
}

- (double) valueFromSequence
        : (ArSequenceID) sequenceToUse
{
    return [self valueFromNewSequence];
}

- (void) reInitializeWith
        : (UInt32) uInt32Value
{
    [uniform_generator reInitializeWith : uInt32Value];
    iset = 0;
}

- (void) resetSequenceIDs
{
    [uniform_generator resetSequenceIDs];
}

- (ArSequenceID) currentSequenceID
{
    return [ uniform_generator currentSequenceID ];
}

- (void) setCurrentSequenceID
        : (ArSequenceID) sequenceID
{
    [ uniform_generator setCurrentSequenceID: sequenceID ];
}

// repeated just for protocol completeness.
- (ArcObject <ArpReporter> *) reporter
{
  return reporter;
}
- (void) setReporter
        : (ArcObject <ArpReporter> *) newReporter
{
  reporter=newReporter;
}

- (void) dealloc
{
    RELEASE_OBJECT(uniform_generator);

    [ super dealloc ];
}

@end


@implementation ArcEfficientRandomGenerator

- (id) init
{
    return
        [ self initWithReporter
            :   NULL
            :   ARCEFFICIENTRANDOMGENERATOR_DEFAULTNETSIZE ];
}

- (id) init
        : (unsigned int) newTmsNetSize
{
    return
        [ self initWithReporter
            :   NULL
            :   newTmsNetSize ];
}

- (id) initWithReporter
        : (ArcObject <ArpReporter> *) newReporter
        : (unsigned int) newTmsNetSize
{
    self  = [ super init ];
    
    if ( self )
    {
        reporter=newReporter;
#ifdef RANDOM_GENERATOR_DEBUG_PRINTFS
        [reporter printf: "Using %s\n", [self cStringClassName]];
#endif
        tmsNetSize=newTmsNetSize;
        // make sure tmsNetSize is (probably next higher) power of 2
        UInt16 logCnt=0;
        while ((tmsNetSize >>= 1) & 0xFFFF ) {
        logCnt++;
        }
        if ((1 << logCnt) == newTmsNetSize) {
        tmsNetSize=newTmsNetSize;
        }  else {
        tmsNetSize = 1 << (logCnt+1);
        }
        assert(tmsNetSize>0);
        //if (reporter)
#ifdef RANDOM_GENERATOR_DEBUG_PRINTFS
        [ reporter printf
            :   "ArcEfficientrandomGenerator: TMS net size requested: %ud, "
                "created: %ud\n"
            ,   newTmsNetSize
            ,   tmsNetSize ];
#endif

        // deterministically set the master random seed
        rseed48[0]=0x4D41; // MA
        rseed48[1]=0x5354; // ST  --> what else? ;-) [echo MASTER | od -tx1]
        rseed48[2]=0x4552; // ER

        current1=0;
        current2=0;
        maxused1=0;
        maxused2=0;

        // initialize with defined number of sequences each
        state1 =
            ALLOC_ARRAY_ZERO(
                ArEffRNGseq1State,
                ARCEFFICIENTRANDOMGENERATOR_INITIALNUMBEROFSEQUENCES );
        size1 = ARCEFFICIENTRANDOMGENERATOR_INITIALNUMBEROFSEQUENCES;
        state2 =
            ALLOC_ARRAY_ZERO(
                ArEffRNGseq2State,
                ARCEFFICIENTRANDOMGENERATOR_INITIALNUMBEROFSEQUENCES );
        size2 = ARCEFFICIENTRANDOMGENERATOR_INITIALNUMBEROFSEQUENCES;

        for ( unsigned int i = 0;
              i < ARCEFFICIENTRANDOMGENERATOR_INITIALNUMBEROFSEQUENCES;
              ++i )
        {
            state1[i].r=(UInt32) nrand48(rseed48);
            state1[i].shuffledIndices=ALLOC_ARRAY_ZERO(UInt32, tmsNetSize);
            state2[i].r=(UInt32) nrand48(rseed48);
            state2[i].s=(UInt32) nrand48(rseed48);
            state2[i].shuffledIndices=ALLOC_ARRAY_ZERO(UInt32, tmsNetSize);
        }
        [self _shuffleIndices];
    }
    
    return self;
}

// in case all sequences are used up, create some new ones.
//  (called internally)
- (id) _createMoreSequencesSingle: (BOOL) moreSingle
                      Double: (BOOL) moreDouble
{
    if (moreSingle){
    state1=REALLOC_ARRAY(state1, ArEffRNGseq1State, 2*size1);
    for (UInt32 i=size1; i<2*size1; i++) {
      state1[i].i=0;
      state1[i].r=nrand48(rseed48);
      state1[i].max=0;
      state1[i].total=0;
      state1[i].shuffledIndices=ALLOC_ARRAY_ZERO(UInt32, tmsNetSize);
    }
    size1 *= 2;
    }
    if (moreDouble){
    state2=REALLOC_ARRAY(state2, ArEffRNGseq2State, 2*size2);
    for (UInt32 i=size2; i<2*size2; i++) {
      state2[i].i=0;
      state2[i].r=nrand48(rseed48);
      state2[i].s=nrand48(rseed48);
      state2[i].max=0;
      state2[i].total=0;
      state2[i].shuffledIndices=ALLOC_ARRAY_ZERO(UInt32, tmsNetSize);
    }
    size2 *= 2;
    }
    #ifdef FULL_DIAGNOSTICS
    [ reporter printf
        :   "ArcEfficientrandomGenerator: Now %d 1-dim and %d 2-dim "
            "sequences.\n"
        ,   size1
        ,   size2 ];
    #endif
    return self;
}


/** <p>
    Returns a pair of random values from a new 2-dimensional sequence.</p><p>
    Following the paper strictly, one would create the value_0
    deterministically in increasing order,
    which means if we are not using all tmsNetSize values of the sequence,
    we would get terrible artifacts.
    To counteract, we use shuffled indices.
    </p>*/
- (void) getValuesFromNewSequences
        : (double *) value_0
        : (double *) value_1
{

  if (current2==size2) {
    [self _createMoreSequencesSingle: NO Double: YES];
  }

  if (state2[current2].i >= tmsNetSize) {
    // if we have used up one scrambled version of the (0, m, 2) net,
    // re-init with new scramble values (this should be rare, and harmless
    // if really needed!)
    // The following line sometimes causes trouble in output formatting
#ifdef FULL_DIAGNOSTICS
    [ reporter printf
        :   "ArcEfficientrandomGenerator: re-init 2-dim net %d\n"
        ,   current2];
#endif
    state2[current2].i = 0;
    state2[current2].r = (UInt32) nrand48(rseed48);
    state2[current2].s = (UInt32) nrand48(rseed48);
  }

  UInt32 v0 =
      state2[current2].shuffledIndices[state2[current2].i]
    * ( 0x100000000LL  / tmsNetSize)  ;
  v0 ^= state2[current2].r;
  *value_0 = (double) v0 / (double) 0x100000000LL;

  *value_1 =
    arrandom_RI_LP(
        state2[current2].shuffledIndices[state2[current2].i],
        state2[current2].s);

#ifndef NDEBUG_RANDOM_GENERATOR
    if (state2[current2].i>state2[current2].max)
        state2[current2].max=state2[current2].i;
    state2[current2].total ++;
#endif
    state2[current2].i ++;

#ifndef NDEBUG_RANDOM_GENERATOR
  if (current2>maxused2) maxused2=current2;
#endif
  ++current2;
  assert(*value_0 <  1.0);
  assert(*value_0 >= 0.0);
  assert(*value_1 <  1.0);
  assert(*value_1 >= 0.0);
}

- (double) valueFromNewSequence
{
  ArSequenceID dummy;
  return [self valueFromNewSequence: &dummy];
}

- (double) valueFromNewSequence
        : (ArSequenceID *) usedSequenceIndex
{
  if (current1==size1) {
    [self _createMoreSequencesSingle: YES Double: NO];
  }
  *usedSequenceIndex = current1;

  if (state1[current1].i >= tmsNetSize) {
    // if we have used up one scrambled version of the (0, m, 1) net,
    // re-init with new scramble values (this should be rare, and harmless
    // if really needed!)
    // The following line sometimes causes trouble in output formatting
#ifdef FULL_DIAGNOSTICS
    [ reporter printf
        :   "ArcEfficientRandomGenerator: re-init 1-dim net %d\n"
        ,   current1 ];
#endif
    state1[current1].i = 0;
    state1[current1].r = (UInt32) nrand48(rseed48);
  }

    double sample =
        arrandom_RI_LP(
            state1[current1].shuffledIndices[state1[current1].i],
            state1[current1].r);
#ifndef NDEBUG_RANDOM_GENERATOR
    if ( state1[current1].i > state1[current1].max )
        state1[current1].max=state1[current1].i;
#endif
  state1[current1].i ++;

#ifndef NDEBUG_RANDOM_GENERATOR
  if (current1>maxused1) maxused1=current1;
  state1[current1].total ++;
#endif

  ++current1;
  assert(sample <  1.0);
  assert(sample >= 0.0);
  return sample;
}

/** <p>
    Returns a single random value for a given sequence.
</p> */
- (double) valueFromSequence
        : (ArSequenceID) sequenceToUse
{
  if (sequenceToUse>size1)
    err(1, "Bad sequence index %lu! We only have %d", (unsigned long) sequenceToUse, size1);
    // should never happen...

  if (state1[sequenceToUse].i >= tmsNetSize) {
    // if we have used up one scrambled version of the (0, m, 1) net,
    // re-init with new scramble values (this should be rare, and harmless
    // if really needed!)
    // The following line sometimes causes trouble in output formatting
#ifdef FULL_DIAGNOSTICS
    [ reporter printf
        :   "ArcEfficientRandomGenerator: re-init 1-dim net %d\n"
        ,   current1 ];
#endif
    state1[sequenceToUse].i = 0;
    state1[sequenceToUse].r = (UInt32) nrand48(rseed48);
  }

  double sample =
    arrandom_RI_LP(
        state1[sequenceToUse].shuffledIndices[state1[sequenceToUse].i],
        state1[sequenceToUse].r );
#ifndef NDEBUG_RANDOM_GENERATOR
  if ( state1[sequenceToUse].i > state1[sequenceToUse].max )
    state1[sequenceToUse].max=state1[sequenceToUse].i;
#endif
  state1[sequenceToUse].i ++;
#ifndef NDEBUG_RANDOM_GENERATOR
  if (sequenceToUse>maxused1) maxused1=sequenceToUse;
  state1[sequenceToUse].total ++;
#endif

  assert(sample <  1.0);
  assert(sample >= 0.0);
  return sample;
}

- (void) reInitializeWith
        : (UInt32) uInt32Value
{
    rseed48[0]=(unsigned short) (uInt32Value  & 0xFFFF);
    rseed48[1]=(unsigned short) (uInt32Value >> 16);
    rseed48[2]=(unsigned short) (0x4711); // any should do
    current1=0; current2=0;
    for (UInt32 i=0; i<size1; ++i) {
      state1[i].r=(UInt32) nrand48(rseed48);
      state1[i].i=0;
    }
    for (UInt32 i=0; i<size2; ++i) {
      state2[i].r=(UInt32) nrand48(rseed48);
      state2[i].s=(UInt32) nrand48(rseed48);
      state2[i].i=0;
    }
    [self _shuffleIndices];
}

/** <p>
    This function shuffles the indices of the (0, m, 2)-nets to ensure random
    coverage of the area in case not all elements of the net are used.
    This uses Knuth's shuffling algorithm.
    Before, we reset indices. This ensures that we can make sub-images
    with the same noise pattern!
</p> */
- (id) _shuffleIndices
{
  for (UInt32 i=0; i<size1; ++i) {
    for (UInt32 j=0; j<tmsNetSize; ++j) state1[i].shuffledIndices[j]=j;
    // reset
    for (UInt32 j=0; j<tmsNetSize-1; ++j) {
      UInt32 other =
        j+  (UInt32) floor(((double) (tmsNetSize-j)) * erand48(rseed48));
      UInt32 temp=state1[i].shuffledIndices[j];
      state1[i].shuffledIndices[j]=state1[i].shuffledIndices[other];
      state1[i].shuffledIndices[other]=temp;
    }
  }
  for (UInt32 i=0; i<size2; ++i) {
    for (UInt32 j=0; j<tmsNetSize; ++j) state2[i].shuffledIndices[j]=j;
    // reset
    for (UInt32 j=0; j<tmsNetSize-1; ++j) {
      UInt32 other=
        j+  (UInt32) floor(((double) (tmsNetSize-j)) * erand48(rseed48));
      UInt32 temp=state2[i].shuffledIndices[j];
      state2[i].shuffledIndices[j]=state2[i].shuffledIndices[other];
      state2[i].shuffledIndices[other]=temp;
    }
  }
  return self;
}

- (void) resetSequenceIDs
{
  current1=0;
  current2=0;
}

- (ArSequenceID) currentSequenceID
{
  ArSequenceID marker= ((UInt64) current2) << 32 | current1;
  return marker;
}

- (void) setCurrentSequenceID
        : (ArSequenceID) sequenceID
{
  current1=  sequenceID & 0xFFFFFFFF;
  current2=  sequenceID >> 32;
}

// repeated just for protocol completeness.
- (ArcObject <ArpReporter> *) reporter
{
  return reporter;
}
- (void) setReporter
        : (ArcObject <ArpReporter> *) newReporter
{
    reporter = newReporter;
}

- (void) dealloc
{
  // This is indeed very verbose. Remove whatever data seem pointless!
#ifdef RANDOM_GENERATOR_DEBUG_PRINTFS
    [ reporter printf
        : "ArcEfficientRandomGenerator: final state of all used series:"];

  // group into lines of 10 entries each

  for (int line=0; line<=maxused1/10; ++line){
    [reporter printf: "\n\nSeq1 :"];
    for (int i=0; i<10; ++i){
      if (10*line+i<=maxused1) [reporter printf: "%10i ", 10*line+i];
    }
    [reporter printf: "\n   i :" ];
    for (int i=0; i<10; ++i){
      if (10*line+i<=maxused1)
        [reporter printf: "%10i ", state1[10*line+i].i];
    }
#ifndef NDEBUG_RANDOM_GENERATOR
    [reporter printf: "\n max :" ];
    for (int i=0; i<10; ++i){
      if (10*line+i<=maxused1)
        [reporter printf: "%10i ", state1[10*line+i].max];
    }
    [reporter printf: "\n tot :" ];
    for (int i=0; i<10; ++i){
      if (10*line+i<=maxused1)
        [reporter printf: "%10i ", state1[10*line+i].total];
    }
#endif
#ifdef FULL_DIAGNOSTICS
    [reporter printf: "\n seed:" ];
    for (int i=0; i<10; ++i){
      if (10*line+i<=maxused1)
        [reporter printf: "0x%08X ", state1[10*line+i].r ];
    }
#endif
  }

  for (int line=0; line<=maxused2/10; ++line){
    [reporter printf:"\n\nSeq2 :"];
    for (int i=0; i<10; ++i){
      if (10*line+i<=maxused2) [reporter printf: "%10i ", 10*line+i];
    }
    [reporter printf:"\n   i :"];
    for (int i=0; i<10; ++i){
      if (10*line+i<=maxused2)
        [reporter printf: "%10i ", state2[10*line+i].i ];
    }
#ifndef NDEBUG_RANDOM_GENERATOR
    [reporter printf: "\n max :"];
    for (int i=0; i<10; ++i){
      if (10*line+i<=maxused2)
        [reporter printf: "%10i ", state2[10*line+i].max];
    }
    [reporter printf: "\n tot :"];
    for (int i=0; i<10; ++i){
      if (10*line+i<=maxused2)
        [reporter printf: "%10i ", state2[10*line+i].total];
    }
#endif
#ifdef FULL_DIAGNOSTICS
    [reporter printf: "\nseed1:"];
    for (int i=0; i<10; ++i){
      if (10*line+i<=maxused2)
        [reporter printf: "0x%08X ", state2[10*line+i].r ];
    }
    [reporter printf: "\nseed2:" ];
    for (int i=0; i<10; ++i){
      if (10*line+i<=maxused2)
        [reporter printf: "0x%08X ", state2[10*line+i].s ];
    }
#endif
  }

#ifdef FULL_DIAGNOSTICS
  [reporter printf: "\n\nShuffled indices: "];
  for (UInt32 i=0; i<= maxused1; ++i){
    [reporter printf: "\nSeq1 %2d: \n", i];
    for (UInt32 line=0; line<=tmsNetSize/16; ++line){
      for (UInt32 j=0; j<16; ++j){
        if (16*line+j<tmsNetSize)
          [reporter printf: "%3d ", state1[i].shuffledIndices[16*line+j]];
      }
      [reporter printf: "\n"];
    }
  }
  for (UInt32 i=0; i<= maxused2; ++i){
    [reporter printf: "\nSeq2 %2d: \n", i];
    for (UInt32 line=0; line<=tmsNetSize/16; ++line){
      for (UInt32 j=0; j<16; ++j){
        if (16*line+j<tmsNetSize)
            [reporter printf: "%3d ", state2[i].shuffledIndices[16*line+j]];
      }
      [reporter printf: "\n"];
    }
  }
#endif

  [reporter printf: "\nDone with %s\n", [self cStringClassName]];
#endif
  for (UInt16 i=0; i<size1; ++i) FREE_ARRAY(state1[i].shuffledIndices);
  for (UInt16 i=0; i<size2; ++i) FREE_ARRAY(state2[i].shuffledIndices);
  FREE_ARRAY(state1);
  FREE_ARRAY(state2);
  [ super dealloc ];
}

@end



// ===========================================================================
