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

    void add_to_grid() override;
    void remove_from_grid() override;

    void draw() override;
    void undraw() override;

    std::string get_as_string() override;

    bool step(Square anthill, std::vector<std::unique_ptr<Food>> &foods);

    bool return_to_anthill(Square target);

    /**
     * @brief Moves the collector the nearest foods attainable, if there is no food
     * return false
     *
     * @param foods
     */
    bool search_food(std::vector<std::unique_ptr<Food>> &foods);

    /**
     * @brief Finds the nearest attainable food and return the index to it
     *
     * @param foods
     * @return size_t
     */
    size_t find_target_food(std::vector<std::unique_ptr<Food>> &foods);

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