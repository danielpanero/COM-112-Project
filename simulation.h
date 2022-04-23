/**
 * @file simulation.h
 * @author Daniel Panero [+39/-15], Layane Wazen [+11/-1], Andrea Diez [+0/-0]
 * @version 0.1
 * @date 2022-04-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef SIMULATION_H
#define SIMULATION_H

#include "memory"

#include "anthill.h"
#include "food.h"

class Simulation
{
public:
    bool read_file(std::string &path);

    unsigned int get_n_foods();
    bool Simulation::get_info_prev_anthill(unsigned int &n_collectors,
                                           unsigned int &n_defensors,
                                           unsigned int &n_predators,
                                           unsigned int &n_food);

    bool Simulation::get_info_next_anthill(unsigned int &n_collectors,
                                           unsigned int &n_defensors,
                                           unsigned int &n_predators,
                                           unsigned int &n_food);

private:
    void parse_foods(std::ifstream &file);
    void parse_anthills(std::ifstream &file);

    /**
     * @brief This is a convenience function for parsing either Ant::Collector,
     * Ant::Defensor and Ant::Predator
     *
     * @tparam T Ant::Collector / Ant::Defensor / Ant::Predator
     * @param file
     * @param n number of ants
     * @return vector<unique_ptr<T>>
     */
    template <typename T>
    std::vector<std::unique_ptr<T>> parse_ants(std::ifstream &file, unsigned int n);

    void reset();

    void check_overlapping_anthills();
    void check_generator_defensors_inside_anthills();

    unsigned int n_foods;
    unsigned int n_anthills;

    std::vector<std::unique_ptr<Anthill>> anthills;
    std::vector<std::unique_ptr<Food>> foods;

    static int index_anthill;
};

/**
 * @brief Gets the next non-empty line
 *
 * @param file
 * @return string
 */
std::string get_next_line(std::ifstream &file);

#endif