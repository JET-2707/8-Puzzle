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

/* -- TEST -- (implemented with AI to perform functionality testing)*/
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <vector>
#include "modes.hpp"

void printBoard(const Board& b){
    for (int i = 0; i < 3; i++){
        std::cout << "  ";
        for (int j = 0; j < 3; j++){
            int tile = b.getTile(Position{i, j});
            std::cout << (tile == 0 ? '_' : static_cast<char>('0' + tile)) << ' ';
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

std::string statusText(GAME_STATUS s){
    switch (s){
        case GAME_STATUS::PLAYING: return "PLAYING";
        case GAME_STATUS::WIN:     return "WIN";
        case GAME_STATUS::TIME_UP: return "TIME_UP";
        default:                   return "OTHER";
    }
}

// Plays the optimal solution clicking tile by tile: it must end in WIN with 1000 points
void testManualOptimal(){
    std::cout << "=== MANUAL: optimal play ===\n";
    ManualMode manual(DIFFICULTY::EASY);
    std::cout << "Start board:\n";
    printBoard(manual.getBoard());
    std::cout << "Optimal moves: " << manual.getGame().getOptimalMovements() << "\n";

    // The test solves the board by itself with A* and clicks every step
    Graph solver;
    Node from, to;
    from.board = manual.getBoard();
    to.board = manual.getGoal();
    std::vector<Node> path = solver.AStar(from, to);

    for (std::size_t i = 1; i < path.size(); i++){
        Position tile = path[i].board.getEmpty();   // tile that moves in this step
        if (!manual.click(tile)) std::cout << "ERROR: click rejected at step " << i << "\n";
    }

    std::cout << "Finished: " << manual.isFinished() << " (expected 1)\n";
    std::cout << "Status: " << statusText(manual.getGame().getGameStatus()) << " (expected WIN)\n";
    std::cout << "Moves: " << manual.getGame().getMovements()
              << " (expected " << manual.getGame().getOptimalMovements() << ")\n";
    std::cout << "Points: " << manual.getFinalScore().getPoints() << " (expected 1000)\n\n";
}

void testManualClues(){
    std::cout << "=== MANUAL: clues and invalid clicks ===\n";
    ManualMode manual(DIFFICULTY::EASY);
    std::cout << "Clues at start: " << manual.getGame().getRemainingClues() << " (expected 5)\n";

    manual.hint();
    Position c = manual.getClue();
    std::cout << "Clue tile: (" << c.x << ", " << c.y << ")  clues left: "
              << manual.getGame().getRemainingClues() << " (expected 4)\n";

    manual.hint();   // a clue is already showing, it should not spend another one
    std::cout << "Asking again: " << manual.getGame().getRemainingClues() << " (expected 4)\n";

    std::cout << "Click outside: " << manual.click(Position{-1, 0}) << " " << manual.click(Position{3, 3}) << " (expected 0 0)\n";
    std::cout << "Click on the empty tile: " << manual.click(manual.getBoard().getEmpty()) << " (expected 0)\n";
    std::cout << "Moves so far: " << manual.getGame().getMovements() << " (expected 0)\n";

    std::cout << "Click on the clue: " << manual.click(c) << " (expected 1)\n";
    std::cout << "Moves: " << manual.getGame().getMovements() << " (expected 1)  clue reset: ("
              << manual.getClue().x << ", " << manual.getClue().y << ") (expected -1, -1)\n\n";
}

void testHardLevel(){
    std::cout << "=== MANUAL: hard level ===\n";
    ManualMode hard(DIFFICULTY::NO_EASY);
    const Game& g = hard.getGame();
    std::cout << "Clues: " << g.getRemainingClues() << " (expected 3)\n";
    std::cout << "Optimal: " << g.getOptimalMovements()
              << "  time limit: " << g.getLevel().limit_time.timeText() << " (expected optimal x 10 s)\n\n";
}

// Game with a 2 seconds limit: after 3 seconds it must be TIME_UP and reject everything
void testTimeUp(){
    std::cout << "=== GAME: time up (waits 3 seconds) ===\n";
    Board goal;
    Board start = goal;
    start.move(Position{2, 1});     // one move away, so clicking (2,2) would be valid
    Level level = {DIFFICULTY::NO_EASY, Time(2), 3, 2};
    Game game(level, start, goal, 1);

    std::cout << "update at once: " << game.update() << " (expected 1)\n";
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout << "update after 3 s: " << game.update() << " (expected 0)\n";
    std::cout << "Status: " << statusText(game.getGameStatus()) << " (expected TIME_UP)\n";
    std::cout << "Click after time up: " << game.moveClick(Position{2, 2}) << " (expected 0)\n";
    std::cout << "Clue after time up: " << game.useClue() << " (expected 0)\n\n";
}

void testSmart(){
    std::cout << "=== SMART: random start, step by step ===\n";
    SmartMode smart;
    smart.randomStart(MOVES_DIFFICULTY::EASY);
    std::cout << "solve(): " << smart.solve() << " (expected 1)\n";
    std::cout << "Total steps: " << smart.getTotalSteps() << "\n";

    do {
        std::cout << "Step " << smart.getStep() << ":\n";
        printBoard(smart.getBoard());
    } while (smart.nextStep());

    std::cout << "Finished: " << smart.isFinished() << " (expected 1)\n";
    smart.previousStep();
    std::cout << "Going back one step -> finished: " << smart.isFinished() << " (expected 0)\n\n";

    std::cout << "=== SMART: invalid and unsolvable boards ===\n";
    SmartMode other;
    std::cout << "Repeated number accepted: " << other.setStart({1, 1, 2, 3, 4, 5, 6, 7, 8}) << " (expected 0)\n";
    std::cout << "Valid board accepted: " << other.setStart({2, 1, 3, 4, 5, 6, 7, 8, 0}) << " (expected 1)\n";
    std::cout << "isSolvable: " << other.isSolvable() << " (expected 0)\n";
    std::cout << "solve(): " << other.solve() << " (expected 0, and it must be instant)\n";
}

int main(){
    testManualOptimal();
    testManualClues();
    testHardLevel();
    testTimeUp();
    testSmart();
    return 0;
}