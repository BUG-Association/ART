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

ART_MODULE_INTERFACE(ApplicationSupport)

#include "ART_MiscellaneousClasses.h"

//   These flags started out as a fairly orthogonal set of bit
//   values. Which they are no longer - gradual evolution, and all that.

typedef enum
{
    art_appfeatures_none_beyond_baseline    = 0x0000,
    art_appfeatures_logfile_support         = 0x0001,
    art_appfeatures_pick_random_seed        = 0x0002,
    art_appfeatures_provide_output_filename = 0x0004,
    art_appfeatures_alter_output_filename   = 0x0008,
    art_appfeatures_mandatory_output_name   = 0x0010,
    art_appfeatures_open_result             = 0x0020,
    art_appfeatures_arm_to_art_conversion   = 0x0040,
    art_appfeatures_load_actionsequence     = 0x0080,
    art_appfeatures_change_rgb_space        = 0x0100,
    art_appfeatures_change_isr              = 0x0200,
    art_appfeatures_change_whitepoint       = 0x0400,
    art_appfeatures_no_threading            = 0x0800,
    art_appfeatures_quiet_if_args_present   = 0x1000,
    art_appfeatures_no_verbosity_control    = 0x2000
}
ART_CommandApplicationFeature;

void art_define_standard_commandline_options(
        ART_GV                         * art_gv,
        ART_CommandApplicationFeature    features,
        const char                     * task_description
        );

#define ART_APPLICATION_DEFINE_STANDARD_OPTIONS \
    art_define_standard_commandline_options( \
        art_gv, \
        art_appfeatures_none_beyond_baseline \
        | art_appfeatures_no_threading, \
        "" \
        );

#define ART_APPLICATION_DEFINE_STANDARD_OPTIONS_WITH_FEATURES( __task_desc, __features ) \
    art_define_standard_commandline_options( \
        art_gv, \
        __features, \
        __task_desc \
        );


void art_set_option_insertion_point_to_main_options(
        ART_GV  * art_gv
        );

#define ART_APPLICATION_MAIN_OPTIONS_FOLLOW \
        art_set_option_insertion_point_to_main_options( art_gv );


void art_set_option_insertion_point_to_additional_options(
        ART_GV  * art_gv
        );

#define ART_APPLICATION_ADDITIONAL_OUTPUT_OPTIONS_FOLLOW \
        art_set_option_insertion_point_to_additional_options( art_gv );


void art_set_option_insertion_point_to_other_options(
        ART_GV  * art_gv
        );

#define ART_APPLICATION_OTHER_OPTIONS_FOLLOW \
        art_set_option_insertion_point_to_other_options( art_gv );


const char  * art_application_main_filename(
        const ART_GV  * art_gv
        );

#define ART_APPLICATION_MAIN_FILENAME \
        art_application_main_filename( art_gv )


const char  * art_application_platform_description(
        const ART_GV  * art_gv
        );

#define ART_APPLICATION_PLATFORM_DESCRIPTION \
        art_application_platform_description( art_gv )


const char  * art_application_name(
        const ART_GV  * art_gv
        );

void art_set_application_name(
              ART_GV  * art_gv,
        const char    * application_name
        );

#define ART_APPLICATION_NAME \
        art_application_name( art_gv )


const char  * art_application_entire_commandline(
        const ART_GV  * art_gv
        );

#define ART_APPLICATION_ENTIRE_COMMANDLINE \
        art_application_entire_commandline( art_gv )


ArNode <ArpNodeStack> * art_application_nodestack(
        const ART_GV  * art_gv
        );

#define ART_APPLICATION_NODESTACK \
        art_application_nodestack( art_gv )

#define ART_APPLICATION_NODESTACK_PUSH(__obj) \
    [ ART_APPLICATION_NODESTACK push \
        :   HARD_NODE_REFERENCE(__obj) \
        ];

#define ART_APPLICATION_NODESTACK_PUSH_WEAK_REF(__obj) \
    [ ART_APPLICATION_NODESTACK push \
        :   WEAK_NODE_REFERENCE(__obj) \
        ];


ArcOption  * art_application_action_sequence_opt(
        const ART_GV  * art_gv
        );

#define ART_ACTION_SEQUENCE_OPT \
        art_application_action_sequence_opt( art_gv )


BOOL art_application_defines_for_arm2art_were_supplied(
        const ART_GV  * art_gv
        );

#define ART_DEFINES_FOR_ARM2ART_WERE_SUPPLIED_BY_USER \
        art_application_defines_for_arm2art_were_supplied( art_gv )

void art_application_add_arm2art_defines_to_list(
        const ART_GV  * art_gv,
              ArList  * charptr_list
        );

#define ART_ARM2ART_DEFINES \
        art_application_arm2art_defines( art_gv )

void art_application_add_to_arm2art_defines(
        const ART_GV  * art_gv,
              char    * define
        );


ArNode <ArpAction> * art_application_viewing_action(
        const ART_GV  * art_gv
        );

#define ART_VIEWING_ACTION \
        art_application_viewing_action( art_gv )


BOOL art_application_isr_was_manually_set(
        const ART_GV  * art_gv
        );

#define ART_ISR_WAS_MANUALLY_SET \
        art_application_isr_was_manually_set( art_gv )


BOOL art_application_polarisation_was_manually_set(
        const ART_GV  * art_gv
        );

#define ART_POLARISATION_WAS_MANUALLY_SET \
        art_application_polarisation_was_manually_set( art_gv )


int art_application_number_of_input_files(
        const ART_GV  * art_gv
        );

#define NUMBER_OF_INPUT_FILES \
        art_application_number_of_input_files( art_gv )

int art_is_running_as_foreground_process(
        );

int art_interactive_mode_permitted(
        const ART_GV  * art_gv
        );

#define ART_OUTPUT_OPT \
    art_output_option( art_gv )

id art_output_option(
        const ART_GV  * art_gv
        );


/* ---------------------------------------------------------------------------

    "special" startup option and function

    The purpose of these two things is to provide a "hook" at the very
    beginning of the option processing sequence. The only use of this is
    currently the name explanation feature of bugblatter, but this is
    potentially useful for all those cases where you want to process an
    option a) before all others (including the in-built ones handled by the
    application support module), and b) regardless of what other options
    the user specified. Use with care.

------------------------------------------------------------------------aw- */


void art_set_special_startup_option_and_function(
        ART_GV  * art_gv,
        id        option,
        void (*function)(ART_GV*)
        );

#define ART_APPLICATION_SET_SPECIAL_STARTUP_OPTION_AND_FUNCTION( \
    __option, \
    __function \
    ) \
{ \
    art_set_special_startup_option_and_function( \
        art_gv, \
        __option, \
        __function \
        ); \
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
        );

#define ART_DUAL_INPUT_FILE_APPLICATION_STARTUP(__name,__desc,__ldesc,__usage) \
{ \
    if ( ! art_print_banner_and_process_standard_commandline_options( \
               argc, \
               argv, \
               art_gv, \
               __name, \
               __desc, \
               __ldesc, \
               __usage, \
               NULL, \
               YES, \
               3, \
               3 \
               ) ) \
        return 0; \
}

#define ART_SINGLE_INPUT_FILE_APPLICATION_STARTUP(__name,__desc,__usage) \
{ \
    if ( ! art_print_banner_and_process_standard_commandline_options( \
               argc, \
               argv, \
               art_gv, \
               __name, \
               __desc, \
               "", \
               __usage, \
               NULL, \
               YES, \
               2, \
               2 \
               ) ) \
        return 0; \
}

#define ART_SINGLE_INPUT_FILE_APPLICATION_STARTUP_WITH_SYNOPSIS( \
        __name, \
        __desc, \
        __ldesc, \
        __usage \
        ) \
{ \
    if ( ! art_print_banner_and_process_standard_commandline_options( \
               argc, \
               argv, \
               art_gv, \
               __name, \
               __desc, \
               __ldesc, \
               __usage, \
               NULL, \
               YES, \
               2, \
               2 \
               ) ) \
        return 0; \
}

#define ART_NO_INPUT_FILES_APPLICATION_STARTUP_WITH_SYNOPSIS( \
        __name, \
        __desc, \
        __ldesc, \
        __usage \
        ) \
{ \
    if ( ! art_print_banner_and_process_standard_commandline_options( \
               argc, \
               argv, \
               art_gv, \
               __name, \
               __desc, \
               __ldesc, \
               __usage, \
               NULL, \
               YES, \
               1, \
               1 \
               ) ) \
        return 0; \
}

#define ART_NO_INPUT_FILES_APPLICATION_STARTUP(__name,__desc,__usage) \
{ \
    if ( ! art_print_banner_and_process_standard_commandline_options( \
               argc, \
               argv, \
               art_gv, \
               __name, \
               __desc, \
               NULL, \
               __usage, \
               NULL, \
               YES, \
               1, \
               1 \
               ) ) \
        return 0; \
}

#define ART_EXTERNAL_APP_NO_INPUT_FILES_APPLICATION_STARTUP( \
    __name, \
    __desc, \
    __usage, \
    __copyright \
    ) \
{ \
    if ( ! art_print_banner_and_process_standard_commandline_options( \
               argc, \
               argv, \
               art_gv, \
               __name, \
               __desc, \
               NULL, \
               __usage, \
               __copyright, \
               NO, \
               1, \
               1 \
               ) ) \
        return 0; \
}

#define ART_EXTERNAL_APP_NO_INPUT_FILES_APPLICATION_STARTUP_WITH_SYNOPSIS( \
    __name, \
    __desc, \
    __ldesc, \
    __usage \
    ) \
{ \
    if ( ! art_print_banner_and_process_standard_commandline_options( \
               argc, \
               argv, \
               art_gv, \
               __name, \
               __desc, \
               __ldesc, \
               __usage, \
               NULL, \
               NO, \
               1, \
               1 \
               ) ) \
        return 0; \
}

BOOL art_application_runs_in_quiet_mode(
        ART_GV  * art_gv
        );

#define  ART_APPLICATION_RUNS_IN_QUIET_MODE \
    art_application_runs_in_quiet_mode( art_gv )

//   ---- CUT HERE ----

//   These three functions belong somewhere in Foundation - to be moved...

void cptr_argumentlist_add_space_separated_args(
              ArList  * argument_list,
        const char    * argument_string
        );

void cptr_argumentlist_add_stringarray_with_prefix(
              ArList   * argument_list,
        const char    ** argument_stringarray,
        const char     * prefix
        );

void stringarray_from_cptr_argumentlist(
        char    *** argument_list_to_create,
        ArList    * cptr_argument_list
        );


// ===========================================================================
