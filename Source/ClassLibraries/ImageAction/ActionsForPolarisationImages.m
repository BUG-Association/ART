/* ===========================================================================

    Copyright (c) 1996-2018 The ART Development Team
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

#define ART_MODULE_NAME     ActionsForPolarisationImages

#import "ActionsForPolarisationImages.h"
#import "ART_ColourAndSpectra.h"
#import "ART_ImageData.h"
#import "ART_ImageFileFormat.h"
#import "ArnImageManipulationMacros.h"

#import "ColourAndLightSubsystem.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    [ ArnARTRAWLinearPolarisingFilter    registerWithRuntime ];
    [ ArnARTRAWPolarisationVisualisation registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#define REPORTER    ART_GLOBAL_REPORTER

/* ===========================================================================
    'ArnARTRAWLinearPolarisingFilter'
=========================================================================== */

@implementation ArnARTRAWLinearPolarisingFilter

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnARTRAWLinearPolarisingFilter)
ARPACTION_DEFAULT_SINGLE_IMAGE_ACTION_IMPLEMENTATION(ArnARTRAWLinearPolarisingFilter)

- (void) _setupAttenuation
{
}

- _init
        : (double) newAngle
        : (double) newStrength
        : (BOOL) newCloneOption
{
    self =
        [ super init
            : NO
            ];
    
    if ( self )
    {
        filterAngle     = newAngle;
        filterStrength  = newStrength;
        cloneOption     = newCloneOption;

        [ self _setupAttenuation ];
    }
    
    return self;
}

- _init
        : (double) newAngle
        : (double) newStrength
{
    self =
        [ self _init
            :   newAngle
            :   newStrength
            :   YES
            ];

    return self;
}

- angle
                   : (double) newAngle
        strength   : (double) newStrength
        cloneSource: (BOOL) newCloneOption
{
    return
        [ self _init
            :   newAngle
            :   newStrength
            :   newCloneOption
            ];
}

- angle
                : (double) newAngle
        strength: (double) newStrength
{
    return
        [ self _init
            :   newAngle
            :   newStrength
            ];
}

- (void) performOn
        : (ArNode <ArpNodeStack> *) nodeStack
{

    if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE )
    {
        [ REPORTER beginTimedAction
            :   "applying linear polarisation filter"
            ];

        ArAttenuation  * linearPolarisationFilter =
            arattenuation_alloc( art_gv );

        ArReferenceFrame  r;

        ARREFFRAME_RF_I(r,0) = VEC3D_X_UNIT;
        ARREFFRAME_RF_I(r,1) = VEC3D_Y_UNIT;

        arattenuation_ddrr_init_linear_polariser_a(
              art_gv,
              filterAngle,
              filterStrength,
            & r,
            & r,
              linearPolarisationFilter
            );

        /* ------------------------------------------------------------------
             Before calling the function that sets up the framework for
             image manipulation we have to specify what colour type the
             result image will have.

             imageColourType = what we are going to feed it
             fileColourType  = what we want it to write to disk for us
        ---------------------------------------------------------------aw- */

        //   The image colour type is always "native" (i.e. whatever model we
        //   are using), except for RGB images: to avoid issues with different
        //   RGB colour spaces, any RGB results get written to disk as CIE XYZ

        if (   art_isr( art_gv ) == arspectrum_ut_rgb
            || art_isr( art_gv ) == arspectrum_ut_rgb_polarisable )
            destinationImageColourType = arspectrum_ciexyz;
        else
            destinationImageColourType = art_isr( art_gv );

        //   In the case of a renderer directly writing its output to file, the
        //   image and file colour data types are the same.

        destinationFileColourType = destinationImageColourType;


        /* ------------------------------------------------------------------
             Activation of the framework common to all image manipulation
             actions. This takes the source image from the stack, and creates
             the destination image along with all needed scanline buffers.

             In order to do this properly it has to be informed of what
             kind of source image to expect, and what kind of result image
             we wish to create (in our case, ArfARTRAW and ArfARTCSP).
        ---------------------------------------------------------------aw- */

        [ self prepareForImageManipulation
            :   nodeStack
            :   [ ArfARTRAW class ]
            :   [ ArfARTRAW class ]
            ];

        /* ------------------------------------------------------------------
             Process all pixels in the image.
        ---------------------------------------------------------------aw- */

        for ( int i = 0; i < numberOfSourceImages; i++ )
        {
            for ( long y = 0; y < YC(destinationImageSize); y++ )
            {
                [ self loadSourceScanlineBuffer: i : y ];

                for ( long x = 0; x < XC(destinationImageSize); x++ )
                {
                    arlight_al_mul_l(
                          art_gv,
                          linearPolarisationFilter,
                          LIGHTALPHA_SOURCE_BUFFER_LIGHT(x),
                          LIGHTALPHA_DESTINATION_BUFFER_LIGHT(x)
                        );

                    LIGHTALPHA_DESTINATION_BUFFER_ALPHA(x) = LIGHTALPHA_SOURCE_BUFFER_ALPHA(x);
                }

                [ self writeDestinationScanlineBuffer: i : y ];
            }
        }


        /* ------------------------------------------------------------------
             Free the image manipulation infrastructure and end the action;
             this also places the destination image on the stack.
        ---------------------------------------------------------------aw- */

        [ self finishImageManipulation
            :   nodeStack ];

        arattenuation_free(
            art_gv,
            linearPolarisationFilter
            );

        [ REPORTER endAction ];
    }
    else
    {
        [ REPORTER beginAction
            :   "(polarising filter: image w/o polarisation information, skipped)"
            ];

        [ REPORTER endAction ];
    }
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];

    [ coder codeDouble: & filterAngle ];
    [ coder codeDouble: & filterStrength ];
    [ coder codeBOOL  : & cloneOption ];

    if ( [ coder isReading ] )
        [ self _setupAttenuation ];
}

@end

void arlight_l_avg_polarisation_stats(
        const ART_GV   * art_gv,
        const ArLight  * l0,
              double   * dop,
              double   * abs_dop_lin,
              double   * abs_dop_cir,
              double   * rel_dop_lin,
              double   * rel_dop_cir,
              double   * avg_sc
        )
{
    *dop = 0.0;
    *abs_dop_lin = 0.0;
    *abs_dop_cir = 0.0;
    *rel_dop_lin = 0.0;
    *rel_dop_cir = 0.0;

    if ( arlight_l_polarised( art_gv, l0 ) )
    {
        ArStokesVector  * sv0 = arstokesvector_alloc(art_gv);

        arlight_l_to_sv(
            art_gv,
            l0,
            sv0
            );

        for ( unsigned int i = 0; i < 4; i++ )
            avg_sc[i] =
                arlightintensity_i_avg(
                    art_gv,
                    ARSV_I( *sv0, i )
                    );

        double  lP =
            sqrt( M_SQR(avg_sc[1]) + M_SQR(avg_sc[2]) + M_SQR(avg_sc[3]) );

        double  lP_l =
            sqrt( M_SQR(avg_sc[1]) + M_SQR(avg_sc[2]) );

        arstokesvector_free( art_gv, sv0 );

        if ( lP > 0.0 && avg_sc[0] > 0.0 )
        {
            *dop = lP / avg_sc[0];
            *abs_dop_lin = M_ABS( lP_l / avg_sc[0] );
            *abs_dop_cir = M_ABS( avg_sc[3] / avg_sc[0] );
            *rel_dop_lin = lP_l / lP;
            *rel_dop_cir = avg_sc[3] / lP;
        }
    }
    else
    {
        avg_sc[0] =
            arlight_l_norm(
                art_gv,
                l0
                );
    }
}

void arlight_l_polarisation_stats(
        const ART_GV   * art_gv,
        const ArLight  * l0,
        const double     wavelength,
              double   * dop,
              double   * abs_dop_lin,
              double   * abs_dop_cir,
              double   * rel_dop_lin,
              double   * rel_dop_cir,
              double   * sc
        )
{
    *dop = 0.0;
    *abs_dop_lin = 0.0;
    *abs_dop_cir = 0.0;
    *rel_dop_lin = 0.0;
    *rel_dop_cir = 0.0;

    ArStokesVector  * sv0 = arstokesvector_alloc(art_gv);

    arlight_l_to_sv(
        art_gv,
        l0,
        sv0
        );

    for ( unsigned int i = 0; i < 4; i++ )
        sc[i] =
            arlightintensity_id_value_at_wavelength(
                  art_gv,
                  ARSV_I( *sv0, i ),
                  wavelength
                );

    if ( arlight_l_polarised( art_gv, l0 ) )
    {

        double  lP =
            sqrt( M_SQR(sc[1]) + M_SQR(sc[2]) + M_SQR(sc[3]) );

        double  lP_l =
            sqrt( M_SQR(sc[1]) + M_SQR(sc[2]) );

        if ( lP > 0.0 && sc[0] > 0.0 )
        {
            *dop = lP / sc[0];
            *abs_dop_lin = M_ABS( lP_l / sc[0] );
            *abs_dop_cir = M_ABS( sc[3] / sc[0] );
            *rel_dop_lin = lP_l / lP;
            *rel_dop_cir = sc[3] / lP;
        }
    }
    else
    {
        sc[0] =
            arlightintensity_id_value_at_wavelength(
                  art_gv,
                  ARSV_I( *sv0, 0 ),
                  wavelength
                );
        sc[1] = 0.0;
        sc[2] = 0.0;
        sc[3] = 0.0;
    }

    arstokesvector_free( art_gv, sv0 );
}

void binary_polvis_colour_to_ut_rgb(
        ART_GV          * art_gv,
        ArPolVisColour  * pvc,
        double            value,
        ArUT_RGB        * ut_rgb
        )
{
    switch( *pvc )
    {
        case arpolviscolour_RG:
            if ( value > 0.0 )
                *ut_rgb = ARUT_RGB(0,1,0);
            else
                *ut_rgb = ARUT_RGB(1,0,0);
            break;
        case arpolviscolour_BY:
            if ( value < 0.0 )
                *ut_rgb = ARUT_RGB(0,0,1);
            else
                *ut_rgb = ARUT_RGB(1,1,0);
            break;
        default:
            ART_ERRORHANDLING_FATAL_ERROR(
                "invalid colour specification"
                );
    }
}

void polvis_colour_to_ut_rgb(
        ART_GV          * art_gv,
        ArPolVisColour  * pvc,
        ArUT_RGB        * ut_rgb
        )
{
    switch( *pvc )
    {
        case arpolviscolour_R:
            *ut_rgb = ARUT_RGB(1,0,0);
            break;
        case arpolviscolour_G:
            *ut_rgb = ARUT_RGB(0,1,0);
            break;
        case arpolviscolour_B:
            *ut_rgb = ARUT_RGB(0,0,1);
            break;
        case arpolviscolour_C:
            *ut_rgb = ARUT_RGB(0,1,1);
            break;
        case arpolviscolour_Y:
            *ut_rgb = ARUT_RGB(1,1,0);
            break;
        case arpolviscolour_M:
            *ut_rgb = ARUT_RGB(1,0,1);
            break;
        case arpolviscolour_K:
            *ut_rgb = ARUT_RGB(0,0,0);
            break;
        case arpolviscolour_W:
            *ut_rgb = ARUT_RGB(1,1,1);
            break;
        default:
            ART_ERRORHANDLING_FATAL_ERROR(
                "invalid colour specification"
                );
    }
}

@implementation ArnARTRAWPolarisationVisualisation

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnARTRAWPolarisationVisualisation)
ARPACTION_DEFAULT_SINGLE_IMAGE_ACTION_IMPLEMENTATION(ArnARTRAWPolarisationVisualisation)

- sv1
           : (ArPolVisColour) newSC1Colour
        sv2: (ArPolVisColour) newSC2Colour
        sv3: (ArPolVisColour) newSC3Colour
        scs: (BOOL) newSCS
        dop: (ArPolVisColour) newDOPColour
        lcA: (ArPolVisColour) newFPPColour
        lcB: (ArPolVisColour) newFIPColour
        upc: (ArPolVisColour) newUPCColour
        nml: (BOOL) newNormalise
        wvl: (double) newWavelength
        rms: (BOOL) newRemoveSource
{
    return
        [ self init
            :   newSC1Colour
            :   newSC2Colour
            :   newSC3Colour
            :   newSCS
            :   newDOPColour
            :   newFPPColour
            :   newFIPColour
            :   newUPCColour
            :   newNormalise
            :   newWavelength
            :   newRemoveSource
            ];
}

- init
        : (ArPolVisColour) newSC1Colour
        : (ArPolVisColour) newSC2Colour
        : (ArPolVisColour) newSC3Colour
        : (BOOL) newSCS
        : (ArPolVisColour) newDOPColour
        : (ArPolVisColour) newFPPColour
        : (ArPolVisColour) newFIPColour
        : (ArPolVisColour) newUPCColour
        : (BOOL) newNormalise
        : (double) newWavelength
        : (BOOL) newRemoveSource
{
    self =
        [ super init
            :   newRemoveSource
            ];
    
    if ( self )
    {
        scColour[0]  = newSC1Colour;
        scColour[1]  = newSC2Colour;
        scColour[2]  = newSC3Colour;
        scs          = newSCS;
        dopColour    = newDOPColour;
        fppColour    = newFPPColour;
        fipColour    = newFIPColour;
        upcColour    = newUPCColour;
        normalise    = newNormalise;
        wavelength   = newWavelength;
    }
    
    return self;
}

#define  ArCIELCH       ArCIELab

#define  ARCIELCH       ARCIELab
#define  ARCIELCH_L     ARCIELab_L
#define  ARCIELCH_C     ARCIELab_a
#define  ARCIELCH_H     ARCIELab_b

void  lch_to_lab(
        ART_GV    * art_gv,
        ArCIELCH  * lch,
        ArCIELab  * lab
        )
{
    ARCIELab_L(*lab) = ARCIELCH_L(*lch);
    ARCIELab_a(*lab) = cos( ARCIELCH_H(*lch) ) * ARCIELCH_C(*lch);
    ARCIELab_b(*lab) = sin( ARCIELCH_H(*lch) ) * ARCIELCH_C(*lch);
}

- (void) performOn
        : (ArNode <ArpNodeStack> *) nodeStack
{
    [ REPORTER beginTimedAction
        :   "creating polarisation visualisation"
        ];

    ArnColourTransform  * colourTransform =
        [ ALLOC_INIT_OBJECT(ArnColourTransform)
            :   arintent_perceptual
            ];

    [ colourTransform setSourceAndDestinationColourspace
        :   ARCSR_CIELab
        :   [ DEFAULT_RGB_COLOURSPACE colourSpaceRef ]
        ];

    /* ------------------------------------------------------------------
         Before calling the function that sets up the framework for
         image manipulation we have to specify what colour type the
         result image will have.

         imageColourType = what we are going to feed it
         fileColourType  = what we want it to write to disk for us
    ---------------------------------------------------------------aw- */

    destinationImageColourType = arspectrum_ciexyz;
    destinationFileColourType  = arspectrum_ciexyz;

    numberOfDestinationsPerSource = 1;

    destinationFilenameTag = ALLOC_ARRAY(char *, numberOfDestinationsPerSource );
    
    destinationFilenameTag[0] = ALLOC_ARRAY( char, 20 );
    
    if ( dopColour != arpolviscolour_none )
    {
        sprintf(
            destinationFilenameTag[0],
            "dop.%3.0fnm",
            NANO_FROM_UNIT(wavelength)
            );
    }
    else
    {
        if ( fppColour != arpolviscolour_none )
        {
            sprintf(
                destinationFilenameTag[0],
                "lvc.%3.0fnm",
                NANO_FROM_UNIT(wavelength)
                );
        }
        else
        {
            if ( scColour[0] != arpolviscolour_none )
            {
                sprintf(
                    destinationFilenameTag[0],
                    "lin.%3.0fnm",
                    NANO_FROM_UNIT(wavelength)
                    );
            }
            else
            {
                sprintf(
                    destinationFilenameTag[0],
                    "cir.%3.0fnm",
                    NANO_FROM_UNIT(wavelength)
                    );
            }
        }
    }

    /* ------------------------------------------------------------------
         Activation of the framework common to all image manipulation
         actions. This takes the source image from the stack, and creates
         the destination image along with all needed scanline buffers.

         In order to do this properly it has to be informed of what
         kind of source image to expect, and what kind of result image
         we wish to create (in our case, ArfARTRAW and ArfARTCSP).
    ---------------------------------------------------------------aw- */

    [ self prepareForImageManipulation
        :   nodeStack
        :   [ ArfARTRAW class ]
        :   [ ArfARTCSP class ]
        ];

    /* ------------------------------------------------------------------
         Process all pixels in the image.
    ---------------------------------------------------------------aw- */

    for ( int i = 0; i < numberOfSourceImages; i++ )
    {
        for ( long y = 0; y < YC(destinationImageSize); y++ )
        {
            [ self loadSourceScanlineBuffer: i : y ];

            for ( long x = 0; x < XC(destinationImageSize); x++ )
            {
                ArUT_RGB  ut_rgb_fg = ARUT_RGB(0.0,0.0,0.0);
                ArUT_RGB  ut_rgb_bg = ARUT_RGB(0.0,0.0,0.0);

                double  dop, rel_dop_cir, rel_dop_lin, abs_dop_cir, abs_dop_lin;
                double  avg_sc[4];

                arlight_l_polarisation_stats(
                      art_gv,
                      LIGHTALPHA_SOURCE_BUFFER_LIGHT(x),
                      wavelength,
                    & dop,
                    & abs_dop_lin,
                    & abs_dop_cir,
                    & rel_dop_lin,
                    & rel_dop_cir,
                      avg_sc
                    );

                double  coverFactor = dop;

                if ( dop > 0.0 )
                {
                    if (    scColour[0] != arpolviscolour_none
                         || scColour[1] != arpolviscolour_none
                         || scColour[2] != arpolviscolour_none )
                    {
                        if (    scColour[0] != arpolviscolour_none
                             && scColour[1] != arpolviscolour_none )
                        {
                            coverFactor = abs_dop_lin;
                        }
                        else
                        {
                            if ( scColour[2] != arpolviscolour_none )
                            coverFactor = abs_dop_cir;
                        }


                        if ( ! scs )
                            dop = 1.0;

                        for ( int j = 0; j < 3; j++ )
                        {
                            ArUT_RGB  ut_rgb_sc = ARUT_RGB(0.0,0.0,0.0);

                            if ( scColour[j] != arpolviscolour_none )
                            {
                                binary_polvis_colour_to_ut_rgb(
                                      art_gv,
                                    & scColour[j],
                                      avg_sc[j+1],
                                    & ut_rgb_sc
                                    );

                                if (   avg_sc[0] > 0.0
                                    && normalise )
                                    ut_rgb_d_mul_c(
                                          art_gv,
                                          fabs( avg_sc[j+1] ) / avg_sc[0],
                                        & ut_rgb_sc
                                        );
                            }

                            ut_rgb_c_add_c(
                                  art_gv,
                                & ut_rgb_sc,
                                & ut_rgb_fg
                                );
                        }
                    }

                    if ( dopColour != arpolviscolour_none )
                    {
                        polvis_colour_to_ut_rgb(
                              art_gv,
                            & dopColour,
                            & ut_rgb_fg
                            );
                    }

                    if ( fppColour != arpolviscolour_none )
                    {
                        ArUT_RGB  ut_rgb_lin = ARUT_RGB(0.0,0.0,0.0);
                        ArUT_RGB  ut_rgb_cir = ARUT_RGB(0.0,0.0,0.0);

                        polvis_colour_to_ut_rgb(
                              art_gv,
                            & fppColour,
                            & ut_rgb_lin
                            );

                        ut_rgb_d_mul_c(
                              art_gv,
                              rel_dop_lin,
                            & ut_rgb_lin
                            );

                        polvis_colour_to_ut_rgb(
                              art_gv,
                            & fipColour,
                            & ut_rgb_cir
                            );

                        ut_rgb_d_mul_c(
                              art_gv,
                              M_ABS(rel_dop_cir),
                            & ut_rgb_cir
                            );

                        ut_rgb_cc_add_c(
                              art_gv,
                            & ut_rgb_lin,
                            & ut_rgb_cir,
                            & ut_rgb_fg
                            );
                    }

                    ut_rgb_d_mul_c(
                          art_gv,
                          coverFactor,
                        & ut_rgb_fg
                        );
                }

                if ( upcColour == arpolviscolour_A )
                {
                    ut_rgb_to_xyz(
                          art_gv,
                        & ut_rgb_fg,
                        & XYZA_DESTINATION_BUFFER_XYZ(x)
                        );

                    XYZA_DESTINATION_BUFFER_ALPHA(x) = coverFactor;
                }
                else
                {
                    polvis_colour_to_ut_rgb(
                          art_gv,
                        & upcColour,
                        & ut_rgb_bg
                        );

                    ut_rgb_d_mul_c(
                          art_gv,
                          1.0 - coverFactor,
                        & ut_rgb_bg
                        );

                    ArUT_RGB  result;

                    ut_rgb_cc_add_c(
                          art_gv,
                        & ut_rgb_bg,
                        & ut_rgb_fg,
                        & result
                        );

                    ut_rgb_to_xyz(
                          art_gv,
                        & result,
                        & XYZA_DESTINATION_BUFFER_XYZ(x)
                        );

                    XYZA_DESTINATION_BUFFER_ALPHA(x) = LIGHTALPHA_SOURCE_BUFFER_ALPHA(x);
                }
            }

            [ self writeDestinationScanlineBuffer: i : y ];
        }
    }


    /* ------------------------------------------------------------------
         Free the image manipulation infrastructure and end the action;
         this also places the destination image on the stack.
    ---------------------------------------------------------------aw- */

    [ self finishImageManipulation
        :   nodeStack ];

    [ REPORTER endAction ];
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];
    [ coder codeUInt: ((unsigned int *) & scColour[0]) ];
    [ coder codeUInt: ((unsigned int *) & scColour[1]) ];
    [ coder codeUInt: ((unsigned int *) & scColour[2]) ];
    [ coder codeBOOL: & scs ];
    [ coder codeUInt: ((unsigned int *) & dopColour) ];
    [ coder codeUInt: ((unsigned int *) & fppColour) ];
    [ coder codeUInt: ((unsigned int *) & fipColour) ];
    [ coder codeUInt: ((unsigned int *) & upcColour) ];
    [ coder codeBOOL: & normalise ];
    [ coder codeDouble: & wavelength ];
}

@end


// ===========================================================================



