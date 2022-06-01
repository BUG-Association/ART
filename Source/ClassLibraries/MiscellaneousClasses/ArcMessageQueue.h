#include "ART_Foundation.h"


#define MAX_MESSAGE_LENGTH 512
typedef enum{
        M_WRITE,
        M_WRITE_TONEMAP,
        M_TERMINATE,
        M_PORT,
        M_HOST,
        M_TEV_CONNECT,
        M_INVALID,
} message_type_t;
typedef struct {
    message_type_t type;
    char message_data[MAX_MESSAGE_LENGTH];
} message_t;


@interface ArcMessageQueue 
        : ArcObject
{
        int message_queue;
        int process_id;
}

- (id) init
        ;
- (void) sendSimpleMessage
        :(long)pid
        :(message_type_t) message_type
        ;
- (void) sendHostPort
        :(long)pid
        :(uint32_t) port
        ;
- (void) sendHostName
        : (long)pid
        : (const char *) name
        ;
- (message_t) receiveMessage
        ;
- (void) clearMessages
        ;
- (void) clearMessages
        :(long)pid
        ;
- (void)dealloc; 

@end