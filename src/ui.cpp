#include "ui.h"

WINDOW* header;
WINDOW* text;
WINDOW* footer;  

int Ui::text_height = 0, Ui::scroll_offset = 0, Ui::desired_x = 0;
int Ui::cursor_x = 0, Ui::cursor_y = 0;
int Ui::select_start_x = 0, Ui::select_start_y = 0;
int Ui::select_end_x = 0, Ui::select_end_y = 0;
bool Ui::selecting = false;

void Ui::init()
{
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(1);

    int height, width;
    getmaxyx(stdscr, height, width);
    text_height = height - 2;

    header = newwin(1, width, 0, 0);
    text   = newwin(height - 2, width, 1, 0);
    footer = newwin(1, width, height - 1, 0);

    keypad(text, TRUE);
}

void Ui::clean()
{
    delwin(text);
    delwin(header);
    delwin(footer);
    endwin();
}

void Ui::drawHeader(const char* fmt, ...)
{
    wmove(header, 0, 0);
    werase(header);
    wattron(header, A_REVERSE);

    va_list args;
    va_start(args, fmt);
    vw_printw(header, fmt, args);
    va_end(args);

    wattroff(header, A_REVERSE);
    wrefresh(header);
}

void Ui::drawFooter(const char* fmt, ...)
{
    wmove(footer, 0, 0);
    werase(footer);
    wattron(footer, A_REVERSE);

    va_list args;
    va_start(args, fmt);
    vw_printw(footer, fmt, args);
    va_end(args);

    wattroff(footer, A_REVERSE);
    wrefresh(footer);
}

void Ui::drawText(std::vector<std::string> &lines)
{
    werase(text);

    for(int i = 0; i < text_height; i++) {
        int line_index = scroll_offset + i;
        if(line_index >= (int)lines.size())
            break;
        
        const std::string& line = lines[line_index];
        
        for(int j = 0; j < (int)line.size(); j++) {
            bool in_selection = false;
            if(selecting) {
                int start_y = std::min(select_start_y, select_end_y);
                int end_y = std::max(select_start_y, select_end_y);

                int start_x = std::min(select_start_x, select_start_x);
                int end_x = std::max(select_start_x, select_end_x);
            
                if(line_index > start_y && line_index < end_y)
                    in_selection = true;
                else if(line_index == start_y && line_index == end_y && j >= start_x && j < end_x)
                    in_selection = true;
                else if(line_index == start_y && line_index < end_y && j >= start_x)
                    in_selection = true;
                else if(line_index > start_y && line_index == end_y && j < end_x)
                    in_selection = true;
            }

            if(in_selection)
                wattron(text, A_REVERSE);
            
            mvwaddch(text, i, j, line[j]);

            if(in_selection)
                wattroff(text, A_REVERSE);
        }
    }
    
    // cursor
    int line_index = scroll_offset + cursor_y;
    if(line_index < (int)lines.size()) {
        int line_len = lines[line_index].size();
        cursor_x = (desired_x <= line_len) ? desired_x : line_len;
    } else {
        cursor_x = 0;
    }
    
    if(cursor_y >= text_height)
        cursor_y = text_height - 1;

    wmove(text, cursor_y, cursor_x);
    wrefresh(text);
}

int Ui::readCh()
{
    return wgetch(text);
}