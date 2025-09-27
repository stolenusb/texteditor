#pragma once

#include <ncurses.h>
#include <vector>
#include <string>

namespace Ui {
    extern int text_height, scroll_offset, desired_x;
    extern int cursor_x, cursor_y;
    extern int select_start_x, select_start_y;
    extern int select_end_x, select_end_y;
    extern bool selecting;

    void init();
    void clean();
    void drawHeader(const char* fmt, ...);
    void drawFooter(const char* fmt, ...);
    void drawText(std::vector<std::string> &lines);
    int readCh();
}