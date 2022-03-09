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

#define ART_MODULE_NAME     ArnTriangleRayCasting

#import "ArpRayCasting_Categories.h"
#import "RayCastingCommonMacros.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArnTriangle ( RayCasting )

ARPRAYCASTING_DEFAULT_IMPLEMENTATION(ArnTriangle)
ARPRAYCASTING_SINGULAR_SHAPE_IMPLEMENTATION

- (ArNode <ArpVolumeMaterial> *) volumeMaterial_at_WorldPnt3D
        : (ArnRayCaster *) rayCaster
{
    return 0;
}

- (void) getIntersectionList
        : (ArnRayCaster *) rayCaster
        : (Range) range_of_t
        : (struct ArIntersectionList *) intersectionList
{

#ifdef ART_WITH_INTERSECTION_STATISTICS
    arnraycaster_count_test(rayCaster, ArnQuadrangle);
#endif

    *intersectionList = ARINTERSECTIONLIST_EMPTY;

    double  t;
    Pnt3D   intersectionLocalPoint;
    Pnt2D   intersectionTextureCoordinates;

    const Pnt3D * point_array = [ARNRAYCASTER_VERTICES(rayCaster) pointArray];

    unsigned int  result =
        triangledata_hit(
              triangleData,
              dim,
            & point_array[ ARARRAY_I( indexTable, 0 ) ],
            & point_array[ ARARRAY_I( indexTable, 1 ) ],
            & point_array[ ARARRAY_I( indexTable, 2 ) ],
            & OBJECTSPACE_RAY,
            & range_of_t,
              0, // rayCaster->unionOptions & arunion_backface_culling,
            & t,
            & intersectionLocalPoint,
            & intersectionTextureCoordinates
            );

    if ( result )
    {
        ArFaceOnShapeType  face_type =
            ( result > 0
              ?
              arface_on_shape_obverse | arface_on_shape_is_planar
              :
              arface_on_shape_reverse | arface_on_shape_is_planar
            );


        arintersectionlist_init_1(
              intersectionList,
              t,
              0,
              face_type,
              self,
              rayCaster
            );

        ArcIntersection  * intersection =
            INTERSECTIONLIST_HEAD(*intersectionList);

        SET_OBJECTSPACE_POINT(intersection, intersectionLocalPoint);
        
        const FVec3D  * normal_array =
            [ ARNRAYCASTER_VERTICES(rayCaster) normalArray ];
        
        if ( normal_array )
        {
            SET_OBJECTSPACE_NORMAL(intersection, VEC3D(0,0,0));

            double  w = YC(intersectionTextureCoordinates);
            double  v = XC(intersectionTextureCoordinates);
            double  u = 1. - w - v;
            
            Vec3D  uv, vv, wv;
            
            vec3d_fv_to_v(
                & normal_array[ ARARRAY_I( indexTable, 0 ) ],
                & uv
                );

            vec3d_d_mul_v(
                  u,
                & uv
                );

            vec3d_fv_to_v(
                & normal_array[ ARARRAY_I( indexTable, 1 ) ],
                & vv
                );

            vec3d_d_mul_v(
                  v,
                & vv
                );

            vec3d_fv_to_v(
                & normal_array[ ARARRAY_I( indexTable, 2 ) ],
                & wv
                );

            vec3d_d_mul_v(
                  w,
                & wv
                );

            vec3d_v_add_v(
                & uv,
                & OBJECTSPACE_NORMAL(intersection)
                );
            
            vec3d_v_add_v(
                & vv,
                & OBJECTSPACE_NORMAL(intersection)
                );
            
            vec3d_v_add_v(
                & wv,
                & OBJECTSPACE_NORMAL(intersection)
                );
        }
        
        TEXTURE_COORDS(intersection) = intersectionTextureCoordinates;
        FLAG_TEXTURE_COORDS_AS_VALID(intersection);

#ifdef ART_WITH_INTERSECTION_STATISTICS
        arnraycaster_count_intersection(rayCaster, ArnQuadrangle);
#endif
    }
}

- (void) calculateLocalNormalForIntersection
        : (ArcIntersection *) intersection
{
    vec3d_fv_to_v(
        & triangleData->normal,
        & OBJECTSPACE_NORMAL(intersection)
        );

    FLAG_OBJECTSPACE_NORMAL_AS_VALID(intersection);
}

@end

// ===========================================================================
