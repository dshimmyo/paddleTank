#include "gt/gametank.h"
#include "gt/gfx/draw_queue.h"
#include "gen/assets/audio.h"
#include "gt/audio/music.h"
// #define FEEDBACK_AMT 0x04
// #define PITCH_MSB 0x10
// #define PITCH_LSB 0x20
// #define AMPLITUDE 0x30

char box_x = 30, box_y = 20;
char boxA_x = 20, boxA_y = 30;
char boxSkipFrames = 3;
char boxSkipCount = 0;
char bgColor = 0;

char dx = 1, dy = 1;
char dxA = -2, dyA = 2;
void soundTest(){
    //play_sound_effect(ASSET__audio__hit_bin_ID,1);
    play_sound_effect(ASSET__audio__flong_sfx_ID,(char)1);
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
            soundTest();

        } else if(boxA_x >= 119) {
            dxA = -2;
            soundTest();
        }
        if(boxA_y <= 8) {
            dyA = 2;
            soundTest();
        } else if(boxA_y >= 112) {
            dyA = -2;
            soundTest();
        }
}
void main () {
    init_music();
    //play_sound_effect(ASSET__audio__hit_bin);

    while (1) {                                     //  Run forever
        queue_clear_screen(256);//256 black
        queue_draw_box(box_x, box_y, 8, 8, 92);
        queue_draw_box(boxA_x, boxA_y, 8, 8, 127);

        queue_clear_border(2);
        boxMotion();
        boxAMotion();

        await_draw_queue();
        await_vsync(1);
        flip_pages();
        tick_music();
    }
}