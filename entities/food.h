#ifndef ENTITIES_FOOD_H
#define ENTITIES_FOOD_H

#include "squarecell.h"

class Food : Square
{
public:
    static Food* parse_line (std::string &line);

    Food (unsigned int &x, unsigned int &y);


    void add_to_grid();
};

#endif