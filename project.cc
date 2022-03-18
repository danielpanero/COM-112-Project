#include "iostream"

#include "simulation.h"

using namespace std;

int main(int argc, char const *argv[])
{
    if (argc == 1)
    {
        exit(EXIT_FAILURE);
    }

    Simulation simulation;
    simulation.read_file(argv[1]);

    exit(0);
}
