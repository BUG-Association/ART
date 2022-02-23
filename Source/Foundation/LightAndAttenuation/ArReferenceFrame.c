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

#define ART_MODULE_NAME     ArReferenceFrame

#include "ArReferenceFrame.h"
#include "FoundationAssertionMacros.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


ArPathDirection arpathdirection_opposite(
        const ArPathDirection pd
        )
{
    return 3 - pd;
}

void arrefframe_v_pd_init_rf(
        const ART_GV            * art_gv,
        const Vec3D             * v0,
        const ArPathDirection     pd,
              ArReferenceFrame  * rr
        )
{
    (void) art_gv;
    
    ASSERT_VALID_VEC3D( *v0 );
    ASSERT_ALLOCATED_REFERENCE_FRAME( rr );

    Vec3D  propagationDirection;

    if ( pd == arpathdirection_from_eye )
    {
        vec3d_dv_mul_v( -1.0, v0, & propagationDirection );
    }
    else
    {
        propagationDirection = *v0;
    }

    vec3d_v_perpedicular_vv(
        & propagationDirection,
        & ARREFFRAME_RF_I(*rr,0),
        & ARREFFRAME_RF_I(*rr,1)
        );

    ASSERT_VALID_VEC3D( ARREFFRAME_RF_I( *rr, 0 ) );
    ASSERT_VALID_VEC3D( ARREFFRAME_RF_I( *rr, 1 ) );

    vec3d_norm_v( & ARREFFRAME_RF_I(*rr,1) );

    ASSERT_VALID_REFERENCE_FRAME( rr );
}

void arrefframe_vv_pd_init_rf(
        const ART_GV            * art_gv,
        const Vec3D             * v0,
        const Vec3D             * v1,
        const ArPathDirection     pd,
              ArReferenceFrame  * rr
        )
{
    ASSERT_VALID_VEC3D( *v0 );
    ASSERT_VALID_VEC3D( *v1 );
    ASSERT_ALLOCATED_REFERENCE_FRAME( rr );

    vec3d_vv_cross_v(
          v0,
          v1,
        & ARREFFRAME_RF_I( *rr, 0 )
        );

    if (    ! m_d_is_tiny( VEC3D_I( ARREFFRAME_RF_I( *rr, 0 ), 0 ) )
         || ! m_d_is_tiny( VEC3D_I( ARREFFRAME_RF_I( *rr, 0 ), 1 ) )
         || ! m_d_is_tiny( VEC3D_I( ARREFFRAME_RF_I( *rr, 0 ), 2 ) )
        )
    {
        //   normal case - we can do something here

        ASSERT_VALID_VEC3D( ARREFFRAME_RF_I( *rr, 0 ) );

        vec3d_norm_v( & ARREFFRAME_RF_I( *rr, 0 ) );

        vec3d_vv_cross_v(
              v0,
            & ARREFFRAME_RF_I( *rr, 0 ),
            & ARREFFRAME_RF_I( *rr, 1 )
            );

        ASSERT_VALID_VEC3D( ARREFFRAME_RF_I( *rr, 1 ) );

        vec3d_norm_v( & ARREFFRAME_RF_I( *rr, 1 ) );
    }
    else
    {
        //   pathological case, normal incidence

        arrefframe_v_pd_init_rf(
            art_gv,
            v0,
            pd,
            rr
            );
    }
    
    ASSERT_VALID_REFERENCE_FRAME( rr );
}

void arreframe_vv_pd_init_rf_rf(
        const ART_GV            * art_gv,
        const Vec3D             * incoming_vector,
        const Vec3D             * outgoing_vector,
        const ArPathDirection     pathdirection,
              ArReferenceFrame  * refframe_entry,
              ArReferenceFrame  * refframe_exit
        )
{
    (void) art_gv;
    
    ASSERT_VALID_VEC3D(*incoming_vector)
    ASSERT_VALID_VEC3D(*outgoing_vector)
    ASSERT_ALLOCATED_REFERENCE_FRAME( refframe_entry );
    ASSERT_ALLOCATED_REFERENCE_FRAME( refframe_exit );

    Vec3D  light_incoming;
    Vec3D  light_outgoing;

    if ( pathdirection == arpathdirection_from_eye )
    {
        vec3d_dv_mul_v( -1.0, outgoing_vector, & light_incoming );
        vec3d_dv_mul_v( -1.0, incoming_vector, & light_outgoing );
    }
    else
    {
        light_incoming = *incoming_vector;
        light_outgoing = *outgoing_vector;
    }

    vec3d_norm_v( & light_incoming );
    vec3d_norm_v( & light_outgoing );

    Vec3D  senkrecht;
    Vec3D  parallel_incoming;
    Vec3D  parallel_outgoing;

    vec3d_vv_cross_v( & light_incoming, & light_outgoing, & senkrecht );
    
    //   Safety check: what if someone fed us two identical input directions
    
    if ( vec3d_v_sqrlen( & senkrecht ) == 0.0 )
    {
        //   Plan B: any perpendicular directon will do
        
        vec3d_v_perpedicular_v( & light_incoming, & senkrecht );
    }
    
    ASSERT_VALID_VEC3D(senkrecht)

    vec3d_vv_cross_v( & light_incoming, & senkrecht, & parallel_incoming );
    vec3d_vv_cross_v( & light_outgoing, & senkrecht, & parallel_outgoing );

    ASSERT_VALID_VEC3D(parallel_incoming)
    ASSERT_VALID_VEC3D(parallel_outgoing)
    
    vec3d_norm_v( & senkrecht );
    vec3d_norm_v( & parallel_incoming );
    vec3d_norm_v( & parallel_outgoing );
    
    ARREFFRAME_RF_I(*refframe_entry, 0) = senkrecht;
    ARREFFRAME_RF_I(*refframe_entry, 1) = parallel_incoming;
    ARREFFRAME_RF_I(*refframe_exit, 0) = senkrecht;
    ARREFFRAME_RF_I(*refframe_exit, 1) = parallel_outgoing;

    ASSERT_VALID_REFERENCE_FRAME( refframe_entry )
    ASSERT_VALID_REFERENCE_FRAME( refframe_exit )
}

void arreframe_vvv_pd_init_rf_rf(
        const ART_GV            * art_gv,
        const Vec3D             * incoming_vector,
        const Vec3D             * normal_vector,
        const Vec3D             * outgoing_vector,
        const ArPathDirection     pathdirection,
              ArReferenceFrame  * refframe_entry,
              ArReferenceFrame  * refframe_exit
        )
{
    (void) art_gv;
    
    // FIXME: This function expects Fresnel-like incoming-outcoming vector pair!
    //                                                                     (ip)

    Vec3D  light_incoming;
    Vec3D  light_outgoing;

    if ( pathdirection == arpathdirection_from_eye )
    {
        vec3d_dv_mul_v( -1.0, outgoing_vector, & light_incoming );
        vec3d_dv_mul_v( -1.0, incoming_vector, & light_outgoing );
    }
    else
    {
        light_incoming = *incoming_vector;
        light_outgoing = *outgoing_vector;
    }

    vec3d_vv_cross_v(
        & light_incoming,
          normal_vector,
        & ARREFFRAME_RF_I( *refframe_entry, 0 )
        );

    if (    m_d_is_tiny( VEC3D_I( ARREFFRAME_RF_I( *refframe_entry, 0 ), 0 ) )
         && m_d_is_tiny( VEC3D_I( ARREFFRAME_RF_I( *refframe_entry, 0 ), 1 ) )
         && m_d_is_tiny( VEC3D_I( ARREFFRAME_RF_I( *refframe_entry, 0 ), 2 ) )
        )
    {
        // If the result is zero vector, the incoming and normal vector are
        // parallel to each other and we need to create the orthogonal vector
        // by hand.

        vec3d_v_perpedicular_v(
            & light_incoming,
            & ARREFFRAME_RF_I( *refframe_entry, 0 )
            );
    }

    ASSERT_VALID_VEC3D( ARREFFRAME_RF_I( *refframe_entry, 0 ) );

    vec3d_norm_v( & ARREFFRAME_RF_I( *refframe_entry, 0 ) );

    vec3d_vv_cross_v(
        & light_incoming,
        & ARREFFRAME_RF_I( *refframe_entry, 0 ),
        & ARREFFRAME_RF_I( *refframe_entry, 1 )
        );

    ASSERT_VALID_VEC3D( ARREFFRAME_RF_I( *refframe_entry, 1 ) );

    vec3d_norm_v( & ARREFFRAME_RF_I( *refframe_entry, 1 ) );

    vec3d_vv_cross_v(
        & light_outgoing,
          normal_vector,
        & ARREFFRAME_RF_I( *refframe_exit, 0 )
        );
    
    // FIXME: check for case where the normal vector and outgoing vector are parallel to each other

    vec3d_vv_cross_v(
        & light_outgoing,
        & ARREFFRAME_RF_I( *refframe_exit, 0 ),
        & ARREFFRAME_RF_I( *refframe_exit, 1 )
        );

    ASSERT_VALID_VEC3D( ARREFFRAME_RF_I( *refframe_exit, 0 ) );
    ASSERT_VALID_VEC3D( ARREFFRAME_RF_I( *refframe_exit, 1 ) );

    vec3d_norm_v( & ARREFFRAME_RF_I( *refframe_exit, 0 ) );
    vec3d_norm_v( & ARREFFRAME_RF_I( *refframe_exit, 1 ) );

    
//    ASSERT_IDENTICAL_VEC3D(
//        ARREFFRAME_RF_I( *refframe_entry, 0 ),
//        ARREFFRAME_RF_I( *refframe_exit, 0 )
//        )
}

unsigned int arrefframe_rf_valid(
        const ART_GV            * art_gv,
        const ArReferenceFrame  * r0
        )
{
    (void) art_gv;
    
    //   We have two things to check here:

    //   a) whether the two refframe vectors are of length 1.0:

    double  r0_len0 = vec3d_v_len( & ARREFFRAME_RF_I( *r0, 0 ) );
    double  r0_len1 = vec3d_v_len( & ARREFFRAME_RF_I( *r0, 1 ) );

    //   The actual check is done via m_dd_equal() to avoid
    //   numerical gremlins

    if ( m_dd_equal( r0_len0, 1.0 ) && m_dd_equal( r0_len1, 1.0 ) )
    {
        //   b) whether the two vectors are orthogonal

        double  cos_r0r1 =
            vec3d_vv_dot(
                & ARREFFRAME_RF_I( *r0, 0 ),
                & ARREFFRAME_RF_I( *r0, 1 )
                );

        if ( m_dd_equal( cos_r0r1, 0.0 ) )
            return 1;
        else
            ART_ERRORHANDLING_WARNING(
                "non orthogonal refframe basis vectors with cosine %.10f"
                ,   cos_r0r1
                );
    }
    else
        ART_ERRORHANDLING_WARNING(
            "non unit length refframe basis vectors %.8f and %.8f"
            ,   r0_len0
            ,   r0_len1
            );

    return 0;
}

unsigned int arrefframe_rf_rf_d_coaxial(
        const ART_GV            * art_gv,
        const ArReferenceFrame  * r0,
        const ArReferenceFrame  * r1,
        const double              d0
        )
{
    (void) art_gv;
    
    //   We compute the propagation directions for the two refframes.

    Vec3D  dir0, dir1;

    vec3d_vv_cross_v(
        & ARREFFRAME_RF_I( *r0, 0 ),
        & ARREFFRAME_RF_I( *r0, 1 ),
        & dir0
        );

    vec3d_vv_cross_v(
        & ARREFFRAME_RF_I( *r1, 0 ),
        & ARREFFRAME_RF_I( *r1, 1 ),
        & dir1
        );

    double  dir0_len = vec3d_v_len( & dir0 );
    double  dir1_len = vec3d_v_len( & dir1 );

    unsigned int  result = 0;

    //   We only check whether the propagation directions match if
    //   they are both of length 1 (implicit sanity check for valid refframes).

    if ( m_dd_equal( dir0_len, 1.0 ) && m_dd_equal( dir1_len, 1.0 ) )
    {
        if ( vec3d_vv_equal( & dir0, & dir1 ) )
            result = 1;
        else
        {
            //   Also, if the prapagation directions of the two refframes
            //   fail the equality test, we take a look if they are exactly
            //   anti-coaxial (a popular failure case).

            vec3d_d_mul_v(
                 -1.0,
                & dir0
                );

            if ( vec3d_vv_equal( & dir0, & dir1 ) )
            {
                ART_ERRORHANDLING_WARNING(
                    "anti-coaxial refframes detected"
                    );
            }
            else
            {
                double  cos_dir0dir1 =
                      -1.0
                    * vec3d_vv_dot(
                          & dir0,
                          & dir1
                          );

                double  cosMaxDeviation = cos( d0 );

                if ( cos_dir0dir1 > M_ABS(cosMaxDeviation) )
                    result = 1;
                else
                    ART_ERRORHANDLING_WARNING(
                        "non coaxial refframes detected - spread is "
                        "%2.1f degrees, max allowed would be %2.1f "
                        "degrees"
                        ,   acos(cos_dir0dir1) * MATH_RAD_TO_DEG
                        ,   acos(cosMaxDeviation) * MATH_RAD_TO_DEG
                        );
            }

        }
    }
    else
        ART_ERRORHANDLING_WARNING(
            "refframe with non-unit length of its direction vector length detected during coaxiality check"
            );

    //   In both cases that generate warnings, the entire test is marked
    //   as failure, so the calling assertion will likely terminate the
    //   program anyway. And the warnings serve as an additional information.

    return result;
}

unsigned int arrefframe_rf_vd_coaxial(
        const ART_GV            * art_gv,
        const ArReferenceFrame  * r0,
        const Vec3D             * v0,
        const double              d0
        )
{
    (void) art_gv;
    
    //   We compute the propagation directions for the two refframes.

    Vec3D  dir0;

    vec3d_vv_cross_v(
        & ARREFFRAME_RF_I( *r0, 0 ),
        & ARREFFRAME_RF_I( *r0, 1 ),
        & dir0
        );

    double  dir0_len = vec3d_v_len( & dir0 );
    double  dir1_len = vec3d_v_len( v0 );

    unsigned int  result = 0;

    //   We only check whether the propagation directions match if
    //   they are both of length 1 (implicit sanity check for valid refframes).

    if ( m_dd_equal( dir0_len, 1.0 ) && m_dd_equal( dir1_len, 1.0 ) )
    {
        if ( vec3d_vv_equal( & dir0, v0  ) )
            result = 1;
        else
        {
            //   Also, if the prapagation directions of the two refframes
            //   fail the equality test, we take a look if they are exactly
            //   anti-coaxial (a popular failure case).

            vec3d_d_mul_v(
                 -1.0,
                & dir0
                );

            if ( vec3d_vv_equal( & dir0, v0 ) )
                ART_ERRORHANDLING_WARNING(
                    "anti-coaxial refframe and vector detected"
                    );
            else
            {
                double  cos_dir0dir1 =
                      -1.0
                    * vec3d_vv_dot(
                          & dir0,
                            v0
                          );

                double  cosMaxDeviation = cos( d0 );

                if ( cos_dir0dir1 > M_ABS(cosMaxDeviation) )
                    result = 1;
                else
                    ART_ERRORHANDLING_WARNING(
                        "non coaxial refframe and vector detected - "
                        "spread is %2.1f degrees, max allowed would be "
                        "%2.1f degrees"
                        ,   acos(cos_dir0dir1) * MATH_RAD_TO_DEG
                        ,   acos(cosMaxDeviation) * MATH_RAD_TO_DEG
                        );
            }

        }
    }
    else
        ART_ERRORHANDLING_WARNING(
            "refframe with non-unit length of its direction vector length detected during coaxiality check"
            );

    //   In both cases that generate warnings, the entire test is marked
    //   as failure, so the calling assertion will likely terminate the
    //   program anyway. And the warnings serve as an additional information.

    return result;
}

unsigned int arrefframe_rf_rf_equal(
        const ART_GV            * art_gv,
        const ArReferenceFrame  * r0,
        const ArReferenceFrame  * r1
        )
{
    (void) art_gv;
    
    unsigned int  result = 1;

    if ( ! vec3d_vv_equal(
               & ARREFFRAME_RF_I( *r0, 0 ),
               & ARREFFRAME_RF_I( *r1, 0 )
               )
       )
        result = 0;

    if ( ! vec3d_vv_equal(
               & ARREFFRAME_RF_I( *r0, 1 ),
               & ARREFFRAME_RF_I( *r1, 1 )
               )
       )
        result = 0;

    return result;
}

unsigned int arrefframe_rf_rf_d_equal(
        const ART_GV            * art_gv,
        const ArReferenceFrame  * r0,
        const ArReferenceFrame  * r1,
        const double              max_relative_error
        )
{
    (void) art_gv;
    
    unsigned int  result = 1;

    if ( ! vec3d_vvd_equal(
               & ARREFFRAME_RF_I( *r0, 0 ),
               & ARREFFRAME_RF_I( *r1, 0 ),
                 max_relative_error
               )
       )
        result = 0;

    if ( ! vec3d_vvd_equal(
               & ARREFFRAME_RF_I( *r0, 1 ),
               & ARREFFRAME_RF_I( *r1, 1 ),
                 max_relative_error
               )
       )
        result = 0;

    return result;
}

void arrefframe_rf_debugprintf(
        const ART_GV            * art_gv,
        const ArReferenceFrame  * r0
        )
{
    printf("Reference frame:\n");

    printf( "R 0: " );
    vec3d_v_debugprintf(
          art_gv,
        & ARREFFRAME_RF_I( *r0, 0 )
        );
    printf( "\n" );

    printf( "R 1: " );
    vec3d_v_debugprintf(
          art_gv,
        & ARREFFRAME_RF_I( *r0, 1 )
        );
    printf( "\n" );

    Vec3D  dir;

    vec3d_vv_cross_v(
        & ARREFFRAME_RF_I( *r0, 0 ),
        & ARREFFRAME_RF_I( *r0, 1 ),
        & dir
        );

    printf( "Dir: " );
    vec3d_v_debugprintf(
          art_gv,
        & dir
        );
    printf( "\n" );
}

// ===========================================================================
