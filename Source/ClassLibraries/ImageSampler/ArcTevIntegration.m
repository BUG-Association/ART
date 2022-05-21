
#include "include/ART_SystemDatatypes.h"
#include <bits/stdint-intn.h>
#include <endian.h>
#include <errno.h>

# if __BYTE_ORDER == __LITTLE_ENDIAN
float htolefloat(float x){
    return x;
}

# else
//Not sure if this work 100% but it should
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

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define ART_MODULE_NAME     ArnTevIntegration
#import "ArcTevIntegration.h"
const char
        OpenImage = 0,
        ReloadImage = 1,
        CloseImage = 2,
        UpdateImage = 3,
        CreateImage = 4,
        UpdateImageV2 = 5, // Adds multi-channel support
        UpdateImageV3 = 6, // Adds custom striding/offset support
        OpenImageV2 = 7; // Explicit separation of image name and channel selector
    
// ART_MODULE_INITIALISATION_FUNCTION
// (
//     (void) art_gv;
//     [ ArcTevIntegration registerWithRuntime ];
// )


ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

#define LOCALHOST "127.0.0.1"
#define TEV_PORT 14158
@implementation ArcTevIntegration
-(BOOL) connected {
    return _connected;
}
-(void) setConnected
:  (BOOL) value {
    _connected=value;
}
//ASK why doesnt it work
//@synthesize connected=_connected;


void init_char_buff(char_buff * buff){
    buff->len=0;
    buff->max_size=4096;
    buff->data=ALLOC_ARRAY(char,buff->max_size);
    if(!buff->data){
        //TODO: No memory what should we do?
        perror("Failed Alloc\n");
    }
    buff->end=buff->data;
}
void free_char_buff(char_buff * buff){
    FREE_ARRAY(buff->data);
}
void clean_char_buff(char_buff * buff){
    buff->len=0;
    buff->end=buff->data;
}
void grow_char_buff(char_buff * buff){
    char* new_ptr=REALLOC_ARRAY(buff->data, char, buff->max_size*2);
    if(new_ptr){
        buff->data=new_ptr;
        buff->end=buff->data+buff->len;
        buff->max_size*=2;
    }
    else{
        //TODO: No memory what should we do?

        perror("Failed Realloc\n");
    }
}
//TODO: handle OOM
void check_size(char_buff * buff,uint32_t s){
    while(buff->len+s>=buff->max_size)
        grow_char_buff(buff);
}
void char_buff_append_uint32(char_buff * buff,uint32_t n){
    
    check_size(buff, sizeof n);

    *((uint32_t*)buff->end)= htole32(n);
    buff->len+=sizeof n;
    buff->end+=sizeof n;
}

void char_buff_append_int32(char_buff * buff,int32_t n){
    
    check_size(buff, sizeof n);

    *((int32_t*)buff->end)= htole32(n);
    buff->len+=sizeof n;
    buff->end+=sizeof n;
}
void char_buff_append_int64(char_buff * buff,int64_t n){
    
    check_size(buff, sizeof n);

    *((int32_t*)buff->end)= htole64(n);
    buff->len+=sizeof n;
    buff->end+=sizeof n;
}

void char_buff_append_char(char_buff * buff,char n){
    
    check_size(buff, sizeof n);

    *buff->end= n;
    buff->len+=sizeof n;
    buff->end+=sizeof n;
}
void char_buff_append_str(char_buff * buff,char* n){
    unsigned long len=strlen(n)+1;
    check_size(buff, len);
    for (unsigned long i=0; i<len; i++) {
        buff->end[i]=n[i];
    }
    buff->len+=len;
    buff->end+=len;
}
void char_buff_append_float_array(char_buff * buff,float* data,size_t floats){
    
    check_size(buff, sizeof(float)*floats);
    float* iter=(float*)buff->end;
    for (unsigned long i=0; i<floats; i++) {
        iter[i]=htolefloat(data[i]);
    }
    buff->len+=sizeof(float)*floats;
    buff->end+=sizeof(float)*floats;
}
void char_buff_set_len(char_buff * buff){
    *((uint32_t*)buff->data)= htole32(buff->len);
}

- (id)init {
    self =
[ super init ];

    if ( self )
    {
        socket_handle = -1;
        self.connected=NO;
        address.sin_family = AF_INET;
        address.sin_port = htons(TEV_PORT); 
        address.sin_addr.s_addr = inet_addr(LOCALHOST);
        init_char_buff(&buffer);
        [self retryConnection];
        
    }
    
    return self;
}

- (void)retryConnection {
    if( socket_handle == -1){
        socket_handle=socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (socket_handle == -1) {
            perror("Connection error");
            return ;
        }
    }
    if (!self.connected) {
        if (connect(socket_handle, (struct sockaddr*) &address, sizeof address) == -1) {
            perror("Connection error");
            if(close(socket_handle)==-1){
                perror("Connection error");
            }else{
                socket_handle=-1;
            }
            
        }else{
            self.connected=YES;
        }
    }
}


- (void)createImage
    :(int32_t) width
    :(int32_t) height
{
    if(!self.connected)
    {
        return;
    }
    clean_char_buff(&buffer);
    char * name="render";
    char channel_names[3][2]={"R","G","B"};
    char_buff_append_uint32(&buffer, 0); //reserved for length
    char_buff_append_char(&buffer,CreateImage);
    char_buff_append_char(&buffer,true); //grabfocus
    char_buff_append_str(&buffer, name);
    char_buff_append_int32(&buffer, width);
    char_buff_append_int32(&buffer, height);
    char_buff_append_int32(&buffer, 3);//number of channels

    
    for (int i=0; i<3; i++) {
        char_buff_append_str(&buffer, channel_names[i]);
    }
    char_buff_set_len(&buffer);
    

    [self send];
}

- (void) send
{
    size_t bytes_written=0;
    while(bytes_written!=buffer.len){
        int bytes=write(socket_handle, buffer.data+bytes_written, buffer.len-bytes_written);
        if(bytes==-1){
            perror("write error\n");
        }else{
            bytes_written+=bytes;
        }
    }

}

- (void) updateImage
        :(int32_t) x
        :(int32_t) y
        :(int32_t) width
        :(int32_t) height
        :(float*) data
{
    if(!self.connected)
    {
        return;
    }
    clean_char_buff(&buffer);
    char * name="render";
    char channel_names[3][2]={"R","G","B"};
    int64_t strides[]={3,3,3};
    int64_t offset[]={0,1,2};
    char_buff_append_uint32(&buffer, 0); //reserved for length
    char_buff_append_char(&buffer,UpdateImageV3);
    char_buff_append_char(&buffer,false); //grabfocus
    char_buff_append_str(&buffer, name);
    char_buff_append_int32(&buffer, 3);//number of channels
    for (int i=0; i<3; i++) {
        char_buff_append_str(&buffer, channel_names[i]);
    }
    char_buff_append_int32(&buffer, x);
    char_buff_append_int32(&buffer, y);
    char_buff_append_int32(&buffer, width);
    char_buff_append_int32(&buffer, height);

    for (int i=0; i<3; i++) {
        char_buff_append_int64(&buffer, offset[i]);
    }
    for (int i=0; i<3; i++) {
        char_buff_append_int64(&buffer, strides[i]);
    }
    size_t nPixels = width * height;
    char_buff_append_float_array(&buffer, data, nPixels*3);
    
    
    char_buff_set_len(&buffer);
    

    [self send];
}
- (void)dealloc
{
    free_char_buff(&buffer);
    if(self.connected){
        close(socket_handle);
    }
    [super dealloc];
}
@end