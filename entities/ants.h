#ifndef ENTITIES_ANTS_H
#define ENTITIES_ANTS_H

#include "squarecell.h"

class Generator : Square
{
public:
    Generator(unsigned int x, unsigned int y);

    Square get_as_square();

    void add_to_grid();
};

class Collector : Square
{
public:
    explicit Collector(std::string &line);

    void add_to_grid();
};

class Defensor : Square
{
public:
    explicit Defensor(std::string &line);

    Square get_as_square();

    void add_to_grid();
};

class Predator : Square
{
public:
    explicit Predator(std::string &line);

    void add_to_grid();
};

#endif