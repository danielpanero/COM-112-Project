/**
 * @file defensor.h
 * @author Daniel Panero [+57/-7], Layane Wazen [+34/-0], Andrea Diez [+204/-110]
 * @version 0.1
 * @date 2022-04-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef ANTS_DEFENSOR_H
#define ANTS_DEFENSOR_H

#include <memory>

#include "ants.h"
#include "element.h"
#include "squarecell.h"

class Defensor : public Ant
{
public:
    /**
     * @brief Constructs a new Defensor instance
     *
     * @param x position of defensor in the x-axis
     * @param y position of defensor in the y-axis
     * @param age
     * @param color_index (0 red, 1 green, 2 blue, 3 yellow, 4 magenta, 5 cyan) same as
     * graphic.h
     */
    Defensor(unsigned int x, unsigned int y, unsigned int age,
             unsigned int color_index);

    void add_to_grid() override;
    void remove_from_grid() override;

    void draw() override;
    void undraw() override;

    /**
     * @brief Advances one step the state of the defensor: it tries to remain inside
     * the anthill and near the border, when it fails to do so, it return false
     *
     * @param anthill_square
     * @return false when outside or it is touching the border
     */
    bool step(Squarecell::Square &anthill_square);

    /**
     * @brief It tests that the origin is completly confined and near the border
     *
     * @param origin
     * @param anthill
     * @return true if it is completly confined and near the border
     */
    static bool test_if_confined_and_near_border(Squarecell::Square &origin,
                                                 Squarecell::Square &anthill);

    /**
     * @brief Generates all the possible new positions / moves based on the origin
     *
     * @param origin
     * @return std::vector<Square>
     */
    static std::vector<Square> generate_hv_moves(Squarecell::Square origin);

    /**
     * @brief Creates a new pointed instance of Defensor from its string representation
     *
     * @param line
     * @param color_index (0 red, 1 green, 2 blue, 3 yellow, 4 magenta, 5 cyan) same as
     * graphic.h
     * @return std::unique_ptr<Defensor>
     */
    static std::unique_ptr<Defensor> parse_line(std::string &line,
                                                unsigned int color_index);
};

#endif