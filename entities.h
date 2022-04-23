#ifndef ENTITIES_H
#define ENTITIES_H

#include "memory"

#include "constantes.h"
#include "squarecell.h"

class Entities: protected Square
{
public:
    Entities(unsigned int &x, unsigned int &y, unsigned int side, bool centered);

    Square get_as_square();

    virtual std::string get_as_string() = 0;

};

#endif