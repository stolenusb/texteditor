#include "buffer.h"
#include "ui.h"
#include <fstream>
#include <thread>

std::vector<std::string> Buffer::lines;
std::string Buffer::filename = "";

bool Buffer::load()
{
    std::ifstream file(filename);

    if(!file) {
        std::printf("Failed to open %s\n", filename.c_str());

        return false;
    }

    std::string line;
    while(std::getline(file, line))
        lines.push_back(line);
    
    return true;
}

void Buffer::save()
{
    std::ofstream savefile(filename, std::ios::trunc);

    if(!savefile) {
        Ui::drawFooter("Failed to save to %s", filename.c_str());

        return;
    }

    for(const auto &line : lines)
        savefile << line << "\n";

    Ui::drawFooter("Successfully saved to %s", filename.c_str());
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}