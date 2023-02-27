#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <array>
#include <sstream>
#include <cmath>

struct Vec2 {
    std::size_t x, y;

    Vec2() = default;
    Vec2(const Vec2& v) : x(v.x), y(v.y) {}
    Vec2(size_t xv, size_t yv) : x(xv), y(yv) {}

    float distance(const Vec2& v) {
        return std::sqrt(std::pow(x - v.x, 2) + std::pow(y - v.y, 2));
    }

    void fromStr(const std::string& str) {
        std::string s1, s2;

        s1 = str.substr(0, str.find(','));
        s2 = str.substr(str.find(',') + 1);

        x = std::stoul(s1);
        y = std::stoul(s2);
    }

    bool operator==(const Vec2& v) {
        return x == v.x && y == v.y;
    }

    //Coords printed in reverse order!!!
    friend std::ostream& operator<< (std::ostream& os, const Vec2& v) {
        os << v.y << ", " << v.x;
        return os;
    }
};

struct Node {
    bool walkable;
    std::size_t val;
    Vec2 pos;
    float g, h, f;
    Node* parent;
    std::vector<Node*> nbs;

    void fromStr(const std::string& str) {
        if (str != "Z") {
            val = std::stoul(str);
            walkable = true;
        } else {
            walkable = false;
        }
    }

    friend std::ostream& operator<< (std::ostream& os, const Node& n) {

        os << "[" << n.pos << "], " << n.f << ", [";
        if (n.parent) {
            os << n.parent->pos;
        }
        else {
            os << "NULL";
        }
        os << "]";
        return os;
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
