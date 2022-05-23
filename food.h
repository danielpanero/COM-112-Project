/**
 * @file food.h
 * @author Daniel Panero, Andrea Diez
 * @version 0.1
 * @date 2022-04-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef ENTITIES_FOOD_H
#define ENTITIES_FOOD_H

#include <memory>

#include "element.h"
#include "squarecell.h"

class Food : public Element
{
public:
    /**
     * @brief Constructs a new Food instance
     *
     * @param x position of food in the x-axis
     * @param y position of food in the y-axis
     */
    Food(unsigned int x, unsigned int y);
    ~Food() override = default;

    void add_to_grid();
    void remove_from_grid();

    void draw() override;
    void undraw() override;

    std::string get_as_string() override;

    static std::unique_ptr<Food> parse_line(std::string &line);
};

#endif