/**
 * @file element.cc
 * @author Daniel Panero [+0/-0], Andrea Diez [+34/-9]
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

Element::Element(unsigned int x, unsigned int y, unsigned int side, bool centered,
                 unsigned int color_index)
    : Squarecell::Square{x, y, side, centered}, color_index(color_index)
{
}

Element::~Element() {}

Squarecell::Square Element::get_as_square() { return {*this}; }
unsigned int Element::get_color_index() const { return color_index; }