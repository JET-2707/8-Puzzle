#include "board.hpp"
#include <vector>

Board::Board(){
    int x = 0;
    for (int i=0; i < board.size(); i++)
        for (int j=0; j < board[i].size(); j++)
            board[i][j] = ++x;
    board[2][2] = 0;
    empty.x = 2;
    empty.y = 2;
}

Board::Board(const std::vector<int>& numbers){
    int x = 0;
    empty.x = -1;
    empty.y = -1;

    for (int i=0; i < board.size(); i++){
        for (int j=0; j < board[i].size(); j++){
            if (numbers[x] == 0){
                empty.x = i;
                empty.y = j;
            } // if
            board[i][j] = numbers[x++];
        } // for
    } // for
}

Board::Board(const Board& b){
    for (int i=0; i < board.size(); i++)
        for (int j=0; j < board[i].size(); j++)
            board[i][j] = b.board[i][j];
    empty.x = b.empty.x;
    empty.y = b.empty.y;
}

Position Board::getEmpty() const { return empty; }

int Board::getTile(const Position& p) const { 
    if(p.x < 0 || p.x >= 3 || p.y < 0 || p.y >= 3){ return -1; }
    return board[p.x][p.y]; 
}

Position Board::getTile(int num) const {
    for (int i=0; i < board.size(); i++)
        for (int j=0; j < board[i].size(); j++)
            if (board[i][j] == num) return {i, j};
    return {-1, -1};
}

bool Board::move(const Position& pos){
    int x = empty.x, 
        y = empty.y;
    
    if (pos.x == x-1 && pos.y == y){
        board[x][y] = board[x-1][y];
            board[x-1][y] = 0;
            empty.x--;
            return true;
    } // UP
    if (pos.x == x+1 && pos.y == y){
            board[x][y] = board[x+1][y];
            board[x+1][y] = 0;
            empty.x++;
            return true;
    } // DOWN
    if (pos.x == x && pos.y == y-1){
            board[x][y] = board[x][y-1];
            board[x][y-1] = 0;
            empty.y--;
            return true;
    } // LEFT
    if (pos.x == x && pos.y == y+1){
            board[x][y] = board[x][y+1];
            board[x][y+1] = 0;
            empty.y++;
            return true;
    } // RIGHT

    return false;
}

bool Board::operator==(const Board& b) const {
    for (int i=0; i < board.size(); i++)
        for (int j=0; j < board[i].size(); j++)
            if (board[i][j] != b.board[i][j]) return false;
    return true;
}

bool Board::operator<(const Board& b) const {
    for (int i=0; i < board.size(); i++){
        for (int j=0; j < board[i].size(); j++){
            if (board[i][j] < b.board[i][j]) return true;
            if (board[i][j] > b.board[i][j]) return false;
        } // for
    } // for
    return false;
}

std::vector<int> &Board::operator[](int index){ return board[index]; }

const std::vector<int> &Board::operator[](int index) const{ return board[index]; }

/*
#include <iostream>
void Board::show() const {
    for (int i=0; i < board.size(); i++){
        std:: cout << "    ";
        for (int j=0; j < board[i].size(); j++)
            std::cout << board[i][j] << "  ";
        std::cout << std::endl;
    } // for
    std::cout << std::endl;
}

int main(){
    int nums[9] = {1, 0, 2, 3, 4, 5, 6, 7, 8};
    Board b(nums);

    std::cout << std::endl; b.show();
    if (b.move(b.getTile(1))) b.show();
    if (b.move(b.getTile(3))) b.show();
    if (b.move(b.getTile(8))) b.show();
    if (b.move(b.getTile(5))) b.show();
    if (b.move(b.getTile(4))) b.show();

    return 0;
}
*/
