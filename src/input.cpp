#include "input.h"
#include "ui.h"
#include "buffer.h"
#include <ncurses.h>

#define CTRL_KEY(k) ((k) & 0x1f)

void Input::read(int ch, bool& running, bool& insertion)
{
    switch(ch) {
        case KEY_UP: {
            if(Ui::cursor_y > 0)
                Ui::cursor_y--;
            else if(Ui::scroll_offset > 0)
                Ui::scroll_offset--;
            
            break;
        }
        case KEY_DOWN: {
            if(Ui::cursor_y < Ui::text_height - 1 && Ui::scroll_offset + Ui::cursor_y + 1 < (int)Buffer::lines.size())
                Ui::cursor_y++;
            else if(Ui::scroll_offset + Ui::text_height < (int)Buffer::lines.size())
                Ui::scroll_offset++;
            
            break;
        }
        case KEY_LEFT: {
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

            break;
        }
        case KEY_RIGHT: {
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

            break;
        }
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
        case 27: if(insertion) {insertion = false; break;} // ESCAPE KEY
        case CTRL_KEY('w'): if(!insertion) {running = false; break;}
        case CTRL_KEY('i'): if(!insertion) {insertion = true; break;}
        case CTRL_KEY('s'): if(!insertion) {Buffer::save();}
        default: {
            if(insertion && isprint(ch)) {
                int line_index = Ui::scroll_offset + Ui::cursor_y;
                Buffer::lines[line_index].insert(Ui::cursor_x, 1, char(ch));
                Ui::cursor_x++;
            }
        }

        break;
    }
}