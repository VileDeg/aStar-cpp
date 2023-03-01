#pragma once

#include <vector>
#include "types.h"

class Astar {
public:
    Astar(Args a);
    ~Astar();

    void Init();
    void Print();
    void Show();

    void FindPath();
    void PrintPath();
private:
    void fillNeighbors();

    void printList(const std::vector<Node*>& list);
    void color_print(std::string cl, std::string line, bool newLine = false);
private:
    std::string _filename;
    Vec2 _dim, _start, _end;
    bool _useColor;
    std::vector<Node*> _workOrder;
    Node** _grid;
    Node* _startNode;
    Node* _endNode;
};