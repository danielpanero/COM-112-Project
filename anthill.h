/**
 * @file anthill.h
 * @author Daniel Panero [+44/-6], Layane Wazen [+29/-0], Andrea Diez [+12/-10]
 * @version 0.1
 * @date 2022-04-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef ENTITIES_ANTHILL_H
#define ENTITIES_ANTHILL_H

#include "memory"
#include "vector"

#include "ants.h"
#include "element.h"
#include "squarecell.h"

class Anthill : public Element
{
public:
    /**
     * @brief Constructs a new Anthill instance
     *
     * @param x position of anthill in the x-axis
     * @param y  position of anthill in the y-axis
     * @param side side of anthill
     * @param xg  position of generator in the x-axis
     * @param yg  position of generator in the y-axis
     * @param total_food total number of food
     * @param n_collectors number of collectors
     * @param n_defensors number of defensors
     * @param n_predators number of predators
     */
    Anthill(unsigned int &x, unsigned int &y, unsigned int &side, unsigned int &xg,
            unsigned int &yg, unsigned int total_food, unsigned int &n_collectors,
            unsigned int &n_defensors, unsigned int &n_predators);
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

    std::string get_as_string() override;
    void draw(unsigned int &color_index) override;

    /**
     * @brief Creates a new pointed instance Anthill from its string representation
     *
     * @param line
     * @return std::unique_ptr<Anthill>
     */
    static std::unique_ptr<Anthill> parse_line(std::string &line);

private:
    unsigned int total_food;
    unsigned int n_collectors;
    unsigned int n_defensors;
    unsigned int n_predators;

    std::unique_ptr<Generator> generator;
    std::vector<std::unique_ptr<Collector>> collectors;
    std::vector<std::unique_ptr<Defensor>> defensors;
    std::vector<std::unique_ptr<Predator>> predators;
};

#endif