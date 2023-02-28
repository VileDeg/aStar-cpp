#include "Astar.h"

#include <iostream>
#include <fstream>
#include <array>
#include <sstream>
#include <algorithm>

#include "color.h"

Astar::Astar(const std::string& filename, Vec2 dim, Vec2 start, Vec2 end) 
    : _filename(filename), _dim(dim), _start(start), _end(end) {}

void Astar::Init() {
    std::ifstream ifs(_filename);
    if (!ifs) {
        throw std::runtime_error("Error opening file: " + _filename);
    }

    _grid.resize(_dim.x);
    for (std::size_t i = 0; i < _dim.x; ++i) {
        _grid[i].resize(_dim.y);
    }

    //_grid.reserve(_dim.x);
    /*for (std::size_t i = 0; i < _dim.x; ++i) {
        _grid.push_back(std::vector<Node>(_dim.y));
    }*/
    //Read file line by line, and split each line in by comma
    std::size_t ir = 0;
    std::string line, word;
    while (std::getline(ifs, line)) {
        std::istringstream iss(line);
        
        //Create row
        //_grid.push_back(std::vector<Node>());
        //_grid.resize(_grid.size() + 1);
        //Then take a reference to it
        //std::vector<Node>& row = _grid[ir];
        
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
        color("yellow", " " + std::to_string(i) + " ", false);
    }
    std::cout << "  " << std::endl << "---";
    for (size_t i = 0; i < _dim.y; ++i) {
        std::cout << "---";
    }
    std::cout << std::endl;
    size_t r = 0;
    for (auto& row : _grid) {
        color("yellow", std::to_string(r), false);
        std::cout << " |";
        for (auto& node : row) {
            std::cout << " ";
            if (node.pos == _start) {
                color("green", std::to_string(node.val), false);
            } else if (node.pos == _end) {
                color("red", std::to_string(node.val), false);
            } else if (node.walkable) {
                std::cout << node.val;
            } else {
                color("cyan", "Z", false);
            }
            std::cout << " ";
        }
        std::cout << std::endl << "  |" << std::endl;
        ++r;
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

void Astar::FindPath() {
    std::vector<Node*> open{}, closed{};
    open.push_back(_startNode);

    size_t iteration = 0;

    size_t maxIterations = 1000;
    
    while (true) {
        if (iteration > maxIterations) {
            std::cout << "Max iterations reached. Path not found." << std::endl;
            break;
        }
        std::cout << "Iteration: ";
        color("cyan", std::to_string(iteration), false);
        std::cout << std::endl;
        //Print open and closed lists
        color("green", "\tOpen: ", false);
        //std::cout << "\tOpen: ";
        for (auto& node : open) {
            std::cout << *node;
            color("yellow", "; ", false);
        }
        std::cout << std::endl;
        color("red", "\tClosed: ", false);
        //std::cout << "\tClosed: ";
        for (auto& node : closed) {
            std::cout << *node;
            color("yellow", "; ", false);
        }
        std::cout << std::endl;

        //Find node with lowest F cost in open. The code:
        auto it = std::min_element(open.begin(), open.end(), [](const Node* a, const Node* b) { return a->f < b->f; });
        if (it == open.end()) {
            //No path found
            std::cout << "Open list is empty. Path not found. Iteration: " << iteration << std::endl;
            break;
        }
        Node* curr = *it;
        //Remove curr from open
        open.erase(it);
        //Add curr to closed
        closed.push_back(curr);
        //If curr is the end node, we are done
        if (*curr == *_endNode) {
            std::cout << "Path found. Iteration: " << iteration << std::endl;
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
            if (dstToNb < nb->g || std::find(open.begin(), open.end(), nb) == open.end()) {
                //Set g to new path
                nb->g = dstToNb;
                //Set h to distance from neighbor to end
                nb->h = nb->pos.distance(_endNode->pos);
                //Set f to g + h
                nb->f = nb->g + nb->h;
                //Set parent to curr
                nb->parent = curr;
                //If neighbor is not in open
                if (std::find(open.begin(), open.end(), nb) == open.end()) {
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
    std::cout << "Path(start to end): " << std::endl;
    for (auto& node : path) {
        std::cout << "[" << node->pos << "]" << std::endl;
    }
}

void Astar::Print() {
    std::cout << "Dimensions: " << _dim.x << ", " << _dim.y << std::endl;
    for (auto& row : _grid) {
        for (auto& node : row) {
            std::cout << node << std::endl;
        }
    }
}