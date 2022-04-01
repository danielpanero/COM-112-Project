#ifndef SIMULATION_H
#define SIMULATION_H

#include "entities/anthill.h"
#include "entities/food.h"

class Simulation
{
private:
    unsigned int n_foods;
    unsigned int n_anthills;

    std::vector<Anthill *> anthills;
    std::vector<Food *> foods;

    void parse_foods(std::ifstream &file);
    void parse_anthills(std::ifstream &file);
    void check_overlapping_anthills();
    void parse_collectors(std::ifstream &file, Anthill &anthill);
    void parse_defensors(std::ifstream &file, Anthill &anthill);
    void parse_predators(std::ifstream &file, Anthill &anthill);
    void check_generator_defensors_inside_anthills();

public:
    void read_file(std::string &path);
};

#endif