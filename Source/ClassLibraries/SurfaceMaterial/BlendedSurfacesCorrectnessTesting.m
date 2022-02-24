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

#define ART_MODULE_NAME     BlendedSurfacesCorrectnessTesting

#import "BlendedSurfacesCorrectnessTesting.h"
#import "SurfaceMaterialMacros.h"
#import "ART_ARM_Interface.h"
#import "ART_SurfaceMaterial.h"
#import "ArcIntersection.h"

// FIXME: This should probably be done in a nicer fashion (if possible)
#import "ART_Resources/include/ART_CanonicalSpectralData/Metals.arh"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#define SET_PSSPECTRUM_COLOUR_CHANNEL(_channel) \
    PNT2D( \
        spc_channel_lower_bound(art_gv, _channel), \
        randomVal ## _channel), \
    PNT2D( \
        spc_channel_lower_bound(art_gv, _channel) \
        + spc_channel_width(art_gv, _channel), \
        randomVal ## _channel)

#define RANDOM_VAL_FROM_RANGE(_min, _max) \
    (_min) + (rand() / (double)RAND_MAX) * ((_max) - (_min))

#define RANDOM_ATTENUATION \
    RANDOM_VAL_FROM_RANGE(0.0, 1.0)


typedef struct ArBlendedSurfacesTestingContext
{
    double                              cornerCaseProbability;

    ArcIntersection                   * intersection;
    ArBSDFSampleGenerationContext       sampleGenerationContext;
    double                              blendCoeff;

    int                                 currentTestSuccess;

    int                                 totalIterations;
    int                                 currentBatchIterations;
    int                                 currentIteration;

    double                              currentThetaIn;
    double                              currentSigmaIn;

    Vec3D                               currentDirectionOut;
    double                              currentCosineOut;

    int                                 successfulTestsCount;
    int                                 unsuccessfulTestsCount;

    int                                 inDirNormalCount;
    int                                 inDirRightAngleCount;
    int                                 inDirRandomCount;

    int                                 lowerMatDielectricsCount;
    int                                 lowerMatConductorCount;

    int                                 outSamplingByFirstSurfaceCount;
    int                                 outSamplingBySecondSurfaceCount;
}
ArBlendedSurfacesTestingContext;


void testing_context_alloc_init(
        ART_GV                            * art_gv,
        ArcObject <ArpRandomGenerator>    * randomGenerator,
        int                                 totalIterations,
        int                                 currentBatchIterations,
        double                              cornerCaseProbability,
        ArBlendedSurfacesTestingContext   * testingContext
        )
{
    // Settings
    testingContext->totalIterations         = totalIterations;
    testingContext->currentBatchIterations  = currentBatchIterations;
    testingContext->cornerCaseProbability   = cornerCaseProbability;

    // Intersection data

    testingContext->intersection = [ ALLOC_INIT_OBJECT(ArcIntersection) ];
    assert((testingContext->intersection) != nil);

    ArTraversalState traversalState;
    memset((void*)&traversalState, 0, sizeof(traversalState));
    ArNodeRef trafoRef = HARD_NODE_REFERENCE( TRANSLATION(0.0, 0.0, 0.0) );
    ARTS_TRAFO_REF(traversalState) = trafoRef;

    ARCINTERSECTION_T(testingContext->intersection)              = 0;
    ARCINTERSECTION_SHAPE(testingContext->intersection)          = NULL;
    ARCINTERSECTION_FACE_ID(testingContext->intersection)        = 0;
    ARCINTERSECTION_FACE_TYPE(testingContext->intersection)      = arface_on_shape_is_planar;
    ARCINTERSECTION_TRAVERSALSTATE(testingContext->intersection) = traversalState; // Shallow copy

    // BSDF sample generation context

    ARSGC_RANDOM_GENERATOR(testingContext->sampleGenerationContext) =
        randomGenerator;
    ARSGC_PHASE_INTERFACE_CACHE(testingContext->sampleGenerationContext) =
        [ ALLOC_INIT_OBJECT(ArcPhaseInterfaceCache) ];
    
    arbsdfsamplefreelists_alloc(
          art_gv,
        & ARSGC_BSDFSAMPLE_FREELISTS(testingContext->sampleGenerationContext),
          256
        );

    // Counters

    testingContext->inDirNormalCount                = 0;
    testingContext->inDirRightAngleCount            = 0;
    testingContext->inDirRandomCount                = 0;
    testingContext->successfulTestsCount            = 0;
    testingContext->unsuccessfulTestsCount          = 0;
    testingContext->lowerMatDielectricsCount        = 0;
    testingContext->lowerMatConductorCount          = 0;
    testingContext->outSamplingByFirstSurfaceCount  = 0;
    testingContext->outSamplingBySecondSurfaceCount = 0;
}

void testing_context_prepare_for_use(
        double                              thetaIn,
        double                              sigmaIn,
        ArNode <ArpSurfaceMaterial>               * surface1,
        ArNode <ArpSurfaceMaterial>               * surface2,
        ArNode <ArpVolumeMaterial>              * upperMaterial,
        ArNode <ArpVolumeMaterial>              * lowerMaterial,
        ArBlendedSurfacesTestingContext   * testingContext
        )
{
    (void) surface2;
    testingContext->currentThetaIn = thetaIn;
    testingContext->currentSigmaIn = sigmaIn;

    Ray3D worldspaceIncomingRay;
    Ray3D objectspaceIncomingRay;
    Vec3D objectspaceNormal;

    RAY3D_POINT(objectspaceIncomingRay) =
        PNT3D( 0.0, 0.0, 0.0 );
    RAY3D_VECTOR(objectspaceIncomingRay) =
        VEC3D(
            sin(thetaIn) * cos(sigmaIn),
            sin(thetaIn) * sin(sigmaIn),
            cos(thetaIn)
            );

    // We make things easier by unifying the world space with the object space
    worldspaceIncomingRay = objectspaceIncomingRay;

    objectspaceNormal = VEC3D( 0.0, 0.0, 1.0 );

    ARCINTERSECTION_OBJECTSPACE_INCOMING_RAY(testingContext->intersection) =
        objectspaceIncomingRay;
    ARCINTERSECTION_WORLDSPACE_INCOMING_RAY(testingContext->intersection) =
        worldspaceIncomingRay;
    ARCINTERSECTION_OBJECTSPACE_NORMAL(testingContext->intersection) =
        objectspaceNormal;
    ARCINTERSECTION_FLAG_OBJECTSPACE_NORMAL_AS_VALID(testingContext->intersection);

    ARCPOINTCONTEXT_STATUS_FLAGS(testingContext->intersection) &= ~arpcflag_worldspace_point_valid;
    ARCSURFACEPOINT_STATUS_FLAGS(testingContext->intersection) &= ~arspflag_worldspace_normal_valid;
    ARCPOINTCONTEXT_STATUS_FLAGS(testingContext->intersection) &= ~arpcflag_objectspace_point_valid;

    // Surface
    ARCPOINTCONTEXT_SURFACE_MATERIAL_REF(testingContext->intersection) =
        WEAK_NODE_REFERENCE( surface1 );

    // Set the materials in the correct orded
    ArNode <ArpVolumeMaterial> * fromMaterial;
    ArNode <ArpVolumeMaterial> * intoMaterial;
    // This is not needed since we only sample the surfaces from "above", but
    // in case we revert back to sampling the whole hemisphere, it will be
    // needed again
    if (RAY3D_VI(worldspaceIncomingRay, 2) >= 0.0)
    {
        // Incoming ray points upwards (from lower to upper material)
        fromMaterial = lowerMaterial;
        intoMaterial = upperMaterial;

    }
    else
    {
        // Incoming ray points downwards (from upper to lower material)
        fromMaterial = upperMaterial;
        intoMaterial = lowerMaterial;

    }
    ARCINTERSECTION_VOLUME_MATERIAL_FROM_REF(testingContext->intersection) =
        WEAK_NODE_REFERENCE( fromMaterial );
    ARCINTERSECTION_VOLUME_MATERIAL_INTO_REF(testingContext->intersection) =
        WEAK_NODE_REFERENCE( intoMaterial );

    [ testingContext->intersection prepareForUse
        : ARSGC_PHASE_INTERFACE_CACHE(testingContext->sampleGenerationContext)
        ];
}

void testing_context_release(
        ART_GV                            * art_gv,
        ArBlendedSurfacesTestingContext   * testingContext
        )
{
    (void) art_gv;
    
    RELEASE_OBJECT(testingContext->intersection);

    arbsdfsamplefreelists_free_contents(
          & ARSGC_BSDFSAMPLE_FREELISTS(testingContext->sampleGenerationContext)
        );
}

void test_blended_MMs(
        ART_GV                            * art_gv,
        ArBlendedSurfacesTestingContext   * testingContext,
        ArMuellerMatrix                   * firstMM,
        ArMuellerMatrix                   * secondMM,
        unsigned int                        debug_print_only
        )
{
    ArMuellerMatrix * mm1     = armuellermatrix_alloc(art_gv);
    ArMuellerMatrix * mm2     = armuellermatrix_alloc(art_gv);
    ArMuellerMatrix * mmBlend = armuellermatrix_alloc(art_gv);

    armuellermatrix_md_mul_m(art_gv, firstMM, (1.0 - testingContext->blendCoeff), mm1);
    armuellermatrix_md_mul_m(art_gv, secondMM, testingContext->blendCoeff, mm2);
    armuellermatrix_mm_add_m(art_gv, mm1, mm2, mmBlend);

    // debug
    //armuellermatrix_d_randomly_damage_m(art_gv, 0.000005, mmBlend);

    // The actual MM test
    if (armuellermatrix_m_valid(art_gv, mmBlend, NO, debug_print_only))
    {
        testingContext->currentTestSuccess = YES;
        testingContext->successfulTestsCount++;
    }
    else
    {
        testingContext->currentTestSuccess = NO;
        testingContext->unsuccessfulTestsCount++;
    }

    // debug
    //printf("\nFirst MM\n");
    //armuellermatrix_mi_debugprintf(
    //    art_gv,
    //    firstMM
    //    );
    //printf("Second MM\n");
    //armuellermatrix_mi_debugprintf(
    //    art_gv,
    //    secondMM
    //    );
    //printf("Blended MM\n");
    //armuellermatrix_mi_debugprintf(
    //    art_gv,
    //    mmBlend
    //    );

    armuellermatrix_free(art_gv, mm1);
    armuellermatrix_free(art_gv, mm2);
    armuellermatrix_free(art_gv, mmBlend);
}

void tests_for_incoming_direction(
        ART_GV                            * art_gv,
        ArBlendedSurfacesTestingContext   * testingContext,
        ArNode <ArpSurfaceMaterial>       * firstSurface,
        ArNode <ArpSurfaceMaterial>       * secondSurface,
        const char *                        sampleSourceStr
        )
{
#ifndef NEVERMORE
    (void) art_gv;
    (void) testingContext;
    (void) firstSurface;
    (void) secondSurface;
    (void) sampleSourceStr;
#endif
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
#ifdef NEVERMORE
    ArBSDFSampleList bsdfsampleList = ARFULLIGHTBSDFSAMPLELIST_EMPTY;

    // Let the first model generate the direction(s)
    [ firstSurface calculateFullLightBSDFSampleList
            :   testingContext->intersection
            :   arpathdirection_from_eye
            : & testingContext->sampleGenerationContext
            : & bsdfsampleList
            :   arbsdfsamplingstrategy_average_refraction
            ];

    arfulllightbsdfsamplelist_compute_percentiles( art_gv, & bsdfsampleList );

    double randomVal = RANDOM_VAL_FROM_RANGE(0.0, 1.0);
    ArFullLightBSDFSample * bsdfsample = ARBSDFSAMPLELIST_HEAD(bsdfsampleList);
    while ( bsdfsample )
    {
        if (  ARFULLIGHTBSDFSAMPLE_PERCENTILE(*bsdfsample)
            > randomVal  )
        {
            ArFullLightSurfacePointDirection * outgoingDirection =
                & ARFULLIGHTBSDFSAMPLE_SPDIR(*bsdfsample);

            testingContext->currentDirectionOut =
                ARFULLIGHTSURFACEPOINTDIRECTION_VECTOR(*outgoingDirection);
            testingContext->currentCosineOut =
                ARFULLIGHTSURFACEPOINTDIRECTION_COSINE(*outgoingDirection);

            ArMMDirectAttenuation * firstMMDirectAttenuation =
                ((ArMMDirectAttenuation*)
                    (ARATTENUATION_MAIN_DIAGONAL(
                        ARFULLIGHTBSDFSAMPLE_ATTENUATION(*bsdfsample)
                        )
                     ->value));

            // debug
            //printf("First surface attenuation:\n");
            //armmdirectattenuation_a_debugprintf(
            //    art_gv,
            //    firstMMDirectAttenuation
            //    );
            //armuellermatrix_mi_debugprintf(art_gv, firstMMDirectAttenuation->muellerMatrix);

            // Sample the second one in this direction(s) explicitly
            ArPDFValue      secondSampleProbability;
            ArAttenuation * secondAttenuation = arattenuation_alloc(art_gv);

            [ secondSurface
                calculateBidirectionalFullLightAttenuation
                :       testingContext->intersection
                :       outgoingDirection
                :       arpathdirection_from_eye
                :   &   testingContext->sampleGenerationContext
                :   &   secondSampleProbability
                :       secondAttenuation
                ];

            ArMMDirectAttenuation * secondMMDirectAttenuation =
                ((ArMMDirectAttenuation*)
                    (ARATTENUATION_MAIN_DIAGONAL(secondAttenuation)->value));

            // debug
            //printf("Second surface attenuation:\n");
            //armmdirectattenuation_a_debugprintf(
            //    art_gv,
            //    secondMMDirectAttenuation
            //    );
            //armuellermatrix_mi_debugprintf(art_gv, secondMMDirectAttenuation->muellerMatrix);

            // Blend and test the MMs
            test_blended_MMs(
                art_gv,
                testingContext,
                ARMMDIRECTATTENUATION_MM(*firstMMDirectAttenuation),
                ARMMDIRECTATTENUATION_MM(*secondMMDirectAttenuation),
                NO //YES //
                );

            bsdfsample =
                arfulllightbsdfsample_pop(
                      bsdfsample,
                    & ARSGC_BSDFSAMPLE_FREELISTS(testingContext->sampleGenerationContext)
                    );

            arattenuation_free(art_gv, secondAttenuation);

            // Throw away the rest of the samples
            while ( bsdfsample )
                bsdfsample =
                    arfulllightbsdfsample_pop(
                          bsdfsample,
                        & ARSGC_BSDFSAMPLE_FREELISTS(testingContext->sampleGenerationContext)
                        );
        }
        else
            // Throw away samples with percentile below the random value
            bsdfsample =
                arfulllightbsdfsample_pop(
                      bsdfsample,
                    & ARSGC_BSDFSAMPLE_FREELISTS(testingContext->sampleGenerationContext)
                    );
    }
#endif
}

void arnsurface_test_blended_BSDF_MMs(
        ART_GV                            * art_gv,
        ArBlendedSurfacesTestingContext   * testingContext,
        ArcObject <ArpRandomGenerator>    * randomGenerator,
        ArNode <ArpSurfaceMaterial>               * firstSurface,
        ArNode <ArpSurfaceMaterial>               * secondSurface,
        ArNode <ArpVolumeMaterial>              * upperMaterial,
        ArNode <ArpVolumeMaterial>              * lowerMaterial
        )
{
    (void) randomGenerator;
    
    // Generate eye direction from the "upper" hemi-sphere only. We don't sample
    // the whole sphere because in the current implementation the BSDFs act
    // identically from both sides of the surface (due to inversion of normal
    // in the intersection setup code) and therefore we would get exactly the
    // same testing scenario when using material combination A:B as for
    // combination B:A. If sampled deterministicaly, we may get redundant
    // tests. If sampled randomly, the dielectric-dielectric case
    // (can be at both sides of the surface) would be of double probability
    // of the dielectric-conductor case (conductor can be "inside" the object
    // only).

    double thetaIn;
    double sigmaIn;

    // Choose between cornercase values and regular random sampling
    double randomVal = RANDOM_VAL_FROM_RANGE(0.0, 1.0);
    if (randomVal < (0.5 * testingContext->cornerCaseProbability))
    {
        // 90 degrees
        thetaIn = M_PI_2;
        sigmaIn = 0.0;

        testingContext->inDirRightAngleCount++;
    }
    else
    if (randomVal < testingContext->cornerCaseProbability)
    {
        // 0 degrees - normal incidence
        thetaIn = M_PI;
        sigmaIn = RANDOM_VAL_FROM_RANGE(0.0, 2.0 * M_PI);

        testingContext->inDirNormalCount++;
    }
    else
    {
        // Random direction sampling
        // TODO: Cosine-weighted sampling
        thetaIn = RANDOM_VAL_FROM_RANGE(M_PI_2, M_PI);
        sigmaIn = RANDOM_VAL_FROM_RANGE(0.0, 2.0 * M_PI);

        testingContext->inDirRandomCount++;
    }

    testing_context_prepare_for_use(
            thetaIn,
            sigmaIn,
            firstSurface,
            secondSurface,
            upperMaterial,
            lowerMaterial,
            testingContext
        );

    // Generate blending coefficient
    double blendCoeff = RANDOM_VAL_FROM_RANGE(0.0, 1.0);
    testingContext->blendCoeff = blendCoeff;

    // We now choose one of the surfaces randomly. The choosen surface model
    // is then responsible for the output direction sampling. The catch is that
    // different models can have different sampling dimensionality: specular
    // surfaces have no degree of freedom - reflection/refraction is
    // completely determined by the input direction; diffuse and glossy
    // surfaces can sample the whole (hemi)sphere (2D). ...
    //
    // TODO: This should be cleared up.

    //const int iteration = testingContext->currentIteration;
    //double firstSurfProb;
    //if (isFirstSurfacePerfectlySpecular && !isSecondSurfacePerfectlySpecular)
    //    // First 0D, second 2D
    //    firstSurfProb = 1.0 / (iteration * iteration + 1);
    //else if (!isFirstSurfacePerfectlySpecular && isSecondSurfacePerfectlySpecular)
    //    // First 2D, second 0D
    //    firstSurfProb = 1.0 - 1.0 / (iteration * iteration + 1);
    //else
    //    // Both are of the same dimensionality
    //    firstSurfProb = 0.5;
    //
    //if (RANDOM_VAL_FROM_RANGE(0.0, 1.0) < firstSurfProb)

    if (RANDOM_VAL_FROM_RANGE(0.0, 1.0) < 0.5)
    {
        // Test in output direction(s) sampled by the first surface
        NSString * firstSurfaceClassName = [ firstSurface className ];
        tests_for_incoming_direction(
                art_gv,
                testingContext,
                firstSurface,
                secondSurface,
                [ firstSurfaceClassName UTF8String ]
                //" first surface"
            );

        testingContext->outSamplingByFirstSurfaceCount++;
    }
    else
    {
        // Test in output direction(s) sampled by the second surface
        NSString * secondSurfaceClassName = [ secondSurface className ];
        tests_for_incoming_direction(
                art_gv,
                testingContext,
                secondSurface,
                firstSurface,
                [ secondSurfaceClassName UTF8String ]
                //"second surface"
            );

        testingContext->outSamplingBySecondSurfaceCount++;
    }
}

id generate_random_albedo(
        ART_GV                          * art_gv
        )
{
    id randomColour = nil;

    unsigned int channels_count = spc_channels( art_gv );

    switch (channels_count)
    {
        case 8:
        {
            double randomVal0 = RANDOM_ATTENUATION;
            double randomVal1 = RANDOM_ATTENUATION;
            double randomVal2 = RANDOM_ATTENUATION;
            double randomVal3 = RANDOM_ATTENUATION;
            double randomVal4 = RANDOM_ATTENUATION;
            double randomVal5 = RANDOM_ATTENUATION;
            double randomVal6 = RANDOM_ATTENUATION;
            double randomVal7 = RANDOM_ATTENUATION;

            randomColour =
                CONST_COLOUR_PSSPECTRUM( 1.0,
                    SET_PSSPECTRUM_COLOUR_CHANNEL(0),
                    SET_PSSPECTRUM_COLOUR_CHANNEL(1),
                    SET_PSSPECTRUM_COLOUR_CHANNEL(2),
                    SET_PSSPECTRUM_COLOUR_CHANNEL(3),
                    SET_PSSPECTRUM_COLOUR_CHANNEL(4),
                    SET_PSSPECTRUM_COLOUR_CHANNEL(5),
                    SET_PSSPECTRUM_COLOUR_CHANNEL(6),
                    SET_PSSPECTRUM_COLOUR_CHANNEL(7),
                    PSS_END
                    );
            break;
        }

        default:
            ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR;
    }

    // Debug
    //ArSpectrum * dbgCol = spc_alloc(art_gv);
    //[ randomColour getSpectrum : 0 : dbgCol ];
    //spc_s_debugprintf(art_gv, dbgCol);
    //spc_free(art_gv, dbgCol);

    return randomColour;
}

void arnsurface_test_blended_BSDF_MMs_lambert_fresnel(
        ART_GV                          * art_gv,
        ArcObject <ArpRandomGenerator>  * randomGenerator
        )
{
    ArBlendedSurfacesTestingContext testingContext;

    // Randomize seed (for different multiple runs)
    srand(time(NULL));

    // Prepare the context
    testing_context_alloc_init(
            art_gv,
            randomGenerator,
            100 * 1000 * 1000,
              4 * 1000 * 1000,
            0.1,
          & testingContext
        );

    id fresnelSurface =
        SMOOTH_FRESNEL_SURFACE;

    for (int iteration = 1; iteration <= testingContext.currentBatchIterations; iteration++)
    {
        // debug
        //printf("\n============================================================================\n");

        testingContext.currentIteration   = iteration;
        testingContext.currentTestSuccess = YES;

        // Generate random albedo Lambert surface
        id randomAlbedo = generate_random_albedo(art_gv);
        id lambertSurface =
            LAMBERT_REFLECTOR(
                randomAlbedo
                );
        RELEASE_OBJECT(randomAlbedo);

        // Materials

        // Upper material IoR
        double       upperIoR        = RANDOM_VAL_FROM_RANGE(0.9, 3.0);
        const double upperExtinction = 0.0;

        // For the lower material, we first choose randomly whether
        // dielectric (1D) or conductive (2D) material will be used.
        // The probabilities are set so that the both scenarios will be sampled
        // with similar number of samples per dimension
        double lowerIoR;
        double lowerExtinction;
        double samplesPerDimension =
            0.5 * (sqrt(4.0 * testingContext.totalIterations + 1.0) - 1.0);
        double dielectricsProb = 1.0 / (samplesPerDimension + 1);
        double randomVal = RANDOM_VAL_FROM_RANGE(0.0, 1.0);
        if (randomVal < dielectricsProb)
        {
            // Dielectrics
            lowerIoR        = RANDOM_VAL_FROM_RANGE(0.9, 3.0);
            lowerExtinction = 0.0;

            testingContext.lowerMatDielectricsCount++;
        }
        else
        {
            // Conductor
            lowerIoR        = RANDOM_VAL_FROM_RANGE(0.9, 3.0);
            lowerExtinction = RANDOM_VAL_FROM_RANGE(0.0, 3.0);

            testingContext.lowerMatConductorCount++;
        }

        //id dbgUpperIoR = CONST_COLOUR_GRAY(upperIoR);
        //id dbgUpperExt = CONST_COLOUR_GRAY(upperExtinction);

        id upperMaterial =
            STANDARD_MATERIAL(
                CONST_COLOUR_GRAY(upperIoR),
                CONST_COLOUR_GRAY(upperExtinction)
                //dbgUpperIoR,
                //dbgUpperExt
                );

        id lowerMaterial =
            STANDARD_MATERIAL(
                CONST_COLOUR_GRAY(lowerIoR),
                CONST_COLOUR_GRAY(lowerExtinction)
                );

        // Run the tests for both material combinations

        arnsurface_test_blended_BSDF_MMs(
                art_gv,
              & testingContext,
                randomGenerator,
                lambertSurface,
                fresnelSurface,
                upperMaterial,
                lowerMaterial
            );

        if (testingContext.currentTestSuccess == NO)
        {
            printf("\n");
            printf("Test failed for:\n");
            printf("\tupper material: (%.2f,%.2f)\n", upperIoR, upperExtinction);
            printf("\tlower material: (%.2f,%.2f)\n", lowerIoR, lowerExtinction);
            printf("\tincoming ray: (%.1f,%.1f)->(%+.1f,%+.1f,%+.1f), cos: %.2f"
                   ", normal: (%.1f,%.1f,%.1f)"
                   "\n",
                   testingContext.currentThetaIn, testingContext.currentSigmaIn,
                   VEC3D_I(RAY3D_V(ARCINTERSECTION_OBJECTSPACE_INCOMING_RAY(testingContext.intersection)), 0),
                   VEC3D_I(RAY3D_V(ARCINTERSECTION_OBJECTSPACE_INCOMING_RAY(testingContext.intersection)), 1),
                   VEC3D_I(RAY3D_V(ARCINTERSECTION_OBJECTSPACE_INCOMING_RAY(testingContext.intersection)), 2),
                   ARCINTERSECTION_WORLDSPACE_COSINE(testingContext.intersection),
                   VEC3D_I(ARCINTERSECTION_OBJECTSPACE_NORMAL(testingContext.intersection), 0),
                   VEC3D_I(ARCINTERSECTION_OBJECTSPACE_NORMAL(testingContext.intersection), 1),
                   VEC3D_I(ARCINTERSECTION_OBJECTSPACE_NORMAL(testingContext.intersection), 2)
                   );
            printf("\toutgoing sample direction"
                   //" (from %s)"
                   ": (%+.1f,%+.1f,%+.1f), cos: %+.2f, blend: %.2f\n",
                   //sampleSourceStr,
                   VEC3D_I(testingContext.currentDirectionOut, 0),
                   VEC3D_I(testingContext.currentDirectionOut, 1),
                   VEC3D_I(testingContext.currentDirectionOut, 2),
                   testingContext.currentCosineOut,
                   testingContext.blendCoeff
                   );
        }

        if ((iteration % (100 * 1000)) == 0)
            printf("Finished %d tests\n", iteration);

        RELEASE_OBJECT(upperMaterial);
        RELEASE_OBJECT(lowerMaterial);
        RELEASE_OBJECT(lambertSurface);
    }

    RELEASE_OBJECT(fresnelSurface);

    printf(
        "\nFinished.\n"
        "\tlower material: %d dielectrics + %d conductors\n"
        "\tincoming directions count: %d normal + %d right angle + %d random sampling\n"
        "\toutgoing samples count: %d from first material + %d from second material\n"
        "\tnumber of performed test: %d (%d passed, %d failed)\n",

        testingContext.lowerMatDielectricsCount,
        testingContext.lowerMatConductorCount,

        testingContext.inDirNormalCount,
        testingContext.inDirRightAngleCount,
        testingContext.inDirRandomCount,

        testingContext.outSamplingByFirstSurfaceCount,
        testingContext.outSamplingBySecondSurfaceCount,

        testingContext.successfulTestsCount + testingContext.unsuccessfulTestsCount,
        testingContext.successfulTestsCount,
        testingContext.unsuccessfulTestsCount
        );

    // Release the context
    testing_context_release(
          art_gv,
        & testingContext
        );
}

void arnsurface_test_blended_BSDFs_MMs(
        ART_GV                          * art_gv,
        ArcObject <ArpRandomGenerator>  * randomGenerator
        )
{
    if ( ! LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE )
        ART_ERRORHANDLING_FATAL_ERROR(
            "Evaluation of blended BSDF models correctness "
            "requires running ART in polarisation mode"
            );

    arnsurface_test_blended_BSDF_MMs_lambert_fresnel(art_gv, randomGenerator);
}

// ===========================================================================
