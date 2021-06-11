/* ===========================================================================

    Copyright (c) 1996-2020 The ART Development Team
    ------------------------------------------------

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

/* -----------------------------------------------------
        Macros for Surface Nodes
------------------------------------------------------*/

#define SUB_COLOUR      ((ArNode <ArpSpectrum>*) ARNUNARY_SUBNODE)
#define SUB_MATERIAL    ((ArNode <ArpVolumeMaterial>*) ARNUNARY_SUBNODE)
// TODO: remove this and any references to it, as it should be handled by SuppliedMaterialSurfaceMaterial
#define USER_SUPPLIED_SURFACE_MATERIAL_INTO     SUB_MATERIAL

/* -----------------------------------------------------
        Macros for BSDF-Samples
------------------------------------------------------*/

#define FULL_LIGHT_BSDFSAMPLE_ATTENUATION  ARFULLIGHTBSDFSAMPLE_ATTENUATION(*bsdfsample)
#define FULL_LIGHT_BSDFSAMPLE_COSINE       ARFULLIGHTBSDFSAMPLE_COSINE(*bsdfsample)
#define FULL_LIGHT_BSDFSAMPLE_DIRECTION    ARFULLIGHTBSDFSAMPLE_VECTOR(*bsdfsample)
#define FULL_LIGHT_BSDFSAMPLE_PROBABILITY  ARFULLIGHTBSDFSAMPLE_PROBABILITY(*bsdfsample)
#define FULL_LIGHT_BSDFSAMPLE_SPDIR        ARFULLIGHTBSDFSAMPLE_SPDIR(*bsdfsample)

#define BSDFSAMPLE_ATTENUATION                  ARBSDFSAMPLE_ATTENUATION(*bsdfsample)
#define BSDFSAMPLE_COSINE_WORLDSPACE            ARBSDFSAMPLE_COSINE(*bsdfsample)
#define BSDFSAMPLE_VECTOR_WORLDSPACE            ARBSDFSAMPLE_VECTOR(*bsdfsample)
#define BSDFSAMPLE_PROBABILITY                  ARBSDFSAMPLE_PROBABILITY(*bsdfsample)
#define BSDFSAMPLE_DIRECTION_COSINE_WORLDSPACE  ARBSDFSAMPLE_DIRECTION_COSINE(*bsdfsample)

#define BSDFSAMPLE_ATTENUATION_SO_FAR \
    ARFULLIGHTBSDFSAMPLE_ATTENUATION_SO_FAR(*bsdfsample)

#define BSDFSAMPLE_FREELISTS    ARSGC_BSDFSAMPLE_FREELISTS(*context)

/* -----------------------------------------------------
        Macros for incoming and outgoing directions
------------------------------------------------------*/

        // ------ Incoming Direction-------

#define INCOMING_RAY_WORLDSPACE \
    ARCINTERSECTION_WORLDSPACE_INCOMING_RAY(incomingDirectionAndLocation)

#define INCOMING_VECTOR_WORLDSPACE \
    ARCINTERSECTION_WORLDSPACE_VECTOR(incomingDirectionAndLocation)

#define INCOMING_VECTOR_WORLDSPACE_FULL_LIGHT \
    INCOMING_VECTOR_WORLDSPACE

#define INCOMING_DIRECTION_COSINE_WORLDSPACE \
    ARDIRECTIONCOSINE(INCOMING_VECTOR_WORLDSPACE,INCOMING_COSINE_WORLDSPACE)

#define AVG_INVERSE_IOR             ARCPHASEINTERFACE_AVG_INV_IOR(\
                                            *ARCINTERSECTION_PHASEINTERFACE(incomingDirectionAndLocation))
#define AVG_IOR                         ARCPHASEINTERFACE_AVG_IOR(\
                                            *ARCINTERSECTION_PHASEINTERFACE(incomingDirectionAndLocation))

#define AVG_IOR_I                   ARCPHASEINTERFACE_AVG_IOR_I(\
                                            *ARCINTERSECTION_PHASEINTERFACE(incomingDirectionAndLocation))
#define AVG_IOR_T                   ARCPHASEINTERFACE_AVG_IOR_T(\
                                            *ARCINTERSECTION_PHASEINTERFACE(incomingDirectionAndLocation))

#define INCOMING_DIRECTION_FULL_LIGHT          ARCINTERSECTION_FULL_LIGHT_INCOMING_DIRECTION(incomingDirectionAndLocation)
#define INCOMING_DIRECTION          ARCINTERSECTION_INCOMING_DIRECTION(incomingDirectionAndLocation)
#define INCOMING_COSINE_WORLDSPACE         ARCINTERSECTION_WORLDSPACE_COSINE(incomingDirectionAndLocation)
#define INCOMING_DIR_COSINE_FULL_LIGHT         ARCINTERSECTION_COSINE_FULL_LIGHT(incomingDirectionAndLocation)

#define INCOMING_DIR_ATTENUATION_SO_FAR \
                                    ARCINTERSECTION_ATTENUATION_SO_FAR(incomingDirectionAndLocation)
#define INCOMING_DIR_HEROATTENUATION_SO_FAR \
                                    ARCINTERSECTION_HEROATTENUATION_SO_FAR(incomingDirectionAndLocation)

        // ------ Outgoing Direction-------

#define OUTGOING_COSINE_WORLDSPACE  ARDIRECTIONCOSINE_COSINE(*outgoingDirection)
#define OUTGOING_VECTOR_WORLDSPACE  ARDIRECTIONCOSINE_VECTOR(*outgoingDirection)

#define OUTGOING_COSINE_WORLDSPACE_FULL_LIGHT \
    ARFULLIGHTSURFACEPOINTDIRECTION_COSINE(*outgoingDirection)
#define OUTGOING_VECTOR_WORLDSPACE_FULL_LIGHT \
    ARFULLIGHTSURFACEPOINTDIRECTION_VECTOR(*outgoingDirection)
#define OUTGOINGDIR_ATTENUATION_SO_FAR \
    ARFULLIGHTSURFACEPOINTDIRECTION_ATTENUATION_SO_FAR(*outgoingDirection)

        // ------ Sampled Direction-------

#define SAMPLED_COSINE_WORLDSPACE  ARDIRECTIONCOSINE_COSINE(*sampledDirection)
#define SAMPLED_VECTOR_WORLDSPACE  ARDIRECTIONCOSINE_VECTOR(*sampledDirection)

        // ----------- Normals-------------

#define EMISSION_SURFACE_NORMAL     ARCSURFACEPOINT_OBJECTSPACE_NORMAL(emissionLocation)
#define SURFACE_NORMAL_WORLDSPACE \
    ARCSURFACEPOINT_WORLDSPACE_NORMAL(incomingDirectionAndLocation)
#define SURFACE_NORMAL_WORLDSPACE_FULL_LIGHT \
    SURFACE_NORMAL_WORLDSPACE

/* -----------------------------------------------------
        Macros for trafos
------------------------------------------------------*/

#define CURRENT_TRAFO          \
    ARNTS_TRAFO(ARHIT_TS(ARCRAYHITINFO_HIT(*incomingDirectionAndLocation)))

#define TRANSFORM_BRDF_DIRECTIONS_TO_LOCAL_SYSTEM \
        \
        Trafo3D  world2local; \
        trafo3d_v_world2local_from_worldspace_normal_t( & SURFACE_NORMAL_WORLDSPACE, \
                                 & world2local ); \
        \
        Vec3D  localI, localO; \
        vec3d_v_trafo3d_v(  & INCOMING_VECTOR_WORLDSPACE, \
                            & world2local, \
                            & localI ); \
        vec3d_v_trafo3d_v( & OUTGOING_VECTOR_WORLDSPACE, \
                           & world2local, \
                           & localO); \
        vec3d_norm_v( & localI ); \
        vec3d_norm_v( & localO );


#define TRANSFORM_BSDFSAMPLE_DIRECTIONS_TO_LOCAL_SYSTEM \
        Trafo3D     local2world; \
        Trafo3D     world2local; \
        \
        trafo3d_v_world2local_from_worldspace_normal_t( & SURFACE_NORMAL_WORLDSPACE, & world2local ); \
        trafo3d_t_transpose_t( & world2local, & local2world ); \
        \
        Vec3D localI, localO; \
        vec3d_v_trafo3d_v( & INCOMING_VECTOR_WORLDSPACE, \
                           & world2local, \
                           & localI); \
        vec3d_norm_v( & localI);

#define TRANSFORM_V_FROM_LOCAL_TO_WORLD_V( localO, worldVec ) \
        vec3d_v_trafo3d_v( & (localO), & (local2world), & (worldVec) );

// If your surface model needs the Halfway vector, use this
// macro to produce it. The vectors have to be normalized,
// and localH will be as well!

#define DEFINE_LOCAL_HALFANGLE(_p_localI, _p_localO) \
        Vec3D   localH;  \
        vec3d_vv_sub_v(_p_localI, _p_localO, &localH); \
        vec3d_norm_v(&localH);

#define DEFINE_LOCAL_REFRACTION_HALFANGLE(_p_localI, _p_localO, _ior) \
        Vec3D   localH;  \
        { \
            double ior = (_ior); \
            vec3d_dv_mul_v_add_v(-ior, (_p_localI), (_p_localO), &localH); \
            if(ior < 1.0) \
                vec3d_negate_v(&localH); \
            vec3d_norm_v(&localH); \
        }


/* -----------------------------------------------------
        Sampling macros
------------------------------------------------------*/

//  Generates a cosine-weighted hemisphere direction.
//
//  We first uniformly sample a point on the unit sphere, then we move
//  the sphere in the direction of the normal by one unit so that it touches
//  the point of interest and then we convert the point on the sphere to
//  the direction from the point of interest. This yields a direction on
//  the hemisphere with cosine-weighted probability density.
//
//  The "while" condition only serves as a cornercase check which
//  handles the singular situation when the generated point on
//  the sphere is identical to the point of interest.

/*
#define SAMPLE_HEMISPHERE_COSINE_WEIGHTED(_localVec, _worldVec) \
do { \
    double  sqrlen; \
    \
    do \
    { \
        double  phi, z, r; \
        \
        [ RANDOM_GENERATOR getValuesFromNewSequences: & z : & phi ]; \
        \
        phi *= MATH_2_MUL_PI; \
        z = 1.0 - 2.0 * z; \
        r = sqrt ( M_MAX( 1.0 - z * z, 0.0 ) ); \
        (_localVec) = VEC3D(r * cos(phi), r * sin(phi), z + 1.0); \
        \
        TRANSFORM_V_FROM_LOCAL_TO_WORLD_V( (_localVec), (_worldVec) ); \
        \
        sqrlen = vec3d_v_sqrlen(&(_worldVec)); \
    } \
    while ( sqrlen < MATH_TINY_DOUBLE ); \
    \
    const double len_inv = 1.0 / sqrt(sqrlen); \
    \
    vec3d_d_mul_v( len_inv, & (_localVec) ); \
    vec3d_d_mul_v( len_inv, & (_worldVec) ); \
} while (0)
*/

#define SAMPLE_HEMISPHERE_COSINE_WEIGHTED(_localVec, _worldVec) \
    double  sqrlen; \
    \
        double  phi, z, r; \
        \
        [ RANDOM_GENERATOR getValuesFromNewSequences: & z : & phi ]; \
        \
        phi *= MATH_2_MUL_PI; \
        z = 1.0 - 2.0 * z; \
        r = sqrt ( M_MAX( 1.0 - z * z, 0.0 ) ); \
        (_localVec) = VEC3D(r * cos(phi), r * sin(phi), z + 1.0); \
        \
        TRANSFORM_V_FROM_LOCAL_TO_WORLD_V( (_localVec), (_worldVec) ); \
        \
        sqrlen = vec3d_v_sqrlen(&(_worldVec)); \
    \
    const double len_inv = 1.0 / sqrt(sqrlen); \
    \
    vec3d_d_mul_v( len_inv, & (_localVec) ); \
    vec3d_d_mul_v( len_inv, & (_worldVec) );

/* -----------------------------------------------------
        Miscellaneous Macros
------------------------------------------------------*/

#define PHASE_INTERFACE \
    ARCINTERSECTION_PHASEINTERFACE(incomingDirectionAndLocation)

#define PHASE_INTERFACE_HAS_REALVALUED_IOR \
    ARCINTERSECTION_PHASE_INTERFACE_HAS_REALVALUED_IOR(incomingDirectionAndLocation)
#define PHASE_INTERFACE_HAS_COMPLEX_IOR \
    ARCINTERSECTION_PHASE_INTERFACE_HAS_COMPLEX_IOR(incomingDirectionAndLocation)

#define PHASE_INTERFACE_MATERIAL_FROM \
    (ArNode<ArpVolumeMaterial> *)(PHASE_INTERFACE->mFrom)
#define PHASE_INTERFACE_MATERIAL_INTO \
    (ArNode<ArpVolumeMaterial> *)(PHASE_INTERFACE->mInto)

#define PHASE_INTERFACE_HAS_WAVELENGTH_DEPENDENT_IOR \
    ARCPHASEINTERFACE_HAS_IOR_FOR_EACH_WAVELENGTH( \
        *ARCINTERSECTION_PHASEINTERFACE(incomingDirectionAndLocation) \
        )

#define RANDOM_GENERATOR       ARSGC_RANDOM_GENERATOR(*context)
#define PHASE_INTERFACE_CACHE  ARSGC_PHASE_INTERFACE_CACHE(*context)

// See PBRT p. 60. True if localI and localO point
// into different hemispheres, i.e., light is reflected.

#define SAME_HEMISPHERE(_localI, _localO) (ZC(_localI)*ZC(_localO) < 0.0)

#define PHONG_FACTOR(__cos, __shininess, __scale) \
                (m_dd_pow( (__cos), __shininess ) * __scale)

