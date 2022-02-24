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

#ifndef _ART_FOUNDATION_SYSTEM_ENVIRONMENT_VARIABLES_H_
#define _ART_FOUNDATION_SYSTEM_ENVIRONMENT_VARIABLES_H_

#include "ART_ModuleManagement.h"
#include "ArStringArray.h"

ART_MODULE_INTERFACE(ART_EnvironmentVariables)


/* ---------------------------------------------------------------------------

    'ART_EnvironmentVariables' module

    This module provides a way to set various platform-dependent configuration
    options, such as the ART resource search path, header and library resp.
    framework locations, or the external image viewer that will be used to
    display results of command-line applications.

    The strategy employed by this module varies: for some of the resources it
    provides, the system checks whether a corresponding environment variable
    has been set by the user. If a value has been provided, it uses that. But
    if it has not been set, it provides a reasonable, platform-dependent
    default value instead. For others, it generates a blend of the contents
    of the associated environment variable, and platform-specific system
    defaults.

    See the .c file for notes on what exactly these default values are,
    and what the strategy employed for each resource is. The environment
    variable names are usually the same as the all uppercase access
    macro used for the resource, minus the "S" (e.g. $ART_RESOURCE_PATH
    vs. ART_RESOURCE_PATHS as macro for the char** path array).

------------------------------------------------------------------------aw- */


/* ---------------------------------------------------------------------------

    $ART_RESOURCE_PATH  -  ART_RESOURCE_PATHS

    A null-terminated list of search locations for the resource directories
    of ART, where ICC profiles, spectral measurement data, headers, and the
    like can be found.

------------------------------------------------------------------------aw- */

ArConstStringArray  art_ev_resource_paths(
        const ART_GV  * art_gv
        );

#define ART_RESOURCE_PATHS     art_ev_resource_paths( art_gv )


/* ---------------------------------------------------------------------------

    $ART_LIBRARY_PATH  -  ART_LIBRARY_PATHS

    A null-terminated list of library directories used during ARM->ART file
    conversion, when an executable is compiled, linked an run in a subprocess.
    On OS X, this returns the default framework locations (!), on UNIX, the
    normal library locations.

------------------------------------------------------------------------aw- */

ArConstStringArray  art_ev_library_paths(
        const ART_GV  * art_gv
        );

#define ART_LIBRARY_PATHS      art_ev_library_paths( art_gv )


/* ---------------------------------------------------------------------------

    $ART_INCLUDE_PATH  -  ART_INCLUDE_PATHS

    A null-terminated list of header directories used during ARM->ART file
    conversion, when an executable is compiled, linked an run in a subprocess.
    On OS X, this returns, amongst other things, a location within the
    ART framework (! - the header dirs placed in there), and on UNIX, the
    normal include locations (under the assumption that ART is installed
    according to normal conventions).

------------------------------------------------------------------------aw- */

ArConstStringArray  art_ev_include_paths(
        const ART_GV  * art_gv
        );

#define ART_INCLUDE_PATHS      art_ev_include_paths( art_gv )


/* ---------------------------------------------------------------------------

    $ART_EXECUTE_PATH_PREFIX  -  ART_EXECUTE_PATHS

    A null-terminated list of directories used to find gcc and other
    executables when subprocesses that involve executables are started.
    This is made up of the standard $PATH *plus* the contents of this
    environment variable, which are placed at the front of the search path
    used for the subprocesses.

    Also, on some platforms, canonical additions are made, which are placed
    between the contents of $PATH, and the contents of the envvar. For
    instance, on OS X, the executable directories of XCode are also added to
    the path, since they are rarely part of shell environments.

    Note that this is normally not used directly, but only serves as basis
    for the ART_SUBPROCESS_ENVP resource.

------------------------------------------------------------------------aw- */

ArConstStringArray  art_ev_execute_paths(
        const ART_GV  * art_gv
        );

#define ART_EXECUTE_PATHS    art_ev_execute_paths( art_gv )


#ifdef __APPLE__

/* ---------------------------------------------------------------------------

    ART_OSX_ISYSROOT

    On Apple/Xcode systems only: which -isysroot to use during compilation.

------------------------------------------------------------------------aw- */

ArConstString  art_ev_osx_isysroot(
        const ART_GV  * art_gv
        );

#define ART_OSX_ISYSROOT     art_ev_osx_isysroot( art_gv )

#endif


/* ---------------------------------------------------------------------------

    ART_SUBPROCESS_ENVP

    The variable bindings passed to subprocesses that are started by ART
    during invocations of execle().

    The contents of this resource are not directly created from a single
    corresponding environment variable. However, the contents of $PATH are
    determined by ART_EXECUTABLE_PATHS.

------------------------------------------------------------------------aw- */

ArConstStringArray  art_ev_subprocess_envp(
        const ART_GV  * art_gv
        );

#define ART_SUBPROCESS_ENVP     art_ev_subprocess_envp( art_gv )


/* ---------------------------------------------------------------------------

    ART_VIEWER

    The executable used to display images created by command line utilities.

------------------------------------------------------------------------aw- */

ArConstString  art_ev_viewer(
        const ART_GV  * art_gv
        );

#define ART_VIEWER      art_ev_viewer( art_gv )


/* ---------------------------------------------------------------------------

    ART_RAW_WORKFLOW_FORMAT

    Controls the raw format used within a rendering process.
    
    By default, rendering yields ARTRAW images, which are then processed
    further to some displayable format like EXR or TIFF. However, starting
    with version 2.1, ART also offers the option to use spectral OpenEXR
    images throughout.
    
    Valid string values for the envvar are "ARTRAW" and "OpenEXR".
    
    --->  Default is "ARTRAW"  <---
    
    Note that this envvar only affects action sequences that use
    generic actions to specify workflow formats. If a specific type of
    raw image is referenced in an action sequence, setting this envvar will
    not alter the system behaviour.

------------------------------------------------------------------------aw- */

int  art_ev_raw_workflow_format(
        const ART_GV  * art_gv
        );

#define ART_RAW_WORKFLOW_FORMAT  art_ev_raw_workflow_format( art_gv )

#define ART_RAW_WORKFLOW_FORMAT_NATIVE         0
#define ART_RAW_WORKFLOW_FORMAT_OPENEXR        1

#define ART_RAW_WORKFLOW_FORMAT_IS_NATIVE      \
( ART_RAW_WORKFLOW_FORMAT == ART_RAW_WORKFLOW_FORMAT_NATIVE )
#define ART_RAW_WORKFLOW_FORMAT_IS_OPENEXR     \
( ART_RAW_WORKFLOW_FORMAT == ART_RAW_WORKFLOW_FORMAT_OPENEXR )


/* ---------------------------------------------------------------------------

    ART_END_RESULT_IMAGE_FORMAT

    Determines the default format of the final result of a rendering process.
    
    Valid string values for the envvar are:
    
    "TIFF": RGB LDR images

    "OpenEXR": RGB HDR images
    
    "SpectralOpenEXR": RGB + Spectral HDR images
    
    --->  Default is "OpenEXR"  <---
    
    The ART_END_RESULT_TONE_MAPPING envvar can be used to control
    whether the unmodified raw output of the rendering pass should be put in
    the result image, or whether a tone mapping step should be performed.
    
    Note that it rarely makes sense to write to TIFF without a tone
    mapping step.

    Also note that this envvar only affects action sequences that use
    generic format conversion and tone reproduction actions. Action
    sequences which explicitly reference specific image formats are not
    affected by this envvar!

------------------------------------------------------------------------aw- */

int  art_ev_end_result_image_format(
        const ART_GV  * art_gv
        );

#define ART_END_RESULT_IMAGE_FORMAT  \
    art_ev_end_result_image_format( art_gv )

#define ART_END_RESULT_IMAGE_FORMAT_TIFF                0
#define ART_END_RESULT_IMAGE_FORMAT_OPENEXR             1
#define ART_END_RESULT_IMAGE_FORMAT_SPECTRAL_OPENEXR    2

#define ART_END_RESULT_IMAGE_FORMAT_IS_TIFF     \
( ART_END_RESULT_IMAGE_FORMAT == \
  ART_END_RESULT_IMAGE_FORMAT_TIFF )
  
#define ART_END_RESULT_IMAGE_FORMAT_IS_OPENEXR  \
( ART_END_RESULT_IMAGE_FORMAT == \
  ART_END_RESULT_IMAGE_FORMAT_OPENEXR )

#define ART_END_RESULT_IMAGE_FORMAT_IS_SPECTRAL_OPENEXR  \
( ART_END_RESULT_IMAGE_FORMAT == \
  ART_END_RESULT_IMAGE_FORMAT_SPECTRAL_OPENEXR )


/* ---------------------------------------------------------------------------

    ART_END_RESULT_TONE_MAPPING

    Determines whether a tone mapping step is by default applied to the
    raw result of a rendering job which uses a default action sequence.
    
    Valid string values for the envvar are:
    
    "yes": a generic default tone mapping algorithm is applied

    "no" : no tone mapping, the raw contents are written straight out

    --->  Default is "no"  <---
    
    Please note the following:
    
    - it rarely makes sense to work with non tone mapped TIFF images.
    
    - if ART_END_RESULT_IMAGE_FORMAT is set to "SpectralOpenEXR",
      this envvar is always "no": if the user explicitly sets it to
      "yes" in this case, a warning is issued, and the variable is
      still set to "no". Reason being that tone mapping of spectral images
      is a conceptually dodgy idea in the first place.
      
    - this envvar only affects action sequences that use generic format
      conversion and tone reproduction actions. Action sequences which
      explicitly reference specific image formats are not affected by it.

------------------------------------------------------------------------aw- */

int  art_ev_end_result_tone_mapping(
        const ART_GV  * art_gv
        );

#define ART_END_RESULT_TONE_MAPPING  \
    art_ev_end_result_default_tone_mapping( art_gv )


/* ---------------------------------------------------------------------------

    ARM2ART_SED_PATH

    The path to the 'sed' executable used during .arm -> .art translations.
    The location of sed is determined only once, and re-used for subsequent
    translations.

------------------------------------------------------------------------aw- */

ArConstString  art_ev_arm2art_sed_path(
        const ART_GV  * art_gv
        );

#define ARM2ART_SED_PATH      art_ev_arm2art_sed_path( art_gv )


/* ---------------------------------------------------------------------------

    ARM2ART_COMPILER_PATH

    The path to the compiler used for .arm -> .art translations. The location
    of the compiler is determined only once, and re-used for subsequent
    translations.

------------------------------------------------------------------------aw- */

ArConstString  art_ev_arm2art_compiler_path(
        const ART_GV  * art_gv
        );

#define ARM2ART_COMPILER_PATH      art_ev_arm2art_compiler_path( art_gv )


/* ---------------------------------------------------------------------------

    ARM2ART_COMPILER_PATH

    The path to the compiler used for .arm -> .art translations. The location
    of the compiler is determined only once, and re-used for subsequent
    translations.

------------------------------------------------------------------------aw- */

ArConstString  art_ev_arm2art_compiler_path(
        const ART_GV  * art_gv
        );

#define ARM2ART_COMPILER_PATH      art_ev_arm2art_compiler_path( art_gv )


/* ---------------------------------------------------------------------------

    ARM2ART_STUB_PATH

    The path to the stub .m file used for .arm -> .art translations. The
    location of the file is determined only once, and re-used for subsequent
    translations.

------------------------------------------------------------------------aw- */

ArConstString  art_ev_arm2art_stub_path(
        const ART_GV  * art_gv
        );

#define ARM2ART_STUB_PATH       art_ev_arm2art_stub_path( art_gv )


/* ---------------------------------------------------------------------------

    ART_DEFAULT_ISR

    The default ISR used by ART. The environment variable should be set to
    a string which corresponds to the commandline flags used by 'artist'
    et al (minus the dash, of course), like so:
 
    export ART_DEFAULT_ISR = s11e
 
    or

    export ART_DEFAULT_ISR = rgb

    The function 'art_default_isr' returns an integer and not a string,
    though. The integer is meant to be cast to the enum 'ArDataType'
    by those functions which need to know about the default. We don't
    include 'ArDataType.h' here to avoid cyclical #includes.

------------------------------------------------------------------------aw- */

unsigned int  art_default_isr(
        const ART_GV  * art_gv
        );

#define ART_DEFAULT_ISR         art_default_isr( art_gv )


#endif /* _ART_FOUNDATION_SYSTEM_ENVIRONMENT_VARIABLES_H_ */
/* ======================================================================== */
