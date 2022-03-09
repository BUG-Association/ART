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

#define ART_MODULE_NAME     ART_EnvironmentVariables

#include "ART_EnvironmentVariables.h"
#include "ART_SystemDatatypes.h"
#include "ArString.h"
#include "ArStringArray.h"
#include "ART_PathAndDirectoryTools.h"
#include "ART_ErrorHandling.h"
#include "ArDataType.h"

 #include <string.h>

/* ---------------------------------------------------------------------------

    Notes on the return values supplied by this module
    ==================================================


    a) Image viewer program for command line apps
    ---------------------------------------------

    If the user supplies a value via the 'ART_VIEWER' environment variable,
    this is used. Otherwise a system-specific default is used instead.


    b) Various path lists, such as resource, library, include...
    ------------------------------------------------------------

    Most of these are assembled from several sources, some of them
    system-dependent, some of them valid for all platforms. This is done
    to ensure minimal duplication of data, and maximum configurability for
    various target platforms.


    c) Specialised derived paths, such as stub, gcc...
    --------------------------------------------------

    These are derived automatically, usually based on the contents of the
    execute and/or library paths


------------------------------------------------------------------------aw- */

typedef struct ART_EnvironmentVariables_GV
{
    ArStringArray   art_resource_paths;
    ArString        art_resource_pathlist;
    ArStringArray   art_library_paths;
    ArStringArray   art_include_paths;
    ArStringArray   art_execute_paths;
    ArStringArray   art_subprocess_envp;
#ifdef __APPLE__
    ArString        art_osx_isysroot;
#endif
    ArString        art_viewer;
    int             art_raw_workflow_format;
    int             art_end_result_image_format;
    int             art_end_result_tone_mapping;
    ArString        art_default_isr_string;
    ArDataType      art_default_isr;
    ArString        arm2art_sed_path;
    ArString        arm2art_compiler_path;
    ArString        arm2art_stub_path;
}
ART_EnvironmentVariables_GV;

#define ART_EV_GV                       art_gv->art_environmentvariables_gv
#define ART_EV_RESOURCE_PATHS           ART_EV_GV->art_resource_paths
#define ART_EV_RESOURCE_PATHLIST        ART_EV_GV->art_resource_pathlist
#define ART_EV_LIBRARY_PATHS            ART_EV_GV->art_library_paths
#define ART_EV_INCLUDE_PATHS            ART_EV_GV->art_include_paths
#define ART_EV_EXECUTE_PATHS            ART_EV_GV->art_execute_paths
#define ART_EV_SUBPROCESS_ENVP          ART_EV_GV->art_subprocess_envp
#ifdef __APPLE__
#define ART_EV_OSX_ISYSROOT             ART_EV_GV->art_osx_isysroot
#endif
#define ART_EV_VIEWER                   ART_EV_GV->art_viewer
#define ART_EV_RAW_WORKFLOW_FORMAT      ART_EV_GV->art_raw_workflow_format
#define ART_EV_END_RESULT_IMAGE_FORMAT  ART_EV_GV->art_end_result_image_format
#define ART_EV_END_RESULT_TONE_MAPPING  ART_EV_GV->art_end_result_tone_mapping
#define ART_EV_DEFAULT_ISR_STRING       ART_EV_GV->art_default_isr_string
#define ART_EV_DEFAULT_ISR              ART_EV_GV->art_default_isr
#define ART_EV_ARM2ART_SED_PATH         ART_EV_GV->arm2art_sed_path
#define ART_EV_ARM2ART_COMPILER_PATH    ART_EV_GV->arm2art_compiler_path
#define ART_EV_ARM2ART_STUB_PATH        ART_EV_GV->arm2art_stub_path


/* ---------------------------------------------------------------------------

    System-independent path lists and directory names
    =================================================

------------------------------------------------------------------------aw- */

//   Name for the ART resource directory

const char * art_resource_directory_name =
    "ART_Resources";

//   Subdirectories of the resource directory that are always added to the
//   include path

const char * art_general_resource_include_directories =
    "arm2art:"
    "arm2art/include:"
    "include:"
    "lib_include:"
    "SpectralData:"
    ".";

//   Directories that are always added to the include path

const char * art_general_include_directories =
    ".";


/* ---------------------------------------------------------------------------

    System-dependent path lists
    ===========================

    'art_system_possible_resource_locations'

    Locations in which ART will look for the resource directory.

    'art_system_library_directories'

    System-dependent library/framework search locations used during the
    linking stage of arm2art translation.

    'art_system_include_directories'

    System-specific include directories, i.e. the location for the
    standard system headers (not the ART library headers!).

    'art_system_execute_directories'

    Additional, system-specific execute directories needed for arm2art
    translation that are *not* in the standard system $PATH. Not needed
    on all platforms; OS X is one where this is needed, since gcc does
    not live in any of the standard directories there (at least for older
    versions of XCode).

    'art_system_resource_library_directories'

    Subdirectories of the resource directory that are also used during
    the arm2art linking step. The complete path of these dirs changes
    with the location of the resource directory, and the name also varies
    on a per-platform basis ("lib" on UNIX, "Frameworks" on OS X).

------------------------------------------------------------------------aw- */


#if defined( __linux__ ) || defined( __CYGWIN__) || defined( MINGW )
static const char * art_system_possible_resource_locations =
    "~:~/lib:/lib:/usr/lib:/usr/local/lib";
static const char * art_system_library_directories =
    "~/lib:/lib:/usr/lib:/usr/local/lib:/usr/GNUstep/System/Library/Libraries/";
static const char * art_system_include_directories =
    "/usr/include/:/usr/local/include:/usr/GNUstep/System/Library/Headers/";
static const char * art_system_execute_directories =
    "";
static const char * art_system_resource_library_directories =
    "lib:arm2art/lib";
#endif

#ifdef __APPLE__
static const char * art_system_possible_resource_locations =
    "~:~/Library:/Library";

//   Possible locations of -isysroot, in order of preference

static const char * art_osx_isysroot_directories =
    "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/"
    "Developer/SDKs/MacOSX.sdk:";

//   Library (and framework locations) that will be added to -F in
//   addition to what -isysroot says.

static const char * art_system_library_directories =
    "~/Library/Frameworks:/Library/Frameworks";

//   On OS X / XCode, system includes are covered by -isysroot, so only
//   additional paths have to be specified here.

static const char * art_system_include_directories =
    "~/Library/Frameworks/AdvancedRenderingToolkit.framework/Headers:"
    "/usr/local/include:/opt/local/include:/opt/homebrew/include";

//   Additional unusual execute paths needed to find gcc. This only applies
//   to XCode 4.2 and older, from 4.3 onwards gcc is installed in a normal
//   location.

static const char * art_system_execute_directories =
    "/Developer/usr/bin";

static const char * art_system_resource_library_directories =
    "Frameworks";
#endif

//#define ENVVAR_DEBUG

ART_MODULE_INITIALISATION_FUNCTION
(
    ART_EV_GV = ALLOC(ART_EnvironmentVariables_GV);

    //   All set to zero, lazy evaluation only if needed

    ART_EV_RESOURCE_PATHS          = 0;
    ART_EV_RESOURCE_PATHLIST       = 0;
    ART_EV_LIBRARY_PATHS           = 0;
    ART_EV_INCLUDE_PATHS           = 0;
    ART_EV_EXECUTE_PATHS           = 0;
    ART_EV_SUBPROCESS_ENVP         = 0;
#ifdef __APPLE__
    ART_EV_OSX_ISYSROOT            = 0;
#endif
    ART_EV_ARM2ART_SED_PATH        = 0;
    ART_EV_ARM2ART_COMPILER_PATH   = 0;
    ART_EV_ARM2ART_STUB_PATH       = 0;
    ART_EV_VIEWER                  = 0;
    ART_EV_RAW_WORKFLOW_FORMAT     = -1;
    ART_EV_END_RESULT_IMAGE_FORMAT = -1;
    ART_EV_END_RESULT_TONE_MAPPING = -1;
    ART_EV_DEFAULT_ISR_STRING      = 0;
    ART_EV_DEFAULT_ISR             = 0;
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    arstringarray_free( & ART_EV_RESOURCE_PATHS );
    arstringarray_free( & ART_EV_LIBRARY_PATHS );
    arstringarray_free( & ART_EV_INCLUDE_PATHS );
    arstringarray_free( & ART_EV_SUBPROCESS_ENVP );

#ifdef __APPLE__
    if ( ART_EV_OSX_ISYSROOT )
        FREE( ART_EV_OSX_ISYSROOT );
#endif

    if ( ART_EV_RESOURCE_PATHLIST )
        FREE( ART_EV_RESOURCE_PATHLIST );

    if ( ART_EV_ARM2ART_SED_PATH )
        FREE( ART_EV_ARM2ART_COMPILER_PATH );

    if ( ART_EV_ARM2ART_COMPILER_PATH )
        FREE( ART_EV_ARM2ART_COMPILER_PATH );

    if ( ART_EV_ARM2ART_STUB_PATH )
        FREE( ART_EV_ARM2ART_STUB_PATH );

    FREE( ART_EV_GV );
)

ArConstStringArray  art_ev_resource_paths(
        const ART_GV  * art_gv
        )
{
    if ( ! ART_EV_RESOURCE_PATHS )
    {
        ArStringArray  system_resource_path_stringarray;

        //   Create a resource directory path for all locations
        //   in the candidate list

        create_stringarray_from_two_spliced_pathlists(
            & system_resource_path_stringarray,
              art_system_possible_resource_locations,
              art_resource_directory_name
            );

        //   To avoid feature creep: make do with available functions,
        //   and convert this string array of possible locations to
        //   a pathlist.

        ArString  system_resource_pathlist;

        create_pathlist_from_stringarray(
            & system_resource_pathlist,
              (ArConstStringArray) system_resource_path_stringarray
            );

#ifdef ENVVAR_DEBUG
        debugprintf(
            "Resource path intermediate stringarray, len %d:\n",
            arstringarray_len( system_resource_path_stringarray )
            );

        arstringarray_debugprintf(system_resource_path_stringarray);
#endif

        arstringarray_free( & system_resource_path_stringarray );

#ifdef ENVVAR_DEBUG
        debugprintf(
            "Resource path intermediate pathlist\n'%s'\n",
            system_resource_pathlist
            );
#endif
        //   If the user supplied additional paths for resource directory
        //   locations, we place these in front of the list, otherwise
        //   we just use the path string created earlier.

        if ( getenv("ART_RESOURCE_PATH") )
            create_stringarray_from_pathlists(
                & ART_EV_RESOURCE_PATHS,
                  getenv("ART_RESOURCE_PATH"),
                  system_resource_pathlist,
                  NULL
                );
        else
            create_stringarray_from_pathlists(
                & ART_EV_RESOURCE_PATHS,
                  system_resource_pathlist,
                  NULL
                );

        FREE( system_resource_pathlist );

        create_pathlist_from_stringarray(
            & ART_EV_RESOURCE_PATHLIST,
              (ArConstStringArray) ART_EV_RESOURCE_PATHS
            );

#ifdef ENVVAR_DEBUG
        debugprintf(
            "Resource path pathlist\n'%s'\n",
            ART_EV_RESOURCE_PATHLIST
            );

        debugprintf(
            "Resource path stringarray, len %d:\n",
            arstringarray_len(ART_EV_RESOURCE_PATHS)
            );
        arstringarray_debugprintf(ART_EV_RESOURCE_PATHS);
#endif
    }

    //   The following should (at least in theory) never, ever happen,
    //   which is why we check for it... :-)

    if (    ! ART_EV_RESOURCE_PATHS
         || arstringarray_len(ART_EV_RESOURCE_PATHS) == 0 )
        ART_ERRORHANDLING_FATAL_ERROR(
            "unable to determine ART resource directories"
            );

    return (const char **)ART_EV_RESOURCE_PATHS;
}

ArConstStringArray  art_ev_library_paths(
        const ART_GV  * art_gv
        )
{
    if ( ! ART_EV_LIBRARY_PATHS )
    {
        ArStringArray  resource_library_path_stringarray;

        //   Create a resource directory path for all locations
        //   in the candidate list

        create_stringarray_from_two_spliced_pathlists(
            & resource_library_path_stringarray,
              ART_EV_RESOURCE_PATHLIST,
              art_system_resource_library_directories
            );

        //   To avoid feature creep: make do with available functions,
        //   and convert this string array of possible locations to
        //   a pathlist.

        ArString  resource_library_pathlist;

        create_pathlist_from_stringarray(
            & resource_library_pathlist,
              (ArConstStringArray) resource_library_path_stringarray
            );

        arstringarray_free( & resource_library_path_stringarray );

        //   If the user supplied additional paths for library/framework
        //   locations, we place these in front of the list, otherwise
        //   we just use the path string created earlier.

        if ( getenv("ART_LIBRARY_PATH") )
            create_stringarray_from_pathlists(
                & ART_EV_LIBRARY_PATHS,
                  getenv( "ART_LIBRARY_PATH" ),
                  resource_library_pathlist,
                  art_system_library_directories,
                  NULL
                );
        else
            create_stringarray_from_pathlists(
                & ART_EV_LIBRARY_PATHS,
                  resource_library_pathlist,
                  art_system_library_directories,
                  NULL
                );

        FREE( resource_library_pathlist );
#ifdef ENVVAR_DEBUG
        debugprintf("Library path stringarray:\n");
        arstringarray_debugprintf(ART_EV_LIBRARY_PATHS);
#endif
    }

    return (const char **)ART_EV_LIBRARY_PATHS;
}

ArConstStringArray  art_ev_include_paths(
        const ART_GV  * art_gv
        )
{
    if ( ! ART_EV_INCLUDE_PATHS )
    {
        ArStringArray  resource_include_path_stringarray;

        //   Create a resource directory path for all locations
        //   in the candidate list
        
        //   This call makes sure ART_EV_RESOURCE_PATHLIST is actually
        //   initialised before we use it further down
        
        ART_RESOURCE_PATHS;

        create_stringarray_from_two_spliced_pathlists(
            & resource_include_path_stringarray,
              ART_EV_RESOURCE_PATHLIST,
              art_general_resource_include_directories
            );

        //   To avoid feature creep: make do with available functions,
        //   and convert this string array of possible locations to
        //   a pathlist.

        ArString  resource_include_pathlist;

        create_pathlist_from_stringarray(
            & resource_include_pathlist,
              (ArConstStringArray) resource_include_path_stringarray
            );

        arstringarray_free( & resource_include_path_stringarray );

        //   If the user supplied additional paths for library/framework
        //   locations, we place these in front of the list, otherwise
        //   we just use the path string created earlier.

        if ( getenv("ART_INCLUDE_PATH") )
            create_stringarray_from_pathlists(
                & ART_EV_INCLUDE_PATHS,
                  getenv( "ART_INCLUDE_PATH" ),
                  resource_include_pathlist,
                  art_general_include_directories,
                  art_system_include_directories,
                  NULL
                );
        else
            create_stringarray_from_pathlists(
                & ART_EV_INCLUDE_PATHS,
                  resource_include_pathlist,
                  art_general_include_directories,
                  art_system_include_directories,
                  NULL
                );

        FREE( resource_include_pathlist );

#ifdef ENVVAR_DEBUG
        debugprintf("Include path stringarray:\n");
        arstringarray_debugprintf(ART_EV_INCLUDE_PATHS);
#endif
    }

    return (const char **)ART_EV_INCLUDE_PATHS;
}

ArConstStringArray  art_ev_execute_paths(
        const ART_GV  * art_gv
        )
{
    //   slightly tricky, since empty environment vars evaluate to
    //   zero, and would terminate the vararg list too early -
    //   but they have to go first in the list...

    if ( ! ART_EV_EXECUTE_PATHS )
    {
        if ( getenv( "ART_EXECUTE_PATH_PREFIX" ) )
            create_stringarray_from_pathlists(
                & ART_EV_EXECUTE_PATHS,
                  getenv( "ART_EXECUTE_PATH_PREFIX" ),
                  getenv( "PATH" ),
                  art_system_execute_directories,
                  NULL
                );
        else
        {
            if ( getenv( "PATH" ) )
                create_stringarray_from_pathlists(
                    & ART_EV_EXECUTE_PATHS,
                      getenv( "PATH" ),
                      art_system_execute_directories,
                      NULL
                    );
            else
                create_stringarray_from_pathlists(
                    & ART_EV_EXECUTE_PATHS,
                      art_system_execute_directories,
                      NULL
                    );
        }
    }

    return (const char **)ART_EV_EXECUTE_PATHS;
}

ArConstStringArray  art_ev_subprocess_envp(
        const ART_GV  * art_gv
        )
{
    if ( ! ART_EV_SUBPROCESS_ENVP )
    {
        char  * complete_execute_pathlist;

        create_pathlist_from_stringarray(
            & complete_execute_pathlist,
              ART_EXECUTE_PATHS
            );

        char  * path_envvar_assignment;

        create_envvar_assignment_string(
            & path_envvar_assignment,
              "PATH",
              complete_execute_pathlist
            );

        if ( complete_execute_pathlist )
            FREE( complete_execute_pathlist );

        ART_EV_SUBPROCESS_ENVP    = ALLOC_ARRAY( char *, 2 );
        ART_EV_SUBPROCESS_ENVP[0] = path_envvar_assignment;
        ART_EV_SUBPROCESS_ENVP[1] = NULL;
    }

    return (const char **)ART_EV_SUBPROCESS_ENVP;
}

#ifdef __APPLE__

ArConstString  art_ev_osx_isysroot(
        const ART_GV  * art_gv
        )
{
    if ( ! ART_EV_OSX_ISYSROOT )
    {
        create_string_from_first_existing_path_in_pathlist(
            & ART_EV_OSX_ISYSROOT,
              art_osx_isysroot_directories
            );
    }

    if ( ! ART_EV_OSX_ISYSROOT )
        ART_ERRORHANDLING_WARNING(
            "no valid path for -isysroot found"
            );

    return (ArConstString)ART_EV_OSX_ISYSROOT;
}

#endif

ArConstString  art_ev_viewer(
        const ART_GV  * art_gv
        )
{
    if ( ! ART_EV_VIEWER )
    {
        //   viewer program used to open the results of command-line apps

        ART_EV_VIEWER = getenv( "ART_VIEWER" );

        //   we only supply a default if none was specified

        if ( ! ART_EV_VIEWER )
        {
            //   The default viewer application depends on the platform
            //   ART is being compiled for

            #ifdef __APPLE__
            ART_EV_VIEWER = "open";
            #endif

            #if defined(__linux__) || defined(__CYGWIN__)
            ART_EV_VIEWER = "gnome-open";
            #endif
        }
    }

    return (ArConstString) ART_EV_VIEWER;
}

int  art_ev_raw_workflow_format(
        const ART_GV  * art_gv
        )
{
    if ( ART_EV_RAW_WORKFLOW_FORMAT == -1 )
    {
        //   text specification made by the user

        ArString  user_choice = getenv( "ART_RAW_WORKFLOW" );
        
        if ( user_choice )
        {
            if ( strcmp( user_choice, "ARTRAW" ) == 0 )
                ART_EV_RAW_WORKFLOW_FORMAT = ART_RAW_WORKFLOW_FORMAT_NATIVE;
            if ( strcmp( user_choice, "artraw" ) == 0 )
                ART_EV_RAW_WORKFLOW_FORMAT = ART_RAW_WORKFLOW_FORMAT_NATIVE;
                
            if ( strcmp( user_choice, "EXR" ) == 0 )
                ART_EV_RAW_WORKFLOW_FORMAT = ART_RAW_WORKFLOW_FORMAT_OPENEXR;
            if ( strcmp( user_choice, "exr" ) == 0 )
                ART_EV_RAW_WORKFLOW_FORMAT = ART_RAW_WORKFLOW_FORMAT_OPENEXR;
            if ( strcmp( user_choice, "OPENEXR" ) == 0 )
                ART_EV_RAW_WORKFLOW_FORMAT = ART_RAW_WORKFLOW_FORMAT_OPENEXR;
            if ( strcmp( user_choice, "OpenEXR" ) == 0 )
                ART_EV_RAW_WORKFLOW_FORMAT = ART_RAW_WORKFLOW_FORMAT_OPENEXR;
            if ( strcmp( user_choice, "openexr" ) == 0 )
                ART_EV_RAW_WORKFLOW_FORMAT = ART_RAW_WORKFLOW_FORMAT_OPENEXR;
        }

        //   we only default to ARTRAW if the user string didn't make sense, or none was specified

        if ( ART_EV_RAW_WORKFLOW_FORMAT == -1 )
        {
            ART_EV_RAW_WORKFLOW_FORMAT = ART_RAW_WORKFLOW_FORMAT_NATIVE;
        }
    }

    return ART_EV_RAW_WORKFLOW_FORMAT;
}

int  art_ev_end_result_image_format(
        const ART_GV  * art_gv
        )
{
    if ( ART_EV_END_RESULT_IMAGE_FORMAT == -1 )
    {
        //   text specification made by the user

        ArString  user_choice = getenv( "ART_END_RESULT_IMAGE_FORMAT" );
        
        if ( user_choice )
        {
            if ( strcmp( user_choice, "TIFF" ) == 0 )
                ART_EV_END_RESULT_IMAGE_FORMAT = ART_END_RESULT_IMAGE_FORMAT_TIFF;
            if ( strcmp( user_choice, "Tiff" ) == 0 )
                ART_EV_END_RESULT_IMAGE_FORMAT = ART_END_RESULT_IMAGE_FORMAT_TIFF;
            if ( strcmp( user_choice, "tiff" ) == 0 )
                ART_EV_END_RESULT_IMAGE_FORMAT = ART_END_RESULT_IMAGE_FORMAT_TIFF;
                
            if ( strcmp( user_choice, "OPENEXR" ) == 0 )
                ART_EV_END_RESULT_IMAGE_FORMAT = ART_END_RESULT_IMAGE_FORMAT_OPENEXR;
            if ( strcmp( user_choice, "OpenEXR" ) == 0 )
                ART_EV_END_RESULT_IMAGE_FORMAT = ART_END_RESULT_IMAGE_FORMAT_OPENEXR;
            if ( strcmp( user_choice, "openexr" ) == 0 )
                ART_EV_END_RESULT_IMAGE_FORMAT = ART_END_RESULT_IMAGE_FORMAT_OPENEXR;
                
            if ( strcmp( user_choice, "SPECTRALOPENEXR" ) == 0 )
                ART_EV_END_RESULT_IMAGE_FORMAT = ART_END_RESULT_IMAGE_FORMAT_SPECTRAL_OPENEXR;
            if ( strcmp( user_choice, "SpectralOpenEXR" ) == 0 )
                ART_EV_END_RESULT_IMAGE_FORMAT = ART_END_RESULT_IMAGE_FORMAT_SPECTRAL_OPENEXR;
            if ( strcmp( user_choice, "spectralopenexr" ) == 0 )
                ART_EV_END_RESULT_IMAGE_FORMAT = ART_END_RESULT_IMAGE_FORMAT_SPECTRAL_OPENEXR;
        }

        //   we only default to EXR if the user string didn't make sense, or none was specified

        if ( ART_EV_END_RESULT_IMAGE_FORMAT == -1 )
        {
            ART_EV_END_RESULT_IMAGE_FORMAT = ART_END_RESULT_IMAGE_FORMAT_OPENEXR;
        }
    }

    return ART_EV_END_RESULT_IMAGE_FORMAT;
}

int  art_ev_end_result_tone_mapping(
        const ART_GV  * art_gv
        )
{
    if ( ART_EV_END_RESULT_TONE_MAPPING == -1 )
    {
        //   text specification made by the user

        ArString  user_choice = getenv( "ART_END_RESULT_TONE_MAPPING" );
        
        if ( user_choice )
        {
            if ( strcmp( user_choice, "YES" ) == 0 )
                ART_EV_END_RESULT_TONE_MAPPING = 1;
            if ( strcmp( user_choice, "Yes" ) == 0 )
                ART_EV_END_RESULT_TONE_MAPPING = 1;
            if ( strcmp( user_choice, "yes" ) == 0 )
                ART_EV_END_RESULT_TONE_MAPPING = 1;
                
            if ( strcmp( user_choice, "NO" ) == 0 )
                ART_EV_END_RESULT_TONE_MAPPING = 0;
            if ( strcmp( user_choice, "No" ) == 0 )
                ART_EV_END_RESULT_TONE_MAPPING = 0;
            if ( strcmp( user_choice, "no" ) == 0 )
                ART_EV_END_RESULT_TONE_MAPPING = 0;
                
            if (   ART_END_RESULT_IMAGE_FORMAT_IS_SPECTRAL_OPENEXR
                && ART_EV_END_RESULT_TONE_MAPPING == 1 )
            {
                ART_ERRORHANDLING_WARNING(
                    "overriding environment variable ART_END_RESULT_TONE_MAPPING - "
                    "valuer reverted to NO, as default target image is set to "
                    "spectral OpenEXR"
                    );
                    
                ART_EV_END_RESULT_TONE_MAPPING = 0;
            }
        }

        //   we only default to "no" if the user string didn't make sense,
        //   or none was specified

        if ( ART_EV_END_RESULT_TONE_MAPPING == -1 )
        {
            ART_EV_END_RESULT_TONE_MAPPING = 0;
        }
    }

    return ART_EV_END_RESULT_TONE_MAPPING;
}

unsigned int  art_default_isr(
        const ART_GV  * art_gv
        )
{
    if ( ! ART_EV_DEFAULT_ISR_STRING )
    {
        ART_EV_DEFAULT_ISR_STRING = getenv( "ART_DEFAULT_ISR" );

        //   we only supply a default if none was specified

        if (   ! ART_EV_DEFAULT_ISR_STRING
            || strlen(ART_EV_DEFAULT_ISR_STRING) == 0 )
        {
            ART_EV_DEFAULT_ISR = ardt_spectrum8;
        }
        else
        {
            ART_EV_DEFAULT_ISR =
                ardatatype_from_string(ART_EV_DEFAULT_ISR_STRING);

            if ( ART_EV_DEFAULT_ISR == ardt_unknown )
                ART_ERRORHANDLING_FATAL_ERROR(
                    "unsuitable string '%s' specified for ART_DEFAULT_ISR"
                    "envvar. Use 's8v', 's11e', 's18v' or 's46e'.",
                    ART_EV_DEFAULT_ISR_STRING
                    );
        }
    }

    return (unsigned int) ART_EV_DEFAULT_ISR;
}

static ArConstString arm2art_sed_executable_name = "sed";

ArConstString  art_ev_arm2art_sed_path(
        const ART_GV  * art_gv
        )
{
    if ( ! ART_EV_ARM2ART_SED_PATH )
    {
        int  file_found =
            full_path_for_filename(
                & ART_EV_ARM2ART_SED_PATH,
                  arm2art_sed_executable_name,
                  ART_EXECUTE_PATHS
                );

        if ( ! file_found )
            ART_ERRORHANDLING_FATAL_ERROR(
                "'sed' executable not found"
                );
    }

    return (ArConstString) ART_EV_ARM2ART_SED_PATH;
}

static ArConstString arm2art_compiler_executable_name = "gcc";

ArConstString  art_ev_arm2art_compiler_path(
        const ART_GV  * art_gv
        )
{
    if ( ! ART_EV_ARM2ART_COMPILER_PATH )
    {
        int  file_found =
            full_path_for_filename(
                & ART_EV_ARM2ART_COMPILER_PATH,
                  arm2art_compiler_executable_name,
                  ART_EXECUTE_PATHS
                );

        if ( ! file_found )
            ART_ERRORHANDLING_FATAL_ERROR(
                "compiler executable not found"
                );
    }

    return (ArConstString) ART_EV_ARM2ART_COMPILER_PATH;
}

static ArConstString arm2art_stub_filename = "ART_TranslationStub.m";

ArConstString  art_ev_arm2art_stub_path(
        const ART_GV  * art_gv
        )
{
    if ( ! ART_EV_ARM2ART_STUB_PATH )
    {
        int  file_found =
            full_path_for_filename(
                & ART_EV_ARM2ART_STUB_PATH,
                  arm2art_stub_filename,
                  ART_INCLUDE_PATHS
                );

        if ( ! file_found )
            ART_ERRORHANDLING_FATAL_ERROR(
                ".arm -> .art translation stub file not found"
                );
    }

    return (ArConstString) ART_EV_ARM2ART_STUB_PATH;
}


/* ======================================================================== */
