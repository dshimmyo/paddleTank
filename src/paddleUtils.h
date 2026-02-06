#define YELLOW 0b00011111
#define ORANGE 0b00111111 
#define PEACH 0b01011111 
#define LAVENDER 0b01111111 
#define PERIWINKLE 0b10011111
#define SKY 0b10111111
#define OCEAN 0b11011111
#define ALGAE 0b11111111
#define WHITE 0b00000111
unsigned char packColor(unsigned char h,unsigned char s, unsigned char v) ;
//new masks for experimental gamepad hardware:
//assignments based on genesis controller pinout
#define INPUT_MASK_MODE 1//first read 00000001
#define INPUT_MASK_X 2 //first read 00000010
#define INPUT_MASK_Y 4 //need to add another multiplexor or build npn/pnp circuit
#define INPUT_MASK_Z 8 //need to add another multiplexor or build npn/pnp circuit
#define PMASK0 (INPUT_MASK_UP) //should be second read pin 1
#define PMASK1 (INPUT_MASK_DOWN)
#define PMASK2 (INPUT_MASK_LEFT)
#define PMASK3 (INPUT_MASK_RIGHT)
#define PMASK4 (INPUT_MASK_A)
#define PMASK5 (INPUT_MASK_B)
#define PMASK6 (INPUT_MASK_C)
#define PMASK7 (INPUT_MASK_START)
#define PMASK8 (INPUT_MASK_X) //EXPERIMENTAL GAME HARDWARE
#define PMASK9 (INPUT_MASK_Y) //EXPERIMENTAL GAME HARDWARE
#define PMASK10 (INPUT_MASK_Z) //EXPERIMENTAL GAME HARDWARE
#define PMASK11 (INPUT_MASK_MODE) //EXPERIMENTAL GAME HARDWARE
