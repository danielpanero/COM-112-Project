/**
 * @file anthill.h
 * @author Daniel Panero [+44/-6], Layane Wazen [+29/-0], Andrea Diez [+132/-64]
 * @version 0.1
 * @date 2022-04-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef ENTITIES_ANTHILL_H
#define ENTITIES_ANTHILL_H

#include <memory>
#include <vector>

#include "ants.h"
#include "collector.h"
#include "constantes.h"
#include "defensor.h"
#include "element.h"
#include "generator.h"
#include "predator.h"
#include "squarecell.h"

class Anthill : public Element
{
public:
    /**
     * @brief Constructs a new Anthill instance
     *
     * @param x position of anthill in the x-axis
     * @param y position of anthill in the y-axis
     * @param side side of anthill
     * @param xg position of generator in the x-axis
     * @param yg position of generator in the y-axis
     * @param n_food total number of foods
     * @param n_collectors number of collectors
     * @param n_defensors number of defensors
     * @param n_predators number of predators
     * @param color_index (0 red, 1 green, 2 blue, 3 yellow, 4 magenta, 5 cyan) same as
     * graphic.h
     */
    Anthill(unsigned int x, unsigned int y, unsigned int side, unsigned int xg,
            unsigned int yg, double n_food, unsigned int n_collectors,
            unsigned int n_defensors, unsigned int n_predators,
            unsigned int color_index);
    ~Anthill() override;

    /**
     * @brief Checks if the defensors and generator are contained in the perimeter of
     * the anthill
     *
     * @param index index of the position of anthill in the file
     */
    void test_if_generator_defensors_perimeter(unsigned int index);

    void set_collectors(std::vector<std::unique_ptr<Collector>> &collectors);
    void set_defensors(std::vector<std::unique_ptr<Defensor>> &defensor);
    void set_predators(std::vector<std::unique_ptr<Predator>> &predator);

    unsigned int get_number_of_collectors() const;
    unsigned int get_number_of_defensors() const;
    unsigned int get_number_of_predators() const;
    double get_number_of_food() const;

    void draw() override;
    void undraw() override;

    std::string get_as_string() override;

    bool step(std::vector<std::unique_ptr<Food>> &foods,
              std::vector<std::unique_ptr<Anthill>> &anthills);

    void update_collectors(std::vector<std::unique_ptr<Food>> &foods);
    void update_defensors(std::vector<std::unique_ptr<Anthill>> &anthills);
    void update_predators(std::vector<std::unique_ptr<Anthill>> &anthills);

    /**
     * @brief Called by others Anthills in order to find the nearest attackable Ant
     * (Collector / Predator) in this Anthill that complies with the constraint \b test
     *
     * @param test
     * @param[out] targets
     * @return true if it has found one
     */
    bool get_attackable_ants(const std::function<bool(Squarecell::Square &)> &test,
                             std::vector<Squarecell::Square> &targets);

    /**
     * @brief Called by others Anthills in order to initiate an attack, return true if
     * at least one ant was killed
     *
     * @param test function called for testing if the ant should be killed or not
     * @return true if at least one ant was killed
     */
    bool
    mark_collectors_as_dead(const std::function<bool(Squarecell::Square &)> &test);
    bool mark_predators_as_dead(const std::function<bool(Squarecell::Square &)> &test);

    /**
     * @brief Dumps all the dead ants, it clears the grid and the model
     *
     */
    void clear_dead_ants();

    /**
     * @brief Creates a new pointed instance Anthill from its string representation
     *
     * @param line
     * @param color_index (0 red, 1 green, 2 blue, 3 yellow, 4 magenta, 5 cyan) same as
     * graphic.h
     * @return std::unique_ptr<Anthill>
     */
    static std::unique_ptr<Anthill> parse_line(std::string &line,
                                               unsigned int color_index);

private:
    void try_to_expand(std::vector<std::unique_ptr<Anthill>> &anthill);
    void generate_new_ants();

    unsigned int calculate_side();

    /**
     * @brief It reduce the total food counr based on the number of ants
     *
     * @return false if the total food is not enough
     */
    bool reduce_food();

    bool find_suitable_position_for_ant(unsigned int side_ant,
                                        Squarecell::Square &position);

    double n_food;

    std::unique_ptr<Generator> generator;
    std::vector<std::unique_ptr<Collector>> collectors;
    std::vector<std::unique_ptr<Defensor>> defensors;
    std::vector<std::unique_ptr<Predator>> predators;

    std::vector<std::unique_ptr<Ant>> dead_ants;

    State_anthill state = FREE;
};

#endif