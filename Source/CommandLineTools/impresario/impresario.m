/* ===========================================================================

    Copyright (c) 1996-2021 The ART Development Team
    -------------------------------------------

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

#include "ARM_Foundation.h"
#import "AdvancedRenderingToolkit.h"
#include <stdlib.h>
#include "ArcObject.h"
#include "ArcMessageQueue.h"

#include <stdio.h> 
#include <unistd.h>
#include <signal.h>
#include <termios.h> 

#define INVALID_PID                -1
#define DEFAULT_REPEAT_FREQUENCY    15
#define LOCALHOST "127.0.0.1"
#define TEV_PORT 14158

//   Small function from stackoverflow

char art_getchar(void)
{
    struct termios oldt,newt;
    char ch;
    tcgetattr( STDIN_FILENO, &oldt );
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newt );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
    return ch;
}

int impresario(
        int        argc,
        char    ** argv,
        ART_GV   * art_gv
        )
{
    ART_APPLICATION_DEFINE_STANDARD_OPTIONS_WITH_FEATURES(
        "Renderer IPC",
          art_appfeatures_none_beyond_baseline
        | art_appfeatures_no_threading
        | art_appfeatures_no_verbosity_control
        );

    ART_APPLICATION_MAIN_OPTIONS_FOLLOW

    id  writeOption =
        [ FLAG_OPTION
            :   "writeImage"
            :   "w"
            :   "only write current result image to disk"
            ] ;

    id  displayOption =
        [ FLAG_OPTION
            :   "displayImage"
            :   "d"
            :   "write, tonemap & open current result image"
            ];

    id  terminateOption =
        [ FLAG_OPTION
            :   "terminateSampling"
            :   "t"
            :   "terminate image sampling & proceed with action sequence"
            ];

    id  repeatOption =
        [ [ INTEGER_OPTION
            :   "repeatIPC"
            :   "r"
            :   "<seconds>"
            :   "repeat -w/-d every n seconds until 'artist' terminates"
            ] withDefaultIntegerValue: DEFAULT_REPEAT_FREQUENCY ];

    id  pidOption =
        [ [ INTEGER_OPTION
            :   "processId"
            :   "i"
            :   "<pid>"
            :   "sets pid for other commands"
            ] withDefaultIntegerValue: INVALID_PID ];

    id  allOption =
        [ FLAG_OPTION
            :   "allInstances"
            :   "all"
            :   "send the IPC command to all running instances"
            ];

    id  targetedAppOption =
        [ [ STRING_OPTION
            :   "targetApp"
            :   "a"
            :   "<app name>"
            :   "send commands to a specific running ART app"
            ] withDefaultCStringValue: "artist" ];

    id  tevHostNameOption =
        [ [ STRING_OPTION
            :   "tevHost"
            :   "n"
            :   "<tev host name>"
            :   "sets host name for tev viewer"
            ] withDefaultCStringValue: LOCALHOST ];
    id  tevPortOption =
        [ [ INTEGER_OPTION
            :   "tevPort"
            :   "p"
            :   "<tev port>"
            :   "sets port for tev viewer"
            ]  withDefaultIntegerValue: TEV_PORT ];
    id  connectTevOption =
        [ FLAG_OPTION
            :   "tevConnect"
            :   "c"
            :   "retry connection to tev"
            ] ;
    

    char  * synopsis_string;
    //TODO:UPDATE SYNOPSIS
    asprintf(
	& synopsis_string,
          "This tool is capable of writing (-w) or displaying (-d) intermediate results from\n"
          "currently running instances of ART image sampling processes: by default, it tries\n"
          "to communicate with instances of the 'artist' renderer, but other ART application\n"
          "names can be specified via the -a option. Via -t, it can also terminate image \n"
          "sampling altogether, and proceed with the action sequence of the called app.\n\n"
          "If more than one instance of the app is running, you will asked to specify which\n"
          "instance you want to interact with. Otherwise, the <pid> argument can be left\n"
          "blank, and 'impresario' will figure out the pid itself. The selected command\n"
          "can also be sent to all running instances of a particular ART application.\n\n"
          "Note that any generated intermediate result image will have exactly the same\n"
          "filename as the final result, be in the same place in the filesystem, and\n"
          "will ultimately be over-written by the final result: via -w/-d 'impresario' only \n"
          "forces the app to flush its current results to disk (and possibly display them).\n\n"
          "You can also automatically repeat -w or -d commands every n seconds until\n"
          "the app terminates (default: every %d seconds).",
          DEFAULT_REPEAT_FREQUENCY
        );

    ART_NO_INPUT_FILES_APPLICATION_STARTUP_WITH_SYNOPSIS(
        "impresario",
        "ART inter-process renderer control",
        synopsis_string,
        "impresario [command] <optional pid>"
        );
    
    int  numberOfWriteOptions = 0;
    int  numberOfTevOptions = 0;
    
    if ( [ writeOption hasBeenSpecified ] )
    {   
        numberOfWriteOptions++;
    }
    
    if ( [ displayOption hasBeenSpecified ] )
    {
        numberOfWriteOptions++;
    }
    
    if ( [ terminateOption hasBeenSpecified ] )
    {   
        numberOfWriteOptions++;
    }
    


    if ( [ tevHostNameOption hasBeenSpecified ] )
    {   
        numberOfTevOptions++;
    }
    
    if ( [ tevHostNameOption hasBeenSpecified ] )
    {   
        numberOfTevOptions++;
    }
    
    if ( [ connectTevOption hasBeenSpecified ] )
    {   
        numberOfTevOptions++;
    }

    if ( numberOfWriteOptions > 1 )
    {
        fprintf(stderr,"Conflicting IPC write options specified!\n\n");

        [ ArcOption fprintShortUsageAndExit
            :   art_gv
            :   stderr
            ];
    }
    
    if ( numberOfWriteOptions == 0 && numberOfTevOptions==0)
    {
        fprintf(stderr,"No renderer IPC option specified!\n\n");

        [ ArcOption fprintShortUsageAndExit
            :   art_gv
            :   stderr
            ];
    }
    
    
    
    if (   [ terminateOption hasBeenSpecified ]
        && [ repeatOption hasBeenSpecified ] )
    {
        fprintf(stderr,"Repeated termination commands make no sense!\n\n");

        [ ArcOption fprintShortUsageAndExit
            :   art_gv
            :   stderr
            ];
    }

    //TODO/ASK REPEATED INTERACTING WITH TEV MIGHT NOT MAKE SENSE
    
    if (   [ terminateOption hasBeenSpecified ]
        && numberOfTevOptions>0 )
    {
        fprintf(stderr,"Terminating and interacting with tev makes no sense!\n\n");

        [ ArcOption fprintShortUsageAndExit
            :   art_gv
            :   stderr
            ];
    }
    





    // if (   [ repeatOption hasBeenSpecified ]
    //     && [ allOption hasBeenSpecified ] )
    // {
    //     fprintf(
    //         stderr,
    //         "Repeated sending of commands to all running "
    //         "instances not supported\n\n"
    //         );

    //     [ ArcOption fprintShortUsageAndExit
    //         :   art_gv
    //         :   stderr
    //         ];
    // }
    
    int  * pid = NULL;
    int    number_of_artist_pids = 0;
    
    art_pidof(
          [ targetedAppOption cStringValue ],
        & number_of_artist_pids,
        & pid
        );
    
    if ( number_of_artist_pids == 0 )
    {
        printf(
            "No running instances of '%s' found. Exiting.\n",
            [ targetedAppOption cStringValue ]
            );
        
        return 0;
    }
    
    int  pid_we_will_talk_to = [pidOption integerValue];
    //   Sanity check: if the user did specify a pid, we check the list
    //   of running artists, to see if it's actually there.
    
    if ( pid_we_will_talk_to != INVALID_PID )
    {
        BOOL  user_specified_pid_is_running_artist = NO;

        for ( int i = 0; i < number_of_artist_pids; i++ )
        {
            if( pid[i] == pid_we_will_talk_to ){
                user_specified_pid_is_running_artist = YES;
                break;
            }
        }

        if ( ! user_specified_pid_is_running_artist )
        {
            printf(
                "Target '%s' instance PID %d does not exist.\n",
                [ targetedAppOption cStringValue ],
                pid_we_will_talk_to
                );
            
            FREE_ARRAY(pid);

            return 0;
        }
    }
    else  //  and if they did not specify a PID, we complain if there is
    {     //  more than one copy of artist running
        if (     number_of_artist_pids > 1
            && ! [ allOption hasBeenSpecified ] )
        {
            printf(
                "Several running instances of '%s' found - PIDs are:\n\n",
                [ targetedAppOption cStringValue ]
                );
            
            for ( int i = 0; i < number_of_artist_pids; i++ )
            {
                printf("%d ", pid[i]);
            }

            printf(
                "\n\nPlease explicitly specify one of these as argument, "
                "or consider sending a command to all of them via -all.\n");

            FREE_ARRAY(pid);

            return 0;
        }
        
        pid_we_will_talk_to = pid[0];
    }

    if ( [ terminateOption hasBeenSpecified ] )
    {
        printf(
            "Warning: this IPC command will terminate "
            );

        if (   number_of_artist_pids > 1
            && [ allOption hasBeenSpecified ] )
        {
            printf(
                "all running instances of '%s'\n",
                [ targetedAppOption cStringValue ]
                );
        }
        else
        {
            printf(
                "'%s', PID %d\n",
                [ targetedAppOption cStringValue ],
                pid_we_will_talk_to
                );
        }

        printf(
            "Please confirm (Y/n): "
            );

        char  userChoice = art_getchar();
        
        if ( userChoice != 'Y' )
        {
            printf(
                "%c\n\nTermination command NOT confirmed, exiting.\n\n",
                userChoice
                );
            
            return 0;
        }
        else
        {
            printf(
                "%c\n\n",
                userChoice
                );
        }
    }


    //   If the user requested repeated updates, we need to check this
    //   before repeating.
    
    BOOL  artist_is_still_alive = NO;
    BOOL  first_signal = YES;
    ArcMessageQueue* messageQueue= [ALLOC_INIT_OBJECT(ArcMessageQueue)];
    [messageQueue clearMessages];
    
    
    
    if ( [ repeatOption hasBeenSpecified ] )
    {
        printf(
            "The requested signal will be sent every %ld seconds until '%s' "
            "terminates.\n\n",
            [ repeatOption integerValue ],
            [ targetedAppOption cStringValue ]
            );
    }

    do
    {

        if ( [ writeOption hasBeenSpecified ] )
        {
            if (   number_of_artist_pids > 1
                && [ allOption hasBeenSpecified ] )
            {
                printf(
                    "Sending intermediate result writing signal to PIDs: "
                    );
                
                for ( int i = 0; i < number_of_artist_pids; i++ )
                {
                    printf("%d ", pid[i]);
                    [messageQueue sendSimpleMessage:pid[i] :M_WRITE];
                }
            }
            else
            {
                if ( first_signal )
                {
                    printf(
                        "Sending intermediate result writing signal to PID %d.",
                        pid_we_will_talk_to
                        );
                    
                    first_signal = NO;
                }
                else
                {
                    printf(".");
                }
                [messageQueue sendSimpleMessage:pid_we_will_talk_to :M_WRITE];
            }
        }

        if ( [ displayOption hasBeenSpecified ] )
        {
            if (   number_of_artist_pids > 1
                && [ allOption hasBeenSpecified ] )
            {
                printf(
                    "Sending intermediate result display signal to PIDs: "
                    );
                
                for ( int i = 0; i < number_of_artist_pids; i++ )
                {
                    printf("%d ", pid[i]);
                    [messageQueue sendSimpleMessage:pid[i] :M_WRITE_TONEMAP];
                }
            }
            else
            {
                if ( first_signal )
                {
                    printf(
                        "Sending intermediate result display signal to PID %d.",
                        pid_we_will_talk_to
                        );
                    
                    first_signal = NO;
                }
                else
                {
                    printf(".");
                }
                
                [messageQueue sendSimpleMessage:pid_we_will_talk_to :M_WRITE_TONEMAP];
            }
        }
        
        if ( [ terminateOption hasBeenSpecified ] )
        {
            if (   number_of_artist_pids > 1
                && [ allOption hasBeenSpecified ] )
            {
                printf(
                    "Sending termination signal to PIDs: "
                    );
                
                for ( int i = 0; i < number_of_artist_pids; i++ )
                {
                    printf("%d ", pid[i]);
                    [messageQueue sendSimpleMessage:pid[i] :M_TERMINATE];
                }
            }
            else
            {
                printf(
                    "Sending termination signal to PID %d.",
                    pid_we_will_talk_to
                    );
                [messageQueue sendSimpleMessage:pid_we_will_talk_to :M_TERMINATE];
            }
        }
        
        if ( [ tevHostNameOption hasBeenSpecified ] )
        {
            if (   number_of_artist_pids > 1
                && [ allOption hasBeenSpecified ] )
            {
                printf(
                    "Sending host name to PIDs: "
                    );
                
                for ( int i = 0; i < number_of_artist_pids; i++ )
                {
                    printf("%d ", pid[i]);
                    [messageQueue sendHostName:pid[i] : [tevHostNameOption cStringValue]];
                }
            }
            else
            {
                printf(
                    "Sending host name to PID %d.",
                    pid_we_will_talk_to
                    );
                [messageQueue sendHostName:pid_we_will_talk_to : [tevHostNameOption cStringValue]];
            }
        }

        if ( [ tevPortOption hasBeenSpecified ] )
        {
            if (   number_of_artist_pids > 1
                && [ allOption hasBeenSpecified ] )
            {
                printf(
                    "Sending host port to PIDs: "
                    );
                
                for ( int i = 0; i < number_of_artist_pids; i++ )
                {
                    printf("%d ", pid[i]);
                    [messageQueue sendHostPort:pid[i] : [tevPortOption integerValue]];
                }
            }
            else
            {
                printf(
                    "Sending host port to PID %d.",
                    pid_we_will_talk_to
                    );
                [messageQueue sendHostPort:pid_we_will_talk_to : [tevPortOption integerValue]];
            }
        }

        if ( [ connectTevOption hasBeenSpecified ] )
        {
            if (   number_of_artist_pids > 1
                && [ allOption hasBeenSpecified ] )
            {
                printf(
                    "Sending connect to tev signal to PIDs: "
                    );
                
                for ( int i = 0; i < number_of_artist_pids; i++ )
                {
                    printf("%d ", pid[i]);
                    [messageQueue sendSimpleMessage: pid[i] : M_TEV_CONNECT];
                }
            }
            else
            {
                printf(
                    "Sending connect to tev signal to PID %d.",
                    pid_we_will_talk_to
                    );
                [messageQueue sendSimpleMessage: pid_we_will_talk_to : M_TEV_CONNECT];
            }
        }
        if ( [ repeatOption hasBeenSpecified ] )
        {
            fflush(stdout);
            
            //   Sleep the requested time...
            
            sleep( [ repeatOption integerValue ] );
            
            //   Free the previous list of 'artist' PIDs
            
            FREE_ARRAY(pid);

            //   Get a list of currently running copies of 'artist'
            
            art_pidof(
                  [ targetedAppOption cStringValue ],
                & number_of_artist_pids,
                & pid
                );

            //   See if our dude is still there
            //   If not, say goodbye
            
            artist_is_still_alive = NO;
            if([allOption hasBeenSpecified]){
                artist_is_still_alive=number_of_artist_pids>0;
                //TODO: We should clear messages for disapeared pids
            }else{
                for ( int i = 0; i < number_of_artist_pids; i++ )
                {
                    if( pid[i] == pid_we_will_talk_to )
                        artist_is_still_alive = YES;
                }
                if ( ! artist_is_still_alive )
                {
                    printf(
                        "\n\nPID %d is no longer running, terminating.\n",
                        pid_we_will_talk_to
                        );
                    [messageQueue  clearMessages:pid_we_will_talk_to];
                }
            }
            
        }
        else
        {
            printf("\n");
        }
    }
    while( [ repeatOption hasBeenSpecified ] && artist_is_still_alive );

    FREE_ARRAY(pid);
    
    return 0;
}

ADVANCED_RENDERING_TOOLKIT_MAIN(impresario)

// ===========================================================================
