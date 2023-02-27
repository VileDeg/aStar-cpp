#include <iostream>

#include "types.h"
#include "Astar.h"

void parse_args(int argc, char** argv, Args* args) {
    if (argc != 5) {
        throw std::runtime_error("Invalid number of arguments");
    }
    args->file = argv[1];

    try {       
        args->dim.fromStr(argv[2]);
        args->start.fromStr(argv[3]);
        args->end.fromStr(argv[4]);
    } catch (const std::exception& e) {
        throw std::runtime_error("Error parsing Vec2: " + std::string(e.what()));
    }
}

int main(int argc, char** argv)
{
    const char* usage = "Usage: ./astar <input_file.csv> <R,C> <Y,X> <Y,X>\n\n"
        "\tFile to read the grid from, grid dimensions, start point and end point.\n"
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

    Astar astar(args.file, args.dim);
    try {
        astar.Init();
    } catch (const std::exception& e) {
        std::cout << "Error initializing Astar: " << e.what() << std::endl;
        return 1;
    }
    
    astar.Print();

    return 0;
}