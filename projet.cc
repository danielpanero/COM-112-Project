#include "iostream"

#include "simulation.h"

using std::string;

int main(int argc, char const *argv[])
{
    if (argc == 1)
    {
        exit(EXIT_FAILURE);
    }

    string path(argv[1]);

    Simulation simulation;
    simulation.read_file(path);

    exit(0);
}
