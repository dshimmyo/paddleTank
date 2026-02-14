#ifndef INPUT_H
#define INPUT_H

extern int player1_buttons, player1_old_buttons, player1_new_buttons;
extern int player2_buttons, player2_old_buttons, player2_new_buttons;

#define INPUT_MASK_UP		2048//2056 //removes Z button
#define INPUT_MASK_DOWN		1024//1028 //removes Y button
#define INPUT_MASK_LEFT		512
#define INPUT_MASK_RIGHT	256
#define INPUT_MASK_A		16
#define INPUT_MASK_B		4096
#define INPUT_MASK_C		8192
#define INPUT_MASK_START	32
#define INPUT_MASK_MODE 1// new
#define INPUT_MASK_X 2 // new
#define INPUT_MASK_Y 4 // new
#define INPUT_MASK_Z 8 // new
#define INPUT_MASK_ALL_KEYS (INPUT_MASK_UP|INPUT_MASK_DOWN|INPUT_MASK_LEFT|INPUT_MASK_RIGHT|INPUT_MASK_A|INPUT_MASK_B|INPUT_MASK_C|INPUT_MASK_START|INPUT_MASK_MODE|INPUT_MASK_X|INPUT_MASK_Y|INPUT_MASK_Z)//add new keys
#define INPUT_MASK_ANY_DIRECTION (INPUT_MASK_UP|INPUT_MASK_DOWN|INPUT_MASK_LEFT|INPUT_MASK_RIGHT)

void update_inputs();

#endif