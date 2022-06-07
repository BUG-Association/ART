
#define ART_MODULE_NAME     ArcTevIntegration



#include <netinet/in.h>
#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>

#import "ArcTevIntegration.h"
const char
        ReloadImage = 1,
        CloseImage = 2,
        CreateImage = 4,
        UpdateImageV3 = 6, 
        OpenImageV2 = 7; 
    
ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

@interface ArcTevIntegration ()
- (void) send
    ;
-(void) appendChannelNames
    :(const char*) channelNames
    :(int32_t) channelNumber
    ;
- (void) messageInit
    ;
@end
@implementation ArcTevIntegration


void init_char_buff(message_buffer_t * buff){
    buff->len = 0;
    buff->max_size = 4096;
    
    buff->data=ALLOC_ARRAY_ZERO(char,buff->max_size);
    if(!buff->data){
        ART_ERRORHANDLING_FATAL_ERROR("Failed tev buffer allocation");
        
    }
    buff->end = buff->data;
}
void free_char_buff(message_buffer_t * buff){
    FREE_ARRAY(buff->data);
}
void clean_char_buff(message_buffer_t * buff){
    buff->len = 0;
    buff->end = buff->data;
}
void grow_char_buff(message_buffer_t * buff,uint32_t growTo){
    uint32_t new_size = buff->max_size;
    while (new_size < growTo) {
        new_size *= 2;
    }
    char* new_ptr = REALLOC_ARRAY(buff->data, char, new_size);
    if(new_ptr){
        buff->data = new_ptr;
        buff->end = buff->data + buff->len;
        buff->max_size = new_size;
    }else{
        ART_ERRORHANDLING_FATAL_ERROR("Failed tev buffer reallocation");
    }
}

void check_size(message_buffer_t* buff,uint32_t s){
    if(buff->len + s >= buff->max_size)
        grow_char_buff(buff,buff->len + s);
}
void char_buff_append_uint32(message_buffer_t* buff,uint32_t n){
    uint32_t len_bytes = sizeof(n);
    check_size(buff, len_bytes);

    *((uint32_t*)buff->end) = htole32(n);
    buff->len += len_bytes;
    buff->end += len_bytes;
}

void char_buff_append_int32(message_buffer_t * buff,int32_t n){
    uint32_t len_bytes = sizeof(n);
    check_size(buff, len_bytes);

    *((int32_t*)buff->end) = htole32(n);
    buff->len += len_bytes;
    buff->end += len_bytes;
}
void char_buff_append_int64(message_buffer_t * buff,int64_t n){
    uint32_t len_bytes = sizeof(n);
    check_size(buff, len_bytes);

    *((int32_t*)buff->end) = htole64(n);
    buff->len += len_bytes;
    buff->end += len_bytes;
}

void char_buff_append_char(message_buffer_t * buff,char n){
    uint32_t len_bytes = sizeof(n);
    check_size(buff, len_bytes);

    *buff->end = n;
    buff->len += len_bytes;
    buff->end += len_bytes;
}
void char_buff_append_str(message_buffer_t * buff,const char* n){
    uint32_t len_bytes = (strlen(n) + 1) * sizeof(char);
    check_size(buff, len_bytes);
    for (uint32_t i = 0; i < len_bytes; i++) {
        buff->end[i] = n[i];
    }
    buff->len += len_bytes;
    buff->end += len_bytes;
}
void char_buff_append_float_array(message_buffer_t * buff, const float* data, size_t floats){
    uint32_t len_bytes = sizeof(float)*floats;
    check_size(buff, len_bytes);
    float* iter = (float*)buff->end;
    for (size_t i = 0; i < floats; i++) {
        iter[i] = htolefloat(data[i]);
    }
    buff->len += len_bytes;
    buff->end += len_bytes;
}
void char_buff_set_len(message_buffer_t * buff){
    *((uint32_t*)buff->data) = htole32(buff->len);
}
- (id) init
{
    self = [ super init ];

    if ( self )
    {
        socketHandle = -1;
        hostName = NULL;
        hostPort = NULL;
        connected = NO;
        init_char_buff(&buffer);   
        signal(SIGPIPE, SIG_IGN);
    }
    
    return self;
}
- (void) messageInit
{
    clean_char_buff(&buffer);
    //reserved for length
    char_buff_append_uint32(&buffer, 0); 
}
- (void) setHostName
    :(const char*) newHostName
{
    FREE_ARRAY(hostName);
    
    if(asprintf(
        &hostName, 
        "%s", 
        newHostName) == -1){
        ART_ERRORHANDLING_FATAL_ERROR("asprintf failed");
    }
}
- (void) setHostPort
    :(uint32_t) newHostPort
{
    if(newHostPort > 65535){
        return;
    }
    FREE_ARRAY(hostPort);
    if(asprintf(
        &hostPort, 
        "%u", 
        newHostPort) == -1){
        ART_ERRORHANDLING_FATAL_ERROR("asprintf failed");
    }
}
typedef struct addrinfo addrinfo;
- (BOOL)tryConnection {
    
    if (hostName == NULL || hostPort == NULL) {
        return connected;
    }
    addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    addrinfo* result;
    if(getaddrinfo(
        hostName, 
        hostPort, 
        &hints, 
        &result)==0){
        addrinfo * copy_free = result;
        while(result!=NULL){
            int potentialSocket = socket(
                result->ai_family, 
                result->ai_socktype, 
                result->ai_protocol);

            if(potentialSocket == -1) 
                continue;

            if(connect(
                potentialSocket,
                result->ai_addr,
                result->ai_addrlen) == -1){

                close(potentialSocket);
                result = result->ai_next;

            }else{

                if(connected){
                    close(socketHandle);
                }
                socketHandle = potentialSocket;
                connected = YES;
                break;

            }
        }
        freeaddrinfo(copy_free);
    }
    return connected;
}

- (void) send
{
    char_buff_set_len(&buffer);
    size_t bytes_written = 0;
    while(bytes_written != buffer.len){
        int bytes = write(
            socketHandle, 
            buffer.data + bytes_written, 
            buffer.len - bytes_written);
        if(bytes == -1){
            if(close(socketHandle) == -1){
                ART_ERRORHANDLING_FATAL_ERROR("Closing error\n");
            }else{
                socketHandle = -1;
                connected = NO;
            }
            break;
        }else{
            bytes_written += bytes;
        }
    }

}

- (void)createImage
    :(const char*) name
    :(BOOL) grabFocus
    :(const char*) channelNames
    :(int32_t) channelNumber
    :(int32_t) width
    :(int32_t) height
{
    if(!connected)
        return;
    [self messageInit];
    char_buff_append_char(&buffer,CreateImage);
    char_buff_append_char(&buffer,grabFocus); 
    char_buff_append_str(&buffer, name);
    char_buff_append_int32(&buffer, width);
    char_buff_append_int32(&buffer, height);
    [self appendChannelNames:channelNames :channelNumber];
    

    [self send];
}
-(void) appendChannelNames
    :(const char*) channelNames
    :(int32_t) channelNumber
{
    char_buff_append_int32(&buffer, channelNumber);
    for (int i=0; i<channelNumber; i++) {
        char_buff_append_char(&buffer, channelNames[i]);
        char_buff_append_char(&buffer, '\0');
    }
}
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
{
    if(!connected)
        return;
    [self messageInit];
    char_buff_append_char(&buffer,UpdateImageV3);
    char_buff_append_char(&buffer,grabFocus); 
    char_buff_append_str(&buffer, name);
    [self appendChannelNames:channelNames :channelNumber];
    
    char_buff_append_int32(&buffer, x);
    char_buff_append_int32(&buffer, y);
    char_buff_append_int32(&buffer, width);
    char_buff_append_int32(&buffer, height);

    for (int i=0; i<channelNumber; i++) {
        char_buff_append_int64(&buffer, channelOffsets[i]);
    }
    for (int i=0; i<channelNumber; i++) {
        char_buff_append_int64(&buffer, channelStrides[i]);
    }
    size_t nPixels = width * height;
    char_buff_append_float_array(&buffer, data, nPixels*channelNumber);
    
    
    [self send];
}

- (void)openImage
    :(const char *) name 
    :(BOOL) grabFocus 
    :(const char *) channelSelector 
{
    if(!connected)
        return;
    [self messageInit];
    char_buff_append_char(&buffer,OpenImageV2);
    char_buff_append_char(&buffer,grabFocus); 
    char_buff_append_str(&buffer, name);
    char_buff_append_str(&buffer, channelSelector);
    [self send];
}

- (void)closeImage
    :(const char *) name 
{
    if(!connected)
        return;
    [self messageInit];
    char_buff_append_char(&buffer,CloseImage);
    char_buff_append_str(&buffer, name);
    [self send];
}

- (void)reloadImage
    :(const char *) name
    :(BOOL) grabFocus 
{
    if(!connected)
        return;
    [self messageInit];
    char_buff_append_char(&buffer,ReloadImage);
    char_buff_append_char(&buffer,grabFocus); 
    char_buff_append_str(&buffer, name);
    
    [self send];
}

- (void)dealloc
{
    free_char_buff(&buffer);
    FREE_ARRAY(hostName);
    FREE_ARRAY(hostPort);
    if(connected){
        close(socketHandle);
    }
    [super dealloc];
}

@end
