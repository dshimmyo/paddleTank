#include "gt/gametank.h"
#include "gt/gfx/draw_queue.h"
#include "gen/assets/audio.h"
#include "gt/audio/music.h"
#include "gt/input.h"
#include "gt/feature/random/random.h"
#include "paddleUtils.h"
#include "scoring.h"

#define BOXCOLOR WHITE//92
#define BOXCOLORA WHITE
#define PADDLECOLOR WHITE
#define PADDLEWIDTH 16
#define PADDLEHEIGHT 4
#define PADDLEY 108
#define BALLSIZE 2
#define BRICKSROWSIZE 16
#define BRICKWIDTH 8
#define BRICKHEIGHT 4
#define BINARYTESTPOSY 10

#define NUMBRICKSH 16 
#define NUMBRICKSV 5
#define BRICKSYSTART 25
typedef struct {
    bool visible[NUMBRICKSH];
    char color;
} EntityRow;
typedef struct {
    unsigned long visibleByte16;
    char color;
} EntityRow16;
EntityRow16 testRow = {0b1111111111111111,0b11111111};
EntityRow16 brickRows[NUMBRICKSV] = {
{0b1111111111111111,0b01011011},
{0b1111111111111111,0b00111101},
{0b1111111111111111,0b00011111},
{0b1111111111111111,0b11111101},
{0b0111111110110111,0b10111100}
};//should initialize this in an init function

bool demoMode = true;
bool debugMode = false;
char box_x = 30, box_y = 20;
char boxA_x = 20, boxA_y = 30;

char bgColor = 0;

//256 is 1 pixel per frame
int dx = 192, dy = 128;//change from char to int, try 256 multiplier
int dxA = 512, dyA = 512;//2,2

char paddleX = 64;
unsigned char button_byte=0;
int numCollisions = 0;
char ClampLeft(int x);


void soundTest(){
    play_sound_effect(ASSET__audio__flongNew_sfx_ID,(char)1);
}
void soundTestA(){
    play_sound_effect(ASSET__audio__chirp_sfx_ID,(char)1);
    numCollisions++;
}
void soundCol(){
    play_sound_effect(ASSET__audio__bik_sfx_ID,(char)1);
}

bool detectPaddleCollision(char sourceXLow,char sourceXHi,char sourceYLow,char sourceYHi,
    char targetXLow,char targetXHi,char targetYLow, char targetYHi)
{
    //boxes draw left-right top-down 0->127, 7->120

    char px1 = targetXLow;//paddleX;//leftmost
    char px2 = targetXHi;//paddleX + PADDLEWIDTH;//rightmost bound
    char py1 = targetYLow;//PADDLEY - PADDLEHEIGHT; //top bound, lower number
    char py2 = targetYHi;//PADDLEY;//110 //bottom bount higher number

    if ((sourceXLow >= px1-1 && sourceXLow <= px2+1) || (sourceXHi >= px1-1 && sourceXHi <= px2+1)){//check box inside paddle horizontally
        if (sourceYHi >= py1 && sourceYLow < py2){//bottom of box is at or below the top of the paddle
            return true;
        }
        else return false;
    } else {
        return false;
    }

}

bool boxColPrev = false;
void randomizeBox();
void randomizeBoxA();

int dxRem=0;//remainder
int dyRem=0;
void boxMotion()
{
    char px1 = paddleX;//leftmost
    char px2 = paddleX + PADDLEWIDTH;//rightmost bound
    char py1 = PADDLEY;//110 //bottom bount higher number
    char py2 = PADDLEY + PADDLEHEIGHT; //top bound, lower number

    //check collision for every frame
    //frameskipping/subframe should only limit incrementing movement
    //boxes draw left-right top-down 0->127, 7->120
    int dxTot = dx + dxRem;
    int dyTot = dy + dyRem;
    if ((unsigned int) dxTot >= 255 || (unsigned int) dyTot >= 255)
    {
        box_x += dxTot>>8;
        box_y += dyTot>>8;
        if(box_x <= 1) {
            dx = (dx<0) ? -dx : dx;
            soundTestA();
        } else if(box_x >= 127-BALLSIZE /*119*/) {
            dx = (dx>0) ? -dx : dx;
            soundTestA();
        }
        if(box_y <= 7) {
            dy = (dy<0) ? -dy : dy;
            soundTestA();
        } else if(box_y >= 120-BALLSIZE){//112) {
            randomizeBox();
            soundTest();
        } else if (detectPaddleCollision(box_x,box_x+BALLSIZE,box_y, box_y+BALLSIZE,px1,px2,py1,py2)){
            dy = (dy>0) ? -dy : dy;
            box_y = PADDLEY-BALLSIZE;//height correction, maybe redundant
            soundCol();
        }
    }
    dxRem = dxTot & 255;// % 256;//update the remainder for sub-frame movement
    dyRem = dyTot & 255;//% 256;//update the remainder for sub-frame movement
}

int dxARem=0;//remainder
int dyARem=0;
void boxAMotion()
{
    char px1 = paddleX;//leftmost
    char px2 = paddleX + PADDLEWIDTH;//rightmost bound
    char py1 = PADDLEY;//110 //bottom bount higher number
    char py2 = PADDLEY + PADDLEHEIGHT; //top bound, lower number

    int dxATot = dxA + dxARem;
    int dyATot = dyA + dyARem;
    if ((unsigned int) dxATot >= 255 || (unsigned int) dyATot >= 255)
    {
        boxA_x += dxATot>>8;
        boxA_y += dyATot>>8;
        if (boxA_x <= 1) {
            dxA = (dxA<0) ? -dxA : dxA;
            soundTestA();
        } else if(boxA_x >= 127-BALLSIZE/*119*/) {
            dxA = (dxA>0) ? -dxA : dxA;
            soundTestA();
        }
        if(boxA_y <= 7) {
            dyA = (dyA<0) ? -dyA : dyA;
            soundTestA();
        } else if(boxA_y >= 120-BALLSIZE/*112*/) {
            randomizeBoxA();
            soundTest();
        } else if (detectPaddleCollision(boxA_x,boxA_x+BALLSIZE,boxA_y, boxA_y+BALLSIZE,px1,px2,py1,py2)){
            dyA = (dyA>0) ? -dyA : dyA;
            boxA_y = PADDLEY-BALLSIZE;//height correction, maybe redundant
            soundCol();
        }
    }
    dxARem = dxATot & 255;//% 256;//update the remainder for sub-frame movement
    dyARem = dyATot & 255;//% 256;//update the remainder for sub-frame movement
}

// Convert individual button states to a 8-bit byte
unsigned char buttons_to_byte_xyzm(int player1_buttons) {
    char result = 0;
    
    // This could be optimized further if you know your masks are powers of 2
    if (player1_buttons & INPUT_MASK_UP) result |= (1 << 0);
    if (player1_buttons & INPUT_MASK_DOWN) result |= (1 << 1);
    if (player1_buttons & INPUT_MASK_LEFT) result |= (1 << 2);
    if (player1_buttons & INPUT_MASK_RIGHT) result |= (1 << 3);
    if (player1_buttons & INPUT_MASK_X) result |= (1 << 4);
    if (player1_buttons & INPUT_MASK_Y) result |= (1 << 5);
    if (player1_buttons & INPUT_MASK_Z) result |= (1 << 6);
    if (player1_buttons & INPUT_MASK_MODE) result |= (1 << 7);

    return result;
}

int ClampPaddleX(int paddlex){
    if (paddlex < 1){
        paddlex = 1;
    } else if (paddlex>127-PADDLEWIDTH){
        paddlex = 127 - PADDLEWIDTH;
    }
    return paddlex;
}
int setRange(char input, char inMin, char inMax, char outMin, char outMax)
{
    int output = 0;
    if (input<inMin)return outMin;
    else if (input>inMax)return outMax;

    output = ((outMax - outMin)*256/(inMax-inMin)*(input-inMin)/256 + outMin);
    return output;
    
}
setRangeOpt(char input, char inMin, char inMax)
{
    int output = 0;
    if (input<inMin)return 0;
    else if (input>inMax)return 127;

    output = (127*256/(inMax-inMin)*(input-inMin)/256);
    return output;
    
}
char paddleXFromPot(unsigned char potVal)
{
    unsigned char newPotVal = setRange(potVal,32,96,0,127);//38,90 too fast,(32,96)maybe better
    return ClampPaddleX(((newPotVal<<1)/3 + paddleX/3));//fast and smooth
}
char paddleXFromPot8(unsigned char potVal)
{
    //unsigned char newPotVal = setRange(potVal,0b00011111,0b11100000,0,127);//changed for 8-bit
    unsigned char newPotVal = setRange(potVal,0b01000000,0b11000000,0,127);//changed for 8-bit

    return ClampPaddleX(((newPotVal<<1)/3 + paddleX/3));//fast and smooth
}
char paddleXFromPot8opt(unsigned char potVal)
{
    //need to make a cheaper setrange function
    unsigned char newPotVal = setRangeOpt(potVal,0b01000000,0b11000000);//changed for 8-bit

    return ClampPaddleX(newPotVal);//(((newPotVal<<1)/3 + paddleX/3));//no smoothing
}
char ClampLeft(int x){
    if (x<1){return 1;}
    return x;
}
int SlowLerp(int source, int target)
{
    //return target/3 + (source<<1)/3;//1/3 effort
    //return (target>>2) + ((source*3)>>2);//1/4 effort
    //return (target/5) + ((source<<2)/5);//1/5 effort
    //return (target/6) + ((source*5)/6);//1/6 effort
    //return (target/7) + ((source*6)/7);//1/7 effort
    return (target>>3) + ((source*7)>>3);//1/8 effort
}
int ConstVelocity(char source, char target, char vel){
    int result = source;
    if (source < target){
        result = source + vel;
        if (result > target) result = target;
    }
    else if (source > target){
        result = source - vel;
        if (result < target) result = target;
    }
    return ClampPaddleX(result);
}
char paddleXFromClosestBox(){
    int paddlex=0;
    if (box_y > boxA_y){
        paddlex=ConstVelocity(paddleX,box_x - (PADDLEWIDTH>>1),4);//SlowLerp(paddleX,box_x - (PADDLEWIDTH>>1));
    } else {
        paddlex=ConstVelocity(paddleX,boxA_x - (PADDLEWIDTH>>1),4);//SlowLerp(paddleX,boxA_x - (PADDLEWIDTH>>1));
    }
    return ClampPaddleX(paddlex);
}
void inputButtonsDraw()
{
    //input testing
    if (player1_buttons & INPUT_MASK_UP){queue_draw_box(1,7,8,2,182);}
    if (player1_buttons & INPUT_MASK_DOWN){queue_draw_box(11,7,8,2,182);}
    if (player1_buttons & INPUT_MASK_LEFT){queue_draw_box(21,7,8,2,182);}
    if (player1_buttons & INPUT_MASK_RIGHT){queue_draw_box(31,7,8,2,182);}
    if (player1_buttons & INPUT_MASK_A){queue_draw_box(41,7,8,2,182);}
    if (player1_buttons & INPUT_MASK_B){queue_draw_box(51,7,8,2,182);}
    if (player1_buttons & INPUT_MASK_C){queue_draw_box(61,7,8,2,182);}
    if (player1_buttons & INPUT_MASK_START){queue_draw_box(71,7,8,2,182);}
    if (player1_buttons & INPUT_MASK_X){queue_draw_box(81,7,8,2,199);}//x
    if (player1_buttons & INPUT_MASK_Y){queue_draw_box(91,7,8,2,199);}//y
    if (player1_buttons & INPUT_MASK_Z){queue_draw_box(101,7,8,2,199);}//z
    if (player1_buttons & INPUT_MASK_MODE){queue_draw_box(111,7,8,2,199);}//mode

}
void inputBinaryDraw()
{
    //inputs mapped as 7 bytes from low to high (right to left)
    //up,down,left,right,a,b,c
    //128 possibilities, should map cleanly to pixels from left to right
    queue_draw_box(0,BINARYTESTPOSY,button_byte>>1,1,182);
}
//bool previousStart = false;

void ToggleDemoMode()
{
    if (player1_buttons & INPUT_MASK_START && ~player1_old_buttons & INPUT_MASK_START)
    {
        //if (!previousStart){
            demoMode = !demoMode;
        //}
        //previousStart = true;
    }
    // else {
    //     previousStart = false;
    // }
}

void randomizeBox(){
    box_x = rnd_range(5,114);
    box_y = rnd_range(64,68);
    if (rnd_range(0,10) > 5) dx = -dx;
    if (rnd_range(0,10) > 5) dy = -dy;
}
void randomizeBoxA(){
    boxA_x = rnd_range(5,114);
    boxA_y = rnd_range(64,68);
    if (rnd_range(0,10) > 5) dxA = -dxA;
    if (rnd_range(0,10) > 5) dyA = -dyA;
}
void init_game()
{
    randomizeBox();
    randomizeBoxA();
    scoring_init();
}
// Define the structure to hold Color properties
unsigned char hue;        // 3 bits (0-7)
//unsigned char saturation; // 2 bits (0-3)
//unsigned char luminosity; // 3 bits (0-7)

// char colorTestHueOffset = 0;
// char hueTimer = 0;
// unsigned char simplePack(unsigned char h)
// {
//     return (h & 0x07) << 5 | 0b00011111;
// }
// void ColorTest(){
//     if (++hueTimer > 8) {
//         colorTestHueOffset++;
//         hueTimer = 0;
//     }
//     if (colorTestHueOffset > 7) colorTestHueOffset = 0;
//     hue = colorTestHueOffset;//colorTestHueOffset % 7;
//     //saturation = 3;
//     //luminosity = 7;
//     //boxes draw left-right top-down 0->127, 0->120
//     queue_draw_box(0, 118, 8, 2, simplePack(hue));//packColor(hue,3,7));//0b00011111);
//     queue_draw_box(8, 118, 8, 2, simplePack(hue+1));//packColor(hue+1,3,7));//0b00111111);
//     queue_draw_box(16, 118, 8, 2, simplePack(hue+2));//packColor(hue+2,3,7));//0b01011111);
//     queue_draw_box(24, 118, 8, 2, simplePack(hue+3));//packColor(hue+3,3,7));//0b01111111);
//     queue_draw_box(32, 118, 8, 2, simplePack(hue+4));//packColor(hue+4,3,7));//0b10011111);
//     queue_draw_box(40, 118, 8, 2, simplePack(hue+5));//packColor(hue+5,3,7));//0b10111111);
//     queue_draw_box(48, 118, 8, 2, simplePack(hue+6));//packColor(hue+6,3,7));//0b11011111);
//     queue_draw_box(56, 118, 8, 2, simplePack(hue+7));//packColor(hue+7,3,7));//0b11111111);

//     queue_draw_box(64, 118, 8, 2, simplePack(hue+4));//packColor(hue+4,3,7));//0b00011100);
//     queue_draw_box(72, 118, 8, 2, simplePack(hue+3));//packColor(hue+3,3,7));//0b00111100);
//     queue_draw_box(80, 118, 8, 2, simplePack(hue+2));//packColor(hue+2,3,7));//0b01011100);
//     queue_draw_box(88, 118, 8, 2, simplePack(hue+1));//packColor(hue+1,3,7));//0b01111100);
//     queue_draw_box(96, 118, 8, 2, simplePack(hue));//packColor(hue,3,7));//0b10011100);
//     queue_draw_box(104, 118, 8, 2, simplePack(hue+7));//packColor(hue+7,3,7));//0b10111100);
//     queue_draw_box(112, 118, 8, 2, simplePack(hue+6));//packColor(hue+6,3,7));//0b11011100);
//     queue_draw_box(120, 118, 8, 2, simplePack(hue+5));//packColor(hue+5,3,7));//0b11111100);

// }
char spiralX=0;
char spiralY=7;
unsigned char spiralEndTimer=0;
unsigned char GetSpiralColor(unsigned char y)
{
        unsigned char colorIndex = ((y)>>3) & 0b00000111;
        unsigned char hueShift = colorIndex<<6;
        unsigned char color = 0b000111111 | hueShift;
        return color;
}
void ColorSpiral(bool last)
{
    unsigned char x=0;
    unsigned char y=0;
    queue_clear_screen(256);//256 black

    for (y=8; y<=spiralY;y+=8){//fill in previous rows
        queue_draw_box(1,y,120,8,GetSpiralColor(y-8));
    }
    if (!last) //draws dynamic row and increments values for the next frame
    {
        queue_draw_box(1,spiralY,spiralX+8,8,GetSpiralColor(spiralY));//this row draws the last row dynamically

        spiralX+=8;
        if (spiralX == 120) 
        {
            spiralX = 0;
            spiralY +=8;
        }
    }
        await_draw_queue();
        await_vsync(1);
        flip_pages();
        tick_music();//optional?
        update_inputs();//optional?
}
void Intro_sequence(){
    char i=0;
    bool skipSequence=false;
    while (spiralY<112 && !skipSequence){ //intro color test sequence
        ColorSpiral(false);
        if (player1_buttons & INPUT_MASK_START && ~player1_old_buttons & INPUT_MASK_START) skipSequence=true;
    }

    spiralY=112;
    for (i=0;i<64;i++)
        {
            ColorSpiral(true);
        }
    queue_clear_border(1);
}
//int gamestate = 0;

unsigned char ClampX(unsigned char num)
{
    unsigned char result;
    result = (num < 0) ? 0 : num;
    result = (result > 127) ? 127 : result;
    return result;
}
void DrawBricks(){
    unsigned char y;
    for (y=0;y<NUMBRICKSV;y++)  
    {
        //unsigned char yIndexOffset = y*NUMBRICKSH;
        unsigned char posy = BRICKSYSTART + BRICKHEIGHT * y;//brickRow[y].posy;//brickRowYPos[y];
        unsigned char rowColor = brickRows[y].color;//brickRowColors[y];
        unsigned char x;
        unsigned long rowByte = brickRows[y].visibleByte16;
        for (x=0;x<NUMBRICKSH;x++)
        {
            //if (bricks[yIndexOffset + x].visible){
            if (rowByte & (1 << x))
            //if ((rowByte << x) & 1)
            {//if current brick is visible, draw the box
                unsigned char numBricksWidth = 1;
                unsigned char newX = x;
                unsigned char xDrawStart;
                unsigned char brickDrawWidth;
                if (x<NUMBRICKSH-1){//if it isn't the last box check the next brick in the sequence
                    bool done = false;
                    while (!done){
                        if (newX>=NUMBRICKSH-1){//end
                            done = true;
                        } else if (!(rowByte & (1 << (newX+1)))){//next brick is invisible
                            done = true;
                        }
                        else {
                            numBricksWidth++;
                            newX++;
                        }              
                    }
                }//if it is NUMBRICKSH-1 brick it will draw it
                //if (numBricksWidth>NUMBRICKSH) numBricksWidth=NUMBRICKSH;//total hack
                //queue_draw_box(brickColumnPos[x], posy, BRICKWIDTH, BRICKHEIGHT, rowColor);


                xDrawStart = x * BRICKWIDTH;
                brickDrawWidth = BRICKWIDTH * numBricksWidth;
                if (xDrawStart + brickDrawWidth > 127) brickDrawWidth-=1;//last brick index is one pixel short?

                queue_draw_box(xDrawStart,posy,brickDrawWidth,BRICKHEIGHT,rowColor);
                x=newX;
            }
        }
    }
}
void BreakoutGame(){
    char * num = "   ";
    queue_clear_screen(256);//256 black
    //ColorTest();//expensive calculation

    button_byte = buttons_to_byte_xyzm(player1_buttons);//gets paddle input
    if (player1_buttons & INPUT_MASK_A && ~player1_old_buttons & INPUT_MASK_A) 
    {
        debugMode = !debugMode;
    }
    if (debugMode){
        inputButtonsDraw();//debug display
        inputBinaryDraw();//debug line
    }

    DrawBricks();
    boxMotion();
    boxAMotion();

    ToggleDemoMode();
    if (demoMode){
        paddleX = paddleXFromClosestBox();
    } else {
        paddleX = paddleXFromPot8opt(button_byte);
    }
    queue_draw_box(box_x, box_y, BALLSIZE, BALLSIZE, BOXCOLOR);
    queue_draw_box(boxA_x, boxA_y, BALLSIZE, BALLSIZE, BOXCOLORA);
    queue_draw_box(paddleX,PADDLEY,PADDLEWIDTH,PADDLEHEIGHT,PADDLECOLOR);//draw paddle
    print_scores(numCollisions);
    
}

void main () {
    //init_paddle();
    init_music();
    init_game();
    queue_clear_screen(256);//256 black
    Intro_sequence();
    while (1) 
    {                                     //  Run forever
        BreakoutGame();
        queue_clear_border(2);
        await_draw_queue();
        await_vsync(1);
        flip_pages();
        tick_music();
        update_inputs();
    }
    
}