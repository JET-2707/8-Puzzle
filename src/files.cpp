#include "files.hpp"
#include "core.hpp"

#include <cstddef>
#include <fstream>
#include <ios>
#include <vector>

bool Files::saveFile(const std::vector<Score> &scores){
    if(scores.size() != 5){ return false; }

    std::ofstream file("data.bin", std::ios::binary);

    if(!file.is_open()){ return false; }

    file.write(reinterpret_cast<const char*>(scores.data()), 5 * sizeof(Score));

    file.close();
    return true;
}

std::vector<Score> Files::loadFile(){

    std::ifstream file("data.bin", std::ios::binary);

    if(!file.is_open()){ return { Score(), Score(), Score(), Score(), Score() }; }

    std::vector<Score> v(5);


    file.read(reinterpret_cast<char*>(v.data()), 5 * sizeof(Score));

    file.close();
    return v;
}
