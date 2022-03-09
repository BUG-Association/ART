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

/* ---------------------------------------------------------------------------

        Definition of the ART .arm file format
        =======================================

        .arm is a special Objective-C file that describes an ART scene,
        or at least parts of it.
        See "ART for Newbies" and other documents for descriptions.

--------------------------------------------------------------------------- */

#define ART_MODULE_NAME     ArfArm

#import "ArfArm.h"

#import <sys/types.h>
#import <sys/wait.h>
#import <unistd.h>
#import <stdio.h>
#import <errno.h>
#include <wordexp.h>

#import "ArcObjCCoder.h"

#import "ExecutionEnvironment.h"
#import "ApplicationSupport.h"

#define ARM_EXT        "arm"
#define ART_EXT        "art"
#define ARB_EXT        "arb"

static const char * arfarm_short_class_name =
                    "ART scene";
static const char * arfarm_long_class_name =
            "Advanced Rendering Toolkit scene file format";
const char * arfarm_exts[] =
                    { ARM_EXT, 0 };

ART_MODULE_INITIALISATION_FUNCTION
(
    [ ArfArm registerWithFileProbe
        :   art_gv
        ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#include <sys/stat.h>


#ifdef __APPLE__

    //   Part 0 - the fixed args before the stub filename

static ArConstString  fixed_gcc_arguments_string_part_0 =
        "clang -x objective-c";

    //   Part 1 - the remaining fixed args after the stub filename

    //   previously used option: -mmacosx-version-min=12.0
    
static ArConstString  fixed_gcc_arguments_string_part_1 =
        "-std=gnu99 -DART_4_OPENSTEP "
        "-framework Cocoa -framework AdvancedRenderingToolkit "
        ;

static ArConstString  libdir_prefix = "-F";

#endif

#if defined(__linux__) || defined(__CYGWIN__)

    //   Part 0 - the fixed args before the stub filename

static ArConstString  fixed_gcc_arguments_string_part_0 =
        "gcc -x objective-c";

    //   Part 1 - the remaining fixed args after the stub filename

static ArConstString  fixed_gcc_arguments_string_part_1 =
        "-std=gnu99 -DART_4_GNUSTEP -D_ART_WITHOUT_JPEGLIB_ "
        "-lAdvancedRenderingToolkit -lgnustep-base -ltiff -llcms2 -lobjc -lm "
        "-I/usr/GNUstep/Local/Library/Headers -I/usr/include/GNUstep "
        "-I/usr/GNUstep/System/Library/Headers -D_NATIVE_OBJC_EXCEPTIONS=1 "
        ;

static ArConstString  libdir_prefix = "-L";

#endif

BOOL translation_is_needed(
        ART_GV    * art_gv,
        BOOL        force,
        ArString    outputfile_arg,
        time_t      source_modification_time
        )
{
    struct stat  st;

    //   We check whether the .ar[t|b] file already exists

    if ( stat( outputfile_arg, & st ) != -1 )
    {
        //   And if it does, we compare the creation time
        //   of source and destination.

        time_t  result_modification_time = st.st_mtime;

        if (    result_modification_time > source_modification_time
             && ! force )
        {
            //   If the .ar[t|b] file is newer than the .arm file,
            //   we might not have to do any work - except that the
            //   .ar[t|b] file might be the result of a translation
            //   process done with an older ART version. In which
            //   case a re-translation is the safe course of action.

            //   The only reliable way to check this is to do the same
            //   kind of check with which ArfNative probes files as
            //   to whether they are parseable by one of the native
            //   coders. Anything except a perfect match is suspect,
            //   and leads to a re-translation.

            ArcFile  * outputfile_temp =
                [ ArcFile new
                    :   art_gv
                    :   outputfile_arg
                    ];

            [ outputfile_temp open: arfile_read ];

            ArFiletypeMatch matchOfExistingOutputFile =
                [ ArcObjCReadingCoder matchWithStream
                    :   outputfile_temp
                    ];

            RELEASE_OBJECT( outputfile_temp );

            if ( matchOfExistingOutputFile == arfiletypematch_exact )
            {
                //   If we have a perfect match (same ART version, and
                //   all), we do not need to translate.
                
                return NO;
            }
        }
    }

    return YES;
}

void translate_file(
        ART_GV      * art_gv,
        const char  * input_filename,
        BOOL          binary,
        BOOL          force,
        BOOL          retainExecutable,
        double      * durationOfTranslation
        )
{
    if ( binary )
        ART_ERRORHANDLING_WARNING(
            "binary translation currently unsupported"
            );

    ArTime  beginTime, endTime;

    artime_now( & beginTime );

    struct stat  st;

    if ( stat( input_filename, & st ) == -1 )
        ART_ERRORHANDLING_FATAL_ERROR(
            "source file '%s' not found"
            ,   input_filename
            );

    time_t  source_modification_time = st.st_mtime;

    if ( ! filename_has_extension( input_filename, ARM_EXT ) )
        ART_ERRORHANDLING_FATAL_ERROR(
            "source file '%s' does not have extension '%s'"
            ,   input_filename
            ,   ARM_EXT
            );

    ArString  basic_path;

    arstring_p_copy_without_extension_p(
          input_filename,
        & basic_path
        );

    ArString  basic_filename;

    arstring_p_copy_filename_s(
          basic_path,
        & basic_filename
        );

    ArString  outputfile_arg;

    asprintf(
        & outputfile_arg,
          "%s.%s",
          basic_path,
          ART_EXT
          );

    //   If the user supplied command line #defines for arm2art translation,
    //   it is reasonably safe to assume they want re-translation
    
    if ( ART_DEFINES_FOR_ARM2ART_WERE_SUPPLIED_BY_USER )
    {
        force = TRUE;
    }

    //   Check if translation is needed in the first place.
    
    if ( ! translation_is_needed(
                art_gv, force,
                outputfile_arg,
                source_modification_time
              ) )
    {
        //   If we have a perfect match (same ART version, and
        //   all), we can safely leave

        *durationOfTranslation = 0.0;

        FREE_ARRAY(basic_path);
        FREE_ARRAY(basic_filename);
        FREE_ARRAY(outputfile_arg);

        return;
    }
    
    //   We first assemble the argument list handed to the gcc subprocess as
    //   a dynamic list of strings, and build the ArStringArray array needed
    //   by execve() from this afterwards. Theoretically, it would be possible
    //   (and arguably much faster) to assemble the argument list by
    //   directly placing these strings in a large ArStringArray - something
    //   like char * argv[256]. However, doing it that way makes it
    //   practically certain that someone, somewhere will eventually
    //   find a reason to feed this thing 257 arguments...
    //   (a.k.a. the good old 0-1-n rule)

    //   Also, since parsing of strings with whitespace in them is involved
    //   in this, explicitly counting the arguments first, then allocating
    //   a suitably sized ArStringArray, and directly writing to it, very
    //   probably means more hassle than doing it via a list.

    ArList  gcc_cptr_argument_list = ARLIST_EMPTY;

    cptr_argumentlist_add_space_separated_args(
        & gcc_cptr_argument_list,
          fixed_gcc_arguments_string_part_0
        );

    arlist_add_cptr(
        & gcc_cptr_argument_list,
          (char *) ARM2ART_STUB_PATH
        );

    cptr_argumentlist_add_space_separated_args(
        & gcc_cptr_argument_list,
          fixed_gcc_arguments_string_part_1
        );

#ifdef __APPLE__
    arlist_add_cptr(
        & gcc_cptr_argument_list,
          "-isysroot"
        );

    arlist_add_cptr(
        & gcc_cptr_argument_list,
          (char *) ART_OSX_ISYSROOT
        );
#endif

    if ( ART_DEFINES_FOR_ARM2ART_WERE_SUPPLIED_BY_USER )
    {
        art_application_add_arm2art_defines_to_list(
              art_gv,
            & gcc_cptr_argument_list
            );
    }
    
    //   Two macro arguments that are dependent on the scene filename

    char  * armfile_arg;

    asprintf(
        & armfile_arg,
          "-DARM_FILE=\"%s.arm\"",
          basic_path
          );

    cptr_argumentlist_add_space_separated_args(
        & gcc_cptr_argument_list,
          armfile_arg
        );

    FREE(armfile_arg);

    char  * create_call_arg;

    asprintf(
        & create_call_arg,
          "-DARN_CREATE_CALL=create_%s",
          basic_filename
          );

    cptr_argumentlist_add_space_separated_args(
        & gcc_cptr_argument_list,
          create_call_arg
        );

    FREE(create_call_arg);

    arlist_add_cptr(
        & gcc_cptr_argument_list,
          "-o"
        );

    arlist_add_cptr(
        & gcc_cptr_argument_list,
          basic_filename
        );

    //   The following two together yield "-o <executable name>"

    //   Finally, add the library/framework search directories, and the
    //   include dirs.

    cptr_argumentlist_add_stringarray_with_prefix(
        & gcc_cptr_argument_list,
          ART_INCLUDE_PATHS,
          "-I"
        );

    cptr_argumentlist_add_stringarray_with_prefix(
        & gcc_cptr_argument_list,
          ART_LIBRARY_PATHS,
          libdir_prefix
        );

#ifdef __APPLE__
    cptr_argumentlist_add_stringarray_with_prefix(
        & gcc_cptr_argument_list,
          ART_LIBRARY_PATHS,
          "-Wl,-rpath,"
        );
#endif

    char  ** gcc_argument_list;

    stringarray_from_cptr_argumentlist(
        & gcc_argument_list,
        & gcc_cptr_argument_list
        );

    //  --->   Step 1: make sure the main function has the right name   <---
    
    //  Basically, we go over the main .arm file, and make sure that
    //  the string inside the 'ARM_MAIN_FUNCTION()' macro matches
    //  the filename (as it has to). By doing this, we make .arm files
    //  robust against command line re-naming.

    ArList  sed_cptr_argument_list = ARLIST_EMPTY;

    arlist_add_cptr(
        & sed_cptr_argument_list,
          "sed"
        );

    //  Theoretically, we do not need a backup file. However,
    //  OSX has a special snowflake version of 'sed' which for
    //  some reason does not accept "-i ''" as empty parameter
    //  (tried lots of escape sequences as well). So in the end
    //  it proved easier to just go with the flow, create a backup,
    //  and remove it afterwards.

    arlist_add_cptr(
        & sed_cptr_argument_list,
          "-i.bak"
        );

    char  * sed_argument1;

    asprintf(
        & sed_argument1,
          "/ARM_MAIN_FUNCTION/ s/ARM_MAIN_FUNCTION(.*)/ARM_MAIN_FUNCTION(%s)/",
          basic_filename
        );

    arlist_add_cptr(
        & sed_cptr_argument_list,
          sed_argument1
        );

    char  * sed_argument2;

    asprintf(
        & sed_argument2,
          "%s.arm",
          basic_path
        );

    arlist_add_cptr(
        & sed_cptr_argument_list,
          sed_argument2
        );

    char  ** sed_argument_list;

    stringarray_from_cptr_argumentlist(
        & sed_argument_list,
        & sed_cptr_argument_list
        );

//    Uncomment in case the 'sed' invocation is giving you headaches

//    debugprintf("\nsed path: %s\n",ARM2ART_SED_PATH)
//    arstringarray_debugprintf(sed_argument_list);
//    debugprintf("\n")

    int   subprocessResult;
    pid_t subprocessPID = fork();

    if ( subprocessPID == 0 ) // child process
    {
        subprocessResult =
            execve(
                ARM2ART_SED_PATH,
                sed_argument_list,
                (char * const *)ART_SUBPROCESS_ENVP
                );

        if ( subprocessResult == -1 )
            ART_ERRORHANDLING_FATAL_ERROR(
                "arm2art sed pass failed with error message '%s'"
                ,   strerror(errno)
                );
    }
    else
    {
        if ( subprocessPID == -1 )
            ART_ERRORHANDLING_FATAL_ERROR(
                "arm2art sed fork() failed with error message '%s'"
                ,   strerror(errno)
                );
        else // main process: wait until sed is complete
            waitpid(
                  subprocessPID,
                & subprocessResult,
                  0
                );
    }

    //   removing the sed in-place backup - see above
    
    char  * sed_backup_filename;

    asprintf(
        & sed_backup_filename,
          "%s.arm.bak",
          basic_path
        );

    remove( sed_backup_filename );
    
    FREE_ARRAY(sed_backup_filename);

//   --->   Step 2: compiling the stub   <---

//   Uncomment the following line to see what the actual argument list that
//   is being passed to gcc looks like.

//    arstringarray_debugprintf(gcc_argument_list);

    subprocessPID = fork();

    if ( subprocessPID == 0 ) // child process
    {
        subprocessResult =
            execve(
                ARM2ART_COMPILER_PATH,
                gcc_argument_list,
                (char * const *)ART_SUBPROCESS_ENVP
                );

        if ( subprocessResult == -1 )
            ART_ERRORHANDLING_FATAL_ERROR(
                "arm2art compile call failed with error message '%s'"
                ,   strerror(errno)
                );
    }
    else
    {
        if ( subprocessPID == -1 )
            ART_ERRORHANDLING_FATAL_ERROR(
                "arm2art compile fork() failed with error message '%s'"
                ,   strerror(errno)
                );
        else // main process: wait until compile is complete
            waitpid(
                  subprocessPID,
                & subprocessResult,
                  0
                );

        if ( stat( basic_filename, & st ) == -1 )
            ART_ERRORHANDLING_FATAL_ERROR(
                "executable '%s' not found - compile failed"
                ,   basic_filename
                );
    }

    //   --->   Step 3: using the executable   <---

    //   Now we actually run the executable we built: the only purpose
    //   of this executable is to generate the desired .art file.
    
    subprocessPID = fork();

    if ( subprocessPID == 0 ) // child process
    {
        int subprocess2Result =
            execl(
                basic_filename,
                basic_filename,
                outputfile_arg,
                NULL
                );

        if ( subprocess2Result == -1 )
            ART_ERRORHANDLING_FATAL_ERROR(
                "arm2art executable call failed with error message '%s'"
                ,   strerror(errno)
                );
    }
    else
    {
        if ( subprocessPID == -1 )
            ART_ERRORHANDLING_FATAL_ERROR(
                "arm2art executable fork() failed with error message '%s'"
                ,   strerror(errno)
                );
        else // main process: wait until execution is complete
            waitpid(
                  subprocessPID,
                & subprocessResult,
                  0
                );

        //   After both steps (compile & execution) are done, we double-check
        //   whether translation is still necessary. If it is, something
        //   went wrong, regardless.
        
        if ( translation_is_needed(
                  art_gv,
                  force,
                  outputfile_arg,
                  source_modification_time
                )
             && ! force )
        {
            ART_ERRORHANDLING_FATAL_ERROR(
                "arm2art translation failed, invocation was '%s %s'",
                basic_filename,
                outputfile_arg
                );
        }
    }

    //  -->   Step 4: cleanup   <---

    if ( ! retainExecutable )
        remove( basic_filename );

    FREE_ARRAY(basic_path);
    FREE_ARRAY(basic_filename);
    FREE_ARRAY(outputfile_arg);
    FREE_ARRAY(sed_argument1);
    FREE_ARRAY(sed_argument2);
    arstringarray_free( & sed_argument_list );
    arstringarray_free( & gcc_argument_list );

    artime_now( & endTime );

    *durationOfTranslation =
        artime_seconds( & endTime ) - artime_seconds( & beginTime );
}

@implementation ArfArm

ARPFILE_DEFAULT_IMPLEMENTATION(ArfArm, arfiletypecapabilites_read )
ARPPARSER_AUXLIARY_NODE_DEFAULT_IMPLEMENTATION

+ (const char **) extensions
{
    return arfarm_exts;
}

- (const char*) shortClassName
{
    return arfarm_short_class_name;
}

- (const char*) longClassName
{
    return arfarm_long_class_name;
}

+ (ArFiletypeMatch) matchWithStream
    : (ArcObject <ArpStream> *) stream
{
    // As .arm files come without magic string, we must assume it's valid.
    return arfiletypematch_exact;
}

#define REPORTER        art_global_reporter( art_gv )
#define USE_BINARY_IO   art_native_files_use_binary_io( art_gv )
#define FORCE_ARMART   art_force_ar2mart_translation( art_gv )

// take file, compile with art, initialize ArfNativeObjectiveC child

- (id) initWithFile
        : (ArcFile *) newFile
{
    self = [ super init ];
    
    if ( self )
    {
        native = NULL;

        [ REPORTER printf
            :   "File: %s\n"
            ,   [ newFile name ]
            ];

        [ REPORTER beginAction
                :   "translating .arm->.%s %s"
                ,   ( USE_BINARY_IO ? "arb":"art")
                ,   ( FORCE_ARMART ? ""   :"if necessary")
                ];

        double  durationOfTranslation;

        translate_file(
              art_gv,
              [ newFile name ],
              USE_BINARY_IO,
              FORCE_ARMART,
              NO,
            & durationOfTranslation
            );

        [ REPORTER endActionWithTime
            :   durationOfTranslation
            ];

        ArString  artFilename;

        if ( USE_BINARY_IO )
        {
            arstring_pss_copy_swap_extension_p(
                  [ newFile name ],
                  ARM_EXT,
                  ARB_EXT,
                & artFilename
                );

            native =
                [ ALLOC_OBJECT(ArfNativeBinary) initWithFile
                    :   [ ArcFile new
                            :   art_gv
                            :   artFilename
                            ]
                    ];

            if ( ! native )
                ART_ERRORHANDLING_FATAL_ERROR(
                    "could not create ArfNativeBinary child object"
                    );
        }
        else
        {
            arstring_pss_copy_swap_extension_p(
                  [ newFile name ],
                  ARM_EXT,
                  ART_EXT,
                & artFilename
                );

            native =
                [ ALLOC_OBJECT(ArfNativeObjectiveC) initWithFile
                    :   [ ArcFile new
                            :   art_gv
                            :   artFilename
                            ]
                    ];

            if ( ! native )
                ART_ERRORHANDLING_FATAL_ERROR(
                    "could not create ArfNativeObjectiveC child object"
                    );
        }

        FREE_ARRAY( artFilename );
    }
    return self;
}

- (void) dealloc
{
    RELEASE_OBJECT(native);

    [ super dealloc ];
}


- (void) parseFile
        : (ArNode **) objectPtr
{
    [ native parseFile: objectPtr ];
}

- (void) parseFileGetExternals
        : (ArNode **) objectPtr
        : (ArList *) externals
{
    [ native parseFileGetExternals: objectPtr : externals ];
}

- (void) parseStream
        : (ArNode **) objectPtr
        : (ArcObject <ArpStream> *) stream
{
    [native parseStream :objectPtr :stream ];
}

@end

// ===========================================================================
