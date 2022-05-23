/**
 * @file generator.h
 * @author Daniel Panero, Andrea Diez
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
    ~Generator() override;

    void add_to_grid() override;
    void remove_from_grid() override;

    void draw() override;
    void undraw() override;

    std::string get_as_string() override;

    /**
     * @brief Advances one step the state of the generator: it tries to remain inside
     * the anthill, when it fails to do so, it return false
     *
     * @param anthill
     * @return false when outside the anthill / touches the border
     */
    bool step(const Squarecell::Square &anthill);

    /**
     * @brief Generates all the possible new positions / moves based on the origin
     *
     * @param origin
     * @return std::vector<Square>
     */
    static std::vector<Squarecell::Square> generate_moves(Squarecell::Square origin);

    /**
     * @brief Returns true if origin is inside of square anthill and it doesn't touch
     * any edge
     *
     * @param origin
     * @param anthill
     * @return true if origin is inside of square anthill and it doesn't touch
     * any edge
     */
    static bool
    test_if_confined_and_not_near_border(Squarecell::Square const &origin,
                                         Squarecell::Square const &anthill);
};

#endif