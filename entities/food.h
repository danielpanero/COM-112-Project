#ifndef ENTITIES_FOOD_H
#define ENTITIES_FOOD_H

#include "squarecell.h"

class Food : Square
{
public:
    Food(unsigned int &x, unsigned int &y);

    static Food *parse_line(std::string &line);

    void add_to_grid();
};

#endif