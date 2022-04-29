/**
 * @file element.h
 * @author Daniel Panero, Layane Wazen, Andrea Diez
 * @version 0.1
 * @date 2022-04-23
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef ELEMENT_H
#define ELEMENT_H

#include "memory"

#include "constantes.h"
#include "squarecell.h"

/**
 * @brief Abstract class Element (Base class for all elements: Anthill, Food, Ants...)
 *
 */
class Element : protected Square
{
public:
    /**
     * @brief Construct a new Element instance
     *
     * @param x position of element in the x-axis
     * @param y position of element in the y-axis
     * @param side size of element
     * @param centered
     */
    Element(unsigned int x, unsigned int y, unsigned int side, bool centered,
            unsigned int color_index);

    Square get_as_square();

    /**
     * @brief Converts the instance back to its string representation
     *
     * @return std::string
     */
    virtual std::string get_as_string() = 0;
    virtual void draw() = 0;

    unsigned int get_color_index() const;

private:
    unsigned int color_index;
};

#endif