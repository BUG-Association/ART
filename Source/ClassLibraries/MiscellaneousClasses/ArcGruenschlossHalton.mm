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

#define ART_MODULE_NAME     ArcGruenschlossHalton

#import "ArcGruenschlossHalton.h"

#import "halton_sampler.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


typedef struct ArcGruenschlossHalton_members
{
    int               maxHaltonSequence;
    int               currentSequence;
    int             * lastValueUsed;
    Halton_sampler    haltonSampler;
}
ArcGruenschlossHalton_members;

#define MAX_HALTON_SEQUENCE     member_vars->maxHaltonSequence
#define CURRENT_SEQUENCE        member_vars->currentSequence
#define LAST_VALUE_USED         member_vars->lastValueUsed
#define HALTON_SAMPLER          member_vars->haltonSampler

@implementation ArcGruenschlossHalton

- (id) init
{
    return
        [ self initWithReporter
            :   NULL
            :   0
            ];
}

- (id) init
        : (int) newMaxNumberOfSamples
{
    return
        [ self initWithReporter
            :   NULL
            :   newMaxNumberOfSamples
            ];
}

- (id) initWithReporter
        : (ArcObject <ArpReporter> *) newReporter
        : (int) newMaxNumberOfSamples
{
    self =
        [ super initWithReporter
            :   newReporter
            ];
    
    if ( self )
    {
        if ( ! member_vars )
        {
            member_vars = ALLOC(ArcGruenschlossHalton_members);
            MAX_HALTON_SEQUENCE = 0;
            CURRENT_SEQUENCE = 0;
            LAST_VALUE_USED = NULL;
        }

        HALTON_SAMPLER.init_faure();
        MAX_HALTON_SEQUENCE = HALTON_SAMPLER.get_num_dimensions();
        CURRENT_SEQUENCE = 0;

        if ( MAX_HALTON_SEQUENCE > 0 )
        {
            [ reporter printf
                :   "ArcGruenschlossHalton: %d Halton sequences, reverting to NR2"
                    "above that.\n"
                ,   MAX_HALTON_SEQUENCE
                ];

            LAST_VALUE_USED = ALLOC_ARRAY( int, MAX_HALTON_SEQUENCE );
        }
        else
        {
            [ reporter printf
                :   "ArcGruenschlossHalton: complete fallback to NR2 functionality\n"
                ];
        }
        
        [ self reInitializeWith: 0 ];
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
    CURRENT_SEQUENCE++;

    return [ self valueFromSequence: CURRENT_SEQUENCE ];
}

- (double) valueFromNewSequence
        : (ArSequenceID *) usedSequence
{
    CURRENT_SEQUENCE++;
    *usedSequence = CURRENT_SEQUENCE;

    return [ self valueFromSequence: CURRENT_SEQUENCE ];
}

- (double) valueFromSequence
        : (ArSequenceID) sequenceID
{
    //  If a sequence > the max provided by the sampler is requested,
    //  we revert to NR2 values.
    
    if ( (int) sequenceID >= MAX_HALTON_SEQUENCE )
        return [ super valueFromNewSequence ];

    double  value =
        HALTON_SAMPLER.sample(
            sequenceID,
            LAST_VALUE_USED[ sequenceID ]
            );

    LAST_VALUE_USED[ sequenceID ]++;

    return  value;
}

- (void) reInitializeWith
        : (UInt32) uInt32Value
{
    [ super reInitializeWith: uInt32Value ];

    CURRENT_SEQUENCE = 0;

    for ( int i = 0; i < MAX_HALTON_SEQUENCE; i++ )
        LAST_VALUE_USED[ i ] = 0;
}

- (void) resetSequenceIDs
{
    CURRENT_SEQUENCE = 0;
}

- (ArSequenceID) currentSequenceID
{
    return CURRENT_SEQUENCE;
}

- (void) setCurrentSequenceID
        : (ArSequenceID) sequenceID
{
    CURRENT_SEQUENCE = (int) sequenceID;
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
    if ( member_vars )
    {
        FREE_ARRAY(LAST_VALUE_USED);
        FREE(member_vars);
    }

    [ super dealloc ];
}

@end

// ===========================================================================
