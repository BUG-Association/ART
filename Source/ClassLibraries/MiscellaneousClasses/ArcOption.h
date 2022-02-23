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

#ifndef _ARCOPTION_H_
#define _ARCOPTION_H_

#include "ART_Foundation.h"

ART_MODULE_INTERFACE(ArcOption)

#import "ArcObject.h"


/* ---------------------------------------------------------------------------
    'ERROR_ARCOPTION_...'
        Errors returned by '[ArcOption parse :argc :argv];'.
--------------------------------------------------------------------------- */
#define ERROR_ARCOPTION_S_UNKNOWN \
            "unknown option '%s'"

#define ERROR_ARCOPTION_S_SPECIFIED_TWICE \
            "option '%s' specified more than once"

#define ERROR_ARCOPTION_S_MISSING_PARAMETER \
            "option '%s' is missing a parameter"

#define ERROR_ARCOPTION_S_WRONG_PARAMETER \
            "option '%s' was supplied with wrong parameter data"

#define ERROR_ARCOPTION_S_FLAG_OPTION_HAS_PARAMETER \
            "flag option '%s' has a parameter"


/* ===========================================================================
    'ArpOption'
=========================================================================== */
@protocol ArpOption

/* ---------------------------------------------------------------------------
    'cStringValue'
        Retrieve the string value that was specified on the command line or
        supplied as a default parameter.
--------------------------------------------------------------------------- */
- (const char *) cStringValue
        ;

@end

/* ===========================================================================
    'ArcOption'
=========================================================================== */

@interface ArcOption
        : ArcObject
        < ArpOption >
{

@public

    BOOL  specified;                    // Status variable during parsing.

    BOOL  silent;

    const char * shortName;             // The name that needs to be specified
                                        //  after a single dash (-shortName).

    const char * longName;              // The name that needs to be specified
                                        //  after a double dash (--longName).

    const char * explanation;           // The string that will be printed
                                        //  for this option if the method
                                        //  'printUsage' is called.

    ArcOption * next;                   // The options are stored in a singly
                                        //  linked list.  A hashtable would
                                        //  be a better datastructure for
                                        //  this.

    int noWrite;                        // If this is set to 1, the option is
                                        //  not written out to an option file.
}

/* ---------------------------------------------------------------------------
    'parse'
        Finds all options in the supplied argument vector and removes them.
        The number of remaining arguments is returned.
--------------------------------------------------------------------------- */
+ (int) parse
        : (ART_GV *) art_gv
        : (int) argc
        : (char **) argv
        ;


+ (void) setUsageLine
        : (ART_GV *) art_gv
        : (const char *) newUsageLine
        ;

+ (void) setSynopsisLine
        : (ART_GV *) art_gv
        : (const char *) newSynopsisLine
        ;

+ (void) fprintShortUsageAndExit
        : (ART_GV *) art_gv
        : (FILE *) file
        ;

+ (void) fprintUsageAndExit
        : (ART_GV *) art_gv
        : (FILE *) file
        ;

+ (unsigned int) numberOfDeclaredOptions
        : (ART_GV *) art_gv
        ;

- (void) flagAsSpecified
        ;

- (BOOL) hasBeenSpecified
        ;

- (void) setNoWrite
        ;

- (id) init
        : (const char *) newLongName
        : (const char *) newShortName
        : (const char *) newExplanation
        ;

- (void) removeFromUsageScreen
        ;

- (BOOL) silent
        ;

- (BOOL) hasDefaultValueSet
        ;

- (void) parseArgument
        : (const char *) argument
        ;
@end

/* ===========================================================================
    'ArcOptionFlag'
=========================================================================== */

@interface ArcOptionFlag
        : ArcOption
        < ArpOption >
{
}

@end

/* ===========================================================================
    'ArcOptionCategoryTitle'
=========================================================================== */

@interface ArcOptionCategoryTitle
        : ArcOption
        < ArpOption >
{
}

- (id) init
        : (const char *) newDescription
        ;

@end

/* ===========================================================================
    'ArcOptionParameter'
=========================================================================== */
@interface ArcOptionParameter
        : ArcOption
{
@protected
    const char * parameterName;

    BOOL  parameterHasBeenSet;
    BOOL  defaultValueHasBeenSet;
}

// @private

- (id) init
        : (const char *) newLongName
        : (const char *) newShortName
        : (const char *) newParameterName
        : (const char *) newExplanation
        ;

- (void) setCStringValue
        : (const char *) par
        ;

- (const char *) parameterName
        ;

@end

/* ===========================================================================
    'ArcOptionString'
=========================================================================== */
@interface ArcOptionString
        : ArcOptionParameter
        < ArpOption >
{
@protected
    const char * cStringValue;          // Pointer to the string that was
                                        //  specified on the command line, or
                                        //  supplied as default parameter.
}

- (id) init
        : (const char *) newLongName
        : (const char *) newShortName
        : (const char *) newParameterName
        : (const char *) newExplanation
        ;

- (ArcOptionString *) withDefaultCStringValue
        : (const char *) newCStringValue
        ;

@end

/* ===========================================================================
    'ArcOptionInteger'
=========================================================================== */
@interface ArcOptionInteger
        : ArcOptionString
{
@protected
    int   integerValue;
    int   defaultValue;
}

- (id) init
        : (const char *) newLongName
        : (const char *) newShortName
        : (const char *) newParameterName
        : (const char *) newExplanation
        ;

- (ArcOptionInteger *) withDefaultIntegerValue
        : (int) newIntegerValue
        ;

/* ---------------------------------------------------------------------------
    'integerValue'
        Retrieve the integer value that was specified on the command line or
        supplied as a default parameter.
--------------------------------------------------------------------------- */
- (int) value
        ;

@end

/* ===========================================================================
    'ArcOptionDouble'
=========================================================================== */
@interface ArcOptionDouble
        : ArcOptionString
{
@protected
    double doubleValue;
}

- (id) init
        : (const char *) newLongName
        : (const char *) newShortName
        : (const char *) newParameterName
        : (const char *) newExplanation
        ;

- (ArcOptionDouble *) withDefaultDoubleValue
        : (double) newDoubleValue
        ;

/* ---------------------------------------------------------------------------
    'doubleValue'
        Retrieve the 'double' value that was specified on the command line or
        supplied as a default parameter.
--------------------------------------------------------------------------- */
- (double) value
        ;

@end

void  arcoption_set_insertion_point_for_new_options(
    ART_GV     * art_gv,
    ArcOption  * option_to_insert_before
    );

void  arcoption_remove_empty_option_categories(
    ART_GV  * art_gv
    );

#endif // _ARCOPTION_H_

// ===========================================================================
