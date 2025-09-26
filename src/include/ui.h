#pragma once

#include <ncurses.h>
#include <vector>
#include <string>

namespace Ui {
    extern int text_height, cursor_y, cursor_x, scroll_offset, desired_x;

    void init();
    void clean();
    void drawHeader(const char* fmt, ...);
    void drawFooter(const char* fmt, ...);
    void drawText(std::vector<std::string> &lines);
    int readCh();
}