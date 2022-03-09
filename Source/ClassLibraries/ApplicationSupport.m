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

#define ART_MODULE_NAME     ApplicationSupport

#include "AdvancedRenderingToolkit.h"

#include "ART_Parser.h"

#include "ART_Parser.h"
#include "ART_ARM_Interface.h"
#include "ColourAndLightSubsystem.h"
#include <unistd.h>

typedef struct ApplicationSupport_GV
{
    char  * mainFileName;
    char  * entireCommandLine;
    char  * platformDescription;
    char  * applicationName;
    
    ArList  arm2artDefineList;
    
    ArNode <ArpNodeStack>  * nodeStack;

    ART_CommandApplicationFeature  app_features;

    BOOL  isrWasManuallySet;
    BOOL  interactiveModePermitted;
    BOOL  polarisationWasManuallySet;
    BOOL  printBanner;
    
    int   numberOfInputFiles;

    id  viewingAction;

    id  insertionpoint_main_options;
    id  insertionpoint_output_options;
    id  insertionpoint_other_options;

    id  helpOpt;
    id  quietOpt;
    id  verboseOpt;
    id  loggingOpt;
    id  threadOpt;
    id  j1Opt;
    id  j2Opt;
    id  j3Opt;
    id  j4Opt;
    id  binaryOpt;
    id  forceARTOpt;
    id  randomSeedOpt;
    id  tagOpt;
    id  extraTagOpt;
    id  outputOpt;
    id  batchOpt;
    id  acsOpt;
    id  rgbISROpt;
    id  s8vISROpt;
    id  s11eISROpt;
    id  s18vISROpt;
    id  s46eISROpt;
    id  polarisableISROpt;
    id  srgbOpt;
    id  argbOpt;
    id  wrgbOpt;
    id  ap0rgbOpt;
    id  ap1rgbOpt;
    id  cctOpt;
    id  iccOpt;
    id  isrOpt;

    id  specialStartupOpt;
    void (*specialStartupFunction)(ART_GV*);
}
ApplicationSupport_GV;

#define APPSUPPORT_GV           art_gv->application_support_gv

#define MAIN_FILENAME           APPSUPPORT_GV->mainFileName
#define ENTIRE_COMMANDLINE      APPSUPPORT_GV->entireCommandLine
#define PLATFORM_DESCRIPTION    APPSUPPORT_GV->platformDescription
#define APPLICATION_NAME        APPSUPPORT_GV->applicationName

#define ARM2ART_DEFINE_LIST \
    APPSUPPORT_GV->arm2artDefineList

#define NODESTACK       APPSUPPORT_GV->nodeStack

#define APP_FEATURES    APPSUPPORT_GV->app_features

#define ART_ISR_WAS_MANUALLY_SET_BY_USER \
        APPSUPPORT_GV->isrWasManuallySet
#define ART_INTERACTIVE_MODE_IS_PERMITTED \
        APPSUPPORT_GV->interactiveModePermitted
#define ART_POLARISATION_WAS_MANUALLY_SET_BY_USER \
        APPSUPPORT_GV->polarisationWasManuallySet
#define PRINT_BANNER \
        APPSUPPORT_GV->printBanner

#define NUMBER_OF_INPUT_FILE_ARGUMENTS \
        APPSUPPORT_GV->numberOfInputFiles

#define VIEWING_ACTION  APPSUPPORT_GV->viewingAction

#define INSERTIONPOINT_MAIN_OPTIONS \
        APPSUPPORT_GV->insertionpoint_main_options
#define INSERTIONPOINT_OUTPUT_OPTIONS \
        APPSUPPORT_GV->insertionpoint_output_options
#define INSERTIONPOINT_OTHER_OPTIONS \
        APPSUPPORT_GV->insertionpoint_other_options

#define HELP_OPT        APPSUPPORT_GV->helpOpt
#define QUIET_OPT       APPSUPPORT_GV->quietOpt
#define VERBOSE_OPT     APPSUPPORT_GV->verboseOpt
#define LOGGING_OPT     APPSUPPORT_GV->loggingOpt
#define THREAD_OPT      APPSUPPORT_GV->threadOpt
#define J1_OPT          APPSUPPORT_GV->j1Opt
#define J2_OPT          APPSUPPORT_GV->j2Opt
#define J3_OPT          APPSUPPORT_GV->j3Opt
#define J4_OPT          APPSUPPORT_GV->j4Opt
#define BINARY_OPT      APPSUPPORT_GV->binaryOpt
#define FORCE_ART_OPT   APPSUPPORT_GV->forceARTOpt
#define RANDOMSEED_OPT  APPSUPPORT_GV->randomSeedOpt
#define TAG_OPT         APPSUPPORT_GV->tagOpt
#define EXTRA_TAG_OPT   APPSUPPORT_GV->extraTagOpt
#define OUTPUT_OPT      APPSUPPORT_GV->outputOpt
#define BATCH_OPT       APPSUPPORT_GV->batchOpt
#define ACS_OPT         APPSUPPORT_GV->acsOpt
#define CSP_ISR_OPT     APPSUPPORT_GV->rgbISROpt
#define S8V_ISR_OPT     APPSUPPORT_GV->s8vISROpt
#define S11E_ISR_OPT    APPSUPPORT_GV->s11eISROpt
#define S18V_ISR_OPT    APPSUPPORT_GV->s18vISROpt
#define S46E_ISR_OPT    APPSUPPORT_GV->s46eISROpt
#define POLARISABLE_OPT APPSUPPORT_GV->polarisableISROpt
#define POLARISABLE_OPT APPSUPPORT_GV->polarisableISROpt
#define SRGB_OPT        APPSUPPORT_GV->srgbOpt
#define ARGB_OPT        APPSUPPORT_GV->argbOpt
#define WRGB_OPT        APPSUPPORT_GV->wrgbOpt
#define AP0RGB_OPT      APPSUPPORT_GV->ap0rgbOpt
#define AP1RGB_OPT      APPSUPPORT_GV->ap1rgbOpt
#define IRGB_OPT        APPSUPPORT_GV->iccOpt
#define CCT_OPT         APPSUPPORT_GV->cctOpt
#define ISR_OPT         APPSUPPORT_GV->isrOpt

#define SPECIAL_STARTUP_OPTION \
    APPSUPPORT_GV->specialStartupOpt
#define SPECIAL_STARTUP_FUNCTION \
    APPSUPPORT_GV->specialStartupFunction

ART_MODULE_INITIALISATION_FUNCTION
(
    APPSUPPORT_GV = ALLOC(ApplicationSupport_GV);

    MAIN_FILENAME = NULL;
    ENTIRE_COMMANDLINE = NULL;
    PLATFORM_DESCRIPTION = NULL;
    APPLICATION_NAME = NULL;
 
    ARM2ART_DEFINE_LIST = ARLIST_EMPTY;

    NODESTACK = NULL;

    APP_FEATURES = art_appfeatures_none_beyond_baseline;

    ART_ISR_WAS_MANUALLY_SET_BY_USER = NO;
    ART_POLARISATION_WAS_MANUALLY_SET_BY_USER = NO;
    ART_INTERACTIVE_MODE_IS_PERMITTED = YES;

    NUMBER_OF_INPUT_FILE_ARGUMENTS = 0;

    VIEWING_ACTION = NULL;

    INSERTIONPOINT_MAIN_OPTIONS   = NULL;
    INSERTIONPOINT_OUTPUT_OPTIONS = NULL;
    INSERTIONPOINT_OTHER_OPTIONS  = NULL;

    HELP_OPT        = NULL;
    QUIET_OPT       = NULL;
    VERBOSE_OPT     = NULL;
    LOGGING_OPT      = NULL;
    THREAD_OPT      = NULL;
    J1_OPT          = NULL;
    J2_OPT          = NULL;
    J3_OPT          = NULL;
    J4_OPT          = NULL;
    BINARY_OPT      = NULL;
    FORCE_ART_OPT   = NULL;
    RANDOMSEED_OPT  = NULL;
    TAG_OPT         = NULL;
    EXTRA_TAG_OPT   = NULL;
    OUTPUT_OPT      = NULL;
    BATCH_OPT       = NULL;
    ACS_OPT         = NULL;
    CSP_ISR_OPT     = NULL;
    S8V_ISR_OPT     = NULL;
    S11E_ISR_OPT    = NULL;
    S18V_ISR_OPT    = NULL;
    S46E_ISR_OPT    = NULL;
    POLARISABLE_OPT = NULL;
    SRGB_OPT        = NULL;
    ARGB_OPT        = NULL;
    WRGB_OPT        = NULL;
    AP0RGB_OPT      = NULL;
    AP1RGB_OPT      = NULL;
    IRGB_OPT        = NULL;
    CCT_OPT         = NULL;
    ISR_OPT         = NULL;

    SPECIAL_STARTUP_OPTION = NULL;
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    if ( MAIN_FILENAME )
        FREE_ARRAY( MAIN_FILENAME );

    if ( ENTIRE_COMMANDLINE )
        FREE_ARRAY( ENTIRE_COMMANDLINE );

    if ( PLATFORM_DESCRIPTION )
        FREE_ARRAY( PLATFORM_DESCRIPTION );

    if ( APPLICATION_NAME )
        FREE_ARRAY( APPLICATION_NAME );

    FREE( APPSUPPORT_GV );
)

const char  * art_application_platform_description(
        const ART_GV  * art_gv
        )
{
    return PLATFORM_DESCRIPTION;
}

const char  * art_application_name(
        const ART_GV  * art_gv
        )
{
    return APPLICATION_NAME;
}

id art_output_option(
        const ART_GV  * art_gv
        )
{
    return OUTPUT_OPT;
}

void art_set_application_name(
              ART_GV  * art_gv,
        const char    * application_name
        )
{
    arstring_s_copy_s(
          application_name,
        & APPLICATION_NAME
        );
}

void art_define_standard_commandline_options(
        ART_GV                         * art_gv,
        ART_CommandApplicationFeature    features,
        const char                     * task_description
        )
{
    APP_FEATURES = features;

    [ OPTION_CATEGORY_TITLE
        :   "Generic system options:"
        ];

    HELP_OPT =
        [ FLAG_OPTION
            :   "help"
            :   "h"
            :   "this help page"
            ];

    if (   ! ( APP_FEATURES & art_appfeatures_quiet_if_args_present )
        && ! ( APP_FEATURES & art_appfeatures_no_verbosity_control ) )
        QUIET_OPT =
            [ FLAG_OPTION
                :   "quiet"
                :   "q"
                :   "quiet operation"
                ];

    if ( ! ( APP_FEATURES & art_appfeatures_no_verbosity_control ) )
        VERBOSE_OPT =
            [ FLAG_OPTION
                :  "verbose"
                :  "v"
                :  "verbose operation"
                ];

    if( ! ( APP_FEATURES & art_appfeatures_no_threading ) )
    {
        THREAD_OPT =
            [ INTEGER_OPTION
                :   "threads"
                :   "j"
                :   "<# of threads>"
                :   "use # threads where applicable"
                ];

        J1_OPT =
            [ FLAG_OPTION
                :   "J1"
                :   "j1"
                :   "use 1 thread where applicable"
                ];

        J2_OPT =
            [ FLAG_OPTION
                :   "J2"
                :   "j2"
                :   "use 2 threads where applicable"
                ];

        J3_OPT =
            [ FLAG_OPTION
                :   "J3"
                :   "j3"
                :   "use 3 threads where applicable"
                ];

        J4_OPT =
            [ FLAG_OPTION
                :   "J4"
                :   "j4"
                :   "use 4 threads where applicable"
                ];

        //   The following three options are just convenience options
        //   in case someone mistypes one of the common "-j ..." options
        //   (i.e. mistypes "-j 2" as "-j2"), so they are removed from
        //   the -h usage screen

        [ J1_OPT removeFromUsageScreen ];
        [ J2_OPT removeFromUsageScreen ];
        [ J3_OPT removeFromUsageScreen ];
        [ J4_OPT removeFromUsageScreen ];
    }

    if( APP_FEATURES & art_appfeatures_pick_random_seed )
    {
        RANDOMSEED_OPT =
            [ INTEGER_OPTION
                :   "randomSeed"
                :   "seed"
                :   "<random seed>"
                :   "set random seed (0 = use system time)"
                ];
    }
    
    if(    APP_FEATURES & art_appfeatures_arm_to_art_conversion
        || APP_FEATURES & art_appfeatures_load_actionsequence )
    {
        BINARY_OPT =
            [ FLAG_OPTION
                :   "binary"
                :   "bin"
                :   "use .arb binary file format"
                ];

        FORCE_ART_OPT =
            [ FLAG_OPTION
                :   "force"
                :   "f"
                :   "force regeneration of .ar[tb] files"
                ];
    }

    char  * task_desc;

    if ( strlen(task_description) > 0 )
    {
        arstring_s_copy_s( task_description, & task_desc );
        arstring_cs_add_component_s( ' ', "options:", & task_desc );
    }
    else
        arstring_s_copy_s( "Application options:", & task_desc );

    INSERTIONPOINT_MAIN_OPTIONS =
        [ OPTION_CATEGORY_TITLE
            :   task_desc
            ];

    if( APP_FEATURES & art_appfeatures_logfile_support )
        LOGGING_OPT =
            [ FLAG_OPTION
                :  "logging"
                :  "log"
                :  "generate a log file"
                ];

    if( APP_FEATURES & art_appfeatures_load_actionsequence )
    {
        ACS_OPT =
            [ STRING_OPTION
                :   "actionSequence"
                :   "acs"
                :   "<sequence>"
                :   "perform supplied action sequence"
                ];
    }

    INSERTIONPOINT_OUTPUT_OPTIONS =
        [ OPTION_CATEGORY_TITLE
            :   "Output options:"
            ];

    if( APP_FEATURES & art_appfeatures_open_result )
        BATCH_OPT =
            [ FLAG_OPTION
                :   "batch"
                :   "b"
                :   "batch mode - do not open result file"
                ];

    if(   APP_FEATURES & art_appfeatures_alter_output_filename
       || APP_FEATURES & art_appfeatures_provide_output_filename )
    {
        if ( features & art_appfeatures_change_isr )
            TAG_OPT =
                [ FLAG_OPTION
                    :   "tagOutput"
                    :   "t"
                    :   "tag output with ISR information"
                    ];

        if( APP_FEATURES & art_appfeatures_alter_output_filename )
        {
            EXTRA_TAG_OPT =
                [ STRING_OPTION
                    :   "extraTag"
                    :   "tt"
                    :   "<label>"
                    :   "tag output with additional label"
                    ];
        }

        if( features & art_appfeatures_mandatory_output_name )
            OUTPUT_OPT =
                [ STRING_OPTION
                    :   "output"
                    :   "o"
                    :   "<outputfile>"
                    :   "set output file name (mandatory!)"
                    ];
        else
        {
            if( APP_FEATURES & art_appfeatures_provide_output_filename )
            {
                OUTPUT_OPT =
                    [ STRING_OPTION
                        :   "output"
                        :   "o"
                        :   "<outputfile>"
                        :   "provide output file name"
                        ];
            }
            else
            {
                OUTPUT_OPT =
                    [ STRING_OPTION
                        :   "output"
                        :   "o"
                        :   "<outputfile>"
                        :   "override default output file name"
                        ];
            }
        }
    }

    [ OPTION_CATEGORY_TITLE
        :   "ARTRAW output and polarisation rendering options:"
        ];

    if( APP_FEATURES & art_appfeatures_change_isr )
    {
        CSP_ISR_OPT =
            [ FLAG_OPTION
                :   "CIEXYZ"
                :   "xyz"
                :   "CIE XYZ colour ARTRAWs"
                ];

        S8V_ISR_OPT =
            [ FLAG_OPTION
                :   "spectrum8"
                :   "s8v"
                :   "40nm spacing, 8 samples, visible range"
                ];

        S11E_ISR_OPT =
            [ FLAG_OPTION
                :   "spectrum11"
                :   "s11e"
                :   "40nm spacing, 11 samples, extended range"
                ];

        S18V_ISR_OPT =
            [ FLAG_OPTION
                :   "spectrum18"
                :   "s18v"
                :   "20nm spacing, 18 samples, visible range"
                ];

        S46E_ISR_OPT =
            [ FLAG_OPTION
                :   "spectrum46"
                :   "s46e"
                :   "10nm spacing, 46 samples, extended range"
                ];

        POLARISABLE_OPT =
            [ FLAG_OPTION
                :   "polarisation"
                :   "p"
                :   "enable polarisation during rendering"
                ];

        ISR_OPT =
            [ FLAG_OPTION
                :   "spectralInfo"
                :   "spci"
                :   "display info for ARTRAW & polarisation settings"
                ];
    }

    if(    APP_FEATURES & art_appfeatures_change_rgb_space
        || APP_FEATURES & art_appfeatures_change_isr )
    {
        [ OPTION_CATEGORY_TITLE
            :   "RGB output space options:"
            ];
        
        SRGB_OPT =
            [ FLAG_OPTION
                :   "sRGB"
                :   "srgb"
                :   "use sRGB"
                ];

        ARGB_OPT =
            [ FLAG_OPTION
                :   "AdobeRGB"
                :   "argb"
                :   "use Adobe 1998 RGB"
                ];

        WRGB_OPT =
            [ FLAG_OPTION
                :   "WideGamutRGB"
                :   "wrgb"
                :   "use Adobe Wide Gamut RGB"
                ];

        AP0RGB_OPT =
            [ FLAG_OPTION
                :   "AP0RGB"
                :   "ap0"
                :   "use ACES AP0 RGB"
                ];

        AP1RGB_OPT =
            [ FLAG_OPTION
                :   "AP1RGB"
                :   "ap1"
                :   "use ACES AP1 RGB"
                ];

        IRGB_OPT =
            [ STRING_OPTION
                :   "iccRGB"
                :   "irgb"
                :   "<ICC profile>"
                :   "use RGB space from a given ICC file"
                ];

        if( APP_FEATURES & art_appfeatures_change_whitepoint )
        {
            CCT_OPT =
                [ STRING_OPTION
                    :   "whitepoint"
                    :   "wp"
                    :   "<wp ID or CCT>"
                    :   "use given whitepoint as adaption colour"
                    ];
        }
    }

    INSERTIONPOINT_OTHER_OPTIONS =
        [ OPTION_CATEGORY_TITLE
            :   "Other options:"
            ];

}

void art_set_option_insertion_point_to_main_options(
        ART_GV  * art_gv
        )
{
    arcoption_set_insertion_point_for_new_options(
        art_gv,
        INSERTIONPOINT_MAIN_OPTIONS
        );
}

void art_set_option_insertion_point_to_additional_options(
        ART_GV  * art_gv
        )
{
    arcoption_set_insertion_point_for_new_options(
        art_gv,
        INSERTIONPOINT_OUTPUT_OPTIONS
        );
}

void art_set_option_insertion_point_to_other_options(
        ART_GV  * art_gv
        )
{
    arcoption_set_insertion_point_for_new_options(
        art_gv,
        INSERTIONPOINT_OTHER_OPTIONS
        );
}

const char  * art_application_main_filename(
        const ART_GV  * art_gv
        )
{
    return
        MAIN_FILENAME;
}

const char  * art_application_entire_commandline(
        const ART_GV  * art_gv
        )
{
    return
        ENTIRE_COMMANDLINE;
}

BOOL art_application_defines_for_arm2art_were_supplied(
        const ART_GV  * art_gv
        )
{
    return
        (BOOL) arlist_length( & ARM2ART_DEFINE_LIST );
}

void art_application_add_arm2art_defines_to_list(
        const ART_GV  * art_gv,
              ArList  * charptr_list
        )
{
    char  * argument;
    
    while ( arlist_pop_cptr_from_head( & ARM2ART_DEFINE_LIST, & argument ) )
    {
        arlist_add_cptr( charptr_list, argument );
    }
}

void art_application_add_to_arm2art_defines(
        const ART_GV  * art_gv,
              char    * define
        )
{
    arlist_add_cptr(
        & ARM2ART_DEFINE_LIST,
          define
        );
}

ArNode <ArpNodeStack> * art_application_nodestack(
        const ART_GV  * art_gv
        )
{
    return
        NODESTACK;
}

ArcOption  * art_application_action_sequence_opt(
        const ART_GV  * art_gv
        )
{
    return
        ACS_OPT;
}

ArNode <ArpAction> * art_application_viewing_action(
        const ART_GV  * art_gv
        )
{
    return
        VIEWING_ACTION;
}

BOOL art_application_isr_was_manually_set(
        const ART_GV  * art_gv
        )
{
    return
        ART_ISR_WAS_MANUALLY_SET_BY_USER;
}

BOOL art_application_polarisation_was_manually_set(
        const ART_GV  * art_gv
        )
{
    return
        ART_POLARISATION_WAS_MANUALLY_SET_BY_USER;
}

int art_application_number_of_input_files(
        const ART_GV  * art_gv
        )
{
    return
        NUMBER_OF_INPUT_FILE_ARGUMENTS;
}

//   Code snippet from stackoverflow to determine whether we are
//   running as a foreground process that is actually attached to
//   a terminal.

//   This is deactivated for debug builds, so that output
//   is shown in Xcode.

int art_is_running_as_foreground_process(
        )
{
#ifdef NDEBUG
    pid_t  fg = tcgetpgrp(STDIN_FILENO);
    return ( fg == getpgrp() );
#else
    return 1;
#endif
}

int art_interactive_mode_permitted(
        const ART_GV  * art_gv
        )
{
    return
        (   ART_INTERACTIVE_MODE_IS_PERMITTED
         && art_is_running_as_foreground_process() );
}

void art_set_special_startup_option_and_function(
        ART_GV  * art_gv,
        id        option,
        void (*function)(ART_GV*)
        )
{
    SPECIAL_STARTUP_OPTION   = option;
    SPECIAL_STARTUP_FUNCTION = function;
}

int art_print_banner_and_process_standard_commandline_options(
              int             argc,
              char         ** argv,
              ART_GV        * art_gv,
        const char          * application_name,
        const char          * application_description,
        const char          * application_long_description,
        const char          * application_usage_line,
        const char          * copyright_message,
        const BOOL            native_ART_app,
        const unsigned int    minArgs,
        const unsigned int    maxArgs
        )
{
    //   Concatenate and store the entire command line.
    
    //   This is used by some ART applications to store the exact invocation
    //   used to create a given image in the ARTRAW file.
    
    size_t  len = 0;
    
    char  *_all_args;

    for( int i = 0; i< argc; i++ )
    {
        len += strlen(argv[i]);
    }

    _all_args = ENTIRE_COMMANDLINE = ALLOC_ARRAY(char,len+argc);

    for( int i = 0; i < argc; i++ )
    {
        memcpy(_all_args, argv[i], strlen(argv[i]));
        _all_args += strlen(argv[i])+1;
        *(_all_args-1) = ' ';
    }
    
    *(_all_args-1) = 0;

    if ( application_usage_line )
        [ ArcOption setUsageLine
            :   art_gv
            :   application_usage_line
            ];

    if ( application_long_description )
        [ ArcOption setSynopsisLine
            :   art_gv
            :   application_long_description
            ];

    arcoption_remove_empty_option_categories(
        art_gv
        );

    argc =
        [ ArcOption parse
            :   art_gv
            :   argc
            :   argv
            ];

    NUMBER_OF_INPUT_FILE_ARGUMENTS = argc - 1;

    PRINT_BANNER = YES;

    //   We do not print the banner if "quiet operation if all goes well"
    //   has been requested.

    if (   ( ! QUIET_OPT )
        && ( argc > 1 )
        && ! [ VERBOSE_OPT hasBeenSpecified ]
       )
        PRINT_BANNER = NO;

    //   And then there are some circumstances where we do print it,
    //   even if the previous condition is true.

    if ( [ HELP_OPT hasBeenSpecified ] )
        PRINT_BANNER = YES;

    unsigned int  numberOfInputFiles = 0;

    if ( minArgs == 2 && maxArgs == 2 )
        numberOfInputFiles = 1;

    if ( minArgs == 3 && maxArgs == 3 )
        numberOfInputFiles = 2;

    if (    ( argc < 2 &&  numberOfInputFiles == 1 )
         && ! [ ISR_OPT hasBeenSpecified ] )
        PRINT_BANNER = YES;

    if (    ( argc < 3 &&  numberOfInputFiles == 2 )
         && ! [ ISR_OPT hasBeenSpecified ] )
        PRINT_BANNER = YES;

    if (      numberOfInputFiles == 2
         && ! [ OUTPUT_OPT hasBeenSpecified ] )
        PRINT_BANNER = YES;

    int  number_of_detected_cores =
        art_maximum_number_of_working_threads( art_gv );

    art_set_application_name(
          art_gv,
          application_name
        );
    
    if ( PRINT_BANNER )
    {
        const char * is_debug_build_string;
        #ifndef NDEBUG
            is_debug_build_string = ", debug build";
        #else
            is_debug_build_string = "";
        #endif

        const char * assertions_on_string;
        #ifdef FOUNDATION_ASSERTIONS
            assertions_on_string = ", foundation assertions activated";
        #else
            assertions_on_string = "";
        #endif
        
        char  * second_line = "";
        char  * third_line = "";
        char  * linked_against = "";

        if ( native_ART_app )
        {
            asprintf(
                & third_line,
                  "%s \n",
                  art_copyright_string
                );
        }
        else
        {
            asprintf(
                & linked_against,
                  "linked against "
                );
            
            if ( copyright_message )
            {
                asprintf(
                    & third_line,
                      "%s \n",
                      copyright_message
                    );
            }
        }
        
        if ( THREAD_OPT )
        {
            if ( number_of_detected_cores > 1 )
            {
                asprintf(
                    & second_line,
                      "%s%s version %s%s%s, %d cores detected",
                      linked_against,
                      art_short_name_string,
                      art_version_string,
                      is_debug_build_string,
                      assertions_on_string,
                      number_of_detected_cores
                    );
            }
            else
            {
                asprintf(
                    & second_line,
                      "%s%s version %s%s%s, 1 core detected",
                      linked_against,
                      art_short_name_string,
                      art_version_string,
                      is_debug_build_string,
                      assertions_on_string
                    );
            }
        }
        else
        {
            asprintf(
                & second_line,
                  "%s%s version %s%s%s",
                  linked_against,
                  art_short_name_string,
                  art_version_string,
                  is_debug_build_string,
                  assertions_on_string
                );
        }
        
        printf( "\n"
            ART_STRING_BOLD
            "%s"
            ART_STRING_NORMAL
            " - %s\n"
            "%s\n"
            "%s\n",
            application_name,
            application_description,
            second_line,
            third_line
            );
    }

    if (    SPECIAL_STARTUP_OPTION
         && [ SPECIAL_STARTUP_OPTION hasBeenSpecified ] )
    {
        SPECIAL_STARTUP_FUNCTION(art_gv);
        AdvancedRenderingToolkit_library_shutdown( art_gv );
        exit(0);
    }

    if ( [ HELP_OPT hasBeenSpecified ] )
    {
        [ ArcOption fprintUsageAndExit
            :   art_gv
            :   stderr
            ];
    }


    if (    ( argc < 2 &&  numberOfInputFiles == 1 )
         && ! [ ISR_OPT hasBeenSpecified ] )
    {
        fprintf(stderr,"No input file specified!\n\n");

        [ ArcOption fprintShortUsageAndExit
            :   art_gv
            :   stderr
            ];
    }

    if (    ( argc < 3 &&  numberOfInputFiles == 2 )
         && ! [ ISR_OPT hasBeenSpecified ] )
    {
        fprintf(stderr,"No input files specified!\n\n");

        [ ArcOption fprintShortUsageAndExit
            :   art_gv
            :   stderr
            ];
    }

    if (      numberOfInputFiles == 2
         && ! [ OUTPUT_OPT hasBeenSpecified ] )
    {
        fprintf(stderr,"No output file name specified, use -o <name>!\n\n");

        [ ArcOption fprintShortUsageAndExit
            :   art_gv
            :   stderr
            ];
    }


/* ---------------------------------------------------------------------------
    Setting up the colour and light subsystem: if requested, change the RGB
    working space, and output related diagnostics.
------------------------------------------------------------------------aw- */

    //   Set the L*a*b* white point

    if ( [ CCT_OPT hasBeenSpecified ] )
    {
        art_set_system_white_point_by_desc( art_gv, [ CCT_OPT cStringValue ] );
    }

    unsigned int  numberOfRequestedWorkingSpaces = 0;

    //   sRGB

    if ( [ SRGB_OPT hasBeenSpecified ] )
    {
        art_set_default_rgbspace_ref ( art_gv, ARCSR_sRGB );
        numberOfRequestedWorkingSpaces++;
    }

    //   AdobeRGB

    if ( [ ARGB_OPT hasBeenSpecified ] )
    {
        art_set_default_rgbspace_ref ( art_gv, ARCSR_aRGB );
        numberOfRequestedWorkingSpaces++;
    }

    //   Wide Gamut RGB

    if ( [ WRGB_OPT hasBeenSpecified ] )
    {
        art_set_default_rgbspace_ref ( art_gv, ARCSR_wRGB );
        numberOfRequestedWorkingSpaces++;
    }

    //   ACES AP0 RGB

    if ( [ AP0RGB_OPT hasBeenSpecified ] )
    {
        art_set_default_rgbspace_ref ( art_gv, ARCSR_ap0RGB );
        numberOfRequestedWorkingSpaces++;
    }

    //   ACES AP1 RGB

    if ( [ AP1RGB_OPT hasBeenSpecified ] )
    {
        art_set_default_rgbspace_ref ( art_gv, ARCSR_ap1RGB );
        numberOfRequestedWorkingSpaces++;
    }

    //   RGB space as defined in a user-supplied ICC file

    if ( [ IRGB_OPT hasBeenSpecified ] )
    {
        //   parse what the user is trying to sell us as ICC file

        id  parsedObject =
            art_parse_file(
                art_gv,
                [ IRGB_OPT cStringValue ],
                YES
                );

        //   check whether the resulting object is, in fact, a colour space

        id <ArpColourSpace>  customRGBColourSpace = 0;

        if ( [ parsedObject conformsToProtocol: @protocol(ArpColourSpace) ] )
            customRGBColourSpace = (ArNode <ArpColourSpace> *) parsedObject;
        else
            ART_ERRORHANDLING_FATAL_ERROR(
                "ICC file does not contain a valid profile"
                );

        art_set_default_rgbspace_ref(
            art_gv,
            [ customRGBColourSpace colourSpaceRef ]
            );

        RELEASE_OBJECT(customRGBColourSpace);

        numberOfRequestedWorkingSpaces++;
    }

    //   Error if more than one RGB working space was requested

    if ( numberOfRequestedWorkingSpaces > 1 )
    {
        ART_ERRORHANDLING_FATAL_ERROR(
            "more than one RGB working space specified"
            );
    }

/* ---------------------------------------------------------------------------
    Setting up the colour and light subsystem: if requested, change
    the ISR and the polarisation status.
------------------------------------------------------------------------aw- */

    //   see what the current ISR is before we manipulate it

    ArDataType  defaultDataType = art_isr( art_gv );
    ArDataType  selectedDataType = art_isr( art_gv );

    //   S8V_ISR_OPT is null if the option to change ISR is disabled

    if ( S8V_ISR_OPT )
    {
        unsigned int  numberOfRequestedISRs = 0;

        if ( [ CSP_ISR_OPT hasBeenSpecified ] )
        {
            ART_ISR_WAS_MANUALLY_SET_BY_USER = YES;
            selectedDataType = ardt_xyz;
            if ( ART_POLARISATION_WAS_MANUALLY_SET_BY_USER )
                selectedDataType |= ardt_polarisable;
            numberOfRequestedISRs++;
        }

        if ( [ S8V_ISR_OPT hasBeenSpecified ] )
        {
            ART_ISR_WAS_MANUALLY_SET_BY_USER = YES;
            selectedDataType = ardt_spectrum8;
            if ( ART_POLARISATION_WAS_MANUALLY_SET_BY_USER )
                selectedDataType |= ardt_polarisable;
            numberOfRequestedISRs++;
        }

        if ( [ S11E_ISR_OPT hasBeenSpecified ] )
        {
            ART_ISR_WAS_MANUALLY_SET_BY_USER = YES;
            selectedDataType = ardt_spectrum11;
            if ( ART_POLARISATION_WAS_MANUALLY_SET_BY_USER )
                selectedDataType |= ardt_polarisable;
            numberOfRequestedISRs++;
        }

        if ( [ S18V_ISR_OPT hasBeenSpecified ] )
        {
            ART_ISR_WAS_MANUALLY_SET_BY_USER = YES;
            selectedDataType = ardt_spectrum18;
            if ( ART_POLARISATION_WAS_MANUALLY_SET_BY_USER )
                selectedDataType |= ardt_polarisable;
            numberOfRequestedISRs++;
        }

        if ( [ S46E_ISR_OPT hasBeenSpecified ] )
        {
            ART_ISR_WAS_MANUALLY_SET_BY_USER = YES;
            selectedDataType = ardt_spectrum46;
            if ( ART_POLARISATION_WAS_MANUALLY_SET_BY_USER )
                selectedDataType |= ardt_polarisable;
            numberOfRequestedISRs++;
        }

        //   Error if more than one ISR was requested

        if ( numberOfRequestedISRs > 1 )
        {
            ART_ERRORHANDLING_FATAL_ERROR(
                "more than one ISR specified"
                );
        }

        if ( [ POLARISABLE_OPT hasBeenSpecified ] )
        {
            ART_POLARISATION_WAS_MANUALLY_SET_BY_USER = YES;
            selectedDataType |= ardt_polarisable;
        }
    }

    //   if the user specified any changes to the status quo, perform them

    if ( selectedDataType != defaultDataType )
        art_set_isr( art_gv, selectedDataType );


/* ---------------------------------------------------------------------------
    Now that the colour and light subsystem finally is in exactly the state
    the user wants it to be, we can print the ISR diagnostics if that
    was requested.
------------------------------------------------------------------------aw- */

    if ( [ ISR_OPT hasBeenSpecified ] )
    {
        printf( "Status of the ArSpectrum subsystem\n");
        printf( "----------------------------------\n");
        printf( "ArSpectrum is set to: ");

        ardt_printf_current_isr( art_gv );
        
        printf("\nNumber of channels   : %d\n",spc_channels(art_gv) );
        
        if ( spc_channels(art_gv) > 3 )
        {
            printf("First visible channel: %d\n",spc_first_visible_channel(art_gv) );
            printf("Channel bounds [nm]: " );
            for ( unsigned int i = 0; i <= spc_channels(art_gv); i++ )
                printf("%5.2f ", NANO_FROM_UNIT( spc_channel_lower_bound(art_gv,i) ) );
            printf("\n" );

            printf("Channel centers [nm]: " );
            for ( unsigned int i = 0; i < spc_channels(art_gv); i++ )
                printf("%5.2f ", NANO_FROM_UNIT( spc_channel_center(art_gv,i) ) );
            printf("\n" );

            printf("Channel width [nm]: " );
            for ( unsigned int i = 0; i < spc_channels(art_gv); i++ )
                printf("%5.2f ", NANO_FROM_UNIT( spc_channel_width(art_gv,i) ) );
        }
        
        printf( "\n\nDefault RGB space is %s \n", ARCSR_NAME( DEFAULT_RGB_SPACE_REF ) );

        printf("\n" );

        arlightandattenuation_printf_current_subsystem_state( art_gv );

        printf( "\n" );

        if (   ( numberOfInputFiles == 1  && argc != 2 )
            || ( numberOfInputFiles == 2  && argc != 3 )
           )
        {
            printf( "No input files specified, exiting.\n\n");
            return 0;
        }
    }

/* ---------------------------------------------------------------------------
    Path mangling: determining the output filename, either by adapting
    the input filename, or by using a user-supplied name. This is already
    done here, because we need the correct output name for the logfile.

    This "mainFileName" is not used directly, but used as the base from which
    the names of the output image, the logfile and any other diagnostic
    output files are derived.
------------------------------------------------------------------------aw- */

    if (      APP_FEATURES & art_appfeatures_mandatory_output_name
         && ! [ OUTPUT_OPT hasBeenSpecified ] )
    {
        fprintf(stderr,"No output file name specified, use -o <name>!\n\n");

        [ ArcOption fprintShortUsageAndExit
            :   art_gv
            :   stderr
            ];
    }

    const char  * mainInputFileName = argv[1];

    //   If the user supplied an output name...

    if ( OUTPUT_OPT && [ OUTPUT_OPT hasBeenSpecified ] )
    {
        //   ...we use it...
        arstring_s_copy_s(
              [ OUTPUT_OPT cStringValue ],
            & MAIN_FILENAME
            );
    }
    else
    {
        //   ... otherwise we use the name of the input file, minus
        //   its extension (.arm, .art, .arb, whatever...)
        arstring_p_copy_without_extension_p(
              mainInputFileName,
            & MAIN_FILENAME
            );
    }

    //   We add information on the colourtype of the renderer to the output
    //   filename if this has been requested.

    if ( TAG_OPT && [ TAG_OPT hasBeenSpecified ] )
    {
        arstring_e_add_extension_p(
              spc_shortname_string( art_gv ),
            & MAIN_FILENAME
            );

        if ( LIGHT_SUBSYSTEM_IS_IN_POLARISATION_MODE )
            arstring_e_add_extension_p(
                  "polarising",
                & MAIN_FILENAME
                );
    }

    //   Same with any user-supplied, arbitrary tags.

    if ( EXTRA_TAG_OPT && [ EXTRA_TAG_OPT hasBeenSpecified ] )
        arstring_e_add_extension_p(
              [ EXTRA_TAG_OPT cStringValue],
            & MAIN_FILENAME
            );


/* ---------------------------------------------------------------------------
    Now that the output filename is complete, we create a master output
    filename node, and place it onto the node stack. All node manipulation
    actions that write output file(s) expect such a node, that tells them
    what the desired output filename is, to be on the bottom of the stack.
------------------------------------------------------------------------aw- */

    NODESTACK = ARNNODESTACK( MAIN_FILENAME );


/* ---------------------------------------------------------------------------
    Output setup for logfiles and screen display - the reporter objects
    used by all ART subsystems get created here.
------------------------------------------------------------------------aw- */

    //   Step 1 - the logfile reporter object

    ArcFileReporter  * fileReporter = NULL;

    //   If the user requested this, a logfile writing reporter object
    //   gets created.

    if (    LOGGING_OPT
         && [ LOGGING_OPT hasBeenSpecified ] )
    {
        char  * logFileName;

        arstring_pe_copy_add_extension_p(
              MAIN_FILENAME,
              "log",
            & logFileName
            );

        FILE  * logfile = fopen(logFileName, "w");

        if (logfile != NULL)
        {
            fileReporter =
                [ FILE_REPORTER
                    :   logfile
                    ];
        }

        FREE_ARRAY( logFileName );
    }

    //   Step 2 - the reporter object for on-screen output

    ArcFileReporter  * screenReporter = NULL;

    //   If the user requested verbose operation...

    if ( [ VERBOSE_OPT hasBeenSpecified ] )
    {
        if ( [ QUIET_OPT hasBeenSpecified ] )
            //   ... we nitpick if the user was unclear on the meaning of the
            //   quiet and verbose options...
            ART_ERRORHANDLING_FATAL_ERROR(
                "verbose quiet operation requesed - this does not make sense"
                );
        else
        {
            //   ...and if he was not...
            if ( QUIET_OPT )
                //   ...and if this is not an application with "stay quiet if
                //   everything goes well" behaviour, we create a reporter
                //   that is just as talkative as the one that is used for
                //   the logfile...
                screenReporter =
                    [ FILE_REPORTER
                        :   stdout
                        ];
            else
                //   ...otherwise a normal reporter is used.
                screenReporter =
                    [ SHORT_FILE_REPORTER
                        :   stdout
                        ];
        }
    }
    else
    {
        //   Otherwise - unless the user does not want *any* screen output -
        //   the screen reporter is a "short reporter", which only outputs
        //   an abreviated version of what gets written into the logfile.
        //   This is the standard behaviour.
        //   (if neither quiet nor verbose options exist, we also assume
        //    this standard behaviour)

        if (   (     QUIET_OPT
                && ! [ QUIET_OPT hasBeenSpecified ] )
            || ( ! QUIET_OPT && ! VERBOSE_OPT ) )
        {
            screenReporter =
                [ SHORT_FILE_REPORTER
                    :   stdout
                    ];
        }
        else
        {
            if ( [ QUIET_OPT hasBeenSpecified ] )
                ART_INTERACTIVE_MODE_IS_PERMITTED = NO;
        }
    }

    //   Step 3 - the master reporter object, i.e. the one in the execution
    //            environment.

    ArcReporter  * reporter = NULL;

    //   If neither screen nor file reporter exist (the user can, after all,
    //   request quiet operation with no logfile), no overall reporter object
    //   gets created, but otherwise this "master reporter" object takes the
    //   file and screen reporters created earlier as its subsidiaries.

    //   The advantage of such a combined reporter is, that anything that an
    //   ART component outputs via the reporter provided by the execEnv, is
    //   automatically displayed on screen *and* written into the logfile
    //   in appropriate form.

    if ( fileReporter || screenReporter )
    {
        reporter =
            [ REPORTER
                :   screenReporter
                :   fileReporter
                ];
    }

    art_set_global_reporter(
        art_gv,
        reporter
        );

/* ---------------------------------------------------------------------------
    The remaining execution environment options are initialised based on the
    user input.
------------------------------------------------------------------------aw- */

    //   The number of threads which are permitted to run concurrently.

    if ( [ THREAD_OPT hasBeenSpecified ] )
    {
        art_set_maximum_number_of_working_threads(
            art_gv,
            [ THREAD_OPT integerValue ]
            );
    }

    if ( ! [ THREAD_OPT hasBeenSpecified ]
        && [ J1_OPT hasBeenSpecified ] )
    {
        art_set_maximum_number_of_working_threads(
            art_gv,
            1
            );
    }

    if ( ! [ THREAD_OPT hasBeenSpecified ]
        && [ J2_OPT hasBeenSpecified ] )
    {
        art_set_maximum_number_of_working_threads(
            art_gv,
            2
            );
    }

    if ( ! [ THREAD_OPT hasBeenSpecified ]
        && [ J3_OPT hasBeenSpecified ] )
    {
        art_set_maximum_number_of_working_threads(
            art_gv,
            3
            );
    }

    if ( ! [ THREAD_OPT hasBeenSpecified ]
        && [ J4_OPT hasBeenSpecified ] )
    {
        art_set_maximum_number_of_working_threads(
            art_gv,
            4
            );
    }

    int  number_of_used_cores =
        art_maximum_number_of_working_threads( art_gv );

#ifdef __APPLE__
    const char  * system_name = "MacOS";
#endif

#ifdef __linux__
    const char  * system_name = "Linux";
#endif

    if ( number_of_detected_cores != number_of_used_cores )
    {
        if ( number_of_used_cores == 1 )
        {
            printf(
                "Using 1 core of %d\n\n",
                number_of_detected_cores
                );

            asprintf(
                & PLATFORM_DESCRIPTION,
                  "%s, 1 core of %d used",
                  system_name,
                  number_of_detected_cores
                );
        }
        else
        {
            if ( number_of_detected_cores > number_of_used_cores )
            {
                printf(
                    "Using %d cores of %d\n\n",
                    number_of_used_cores,
                    number_of_detected_cores
                    );

                asprintf(
                    & PLATFORM_DESCRIPTION,
                      "%s, %d cores of %d used",
                      system_name,
                      number_of_used_cores,
                      number_of_detected_cores
                    );
            }
            else
            {
                printf(
                    "Running %d threads on %d physically available cores\n\n",
                    number_of_used_cores,
                    number_of_detected_cores
                    );

                asprintf(
                    & PLATFORM_DESCRIPTION,
                      "%s, running %d threads on %d physical cores",
                      system_name,
                      number_of_used_cores,
                      number_of_detected_cores
                    );
            }
        }
    }
    else
    {
        if ( number_of_used_cores == 1 )
        {
            asprintf(
                & PLATFORM_DESCRIPTION,
                  "%s, 1 core",
                  system_name
                );
        }
        else
        {
            asprintf(
                & PLATFORM_DESCRIPTION,
                  "%s, %d cores",
                  system_name,
                  number_of_detected_cores
                );
        }
    }

    //   Do we force the regeneration of any ARM files found in the input
    //   stream, even if they are seemingly up-to-date?

    if ( [ FORCE_ART_OPT hasBeenSpecified ] )
        art_set_forcing_of_ar2mart_translation( art_gv, YES );
    else
        art_set_forcing_of_ar2mart_translation( art_gv, NO );

    //   If we have to process ARM files, do we create binary ARB files, or
    //   text-based ART files?

    if ( [ BINARY_OPT hasBeenSpecified ] )
        art_set_native_files_use_binary_io( art_gv, YES );
    else
        art_set_native_files_use_binary_io( art_gv, NO );

/* ---------------------------------------------------------------------------
    Initialisation of the random generator subsystems, by explicitly setting
    the random seed. This is currently only relevant for certain older types
    of random generators, but this will change in the future.
------------------------------------------------------------------------aw- */

    if ( RANDOMSEED_OPT && [ RANDOMSEED_OPT hasBeenSpecified ] )
    {
        long  seed = [ RANDOMSEED_OPT integerValue ];

        //   A user-specified seed value of zero means that we should
        //   initialise the random generators based on the system time.

        if ( seed == 0 )
        {
            ArTime  currentTime;

            artime_now( & currentTime );
            seed = artime_hash( & currentTime );
        }

        [ reporter printf
            :   "RANDOM SEED : %u\n\n"
            ,   seed
            ];

        //  Currently, only these RGs explicitly use the seed value.
        //  This will change.

        arrandom_global_set_seed( art_gv, seed );
    }

    if ( [ BATCH_OPT hasBeenSpecified ] )
        VIEWING_ACTION =
            NOP_ACTION;
    else
    {
        VIEWING_ACTION =
            OPEN_RESULT_IMAGE_IN_EXTERNAL_VIEWER_ACTION;
    }

    return 1;
}

BOOL art_application_runs_in_quiet_mode(
        ART_GV  * art_gv
        )
{
    if ( PRINT_BANNER )
        return NO;
    else
        return YES;
}

//   ---- CUT HERE ----

//   These four functions belong somewhere in Foundation - to be moved...

char  * escape_search(
        const char    * str,
              size_t    start,
              char      char_search
        )
{
    char * pStartOfNextArg = NULL;
    int  str_len = strlen( str );
    
    int start_search = start;
    
    do {
        pStartOfNextArg = strchr( str + start_search, char_search);

        if (!pStartOfNextArg) break;

        ++pStartOfNextArg;
        start_search = pStartOfNextArg - str;

    } while ( start_search < str_len &&
              start_search-2 > 0 &&
              str[start_search - 2] == '\\'
              );

    return pStartOfNextArg;
}

// This function doesnt escape \\ for now.
void  cptr_argumentlist_add_space_separated_args(
              ArList  * argument_list,
        const char    * argument_string
        )
{
    if ( ! argument_string )
        return ;
    
    int  argBegin = 0;
    int  argument_string_len = strlen( argument_string );
    
    while ( argument_string_len > argBegin )
    {
        char  * startOfNextArg = NULL;
        
        // 1 - Escaping any space with "
        if ( argument_string[argBegin] == '"' ) {
            startOfNextArg = escape_search(argument_string, argBegin + 1, '"');
        } else {
            
            char * firstEqual = strchr( argument_string + argBegin, '=' );
            char * firstSpace = escape_search( argument_string, argBegin, ' ');

            if (firstEqual != NULL && (firstEqual < firstSpace || firstSpace == NULL) // = found before first space
                && (argument_string - firstEqual) + 2 <= argument_string_len      // =<c><c> length as least
                && *(firstEqual + 1) == '"') {                                    // =" found

                startOfNextArg = escape_search(firstEqual, 2, '"');
            } else {
                // If no " or = found, use space to separate arguments
                if ( firstSpace )
                    startOfNextArg = firstSpace - 1;
            }
        }

        int  argument_len;

        if ( startOfNextArg )
        {
            argument_len =
                  startOfNextArg
                - ( argument_string + argBegin );
        }
        else
        {
            argument_len = strlen( argument_string + argBegin );
        }

        char  * char_buffer = ALLOC_ARRAY(char, argument_len + 1 );
        
        strncpy(
            char_buffer,
            argument_string + argBegin,
            argument_len
            );

        strncpy(
              char_buffer + argument_len,
              "\0",
              1
              );

        arlist_add_cptr( argument_list, char_buffer );

        if ( startOfNextArg )
            argBegin = startOfNextArg - argument_string + 1;
        else
            argBegin = argument_string_len;
    }
}



void cptr_argumentlist_add_stringarray_with_prefix(
              ArList   * argument_list,
        const char    ** argument_stringarray,
        const char     * prefix
        )
{
    int     i = 0;

    while ( argument_stringarray[i] )
    {
        char  * new_entry;

        asprintf(
            & new_entry,
              "%s%s",
              prefix,
              argument_stringarray[i]
            );

        arlist_add_cptr( argument_list, new_entry );
        i++;
    }
}

void stringarray_from_cptr_argumentlist(
        char    *** argument_list_to_create,
        ArList    * cptr_argument_list
        )
{
    int  argument_list_len =
        arlist_length( cptr_argument_list );

    *argument_list_to_create =
        ALLOC_ARRAY( char *, argument_list_len + 1 );

    char  * argument;
    int     i = 0;

    while ( arlist_pop_cptr_from_head( cptr_argument_list, & argument ) )
    {
        (*argument_list_to_create)[i++] = argument;
    }

    (*argument_list_to_create)[argument_list_len] = 0;
}


// ===========================================================================
