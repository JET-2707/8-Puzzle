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

std::vector<DIRECTION> Board::posibleMove() const {
    std::vector<DIRECTION> posible_dir;
    
    if (empty.x > 0) posible_dir.push_back(DIRECTION::UP);
    if (empty.x < 2) posible_dir.push_back(DIRECTION::DOWN);
    if (empty.y > 0) posible_dir.push_back(DIRECTION::LEFT);
    if (empty.y < 2) posible_dir.push_back(DIRECTION::RIGHT);

    return posible_dir;
}
bool Board::move(DIRECTION dir){
    std::vector<DIRECTION> p_moves = posibleMove();
    if (std::find(p_moves.begin(), p_moves.end(), dir) == p_moves.end()) return false;

    int x = empty.x, 
        y = empty.y;
    switch(dir){
        case DIRECTION::UP: 
            matrix[x][y] = matrix[x-1][y];
            matrix[x-1][y] = 0;
            empty.x--;
            break;    
        case DIRECTION::DOWN:
            matrix[x][y] = matrix[x+1][y];
            matrix[x+1][y] = 0;
            empty.x++;
            break;    
        case DIRECTION::LEFT:
            matrix[x][y] = matrix[x][y-1];
            matrix[x][y-1] = 0;
            empty.y--;
            break;    
        case DIRECTION::RIGHT:
            matrix[x][y] = matrix[x][y+1];
            matrix[x][y+1] = 0;
            empty.y++;
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

/*
#include <iostream>
void Board::show() const {
    for (int i=0; i<3; i++){
        std:: cout << "    ";
        for (int j=0; j<3; j++)
            std::cout << matrix[i][j] << "  ";
        std::cout << std::endl;
    } // for
    std::cout << std::endl;
}

int main(){
    int nums[9] = {1, 0, 2, 3, 4, 5, 6, 7, 8};
    Board b(nums);
    
    std::cout << std::endl; b.show();
    if (b.move(DIRECTION::LEFT)) b.show();
    if (b.move(DIRECTION::UP)) b.show();
    if (b.move(DIRECTION::DOWN)) b.show();
    if (b.move(DIRECTION::LEFT)) b.show();
    if (b.move(DIRECTION::RIGHT)) b.show();

    return 0;
}
*/