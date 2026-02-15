#include "gt/feature/text/text.h"
#include <stdio.h>


void print_scores(int num)
{
    char buffer = "";
    text_init();
    text_color = 0;
    text_cursor_y = 10;
    text_cursor_x = 5;
    sprintf(buffer, "%d", num);
    text_print_string(buffer);
}
void scoring_init()
{
    text_load_font();
    text_init();
}
