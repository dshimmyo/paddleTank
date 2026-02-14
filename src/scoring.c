#include "gt/feature/text/text.h"


void print_scores(int num)
{
    text_init();
    text_color = 0;
    text_cursor_y = 10;
    text_cursor_x = 5;
    text_print_string("0123");
}
void scoring_init()
{
    text_load_font();
    text_init();
}
