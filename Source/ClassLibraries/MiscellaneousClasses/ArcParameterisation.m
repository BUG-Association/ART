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

#define ART_MODULE_NAME     ArcParameterisation

#include "ART_Foundation.h"

#import "ArcParameterisation.h"
#import "ArpSampling2D.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

@implementation ArcBarycentricParameterisation

+ (ArcParameterRange *) wholeRange
        : (ART_GV *) new_art_gv
{
    ArcTriangularParameterRange  * range =
        [ ALLOC_INIT_OBJECT_AGV(new_art_gv,ArcTriangularParameterRange) ];

    range->p[0] = PNT2D( 0.0, 0.0 );
    range->p[1] = PNT2D( 1.0, 0.0 );
    range->p[2] = PNT2D( 0.0, 1.0 );

    return range;
}

+ (double) crossproductArea
{
    return 0.5;
}

+ (void) getRandomPoint
        : (Pnt2D *) location
        : (ArcObject <ArpRandomGenerator> *) randomGenerator
{
    double  u, v;

    [ randomGenerator getValuesFromNewSequences
        : & u
        : & v ];

    if ( u + v > 1.0 )
    {
        u = 1.0 - u;
        v = 1.0 - v;
    }

    XC(*location) = u;
    YC(*location) = v;
}

+ (void) getRandomPointInRange
        : (Pnt2D *) location
        : (ArcParameterRange *) range
        : (ArcObject <ArpRandomGenerator> *) randomGenerator
{
    ArcTriangularParameterRange  * tpr =
        ((ArcTriangularParameterRange *) range);

    double  u, v;

    [ randomGenerator getValuesFromNewSequences
        : & u
        : & v ];

    if (u + v > 1.0) { u = 1.0 - u; v = 1.0 - v; }

    Pnt2D   tmp;

    c2_dc_mul_c(u,&tpr->p[0].c,&location->c);
    c2_dc_mul_c(v,&tpr->p[1].c,&tmp.c);
    c2_cc_add_c(&location->c,&tmp.c,&location->c);
    c2_dc_mul_c(v,&tpr->p[2].c,&tmp.c);
    c2_cc_add_c(&location->c,&tmp.c,&location->c);
}

+ (void) sample
        : (ArcParameterRange *) range
        : (struct ArSamplingContext2D *) context
        : (ArcObject <ArpSampling2D> *) sampler
        : (ArcObject <ArpTriangleEstimation2D> *) estimator
{
    if ( [ range isKindOfClass: [ ArcTriangularParameterRange class ] ] )
    {
        Triangle2D  t;

        [ range getTriangulation: & t ];

        [ ((ArcObject <ArpSampling2D> *)sampler) sampleTriangle
            : & t
            :   context
            :   estimator
            ];
    }
    else
        ART_ERRORHANDLING_FATAL_ERROR(
            "ArcBarycentricParameterisation: unsupported parameter range type"
            );
}

@end

@implementation ArcRectangularParameterisation

+ (ArcParameterRange *) wholeRange
        : (ART_GV *) new_art_gv
{
    ArcRectangularParameterRange  * range;

    range = [ ALLOC_INIT_OBJECT_AGV( new_art_gv, ArcRectangularParameterRange ) ];

    range->min = PNT2D( 0.0, 0.0 );
    range->max = PNT2D( 1.0, 1.0 );

    return range;
}

+ (double) crossproductArea
{
    return 1.0;
}

+ (void) getRandomPoint
        : (Pnt2D *) location
        : (ArcObject <ArpRandomGenerator> *) randomGenerator
{
    double  u, v;

    [ randomGenerator getValuesFromNewSequences
        : & u
        : & v ];

    XC(*location) = u;
    YC(*location) = v;
}

+ (void) getRandomPointInRange
        : (Pnt2D *) location
        : (ArcParameterRange *) range
        : (ArcObject <ArpRandomGenerator> *) randomGenerator
{
    ArcRectangularParameterRange  * rpr;
    double                          u, v;

    [ randomGenerator getValuesFromNewSequences
        : & u
        : & v ];

    rpr = ((ArcRectangularParameterRange *) range);
    XC(*location) = u * (XC(rpr->max)-XC(rpr->min)) + XC(rpr->min);
    YC(*location) = v * (YC(rpr->max)-YC(rpr->min)) + YC(rpr->min);
}

+ (void) sample
        : (ArcParameterRange *) range
        : (struct ArSamplingContext2D *) context
        : (ArcObject <ArpSampling2D> *) sampler
        : (ArcObject <ArpTriangleEstimation2D> *) estimator
{
    if ( [ range isKindOfClass: [ ArcRectangularParameterRange class ] ] )
    {
        Triangle2D  t[2];
        ArcObject <ArpTriangleEstimation2D>  * partEstimator[2];

        partEstimator[0] = [ estimator newTrianglePartEstimator: 1 : 2 ];
        partEstimator[1] = [ estimator newTrianglePartEstimator: 2 : 2 ];

        [ range getTriangulation: t ];
        [ sampler sampleTriangle: &t[0] : context : partEstimator[0] ];
        [ sampler sampleTriangle: &t[1] : context : partEstimator[1] ];

        [ estimator addTrianglePartEstimates
            :   2
            :   (ArcObject <ArpTriangleEstimation2D>  **) partEstimator
            ];

        RELEASE_OBJECT(partEstimator[0]);
        RELEASE_OBJECT(partEstimator[1]);
    }
    else
        ART_ERRORHANDLING_FATAL_ERROR(
            "ArcRectangularParameterisation: unsupported parameter range type"
            );
}

@end

// ===========================================================================
