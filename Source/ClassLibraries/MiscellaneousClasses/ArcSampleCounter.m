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

#define ART_MODULE_NAME     ArcSampleCounter

#import "ArcSampleCounter.h"



ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

int int_num_digits(int x)
{  
    return (x < 10 ? 1 :
    	(x < 100 ? 2 :   
    	(x < 1000 ? 3 :   
    	(x < 10000 ? 4 :   
    	(x < 100000 ? 5 :   
    	(x < 1000000 ? 6 :   
    	(x < 10000000 ? 7 :  
    	(x < 100000000 ? 8 :  
    	(x < 1000000000 ? 9 :  
    	10)))))))));
}

@implementation ArcSampleCounter

- (id) init
        : (ArcObject <ArpReporter> *) newReporter
        : (ArNode <ArpPathspaceIntegratorCore> *) newPathspaceIntegrator
        : (id<ArpImageSamplerMessenger> ) newImageSampler
        : (int) newTargetSamples
{
    reporter = newReporter;
    imageSampler = newImageSampler;
    targetSamples = newTargetSamples;
    samplesSoFar = 0;
    
    [ reporter consolePrintf
        :   "%s"
        ,   [ imageSampler preSamplingMessage ]
        ];

    [ reporter beginTimedAction
        :   "image sampling: %s, %d spp"
        ,   [ newPathspaceIntegrator descriptionString ]
        ,   samplesSoFar
        ];

    [ reporter consolePrintf
        :   "\b"
        ];

    digits = int_num_digits( samplesSoFar );

    return self;
}


- (void) start
{
    arclock_now( & startClock );
}

- (void) step
        : (unsigned int) addedSamples
{
    samplesSoFar += addedSamples;

    [ reporter consolePrintf
        :   "\b\b\b\b"
        ];

    for ( int i = 0; i < digits; i++ )
    {
        [ reporter consolePrintf
            :   "\b"
            ];
    }
    
    [ reporter consolePrintf
        :   "%d spp"
        ,   samplesSoFar
        ];
    
    digits = int_num_digits( samplesSoFar );
}

- (void) stop
        : (double) duration
{
    if ( duration > 0.0 )
        totalSeconds = duration;
    else
    {
        arclock_now( & endClock );

        totalSeconds =
              arclock_seconds( & endClock )
            - arclock_seconds( & startClock );
    }
}

- (void) dealloc
{
    [ reporter modifyIndent
        : digits - 2
        ];
    
    [ reporter endActionWithTime
        : totalSeconds
        ];
    
    [ reporter consolePrintf
        :   "%s"
        ,   [ imageSampler postSamplingMessage ]
        ];

    [ super dealloc ];
}

@end

// ===========================================================================
