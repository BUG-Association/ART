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

#define ART_MODULE_NAME     ArcOption

#import "ART_Foundation_Datastructures.h"

#include "AdvancedRenderingToolkit.h"

#import "ApplicationSupport.h"

#import "ArcOption.h"
#import <ctype.h>

typedef struct ArcOption_GV
{
    ArcOption     * head;
    ArcOption     * insertionpoint;
    ArcOption     * tail;
    const char    * usageLine;
    const char    * synopsisLine;
    unsigned int    numberOfDeclaredOptions;
}
ArcOption_GV;


#define ARCOPTION_HEAD              art_gv->arcoption_gv->head
#define ARCOPTION_INSERTIONPOINT    art_gv->arcoption_gv->insertionpoint
#define ARCOPTION_TAIL              art_gv->arcoption_gv->tail
#define USAGE_LINE                  art_gv->arcoption_gv->usageLine
#define SYNOPSIS_LINE               art_gv->arcoption_gv->synopsisLine
#define NUMBER_OF_OPTIONS           art_gv->arcoption_gv->numberOfDeclaredOptions

ART_MODULE_INITIALISATION_FUNCTION
(
    ArcOption_GV  * arcoption_gv;

    arcoption_gv = ALLOC(ArcOption_GV);

    art_gv->arcoption_gv = arcoption_gv;

    ARCOPTION_HEAD              = NULL;
    ARCOPTION_INSERTIONPOINT    = NULL;
    ARCOPTION_TAIL              = NULL;
    USAGE_LINE                  = "";
    SYNOPSIS_LINE               = "";
    NUMBER_OF_OPTIONS           = 0;
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    FREE( art_gv->arcoption_gv );
)


void  arcoption_set_insertion_point_for_new_options(
    ART_GV     * art_gv,
    ArcOption  * option_to_insert_before
    )
{
    ARCOPTION_INSERTIONPOINT = option_to_insert_before;
}

void  arcoption_remove_empty_option_categories(
    ART_GV  * art_gv
    )
{
    ArcOption  * current = ARCOPTION_HEAD;

    //   Note: this function only removes "dead" option categories *after*
    //         the first one. Since the first category ("system options")
    //         will always contain at least the "-h" option, this should not
    //         be an issue.

    do
    {
        BOOL  removal = NO;

        if ( current && current->next )
        {
            if ( [ current->next isMemberOfClass: [ ArcOptionCategoryTitle class ] ] )
            {
                if (    current->next->next
                     && [ current->next->next isMemberOfClass:
                            [ ArcOptionCategoryTitle class ]
                            ] )
                {
                    current->next = current->next->next;

                    removal = YES;
                }

                if ( ! current->next->next )
                {
                    current->next = 0;

                    removal = YES;
                }
            }
        }

        if ( ! removal && current )
            current = current->next;
    }
    while ( current );
}

@implementation ArcOption

/* ---------------------------------------------------------------------------
    'findByLongName', 'findByShortName'
        Find an option in the dictionary based on its short/long Name.  If it
        is not found a nil-pointer is returned.
--------------------------------------------------------------------------- */

+ (ArcOption *) findByLongName
        : (ART_GV *) art_gv
        : (const char *) name
{
    ArcOption * option;

    for (option = ARCOPTION_HEAD; option != NULL; option = option->next)
        if (strcmp(name, option->longName) == 0)
            return option;
    return NULL;
}

+ (ArcOption *) findByShortName
        : (ART_GV *) art_gv
        : (const char *) name
{
    ArcOption * option;

    for (option = ARCOPTION_HEAD; option != NULL; option = option->next)
        if (strcmp(name, option->shortName) == 0)
            return option;
    return NULL;
}


/* ---------------------------------------------------------------------------
    'ArCMDLineInput'
    
    A small structure to hold command line user input for individual options: 
    one such struct is used for each pair of "-optionName" and "parameter" 
    argv component that the user supplies.
    
    The idea is to first go over the entire command line, find the matching
    pairs, and then have the options actually parse their supplied
    arguments in a second round. Only this way we can make sure that we are 
    both able to detect pathological input, as well as make sure default
    values for options are still honoured if no parameters are supplied.
------------------------------------------------------------------------aw- */

typedef struct ArCMDLineInput
{
    ArcOption  * optionInstance;  // the option instance we found for this
    char       * optionName;      // the name of the option (for error msg)
    char       * optionArgument;  // the argument it will parse (can be 0)
}
ArCMDLineInput;

//  An empty cmdline entry

#define ARCMDLI_EMPTY       ((ArCMDLineInput){NULL,0,0})

//  We define a dynarray data type for this struct

ARDYNARRAY_INTERFACE_FOR_ARTYPE(CMDLineInput, cmdli, c);
ARDYNARRAY_IMPLEMENTATION_FOR_ARTYPE(CMDLineInput, cmdli, c, ARCMDLI_EMPTY);

#define MAX_INPUTS  96

+ (int) parse
        : (ART_GV *) art_gv
        : (int) argc
        : (char **) argv
{
    if ( argc > MAX_INPUTS - 1 )
    {
        ART_ERRORHANDLING_FATAL_ERROR("too many command line arguments, raise MAX_INPUTS");
    }
    
    //   Initialised to a value that will likely suffice for most cases
    
    ArCMDLineInputDynArray  inputDynArray = arcmdlidynarray_init(MAX_INPUTS);
    
    for ( int i = 0; i < MAX_INPUTS; i++ )
    {
        arcmdlidynarray_push( & inputDynArray, ARCMDLI_EMPTY );
    }

    ArCMDLineInput  * currentInput = NULL;
    
    //   Initialised to -1 to account for the ++ operator whenever a new option
    //   is found
    
    int   currentInputIndex  = -1;
    BOOL  optionsHaveStarted = NO;
    int   remainingInputFiles = 0;
    
    for ( int i = 1; i < argc; i++ )
    {
        //   Option names start with a dash, but so do negative numbers. And
        //   we want to file those as arguments, not as command names.
        
        //   Potential problem: option short names cannot start with a number.
        //   Severity of this: zero (IMHO, that is)

        if (    argv[i][0] == '-'
             && argv[i][1] != '.' && argv[i][1] != '0' && argv[i][1] != '1'
             && argv[i][1] != '2' && argv[i][1] != '3' && argv[i][1] != '4'
             && argv[i][1] != '5' && argv[i][1] != '6' && argv[i][1] != '7'
             && argv[i][1] != '8' && argv[i][1] != '9'
           )
        {
            //   Command line arguments that start with 'D' are special
            //   snowflakes. These are assumed to be compiler #defines,
            //   gcc-style, are collected, and passed on to any arm2art
            //   sub-process that might be started later.
            
            if ( argv[i][1] == 'D' )
            {
                //   We just feed this to the global collection of such things
                
                art_application_add_to_arm2art_defines(
                      art_gv,
                      argv[i]
                    );
            }
            else
            {
                //   Finally, a normal option!
                
                optionsHaveStarted = YES;
                
                //   We found a new option, so we move forward in the input array
                
                currentInputIndex++;

                currentInput =
                    arcmdlidynarray_ptr_to_i( & inputDynArray, currentInputIndex );
                
                //   We remember the option name for potential later error messages
                
                currentInput->optionName = argv[i];
                currentInput->optionArgument = 0;
                
                //   Was the syntax "-optionName=parameter" used?
                
                char  * parameter = strchr(argv[i],'=');

                //   If so, put a zero in place of the '=', to terminate the
                //   string there - otherwise the option name search would get
                //   confused.
                
                if ( parameter )
                    *parameter++ = 0;

                //   All long options start with two dashes, all short ones with
                //   a single dash.
                
                if ( argv[i][1] == '-' )
                    currentInput->optionInstance =
                        [ ArcOption findByLongName
                            :   art_gv
                            :   (argv[i] + 2)
                            ];
                else
                    currentInput->optionInstance =
                        [ ArcOption findByShortName
                            :   art_gv
                            :   (argv[i] + 1)
                            ];
                
                //   Was an option found, for this name?
                
                if ( currentInput->optionInstance == NULL )
                {
                    ART_ERRORHANDLING_FATAL_ERROR(
                        ERROR_ARCOPTION_S_UNKNOWN
                        ,   argv[i]
                        );

                    return 0;
                }
                
                //   Has this option been specified already?
                
                if ( [ currentInput->optionInstance hasBeenSpecified ] )
                {
                    ART_ERRORHANDLING_FATAL_ERROR(
                        ERROR_ARCOPTION_S_SPECIFIED_TWICE
                        ,   argv[i]
                        );

                    return 0;
                }
                
                //   Mark this option instance as specified.
                
                [ currentInput->optionInstance flagAsSpecified ];
                
                //   If the input syntax '-optionName=parameter' was used,
                //   we remembered the start of the 'parameter' section of the
                //   string. And supply that as argument to the option now.
                
                if ( parameter )
                    currentInput->optionArgument = parameter;
            }
        }
        else
        {
            //   At the start of the command line, there might be an arbitrary
            //   number of input filenames we just ignore, for the purposes
            //   of option handing & parsing.
            
            if ( optionsHaveStarted )
            {
                currentInput->optionArgument = argv[i];
            }
            else
                remainingInputFiles++;
        }
    }

    for ( int i = 0; i <= currentInputIndex; i++ )
    {
        currentInput =
            arcmdlidynarray_ptr_to_i( & inputDynArray, i );
        
        //   Flag options by definition don't take input, while all others do.
        //   So they get different treatment.
        
        if ( [ currentInput->optionInstance isMemberOfClass: [ ArcOptionFlag class ] ])
        {
            //   With flag options, the only thing we check is if someone
            //   didn't supply an argument by mistake
            
            if ( currentInput->optionArgument != 0 )
                ART_ERRORHANDLING_FATAL_ERROR(
                    ERROR_ARCOPTION_S_FLAG_OPTION_HAS_PARAMETER
                    ,   currentInput->optionName
                    );
        }
        else
        {
            //   Was an argument supplied on the command line?
            
            if ( currentInput->optionArgument == 0 )
            {
                if ( ! [ currentInput->optionInstance hasDefaultValueSet ] )
                    ART_ERRORHANDLING_FATAL_ERROR(
                        ERROR_ARCOPTION_S_MISSING_PARAMETER
                        ,   currentInput->optionName
                        );
            }
            else
            {
                //   Parse the argument that was supplied, and complain if it
                //   is rejected by the option instance.

                [ currentInput->optionInstance
                    parseArgument
                    :   currentInput->optionArgument
                    ];
            }
        }
    }

    arcmdlidynarray_free_contents( & inputDynArray );

    return remainingInputFiles + 1;
}

- (void) flagAsSpecified
{
    specified = 1;
}

- (void) removeFromUsageScreen
{
    silent = YES;
}

- (BOOL) hasDefaultValueSet
{
    return NO;
}

- (BOOL) silent
{
    return silent;
}

+ (void) setUsageLine
        : (ART_GV *) art_gv
        : (const char *) newUsageLine
{
    if ( strlen(newUsageLine) > 0 )
        USAGE_LINE = newUsageLine;
}

+ (void) setSynopsisLine
        : (ART_GV *) art_gv
        : (const char *) newSynopsisLine
{
    if ( strlen(newSynopsisLine) > 0 )
        SYNOPSIS_LINE = newSynopsisLine;
}

- (const char *) explanation
{
    return explanation;
}

- (const char *) parameterName
{
    return "";
}

+ (void) fprintUsageAndExit
        : (ART_GV *) art_gv
        : (FILE *) file
{
    ArcOption * option;
    char optionLine[80];

    fprintf(file,"USAGE:  %s\n", USAGE_LINE);

    if ( strlen( SYNOPSIS_LINE ) > 0 )
        fprintf(file,"\nSYNOPSIS:\n%s\n", SYNOPSIS_LINE);

    for (option = ARCOPTION_HEAD; option != 0; option = option->next)
    {
        if ( [ option isMemberOfClass: [ ArcOptionCategoryTitle class ] ] )
        {
            sprintf(optionLine,"\n%s",
                    option->shortName);

            fprintf(file,"%s\n", optionLine);
        }
        else
        {
            if ( ! [ option silent ] )
            {
                sprintf( optionLine,
                    "-%-4s --%s %s"
                    ,   option->shortName
                    ,   option->longName
                    ,   [ option parameterName ]
                    );

                fprintf( file,
                    "  %-37s %s\n"
                    ,   optionLine
                    ,   [ option explanation ]
                    );
            }
        }
    }
    fprintf(file,"\n");
    AdvancedRenderingToolkit_library_shutdown( art_gv );
    exit (-1);
}

+ (void) fprintShortUsageAndExit
        : (ART_GV *) art_gv
        : (FILE *) file
{
    //   The "if ( NUMBER_OF_OPTIONS > 9 )" coditional has the following idea
    //   behind it: if only a few options have been defined, it does not make
    //   sense to offer the user a separate short usage screen; it is much more
    //   logical to just print the full usage screen in all circumstances for
    //   such applications.

    //   The number 9 does NOT mean that this cut-off value is actually 9; there
    //   are a number of "silent" options that are always included, so a value
    //   of 9 actually amounts to 4 visible options.

    if ( NUMBER_OF_OPTIONS > 9 )
    {
        fprintf(file,"USAGE:  %s\n\n", USAGE_LINE);
        fprintf(file,"For further information:\n");
        fprintf(file," -h    --help                          "
                     "full list of available options");
        fprintf(file,"\n\n");
        AdvancedRenderingToolkit_library_shutdown( art_gv );
        exit (-1);
    }
    else
        [ self fprintUsageAndExit
            :   art_gv
            :   file
            ];
}

+ (unsigned int) numberOfDeclaredOptions
        : (ART_GV *) art_gv
{
    return
        NUMBER_OF_OPTIONS;
}

- (id) init
        : (const char *) newLongName
        : (const char *) newShortName
        : (const char *) newExplanation
{
    self = [ super init ];

    if ( self )
    {
        specified = 0;
        shortName = newShortName;
        longName = newLongName;
        explanation = newExplanation;
        next = 0;
        noWrite = 0;

        if (      shortName[0] == 'D'
             && ! [ self isMemberOfClass: [ArcOptionCategoryTitle class ] ] )
        {
            ART_ERRORHANDLING_FATAL_ERROR(
                "option short names must not start with a capital D: '%s'"
                ,   shortName
                );
        }

        if ([ArcOption findByLongName: art_gv : longName])
        {
            ART_ERRORHANDLING_FATAL_ERROR(
                "option with long name '%s' already exists"
                ,   longName
                );
        }

        if ([ArcOption findByShortName: art_gv :shortName])
        {
            ART_ERRORHANDLING_FATAL_ERROR(
                "option with short name '%s' already exists"
                ,   shortName
                );
        }

        if ( ARCOPTION_INSERTIONPOINT )
        {
            self->next = ARCOPTION_INSERTIONPOINT->next;
            ARCOPTION_INSERTIONPOINT->next = self;
            ARCOPTION_INSERTIONPOINT = self;
        }
        else
        {
            if (ARCOPTION_TAIL)
                ARCOPTION_TAIL->next = self;
            else
                ARCOPTION_HEAD = self;
            ARCOPTION_TAIL = self;
        }

        NUMBER_OF_OPTIONS += 1;
    }
    
    return self;
}

- (BOOL) hasBeenSpecified
{
    return specified;
}

- (void) parseArgument
        : (const char *) argument
{
    ART__VIRTUAL_METHOD__EXIT_WITH_ERROR;
}

- (const char *) cStringValue
{
    return 0;
}

- (void) setNoWrite
{
    noWrite = 1;
}

@end

@implementation ArcOptionFlag

- (const char *) cStringValue
{
    if ( [ self hasBeenSpecified ] )
        return "yes";
    else
        return "no";
}

@end

@implementation ArcOptionCategoryTitle

- (id) init
        : (const char *) newDescription
{
    self =
        [ super init
            :   newDescription
            :   newDescription
            :   newDescription
            ];

    return self;
}

- (const char *) cStringValue
{
    if ( [ self hasBeenSpecified ] )
        return "yes";
    else
        return "no";
}

@end

@implementation ArcOptionParameter

- (id) init
        : (const char *) newLongName
        : (const char *) newShortName
        : (const char *) newParameterName
        : (const char *) newExplanation
{
    self =
        [ super init
            :   newLongName
            :   newShortName
            :   newExplanation
            ];
    
    if ( self )
    {
        parameterHasBeenSet = NO;
        parameterName = newParameterName;
    }
    
    return self;
}

- (BOOL) hasDefaultValueSet
{
    return defaultValueHasBeenSet;
}

- (void) flagAsSpecified
{
    [ super flagAsSpecified ];
    
    parameterHasBeenSet = NO;
}

- (void) parseArgument
        : (const char *) argument
{
    if ( ! parameterHasBeenSet )
    {
        [ self setCStringValue: argument ];
        parameterHasBeenSet = YES;
    }
}

- (void) setCStringValue
        : (const char *) newCStringValue
{
    ART_ERRORHANDLING_FATAL_ERROR( "virtual method called" );
}

- (const char *) parameterName
{
    return parameterName;
}

@end

@implementation ArcOptionString

- (id) init
        : (const char *) newLongName
        : (const char *) newShortName
        : (const char *) newParameterName
        : (const char *) newExplanation
{
    self =
        [ super init
            :   newLongName
            :   newShortName
            :   newParameterName
            :   newExplanation
            ];

    if ( self )
    {
        cStringValue = 0;
    }
    
    return self;
}

- (ArcOptionString *) withDefaultCStringValue
    : (const char *) newCStringValue
{
    cStringValue = newCStringValue;
    defaultValueHasBeenSet = TRUE;
    return self;
}

- (const char *) cStringValue
{
    return cStringValue;
}

- (void) setCStringValue
        : (const char *) newCStringValue
{
    cStringValue = newCStringValue;
}

@end

@implementation ArcOptionInteger

- (id) init
        : (const char *) newLongName
        : (const char *) newShortName
        : (const char *) newParameterName
        : (const char *) newExplanation
{
    self =
        [ super init
            :   newLongName
            :   newShortName
            :   newParameterName
            :   newExplanation
            ];
    
    if ( self )
    {
        integerValue = 0;
    }
    
    return self;
}

- (ArcOptionInteger *) withDefaultIntegerValue
        : (int) newIntegerValue
{
    defaultValue           = newIntegerValue;
    defaultValueHasBeenSet = TRUE;

    return self;
}

- (int) value
{
    if ( parameterHasBeenSet )
        return integerValue;
    else
    {
        if ( defaultValueHasBeenSet )
            return defaultValue;
    }

    ART_ERRORHANDLING_FATAL_ERROR(
        "integer option queried without default or command line invocation"
        );

    return 0;
}

- (int) integerValue
{
    return [ self value ];
}

- (void) setCStringValue
        : (const char *) newCStringValue
{
    cStringValue      = newCStringValue;
    integerValue      = atoi(cStringValue);
}

@end

@implementation ArcOptionDouble

- (id) init
        : (const char *) newLongName
        : (const char *) newShortName
        : (const char *) newParameterName
        : (const char *) newExplanation
{
    self =
        [ super init
            :   newLongName
            :   newShortName
            :   newParameterName
            :   newExplanation
            ];
    
    if ( self )
    {
        doubleValue = 0.0;
    }
    
    return self;
}

- (ArcOptionDouble *) withDefaultDoubleValue
        : (double) newDoubleValue
{
    doubleValue = newDoubleValue;
    return self;
}

- (double) value
{
    return doubleValue;
}

- (double) doubleValue
{
    return [ self value ];
}

- (void) setCStringValue
        : (const char *) newCStringValue
{
    cStringValue = newCStringValue;
    doubleValue = atof(cStringValue);
}

@end

// ===========================================================================

