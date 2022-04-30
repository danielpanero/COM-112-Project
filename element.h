/**
 * @file element.h
 * @author Daniel Panero [¨+0/-0], Andrea Diez [+66/-11]
 * @version 0.1
 * @date 2022-04-23
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef ELEMENT_H
#define ELEMENT_H

#include <memory>

#include "constantes.h"
#include "squarecell.h"

/**
 * @brief Abstract class Element (Base class for all elements: Anthill, Food, Ants...)
 *
 */
class Element : protected Squarecell::Square
{
public:
    /**
     * @brief Construct a new Element instance
     *
     * @param x position of element in the x-axis
     * @param y position of element in the y-axis
     * @param side size of element
     * @param centered
     * @param color_index (0 red, 1 green, 2 blue, 3 yellow, 4 magenta, 5 cyan) same as
     * in the module Graphic
     */
    Element(unsigned int x, unsigned int y, unsigned int side, bool centered,
            unsigned int color_index);
    virtual ~Element() = 0;

    Squarecell::Square get_as_square();
    unsigned int get_color_index() const;

    /**
     * @brief Converts the instance back to its string representation
     *
     * @return std::string
     */
    virtual std::string get_as_string() = 0;
    virtual void draw() = 0;

private:
    /**
     * @brief (0 red, 1 green, 2 blue, 3 yellow, 4 magenta, 5 cyan) same as
     * in the module Graphic
     */
    unsigned int color_index;
};

#endif