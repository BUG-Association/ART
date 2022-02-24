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

#define ART_MODULE_NAME     ArnColourMatchingFunctions

#import "ArnColourMatchingFunctions.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ ArnColourMatchingFunctions registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#define MATCHING_FUNCTION(__i)  \
    (ArNode <ArpSpectrum> *)ARNODEREF_POINTER(arnoderefdynarray_i(&subnodeRefArray,(__i)))
#define NORMALISATION_FUNCTION(__c,__i) \
    (ArNode <ArpSpectrum> *)ARNODEREF_POINTER(arnoderefdynarray_i(&subnodeRefArray,(__i)+(__c)))

@implementation ArnColourMatchingFunctions

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnColourMatchingFunctions)

- (id) init
        : (ArNodeRefDynArray *) newSubnodeArray
        : (ArDataType) newTargetType
        : (ArCMFNormalisation *) newNormalisationType
{
    self =
        [ super init
            :   newSubnodeArray
            ];
    
    if ( self )
    {
        validCMF = NO;
        colourMatchingFunctions.resultDataType = newTargetType;
        normalisationType = newNormalisationType;
    }
    
    return self;
}

- (void) getColourMatchingFunctions
        : (ArCMF *) outMatchingFunctions
{
    if ( !validCMF )
    {
        int  masterIndex = -1;
        int  i, numChannels =
             ARDATATYPE_NUMCHANNELS(colourMatchingFunctions.resultDataType);
        int  currentNormalisation = 0;
        double  area = 0.0;

        ARCMF_CURVES(colourMatchingFunctions) =
            ALLOC_ARRAY(ArPSSpectrum,numChannels);

        ARCMF_CURVES_500(colourMatchingFunctions) =
            ALLOC_ARRAY(ArSpectrum500,numChannels);

        for ( i = 0; i < numChannels; i++ )
        {
            [ MATCHING_FUNCTION(i) getNewPSSpectrum
                :   0
                : & ARCMF_CURVE(colourMatchingFunctions,i)
                ];

            if (ARCMF_NORMALISATION_MASTER(normalisationType[i]))
            {
                masterIndex = i;
        
                if(ARCMF_NORMALISATION_EXTERNAL( normalisationType[i]))
                {
                    ArPSSpectrum  normSpectrum;

                    [ NORMALISATION_FUNCTION(numChannels,
                                             currentNormalisation++)
                        getNewPSSpectrum
                            :   0
                            : & normSpectrum ];

                    area =
                        pss_inner_product(
                              art_gv,
                            & normSpectrum,
                            & ARCMF_CURVE(colourMatchingFunctions,i)
                            );

                    FREE_ARRAY(normSpectrum.array);
                }
                else
                    area =
                        pss_inner_product(
                              art_gv,
                              VISIBLE_RANGE,
                            & ARCMF_CURVE(colourMatchingFunctions,i)
                            );
            }

            if (ARCMF_NORMALISATION_INDEPENDENT( normalisationType[i]))
            {
                double  area_i;

                if(ARCMF_NORMALISATION_EXTERNAL(
                                               normalisationType[i]))
                {
                    ArPSSpectrum  normSpectrum;

                    [ NORMALISATION_FUNCTION(numChannels,
                                             currentNormalisation++)
                        getNewPSSpectrum
                            :   0
                            : & normSpectrum ];
                    area_i =
                        pss_inner_product(
                              art_gv,
                            & normSpectrum,
                            & ARCMF_CURVE(colourMatchingFunctions,i)
                            );

                    FREE_ARRAY(normSpectrum.array);
                }
                else
                    area_i =
                        pss_inner_product(
                              art_gv,
                              VISIBLE_RANGE,
                            & ARCMF_CURVE(colourMatchingFunctions,i)
                            );

                if ( area_i != 0.0 )
                    ARCMF_CURVE_SCALE(colourMatchingFunctions,i) /= area_i;
            }
            
            pss_to_s500(
                  art_gv,
                & ARCMF_CURVE(colourMatchingFunctions,i),
                & ARCMF_CURVE_500(colourMatchingFunctions,i)
                );
        }

        if ( masterIndex != -1 )
        {
            if ( area != 0.0 )
                for ( i = 0; i < numChannels; i++ )
                    if (
                      ARCMF_NORMALISATION_MASTER( normalisationType[i] )
                   || ARCMF_NORMALISATION_SLAVE( normalisationType[i]) )
                        ARCMF_CURVE_SCALE(colourMatchingFunctions,i) /= area;
        }

        validCMF = YES;
    }

    arcmf_c_copy_c(
          art_gv,
        & colourMatchingFunctions,
          outMatchingFunctions
          );
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    int  i, numChannels;

    [ super code: coder ];
    [ coder codeInt: ((int*)&colourMatchingFunctions.resultDataType) ];

    numChannels = ARDATATYPE_NUMCHANNELS(colourMatchingFunctions.resultDataType);

    if ( [ coder isReading ] )
    {
        validCMF = NO;
        normalisationType
            = ALLOC_ARRAY(ArCMFNormalisation,numChannels);
    }

    for ( i = 0; i < numChannels; i++ )
        [ coder codeInt: ((int*)&normalisationType[i]) ];
}

@end

// ===========================================================================
