#include "../headers/controller.hpp"
#include "../headers/board.hpp"
#include "../headers/core.hpp"
#include "../headers/files.hpp"

#include <qobject.h>
#include <qtmetamacros.h>
#include <vector>

PuzzleController::PuzzleController(QObject *parent) : QObject(parent){
    manual_timer.setInterval(250);
    connect(&manual_timer, &QTimer::timeout, this, [this](){ // wtf
        if(!getIsManualMode()){ manual_timer.stop(); return; }    
        refreshManualStatus();
        emit stateChanged();
    });

    loadSortScores(); 
    generateAndSolve(0);
}

void PuzzleController::syncTimerStatus(){
    if(!getIsManualMode()){ manual_timer.stop(); return; }

    const Game &current_game = manual_mode.getGame();
    bool is_hard_manual = (current_game.getLevel().difficulty == DIFFICULTY::NO_EASY);
    bool is_playing = (current_game.getGameStatus() == GAME_STATUS::PLAYING);

    if(is_hard_manual && is_playing){
        if(!manual_timer.isActive()){ manual_timer.start(); }
    } else { manual_timer.stop(); }
}

void PuzzleController::loadSortScores(){
    best_scores_vector =  Files::loadFile();

    if(best_scores_vector.size() < 5){ best_scores_vector = { Score(), Score(), Score(), Score(), Score()}; }
    else if(best_scores_vector.size() > 5){ best_scores_vector.resize(5); }
    Score::sortScores(best_scores_vector);
}

void PuzzleController::recordManualScoreWon(){
    if(score_recorded){ return; }
    if(manual_mode.getGame().getGameStatus() != GAME_STATUS::WIN){ return; }

    int earned_points = manual_mode.getFinalScore().getPoints();
    best_scores_vector.push_back(Score(earned_points));

    Score::sortScores(best_scores_vector);

    if(best_scores_vector.size() > 5){ best_scores_vector.resize(5); }

    Files::saveFile(best_scores_vector);
    score_recorded = true;
}

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

bool PuzzleController::parseBoardInput(const QStringList &raw_values, std::vector<int> &out_values) const {
    if(raw_values.size() != 9){ return false; }

    out_values.clear();
    out_values.reserve(9);

    for(const QString &raw_item : raw_values){
        QString taken = raw_item.trimmed();

        if(taken.isEmpty()){ out_values.push_back(0); continue; }
    
        bool ok_flag = false;
        int num_value = taken.toInt(&ok_flag);

        if(!ok_flag){ return false; }
        out_values.push_back(num_value);
    }
    return Board::validLayout(out_values);
}

void PuzzleController::refreshManualStatus(){
    manual_mode.update();
    recordManualScoreWon();
    syncTimerStatus();
    GAME_STATUS game_status = manual_mode.getGame().getGameStatus();

    if(game_status == GAME_STATUS::WIN){ status_message_ = QStringLiteral("Finished. Score: %1").arg(manual_mode.getFinalScore().getPoints()); }
    else if(game_status == GAME_STATUS::TIME_UP){ status_message_ = QStringLiteral("Time up."); }
    else { status_message_ = QStringLiteral("Manual mode: start"); }
}

QString PuzzleController::getCurrentMode() const { return active_mode; }

bool PuzzleController::getIsManualMode() const { return active_mode == QStringLiteral("manual"); }

QStringList PuzzleController::getTileValues() const { 
    if(getIsManualMode()){ return boardToStringList(manual_mode.getBoard()); }
    return boardToStringList(smart_mode.getBoard());
}

int PuzzleController::getCurrentStep() const {
    if(getIsManualMode()){ return manual_mode.getGame().getMovements(); }
    return smart_mode.getStep();
}

int PuzzleController::getTotalSteps() const {
    if(getIsManualMode()){ return manual_mode.getGame().getOptimalMovements(); }
    return smart_mode.getTotalSteps();
}

bool PuzzleController::getIsSolved() const {
    if(getIsManualMode()){ return manual_mode.getGame().getGameStatus() == GAME_STATUS::WIN; }
    return smart_mode.isFinished();
}

bool PuzzleController::getIsSolvable() const { return current_solvable; }

int PuzzleController::getNextTileIndex() const {
    if(getIsManualMode()){ 
        Position clue_position = manual_mode.getClue();

        if(clue_position.x < 0 || clue_position.x >= 3 || clue_position.y < 0 || clue_position.y >= 3){ return -1; }
        return clue_position.x * 3 + clue_position.y;
    }

    const Position next_position = smart_mode.getNextTile();

    if(next_position.x < 0 || next_position.x >= 3 || next_position.y < 0 || next_position.y >= 3){ return -1; }
    return next_position.x * 3 + next_position.y;
}

void PuzzleController::generateAndSolve(int difficulty_index){
    if(getIsManualMode()){
        DIFFICULTY manual_difficulty = (difficulty_index == 0) ? DIFFICULTY::EASY : DIFFICULTY::NO_EASY;
        manual_mode.newGame(manual_difficulty);
        score_recorded = false;
        current_solvable = true;
        refreshManualStatus();
        emit stateChanged();
        return;
    }

    MOVES_DIFFICULTY difficulty = (difficulty_index == 0) ? MOVES_DIFFICULTY::EASY : MOVES_DIFFICULTY::NO_EASY;

    do { smart_mode.randomStart(difficulty); } 
    while(!smart_mode.solve() || smart_mode.getTotalSteps() == 0);

    current_solvable = true;
    status_message_ = QStringLiteral("Board ready");
    emit stateChanged();
}

QString PuzzleController::getStatusMessage() const { return status_message_; }

int PuzzleController::getMoveCount() const { return manual_mode.getGame().getMovements(); }

int PuzzleController::getFinalScore() const { return manual_mode.getFinalScore().getPoints(); }

QVariantList PuzzleController::getBestScores() const {
    QVariantList results;

    results.reserve(static_cast<int>(best_scores_vector.size()));

    for(const Score &sc : best_scores_vector){ results.append(sc.getPoints()); }
    return results;
}

bool PuzzleController::getHasTimer() const {
    if(!getIsManualMode()){ return false; }
    return manual_mode.getGame().getLevel().difficulty == DIFFICULTY::NO_EASY;
}

QString PuzzleController::getRemainingTimeTxt() const {
    if(!getHasTimer()){ return QString(); }

    const Game& current_game = manual_mode.getGame();
    Time remaining = current_game.getLevel().limit_time - current_game.getPlaytime().elapsed();

    return QString::fromStdString(remaining.timeText());
}

void PuzzleController::setMode(const QString &mode_name){
    if(mode_name == QStringLiteral("manual")){
        active_mode = QStringLiteral("manual");
        manual_mode.newGame(DIFFICULTY::EASY);
        score_recorded = false;
        current_solvable = true;
        refreshManualStatus();
        emit stateChanged();
        return;
    }

    active_mode = QStringLiteral("smart");
    current_solvable = smart_mode.isSolvable();
    status_message_ = (smart_mode.getPath().empty()) ? QStringLiteral("Ready") : QStringLiteral("Solution ready");
    emit stateChanged();
}

bool PuzzleController::solveCustomBoards(const QStringList &start_inputs, const QStringList &goal_inputs){
    std::vector<int> parsed_start;
    std::vector<int> parsed_goal;

    if(!parseBoardInput(start_inputs, parsed_start) || !parseBoardInput(goal_inputs, parsed_goal)){
        current_solvable = false;
        status_message_ = QStringLiteral("Invalid entry");
        emit stateChanged();
        return false;
    }

    smart_mode.setStart(parsed_start);
    smart_mode.setGoal(parsed_goal);

    if(!smart_mode.isSolvable()){
        current_solvable = false;
        status_message_ = QStringLiteral("Is not solvable");
        emit stateChanged();
        return false;
    }
    if(!smart_mode.solve()){
        current_solvable = false;
        status_message_ = QStringLiteral("A* didin't found a solution");
        emit stateChanged();
        return false;
    }

    active_mode = QStringLiteral("smart");
    current_solvable = true;
    status_message_ = QStringLiteral("Soluble");
    emit stateChanged();
    return true;
}

void PuzzleController::nextStep(){ 
    if(getIsManualMode()){ return; }
    if(smart_mode.nextStep()){ emit stateChanged(); } 
}

void PuzzleController::previousStep(){
    if(getIsManualMode()){ return; }
    if(smart_mode.previousStep()){ emit stateChanged(); }
}

void PuzzleController::restartSteps(){
    if(getIsManualMode()){ return; }

    smart_mode.restartSteps();
    emit stateChanged();
}

bool PuzzleController::clickTile(int tile_index){
    if(!getIsManualMode()){ return false; }
    if(tile_index < 0 || tile_index >= 9){ return false; }

    Position clicked_position{tile_index / 3, tile_index % 3};
    bool moved = manual_mode.click(clicked_position);
    refreshManualStatus();
    emit stateChanged();
    return moved;
}
