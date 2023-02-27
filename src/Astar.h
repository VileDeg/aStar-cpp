#pragma once

#include <vector>
#include "types.h"

class Astar {
public:
    Astar(const std::string& filename, Vec2 dim);
    void Init();
    void Print();

    void FindPath();
private:
    std::string _filename;
    Vec2 _dim;
    std::vector<std::vector<Node>> _grid;
    // std::vector<Node*> open;
    // std::vector<Node*> closed;
    // std::vector<Node*> path;
    // Node* start;
    // Node* end;
};