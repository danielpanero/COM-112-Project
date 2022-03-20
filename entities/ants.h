#ifndef ANTS_H
#define ANTS_H

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
    Collector(std::string &line);

    void add_to_grid();
};

class Defensor : Square
{
public:
    Defensor(std::string &line);

    Square get_as_square();

    void add_to_grid();
};

class Predator : Square
{
public:
    Predator(std::string &line);

    void add_to_grid();
};

#endif