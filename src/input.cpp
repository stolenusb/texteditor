#include "input.h"
#include "ui.h"
#include "buffer.h"
#include <ncurses.h>

#define KEY_CTRl(k) ((k) & 0x1f)
#define KEY_ESCAPE 27
#define KEY_SUP 337
#define KEY_SDOWN 336

static void scroll_up(), scroll_down(), scroll_right(), scroll_left();

void Input::read(int ch, bool& running, bool& insertion)
{
    switch(ch) {
        // SCROLLING
        case KEY_UP: {
            Ui::selecting = false;
            scroll_up();
            
            break;
        }
        case KEY_DOWN: {
            Ui::selecting = false;
            scroll_down();
            
            break;
        }
        case KEY_LEFT: {
            Ui::selecting = false;
            scroll_left();

            break;
        }
        case KEY_RIGHT: {
            Ui::selecting = false;
            scroll_right();

            break;
        }

        // SCROLLING WITH SELECTION
        case KEY_SRIGHT: {
            if(!Ui::selecting) {
                Ui::selecting = true;
                Ui::select_start_x = Ui::cursor_x;
                Ui::select_start_y = Ui::cursor_y + Ui::scroll_offset;
            }

            scroll_right();

            Ui::select_end_x = Ui::cursor_x;
            Ui::select_end_y = Ui::cursor_y + Ui::scroll_offset;

            break;
        }
        case KEY_SLEFT: {
            if(!Ui::selecting) {
                Ui::selecting = true;
                Ui::select_start_x = Ui::cursor_x;
                Ui::select_start_y = Ui::cursor_y + Ui::scroll_offset;
            }
            
            scroll_left();

            Ui::select_end_x = Ui::cursor_x;
            Ui::select_end_y = Ui::cursor_y + Ui::scroll_offset;

            break;
        }
        case KEY_SUP: {
            if(!Ui::selecting) {
                Ui::selecting = true;
                Ui::select_start_x = Ui::cursor_x;
                Ui::select_start_y = Ui::cursor_y + Ui::scroll_offset;
            }

            scroll_up();
            
            Ui::select_end_x = Ui::cursor_x;
            Ui::select_end_y = Ui::cursor_y + Ui::scroll_offset;

            break;
        }
        case KEY_SDOWN: {
            if(!Ui::selecting) {
                Ui::selecting = true;
                Ui::select_start_x = Ui::cursor_x;
                Ui::select_start_y = Ui::cursor_y + Ui::scroll_offset;
            }
            
            scroll_down();
            
            Ui::select_end_x = Ui::cursor_x;
            Ui::select_end_y = Ui::cursor_y + Ui::scroll_offset;

            break;
        }

        // DELETE
        case KEY_BACKSPACE: {
            if(insertion) {
                int line_index = Ui::scroll_offset + Ui::cursor_y;
                if(Ui::cursor_x > 0) {
                    Buffer::lines[line_index].erase(Ui::cursor_x - 1, 1);
                    Ui::cursor_x--;
                } else if(line_index > 0) {
                    int prev_len = Buffer::lines[line_index - 1].size();
                    Buffer::lines[line_index - 1] += Buffer::lines[line_index];
                    Buffer::lines.erase(Buffer::lines.begin() + line_index);
                    if(Ui::cursor_y > 0)
                        Ui::cursor_y--;
                    else
                        Ui::scroll_offset--;

                    Ui::cursor_x = prev_len;
                }
            }

            break;
        }

        // NEW LINE (ENTER)
        case '\n': {
            if(insertion) {
                int line_index = Ui::scroll_offset + Ui::cursor_y;
                std::string &curr_line = Buffer::lines[line_index];
                std::string new_line = curr_line.substr(Ui::cursor_x);
                curr_line.erase(Ui::cursor_x);
                Buffer::lines.insert(Buffer::lines.begin() + line_index + 1, new_line);
                if(Ui::cursor_y < Ui::text_height - 1)
                    Ui::cursor_y++;
                else
                    Ui::scroll_offset++;
                
                Ui::cursor_x = 0;
            }

            break;
        }

        // keybinds
        case KEY_ESCAPE: if(insertion) insertion = false; break;
        case KEY_CTRl('w'): if(!insertion) running = false; break;
        case KEY_CTRl('i'): if(!insertion) insertion = true; break;
        case KEY_CTRl('s'): if(!insertion) Buffer::save(); break;

        // text character insertions
        default: {
            if(insertion && isprint(ch)) {
                int line_index = Ui::scroll_offset + Ui::cursor_y;
                Buffer::lines[line_index].insert(Ui::cursor_x, 1, char(ch));
                Ui::cursor_x++;
            }
            
            break;
        }
    }

    Ui::desired_x = Ui::cursor_x;
}

static void scroll_up()
{
    if(Ui::cursor_y > 0)
        Ui::cursor_y--;
    else if(Ui::scroll_offset > 0)
        Ui::scroll_offset--;                        
}

static void scroll_down()
{
    if(Ui::cursor_y < Ui::text_height - 1 && Ui::scroll_offset + Ui::cursor_y + 1 < (int)Buffer::lines.size())
        Ui::cursor_y++;
    else if(Ui::scroll_offset + Ui::text_height < (int)Buffer::lines.size())
        Ui::scroll_offset++;
}

static void scroll_right()
{
    int line_index = Ui::scroll_offset + Ui::cursor_y;
    if(line_index < (int)Buffer::lines.size()) {
        if(Ui::cursor_x < (int)Buffer::lines[line_index].size())
            Ui::cursor_x++;
        else if(line_index + 1 < (int)Buffer::lines.size()){
            if(Ui::cursor_y < Ui::text_height - 1) 
                Ui::cursor_y++;
            else
                Ui::scroll_offset++;
            
            Ui::cursor_x = 0;
        }
    }
}

static void scroll_left()
{
    if(Ui::cursor_x > 0)
        Ui::cursor_x--;
    else if(Ui::scroll_offset + Ui::cursor_y > 0) {
        if(Ui::cursor_y > 0)
            Ui::cursor_y--;
        else
            Ui::scroll_offset--;
        
        int prev_line_index = Ui::scroll_offset + Ui::cursor_y;
        if(prev_line_index < (int)Buffer::lines.size()) {
            Ui::cursor_x = Buffer::lines[prev_line_index].size();
        } else {
            Ui::cursor_x = 0;
        }
    }
}