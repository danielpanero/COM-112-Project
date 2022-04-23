#include "iostream"
#include "sstream"

#include "entities.h"
#include "squarecell.h"

Entities::Entities(unsigned int &x, unsigned int &y, unsigned int side, bool centered)
    : Square{x, y, side, centered}
{
}

Square Entities::get_as_square() { return {*this}; }
