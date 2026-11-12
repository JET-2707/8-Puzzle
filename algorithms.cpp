#include "algorithms.hpp"
#include <algorithm>
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

std::vector<Node> Graph::makePath(Node *current){
    std::vector<Node> path;

    while(current != nullptr){
        path.push_back(*current);
        current = current->parent;
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
    std::priority_queue<Node, std::vector<Node>, Comparator> to_explore;
    std::set<Node> explored;
    std::map<std::vector<std::vector<int>>, float> best_g;
    Heuristic h;

    start.g = 0;
    start.h = h.Manhattan(start, goal_positions);
    start.f = start.g + start.h;
    start.parent = nullptr;
    to_explore.push(start);

    best_g[start.board] = 0;

    while(!to_explore.empty()){
        Node current = to_explore.top();

        if(current == goal){ return makePath(&current); }

        to_explore.pop();
        explored.insert(current);

        for(auto &neighbor : getNeighbors_(current)){
            // if(Explored(it, explored)){ continue; }
            if(explored.count(neighbor)){ continue; }

            int tentative_g = current.g + distance(current, neighbor);

            auto it = best_g.find(neighbor.board);
            // if(it != best_g.end() && tentative_g >= it.second){ to_explore.push(it); }
            if(it != best_g.end() && tentative_g >= it->second){ continue; }

            best_g[neighbor.board] = tentative_g;
            neighbor.g = tentative_g;
            neighbor.h = h.Manhattan(neighbor, goal_positions);
            neighbor.f = neighbor.g + neighbor.h;
            neighbor.parent = &current;

            to_explore.push(neighbor);
        }
    }
        return {}; // failure
}
