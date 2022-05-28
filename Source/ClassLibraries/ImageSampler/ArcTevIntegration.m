

#define ART_MODULE_NAME     ArnTevIntegration

#include <endian.h>
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
    
# if __BYTE_ORDER == __LITTLE_ENDIAN
float htolefloat(float x){
    return x;
}
# else
float htolefloat(float x){
    float le;
    char * originalPtr =&x; 
    char * returnPtr =&le;
    for(int i=0;i<4;i++){
        returnPtr[i] = originalPtr[3-i];
    }
    return le;
}
#endif


ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArcTevIntegration
-(BOOL) connected {
    return _connected;
}
-(void) setConnected
:  (BOOL) value {
    _connected=value;
}

void init_char_buff(message_buffer * buff){
    buff->len=0;
    buff->max_size=4096;
    
    buff->data=ALLOC_ARRAY_ZERO(char,buff->max_size);
    if(!buff->data){
        perror("Failed tev buffer allocation\n");
        exit(1);
    }
    buff->end=buff->data;
}
void free_char_buff(message_buffer * buff){
    FREE_ARRAY(buff->data);
}
void clean_char_buff(message_buffer * buff){
    buff->len=0;
    buff->end=buff->data;
}
void grow_char_buff(message_buffer * buff){
    char* new_ptr=REALLOC_ARRAY(buff->data, char, buff->max_size*2);
    if(new_ptr){
        buff->data=new_ptr;
        buff->end=buff->data+buff->len;
        buff->max_size*=2;
    }else{
        perror("Failed tev buffer reallocation\n");
        exit(1);
    }
}

void check_size(message_buffer * buff,uint32_t s){
    while(buff->len+s>=buff->max_size)
        grow_char_buff(buff);
}
void char_buff_append_uint32(message_buffer * buff,uint32_t n){
    uint32_t len_bytes =sizeof(n);
    check_size(buff, len_bytes);

    *((uint32_t*)buff->end)= htole32(n);
    buff->len+=len_bytes;
    buff->end+=len_bytes;
}

void char_buff_append_int32(message_buffer * buff,int32_t n){
    uint32_t len_bytes =sizeof(n);
    check_size(buff, len_bytes);

    *((int32_t*)buff->end)= htole32(n);
    buff->len+=len_bytes;
    buff->end+=len_bytes;
}
void char_buff_append_int64(message_buffer * buff,int64_t n){
    uint32_t len_bytes =sizeof(n);
    check_size(buff, len_bytes);

    *((int32_t*)buff->end)= htole64(n);
    buff->len+=len_bytes;
    buff->end+=len_bytes;
}

void char_buff_append_char(message_buffer * buff,char n){
    uint32_t len_bytes =sizeof(n);
    check_size(buff, len_bytes);

    *buff->end= n;
    buff->len+=len_bytes;
    buff->end+=len_bytes;
}
void char_buff_append_str(message_buffer * buff,const char* n){
    unsigned long len=strlen(n)+1;
    check_size(buff, len);
    for (unsigned long i=0; i<len; i++) {
        buff->end[i]=n[i];
    }
    buff->len+=len;
    buff->end+=len;
}
void char_buff_append_float_array(message_buffer * buff,const float* data,size_t floats){
    uint32_t len_bytes =sizeof(float)*floats;
    check_size(buff, len_bytes);
    float* iter=(float*)buff->end;
    for (unsigned long i=0; i<floats; i++) {
        iter[i]=htolefloat(data[i]);
    }
    buff->len+=len_bytes;
    buff->end+=len_bytes;
}
void char_buff_set_len(message_buffer * buff){
    *((uint32_t*)buff->data)= htole32(buff->len);
}
- (id) init
{
    self = [ super init ];

    if ( self )
    {
        socket_handle = -1;
        _hostName=NULL;
        _hostPort=NULL;
        self.connected=NO;
        init_char_buff(&buffer);   
    }
    
    return self;
}
- (void) bufferStart
{
    clean_char_buff(&buffer);
    char_buff_append_uint32(&buffer, 0); //reserved for length
}
- (void) switchHost
    :(const char*) hostName
    :(uint32_t) hostPort
{
    if(hostPort>65535){
        printf("Invalid port\n");
        return;
    }
    FREE_ARRAY(_hostName);
    FREE_ARRAY(_hostPort);
    uint32_t hostNameBufferLen=strlen(hostName)+1;
    uint32_t portNameBufferLen=6;
    _hostName=ALLOC_ARRAY(char, hostNameBufferLen);
    _hostPort=ALLOC_ARRAY(char, portNameBufferLen);
    strcpy(_hostName, hostName);
    sprintf(_hostPort, "%u", hostPort);
}
typedef struct addrinfo addrinfo;
- (void)tryConnection {
    if (_hostName==NULL|| _hostPort==NULL) {
        return;
    }
    addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family=AF_INET;
    hints.ai_socktype=SOCK_STREAM;
    hints.ai_protocol=IPPROTO_TCP;
    addrinfo * result;
    if(getaddrinfo(_hostName, _hostPort, &hints, &result)==0){
        addrinfo * copy_free=result;
        while(result!=NULL){
            int potentialSocket= socket(result->ai_family, result->ai_socktype, result->ai_protocol);
            if(potentialSocket==-1) continue;
            if(connect(potentialSocket,result->ai_addr,result->ai_addrlen)==-1){
                close(potentialSocket);
                result=result->ai_next;
            }else{
                socket_handle=potentialSocket;
                self.connected=YES;
                break;
            }
        }
        freeaddrinfo(copy_free);
    }
}

- (void) send
{
    size_t bytes_written=0;
    while(bytes_written!=buffer.len){
        int bytes=write(socket_handle, buffer.data+bytes_written, buffer.len-bytes_written);
        if(bytes==-1){
            if(close(socket_handle)==-1){
                perror("Closing error\n");
                exit(1);
            }else{
                socket_handle=-1;
                self.connected=NO;
            }
            break;
        }else{
            bytes_written+=bytes;
        }
    }

}

- (void)createImage
    :(const char*) name
    :(BOOL) grabfocus
    :(const char*) channel_names
    :(int32_t) channel_number
    :(int32_t) width
    :(int32_t) height
{
    if(!self.connected)
        return;
    [self bufferStart];
    char_buff_append_char(&buffer,CreateImage);
    char_buff_append_char(&buffer,grabfocus); 
    char_buff_append_str(&buffer, name);
    char_buff_append_int32(&buffer, width);
    char_buff_append_int32(&buffer, height);
    [self appendChannelNames:channel_names :channel_number];
    char_buff_set_len(&buffer);
    

    [self send];
}
-(void) appendChannelNames
    :(const char*) channel_names
    :(int32_t) channel_number
{
    char_buff_append_int32(&buffer, channel_number);
    for (int i=0; i<channel_number; i++) {
        char_buff_append_char(&buffer, channel_names[i]);
        char_buff_append_char(&buffer, 0);
    }
}
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
{
    if(!self.connected)
        return;
    [self bufferStart];
    char_buff_append_char(&buffer,UpdateImageV3);
    char_buff_append_char(&buffer,grabfocus); 
    char_buff_append_str(&buffer, name);
    [self appendChannelNames:channel_names :channel_number];
    
    char_buff_append_int32(&buffer, x);
    char_buff_append_int32(&buffer, y);
    char_buff_append_int32(&buffer, width);
    char_buff_append_int32(&buffer, height);

    for (int i=0; i<channel_number; i++) {
        char_buff_append_int64(&buffer, channel_offsets[i]);
    }
    for (int i=0; i<channel_number; i++) {
        char_buff_append_int64(&buffer, channel_strides[i]);
    }
    size_t nPixels = width * height;
    char_buff_append_float_array(&buffer, data, nPixels*channel_number);
    
    
    char_buff_set_len(&buffer);
    [self send];
}

- (void)openImage
    :(const char *)name 
    :(BOOL)grabfocus 
    :(const char *)channel_selector 
{
    if(!self.connected)
        return;
    [self bufferStart];
    char_buff_append_char(&buffer,OpenImageV2);
    char_buff_append_char(&buffer,grabfocus); 
    char_buff_append_str(&buffer, name);
    char_buff_append_str(&buffer, channel_selector);
    char_buff_set_len(&buffer);
    [self send];
}

- (void)closeImage
    :(const char *)name 
{
    if(!self.connected)
        return;
    [self bufferStart];
    char_buff_append_char(&buffer,CloseImage);
    char_buff_append_str(&buffer, name);
    char_buff_set_len(&buffer);
    [self send];
}

- (void)reloadImage
    :(const char *)name
    :(BOOL)grabfocus 
{
    if(!self.connected)
        return;
    [self bufferStart];
    char_buff_append_char(&buffer,ReloadImage);
    char_buff_append_char(&buffer,grabfocus); 
    char_buff_append_str(&buffer, name);
    char_buff_set_len(&buffer);
    [self send];
}

- (void)dealloc
{
    free_char_buff(&buffer);
    FREE_ARRAY(_hostName);
    FREE_ARRAY(_hostPort);
    if(self.connected){
        close(socket_handle);
    }
    [super dealloc];
}

@end