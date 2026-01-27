#include "gt/gametank.h"
#include "gt/gfx/draw_queue.h"
#include "gen/assets/audio.h"
#include "gt/audio/music.h"
#include "gt/input.h"

#define BOXCOLOR 92
#define BOXCOLORA 127
#define PADDLECOLOR 7
#define PADDLEWIDTH 12
#define PADDLEHEIGHT 4
#define PADDLEY 110

#define PMASK0 (INPUT_MASK_UP)
#define PMASK1 (INPUT_MASK_DOWN)
#define PMASK2 (INPUT_MASK_LEFT)
#define PMASK3 (INPUT_MASK_RIGHT)
#define PMASK4 (INPUT_MASK_A)
#define PMASK5 (INPUT_MASK_B)
#define PMASK6 (INPUT_MASK_C)
#define PMASK7 (INPUT_MASK_START)


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
    //play_sound_effect(ASSET__audio__hit_bin_ID,1);
    play_sound_effect(ASSET__audio__flongNew_sfx_ID,(char)1);
}
void soundTestA(){
    //play_sound_effect(ASSET__audio__hit_bin_ID,1);
    play_sound_effect(ASSET__audio__chirp_sfx_ID,(char)1);
}
void soundCol(){
    play_sound_effect(ASSET__audio__bik_sfx_ID,(char)1);
}
bool detectPaddleCollision(char x1,char x2,char y1,char y2){

char px1 = paddleX;//leftmost
char px2 = paddleX + PADDLEWIDTH;//rightmost bount
char py2 = PADDLEY;//110 //bottom
char py1 = PADDLEY - PADDLEHEIGHT;

//flawed code only detects corners in bounds, not intersection
if ((x1 >= px1 & x1 <= px2) | x2 >= px1 & x2 <= px2){//check box inside paddle horizontally
    if ((py1 >= y1 & py1 <= y2) | (py2 >= y1 & py2 <= y2)){//check paddle inside box vertically
        return true;
    }
} else {
    return false;
}

return false;

}
bool boxColPrev = false;
bool boxSkipFrame = false;
void boxMotion(){
        boxSkipFrame = !boxSkipFrame;//simulating half-speed motion
        boxSkipCount++;
        if (boxSkipCount >= boxSkipFrames)
        {
            boxSkipCount=0;
            if (detectPaddleCollision(box_x,box_x+8,box_y - 8, box_y) & !boxColPrev){
                boxColPrev = true;
                dy = -1;
                box_x += dx;
                box_y = PADDLEY-PADDLEHEIGHT;
                soundCol();
            }
            else 
            {
                box_x += dx;
                box_y += dy;
                boxColPrev = false;
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
                    dy = -1;
                    soundTest();
                }
            }
        }

}


bool boxAColPrev = false;

void boxAMotion(){

        if (detectPaddleCollision(boxA_x,boxA_x+8,boxA_y - 8, boxA_y)  & !boxAColPrev){
            boxAColPrev = true;
            //dxA = -dxA;
            dyA = -2;
            boxA_x += dxA;
            boxA_y = PADDLEY-PADDLEHEIGHT;
            soundCol();
        }
            else
        {
            boxA_x += dxA;
            boxA_y += dyA;
            boxAColPrev = false;
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
                dyA = -2;
                soundTestA();
            }
        }

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


void paddleDraw(char potVal)
{
    char paddlex=potVal;
    if (paddlex < 1){
        paddlex = 1;
    } else if (paddlex>127-PADDLEWIDTH){
        paddlex = 127 - PADDLEWIDTH;
    }
    queue_draw_box(paddlex,PADDLEY,PADDLEWIDTH,PADDLEHEIGHT,PADDLECOLOR);
    paddleX = paddlex;
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
}
void inputBinaryDraw()
{
    //inputs mapped as 7 bytes from low to high (right to left)
    //up,down,left,right,a,b,c
    //128 possibilities, should map cleanly to pixels from left to right
    queue_draw_box(0,BINARYTESTPOSY,button_byte,1,182);

}

void main () {
    init_music();
    
    //play_sound_effect(ASSET__audio__hit_bin);

    while (1) {                                     //  Run forever
        queue_clear_screen(256);//256 black
        queue_draw_box(box_x, box_y, 8, 8, BOXCOLOR);
        queue_draw_box(boxA_x, boxA_y, 8, 8, BOXCOLORA);
        button_byte = buttons_to_byte(player1_buttons);

        inputButtonsDraw();
        inputBinaryDraw();//interpret button presses as 7 byte number

        queue_clear_border(2);
        boxMotion();
        boxAMotion();

        paddleDraw(button_byte);
        await_draw_queue();
        await_vsync(1);
        flip_pages();
        tick_music();
        update_inputs();
    }
}