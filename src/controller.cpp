#include "../headers/controller.hpp"

PuzzleController::PuzzleController(QObject *parent) : QObject(parent){ generateAndSolve(0); }

QStringList PuzzleController::boardToQStringList(const Board& board) const {
    QStringList list;

    list.reserve(9);
    for(int row = 0; row < 3; row++){
        for (int col = 0; col < 3; col++){
            list.append(QString::number(board.getTile(Position{row, col})));
        }
    }
    return list;
}

QStringList PuzzleController::tiles() const { return boardToQStringList(m_smart_mode.getBoard()); }

int PuzzleController::currentStep() const { return m_smart_mode.getStep(); }

int PuzzleController::totalSteps() const { return m_smart_mode.getTotalSteps(); }

bool PuzzleController::isSolved() const { return m_smart_mode.isFinished(); }

int PuzzleController::nextTileIndex() const {
    const Position next_position = m_smart_mode.getNextTile();

    if(next_position.x < 0 || next_position.x >= 3 || next_position.y < 0 || next_position.y >= 3){ return -1; }
    return next_position.x * 3 + next_position.y;
}

void PuzzleController::generateAndSolve(int difficultyIndex){
    MOVES_DIFFICULTY difficulty = (difficultyIndex == 0) ? MOVES_DIFFICULTY::EASY : MOVES_DIFFICULTY::NO_EASY;

    do { m_smart_mode.randomStart(difficulty); } 
    while (!m_smart_mode.solve() || m_smart_mode.getTotalSteps() == 0);

    emit boardChanged();
}

void PuzzleController::nextStep(){ if (m_smart_mode.nextStep()){ emit boardChanged(); } }

void PuzzleController::previousStep(){ if(m_smart_mode.previousStep()){ emit boardChanged(); } }

void PuzzleController::restartSteps(){
    m_smart_mode.restartSteps();
    emit boardChanged();
}
