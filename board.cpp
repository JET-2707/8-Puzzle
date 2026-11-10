#include "board.hpp"
#include <vector>
#include <algorithm>

Board::Board(){
    int x = 0;
    for (int i=0; i<3; i++)
        for (int j=0; j<3; j++)
            matrix[i][j] = ++x;
    matrix[2][2] = 0;
    empty.x = 2;
    empty.y = 2;
}
Board::Board(const int numbers[9]){
    int x = 0;
    empty.x = -1;
    empty.y = -1;

    for (int i=0; i<3; i++){
        for (int j=0; j<3; j++){
            if (numbers[x] == 0){
                empty.x = i;
                empty.y = j;
            } // if
            matrix[i][j] = numbers[x++];
        } // for
    } // for
}
Board::Board(const Board& b){
    for (int i=0; i<3; i++)
        for (int j=0; j<3; j++)
            matrix[i][j] = b.matrix[i][j];
    empty.x = b.empty.x;
    empty.y = b.empty.y;
}

Position Board::getEmpty() const { return empty; }
int Board::getTile(const Position& p) const { return matrix[p.x][p.y]; }
Position Board::getTile(int num) const {
    for (int i=0; i<3; i++)
        for (int j=0; j<3; j++)
            if (matrix[i][j] == num) return {i, j};
    return {-1, -1};
}

std::vector<DIRECTION> Board::posible_move() const {
    std::vector<DIRECTION> posible_dir;
    
    if (empty.x > 0) posible_dir.push_back(UP);
    if (empty.x < 2) posible_dir.push_back(DOWN);
    if (empty.y > 0) posible_dir.push_back(LEFT);
    if (empty.y < 2) posible_dir.push_back(RIGHT);

    return posible_dir;
}
bool Board::move(DIRECTION dir){
    std::vector<DIRECTION> p_moves = posible_move();
    if (std::find(p_moves.begin(), p_moves.end(), dir) == p_moves.end()) return false;

    int x = empty.x, 
        y = empty.y;
    switch(dir){
        case UP: 
            matrix[x][y] = matrix[x-1][y];
            matrix[x-1][y] = 0;
            empty.x = x--;
            break;    
        case DOWN:
            matrix[x][y] = matrix[x+1][y];
            matrix[x+1][y] = 0;
            empty.x = x++;
            break;    
        case LEFT:
            matrix[x][y] = matrix[x][y-1];
            matrix[x][y-1] = 0;
            empty.y = y--;
            break;    
        case RIGHT:
            matrix[x][y] = matrix[x][y+1];
            matrix[x][y+1] = 0;
            empty.y = y++;
            break;
    } // switch

    return true;
}

bool Board::operator==(const Board& b) const {
    for (int i=0; i<3; i++)
        for (int j=0; j<3; j++)
            if (matrix[i][j] != b.matrix[i][j]) return false;
    return true;
}
bool Board::operator<(const Board& b) const {
    for (int i=0; i<3; i++){
        for (int j=0; j<3; j++){
            if (matrix[i][j] < b.matrix[i][j]) return true;
            if (matrix[i][j] > b.matrix[i][j]) return false;
        } // for
    } // for
    return false;
}