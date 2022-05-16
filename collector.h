/**
 * @file collector.h
 * @author Daniel Panero [+57/-7], Layane Wazen [+34/-0], Andrea Diez [+204/-110]
 * @version 0.1
 * @date 2022-04-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef ANTS_COLLECTOR_H
#define ANTS_COLLECTOR_H

#include <memory>

#include "ants.h"
#include "food.h"
#include "squarecell.h"

class Collector : public Ant
{
public:
    /**
     * @brief Constructs a new Collector instance
     *
     * @param x position of collector in the x-axis
     * @param y position of collector in the y-axis
     * @param age
     * @param state state of collector: EMPTY / LOADED
     * @param color_index (0 red, 1 green, 2 blue, 3 yellow, 4 magenta, 5 cyan) same as
     * graphic.h
     */
    Collector(unsigned int x, unsigned int y, unsigned int age, State_collector state,
              unsigned int color_index);
    ~Collector() override;

    void add_to_grid() override;
    void remove_from_grid() override;

    void draw() override;
    void undraw() override;

    std::string get_as_string() override;

    State_collector get_state();

    /**
     * @brief It increases the age and return true if the predator died of old age
     *
     * @return true if dead
     */
    bool step();

    /**
     * @brief Moves the collector to \b anthill, if it reaches it border it returns
     * true and the state change to EMPTY
     *
     * @param anthill_square
     * @return true if it reaches the border of \b anthill
     */
    bool return_to_anthill(Squarecell::Square &anthill_square);

    /**
     * @brief Moves the collector to \b food, if superposes with the
     * food returns true and the state changes to LOADED
     *
     * @param food
     * @return true if it reaches the food
     */
    bool search_food(std::unique_ptr<Food> &food);

    /**
     * @brief Finds the nearest attainable food
     *
     * @param foods
     * @param[out] target
     * @return true if it has found one
     */
    bool find_target_food(std::vector<std::unique_ptr<Food>> &foods, size_t &target);

    /**
     * @brief Generates all the possible new positions / moves based on the origin
     *
     * @param origin
     * @return std::vector<Square>
     */
    static std::vector<Square> generate_diagonal_moves(Squarecell::Square origin);

    /**
     * @brief Creates a new pointed instance of Collector from its string
     * representation
     *
     * @param line
     * @param color_index (0 red, 1 green, 2 blue, 3 yellow, 4 magenta, 5 cyan) same as
     * graphic.h
     * @return std::unique_ptr<Collector>
     */
    static std::unique_ptr<Collector> parse_line(std::string &line,
                                                 unsigned int color_index);

private:
    State_collector state;
};

#endif