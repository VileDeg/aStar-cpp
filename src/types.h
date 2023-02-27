#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <array>
#include <sstream>

struct Vec2 {
    std::size_t x, y;

    Vec2() = default;
    Vec2(const Vec2& v) : x(v.x), y(v.y) {}
    Vec2(size_t xv, size_t yv) : x(xv), y(yv) {}

    void fromStr(const std::string& str) {
        std::string s1, s2;

        s1 = str.substr(0, str.find(','));
        s2 = str.substr(str.find(',') + 1);

        x = std::stoul(s1);
        y = std::stoul(s2);
    }
};

//class UsageException : public std::exception {
//private:
//    std::string _msg;
//    const char* _usage = "Usage: ./astar <input_file.csv> <R,C> <Y,X> <Y,X>\n"
//    "\tFile to read the grid from, grid dimensions, start point and end point\n"
//    "\tWhere R - number of rows(Y), C - number of columns(X).";
//
//public:
//    UsageException(const std::string& msg) : _msg(msg) {}
//
//    const char * what() const throw () {
//        std::string str = _usage;
//        if (!_msg.empty()) {
//            str = "Error: " + _msg + "\n" + str;
//        }
//        return str.data();
//    }
//};

struct Node {
    bool walkable;
    std::size_t val;
    float g, h, f;
    Node* parent;
    std::vector<Node*> nbs;

    void fromStr(const std::string& str) {
        if (str != "Z") {
            val = std::stoul(str);
            walkable = false;
        } else {
            walkable = true;
        }
    }

    void print() {
        std::cout << "Node: " << val << " walkable: " << walkable << std::endl;
    }
};

struct Args {
    std::string file;
    Vec2 dim, start, end;

    void print() {
        std::cout << "File: " << file << std::endl;
        std::cout << "Dim: " << dim.x << ", " << dim.y << std::endl;
        std::cout << "Start: " << start.x << ", " << start.y << std::endl;
        std::cout << "End: " << end.x << ", " << end.y << std::endl;
    }
};
