#include "Astar.h"

#include <iostream>
#include <fstream>
#include <array>
#include <sstream>
#include <algorithm>
#include <cstring>

#include "color.h"

void Astar::color_print(std::string cl, std::string line, bool newLine)
{
    if (_useColor) {
        color(cl, line, newLine);
    }
    else {
        std::cout << line;
        if (newLine) {
            std::cout << std::endl;
        }
    }
}

Astar::Astar(Args a)
    : _filename(a.file), _dim(a.dim), _start(a.start), _end(a.end), _useColor(a.useColor) {
    _grid = new Node*[_dim.x];
    memset(_grid, 0, sizeof(Node*) * _dim.x);
    for (std::size_t i = 0; i < _dim.x; ++i) {
        _grid[i] = new Node[_dim.y];
        memset(_grid[i], 0, sizeof(Node) * _dim.y);
    }
}

Astar::~Astar() {
    for (std::size_t i = 0; i < _dim.x; ++i) {
        delete[] _grid[i];
    }
    delete[] _grid;
}

void Astar::Init() {
    std::ifstream ifs(_filename);
    if (!ifs) {
        throw std::runtime_error("Error opening file: " + _filename);
    }

    //Read file line by line, and split each line in by comma
    std::size_t ir = 0;
    std::string line, word;
    while (std::getline(ifs, line)) {
        std::istringstream iss(line);
        
        std::size_t pos = 0;
        std::string token;
        std::size_t ic = 0;
        while ((pos = line.find(',')) != std::string::npos) {
            token = line.substr(0, pos);
            Node node{};
            try {
                node.fromStr(token);
                node.nbs.resize(8);
                node.pos = Vec2(ir, ic);
            } catch (const std::exception& e) {
                throw std::runtime_error("Error parsing Node: " + std::string(e.what()));
            }

            _grid[ir][ic] = node;

            if (node.pos == _start) {
                _startNode = &_grid[ir][ic];
            } else if (node.pos == _end) {
                _endNode = &_grid[ir][ic];
            }
            
            line.erase(0, pos + 1);
            ++ic;
        }
        if (ic != _dim.y) {
            throw std::runtime_error("Invalid dimensions");
        }
        
        ++ir;
    }

    if (ir != _dim.x) {
        throw std::runtime_error("Invalid dimensions");
    }

    fillNeighbors();
}

void Astar::Show()
{
    std::cout << std::endl;
    std::cout << "Grid: " << _dim.x << " by " << _dim.y << std::endl;
    std::cout << "   ";
    for (size_t i = 0; i < _dim.x; ++i) {
        color_print("yellow", " " + std::to_string(i) + " ", false);
    }
    std::cout << "  " << std::endl << "---";
    for (size_t i = 0; i < _dim.y; ++i) {
        std::cout << "---";
    }
    std::cout << std::endl;
    for (size_t r = 0; r < _dim.x; ++r) {
        color_print("yellow", std::to_string(r), false);
        std::cout << " |";
        for (size_t c = 0; c < _dim.y; ++c) {
            const Node& node = _grid[r][c];
            std::cout << " ";
            if (node.pos == _start) {
                color_print("green", std::to_string(node.val), false);
            } else if (node.pos == _end) {
                color_print("red", std::to_string(node.val), false);
            } else if (node.walkable) {
                std::cout << node.val;
            } else {
                color_print("cyan", "Z", false);
            }
            std::cout << " ";
        }
        std::cout << std::endl << "  |" << std::endl;
    }
}

void Astar::fillNeighbors() {
    for (std::size_t r = 0; r < _dim.x; ++r) {
        for (std::size_t c = 0; c < _dim.y; ++c) {
            Node& curr = _grid[r][c];

            if (r > 0) {
                if (c > 0) {
                    curr.nbs[0] = &_grid[r - 1][c - 1];
                }
                curr.nbs[1] = &_grid[r - 1][c];
                if (c < _dim.y - 1) {
                    curr.nbs[2] = &_grid[r - 1][c + 1];
                }
            }

            if (c > 0) {
                curr.nbs[3] = &_grid[r][c - 1];
            }
            if (c < _dim.y - 1) {
                curr.nbs[4] = &_grid[r][c + 1];
            }

            if (r < _dim.x - 1) {
                if (c > 0) {
                    curr.nbs[5] = &_grid[r + 1][c - 1];
                }
                curr.nbs[6] = &_grid[r + 1][c];
                if (c < _dim.y-1) {
                    curr.nbs[7] = &_grid[r + 1][c + 1];
                }
            }
        }
    }
}

void Astar::printList(const std::vector<Node*>& list) {
    
    std::cout << "[";
    if (!list.empty()) {
        for (size_t i = 0; i < list.size() - 1; ++i) {
            Node* node = list[i];
            if (node != nullptr) {
                std::cout << *node;
            }
            else {
                break;
            }
            color_print("yellow", ", ", false);
        }
        std::cout << *list.back();
    }
    std::cout << "]" << std::endl;
}

void Astar::FindPath() {
    _startNode->g = 0;
    _startNode->h = _startNode->pos.distance(_endNode->pos);
    _startNode->f = _startNode->h + _startNode->g;

    _workOrder.push_back(_startNode);
    std::vector<Node*> open{}, closed{};
    open.push_back(_startNode);



    size_t iteration = 1;

    size_t maxIterations = 1000;
    
    while (true) {
        if (iteration > maxIterations) {
            std::cout << "Max iterations reached. Path not found." << std::endl;
            break;
        }
        
        //Find node with lowest F cost in open. If there are multiple nodes with the same F cost, choose the one with the lowest H cost.
        auto it = std::min_element(open.begin(), open.end(), 
            [](const Node* a, const Node* b) {
                if (a->f == b->f) {
                    return a->h < b->h;
                } else {
                    return a->f < b->f;
                }
            }
        );

        if (it == open.end()) { //No path found
            std::cout << "Open list is empty. Path not found. Iteration: " << iteration << std::endl;
            break;
        }
        Node* curr = *it;

        std::cout << "Iteration: ";
        color_print("cyan", std::to_string(iteration), false);
        std::cout << std::endl;
        color_print("green", "\tOpen: ", false);
        printList(open);
        color_print("red", "\tClosed: ", false);
        printList(closed);

        //Remove curr from open
        open.erase(it);
        //Add curr to closed
        closed.push_back(curr);
        //If curr is the end node, we are done
        if (*curr == *_endNode) {
            color_print("magenta", "Path found in " + std::to_string(iteration) + " iterations.", true);
            return;
        }

        //For each neighbor of curr
        for (std::size_t i = 0; i < curr->nbs.size(); ++i) {
            Node* nb = curr->nbs[i];
            if (!nb) {
                continue;
            }
            //If neighbor is not walkable or neighbor is in closed, skip to next neighbor
            if (!nb->walkable || std::find(closed.begin(), closed.end(), nb) != closed.end()) {
                continue;
            }
            
            float diff = nb->val;
            float dstToNb = curr->g + diff;
            //If new path to neighbor is shorter OR neighbor is not in open
            bool inOpen = std::find(open.begin(), open.end(), nb) != open.end();
            if (dstToNb < nb->g || !inOpen) {
                _workOrder.push_back(nb);
                //Set g to new path
                nb->g = dstToNb;
                //Set h to distance from neighbor to end
                nb->h = nb->pos.distance(_endNode->pos);
                //Set f to g + h
                nb->f = nb->g + nb->h;
                //Set parent to curr
                nb->parent = curr;
                //If neighbor is not in open
                if (!inOpen) {
                    //Add neighbor to open
                    open.push_back(nb);
                }
            }
        }

        ++iteration;
    }
}

void Astar::PrintPath()
{
    std::vector<Node*> path{};
    Node* curr = _endNode;
    while (curr != nullptr) {
        path.push_back(curr);
        curr = curr->parent;
    }
    std::reverse(path.begin(), path.end());
    color_print("yellow", "Path(start to end): ", true);
    std::cout << "\t";
    if (!path.empty()) {
        for (size_t i = 0; i < path.size()-1; ++i) {
            std::cout << "[" << path[i]->pos << "], ";
        }
        std::cout << "[" << path.back()->pos << "]\n";
    }

    size_t size = _workOrder.size();
    size_t ml = std::to_string(size).length();
    
    color_print("yellow", "Nodes work order: ", true);
    
    for (size_t i = 0; i < size; ++i) {
        std::cout << "\t";
        for (size_t j = 0; j < ml - std::to_string(i).length(); ++j) {
            std::cout << "0";
        }
        std::cout << i << ": [" << _workOrder[i]->pos << "] " <<
            _workOrder[i]->g << " " << 
            FPFL(_workOrder[i]->h, 2) << " " << 
            FPFL(_workOrder[i]->f, 2) << 
            std::endl;
    }
}

void Astar::Print() {
    std::cout << "Dimensions: " << _dim.x << ", " << _dim.y << std::endl;
    for (size_t r = 0; r < _dim.x; ++r) {
        for (size_t c = 0; c < _dim.x; ++c) {
            std::cout << _grid[r][c] << std::endl;
        }
    }
}