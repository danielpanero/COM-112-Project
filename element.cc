/**
 * @file element.cc
 * @author Daniel Panero 10% Andrea Diez 90%
 * @version 0.1
 * @date 2022-04-23
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <iostream>
#include <sstream>

#include "squarecell.h"

#include "element.h"

using Squarecell::Square;

Element::Element(unsigned int x, unsigned int y, unsigned int side, bool centered,
                 unsigned int color_index)
    : Square{x, y, side, centered}, color_index(color_index)
{
}

Element::~Element() = default;

Square Element::get_as_square() { return {*this}; }

unsigned int Element::get_color_index() const { return color_index; }