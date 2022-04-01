#ifndef ENTITIES_ANTS_H
#define ENTITIES_ANTS_H

#include "constantes.h"

#include "squarecell.h"

class Generator : Square
{
public:
    Generator(unsigned int &x, unsigned int &y);

    const Square get_as_square();

    void add_to_grid();
};

class Collector : Square
{
public:
    Collector(unsigned int &x, unsigned int &y);

    static Collector *parse_line(std::string &line);

    void add_to_grid();
};

class Defensor : Square
{
public:
    Defensor(unsigned int &x, unsigned int &y);

    static Defensor *parse_line(std::string &line);

    const Square get_as_square();

    void add_to_grid();
};

class Predator : Square
{
public:
    Predator(unsigned int &x, unsigned int &y);

    static Predator *parse_line(std::string &line);

    void add_to_grid();
};

#endif