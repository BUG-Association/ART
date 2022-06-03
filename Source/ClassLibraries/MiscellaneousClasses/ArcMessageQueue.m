
#define ART_MODULE_NAME     ArcMessageQueue

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <unistd.h>
#import "ArcMessageQueue.h"
#include "ART_ErrorHandling.h"
#define ALL_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |S_IROTH | S_IWOTH)
typedef struct {
    long mtype;
    message_t mtext;
} message_buffer_t;

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY
@interface ArcMessageQueue()
- (void) messageSend 
    : (message_buffer_t*) messageBuffer
    ;
@end

@implementation ArcMessageQueue

- (id) init
{
    self = [ super init ];

    if ( self )
    {
        key_t key = ftok("/", 46290);
        messageQueue = msgget(key, IPC_CREAT|ALL_MODE);
        ourProcessId = getpid();

        if(messageQueue == -1){
            ART_ERRORHANDLING_FATAL_ERROR("Failed to create/join Message Queue");
        }
    }
    
    return self;
}
- (void)dealloc
{
    [super dealloc];
}

- (void)clearMessages 
{
    message_buffer_t messageBuffer;
    while(true){
        int r = msgrcv(
            messageQueue, 
            &messageBuffer, 
            sizeof(message_t), 
            0, 
            IPC_NOWAIT);
        if(r == -1&& errno == ENOMSG)
            break;
    }
}

- (void)clearMessages:(long)pid {
    message_buffer_t messageBuffer;
    while(true){
        int r = msgrcv(
            messageQueue, 
            &messageBuffer, 
            sizeof(message_t), 
            pid, 
            IPC_NOWAIT);
        if(r == -1 && errno == ENOMSG)
            break;
    }
}

- (message_t)receiveMessage 
{
    message_buffer_t messageBuffer;
    while(true){
        int r = msgrcv(
            messageQueue, 
            &messageBuffer, 
            sizeof(message_t), 
            ourProcessId, 
            0);
        if(r != -1)
            break;
        if(errno == EINTR)
            continue;
        messageBuffer.mtext.type = M_INVALID;
    }
    return messageBuffer.mtext;
}


- (void) messageSend
    : (message_buffer_t*) messageBuffer
{
    while(true){
        int r = msgsnd(
            messageQueue, 
            messageBuffer, 
            sizeof(message_t),
            0);
        if(r != -1)
            break;
        if(errno == EINTR)
            continue;
        return;
    }
}

- (void)sendSimpleMessage
    :(long) pid 
    :(message_type_t) messageType 
{
    message_buffer_t messageBuffer;
    messageBuffer.mtext.type = messageType;
    messageBuffer.mtype = pid;
    [self messageSend: &messageBuffer];
}

- (void)sendHostName
    :(long) pid 
    :(const char *) name 
{
    message_buffer_t messageBuffer;
    messageBuffer.mtext.type = M_HOST;
    messageBuffer.mtype = pid;
    if(strlen(name) + 1 > MAX_MESSAGE_LENGTH){
        ART_ERRORHANDLING_FATAL_ERROR("hostname too long");
    }
    sprintf(messageBuffer.mtext.message_data,"%s",name);
    [self messageSend:&messageBuffer];
}

- (void)sendHostPort
    :(long) pid 
    :(uint32_t) port 
{
    message_buffer_t messageBuffer;
    messageBuffer.mtext.type = M_PORT;
    messageBuffer.mtype = pid;
    *(uint32_t*)(messageBuffer.mtext.message_data) = port;
    [self messageSend: &messageBuffer];
}

@end