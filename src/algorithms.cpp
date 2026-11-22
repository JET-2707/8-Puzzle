#include "algorithms.hpp"
#include "board.hpp"

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

std::vector<Node> Graph::getNeighbors(const Node &current){
    std::vector<Node> result; 
    const int dx[] = {1, -1, 0, 0};
    const int dy[] = {0, 0, 1, -1};
    Position empty_position = current.board.getEmpty();
    int x = empty_position.x, 
        y = empty_position.y;

    for(int i = 0; i < 4; i++){
        int nx = x + dx[i], 
            ny = y + dy[i];

        if(nx >= 0 && nx < 3 && ny >= 0 && ny < 3){
            Node neighbor = current;

            // std::swap(neighbor.board[x][y], neighbor.board[nx][ny]);
            Position target_position = {nx, ny};
            neighbor.board.move(target_position);
            result.push_back(neighbor);
        }
    }
    return result;
}

std::vector<std::pair<int, int>> Graph::makeGoalPositions(const Board &goal) const{
    std::vector<std::pair<int, int>> positions(9);

    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            int value = goal[i][j];
            positions[value] = {i, j};
        }
    }
    return positions;
}

std::vector<Node> Graph::AStar(Node start, Node goal){
    std::vector<Node> nodes;
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> to_explore;
    std::set<Node> explored;
    std::map<Board, int> best_g;

    std::vector<std::pair<int, int>> target_position = makeGoalPositions(goal.board);

    start.g = 0;
    start.h = Heuristic::Manhattan(start, target_position);
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

        for(auto &neighbor : getNeighbors(current)){
            if(explored.count(neighbor)){ continue; }

            int tentative_g = current.g + distance(current, neighbor);

            auto it = best_g.find(neighbor.board);
            if(it != best_g.end() && tentative_g >= it->second){ continue; }

            neighbor.g = tentative_g;
            neighbor.h = Heuristic::Manhattan(neighbor, target_position);
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
