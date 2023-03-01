#pragma once

#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <array>
#include <sstream>
#include <cmath>

struct Vec2 {
    std::size_t x, y;

    Vec2() : x(0), y(0) {}
    Vec2(const Vec2& v) : x(v.x), y(v.y) {}
    Vec2(size_t xv, size_t yv) : x(xv), y(yv) {}

    float distance(const Vec2& v) {
        //Calculate sqrt but avoid overflow
        size_t dx = x > v.x ? x - v.x : v.x - x;
        size_t dy = y > v.y ? y - v.y : v.y - y;

        return std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));
    }

    void fromStr(const std::string& str) {
        std::string s1, s2;

        s1 = str.substr(0, str.find(','));
        s2 = str.substr(str.find(',') + 1);

        x = std::stoul(s1);
        y = std::stoul(s2);
    }

    friend bool operator== (const Vec2& v1, const Vec2& v2) {
        return v1.x == v2.x && v1.y == v2.y;
    }

    //Coords printed in reverse order!!!
    friend std::ostream& operator<< (std::ostream& os, const Vec2& v) {
        os << v.y << ", " << v.x;
        return os;
    }
};

//Fixed precision float printing
#define FPFL(_x, _p) std::fixed <<std::setprecision((_p)) << (_x)

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
        std::cout << "(";
        os << "[" << n.pos << "], " << FPFL(n.f, 2) << ", [";
        if (n.parent) {
            os << n.parent->pos;
        }
        else {
            os << "NULL";
        }
        os << "])";
        return os;
    }

    bool operator==(const Node& n) {
        return pos == n.pos;
    }
};

struct Args {
    std::string file;
    Vec2 dim, start, end;
    bool useColor;

    void print() {
        std::cout << "File: " << file << std::endl;
        std::cout << "Dim: " << dim.x << ", " << dim.y << std::endl;
        std::cout << "Start: " << start.x << ", " << start.y << std::endl;
        std::cout << "End: " << end.x << ", " << end.y << std::endl;
        std::cout << "Use color: " << useColor << std::endl;
    }
};
