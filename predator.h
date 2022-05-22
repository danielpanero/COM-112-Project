/**
 * @file predator.h
 * @author Daniel Panero [+57/-7], Layane Wazen [+34/-0], Andrea Diez [+204/-110]
 * @version 0.1
 * @date 2022-04-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef ANTS_PREDATOR_H
#define ANTS_PREDATOR_H

#include <memory>

#include "ants.h"
#include "constantes.h"
#include "squarecell.h"

class Predator : public Ant
{
public:
    /**
     * @brief Constructs a new Predator instance
     *
     * @param x position of predator in the x-axis
     * @param y position of predator in the y-axis
     * @param age
     * @param color_index (0 red, 1 green, 2 blue, 3 yellow, 4 magenta, 5 cyan) same as
     * graphic.h
     */
    Predator(unsigned int x, unsigned int y, unsigned int age,
             unsigned int color_index);
    ~Predator() override;

    void add_to_grid() override;
    void remove_from_grid() override;

    void draw() override;
    void undraw() override;

    /**
     * @brief It increases the age and return true if the predator died of old age
     *
     * @return true if dead
     */
    bool step();

    void remain_inside(Squarecell::Square &anthill_square);

    void move_toward_nearest_ant(std::vector<Squarecell::Square> &ants);

    Squarecell::Square find_target_ant(std::vector<Squarecell::Square> &ants);

    /**
     * @brief Given the position of the ant and the state of the Anthill, it determines
     * if ant is attackable or not
     *
     * @param state
     * @param anthill
     * @param ant
     * @return true if attacckable
     */
    static bool filter_ants(State_anthill state, Squarecell::Square &anthill,
                            Squarecell::Square &ant);

    /**
     * @brief Generates all the possible new positions / moves based on the origin
     *
     * @param origin
     * @return std::vector<Square>
     */
    static std::vector<Squarecell::Square> generate_moves(Squarecell::Square origin);

    static bool test_if_reached_ant(Squarecell::Square const &origin,
                                    Squarecell::Square const &anthill);
    /**
     * @brief Creates a new pointed instance of Predator from its string representation
     *
     * @param line
     * @param color_index
     * @return std::unique_ptr<Predator>
     */
    static std::unique_ptr<Predator> parse_line(std::string &line,
                                                unsigned int color_index);
};

#endif