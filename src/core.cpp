#include "core.hpp"
#include "board.hpp"
#include <algorithm>
#include <chrono>
#include <math.h>
#include <random>

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

Level Game::makeLevel(DIFFICULTY diff, int optimal){
        if (diff == DIFFICULTY::EASY) return {DIFFICULTY::EASY, Time(0), 5, 1};
        return {DIFFICULTY::NO_EASY, Time(optimal * SECONDS_PER_MOVE), 3, 2};
    }
bool Game::moveClick(const Position& p){ 
    if (game_status != GAME_STATUS::PLAYING) return false;
    if (board.move(p)){
        movements++;
        won();
        return true;
    } // if

    return false;
}
bool Game::update(){
    if (game_status != GAME_STATUS::PLAYING) return false;
    if (level.limit_time.getSeconds() > 0 && playtime.elapsed() >= level.limit_time){
        game_status = GAME_STATUS::TIME_UP;
        playtime.pause();
        return false;
    } // if

    return true;
}
void Game::won(){ 
    if (board == goal){
         game_status = GAME_STATUS::WIN;
         playtime.pause();
    } // if
}
bool Game::useClue(){
    if (game_status == GAME_STATUS::PLAYING && remaining_clues>0){
        remaining_clues--;
        return true;
    } // if
    return false;
}

Level Game::getLevel() const { return level; }

Board Game::getBoard() const { return board; }

Board Game::getGoal() const { return goal; }

Stopwatch Game::getPlaytime() const { return playtime; }

GAME_STATUS Game::getGameStatus() const { return game_status; }

int Game::getRemainingClues() const { return remaining_clues; }

int Game::getMovements() const { return movements; }

int Game::getOptimalMovements() const { return optimal_movements; }

Board Game::generatePuzzle(const Board &goal_board, const MOVES_DIFFICULTY &difficulty){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(0, 3);

    Board board(goal_board);
    const int dx[] = {1, -1, 0, 0};
    const int dy[] = {0, 0, 1, -1};
    int last_direction = -1;

    for(int i = 0; i < static_cast<int>(difficulty); i++){
        int random_number = distribution(gen);

        if((random_number == 0 && last_direction == 1) || 
           (random_number == 1 && last_direction == 0) || 
           (random_number == 2 && last_direction == 3) || 
           (random_number == 3 && last_direction == 2)){
            i--;
            continue;
        }

        Position empty_position = board.getEmpty();
        int nx = empty_position.x + dx[random_number], 
            ny = empty_position.y + dy[random_number];

        if(nx >= 0 && nx < 3 && ny >= 0 && ny < 3){
            board.move({nx, ny});
            last_direction = random_number;
        } else { i--; }
    }
    return board; 
}

std::string Game::calculateDate(){
    auto current = std::chrono::system_clock::now();
    std::time_t time_ = std::chrono::system_clock::to_time_t(current);
    std::tm* local = std::localtime(&time_);

    std::string d = std::to_string(local->tm_mday) + "/" + std::to_string(local->tm_mon + 1) + "/" + std::to_string(local->tm_year + 1900);
    return d;
}

Score::Score(int points) : points(points){}

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

void Score::sortScores(std::vector<Score> &best_scores){
    std::sort(best_scores.begin(), best_scores.end(), [](const Score &a, const Score &b){ return a.points > b.points; });
}
