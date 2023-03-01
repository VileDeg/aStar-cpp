#include <iostream>

#include "types.h"
#include "Astar.h"

void parse_args(int argc, char** argv, Args* args) {
    if (argc != 6) {
        throw std::runtime_error("Invalid number of arguments");
    }
    args->file = argv[1];

    try {       
        args->dim.fromStr(argv[2]);
        args->start.fromStr(argv[3]);
        std::swap(args->start.x, args->start.y);
        args->end.fromStr(argv[4]);
        std::swap(args->end.x, args->end.y);
        args->useColor = std::stoi(argv[5]);
    } catch (const std::exception& e) {
        throw std::runtime_error("Error parsing Vec2: " + std::string(e.what()));
    }
}

int main(int argc, char** argv)
{
    const char* usage = "Usage: ./astar <input_file.csv> <R,C> <X,Y> <X,Y> <use_color>\n\n"
        "\tFile to read the grid from, grid dimensions, start point and end point. And wether to use color print (0 or 1).\n"
        "\tWhere R - number of rows(Y), C - number of columns(X).";

    Args args{};
    try {
        parse_args(argc, argv, &args);
    } catch (const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
        std::cout << usage << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cout << "Unknown exception: " << e.what() << std::endl;
        return 1;
    }

    args.print();

    Astar astar(args);
    try {
        astar.Init();
    } catch (const std::exception& e) {
        std::cout << "Error initializing Astar: " << e.what() << std::endl;
        return 1;
    }
    
    astar.Show();
    astar.FindPath();
    astar.PrintPath();

    return 0;
}