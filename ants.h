/**
 * @file ants.h
 * @author Daniel Panero [+57/-7], Layane Wazen [+34/-0], Andrea Diez [+204/-110]
 * @version 0.1
 * @date 2022-04-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef ANTS_H
#define ANTS_H

#include "element.h"

/**
 * @brief Abstract class Ant (Base class for: Collector, Defensor, Predator)
 *
 */
class Ant : public Element
{
public:
    /**
     * @brief Construct a new Ant instance
     *
     * @param x position of generator in the x-axis
     * @param y position of generator in the y-axis
     * @param side size of element
     * @param age
     * @param color_index (0 red, 1 green, 2 blue, 3 yellow, 4 magenta, 5 cyan) same as
     * graphic.h
     */
    Ant(unsigned int x, unsigned int y, unsigned int side, unsigned int age,
        unsigned int color_index);
    ~Ant() override = default;

    /**
     * @brief Checks that position in the grid is empty and either throw an error or
     * fills the grid
     *
     */
    virtual void add_to_grid() = 0;
    virtual void remove_from_grid() = 0;

    std::string get_as_string() override;

    /**
     * @brief Increases age by one
     *
     * @return false if age >= bug life and therefore the Ant should be dead
     */
    bool increase_age();

    static std::vector<Square> generate_moves(Squarecell::Square origin,
                                              std::vector<int> x_shift,
                                              std::vector<int> y_shift);

protected:
    unsigned int age;
};

#endif