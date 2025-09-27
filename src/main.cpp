#include <ncurses.h>
#include <iostream>
#include <vector>
#include "buffer.h"
#include "ui.h"
#include "input.h"

int main(int argc, char* argv[])
{
    if(argc < 2) {
        std::printf("Usage: %s <filename>\n", argv[0]);

        return 1;
    }
    
    Buffer::filename = argv[1];

    if(!Buffer::load())
        return 1;

    Ui::init();

    bool running = true;
    bool insertion = false;
    while(running) {
        // HEADER WINDOW
        Ui::drawHeader("Editing %s", Buffer::filename.c_str());

        // FOOTER WINDOW
        Ui::drawFooter(
            "MODE: %s | LINE: %d / %lu",
            insertion ? "INSERTION" : "READING",
            Ui::scroll_offset + Ui::cursor_y + 1,
            Buffer::lines.size()
        );

        // TEXT WINDOW (middle)
        Ui::drawText(Buffer::lines);

        // INPUT
        Input::read(Ui::readCh(), running, insertion);
    }

    Ui::clean();
    
    return 0;
}
