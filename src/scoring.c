#include "gt/feature/text/text.h"


void print_scores(int num)
{
    text_init();
    text_color = 0;
    text_cursor_y = 10;
    text_cursor_x = 5;
    text_print_string("pppa");//pabcdefghi -> 0123456789
    // text_cursor_y = 20;
    // text_cursor_x = 5;
    // text_print_string("pabcdefghi");
    //display_score(10);
    // text_sprint_num(num,10);
    // text_print_string(num);
}
void scoring_init()
{
    text_load_font();
    text_init();
}
