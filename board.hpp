#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>

enum DIRECTION { UP, DOWN, LEFT, RIGHT };

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
    Board(const Board&); // copia

    Position getEmpty() const;
    int getTile(const Position&) const;
    Position getTile(int) const;

    std::vector<DIRECTION> posible_move() const;
    bool move(DIRECTION);

    bool operator ==(const Board&) const;
    bool operator <(const Board&) const;
};

#endif