#include <vector>
#include <set>

struct Node {
    std::vector<std::vector<int>> board;
    int h, g, f;
    Node *parent;

    bool operator==(const Node &other) const {
        for(int i = 0; i < board.size(); i++){
            if(this->board[i] != other.board[i]){ return false; }
        }
        return true;
    }

    bool operator<(const Node &other) const { return board < other.board; }
};

struct Comparator {
    bool operator()(const Node &a, const Node &b) const { return a.f > b.f; }
};

class Heuristic {
    public:
        int Manhattan(const Node &current, const std::vector<std::pair<int, int>> &goal_positions);
};
class Graph {
    private:
        std::vector<std::pair<int,int>> goal_positions = {{2, 2}, {0, 0}, {0, 1}, {0, 2}, {1, 0}, {1, 1}, {1, 2}, {2, 0}, {2, 1}};
        std::vector<std::pair<int, int>> neighbors;
    public:
        int distance(Node a, Node b);
        std::vector<Node> makePath(Node *current);
        std::vector<Node> getNeighbors_(const Node &current);
        bool Explored(const Node &node, std::set<Node> explored);
        std::vector<Node> AStar(Node start, Node goal);
};
