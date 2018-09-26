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
/**
 * @file ARM_Action.h
 * @brief Actions
 * @type Action
 */

#import "ART_Foundation.h"

ART_MODULE_INTERFACE(ARM_Action)

#import "ART_Scenegraph.h"

/**
 * @brief Internal Spectral Representation selection action
 *
 * These actions change the Internal Spectral Representation (ISR) of
 * ART. You also can select the mode when invoking \verb?artist? by using the
 * relevant flags.
 *
 * You can put one of those define in the Action Sequence othewise.\\
 *
 \noindent\begin{tabularx}{\textwidth}{lXl}
    \toprule
    \textbf{Action}                        & \textbf{Description}                      & \textbf{artist flags}   \\
    \toprule
    \verb?SET_ISR_TO_PLAIN_RGB?            & RGB                                       & \verb?-rgb?     \\
    \verb?SET_ISR_TO_RGB?                  &                                           &                 \\
    \midrule
    \verb?SET_ISR_TO_POLARISABLE_RGB?      & Polarisation mode in RGB                  & \verb?-rgb -p?  \\
    \midrule
    \verb?SET_ISR_TO_PLAIN_SPECTRUM_8?     & 8 spectral samples                        & \verb?-s8?      \\
    \verb?SET_ISR_TO_S8?                   &                                           &                 \\
    \verb?SET_ISR_TO_SPECTRUM_8?           &                                           &                 \\
    \midrule
    \verb?SET_ISR_TO_POLARISABLE_S8?       & Polarisation mode with 8 spectral samples  & \verb?-s8 -p?  \\
    \midrule
    \verb?SET_ISR_TO_PLAIN_SPECTRUM_16?    & 16 spectral samples                        & \verb?-s18?    \\
    \verb?SET_ISR_TO_s18?                  &                                            &                \\
    \verb?SET_ISR_TO_SPECTRUM_16?          &                                            &                \\
    \midrule
    \verb?SET_ISR_TO_POLARISABLE_s18?      & Polarisation mode with 16 spectral samples & \verb?-s18 -p? \\
    \midrule
    \verb?SET_ISR_TO_PLAIN_SPECTRUM_45?    & 45 spectral samples                        & \verb?-s46?    \\
    \verb?SET_ISR_TO_s46?                  &                                            &                \\
    \verb?SET_ISR_TO_SPECTRUM_45?          &                                            &                \\
    \midrule
    \verb?SET_ISR_TO_POLARISABLE_s46?      & Polarisation mode with 45 spectral samples & \verb?-s46 -p? \\
    \bottomrule
 \end{tabularx}
 *
 * @def SET_ISR_ACTION(c)
 *
 * @param c SpectrumType    The type of the spectrum that is going to be used
 */
#define SET_ISR_ACTION(__c) \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnSetISRAction) \
        : (__c) \
        ]

#define SET_ISR_TO_PLAIN_RGB \
    SET_ISR_ACTION(arspectrum_ut_rgb)
#define SET_ISR_TO_POLARISABLE_RGB \
    SET_ISR_ACTION(arspectrum_ut_rgb_polarisable)
#define SET_ISR_TO_RGB                SET_ISR_TO_PLAIN_RGB

#define SET_ISR_TO_PLAIN_SPECTRUM_8 \
    SET_ISR_ACTION(arspectrum_spectrum8)
#define SET_ISR_TO_POLARISABLE_SPECTRUM_8 \
    SET_ISR_ACTION(arspectrum_spectrum8_polarisable)
#define SET_ISR_TO_SPECTRUM_8         SET_ISR_TO_PLAIN_SPECTRUM_8
#define SET_ISR_TO_S8                 SET_ISR_TO_SPECTRUM_8
#define SET_ISR_TO_PLAIN_S8           SET_ISR_TO_PLAIN_SPECTRUM_8
#define SET_ISR_TO_POLARISABLE_S8     SET_ISR_TO_POLARISABLE_SPECTRUM_8

#define SET_ISR_TO_PLAIN_SPECTRUM_16 \
    SET_ISR_ACTION(arspectrum_spectrum18)
#define SET_ISR_TO_POLARISABLE_SPECTRUM_16 \
    SET_ISR_ACTION(arspectrum_spectrum18_polarisable)
#define SET_ISR_TO_SPECTRUM_16        SET_ISR_TO_PLAIN_SPECTRUM_16
#define SET_ISR_TO_s18                SET_ISR_TO_SPECTRUM_16
#define SET_ISR_TO_PLAIN_s18          SET_ISR_TO_PLAIN_SPECTRUM_16
#define SET_ISR_TO_POLARISABLE_s18    SET_ISR_TO_POLARISABLE_SPECTRUM_16

#define SET_ISR_TO_PLAIN_SPECTRUM_45 \
    SET_ISR_ACTION(arspectrum_spectrum46)
#define SET_ISR_TO_POLARISABLE_SPECTRUM_45 \
    SET_ISR_ACTION(arspectrum_spectrum46_polarisable)
#define SET_ISR_TO_SPECTRUM_45        SET_ISR_TO_PLAIN_SPECTRUM_45
#define SET_ISR_TO_s46                SET_ISR_TO_SPECTRUM_45
#define SET_ISR_TO_PLAIN_s46          SET_ISR_TO_PLAIN_SPECTRUM_45
#define SET_ISR_TO_POLARISABLE_s46    SET_ISR_TO_POLARISABLE_SPECTRUM_45


/**
 * @section Image actions
 */
    #import "ART_ImageActions.h"

    /**
     * @pushsection Visualisations and operations
     */

        /**
         * @def LINEAR_POLARISATION_FILTER
         */
        #define LINEAR_POLARISATION_FILTER \
                ALLOC_OBJECT_AUTORELEASE(ArnARTRAWLinearPolarisingFilter)

        /**
         * @def ARTRAW_POLARISATION_VISUALISATION
         */
        #define ARTRAW_POLARISATION_VISUALISATION \
                ALLOC_OBJECT_AUTORELEASE(ArnARTRAWPolarisationVisualisation)

        /**
         * @def OUTPUT_ART_CURRENT_ISR_ACTION
         */
        #define OUTPUT_ART_CURRENT_ISR_ACTION \
                ALLOC_OBJECT_AUTORELEASE(ArnOutputCurrentISR)

        /**
         * @def CHANGE_ISR_TO_MATCH_ARTRAW_CONTENTS_ACTION
         */
        #define CHANGE_ISR_TO_MATCH_ARTRAW_CONTENTS_ACTION \
                [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnChangeISR_to_Match_ARTRAW_Contents) \
                    :   ISR_CHANGE_PERFORM_NO_WAVELENGTH_CHECK \
                    :   NO \
                    ] \
        /**
         * @def CHANGE_ISR_TO_MATCH_ARTRAW_CONTENTS_CHECK_WL_ACTION(wavelength)
         *
         * @param wavelength    double  Wavelength (in \verb?NANOMETERS?).
         */
        #define CHANGE_ISR_TO_MATCH_ARTRAW_CONTENTS_CHECK_WL_ACTION(__wl) \
                [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnChangeISR_to_Match_ARTRAW_Contents) \
                    :   (__wl) \
                    :   NO \
                    ]
        /**
         * @def CHANGE_ISR_TO_MATCH_ARTRAW_CONTENTS_CHECK_WL_REQUIRE_POL_ACTION(wavelength)
         *
         * @param wavelength    double  Wavelength (in \verb?NANOMETERS?).
         */
        #define CHANGE_ISR_TO_MATCH_ARTRAW_CONTENTS_CHECK_WL_REQUIRE_POL_ACTION(__wl) \
                [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnChangeISR_to_Match_ARTRAW_Contents) \
                    :   (__wl) \
                    :   YES \
                    ]

        /**
         * @def MUL_ARTRAW_IMAGE
         */
        #define MUL_ARTRAW_IMAGE \
                ALLOC_OBJECT_AUTORELEASE(ArnARTRAW_Double_Mul_ARTRAW)

        /**
         * @def DOWNSCALE_ARTRAW_IMAGE
         */
        #define DOWNSCALE_ARTRAW_IMAGE \
                ALLOC_OBJECT_AUTORELEASE(ArnDownscaleARTRAW)

        /**
         * @def ADD_2_ARTRAW_IMAGES
         */
        #define ADD_2_ARTRAW_IMAGES \
                ALLOC_OBJECT_AUTORELEASE(Arn2xARTRAW_Add_ARTRAW)


        /**
         * @def GENERATE_2xARTCSP_TO_ARTGSC_DIFFERENCE_IMAGE
         * Creates an ARTGSC difference image from two ARTCSP images on the stack.
         */
        #define GENERATE_2xARTCSP_TO_ARTGSC_DIFFERENCE_IMAGE \
                ALLOC_OBJECT_AUTORELEASE(Arn2xARTCSP_To_ARTGSC_DifferenceImage)

        /**
         * @def GENERATE_2xARTRAW_TO_ARTGSC_DIFFERENCE_IMAGE
         * Creates an ARTGSC difference image from two ARTRAW images on the stack.
         */
        #define GENERATE_2xARTRAW_TO_ARTGSC_DIFFERENCE_IMAGE \
                ALLOC_OBJECT_AUTORELEASE(Arn2xARTRAW_To_ARTGSC_DifferenceImage)

        #define COMPUTE_2xARTRAW_SNR \
                ALLOC_OBJECT_AUTORELEASE(Arn2xARTRAW_SNR)

        #define COMPUTE_2xARTCSP_AVG_DIFF \
                ALLOC_OBJECT_AUTORELEASE(Arn2xARTCSP_avg_diff)
    /**
     * @popsection
     */

    /**
     * @pushsection Image conversion actions
     *
     \begin{center}
     \noindent\begin{tabular}{cl|cccccc}
     \toprule

                                                                                        & &\multicolumn{6}{c}{\textbf{Target}}                                       \\
                                                      &                 & \textbf{ARTRAW} & \textbf{ARTCSP} & \textbf{ARTGSC} & \textbf{TIFF} & \textbf{EXR} & \textbf{CSV} \\
     \midrule
     \multirow{5}{*}{\rotatebox{90}{\textbf{Source}}} & \textbf{ARTRAW} & Same             & Y              & Y                &               &             &        \\
                                                      & \textbf{ARTCSP} &                  & Same           &                  & Y             & Y           &        \\
                                                      & \textbf{ARTGSC} &                  &                & Same             & Y             & Y           & Y      \\
                                                      & \textbf{TIFF}   &                  & Y              &                  & Same          &             &        \\
                                                      & \textbf{EXR}    &                  & Y              &                  &               & Same        &        \\
     \bottomrule

     \end{tabular}
     \end{center}
     */
        /**
         * @def IMAGECONVERSION_ARTRAW_TO_ARTCSP
         * @brief ARTRAW to ARTCSP
         * Converts an ARTRAW image from the stack to an ARTCSP image.
         */
        #define IMAGECONVERSION_ARTRAW_TO_ARTCSP \
                ALLOC_OBJECT_AUTORELEASE(ArnImageConverter_ARTRAW_To_ARTCSP)

        /**
         * @def IMAGECONVERSION_ARTRAW_TO_MONO_ARTCSP
         * @brief ARTRAW to monochromatic ARTCSP
         * Converts an ARTRAW image from the stack to a monochromatic ARTCSP image.
         */
        #define IMAGECONVERSION_ARTRAW_TO_MONO_ARTCSP \
                ALLOC_OBJECT_AUTORELEASE(ArnImageConverter_ARTRAW_To_Monochrome_ARTCSP)

        /**
         * @def IMAGECONVERSION_ARTRAW_TO_SINGLECHANNEL_ARTGSC
         * @brief ARTRAW to single channel ARTGSC
         * Converts an ARTRAW image from the stack to a monochromatic ARTGSC
         * image with the selected wavelength.
         */
        #define IMAGECONVERSION_ARTRAW_TO_SINGLECHANNEL_ARTGSC \
                ALLOC_OBJECT_AUTORELEASE(ArnImageConverter_ARTRAW_To_Singlechannel_ARTGSC)

        /**
         * @def IMAGECONVERSION_ARTRAW_TO_SINGLECHANNEL_ARTGSCs
         * @brief ARTRAW to single channel ARTGSPs
         * Converts each wavelength channel of an ARTRAW image from the stack
         * to a set of monochromatic ARTGSP images.
         */
        #define IMAGECONVERSION_ARTRAW_TO_SINGLECHANNEL_ARTGSCs \
                ALLOC_OBJECT_AUTORELEASE(ArnImageConverter_ARTRAW_To_Singlechannel_ARTGSCs)

        /**
         * @def IMAGECONVERSION_ARTCSP_TO_TIFF
         * @brief ARTCSP to TIFF
         * Converts an ARTCSP image from the stack to a TIFF image.
         */
        #define IMAGECONVERSION_ARTCSP_TO_TIFF \
                ALLOC_OBJECT_AUTORELEASE(ArnImageConverter_ARTCSP_To_TIFF)

        /**
         * @def IMAGECONVERSION_TIFF_TO_ARTCSP
         * @brief TIFF to ARTCSP
         * Converts a TIFF image from the stack to an ARTCSP image.
         */
        #define IMAGECONVERSION_TIFF_TO_ARTCSP \
                ALLOC_OBJECT_AUTORELEASE(ArnImageConverter_TIFF_To_ARTCSP)

        /**
         * @def IMAGECONVERSION_ARTGSC_TO_TIFF
         * @brief ARTGSC to TIFF
         * Converts an ARTGSC image from the stack to a TIFF image.
         */
        #define IMAGECONVERSION_ARTGSC_TO_TIFF \
                ALLOC_OBJECT_AUTORELEASE(ArnImageConverter_ARTGSC_To_TIFF)

        /**
         * @def IMAGECONVERSION_ARTGSC_TO_CSV
         * @brief ARTGSC to CSV
         * Converts an ARTGSC image from the stack to a CSV file.
         */
        #define IMAGECONVERSION_ARTGSC_TO_CSV \
                ALLOC_OBJECT_AUTORELEASE(ArnImageConverter_ARTGSC_To_GreyCSV)

        #ifdef ART_WITH_OPENEXR
            /**
             * @def IMAGECONVERSION_ARTCSP_TO_EXR
             * @brief ARTCSP to EXR
             * Converts an ARTCSP image from the stack to an EXR file.
             */
            #define IMAGECONVERSION_ARTCSP_TO_EXR \
                    ALLOC_OBJECT_AUTORELEASE(ArnImageConverter_ARTCSP_To_EXR)

            /**
             * @def IMAGECONVERSION_ARTGSC_TO_EXR
             * @brief ARTGSC to EXR
             * Converts an ARTGSC image from the stack to an EXR file.
             */
            #define IMAGECONVERSION_ARTGSC_TO_EXR \
                    ALLOC_OBJECT_AUTORELEASE(ArnImageConverter_ARTGSC_To_EXR)

            /**
             * @def IMAGECONVERSION_EXR_TO_ARTCSP
             * @brief EXR to ARTCSP
             * Converts an EXR image from the stack to an ARTCSP image.
             */
            #define IMAGECONVERSION_EXR_TO_ARTCSP \
                    ALLOC_OBJECT_AUTORELEASE(ArnImageConverter_EXR_To_ARTCSP)

        #endif

    /**
     * @popsection # Image conversion actions
     */

    /**
     * @pushsection Tonemapping actions
     */
        /**
         * @def EXPONENTIAL_TONEMAPPING_OPERATOR
         */
        #define EXPONENTIAL_TONEMAPPING_OPERATOR \
                ALLOC_OBJECT_AUTORELEASE(ArnExponentialToneMapper)

        /**
         * @def STANDARD_EXPONENTIAL_TONEMAPPING_OPERATOR
         */
        #define STANDARD_EXPONENTIAL_TONEMAPPING_OPERATOR \
                [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnExponentialToneMapper) ]

        /**
         * @def INTERACTIVE_CALIBRATION_TONEMAPPING_OPERATOR
         */
        #define INTERACTIVE_CALIBRATION_TONEMAPPING_OPERATOR \
                ALLOC_OBJECT_AUTORELEASE(ArnInteractiveCalibrationToneMapper)

        /**
         * @def STANDARD_INTERACTIVE_CALIBRATION_TONEMAPPING_OPERATOR
         */
        #define STANDARD_INTERACTIVE_CALIBRATION_TONEMAPPING_OPERATOR \
                [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnInteractiveCalibrationToneMapper) ]

        /**
         * @def STANDARD_LINEAR_GLOBAL_TONEMAPPING_OPERATOR
         */
        #define STANDARD_LINEAR_GLOBAL_TONEMAPPING_OPERATOR \
                STANDARD_INTERACTIVE_CALIBRATION_TONEMAPPING_OPERATOR

        /**
         * @def STANDARD_GLOBAL_TONEMAPPING_OPERATOR
         */
        #define STANDARD_GLOBAL_TONEMAPPING_OPERATOR \
                STANDARD_LINEAR_GLOBAL_TONEMAPPING_OPERATOR

        /**
         * @def ARTCSP_LUMINANCE_CLIPPING
         */
        #define ARTCSP_LUMINANCE_CLIPPING \
                ALLOC_OBJECT_AUTORELEASE(ArnARTCSPLuminanceClipping)

        /**
         * @def ARTCSP_LUMINANCE_CLIPPING_WITH_WHITE_LUMINANCE(whiteLuminance)
         */
        #define ARTCSP_LUMINANCE_CLIPPING_WITH_WHITE_LUMINANCE(__whiteLuminance) \
                [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnARTCSPLuminanceClipping) \
                    :   YES \
                    :   (__whiteLuminance) \
                    ]

        /**
         * @def ARTCSP_LUMINANCE_CLIPPING_WWL
         */
        #define ARTCSP_LUMINANCE_CLIPPING_WWL \
                ARTCSP_LUMINANCE_CLIPPING_WITH_WHITE_LUMINANCE

        /**
         * @def STANDARD_LUMINANCE_CLIPPING
         */
        #define STANDARD_LUMINANCE_CLIPPING \
                ARTCSP_LUMINANCE_CLIPPING_WITH_WHITE_LUMINANCE( \
                    ARNARTCSP_LUMINANCE_CLIPPING_DEFAULT_WHITE_LUMINANCE \
                    )

        /**
         * @def OPEN_RESULT_IMAGE_IN_EXTERNAL_VIEWER_ACTION
         * Open the image generated after previous operations.
         */
        #define OPEN_RESULT_IMAGE_IN_EXTERNAL_VIEWER_ACTION \
                [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnOpenImageInExternalViewer) \
                    ]

        /**
         * @def FILTER_TINY_ARTRAW_VALUES
         */
        #define FILTER_TINY_ARTRAW_VALUES \
                ALLOC_OBJECT_AUTORELEASE(ArnFilterTinyARTRAWValues)

        /**
         * @def FILTER_HIGH_DOP_ARTRAW_VALUES
         */
        #define FILTER_HIGH_DOP_ARTRAW_VALUES \
                ALLOC_OBJECT_AUTORELEASE(ArnFilterHighDopARTRAWValues)
    /**
     * @popsection # Tonemapping actions
     */

/**
 * @popsection # Image actions
 */

/**
 * @section Scene graph actions
 */
#import "ART_ActionSequence.h"

ArNode <ArpAction> * nop_action_singleton( ART_GV  * art_gv );
ArNode <ArpAction> * remove_externals_action_singleton( ART_GV  * art_gv );
ArNode <ArpAction> * read_extra_data_action_singleton( ART_GV  * art_gv );
ArNode <ArpAction> * setup_node_data_action_singleton( ART_GV  * art_gv );
ArNode <ArpAction> * convert_to_tree_action_singleton( ART_GV  * art_gv );
ArNode <ArpAction> * combine_attributes_action_singleton( ART_GV  * art_gv );
ArNode <ArpAction> * combine_print_csg_tree_singleton( ART_GV  * art_gv );
ArNode <ArpAction> * alloc_bboxes_action_singleton( ART_GV  * art_gv );
ArNode <ArpAction> * init_bboxes_action_singleton( ART_GV  * art_gv );
ArNode <ArpAction> * shrink_bboxes_action_singleton( ART_GV  * art_gv );
ArNode <ArpAction> * optimise_bboxes_action_singleton( ART_GV  * art_gv );

#define NOP_ACTION_SINGLETON            nop_action_singleton( art_gv )
#define SCENEGRAPH_REMOVE_EXTERNALS     remove_externals_action_singleton( art_gv )
#define SCENEGRAPH_READ_EXTRA_DATA      read_extra_data_action_singleton( art_gv )
#define SCENEGRAPH_SETUP_DATA           setup_node_data_action_singleton( art_gv )
#define SCENEGRAPH_CREATE_FLATTENED_COPY convert_to_tree_action_singleton( art_gv )
#define SCENEGRAPH_COMBINE_ATTRIBUTES   combine_attributes_action_singleton( art_gv )
#define SCENEGRAPH_PRINT_CSG_TREE       combine_print_csg_tree_singleton( art_gv )
#define SCENEGRAPH_ALLOC_BBOXES         alloc_bboxes_action_singleton( art_gv )
#define SCENEGRAPH_INIT_BBOXES          init_bboxes_action_singleton( art_gv )
#define SCENEGRAPH_SHRINK_BBOXES        shrink_bboxes_action_singleton( art_gv )
#define SCENEGRAPH_OPTIMISE_BBOXES      optimise_bboxes_action_singleton( art_gv )

/**
 * @popsection # Scene graph actions
 */



ArNode <ArpAction> * scenegraph_bounding_box_insertion(
        ART_GV  * art_gv
        );

ArNode <ArpAction> * scenegraph_raycasting_optimisations_create(
        ART_GV  * art_gv
        );

ArNode <ArpAction> * scenegraph_raycasting_optimisations(
        ART_GV  * art_gv
        );



#define STANDARD_RAYCASTER \
        standard_raycaster(art_gv)

#define SCENEGRAPH_INSERT_BOUNDING_BOXES \
        scenegraph_bounding_box_insertion(art_gv)

#define CREATE_STANDARD_RAYCASTING_ACCELERATION_STRUCTURE \
        scenegraph_raycasting_optimisations(art_gv)



#define STANDARD_RAYCASTER_CREATOR \
        standard_raycaster

#define SCENEGRAPH_INSERT_BOUNDING_BOXES_CREATOR \
        scenegraph_bounding_box_insertion

#define CREATE_STANDARD_RAYCASTING_ACCELERATION_STRUCTURE_CREATOR \
        scenegraph_raycasting_optimisations_create
