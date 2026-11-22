#include "modes.hpp"
#include "algorithms.hpp"
#include "board.hpp"
#include "core.hpp"
#include <memory>
#include <vector>

bool Mode::click(const Position&){ return false; }
std::vector<Node> Mode::obtainPath(const Board& from, const Board& to){
    Node start_node, goal_node;

    start_node.board = from;
    start_node.g = 0;
    start_node.h = 0;
    start_node.f = 0;
    start_node.parent = -1;

    goal_node.board = to;
    goal_node.g = 0;
    goal_node.h = 0;
    goal_node.f = 0;
    goal_node.parent = -1;

    return solver.AStar(start_node, goal_node);
}

ManualMode::ManualMode(DIFFICULTY diff)
    : difficulty(diff), clue{-1, -1}, score_ready(false) { newGame(diff); }

void ManualMode::newGame(DIFFICULTY diff){
    difficulty = diff;
    clue = {-1, -1};
    final_score = Score();
    score_ready = false;

    MOVES_DIFFICULTY depth = (difficulty == DIFFICULTY::EASY) ? MOVES_DIFFICULTY::EASY : MOVES_DIFFICULTY::NO_EASY;
    Board start;
    std::vector<Node> path;

    while (path.size() < 2){
        start = Game::generatePuzzle(goal, depth);
        path = obtainPath(start, goal);
    } // while

    int optimal = static_cast<int>(path.size()) - 1;
    Level level = Game::makeLevel(diff, optimal);
    game = std::make_unique<Game>(level, start, goal, optimal);
}

void ManualMode::checkEnd(){
    if (score_ready || !isFinished()) return;
    final_score.calculatePoints(*game);
    score_ready = true;
}

bool ManualMode::click(const Position& p){
    game->update();
    if (game->getGameStatus() != GAME_STATUS::PLAYING){
        checkEnd();
        return false;
    } // if
    if (p.x<0 || p.x>2 || p.y<0 || p.y>2) return false;

    bool moved = game->moveClick(p);
    if (moved) clue = {-1, -1};
    checkEnd();
    
    return moved;
}
bool ManualMode::hint(){
    game->update();
    if (game->getGameStatus() != GAME_STATUS::PLAYING || game->getRemainingClues() <= 0){
        checkEnd();
        return false;
    } // if
    if (clue.x != -1) return true;

    std::vector<Node> path = obtainPath(game->getBoard(), goal);
    if (path.size() < 2) return false;

    if (!game->useClue()) return false;
    clue = path[1].board.getEmpty();
    return true;
}

bool ManualMode::update(){
    game->update();
    checkEnd();
    return !isFinished();
}
bool ManualMode::isFinished() const{
    GAME_STATUS status = game->getGameStatus();
    return (status == GAME_STATUS::WIN || status == GAME_STATUS::TIME_UP);
}
Board ManualMode::getBoard() const { return game->getBoard(); }

const Game& ManualMode::getGame() const { return *game; }
const Position& ManualMode::getClue() const { return clue; }
const Score& ManualMode::getFinalScore() const { return final_score; }

SmartMode::SmartMode() : start(goal), step(0) {}

bool SmartMode::setStart(const std::vector<int>& values){
    if (!Board::validLayout(values)) return false;
    start = Board(values);
    path.clear();
    step = 0;

    return true;
}
bool SmartMode::setGoal(const std::vector<int>& values){
    if (!Board::validLayout(values)) return false;
    goal = Board(values);
    path.clear();
    step = 0;

    return true;
}
void SmartMode::randomStart(const MOVES_DIFFICULTY& difficulty){
    start = Game::generatePuzzle(goal, difficulty);
    path.clear();
    step = 0;
}
bool SmartMode::isSolvable() const { return start.isSolvableTo(goal); }
bool SmartMode::solve(){
    path.clear();
    step = 0;
    
    if (!isSolvable()) return false;
    path = obtainPath(start, goal);
    
    return !path.empty();
}

bool SmartMode::nextStep(){
    if (path.empty() || step + 1 >= path.size()) return false;
    step++;
    return true;
}
bool SmartMode::previousStep(){
    if (path.empty() || step == 0) return false;
    step--;
    return true;
}
void SmartMode::restartSteps(){ step = 0; }
int SmartMode::getStep() const { return static_cast<int>(step); }
int SmartMode::getTotalSteps() const {
    return path.empty() ? 0 : static_cast<int>(path.size()) - 1;
}
Position SmartMode::getNextTile() const {
    if (path.empty() || step + 1 >= path.size()) return {-1, -1};
    return path[step + 1].board.getEmpty();
}
const std::vector<Node>& SmartMode::getPath() const { return path; }

bool SmartMode::update(){ return !isFinished(); }
bool SmartMode::isFinished() const {
    return !path.empty() && step + 1 >= path.size();
}
Board SmartMode::getBoard() const {
    return path.empty() ? start : path[step].board;
}
