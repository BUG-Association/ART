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

#define ART_MODULE_NAME     LengthAndAreaComputations

#import "LengthAndAreaComputations.h"
#import "ArcMeasurementEstimators.h"
#import "ART_Scenegraph.h"
#import "FoundationAssertionMacros.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


double averageLengthU(
        ART_GV                     * art_gv,
        ArSamplingContext2D        * context,
        ArcObject <ArpSampling2D>  * sampler
        )
{
    int     i, resolution = 15;
    double  length = 0.0;
    double  stride = 1.0 / (double) resolution;

    for( i = 0; i < resolution + 1; i++ )
    {
        Line2D line;

        XC(line.p[0]) = 0.0;
        YC(line.p[0]) = i * stride;
        XC(line.p[1]) = 1.0;
        YC(line.p[1]) = i * stride;

        length += lengthOfLine( art_gv, &line, context, sampler);
    }
    length /= resolution;

    return length;
}

double averageLengthV(
        ART_GV                     * art_gv,
        ArSamplingContext2D        * context,
        ArcObject <ArpSampling2D>  * sampler
        )
{
    int     i, resolution = 15;
    double  length = 0.0;
    double  stride = 1.0 / (double) resolution;

    for( i = 0; i < resolution + 1; i++ )
    {
        Line2D line;

        XC(line.p[0]) = i * stride;
        YC(line.p[0]) = 0.0;
        XC(line.p[1]) = i * stride;
        YC(line.p[1]) = 1.0;

        length += lengthOfLine( art_gv, &line, context, sampler);
    }
    length /= resolution;

    return length;
}

double lengthOfLine(
        ART_GV                     * art_gv,
        Line2D                     * line,
        ArSamplingContext2D        * context,
        ArcObject <ArpSampling2D>  * sampler
        )
{
    double  length;
    ArcLineMeasurementEstimator  * estimator;

    estimator = [ ALLOC_INIT_OBJECT(ArcLineMeasurementEstimator) ];
    [ sampler sampleLine: line : context : estimator ];
    length = [ estimator arcLength ];
    RELEASE_OBJECT(estimator);

    return length;
}

double areaOfParameterRange(
        ART_GV                     * art_gv,
        ArcParameterRange          * range,
        ArSamplingContext2D        * context,
        ArcObject <ArpSampling2D>  * sampler
        )
{
    double  area;
    ArcTriangleMeasurementEstimator  * estimator;

    estimator = [ ALLOC_INIT_OBJECT(ArcTriangleMeasurementEstimator) ];

    [ [ SC2D_MAPPING(*context) parameterisationOfPatch
        :   SC2D_PATCHINDEX(*context) ] sample
          :   range
          :   context
          :   sampler
          :   estimator ];

    area = [ estimator area ];

    RELEASE_OBJECT(estimator);

    return area;
}

double areaOfPatch(
        ART_GV                     * art_gv,
        ArSamplingContext2D        * context,
        ArcObject <ArpSampling2D>  * sampler
        )
{
    ArcParameterRange  * range;
    ArcTriangleMeasurementEstimator  * estimator;

    estimator = [ ALLOC_INIT_OBJECT(ArcTriangleMeasurementEstimator) ];

    range = [ [ SC2D_MAPPING(*context)
                parameterisationOfPatch: SC2D_PATCHINDEX(*context) ]
              wholeRange
                :   art_gv
                ];

    [ [ SC2D_MAPPING(*context) parameterisationOfPatch
          : SC2D_PATCHINDEX(*context)
          ]
        sample
        :   range
        :   context
        :   sampler
        :   estimator
        ];

    double area = [ estimator area ];

    RELEASE_OBJECT(range);
    RELEASE_OBJECT(estimator);

    return area;
}

double areaOfShape(
        ART_GV                        * art_gv,
        ArNode <ArpShape>             * shape,
        struct ArTraversalState       * state,
        id <ArpTriangleEstimation2D>    estimator
        )
{
    double  area = 0.0;
    id  mapping = [ shape createMappingFor: 0 ];
    ArSamplingContext2D  context;

    SC2D_TRAVERSALSTATE(context) = state;

    SC2D_MAPPING(context) = mapping;
    SC2D_SHAPE(context) = shape;

    for( unsigned int i = 0; i < [ mapping numberOfPatches ]; i++ )
    {
        SC2D_PATCHINDEX(context) = i;
        area += areaOfPatch(art_gv,&context, ((ArcObject <ArpSampling2D> *)estimator));
    }

    return area;
}

typedef struct ArAreaCalculationStruct
{
    double  area;
    id <ArpTriangleEstimation2D>  estimator;
}
ArAreaCalculationStruct;

@interface ArnVisitor ( AreaEstimation )

- (ArNode *) objectArea
        : (ArNode *) node
        : (ArAreaCalculationStruct *) aStruct
        ;

@end

@implementation ArnVisitor ( AreaEstimation )

- (ArNode *) objectArea
        : (ArNode *) node
        : (ArAreaCalculationStruct *) aStruct
{
    if ( [ node conformsToArProtocol: ARPROTOCOL(ArpShape) ] )
    {
        ASSERT_PROTOCOL_SUPPORT(
            node,
            ArpShape
            );

        aStruct->area +=
            areaOfShape(
                  art_gv,
                  ((ArNode <ArpShape> *)node),
                & self->state,
                  aStruct->estimator
                );

        [ self pruneTraversalAtNode
            :   node
            ];

        return node;
    }

    return node;
}

@end

double areaOfObject(
        ART_GV                        * art_gv,
        id                              object,
        struct ArTraversalState       * state,
        id <ArpTriangleEstimation2D>    estimator
        )
{
    ArnVisitor  * visitor = [ ALLOC_INIT_OBJECT(ArnVisitor) ];
    ArAreaCalculationStruct  aStruct;

    aStruct.area = 0.0;
    aStruct.estimator = estimator;

    visitor->state = *state;

    [visitor modifyPreOrder: arvisitmode_geom_dag_with_attributes
                           : object
                           : @selector(objectArea::)
                           : &aStruct ];

    RELEASE_OBJECT(visitor);

    return aStruct.area;
}


// ===========================================================================
