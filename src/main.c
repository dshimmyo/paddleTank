#include "gt/gametank.h"
#include "gt/gfx/draw_queue.h"
#include "gen/assets/audio.h"
#include "gt/audio/music.h"
#include "gt/input.h"
// #define FEEDBACK_AMT 0x04
// #define PITCH_MSB 0x10
// #define PITCH_LSB 0x20
// #define AMPLITUDE 0x30
#define BOXCOLOR 92
#define BOXCOLORA 127

//#define INPUT_MASK_ALL_KEYS (INPUT_MASK_UP|INPUT_MASK_DOWN|INPUT_MASK_LEFT|INPUT_MASK_RIGHT|INPUT_MASK_A|INPUT_MASK_B|INPUT_MASK_C|INPUT_MASK_START)

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
void soundTest(){
    //play_sound_effect(ASSET__audio__hit_bin_ID,1);
    play_sound_effect(ASSET__audio__flongNew_sfx_ID,(char)1);
}
void soundTestA(){
    //play_sound_effect(ASSET__audio__hit_bin_ID,1);
    play_sound_effect(ASSET__audio__chirp_sfx_ID,(char)1);
}
void boxMotion(){
        //boxSkipFrame = !boxSkipFrame;//simulating half-speed motion
        boxSkipCount++;
        if (boxSkipCount >= boxSkipFrames)
        {
            boxSkipCount=0;
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
                dy = -1;
                soundTest();
            }
        }
}

void boxAMotion(){
        boxA_x += dxA;
        boxA_y += dyA;
        if(boxA_x <= 1) {
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
void main () {
    init_music();
    
    //play_sound_effect(ASSET__audio__hit_bin);

    while (1) {                                     //  Run forever
        queue_clear_screen(256);//256 black
        queue_draw_box(box_x, box_y, 8, 8, BOXCOLOR);
        queue_draw_box(boxA_x, boxA_y, 8, 8, BOXCOLORA);

        //input testing
        if (player1_buttons==0){queue_draw_box(1,7,8,2,20);}
        if (player1_buttons & PMASK0){queue_draw_box(11,7,8,2,182);}
        if (player1_buttons & PMASK1){queue_draw_box(21,7,8,2,182);}
        if (player1_buttons & PMASK2){queue_draw_box(31,7,8,2,182);}
        if (player1_buttons & PMASK3){queue_draw_box(41,7,8,2,182);}
        if (player1_buttons & PMASK4){queue_draw_box(51,7,8,2,182);}
        if (player1_buttons & PMASK5){queue_draw_box(61,7,8,2,182);}
        if (player1_buttons & PMASK6){queue_draw_box(71,7,8,2,182);}
        if (player1_buttons & PMASK7){queue_draw_box(81,7,8,2,182);}
        queue_clear_border(2);
        boxMotion();
        boxAMotion();

        await_draw_queue();
        await_vsync(1);
        flip_pages();
        tick_music();
        update_inputs();
    }
}