/**
 * @file simulation.h
 * @author Daniel Panero [+39/-15], Layane Wazen [+11/-1], Andrea Diez Leboffe [+0/-0]
* @version 0.1
 * @date 2022-04-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef SIMULATION_H
#define SIMULATION_H

#include "anthill.h"
#include "food.h"

class Simulation
{
private:
    unsigned int n_foods;
    unsigned int n_anthills;

    std::vector<Anthill *> anthills;
    std::vector<Food *> foods;

    void parse_foods(std::ifstream &file);
    void parse_anthills(std::ifstream &file);

    void parse_collectors(std::ifstream &file, Anthill *anthill);
    void parse_defensors(std::ifstream &file, Anthill *anthill);
    void parse_predators(std::ifstream &file, Anthill *anthill);

    void check_overlapping_anthills();
    void check_generator_defensors_inside_anthills();

public:
    void read_file(std::string &path);
};

/**
 * @brief Gets the next non-empty line
 *
 * @param file
 * @return string
 */
std::string get_next_line(std::ifstream &file);

#endif