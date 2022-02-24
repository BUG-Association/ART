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

ART_MODULE_INTERFACE(ArpMapping)

#import "ArpParameterisation.h"
#import "ArpEstimation2D.h"
#import "ArpTrafo.h"
#import "ArpNode.h"

typedef enum ArMappingCriteria
{
    armapping_default           = 0x00,
    armapping_barycentric       = 0x01,
    armapping_rectangular       = 0x02,
    armapping_areaestimation    = 0x04,
    armapping_lightmap          = 0x08,
    armapping_lightsource       = 0x10,
    armapping_cylindric         = 0x20,
    armapping_vertex_coords     = 0x40
}
ArMappingCriteria;

#define AREDGEINDEX_INVALID     -1
#define ARFACEINDEX_INVALID     -1
#define ARPATCHINDEX_INVALID    -1

typedef struct ArPatchEdge
{
    int  patch;
    int  edge;
}
ArPatchEdge;

@protocol ArpMapping <ArpNode>

/* ---------------------------------------------------------------------------
    'numberOfPatches', 'numberOfPatchesOnFace'
        Returns the total number of patches on a shape or on a shape face,
        respectively.
--------------------------------------------------------------------------- */

- (unsigned int) numberOfPatches
        ;

- (unsigned int) numberOfPatchesOnFace
        : (unsigned int)  face
        ;

/* ---------------------------------------------------------------------------
    'faceIndexOfPatch'
        Returns the index of the shape face on which a given patch lies.
--------------------------------------------------------------------------- */

- (int) faceIndexOfPatch
        : (unsigned int)  patchIndex
        ;

/* ---------------------------------------------------------------------------
    'parameterisationOfPatch'
        Returns the class object of the parameterisation used
        on a given patch.
--------------------------------------------------------------------------- */

- (Class) parameterisationOfPatch
        : (unsigned int) patchIndex
        ;

/* ---------------------------------------------------------------------------
    'getPnt2DE_for_ObjectSpacePnt3DE'
        Calculates the patch index and 2D coordinates for a 3D point
        given in object coordinates on a particular shape.
--------------------------------------------------------------------------- */

- (BOOL) getPnt2DE_for_ObjectSpacePnt3DE
        : (const struct ArTraversalState *) traversalState
        : (const Pnt3DE *) inPoint3D
        : (Pnt2DE *) outPoint2D
        ;

- (void) getPnt2DE_for_ObjectSpacePnt3DE
        : (const struct ArTraversalState *) traversalState
        : (const Pnt3DE *) inPoint3D
        : (unsigned int) numberOfPoints
        : (BOOL *) outPointValidity
        : (Pnt2DE *) outPoint2D
        ;

/* ---------------------------------------------------------------------------
    'getObjectSpacePnt3DE_for_Pnt2DE'
        Calculates the face index, 3D coordinates and surface normal of a
        given 2D point on a given patch. All computed 3D coordinates are in
        the object coordinate system of the shape in question.
--------------------------------------------------------------------------- */

- (BOOL) getObjectSpacePnt3DE_for_Pnt2DE
        : (const struct ArTraversalState *) traversalState
        : (const Pnt2DE *) inPoint2D
        : (Pnt3DE *) outPoint3D
        ;

- (void) getObjectSpacePnt3DE_for_Pnt2DE
        : (const struct ArTraversalState *) traversalState
        : (const Pnt2DE *) inPoint2D
        : (unsigned int) numberOfPoints
        : (BOOL *) outPointValidity
        : (Pnt3DE *) outPoint3D
        ;

/* ---------------------------------------------------------------------------
    'get_UV_BasisVectors'
        Returns the two basis vectors u and v. The return value is
        YES if UV-vectors can be calculated.
--------------------------------------------------------------------------- */

- (BOOL) get_UV_BasisVectors  // object space!
        : (const struct ArTraversalState *) traversalState
        : (const Pnt3DE *) inPoint3D
        : (Vec3D *) outBasisVectorU
        : (Vec3D *) outBasisVectorV
        ;

/* ---------------------------------------------------------------------------
    'getAdjacentPatchEdge'
        Returns the edge adjacent to a given patch edge. The return value is
        NO if there is no adjacent patch, and YES if there is one.
--------------------------------------------------------------------------- */

- (BOOL) getAdjacentPatchEdge
        : (ArPatchEdge *)  inEdge
        : (ArPatchEdge *)  outEdge
        ;

@end

/* ---------------------------------------------------------------------------
        Applicable to all mappings.
--------------------------------------------------------------------------- */

#define ARPMAPPING_UV_BASIS_EPSILON     0.00001

#define ARPMAPPING_DEFAULT_IMPLEMENTATION(__numberOfPatches) \
- (unsigned int) numberOfPatches \
{ \
    return (__numberOfPatches); \
} \
- (void) getPnt2DE_for_ObjectSpacePnt3DE \
        : (const ArTraversalState *) traversalState \
        : (const Pnt3DE *) inPoint3D \
        : (unsigned int) numberOfPoints \
        : (BOOL *) outPointValidity \
        : (Pnt2DE *) outPoint2D \
{ \
    for ( unsigned int i = 0; i < numberOfPoints; i++ ) \
    { \
        BOOL temp = \
        [ self getPnt2DE_for_ObjectSpacePnt3DE \
            :   traversalState \
            : & inPoint3D[i] \
            : & outPoint2D[i] ]; \
        if ( outPointValidity ) \
            outPointValidity[i] = temp; \
    } \
} \
- (void) getObjectSpacePnt3DE_for_Pnt2DE \
        : (const ArTraversalState *) traversalState \
        : (const Pnt2DE *) inPoint2D \
        : (unsigned int) numberOfPoints \
        : (BOOL *) outPointValidity \
        : (Pnt3DE *) outPoint3D \
{ \
    for ( unsigned int i = 0; i < numberOfPoints; i++ ) \
    { \
        BOOL temp = \
        [ self getObjectSpacePnt3DE_for_Pnt2DE \
            :   traversalState \
            : & inPoint2D[i]  \
            : & outPoint3D[i] ]; \
        if ( outPointValidity ) \
            outPointValidity[i] = temp; \
    } \
} \
\
- (BOOL) get_UV_BasisVectors \
        : (const struct ArTraversalState *) traversalState \
        : (const Pnt3DE *) inPoint3D \
        : (Vec3D *) outBasisVectorU \
        : (Vec3D *) outBasisVectorV \
{ \
    Pnt2DE  mainPoint2D; \
\
    if (    [ self getPnt2DE_for_ObjectSpacePnt3DE \
                :   traversalState \
                :   inPoint3D \
                : & mainPoint2D ] ) \
    { \
        Pnt2DE  secondaryPoint2D_U = mainPoint2D; \
\
        PNT2DE_U(secondaryPoint2D_U) += ARPMAPPING_UV_BASIS_EPSILON; \
\
        Pnt3DE  secondaryPoint3D_U; \
\
        if (    [ self getObjectSpacePnt3DE_for_Pnt2DE \
                    :   traversalState \
                    : & secondaryPoint2D_U \
                    : & secondaryPoint3D_U ] ) \
        { \
            vec3d_pp_sub_v( \
                & PNT3DE_COORD( secondaryPoint3D_U ), \
                & PNT3DE_COORD( *inPoint3D ), \
                  outBasisVectorU ); \
        } \
        else \
        { \
            PNT2DE_U(secondaryPoint2D_U) -= 2 * ARPMAPPING_UV_BASIS_EPSILON; \
\
            if (    [ self getObjectSpacePnt3DE_for_Pnt2DE \
                        :   traversalState \
                        : & secondaryPoint2D_U \
                        : & secondaryPoint3D_U ] ) \
            { \
                vec3d_pp_sub_v( \
                    & PNT3DE_COORD( *inPoint3D ), \
                    & PNT3DE_COORD( secondaryPoint3D_U ), \
                      outBasisVectorU ); \
            } \
            else \
                return NO; \
        } \
\
        Pnt2DE  secondaryPoint2D_V = mainPoint2D; \
\
        PNT2DE_V(secondaryPoint2D_V) += ARPMAPPING_UV_BASIS_EPSILON; \
\
        Pnt3DE  secondaryPoint3D_V; \
\
        if (    [ self getObjectSpacePnt3DE_for_Pnt2DE \
                    :   traversalState \
                    : & secondaryPoint2D_V \
                    : & secondaryPoint3D_V ] ) \
        { \
            vec3d_pp_sub_v( \
                & PNT3DE_COORD( secondaryPoint3D_V ), \
                & PNT3DE_COORD( *inPoint3D ), \
                  outBasisVectorV ); \
        } \
        else \
        { \
            PNT2DE_U(secondaryPoint2D_V) -= 2 * ARPMAPPING_UV_BASIS_EPSILON; \
\
            if (    [ self getObjectSpacePnt3DE_for_Pnt2DE \
                        :   traversalState \
                        : & secondaryPoint2D_V \
                        : & secondaryPoint3D_V ] ) \
            { \
                vec3d_pp_sub_v( \
                    & PNT3DE_COORD( *inPoint3D ), \
                    & PNT3DE_COORD( secondaryPoint3D_V ), \
                      outBasisVectorV ); \
            } \
            else \
                return NO; \
        } \
\
        vec3d_norm_v( outBasisVectorU ); \
        vec3d_norm_v( outBasisVectorV ); \
\
        return YES; \
    } \
    else \
        return NO; \
}

#define ARPMAPPING_PATCH_PARAMETERISATION(__parameterisation) \
- (Class) parameterisationOfPatch \
    : (unsigned int) patchIndex \
{ \
    (void) patchIndex; \
\
    return [ __parameterisation class ]; \
}

/* ---------------------------------------------------------------------------
        Applicable if a one-to-one correspondence between faces
        and patches exists.
--------------------------------------------------------------------------- */

#define ARPMAPPING_ONE_TO_ONE_PATCHING \
- (unsigned int) numberOfPatchesOnFace \
    : (unsigned int) faceIndex \
{ \
    (void) faceIndex; \
\
    return 1; \
} \
- (int) faceIndexOfPatch \
    : (unsigned int) patchIndex \
{ \
    return patchIndex; \
}

/* ---------------------------------------------------------------------------
        This is a shortcut for cases where one-to-one correspondence between
        faces and patches exists and no two faces share an edge across which
        one would possibly want to interpolate.
--------------------------------------------------------------------------- */

#define ARPMAPPING_NO_ADJACENT_PATCHES \
- (BOOL) getAdjacentPatchEdge \
        : (ArPatchEdge *)  inEdge \
        : (ArPatchEdge *)  outEdge \
{ \
    (void) inEdge; \
\
    outEdge->patch = ARPATCHINDEX_INVALID; \
    outEdge->edge  = AREDGEINDEX_INVALID; \
    return NO; \
}

// ===========================================================================
