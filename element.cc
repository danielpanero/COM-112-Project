#include "iostream"
#include "sstream"

#include "element.h"
#include "squarecell.h"

Element::Element(unsigned int &x, unsigned int &y, unsigned int side, bool centered)
    : Square{x, y, side, centered}
{
}

Square Element::get_as_square() { return {*this}; }
