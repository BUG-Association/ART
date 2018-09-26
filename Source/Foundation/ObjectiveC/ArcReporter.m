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

#define ART_MODULE_NAME     ArcReporter

#import "ART_Foundation_Datastructures.h"
#import "ArcReporter.h"
#import "ArpReporter.h"

#import "ApplicationSupport.h"

#import "ART_Foundation_Math.h" /* M_MAX */
#import "ctype.h" /* isupper, toupper */
#include <unistd.h>

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

void fputspaces(int n, FILE * file)
{
    static const char * spacesString =
            "                                        "
            "                                        ";
    fputs(spacesString + 80 - M_MAX(0,n), file);
}

@implementation ArcReporter

- init
        : (ArcFileReporter *) newFileReporter
{
    fileReporterArray[0] = RETAIN_OBJECT(newFileReporter);
    fileReporterArray[1] = NULL;
    return self;
}

- init
        : (ArcFileReporter *) newFileReporter0
        : (ArcFileReporter *) newFileReporter1
{
    fileReporterArray[0] = RETAIN_OBJECT(newFileReporter0);
    fileReporterArray[1] = RETAIN_OBJECT(newFileReporter1);
    return self;
}

- (void) dealloc
{
    if ( fileReporterArray[0] )
        RELEASE_OBJECT( fileReporterArray[0] );

    if ( fileReporterArray[1] )
        RELEASE_OBJECT( fileReporterArray[1] );

    [ super dealloc ];
}


- (void) beginActionWithProgressBar
        : (unsigned long) printfsUntilActionIsComplete
        : (const char *) format
        , ...
{
    char buffer[1024];
    unsigned long length;
    va_list argPtr;
    va_start(argPtr, format);
    vsprintf(buffer,format,argPtr);
    va_end(argPtr);
    length = strlen(buffer);

    [ fileReporterArray[0] beginActionWithProgressBar
        :   printfsUntilActionIsComplete
        :   buffer
        :   length
        ];

    [ fileReporterArray[1] beginActionWithProgressBar
        :   printfsUntilActionIsComplete
        :   buffer
        :   length
        ];

    arclock_now(&clockArray[numberOfClocks]);
    ++numberOfClocks;
}

- (void) beginAction
        : (const char *) format
        , ...
{
    char buffer[1024];
    unsigned long length;
    va_list argPtr;
    va_start(argPtr, format);
    vsprintf(buffer,format,argPtr);
    va_end(argPtr);
    length = strlen(buffer);

    [ fileReporterArray[0] beginAction
        :   buffer
        :   length
        ];

    [ fileReporterArray[1] beginAction
        :   buffer
        :   length
        ];

    arclock_now(&clockArray[numberOfClocks]);
    ++numberOfClocks;
}

- (void) beginTimedAction
        : (const char *) format
        , ...
{
    char buffer[1024];
    unsigned long length;
    va_list argPtr;
    va_start(argPtr, format);
    vsprintf(buffer,format,argPtr);
    va_end(argPtr);
    length = strlen(buffer);

    [ fileReporterArray[0] beginTimedAction
        :   buffer
        :   length
        ];

    [ fileReporterArray[1] beginTimedAction
        :   buffer
        :   length
        ];

    arclock_now(&clockArray[numberOfClocks]);
    ++numberOfClocks;
}

- (void) beginSecondaryAction
        : (const char *) format
        , ...
{
    char buffer[1024];
    unsigned long length;
    va_list argPtr;
    va_start(argPtr, format);
    vsprintf(buffer,format,argPtr);
    va_end(argPtr);
    length = strlen(buffer);

    [ fileReporterArray[0] beginSecondaryAction
        :   buffer
        :   length
        ];

    [ fileReporterArray[1] beginSecondaryAction
        :   buffer
        :   length
        ];

    arclock_now(&clockArray[numberOfClocks]);
    ++numberOfClocks;
}

- (void) failure
        : (const char *) format, ...
{
    char buffer[1024];
    va_list argPtr;
    va_start(argPtr, format);
    vsprintf(buffer,format,argPtr);
    va_end(argPtr);
    failure = 1;

    [fileReporterArray[0] prints :buffer];
    [fileReporterArray[1] prints :buffer];
}

- (void) endAction
{
    [ self endActionWithTime: ARPREPORTER_INVALID_DURATION ];
}

- (void) endActionWithTime
        : (double) durationInSeconds
{
    ArClock end;
    double secs;

    if ( durationInSeconds == ARPREPORTER_INVALID_DURATION )
    {
        arclock_now(&end);
        --numberOfClocks;
        secs =   arclock_seconds( & end )
               - arclock_seconds( & clockArray[numberOfClocks] );
    }
    else
        secs = durationInSeconds;

    if (secs < 0.0) secs = 0.0;
    if (failure) { secs = -1.0; failure = 0; ++numberOfFailures; }

    [fileReporterArray[0] endAction :secs :NULL :0];
    [fileReporterArray[1] endAction :secs :NULL :0];
}

- (void) endAction
        : (const char *) format, ...
{
    char buffer[1024];
    unsigned long length;
    va_list argPtr;
    ArClock end;
    double secs;

    arclock_now(&end);
    --numberOfClocks;
    secs = arclock_seconds(&end)-arclock_seconds(&clockArray[numberOfClocks]);
    va_start(argPtr, format);
    vsprintf(buffer,format,argPtr);
    va_end(argPtr);
    length = strlen(buffer);

    if (secs < 0.0) secs = 0.0;
    if (failure) { secs = -1.0; failure = 0; ++numberOfFailures; }

    [fileReporterArray[0] endAction :secs :buffer :length];
    [fileReporterArray[1] endAction :secs :buffer :length];
}

- (void) printf
        : (const char *) format, ...
{
    char buffer[1024];
    va_list argPtr;
    va_start(argPtr, format);
    vsprintf(buffer,format,argPtr);
    va_end(argPtr);

    [fileReporterArray[0] prints :buffer];
    [fileReporterArray[1] prints :buffer];
}

- (void) printfRaw
        : (const char *) format, ...
{
    char buffer[1024];
    va_list argPtr;
    va_start(argPtr, format);
    vsprintf(buffer,format,argPtr);
    va_end(argPtr);

    [fileReporterArray[0] printsRaw :buffer];
    [fileReporterArray[1] printsRaw :buffer];
}

- (void) consolePrintf
        : (const char *) format, ...
{
    char buffer[1024];
    va_list argPtr;
    va_start(argPtr, format);
    vsprintf(buffer,format,argPtr);
    va_end(argPtr);

    [fileReporterArray[0] consolePrints :buffer];
    [fileReporterArray[1] consolePrints :buffer];
}

- (void) printProgress
        : (ArProgress *) progress
        : (unsigned long) new_value
{
    char buffer[32];
    arprogress_sprint(progress, new_value, buffer);
    [fileReporterArray[0] printsRaw :buffer];
    [fileReporterArray[1] printsRaw :buffer];
}

- (void) modifyIndent
        : (int) delta
{
    [ fileReporterArray[0] modifyIndent :delta ];
    [ fileReporterArray[1] modifyIndent :delta ];
}

@end

@implementation ArcFileReporter

- init
        : (FILE *) newFile
{
    file = newFile;
    numberOfActions = 0;
    return self;
}

- (void) beginActionWithProgressBar
        : (unsigned long) steps
        : (const char *) string
        : (unsigned long) length
{
    if ( ( file == stdout && art_is_running_as_foreground_process() )
         || file != stdout )
    {
        char buffer[1024];
        if (numberOfActions == 0)
        {
            unsigned long i;
            BOOL nochange = NO;
            for (i = 0; i < length; i++)
            {
                int ch = string[i];
                if (ch == '"') nochange = ~nochange;
                buffer[i] = (nochange || isupper(ch)) ? ch : toupper(ch);
            }
            buffer[length] = 0;
            string = buffer;
        }
        else if (indent > 0)
            { fputs(":\n", file); indent = 0; }

        indent = length;
        fputspaces(4 * numberOfActions, file);
        fputs(string, file);
        fputs(" ", file);
        fflush(file);
        ++numberOfActions;
    }
}

- (void) beginAction
        : (const char *) string
        : (unsigned long) length
{
    if ( ( file == stdout && art_is_running_as_foreground_process() )
         || file != stdout )
    {
        char buffer[1024];
        if (numberOfActions == 0)
        {
            unsigned long i;
            BOOL nochange = NO;
            for (i = 0; i < length; i++)
            {
                int ch = string[i];
                if (ch == '"') nochange = ~nochange;
                buffer[i] = (nochange || isupper(ch)) ? ch : toupper(ch);
            }
            buffer[length] = 0;
            string = buffer;
        }
        else if (indent > 0)
            { fputs(":\n", file); indent = 0; }

        indent = length;
        fputspaces(4 * numberOfActions, file);
        fputs(string, file);
        fputs(" ", file);
        fflush(file);
        ++numberOfActions;
    }
}

- (void) beginTimedAction
        : (const char *) string
        : (unsigned long) length
{
    if ( ( file == stdout && art_is_running_as_foreground_process() )
         || file != stdout )
    {
        char buffer[1024];
        if (numberOfActions == 0)
        {
            unsigned long i;
            BOOL nochange = NO;
            for (i = 0; i < length; i++)
            {
                int ch = string[i];
                if (ch == '"') nochange = ~nochange;
                buffer[i] = (nochange || isupper(ch)) ? ch : toupper(ch);
            }
            buffer[length] = 0;
            string = buffer;
        }
        else if (indent > 0)
            { fputs(":\n", file); indent = 0; }

        indent = length;
        fputspaces(4 * numberOfActions, file);
        fputs(string, file);
        fputs(" ", file);
        fflush(file);
        ++numberOfActions;
    }
}

- (void) beginSecondaryAction
        : (const char *) string
        : (unsigned long) length
{
    if ( ( file == stdout && art_is_running_as_foreground_process() )
         || file != stdout )
    {
        char buffer[1024];
        if (numberOfActions == 0)
        {
            unsigned long i;
            BOOL nochange = NO;
            for (i = 0; i < length; i++)
            {
                int ch = string[i];
                if (ch == '"') nochange = ~nochange;
                buffer[i] = (nochange || isupper(ch)) ? ch : toupper(ch);
            }
            buffer[length] = 0;
            string = buffer;
        }
        else if (indent > 0)
            { fputs(":\n", file); indent = 0; }

        indent = length;
        fputspaces(4 * numberOfActions, file);
        fputs(string, file);
        fputs(" ", file);
        fflush(file);
        ++numberOfActions;
    }
}

- (void) modifyIndent
        : (int) delta
{
    indent += delta;
}

- (void) endAction
        : (double) seconds
        : (const char *) string
        : (unsigned long) length
{
    if ( ( file == stdout && art_is_running_as_foreground_process() )
         || file != stdout )
    {
        --numberOfActions;
        if (string)
        {
            fputs(string, file);
            indent += length;
        }
        if (indent)
        {
            fputspaces(61 - indent - 4 * numberOfActions,file);
            indent = 0;
            if (seconds >= 0.0)
                fprintf(file, "%10.2f sec\n", seconds);
            else
                fprintf(file, "!!! FAILED !!!\07\n");
        }
        else
        {
            if (numberOfActions == 0)
                fprintf(file, "\n");
        }
    }
}


- (void) prints
        : (const char *) string
{
    if ( ( file == stdout && art_is_running_as_foreground_process() )
         || file != stdout )
    {
        if (numberOfActions > 0 && indent > 0)
            { fputs(":\n", file); indent = 0; }
        fputspaces(4 * numberOfActions, file);
        fputs(string, file);
        fflush(file);
    }
}

- (void) printsRaw
        : (const char *) string
{
    if ( ( file == stdout && art_is_running_as_foreground_process() )
         || file != stdout )
    {
        if (string[0])
        {
            fputs(string, file);
            fflush(file);
        }
    }
}

- (void) consolePrints
        : (const char *) string
{
    if ( file == stdout && art_is_running_as_foreground_process() )
    {
        fputs( string, file );
    }
}

- (void) dealloc
{
#ifndef ART_MODULE_MANAGEMENT_DIAGNOSTIC_OUTPUT
    fclose(file);
#endif

    return [ super dealloc ];
}

@end

@implementation ArcShortFileReporter

- init
        : (FILE *) newFile
{
    file = newFile;
    numberOfActions = 0;
    return self;
}

- (void) beginActionWithProgressBar
        : (unsigned long) newSteps
        : (const char *) string
        : (unsigned long) length
{
    if ( ( file == stdout && art_is_running_as_foreground_process() )
         || file != stdout )
    {
    steps[numberOfActions] = newSteps;
    if (numberOfActions == 0)// && steps[numberOfActions] > 0)
    {
        indent = length;
        fputspaces(4 * numberOfActions, file);
        fputs(string, file);
        fputs(" ", file);
        if (steps[numberOfActions] > 1)
        {
            count = 0;
            progress = arprogress(steps[numberOfActions],2);
        }
        fflush(file);
    }
    ++numberOfActions;
    }
}

- (void) beginAction
        : (const char *) string
        : (unsigned long) length
{
    if ( ( file == stdout && art_is_running_as_foreground_process() )
         || file != stdout )
    {
    steps[numberOfActions] = 0;
    if (numberOfActions == 0)// && steps[numberOfActions] > 0)
    {
        indent = length;
        fputspaces(4 * numberOfActions, file);
        fputs(string, file);
        fputs(" ", file);
        if (steps[numberOfActions] > 1)
        {
            count = 0;
            progress = arprogress(steps[numberOfActions],2);
        }
        fflush(file);
    }
    ++numberOfActions;
    }
}

- (void) beginTimedAction
        : (const char *) string
        : (unsigned long) length
{
    if ( ( file == stdout && art_is_running_as_foreground_process() )
         || file != stdout )
    {
    steps[numberOfActions] = 1;
    if (numberOfActions == 0)// && steps[numberOfActions] > 0)
    {
        indent = length;
        fputspaces(4 * numberOfActions, file);
        fputs(string, file);
        fputs(" ", file);
        if (steps[numberOfActions] > 1)
        {
            count = 0;
            progress = arprogress(steps[numberOfActions],2);
        }
        fflush(file);
    }
    ++numberOfActions;
    }
}

- (void) beginSecondaryAction
        : (const char *) string
        : (unsigned long) length
{
    steps[numberOfActions] = -1;
    ++numberOfActions;
}

- (void) endAction
        : (double) seconds
        : (const char *) string
        : (unsigned long) length
{
    if ( ( file == stdout && art_is_running_as_foreground_process() )
         || file != stdout )
    {
    --numberOfActions;
    if ( numberOfActions == 0 )
    {
        if ( steps[numberOfActions] > 0 )
        {
            if (string)
            {
                fputs(string, file);
                indent += length;
            }
            fputspaces(61 - indent - 4 * numberOfActions,file);
            indent = 0;
            if (seconds >= 0.0)
                fprintf(file, "%10.2f sec\n", seconds);
            else
                fprintf(file, "!!! FAILED !!!\07\n");
        }
        else
            if ( steps[numberOfActions] != -1 )
                fprintf(file, "\n");
    }
    }
}

- (void) modifyIndent
        : (int) delta
{
    indent += delta;
}

- (void) prints
        : (const char *) string
{
    if ( ( file == stdout && art_is_running_as_foreground_process() )
         || file != stdout )
    {
        if (numberOfActions == 1 && steps[0] > 1)
            arprogress_fprint(&progress, count++, file);
    }
}

- (void) printsRaw
        : (const char *) string
{
    if ( ( file == stdout && art_is_running_as_foreground_process() )
         || file != stdout )
    {
        if (string[0])
            arprogress_fprint(&progress, count++, file);
    }
}

- (void) consolePrints
        : (const char *) string
{
    if ( file == stdout && art_is_running_as_foreground_process() )
    {
        fputs( string, file );
    }
}

- (void) dealloc
{
#ifndef ART_MODULE_MANAGEMENT_DIAGNOSTIC_OUTPUT
    fclose(file);
#endif

    [ super dealloc ];
}

@end

// ===========================================================================
