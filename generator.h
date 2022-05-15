/**
 * @file generator.h
 * @author Daniel Panero [+57/-7], Layane Wazen [+34/-0], Andrea Diez [+204/-110]
 * @version 0.1
 * @date 2022-04-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef ANTS_GENERATOR_H
#define ANTS_GENERATOR_H

#include "ants.h"

class Generator : public Ant
{
public:
    /**
     * @brief Constructs a new Generator instance
     *
     * @param x position of generator in the x-axis
     * @param y position of generator in the y-axis
     * @param age
     * @param color_index (0 red, 1 green, 2 blue, 3 yellow, 4 magenta, 5 cyan) same as
     * graphic.h
     */
    Generator(unsigned int x, unsigned int y, unsigned int age,
              unsigned int color_index);

    void add_to_grid() override;
    void remove_from_grid() override;

    void draw() override;
    void undraw() override;

    std::string get_as_string() override;

    bool step();
};

#endif