#pragma once

#include "core.hpp"
#include <vector>

class Files {
    public:
        static bool saveFile(const std::vector<Score> &scores);
        static std::vector<Score> loadFile();
};
