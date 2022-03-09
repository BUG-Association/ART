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

#define ART_MODULE_NAME     ArcMeasurementEstimators

#import "ArcMeasurementEstimators.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArcLineMeasurementEstimator

- (id) init
        : (double) newLength
{
    self = [ super init ];
    
    if ( self )
    {
        length = newLength;
    }
    
    return self;
}

- (int) furtherLineSubdivisionNecessary
        : (ArLineEstimationData *)  dat
{
    double  subLengths, error = 1.0;

    length       = pnt3d_pp_dist(&dat->pnt3D[0].coord, &dat->pnt3D[2].coord);
    subLength[0] = pnt3d_pp_dist(&dat->pnt3D[0].coord, &dat->pnt3D[1].coord);
    subLength[1] = pnt3d_pp_dist(&dat->pnt3D[1].coord, &dat->pnt3D[2].coord);

    subLengths = subLength[0] + subLength[1];

    if ( !m_d_is_tiny(subLengths) )
        error = fabs(1.0 - length / subLengths);

    length = subLengths;
    return ( error > dat->maxError);
}

- (void) performLineEstimation
        : (ArLineEstimationData *)  dat
{
    if (subLength[0] == 0.0 && subLength[1] == 0.0)
    {
        subLength[0] = pnt3d_pp_dist(&dat->pnt3D[0].coord, &dat->pnt3D[1].coord);
        subLength[1] = pnt3d_pp_dist(&dat->pnt3D[1].coord, &dat->pnt3D[2].coord);
        length = subLength[0] + subLength[1];
    }
}

- (ArcObject <ArpLineEstimation2D> *) newLineSubEstimator
        : (int) index
{
    return
        [ ALLOC_INIT_OBJECT(ArcLineMeasurementEstimator)
            :   subLength[index]
            ];
}

- (void) averageLineSubEstimates
        : (ArcObject <ArpLineEstimation2D> **) estimator
{
    length = [ estimator[0] arcLength ] + [ estimator[1] arcLength ];
}

- (double) arcLength
{
    return length;
}

@end

@implementation ArcTriangleMeasurementEstimator

- (id) init
        : (double) newArea
{
    self = [ super init ];
    
    if ( self )
    {
        area = newArea;
    }
    
    return self;
}

- (int) furtherTriangleSubdivisionNecessary
        : (ArTriangleEstimationData *)  dat
{
    double  subAreas, subWeight[4], error = 1.0;

    if (area == 0.0)
    {
        double  weight =
            ( dat->valid[0] + dat->valid[1] + dat->valid[2] ) / 3.0;
        area = triangle_ppp_area(&dat->pnt3D[0].coord, &dat->pnt3D[1].coord,
                                 &dat->pnt3D[2].coord) * weight;
    }

    subWeight[0] = ( dat->valid[0] + dat->valid[3] + dat->valid[5] ) / 3.0;
    subWeight[1] = ( dat->valid[3] + dat->valid[4] + dat->valid[5] ) / 3.0;
    subWeight[2] = ( dat->valid[3] + dat->valid[1] + dat->valid[4] ) / 3.0;
    subWeight[3] = ( dat->valid[5] + dat->valid[4] + dat->valid[2] ) / 3.0;

    subArea[0] = triangle_ppp_area(&dat->pnt3D[0].coord, &dat->pnt3D[3].coord,
                                   &dat->pnt3D[5].coord) * subWeight[0];
    subArea[1] = triangle_ppp_area(&dat->pnt3D[3].coord, &dat->pnt3D[4].coord,
                                   &dat->pnt3D[5].coord) * subWeight[1];
    subArea[2] = triangle_ppp_area(&dat->pnt3D[3].coord, &dat->pnt3D[1].coord,
                                   &dat->pnt3D[4].coord) * subWeight[2];
    subArea[3] = triangle_ppp_area(&dat->pnt3D[5].coord, &dat->pnt3D[4].coord,
                                   &dat->pnt3D[2].coord) * subWeight[3];

    subAreas = subArea[0] + subArea[1] + subArea[2] + subArea[3];

    if ( !m_d_is_tiny(subAreas) )
        error = fabs(1.0 - area / subAreas);

    area = subAreas;

    return ( error > dat->maxError);
}

- (void) performTriangleEstimation
        : (ArTriangleEstimationData *)  dat
{
    if (   subArea[0] == 0.0 && subArea[1] == 0.0
        && subArea[2] == 0.0 && subArea[3] == 0.0)
    {
        double  subWeight[4];

        subWeight[0] = ( dat->valid[0] + dat->valid[3] + dat->valid[5] ) / 3.0;
        subWeight[1] = ( dat->valid[3] + dat->valid[4] + dat->valid[5] ) / 3.0;
        subWeight[2] = ( dat->valid[3] + dat->valid[1] + dat->valid[4] ) / 3.0;
        subWeight[3] = ( dat->valid[5] + dat->valid[4] + dat->valid[2] ) / 3.0;

        subArea[0] = triangle_ppp_area(&dat->pnt3D[0].coord,
                                       &dat->pnt3D[3].coord,
                                       &dat->pnt3D[5].coord) * subWeight[0];
        subArea[1] = triangle_ppp_area(&dat->pnt3D[3].coord,
                                       &dat->pnt3D[4].coord,
                                       &dat->pnt3D[5].coord) * subWeight[1];
        subArea[2] = triangle_ppp_area(&dat->pnt3D[3].coord,
                                       &dat->pnt3D[1].coord,
                                       &dat->pnt3D[4].coord) * subWeight[2];
        subArea[3] = triangle_ppp_area(&dat->pnt3D[5].coord,
                                       &dat->pnt3D[4].coord,
                                       &dat->pnt3D[2].coord) * subWeight[3];

        area = subArea[0] + subArea[1] + subArea[2] + subArea[3];
    }
}

- (ArcObject <ArpTriangleEstimation2D> *) newTriangleSubEstimator
        : (int) index
{
    return [ ALLOC_OBJECT(ArcTriangleMeasurementEstimator)
             init: subArea[index] ];
}

- (void) averageTriangleSubEstimates
        : (ArcObject <ArpTriangleEstimation2D> **) estimator
{
    area = 0.0;

    for ( unsigned int i = 0; i < 4; i++ )
        area += [ estimator[i] area ];
}

- (ArcObject <ArpTriangleEstimation2D> *) newTrianglePartEstimator
        : (int) index
        : (int) numberOfParts
{
    return [ ALLOC_INIT_OBJECT(ArcTriangleMeasurementEstimator) ];
}

- (void) addTrianglePartEstimates
        : (int) numberOfParts
        : (ArcObject <ArpTriangleEstimation2D> **) estimator
{
    area = 0.0;

    for ( int i = 0; i < numberOfParts; i++ )
        area += [ estimator[i] area ];
}

- (double) area
{
    return area;
}

@end

// ===========================================================================
