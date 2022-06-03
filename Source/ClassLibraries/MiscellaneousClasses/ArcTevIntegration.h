#include "ART_Foundation.h"

typedef  struct {
    char* data;
    char* end;
    uint32_t len;
    uint32_t max_size;
} message_buffer_t;

@interface ArcTevIntegration 
        : ArcObject
{
        int socketHandle;
        char* hostName;
        char* hostPort;
        message_buffer_t buffer;
        @public 
        BOOL connected;
}

- (id) init
        ;
- (void) setHostName
        :(const char*) hostName
        ;
- (void) setHostPort
        :(uint32_t) hostPort
        ;
- (BOOL) tryConnection
        ;

- (void) createImage
        :(const char*) name
        :(BOOL) grabfocus
        :(const char*) channelNames
        :(int32_t) channelNumber
        :(int32_t) width
        :(int32_t) height
        ;
- (void) openImage
        :(const char*) name
        :(BOOL) grabfocus
        :(const char*) channelSelector
        ;
- (void) closeImage
        :(const char*) name
        ;
- (void) reloadImage
        :(const char*) name
        :(BOOL) grabfocus
        ;
        
- (void) updateImage
        :(const char*) name
        :(BOOL) grabFocus
        :(const char*) channelNames
        :(int32_t) channelNumber
        :(const int64_t*)channelOffsets
        :(const int64_t*)channelStrides
        :(int32_t) x
        :(int32_t) y
        :(int32_t) width
        :(int32_t) height
        :(const float*) data
        ;
- (void)dealloc; 



@end



