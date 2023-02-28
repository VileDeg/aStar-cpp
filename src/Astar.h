#pragma once

#include <vector>
#include "types.h"

class Astar {
public:
    Astar(const std::string& filename, Vec2 dim, Vec2 start, Vec2 end);
    ~Astar();

    void Init();
    void Print();
    void Show();

    void FindPath();
    void PrintPath();
private:
    void fillNeighbors();
private:
    std::string _filename;
    Vec2 _dim, _start, _end;
    //std::vector<std::vector<Node>> _grid;
    Node** _grid;
    Node* _startNode;
    Node* _endNode;
};