#include "Astar.h"

#include <iostream>
#include <fstream>
#include <array>
#include <sstream>

Astar::Astar(const std::string& filename, Vec2 dim) : _filename(filename), _dim(dim) {}

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
        std::vector<Node> row;
        
        std::size_t pos = 0;
        std::string token;
        std::size_t ic = 0;
        while ((pos = line.find(',')) != std::string::npos) {
            token = line.substr(0, pos);
            Node node{};
            try {
                node.fromStr(token);
            } catch (const std::exception& e) {
                throw std::runtime_error("Error parsing Node: " + std::string(e.what()));
            }
            row.push_back(node);
            line.erase(0, pos + 1);

            ++ic;
        }
        if (ic != _dim.y) {
            throw std::runtime_error("Invalid dimensions");
        }

        _grid.push_back(row);
        ++ir;
    }

    if (ir != _dim.x)
    {
        throw std::runtime_error("Invalid dimensions");
    }
}

void Astar::FindPath() {

}

void Astar::Print() {
    std::cout << "Dimensions: " << _dim.x << ", " << _dim.y << std::endl;
    for (auto& row : _grid) {
        for (auto& node : row) {
            node.print();
        }
    }
}