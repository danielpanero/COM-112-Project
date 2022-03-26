#ifndef ENTITIES_ANTHILL_H
#define ENTITIES_ANTHILL_H

#include "vector"

#include "ants.h"
#include "squarecell.h"

class Anthill : Square
{
private:
    unsigned int total_food;
    unsigned int nbC;
    unsigned int nbD;
    unsigned int nbP;

    Generator *generator;
    std::vector<Collector *> collectors;
    std::vector<Defensor *> defensors;
    std::vector<Predator *> predators;

public:
    explicit Anthill(std::string &line);

    void test_if_generator_defensors_perimeter(unsigned int index);

    void set_collectors(std::vector<Collector *> &collectors);
    void set_defensors(std::vector<Defensor *> &defensor);
    void set_predators(std::vector<Predator *> &predators);

    unsigned int get_number_of_collectors() const;
    unsigned int get_number_of_defensors() const;
    unsigned int get_number_of_predators() const;

    Square get_as_square();
};

#endif