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

    WINDOW* status = newwin(1, width, 0, 0);
    WINDOW* text   = newwin(height - 1, width, 1, 0);

    int cursor_y = 0, cursor_x = 0;

    bool running = true;
    while(running) {
        // --- Draw status bar ---
        wattron(status, A_REVERSE);
        mvwprintw(status, 0, 0, "Editing: %s | Cursor: %d,%d", filename.c_str(), cursor_y, cursor_x);
        wattroff(status, A_REVERSE);
        wrefresh(status);

        // --- Draw text ---
        werase(text);
        for(int i = 0; i < height - 1 && i < (int)lines.size(); i++) {
            mvwprintw(text, i, 0, "%s", lines[i].c_str());
        }

        // --- Move cursor ---
        if(cursor_y >= height - 1) cursor_y = height - 2;   // prevent overflow
        if(cursor_x >= width) cursor_x = width - 1;
        wmove(text, cursor_y, cursor_x);
        wrefresh(text);

        // --- Input ---
        int ch = wgetch(text);
        switch(ch) {
            case KEY_UP:    if(cursor_y > 0) cursor_y--; break;
            case KEY_DOWN:  if(cursor_y < height - 2) cursor_y++; break;
            case KEY_LEFT:  if(cursor_x > 0) cursor_x--; break;
            case KEY_RIGHT: if(cursor_x < width - 1) cursor_x++; break;
            case 'q': running = false; break;
        }
    }

    delwin(text);
    delwin(status);
    endwin();
    return 0;
}
