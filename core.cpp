#include "core.hpp"
#include <chrono>
#include <math.h>
#include <random>
#include <utility>
#include <vector>

void Time::setSeconds(int secs) { seconds = secs; }
int Time::getSeconds() const { return seconds; }

std::string Time::timeText() const {
    int mins, secs;
    std::string str_time;
    
    mins = seconds/60;
    secs = seconds%60;
    
    if (secs < 10)
        str_time = std::to_string(mins) + ":0" + std::to_string(secs);
    else
        str_time = std::to_string(mins) + ":" + std::to_string(secs);

    return str_time;
}
Time Time::operator-(const Time& t) const { 
    int res = seconds - t.seconds;
    if (res < 0) res = 0; // negative values will always be treated as 0
    return Time (res);
}
bool Time::operator <(const Time& t) const { return (seconds < t.seconds); }
bool Time::operator >=(const Time& t) const { return (seconds >= t.seconds); }

void Stopwatch::start(){
    accumulated = std::chrono::steady_clock::duration::zero();
    time_point = std::chrono::steady_clock::now();
    running = true;
}
Time Stopwatch::elapsed() const{
    auto total = accumulated;
    if (running)
        total += std::chrono::steady_clock::now() - time_point;
    return Time(static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(total).count()));
}
void Stopwatch::pause(){
    if (running){
        accumulated += std::chrono::steady_clock::now() - time_point;
        running = false;
    } // if
}
void Stopwatch::resume(){
    if (!running){
        time_point = std::chrono::steady_clock::now();
        running = true;
    }
}

bool Game::moveClick(const Position& p){ 
    if (board.move(p)){
        movements++;
        won();
        return true;
    } // if

    return false;
}
void Game::won(){ 
    if (board == goal){
         game_status = GAME_STATUS::WIN;
         playtime.pause();
    } // if
}
void Game::useClue(const Position& p){
    if (remaining_clues>0 && board.move(p)){
        remaining_clues--;
        movements++;
        won();
    } // if
}

Level Game::getLevel() const { return level; }

Board Game::getBoard() const { return board; }

Board Game::getGoal() const { return goal; }

Stopwatch Game::getPlaytime() const { return playtime; }

GAME_STATUS Game::getGameStatus() const { return game_status; }

int Game::getRemainingClues() const { return remaining_clues; }

int Game::getMovements() const { return movements; }

int Game::getOptimalMovements() const { return optimal_movements; }

std::vector<std::vector<int>> Game::generatePuzzle(const std::vector<std::vector<int>> &goal_board, const MOVES_DIFFICULTY &difficulty){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(0, 3);

    std::vector<std::vector<int>> board = goal_board;
    int x = -1, y = -1;

    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if(board[i][j] == 0){
                x = i;
                y = j;
                break;
            }
        }
        if(x != -1){ break; }
    }

    const int dx[] = {1, -1, 0, 0};
    const int dy[] = {0, 0, 1, -1};
    int last_direction = -1;

    for(int i = 0; i < static_cast<int>(difficulty); i++){
        int random_number = distribution(gen);

        if((random_number == 0 && last_direction == 1) || 
           (random_number == 1 && last_direction == 0) || 
           (random_number == 2 && last_direction == 3) || 
           (random_number == 3 && last_direction == 2)){
           continue;
        }

        int nx = x + dx[random_number], 
            ny = y + dy[random_number];

        if(nx >= 0 && nx < 3 && ny >= 0 && ny < 3){
            std::swap(board[x][y], board[nx][ny]);
            x = nx;
            y = ny;
        }
    }
    return board; 
}

int Score::getPoints() const { return points; }

void Score::calculatePoints(const Game& g){
    constexpr double BASE = 1000.0;
    constexpr double PENALTY = 50.00;
    constexpr double MOV_WEIGHT = 0.7;
    constexpr double TIME_WEIGHT = 0.3;

    if (g.getGameStatus() != GAME_STATUS::WIN){
        points = 0;
        return;
    } // if
    
    int movs = g.getMovements();
    int optimal = g.getOptimalMovements();
    if (movs<=0 || optimal < 0){
        points = 0;
        return;
    } // if

    double movs_factor = static_cast<double>(optimal)/movs;
    if (movs_factor > 1.0) movs_factor = 1.0;

    int used_clues = g.getLevel().clues - g.getRemainingClues();
    double weighted_factor;
    if (g.getLevel().difficulty == DIFFICULTY::EASY) weighted_factor = movs_factor;
    else {
        int limit = g.getLevel().limit_time.getSeconds();
        int elapsed = g.getPlaytime().elapsed().getSeconds();
        double time_factor = 0.0;
        
        if (limit > 0) time_factor = 1.0 - static_cast<double>(elapsed) / limit;
        if (time_factor < 0.0) time_factor = 0.0;
        if (time_factor > 1.0) time_factor = 1.0;
        weighted_factor = MOV_WEIGHT*movs_factor + TIME_WEIGHT*time_factor;
    } // if

    double result = BASE * g.getLevel().multiplier * weighted_factor - used_clues * PENALTY;
    if (result < 0.0) result = 0.0;

    points = static_cast<int>(std::round(result));
}

/*
#include <iostream>
void Stopwatch::setElapsed(const Time& t){
    accumulated = std::chrono::seconds(t.getSeconds());
    running = false;
}
void Game::setMovements(int movs) { movements = movs; }
void Game::setRemainingClues(int r_clues) { remaining_clues = r_clues; }
void Game::setElapsedTime(const Time& e_time){ playtime.setElapsed(e_time); }

int main(){
    Level easy = {DIFFICULTY::EASY, Time(0), 5, 1};
    Level dif = {DIFFICULTY::NO_EASY, Time(120), 3, 2};
    Game g_e(easy, Board(), Board(), 14); 
    Game g_d(dif, Board(), Board(), 14); 
    Score s_e, s_d;

    g_e.setMovements(14); g_e.setRemainingClues(5); g_e.setElapsedTime(Time(60));
    g_d.setMovements(20); g_d.setRemainingClues(2); g_d.setElapsedTime(Time(60));

    g_e.won();
    g_d.won();

    s_e.calculatePoints(g_e);
    s_d.calculatePoints(g_d);

    std::cout << std::endl;
    std::cout << "Game: EASY, Optimal moves: 14, Moves: 14, Used clues: 0" << std::endl
              << "\tPoints: " << s_e.getPoints() << std::endl;
    std::cout << std::endl;
    std::cout << "Game: DIFICULT, Optimal moves: 14, Moves: 20, Used clues: 1, Limit time: 120 secs, Time: 60 secs" << std::endl
              << "\tPoints: " << s_d.getPoints() << std::endl;
    std::cout << std::endl;

    return 0;
}
*/
