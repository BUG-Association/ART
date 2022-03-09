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

#define ART_MODULE_NAME     ArBSDFSample

#import "ArBSDFSample.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

int arbsdfsample_s_valid(
        ART_GV        * art_gv,
        ArBSDFSample  * sample
        )
{
    return
        arattenuationsample_a_valid(
            art_gv,
            ARBSDFSAMPLE_ATTENUATION( *sample )
            );
}

void arbsdfsamplelist_fast_free(
        ArBSDFSampleList       * sl,
        ArBSDFSampleFreelists  * sfl
        )
{
    ArBSDFSample  * s;

    while ( ( s = ARBSDFSAMPLELIST_HEAD(*sl) ) )
    {
        ARBSDFSAMPLELIST_HEAD(*sl) = ARBSDFSAMPLE_NEXT(*s);

        arbsdfsample_fast_free_to_freelist( s, sfl );
    }

    arfreelist_push( & ARBSDFSAMPLELIST_FREELIST(*sfl), sl  );
}

void arbsdfsamplelist_free_contents(
        ArBSDFSampleList       * sl,
        ArBSDFSampleFreelists  * sfl
        )
{
    if ( sl )
        arbsdfsamplelist_fast_free( sl, sfl );
}

void arbsdfsamplelist_add_and_filter_special(
        ART_GV                  * art_gv,
        ArBSDFSampleList        * srcList,
        double                    weight,
        ArBSDFSampleList        * dstList,
        ArBSDFSample           ** specialArray,
        ArBSDFSampleFreelists   * sfl
        )
{
    ArBSDFSample * s = ARBSDFSAMPLELIST_HEAD(*srcList);

    while ( s )
    {
        ArBSDFSample  * next = ARBSDFSAMPLE_NEXT(*s);

        arattenuationsample_d_mul_a(
            art_gv,
            weight,
            ARBSDFSAMPLE_ATTENUATION(*s)
            );

        if ( ARBSDFSAMPLE_IS_SPECULAR_REFLECTION(*s) )
        {
            if ( specialArray[0] )
            {
                arattenuationsample_a_add_a(
                    art_gv,
                    ARBSDFSAMPLE_ATTENUATION(*s),
                    ARBSDFSAMPLE_ATTENUATION(*specialArray[0])
                    );
                s = arbsdfsample_pop( s, sfl );
            }
            else
                specialArray[0] = s;
        }
        else if ( ARBSDFSAMPLE_IS_SPECULAR_REFRACTION(*s) )
        {
            if ( specialArray[1] )
            {
                arattenuationsample_a_add_a(
                    art_gv,
                    ARBSDFSAMPLE_ATTENUATION(*s),
                    ARBSDFSAMPLE_ATTENUATION(*specialArray[1])
                    );
                s = arbsdfsample_pop( s, sfl );
            }
            else
                specialArray[1] = s;
        }
        else
            arbsdfsamplelist_push( dstList, s );
        s = next;
    }
}

#define  BSDFSAMPLELIST_HEAD       ARBSDFSAMPLELIST_HEAD(*list)

void arbsdfsamplelist_compute_percentiles(
        ART_GV            * art_gv,
        ArBSDFSampleList  * list
        )
{
    ArBSDFSample  * s = BSDFSAMPLELIST_HEAD;
    int             numberOfSamples = 0;
    double          totalContributionEstimate = 0.0;

    while ( s )
    {
        ASSERT_VALID_BSDFSAMPLE( s );

        double avg =
            arattenuationsample_a_avg( art_gv, ARBSDFSAMPLE_ATTENUATION(*s) );
        // TODO: Optimization: store this light contribution in BSDF sample
        //       structure value so that it can be reused later in this function.
        double sampleContributionEstimate =
            //(ARBSDFSAMPLE_AVERAGE_ATTENUATION(*s) * M_ABS(ARBSDFSAMPLE_COSINE(*s)))
            /// ARPDFVALUE_MAIN(ARBSDFSAMPLE_PROBABILITY(*s));
            ARPDFVALUE_IS_INFINITE(ARBSDFSAMPLE_PROBABILITY(*s))
                ? avg
                : ((avg * M_ABS(ARBSDFSAMPLE_COSINE(*s)))
                   / ARPDFVALUE_MAIN(ARBSDFSAMPLE_PROBABILITY(*s)));

        totalContributionEstimate += sampleContributionEstimate;
        numberOfSamples++;

        s = ARBSDFSAMPLE_NEXT(*s);
    }

    if ( numberOfSamples == 0 )
        return;

    if ( numberOfSamples == 1 )
    {
        ARBSDFSAMPLE_SELECTION_PROBABILITY( *BSDFSAMPLELIST_HEAD ) = 1.0;
        ARBSDFSAMPLE_PERCENTILE( *BSDFSAMPLELIST_HEAD ) = 1.0;
    }
    else
    {
        double percentileSoFar = 0.0;

        s = BSDFSAMPLELIST_HEAD;

        while ( s )
        {
            //ARBSDFSAMPLE_SELECTION_PROBABILITY(*s) =
            //    1.0 / numberOfSamples;

            double avg =
                arattenuationsample_a_avg( art_gv, ARBSDFSAMPLE_ATTENUATION(*s) );
            double sampleContributionEstimate =
                //(ARBSDFSAMPLE_AVERAGE_ATTENUATION(*s) * M_ABS(ARBSDFSAMPLE_COSINE(*s)))
                // / ARPDFVALUE_MAIN(ARBSDFSAMPLE_PROBABILITY(*s));
                ARPDFVALUE_IS_INFINITE(ARBSDFSAMPLE_PROBABILITY(*s))
                    ? avg
                    : ((avg * M_ABS(ARBSDFSAMPLE_COSINE(*s)))
                       / ARPDFVALUE_MAIN(ARBSDFSAMPLE_PROBABILITY(*s)));

            ARBSDFSAMPLE_SELECTION_PROBABILITY(*s) =
                  sampleContributionEstimate
                / totalContributionEstimate;
            ARBSDFSAMPLE_PERCENTILE(*s) =
                percentileSoFar
                + ARBSDFSAMPLE_SELECTION_PROBABILITY(*s) ;
            percentileSoFar = ARBSDFSAMPLE_PERCENTILE(*s);

            s = ARBSDFSAMPLE_NEXT(*s);
        }
    }
}

void arbsdfsamplefreelists_alloc(
        ART_GV                 * art_gv,
        ArBSDFSampleFreelists  * sfl,
        int                      initialSize
        )
{
    ARFREELIST_INIT_FOR_STRUCT(
        ARBSDFSAMPLELIST_FREELIST(*sfl),
        ArBSDFSampleList,
        initialSize
        );

    ARFREELIST_INIT_FOR_TYPE(
        ARBSDFSAMPLE_FREELIST(*sfl),
        arbsdfsample,
        initialSize
        );
}

void arbsdfsamplefreelists_free_contents(
        ArBSDFSampleFreelists  * sfl
        )
{
    arfreelist_free_contents( & ARBSDFSAMPLELIST_FREELIST(*sfl) );
    arfreelist_free_contents( & ARBSDFSAMPLE_FREELIST(*sfl) );
}

ArBSDFSampleList * arbsdfsamplelist_alloc(
        ArBSDFSampleFreelists  * sfl
        )
{
    ArBSDFSampleList  * sl =
        arfreelist_pop( & ARBSDFSAMPLELIST_FREELIST(*sfl) );
    ARBSDFSAMPLELIST_HEAD(*sl) = 0;
    return sl;
}

ArBSDFSample * arbsdfsample_alloc(
        ART_GV  * art_gv
        )
{
    ArBSDFSample  * s = ALLOC(ArBSDFSample);

    ARBSDFSAMPLE_ATTENUATION(*s) = arattenuationsample_alloc( art_gv );

    return s;
}

void arbsdfsample_free(
        ART_GV        * art_gv,
        ArBSDFSample  * sample
        )
{
    arattenuationsample_free( art_gv, ARBSDFSAMPLE_ATTENUATION(*sample) );

    FREE(sample);
}

ArBSDFSample * arbsdfsample_alloc_from_freelist(
        ArBSDFSampleFreelists  * sfl
        )
{
    ArBSDFSample * s = arfreelist_pop( & ARBSDFSAMPLE_FREELIST(*sfl) );
    ARBSDFSAMPLE_TYPE(*s) = arbsdfsample_general;
    ARBSDFSAMPLE_NEXT(*s) = 0;
    return s;
}

void arbsdfsample_init(
        const ART_GV               * art_gv,
              ArBSDFSample         * s,
        const Vec3D                * vector,
              double                 cosine,
        const ArAttenuationSample  * attenuationSample
        )
{
    ARBSDFSAMPLE_PERCENTILE(*s) =   0.0;

    ARBSDFSAMPLE_VECTOR(*s)     = * vector;
    ARBSDFSAMPLE_COSINE(*s)     =   cosine;

    arattenuationsample_a_init_a(
        art_gv,
        attenuationSample,
        ARBSDFSAMPLE_ATTENUATION(*s)
        );
}

void arbsdfsample_fast_free_to_freelist(
        ArBSDFSample           * s,
        ArBSDFSampleFreelists  * sfl
        )
{
    arfreelist_push( & ARBSDFSAMPLE_FREELIST(*sfl), s );
}

void arbsdfsample_free_to_freelist(
        ArBSDFSample           * s,
        ArBSDFSampleFreelists  * sfl
        )
{
    if ( s )
        arbsdfsample_fast_free_to_freelist( s, sfl );
}

void arbsdfsamplelist_push(
        ArBSDFSampleList  * l,
        ArBSDFSample      * s
        )
{
    ARBSDFSAMPLE_NEXT(*s) = ARBSDFSAMPLELIST_HEAD(*l);
    ARBSDFSAMPLELIST_HEAD(*l) = s;
}

ArBSDFSample * arbsdfsample_pop(
        ArBSDFSample           * s,
        ArBSDFSampleFreelists  * sfl
        )
{
    ArBSDFSample * next = ARBSDFSAMPLE_NEXT(*s);
    arbsdfsample_fast_free_to_freelist( s, sfl );
    return next;
}

void arbsdfsamplelist_add_special(
        ArBSDFSampleList   * list,
        ArBSDFSample      ** sp
        )
{
    for ( unsigned int i = 0; i < ARBSDFSAMPLE_NUMBER_OF_SPECIAL_CASES; i++ )
        if ( sp[i] )
            arbsdfsamplelist_push( list, sp[i] );
}

// ===========================================================================
