#include "ART_Foundation.h"
#import "ART_Scenegraph.h"
#include <stdint.h>
#include "ArNode.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include <arpa/inet.h>
//ArpImageSampler, ArpImageSamplerMessenger, 
        //ArpCoding

typedef  struct {
    char* data;
    uint32_t len;
    uint32_t max_size;
    char* end;
} char_buff;

@interface ArcTevIntegration 
        : ArcObject
{
        struct sockaddr_in address;
        int socket_handle;
        BOOL _connected;
        char_buff buffer;
}
@property BOOL connected;
- (id) init
        ;
- (void) retryConnection
        ;

- (void) createImage
        :(int32_t) width
        :(int32_t) height
        ;
        
- (void) updateImage
        :(int32_t) x
        :(int32_t) y
        :(int32_t) width
        :(int32_t) height
        :(float*) data
        ;
- (void)dealloc; 



@end



