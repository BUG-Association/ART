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

#define ART_MODULE_NAME     ArpCoding

#import "ArpCoding.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    RUNTIME_REGISTER_PROTOCOL(ArpCoding);
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

#define ARPCODER_ARARRAY_IMPLEMENTATION(_Type,_type) \
void arpcoder_ar##_type##array( \
        id <ArpCoder> coder, \
        Ar##_Type##Array * array \
        ) \
{ \
    if ( [ coder isReading ] ) \
    { \
        unsigned int  arraySize; \
        \
        [ coder codeTableBegin \
            :   "ar" #_type "array" \
            : & arraySize ]; \
        (*array) = ar##_type##array_init( arraySize ); \
        for ( unsigned int index = 0; index < arraySize; index++) \
            [ coder code##_Type: & ARARRAY_I( *array, index ) ]; \
        [ coder codeTableEnd ]; \
    } \
    else \
    { \
        unsigned int  arraySize = ARARRAY_SIZE(*array); \
        \
        [ coder codeTableBegin \
            :   "ar" #_type "array" \
            : & arraySize ]; \
        for ( unsigned int index = 0; index < arraySize; index++) \
            [ coder code##_Type: & ARARRAY_I( *array, index ) ]; \
        [ coder codeTableEnd ]; \
    } \
}

ARPCODER_ARARRAY_IMPLEMENTATION(Int,int)
ARPCODER_ARARRAY_IMPLEMENTATION(Long,long)
ARPCODER_ARARRAY_IMPLEMENTATION(Float,float)
ARPCODER_ARARRAY_IMPLEMENTATION(Double,double)
ARPCODER_ARARRAY_IMPLEMENTATION(Pnt2D,pnt2d)
ARPCODER_ARARRAY_IMPLEMENTATION(Pnt3D,pnt3d)
ARPCODER_ARARRAY_IMPLEMENTATION(Pnt4D,pnt4d)
ARPCODER_ARARRAY_IMPLEMENTATION(FPnt2D,fpnt2d)
ARPCODER_ARARRAY_IMPLEMENTATION(FPnt3D,fpnt3d)
ARPCODER_ARARRAY_IMPLEMENTATION(Vec2D,vec2d)
ARPCODER_ARARRAY_IMPLEMENTATION(Vec3D,vec3d)
ARPCODER_ARARRAY_IMPLEMENTATION(FVec2D,fvec2d)
ARPCODER_ARARRAY_IMPLEMENTATION(FVec3D,fvec3d)

void arpcoder_arcolour(
        ART_GV         * art_gv,
        id <ArpCoder>    coder,
        ArSpectrum       * spc
        )
{
    if ( [ coder isReading ] )
    {
        ART_ERRORHANDLING_FATAL_ERROR(
            "polymorphic colour value reading not implemented"
            );
    }
    else
    {
        if ( art_foundation_isr(art_gv) == ardt_xyz )
            [ coder codeArCIEXYZ: ((ArCIEXYZ*)spc->value) ];

        if ( art_foundation_isr(art_gv) == ardt_spectrum8 )
            [ coder codeArSpectrum8: ((ArSpectrum8*)spc->value) ];

        if ( art_foundation_isr(art_gv) == ardt_spectrum11 )
            [ coder codeArSpectrum18: ((ArSpectrum18*)spc->value) ];

        if ( art_foundation_isr(art_gv) == ardt_spectrum18 )
            [ coder codeArSpectrum18: ((ArSpectrum18*)spc->value) ];

        if ( art_foundation_isr(art_gv) == ardt_spectrum46 )
            [ coder codeArSpectrum46: ((ArSpectrum46*)spc->value) ];
    }
}

#define ARPCODER_COLOURTYPE_IMPLEMENTATION(__Type,__type) \
void arpcoder_##__type ( \
        ART_GV         * art_gv, \
        id <ArpCoder>    coder, \
        __Type         * c \
        ) \
{ \
    (void) art_gv;\
\
    [ coder codeDouble: & C3_CI(c->c,0) ]; \
    [ coder codeDouble: & C3_CI(c->c,1) ]; \
    [ coder codeDouble: & C3_CI(c->c,2) ]; \
}

ARPCODER_COLOURTYPE_IMPLEMENTATION(ArRGB,arrgb);
ARPCODER_COLOURTYPE_IMPLEMENTATION(ArCIEXYZ,arciexyz);
ARPCODER_COLOURTYPE_IMPLEMENTATION(ArCIExyY,arciexyy);
ARPCODER_COLOURTYPE_IMPLEMENTATION(ArCIELab,arcielab);
ARPCODER_COLOURTYPE_IMPLEMENTATION(ArCIELuv,arcieluv);

#define ARPCODER_SPECTRUM_N_IMPLEMENTATION(__NC) \
\
void arpcoder_arspectrum##__NC ( \
        ART_GV            * art_gv, \
        id <ArpCoder>       coder, \
        ArSpectrum##__NC  * s \
        ) \
{ \
    unsigned int  size = __NC ; \
\
    [ coder codeTableBegin: "ArSpectrum" : & size ]; \
\
    for( unsigned int i = 0; i < size ; i++) \
    { \
        double  temp; \
\
        if ( [ coder isReading ] ) \
        { \
            [ coder codeDouble \
                : & temp \
                ]; \
\
            s##__NC##_set_sid( art_gv, s, i, temp ); \
        } \
        else \
        { \
            temp = s##__NC##_si( art_gv, s, i ); \
\
            [ coder codeDouble \
                : & temp \
                ]; \
        } \
    } \
\
    [ coder codeTableEnd ]; \
}

ARPCODER_SPECTRUM_N_IMPLEMENTATION(8);
ARPCODER_SPECTRUM_N_IMPLEMENTATION(11);
ARPCODER_SPECTRUM_N_IMPLEMENTATION(18);
ARPCODER_SPECTRUM_N_IMPLEMENTATION(46);

void arpcoder_arpsspectrum(
        ART_GV         * art_gv,
        id <ArpCoder>    coder,
        ArPSSpectrum   * pss
        )
{
    (void) art_gv;
    
    [ coder codeTableBegin: "psspectrum" : & ARPSS_SIZE(*pss) ];
    [ coder codeDouble: & ARPSS_SCALE(*pss) ];

    if ( [ coder isReading ] )
        ARPSS_ARRAY(*pss) = ALLOC_ARRAY( Pnt2D, ARPSS_SIZE(*pss) );

    for( unsigned int i = 0; i < ARPSS_SIZE(*pss); i++)
    {
        [ coder codePnt2D: & ARPSS_ARRAY_I(*pss,i) ];
    }

    [ coder codeTableEnd ];
}

void arpcoder_arrsspectrum(
        ART_GV         * art_gv,
        id <ArpCoder>    coder,
        ArRSSpectrum   * rss
        )
{
    (void) art_gv;
    
    [ coder codeTableBegin: "rsspectrum" : & ARRSS_SIZE(*rss) ];
    [ coder codeDouble: & ARRSS_START(*rss) ];
    [ coder codeDouble: & ARRSS_STEP(*rss) ];
    [ coder codeDouble: & ARRSS_SCALE(*rss) ];

    if ( [ coder isReading ] )
        ARRSS_ARRAY(*rss) = ALLOC_ARRAY( double, ARRSS_SIZE(*rss) );

    for( unsigned int i = 0; i < ARRSS_SIZE(*rss); i++)
        [ coder codeDouble: & ARRSS_ARRAY_I(*rss,i) ];

    [ coder codeTableEnd ];
}

void arpcoder_arrsspectrum2d(
        ART_GV          * art_gv,
        id <ArpCoder>     coder,
        ArRSSpectrum2D  * rss2d
        )
{
    (void) art_gv;
    
    [ coder codeTableBegin: "rsspectrum2d" : & rss2d->size ];
    [ coder codeInt:  & rss2d->stride ];
    [ coder codeDouble: & rss2d->excitation_start ];
    [ coder codeDouble: & rss2d->excitation_step ];
    [ coder codeDouble: & rss2d->emission_start ];
    [ coder codeDouble: & rss2d->emission_step ];
    [ coder codeDouble: & rss2d->scale ];

    if ( [ coder isReading ] )
        rss2d->array = ALLOC_ARRAY( double, rss2d->size );

    for( unsigned int i = 0; i < rss2d->size; i++)
        [ coder codeDouble: & rss2d->array[i] ];

    [ coder codeTableEnd ];
}

//#define OBJREFDYNARRAY_CODING_DEBUGPRINTF

void arpcoder_arnoderefdynarray(
        id <ArpCoder>       coder,
        ArNodeRefDynArray  * nodeRefArray
        )
{
    unsigned int  numberOfNodes;

    if ( ! [ coder isReading ] )
        numberOfNodes = arnoderefdynarray_size( nodeRefArray );

    [ coder codeTableBegin
        :   "arnoderefdynarray"
        : & numberOfNodes
        ];

#ifdef OBJREFDYNARRAY_CODING_DEBUGPRINTF
    printf(
        "arnoderefdynarray code: %u slots to fill\n"
        ,   numberOfNodes
        );
    fflush(stdout);
#endif

    if ( [ coder isReading ] )
    {
        arnoderefdynarray_free_contents(
            nodeRefArray
            );

        *nodeRefArray =
            arnoderefdynarray_init( 0 );

        unsigned long  entriesToProcess = numberOfNodes;

        while ( entriesToProcess-- )
        {
            ArNodeRef  nodeRef;

            [ coder codeSubnodeRef
                : & nodeRef
                ];

            arnoderefdynarray_push(
                nodeRefArray,
                nodeRef
                );

            //  It belongs to the dynarray now, so we have to
            //  release it.

            RELEASE_NODE_REF( nodeRef );

#ifdef OBJREFDYNARRAY_CODING_DEBUGPRINTF
            printf(
                "Slot %u: %s instance %p retain count %u\n"
                ,   entriesToProcess
                ,   [ node cStringClassName ]
                ,   node
                ,   RETAIN_COUNT_OF_OBJECT(node )
                );
            fflush(stdout);
#endif
        }
    }
    else
    {
        for ( unsigned long i = 0;
              i < numberOfNodes;
              i++ )
        {
            ArNodeRef  nodeRef;

            nodeRef = arnoderefdynarray_i( nodeRefArray, i );

            [ coder codeSubnodeRef
                : & nodeRef
                ];
        }
    }

    [ coder codeTableEnd ];
}

// ===========================================================================
