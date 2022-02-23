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

ART_MODULE_INTERFACE(ArcReporter)

/*---------------------------------------------------------------------------
   How do reporters work ?

         The reporter is an object that keeps track of printf calls in the code, and
         display them according to the verbosity level used at runtime (-v option).
         If the -v option is turned on, the long version of the reporter will be used
         (ArcFileReporter) and one line per printf call will be displayed.
         If the -v option is NOT used, the short version of the reporter will be used
         (ArcShortFileReporter) and one line per action will be displayed, together
         with a progress bar under certain conditions.

         An "action" enables one to group a serie of printf calls, for example, to
         display information at each iteration of a loop. An action is created by a
         call to beginAction, and is terminated by a call to endAction. Inside a
         reporter, several actions may run at the same time. In that case, they are
         imbricated and an indent is added to the display of the line, according to
         the imbrication level. The beginAction and endAction functions behave like
         the push and pop functions in a stack.
----------------------------------------------------------------------cl--*/

#import "ArcObject.h"
#import "ArpReporter.h"

#define ARPREPORTER_INVALID_DURATION -1.0

@class ArcFileReporter;

/* ===========================================================================
    'ArcReporter'
=========================================================================== */
@interface ArcReporter
        : ArcObject
        < ArpReporter >
{
    ArcFileReporter * fileReporterArray[2];
    int numberOfClocks;
    ArClock clockArray[32];
    int failure;
    int numberOfFailures;
}

- (id) init
        : (ArcFileReporter *) newFileReporter
        ;

- (id) init
        : (ArcFileReporter *) newFileReporter0
        : (ArcFileReporter *) newFileReporter1
        ;

@end

#define ARPREPORTER_TEST_FUNCTION(_rep,_msg,_fun) \
        do { \
            [_rep beginAction :1 :(_msg ? _msg : #_fun)]; \
            test_##_fun(_rep); \
            [_rep endAction]; \
        } \
        while (0)

#define ARPREPORTER_TEST_CODE(_rep,_msg,_code) \
        do { \
            [_rep beginAction :1 :_msg]; \
            _code; \
            [_rep endAction]; \
        } \
        while (0)


/* ------------------------------------------------------------------------------
    'ArcFileReporter'
    This object enables the display of the long version of the command, that is,
    one line per printf call.
---------------------------------------------------------------------------cl--*/
@interface ArcFileReporter
        : ArcObject
        < ArpFileReporter >
{
    FILE * file;
    int numberOfActions;
    int indent;
}

- (id) init
        : (FILE *) newFile
        ;

@end

/* ---------------------------------------------------------------------------
    'ArcShortFileReporter'
    This object is used in the short version display, that is, one line per
    action together with a progress bar, under certain conditions (see
    beginAction for more information, in ArpReporter.h)
-------------------------------------------------------------------------cl--*/
@interface ArcShortFileReporter
        : ArcFileReporter
        < ArpFileReporter >
{
    long steps[42]; // static size -> bad, bad style...
    unsigned long count;
    ArProgress progress;
}

@end


// ===========================================================================
