#include "../headers/controller.hpp"
#include "../headers/board.hpp"

PuzzleController::PuzzleController(QObject *parent) : QObject(parent){ generateAndSolve(0); }

QStringList PuzzleController::boardToStringList(const Board& goal_board) const {
    QStringList list;
    list.reserve(9);

    for(int row = 0; row < 3; row++){
        for(int col = 0; col < 3; col++){
            int tile_v = goal_board.getTile(Position{row, col});
            list.append((tile_v == 0) ? QString() : QString::number(tile_v));
        }
    }
    return list;
}

QStringList PuzzleController::getTileValues() const { return boardToStringList(smart_mode.getBoard()); }

int PuzzleController::getCurrentStep() const { return smart_mode.getStep(); }

int PuzzleController::getTotalSteps() const { return smart_mode.getTotalSteps(); }

bool PuzzleController::getIsSolved() const { return smart_mode.isFinished(); }

int PuzzleController::getNextTileIndex() const {
    const Position next_position = smart_mode.getNextTile();

    if(next_position.x < 0 || next_position.x >= 3 || next_position.y < 0 || next_position.y >= 3){ return -1; }
    return next_position.x * 3 + next_position.y;
}

void PuzzleController::generateAndSolve(int difficulty_index){
    MOVES_DIFFICULTY difficulty = (difficulty_index == 0) ? MOVES_DIFFICULTY::EASY : MOVES_DIFFICULTY::NO_EASY;

    do { smart_mode.randomStart(difficulty); } 
    while(!smart_mode.solve() || smart_mode.getTotalSteps() == 0);

    emit boardChanged();
}

void PuzzleController::nextStep(){ if(smart_mode.nextStep()){ emit boardChanged(); } }

void PuzzleController::previousStep(){ if(smart_mode.previousStep()){ emit boardChanged(); } }

void PuzzleController::restartSteps(){
    smart_mode.restartSteps();
    emit boardChanged();
}
