#include "gt/gametank.h"
#include "gt/gfx/draw_queue.h"
#include "gen/assets/audio.h"
#include "gt/audio/music.h"
#include "gt/input.h"
#include "gt/feature/random/random.h"

#define BOXCOLOR 92
#define BOXCOLORA 127
#define PADDLECOLOR 7
#define PADDLEWIDTH 12
#define PADDLEHEIGHT 4
#define PADDLEY 110

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

bool demoMode = true;
char box_x = 30, box_y = 20;
char boxA_x = 20, boxA_y = 30;
char boxSkipFrames = 3;
char boxSkipCount = 0;
char bgColor = 0;

char dx = 1, dy = 1;
char dxA = -2, dyA = 2;
char paddleX = 64;
#define BUTTONTESTPOSY 7
#define BINARYTESTPOSY 10
char button_byte=0;

void soundTest(){
    play_sound_effect(ASSET__audio__flongNew_sfx_ID,(char)1);
}
void soundTestA(){
    play_sound_effect(ASSET__audio__chirp_sfx_ID,(char)1);
}
void soundCol(){
    play_sound_effect(ASSET__audio__bik_sfx_ID,(char)1);
}
bool detectPaddleCollision(char x1,char x2,char y1,char y2)
{

char px1 = paddleX;//leftmost
char px2 = paddleX + PADDLEWIDTH;//rightmost bound
char py2 = PADDLEY;//110 //bottom bount higher number
char py1 = PADDLEY - PADDLEHEIGHT; //top bound, lower number

//flawed code only detects corners in bounds, not intersection
if ((x1 >= px1-1 && x1 <= px2+1) || (x2 >= px1-1 && x2 <= px2+1)){//check box inside paddle horizontally
    if (y2 == py1-2 && y1 < py1-2){//bottom of box is at or below the top of the paddle
    //if ((py1 >= y1-2 && py1 <= y2+2) || (py2 >= y1-2 && py2 <= y2+2)){//check paddle inside box vertically
        return true;
    }
    else return false;
} else {
    return false;
}

return false;

}
bool boxColPrev = false;
bool boxSkipFrame = false;
void randomizeBox();
void randomizeBoxA();
void boxMotion(){
        //boxColPrev = false;//hack test
        boxSkipFrame = !boxSkipFrame;//simulating half-speed motion
        boxSkipCount++;
        if (boxSkipCount >= boxSkipFrames)
        {
            boxSkipCount=0;
            {
                box_x += dx;
                box_y += dy;
                if(box_x == 1) {
                    dx = 1;
                    soundTest();
                } else if(box_x == 119) {
                    dx = -1;
                    soundTest();
                }
                if(box_y == 8) {
                    dy = 1;
                    soundTest();
                } else if(box_y == 112) {
                    randomizeBox();
                    //dy = -1;
                    soundTest();
                }
                if (detectPaddleCollision(box_x,box_x+8,box_y - 8, box_y) 
                //&& !boxColPrev
                ){
                    //boxColPrev = true;
                    dy = -1;
                    //box_y = PADDLEY-PADDLEHEIGHT-1;//height correction, maybe redundant
                    soundCol();
                }
                else{
                    //boxColPrev = false;
                }
            }
        }
}

//bool boxAColPrev = false;
void boxAMotion()
{
    //boxAColPrev = false;//hack test
    {
        boxA_x += dxA;
        boxA_y += dyA;
        if (boxA_x <= 1) {
            dxA = 2;
            soundTestA();
        } else if(boxA_x >= 119) {
            dxA = -2;
            soundTestA();
        }
        if(boxA_y <= 8) {
            dyA = 2;
            soundTestA();
        } else if(boxA_y >= 112) {
            randomizeBoxA();
            //dyA = -2;
            soundTestA();
        }
    }
    if (detectPaddleCollision(boxA_x,boxA_x+8,boxA_y - 8, boxA_y)
        // && !boxAColPrev
        ){
        //boxAColPrev = true;
        dyA = -1;
        //boxA_y = PADDLEY-PADDLEHEIGHT-1;//height correction, maybe redundant
        soundCol();
    }
    // else{
    //     boxAColPrev = false;
    // }
}
// Convert individual button states to a 7-bit byte
char buttons_to_byte(int player1_buttons) {
    char result = 0;
    
    // This could be optimized further if you know your masks are powers of 2
    if (player1_buttons & PMASK0) result |= (1 << 0);
    if (player1_buttons & PMASK1) result |= (1 << 1);
    if (player1_buttons & PMASK2) result |= (1 << 2);
    if (player1_buttons & PMASK3) result |= (1 << 3);
    if (player1_buttons & PMASK4) result |= (1 << 4);
    if (player1_buttons & PMASK5) result |= (1 << 5);
    if (player1_buttons & PMASK6) result |= (1 << 6);
    
    return result;
}

char ClampPaddleX(char paddlex){
    if (paddlex < 1){
        paddlex = 1;
    } else if (paddlex>127-PADDLEWIDTH){
        paddlex = 127 - PADDLEWIDTH;
    }
    return paddlex;
}
void paddleXFromPot(char potVal)
{
    char paddlex=potVal;
    paddleX = ClampPaddleX(paddlex);//paddlex;

}
void paddleXFromClosestBox(){
    char paddlex=paddleX;
    if (box_y > boxA_y){
        paddlex=(box_x + paddlex)/2;//kind of a lerp
    } else {
        paddlex=(boxA_x + paddlex)/2;//kind of a lerp
    }
    paddleX = ClampPaddleX(paddlex);
}
void inputButtonsDraw()
{
    //input testing
    //if (player1_buttons==0){queue_draw_box(1,BUTTONTESTPOSY,8,2,20);}//no button press
    if (player1_buttons & PMASK0){queue_draw_box(1,BUTTONTESTPOSY,8,2,182);}
    if (player1_buttons & PMASK1){queue_draw_box(11,BUTTONTESTPOSY,8,2,182);}
    if (player1_buttons & PMASK2){queue_draw_box(21,BUTTONTESTPOSY,8,2,182);}
    if (player1_buttons & PMASK3){queue_draw_box(31,BUTTONTESTPOSY,8,2,182);}
    if (player1_buttons & PMASK4){queue_draw_box(41,BUTTONTESTPOSY,8,2,182);}
    if (player1_buttons & PMASK5){queue_draw_box(51,BUTTONTESTPOSY,8,2,182);}
    if (player1_buttons & PMASK6){queue_draw_box(61,BUTTONTESTPOSY,8,2,182);}
    if (player1_buttons & PMASK7){queue_draw_box(71,BUTTONTESTPOSY,8,2,182);}
    if (player1_buttons & INPUT_MASK_X){queue_draw_box(81,BUTTONTESTPOSY,8,2,199);}//mode
    if (player1_buttons & INPUT_MASK_Y){queue_draw_box(91,BUTTONTESTPOSY,8,2,199);}//x
    if (player1_buttons & INPUT_MASK_Z){queue_draw_box(101,BUTTONTESTPOSY,8,2,199);}//Z
    if (player1_buttons & INPUT_MASK_MODE){queue_draw_box(111,BUTTONTESTPOSY,8,2,199);}//Z

}
void inputBinaryDraw()
{
    //inputs mapped as 7 bytes from low to high (right to left)
    //up,down,left,right,a,b,c
    //128 possibilities, should map cleanly to pixels from left to right
    queue_draw_box(0,BINARYTESTPOSY,button_byte,1,182);

}
bool previousStart = false;

void ToggleDemoMode()
{
    if (player1_buttons & INPUT_MASK_START)
    {
        if (!previousStart){
            demoMode = !demoMode;
        }
        previousStart = true;
    }
    else {
        previousStart = false;
    }
}

void randomizeBox(){
    box_x = rnd_range(0,119);
    box_y = rnd_range(8,68);
    if (rnd_range(0,10) > 5) dx = -dx;
    if (rnd_range(0,10) > 5) dy = -dy;
}
void randomizeBoxA(){
    boxA_x = rnd_range(0,119);
    boxA_y = rnd_range(8,68);
    if (rnd_range(0,10) > 5) dxA = -dxA;
    if (rnd_range(0,10) > 5) dyA = -dyA;
}
init_game()
{
    randomizeBox();
    randomizeBoxA();
}
void main () {
    init_music();
    init_game;
    while (1) {                                     //  Run forever
        queue_clear_screen(256);//256 black
        queue_draw_box(box_x, box_y, 8, 8, BOXCOLOR);
        queue_draw_box(boxA_x, boxA_y, 8, 8, BOXCOLORA);
        button_byte = buttons_to_byte(player1_buttons);//gets paddle input

        inputButtonsDraw();//debug display
        inputBinaryDraw();//debug line

        queue_clear_border(2);
        boxMotion();
        boxAMotion();

        ToggleDemoMode();
        if (demoMode){
            paddleXFromClosestBox();
        } else {
            paddleXFromPot(button_byte);
        }
        queue_draw_box(paddleX,PADDLEY,PADDLEWIDTH,PADDLEHEIGHT,PADDLECOLOR);//draw paddle

        await_draw_queue();
        await_vsync(1);
        flip_pages();
        tick_music();
        update_inputs();
    }
}