/*
    Modo Manual
    1. Mover piezas según las reglas base
    2. Crear juego (aleatorio)
    3. Opcion: sugerir jugada
*/

#include "algorithms.hpp"
#include "modes.hpp"
#include "board.hpp"
#include "core.hpp"
#include <vector>


void SmartMode::generateOptions(const MOVES_DIFFICULTY &difficulty){ 
    Board goal({1, 2, 3, 4, 5, 6, 7, 8, 0});

    for(int i = 0; i < 3; i++){
        start_options.push_back(Game::generatePuzzle(goal, difficulty));
    }
}

void SmartMode::choiceOption(int option){
    switch(option){
        case 1: choiced = start_options[0]; break;
        case 2: choiced = start_options[1]; break;
        case 3: choiced = start_options[2]; break;
        default: choiced = Board(); break;
    }
}

void SmartMode::obtainPath(){
    Node start, goal;
    start.board = choiced;

    goal.board = Board({1, 2, 3, 4, 5, 6, 7, 8, 0});
    goal.g = 0;
    goal.h = 0;
    goal.f = 0;
    goal.parent = -1;

    Graph graph;
    path = graph.AStar(start, goal);    
}

std::vector<Board> SmartMode::getOptions(){ return start_options; }

std::vector<Node> SmartMode::getPath(){ return path; }
