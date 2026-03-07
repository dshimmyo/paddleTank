#include "scoring.h"
#include "gt/feature/text/text.h"
#include <stdio.h>
#include <stdint.h>
#include "gen/assets/font.h"

void uint_to_string(unsigned int num, char* buffer) {
    char* ptr = buffer;
    char* start;
    char* end;
    
    // Handle the special case for 0
    if (num == 0) {
        *ptr++ = '0';
        *ptr = '\0';
        return;
    }

    // Convert number to string in reverse
    while (num > 0) {
        *ptr++ = '0' + (num % 10);
        num /= 10;
    }
    *ptr = '\0'; // Null terminate the string

    // Reverse the string to correct order
    start = buffer;
    end = ptr - 1;
    while (start < end) {
        char temp = *start;
        *start++ = *end;
        *end-- = temp;
    }
}

void print_scores(int num)
{
    char buffer = "";
    text_init();
    text_color = TEXT_COLOR_WHITE;
    text_cursor_y = 11;
    text_cursor_x = 5;
    #if SCORING_DIGITS==3
        if (num > 999) text_print_string("999");
        else
        {
            if (num < 10) text_print_string("00");
            else if (num < 100) text_print_string("0");
            //else if (num < 1000) text_print_string("0");
            uint_to_string(num,buffer);
            text_print_string(buffer);
        }
    #else //if SCORING_DIGITS==4
        if (num > 9999) text_print_string("9999");
        else
        {
            if (num < 10) text_print_string("000");
            else if (num < 100) text_print_string("00");
            else if (num < 1000) text_print_string("0");
            uint_to_string(num,buffer);
            text_print_string(buffer);
        }
    #endif
}
void scoring_init()
{
    //text_load_font();
    allocate_sprite(&ASSET__font__breakoutFont_bmp_load_list);
    text_init();
}
