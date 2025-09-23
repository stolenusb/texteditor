#include <ncurses.h>
#include <iostream>
#include <fstream>
#include <vector>

int main(int argc, char* argv[]) {
    if(argc < 2) {
        std::printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    std::string filename = argv[1];
    std::ifstream file(filename);
    if(!file) {
        std::printf("Failed to open %s\n", filename.c_str());
        return 1;
    }

    std::vector<std::string> lines;
    std::string line;
    while(std::getline(file, line))
        lines.push_back(line);

    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(1);

    int height, width;
    getmaxyx(stdscr, height, width);

    WINDOW* header = newwin(1, width, 0, 0);
    WINDOW* text   = newwin(height - 2, width, 1, 0);
    WINDOW* footer = newwin(1, width, height - 1, 0);
    keypad(text, TRUE);

    int cursor_y = 0, cursor_x = 0, scroll_offset = 0, desired_x = 0;
    int text_height = height - 2;

    bool running = true;
    while(running) {
        // HEADER WINDOW
        werase(header);
        wattron(header, A_REVERSE);
        mvwprintw(header, 0, 0, "Editing: %s", filename.c_str());
        wattroff(header, A_REVERSE);
        wrefresh(header);

        // FOOTER WINDOW
        werase(footer);
        wattron(footer, A_REVERSE);
        mvwprintw(footer, 0, 0, "LINE: %d / %lu", scroll_offset + cursor_y + 1, lines.size());
        wattroff(footer, A_REVERSE);
        wrefresh(footer);

        // TEXT WINDOW (middle)
        werase(text);
        for(int i = 0; i < text_height; i++) {
            int line_index = scroll_offset + i;
            if(line_index >= (int)lines.size())
                break;
            mvwprintw(text, i, 0, "%s", lines[line_index].c_str());
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

        // input
        int ch = wgetch(text);
        switch(ch) {
            case KEY_UP:    {
                if(cursor_y > 0)
                    cursor_y--;
                else if(scroll_offset > 0)
                    scroll_offset--;
                break;
            }
            case KEY_DOWN: {
                if(cursor_y < text_height - 1 && scroll_offset + cursor_y + 1 < (int)lines.size())
                    cursor_y++;
                else if(scroll_offset + text_height < (int)lines.size())
                    scroll_offset++;
                break;
            }
            case KEY_LEFT:  if(cursor_x > 0) cursor_x--; break;
            case KEY_RIGHT: cursor_x++; break;
            case 'q': running = false; break;
        }

        desired_x = cursor_x;
    }

    delwin(text);
    delwin(header);
    delwin(footer);
    endwin();
    
    return 0;
}
