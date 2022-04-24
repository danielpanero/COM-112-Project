/**
 * @file element.cc
 * @author Daniel Panero, Layane Wazen, Andrea Diez
 * @version 0.1
 * @date 2022-04-23
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "iostream"
#include "sstream"

#include "squarecell.h"

#include "element.h"

Element::Element(unsigned int x, unsigned int y, unsigned int side, bool centered)
    : Square{x, y, side, centered}
{
}

Square Element::get_as_square() { return {*this}; }
