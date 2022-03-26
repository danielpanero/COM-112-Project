#ifndef ENTITIES_FOOD_H
#define ENTITIES_FOOD_H

#include "squarecell.h"

class Food : Square
{
public:
    explicit Food(std::string &line);

    void add_to_grid();
};

#endif