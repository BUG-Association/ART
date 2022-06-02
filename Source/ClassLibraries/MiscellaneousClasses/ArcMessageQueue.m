




#include <stdio.h>
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
} msgbuf_t;

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArcMessageQueue

- (id) init
{
    
    self = [ super init ];

    if ( self )
    {
        //TODO: it would be better to do this with some other file.
        key_t key=ftok("/", 46290);
        message_queue= msgget(key, IPC_CREAT|ALL_MODE);
        process_id=getpid();

        if(message_queue==-1){
            ART_ERRORHANDLING_FATAL_ERROR("Failed to create/join Message Queue");
        }
    }
    
    return self;
}
- (void)dealloc
{
    
    [super dealloc];
}

- (void)clearMessages {
    msgbuf_t m;
    while(true){
        int r=msgrcv(message_queue, &m, sizeof(message_t), 0, IPC_NOWAIT);
        if(r==-1&&errno==ENOMSG)
            break;
    }
}

- (void)clearMessages:(long)pid {
    msgbuf_t m;
    while(true){
        int r=msgrcv(message_queue, &m, sizeof(message_t), pid, IPC_NOWAIT);
        if(r==-1&&errno==ENOMSG)
            break;
    }
}

- (message_t)receiveMessage {
    msgbuf_t m;
    while(true){
        int r=msgrcv(message_queue, &m, sizeof(message_t), process_id, 0);
        
        if(r!=-1)
            break;
        if(errno==EINTR)
            continue;
        m.mtext.type=M_INVALID;
    }
    return m.mtext;
}


- (void) messageSend : (msgbuf_t*) mbuff{
    while(true){
        int r=msgsnd(message_queue, mbuff, sizeof(message_t),0);
        if(r!=-1)
        {
            break;
        }
        if(errno==EINTR)
        {
            continue;
        }
        return;
    }
}

- (void)sendSimpleMessage:(long)pid :(message_type_t)message_type {
    msgbuf_t m;
    m.mtext.type=message_type;
    m.mtype=pid;
    [self messageSend:&m];
}

- (void)sendHostName:(long)pid :(const char *)name {
    msgbuf_t m;
    m.mtext.type=M_HOST;
    m.mtype=pid;
    sprintf(m.mtext.message_data,"%s",name);
    [self messageSend:&m];
}

- (void)sendHostPort:(long)pid :(uint32_t) port {
    msgbuf_t m;
    m.mtext.type=M_PORT;
    m.mtype=pid;
    *(uint32_t*)(m.mtext.message_data)=port;
    [self messageSend:&m];
}

@end