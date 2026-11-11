#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>

struct Position{
    int x;
    int y;
};

class Board{
private:
    int matrix[3][3];
    Position empty;
public:
    Board();
    Board(const int[9]);
    Board(const Board&); // copy

    Position getEmpty() const;
    int getTile(const Position&) const;
    Position getTile(int) const;

    bool move(const Position&);
    void show() const; // temp

    bool operator ==(const Board&) const;
    bool operator <(const Board&) const;
};

#endif