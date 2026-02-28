#ifndef PADDLE_H
#define PADDLE_H

#include "gen/modules_enabled.h"

#ifndef ENABLE_MODULE_PADDLE
#error "Module PADDLE included but not enabled!"
#endif

#ifdef ENABLE_MODULE_PADDLE

//Input mask overrides
#undef INPUT_MASK_UP 
#undef INPUT_MASK_DOWN 
#undef INPUT_MASK_LEFT 
#undef INPUT_MASK_RIGHT 
#undef INPUT_MASK_A 
#undef INPUT_MASK_B 
#undef INPUT_MASK_C 
#undef INPUT_MASK_START 
#undef INPUT_MASK_ALL_KEYS

#define INPUT_MASK_UP		2048    //prioritizes high, was 2056
#define INPUT_MASK_DOWN		1024    //prioritizes high, was 1028 
#define INPUT_MASK_LEFT		512     //high
#define INPUT_MASK_RIGHT	256     //high
#define INPUT_MASK_A		16      //low
#define INPUT_MASK_B		4096    //high
#define INPUT_MASK_C		8192    //high
#define INPUT_MASK_START	32      //low
#define INPUT_MASK_MODE 1           //low
#define INPUT_MASK_X 2              //low
#define INPUT_MASK_Y 4              //low
#define INPUT_MASK_Z 8              //low
#define INPUT_MASK_ALL_KEYS (INPUT_MASK_UP|INPUT_MASK_DOWN|INPUT_MASK_LEFT|INPUT_MASK_RIGHT|INPUT_MASK_A|INPUT_MASK_B|INPUT_MASK_C|INPUT_MASK_START|INPUT_MASK_MODE|INPUT_MASK_X|INPUT_MASK_Y|INPUT_MASK_Z)//add new keys

unsigned char packColor(unsigned char h,unsigned char s, unsigned char v) ;
#endif //ifdef ENABLE_MODULE_PADDLE
#endif //ifdef PADDLE_H
