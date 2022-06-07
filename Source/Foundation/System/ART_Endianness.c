#include "ART_Endianness.h"

# if __BYTE_ORDER == __LITTLE_ENDIAN
float htolefloat(float x){
    return x;
}
# else
float htolefloat(float x){
    float le;
    char * originalPtr = &x; 
    char * returnPtr = &le;
    for(int i = 0; i < 4; i++){
        returnPtr[i] = originalPtr[3 - i];
    }
    return le;
}
#endif