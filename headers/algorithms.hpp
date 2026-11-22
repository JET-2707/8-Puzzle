#pragma once

#include <vector>
#include "board.hpp"

struct Node {
    Board board;
    int h, g, f;
    int parent;

    bool operator==(const Node &other) const { return board == other.board; }
    bool operator<(const Node &other) const { return board < other.board; }
};

class Heuristic {
    public:
        static int Manhattan(const Node &current, const std::vector<std::pair<int, int>> &goal_positions);
};

class Graph {
    private:
        std::vector<std::pair<int,int>> goal_positions = {{2, 2}, {0, 0}, {0, 1}, {0, 2}, {1, 0}, {1, 1}, {1, 2}, {2, 0}, {2, 1}};
        std::vector<std::pair<int, int>> neighbors;

        std::vector<std::pair<int, int>> makeGoalPositions(const Board &goal) const;
    public:
        int distance(Node a, Node b);
        std::vector<Node> makePath(const std::vector<Node> &nodes, int current_index);
        std::vector<Node> getNeighbors(const Node &current);
        std::vector<Node> AStar(Node start, Node goal);
};
