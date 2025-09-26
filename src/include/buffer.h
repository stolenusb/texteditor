#pragma once

#include <vector>
#include <string>

namespace Buffer {
    extern std::vector<std::string> lines;
    extern std::string filename;

    bool load();
    void save();
}