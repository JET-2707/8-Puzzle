#include "algorithms.hpp"

#include <algorithm>
#include <functional>
#include <map>
#include <queue>
#include <utility>
#include <vector>
#include <cmath>
#include <set>

int Heuristic::Manhattan(const Node &current, const std::vector<std::pair<int, int>> &goal_positions){
    int sum = 0;

    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            int value = current.board[i][j];

            if(value == 0){ continue; }

            std::pair<int, int> goal_position = goal_positions[value];
            int goal_x = goal_position.first, 
                goal_y = goal_position.second;

            sum += std::abs(i - goal_x) + std::abs(j - goal_y);                
        }
    }
    return sum;
}

int Graph::distance(Node a, Node b){ return 1; }

std::vector<Node> Graph::makePath(const std::vector<Node> &nodes, int current_index){
    std::vector<Node> path;

    while(current_index != -1){
        Node current = nodes[current_index];

        path.push_back(current);
        current_index = current.parent;
    }
    std::reverse(path.begin(), path.end());
    return path;
}

std::vector<Node> Graph::getNeighbors_(const Node &current){
    std::vector<Node> result; 
    const int dx[] = {1, -1, 0, 0};
    const int dy[] = {0, 0, 1, -1};
    int x, y;

    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if(current.board[i][j] == 0){
                x = i;
                y = j;
                break;
            }
        }
    }

    for(int i = 0; i < 4; i++){
        int nx = x + dx[i], 
            ny = y + dy[i];

        if(nx >= 0 && nx < 3 && ny >= 0 && ny < 3){
            Node neighbor = current;

            std::swap(neighbor.board[x][y], neighbor.board[nx][ny]);
            result.push_back(neighbor);
        }
    }
    return result;
}

std::vector<Node> Graph::AStar(Node start, Node goal){
    std::vector<Node> nodes;
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> to_explore;
    std::set<Node> explored;
    std::map<std::vector<std::vector<int>>, int> best_g;
    Heuristic h;

    start.g = 0;
    start.h = h.Manhattan(start, goal_positions);
    start.f = start.g + start.h;
    start.parent = -1;

    nodes.push_back(start);
    to_explore.push({start.f, 0});

    best_g[start.board] = 0;

    while(!to_explore.empty()){
        int current_index = to_explore.top().second;

        to_explore.pop();

        Node current = nodes[current_index];

        if(current == goal){ return makePath(nodes, current_index); }
        if(explored.count(current)){ continue; }

        explored.insert(current);

        for(auto &neighbor : getNeighbors_(current)){
            if(explored.count(neighbor)){ continue; }

            int tentative_g = current.g + distance(current, neighbor);

            auto it = best_g.find(neighbor.board);
            if(it != best_g.end() && tentative_g >= it->second){ continue; }

            neighbor.g = tentative_g;
            neighbor.h = h.Manhattan(neighbor, goal_positions);
            neighbor.f = neighbor.g + neighbor.h;
            neighbor.parent = current_index;

            int neighbor_index = nodes.size();

            nodes.push_back(neighbor);
            best_g[neighbor.board] = tentative_g;
            to_explore.push({neighbor.f, neighbor_index});
        }
    }
    return {};
}

// --- MADE WITH AI TO QUICKLY TASTE THE ALGORITHM ---
// #include <iostream>
//
// void printBoard(const std::vector<std::vector<int>>& board) {
//     std::cout << "-------------\n";
//     for (int i = 0; i < 3; ++i) {
//         std::cout << "| ";
//         for (int j = 0; j < 3; ++j) {
//             if (board[i][j] == 0) {
//                 std::cout << "  | ";
//             } else {
//                 std::cout << board[i][j] << " | ";
//             }
//         }
//         std::cout << "\n-------------\n";
//     }
// }
//
// int main() {
//     Node start;
//     start.board = {
//         {4, 1, 3},
//         {7, 2, 6},
//         {0, 5, 8}
//     };
//
//     Node goal;
//     goal.board = {
//         {1, 2, 3},
//         {4, 5, 6},
//         {7, 8, 0}
//     };
//
//     std::cout << "=== ESTADO INICIAL ===" << std::endl;
//     printBoard(start.board);
//
//     std::cout << "\n=== ESTADO OBJETIVO ===" << std::endl;
//     printBoard(goal.board);
//
//     Graph solver;
//     std::cout << "\nCalculando solucion con A* (Version Indices)...\n" << std::endl;
//
//     std::vector<Node> path = solver.AStar(start, goal);
//
//     if (path.empty()) {
//         std::cout << "No se encontro ninguna solucion (o el estado inicial es el objetivo)." << std::endl;
//     } else {
//         std::cout << "¡Solucion encontrada en " << path.size() - 1 << " movimientos!\n" << std::endl;
//
//         for (size_t i = 0; i < path.size(); ++i) {
//             std::cout << "Paso " << i << ":" << std::endl;
//             printBoard(path[i].board);
//             std::cout << std::endl;
//         }
//     }
//
//     return 0;
// }
