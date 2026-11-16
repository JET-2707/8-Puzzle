#pragma once

#include "board.hpp"
#include <string>
#include <chrono>

enum class DIFFICULTY {EASY, NO_EASY};
enum class MOVES_DIFFICULTY { EASY = 8, NO_EASY = 16 };
enum class GAME_STATUS {PLAYING, WIN, TIME_UP};

class Time {
private:
    int seconds;
public:
    Time() : seconds(0) {}
    explicit Time(int secs) : seconds(secs) {}
    
    void setSeconds(int);
    int getSeconds() const;

    std::string timeText() const;
    Time operator -(const Time&) const;
    bool operator <(const Time&) const;
    bool operator >=(const Time&) const;
    
};

class Stopwatch {
private:
    std::chrono::time_point<std::chrono::steady_clock> time_point;
    std::chrono::steady_clock::duration accumulated = std::chrono::steady_clock::duration::zero();
    bool running = false;
public:
    void start();
    Time elapsed() const;
    void pause();
    void resume();
    // TEMP: only test / void setElapsed(const Time&);
};

struct Level {
    DIFFICULTY difficulty;
    Time limit_time;
    int clues,
        multiplier;
};

class Game {
private:
    Level level;
    Board board,
          goal;
    Stopwatch playtime;
    GAME_STATUS game_status;
    int remaining_clues,
        movements,
        optimal_movements;
public:    
    Game(const Level& l, const Board& b, const Board& g, int opt_mov) : level(l), board(b), goal(g), game_status(GAME_STATUS::PLAYING), remaining_clues(l.clues), movements(0), optimal_movements(opt_mov) { playtime.start(); }

    Level getLevel() const;
    Board getBoard() const;
    Board getGoal() const;
    Stopwatch getPlaytime() const;
    GAME_STATUS getGameStatus() const;
    int getRemainingClues() const;
    int getMovements() const;
    int getOptimalMovements() const;

    static Board generatePuzzle(const Board &goal_board, const MOVES_DIFFICULTY &difficulty);

    // TEMP: only test / void setMovements(int);
    // TEMP: only test / void setRemainingClues(int);
    // TEMP: only test / void setElapsedTime(const Time&);

    bool moveClick(const Position&);
    void won();
    void useClue(const Position& p);
};

class Score {
private:
    int points;
public:
    Score() : points(0) {}

    int getPoints() const;
    void calculatePoints(const Game&);
};
