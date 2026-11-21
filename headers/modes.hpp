#pragma once

#include "board.hpp"
#include "algorithms.hpp"
#include "core.hpp"
#include <cstddef>
#include <memory>
#include <vector>

class Mode {
protected:
    Graph solver;    
    Board goal = Board({1, 2, 3, 4, 5, 6, 7, 8, 0});

    std::vector<Node> obtainPath(const Board&, const Board&);
public:
    virtual ~Mode() = default;

    virtual bool update() = 0;
    virtual bool isFinished() const = 0;
    virtual Board getBoard() const = 0;
    virtual bool click(const Position&);

    const Board& getGoal() const { return goal; }
};

class ManualMode : public Mode {
private:
    std::unique_ptr<Game> game;
    DIFFICULTY difficulty;
    Position clue;
    Score final_score;
    bool score_ready;

    void checkEnd();
public:
    explicit ManualMode(DIFFICULTY);

    void newGame(DIFFICULTY);
    bool click(const Position&) override;
    bool hint();

    bool update() override;
    bool isFinished() const override;
    Board getBoard() const override;

    const Game& getGame() const;
    const Position& getClue() const;
    const Score& getFinalScore() const;
};

class SmartMode : public Mode {
private:
    Board start;
    std::vector<Node> path;
    std::size_t step;
public:
    SmartMode();

    bool setStart(const std::vector<int>& values);
    bool setGoal(const std::vector<int>& values);
    void randomStart(const MOVES_DIFFICULTY& difficulty);
    bool isSolvable() const;
    bool solve();

    bool nextStep();
    bool previousStep();
    void restartSteps();
    int getStep() const;
    int getTotalSteps() const;
    Position getNextTile() const;
    const std::vector<Node>& getPath() const;

    bool update() override;
    bool isFinished() const override;
    Board getBoard() const override;
};
