/**
 * @file food.h
 * @author Daniel Panero, Layane Wazen, Andrea Diez Leboffe
 * @version 0.1
 * @date 2022-04-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef ENTITIES_FOOD_H
#define ENTITIES_FOOD_H

#include "squarecell.h"

class Food : Square
{
public:
    /**
     * @brief Constructs a new Food instance
     *
     * @param x position of food in the x-axis
     * @param y position of food in the y-axis
     */
    Food(unsigned int &x, unsigned int &y);

    static Food *parse_line(std::string &line);

    void add_to_grid();
};

#endif