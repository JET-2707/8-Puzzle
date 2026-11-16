#pragma once

#include <vector>

struct Position{
    int x;
    int y;
};

class Board{
private:
    std::vector<std::vector<int>> board{3, std::vector<int>(3, 0)};
    Position empty;
public:
    Board();
    Board(const std::vector<int> &numbers);
    Board(const Board&); // copy

    Position getEmpty() const;
    int getTile(const Position&) const;
    Position getTile(int) const;

    bool move(const Position&);
    void show() const; // temp

    bool operator ==(const Board&) const;
    bool operator <(const Board&) const;
    std::vector<int> &operator[](int index);
    const std::vector<int> &operator[](int index) const;
};
