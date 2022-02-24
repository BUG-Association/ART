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

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(ArpReporter)

#import "ART_Foundation_Datastructures.h"

@protocol ArpReporter

/* ---------------------------------------------------------------------------

    'beginAction'

        Start a new action consisting of a number of steps. The reporter
        is informed about each step by a single call to its 'printf'
        method. Depending on the verbosity of the reporter it can print
        each of the lines in these 'printf' calls, or it can start a
        progress indicator, that just prints the percentage of steps that
        have been successfully completed.

        The reporter keeps track of how many actions have been started and
        maintains an according indentation level.

        The first argument of the beginAction function specifies the number of
        steps which is the number of printf that will be called between the
        beginAction call and the corresponding endAction. This parameter serves
        only for the progress bar. If one do not know how many printf calls
        will occur, this step number can be set to 0, in which case no progress
        bar will be displayed. If there is nothing to print, the begin and end
        action functions can still be used as a timer.

        The second and remaining arguments are just text strings to be
        displayed.

--------------------------------------------------------------------------- */
- (void) beginActionWithProgressBar
        : (unsigned long) printfsUntilActionIsComplete
        : (const char *) format
        , ...
        ;

- (void) beginAction
        : (const char *) format
        , ...
        ;

- (void) beginTimedAction
        : (const char *) format
        , ...
        ;

- (void) beginSecondaryAction
        : (const char *) format
        , ...
        ;

- (void) failure
        : (const char *) format, ...
        ;
/* ---------------------------------------------------------------------------
    'endAction'
        This finishes an action and reports the time used up for the complete
        action. Optionally you can print another message when completing
        an action, or provide the duration of the action yourself. The latter
        feature is sometimes necessary for multithreaded applications where
        the main thread with the action on it sleeps while subthreads do the
        work, and consequently does not report the correct time.
--------------------------------------------------------------------------- */
- (void) endAction
        ;

- (void) endActionWithTime
        : (double) durationInSeconds
        ;

- (void) endAction
        : (const char *) format, ...
        ;

/* ---------------------------------------------------------------------------
    'printf'
        The standard method for printing messages. Indentation is based on
        the number of actions that have been started.
--------------------------------------------------------------------------- */
- (void) printf
        : (const char *) format, ...
        ;

/* ---------------------------------------------------------------------------
    'printfRaw'
        A method for printing raw messages without indentation.
--------------------------------------------------------------------------- */
- (void) printfRaw
        : (const char *) format, ...
        ;

- (void) modifyIndent
        : (int) delta
        ;

/* ---------------------------------------------------------------------------
    'consolePrintf'
        A method for printing only on the terminal (as opposed to a log file).
--------------------------------------------------------------------------- */
- (void) consolePrintf
        : (const char *) format, ...
        ;

/* ---------------------------------------------------------------------------
    'printProgress'
--------------------------------------------------------------------------- */
- (void) printProgress
        : (ArProgress *) progress
        : (unsigned long) new_value
        ;

@end


/* ===========================================================================
    'ArpFileReporter'
=========================================================================== */
@protocol ArpFileReporter

- (void) beginActionWithProgressBar
        : (unsigned long) steps
        : (const char *) string
        : (unsigned long) length
        ;

- (void) beginAction
        : (const char *) string
        : (unsigned long) length
        ;

- (void) beginTimedAction
        : (const char *) string
        : (unsigned long) length
        ;

- (void) beginSecondaryAction
        : (const char *) string
        : (unsigned long) length
        ;

- (void) endAction
        : (double) seconds
        : (const char *) string
        : (unsigned long) length
        ;

- (void) prints
        : (const char *) string
        ;

- (void) consolePrints
        : (const char *) string
        ;

- (void) printsRaw
        : (const char *) string
        ;

- (void) modifyIndent
        : (int) delta
        ;

@end

// ===========================================================================
