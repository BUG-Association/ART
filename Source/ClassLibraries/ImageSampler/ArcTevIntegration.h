#include "ART_Foundation.h"
#include <bits/stdint-uintn.h>

typedef  struct {
    char* data;
    char* end;
    uint32_t len;
    uint32_t max_size;
} message_buffer;

@interface ArcTevIntegration 
        : ArcObject
{
        int socket_handle;
        char* _hostName;
        char* _hostPort;
        BOOL _connected;
        message_buffer buffer;
}
@property BOOL connected;
- (id) init
        ;
- (void) switchHost
        :(const char*) hostName
        :(uint32_t) hostPort
        ;
- (void) tryConnection
        ;

- (void) createImage
        :(const char*) name
        :(BOOL) grabfocus
        :(const char*) channel_names
        :(int32_t) channel_number
        :(int32_t) width
        :(int32_t) height
        ;
- (void) openImage
        :(const char*) name
        :(BOOL) grabfocus
        :(const char*) channel_selector
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
        :(BOOL) grabfocus
        :(const char*) channel_names
        :(int32_t) channel_number
        :(const int64_t*)channel_offsets
        :(const int64_t*)channel_strides
        :(int32_t) x
        :(int32_t) y
        :(int32_t) width
        :(int32_t) height
        :(const float*) data
        ;
- (void)dealloc; 



@end



