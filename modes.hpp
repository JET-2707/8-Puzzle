#pragma once

#include "board.hpp"
#include "algorithms.hpp"
#include "core.hpp"
#include <vector>

class SmartMode {
    private:
        std::vector<Board> start_options; // It will give the user 3 options of a start board to choice.
        Board choiced;
        std::vector<Node> path;
    public:
        void generateOptions(const MOVES_DIFFICULTY &difficulty);
        void choiceOption(int option);
        void obtainPath();

        std::vector<Board> getOptions();
        std::vector<Node> getPath();

};
