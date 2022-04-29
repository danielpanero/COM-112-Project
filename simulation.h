/**
 * @file simulation.h
 * @author Daniel Panero [+155/-60], Layane Wazen [+11/-1], Andrea Diez [+0/-0]
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
    /**
     * @brief Reads the file, prepares the simulation model and draws all the elements
     * on the model_surface. In the case of an error in the configuration file, it
     * safely discards all the elements of model and clears it self
     *
     * @param path
     * @return true if there are no errors in the configuration file, false in the case
     * a error was detected.
     */
    bool read_file(std::string &path);
    void save_file(std::string &path);

    /**
     * @brief Resets and disallocates every aspect / object of the simulation: foods,
     * anthills, collectors... Furthermore, it resets also the grid of the module
     * Squarecell and clears the model_surface of Graphic
     *
     */
    void reset();

    unsigned int get_n_foods() const;

    /**
     * @brief Convenience function which cycles through all the anthills (descending
     * order) and return the data from the first not-dead anthill that it has found. If
     * there are no more anthill alive, it returns false. Under the hood, it calls
     * \b Simulation::cycle_info_anthills()
     *
     * @param[out] index the index of the first non-dead anthill
     * @param[out] n_collectors
     * @param[out] n_defensors
     * @param[out] n_predators
     * @param[out] n_food
     * @return true if there are anthills left and false if they are all dead
     */
    bool get_info_prev_anthill(unsigned int &index, unsigned int &n_collectors,
                               unsigned int &n_defensors, unsigned int &n_predators,
                               unsigned int &n_food);

    /**
     * @brief Convenience function which cycles through all the anthills (asceding
     * order) and return the data from the first not-dead anthill that it has found. If
     * there are no more anthill alive, it returns false. Under the hood, it calls
     * \b Simulation::cycle_info_anthills()
     *
     * @param[out] index the index of the first non-dead anthill
     * @param[out] n_collectors
     * @param[out] n_defensors
     * @param[out] n_predators
     * @param[out] n_food
     * @return true if there are anthills left and false if they are all dead
     */
    bool get_info_next_anthill(unsigned int &index, unsigned int &n_collectors,
                               unsigned int &n_defensors, unsigned int &n_predators,
                               unsigned int &n_food);

    /**
     * @brief Cycles through all the anthills (asceding / descending order) and return
     * the data from the first not-dead anthill that it has found. If there are no more
     * anthill alive, it returns false
     *
     * @param[out] index the index of the first non-dead anthill
     * @param[out] n_collectors
     * @param[out] n_defensors
     * @param[out] n_predators
     * @param[out] n_food
     * @param[in] order true = ascending / false = descending
     * @return true if there are anthills left and false if they are all dead
     */
    bool cycle_info_anthill(unsigned int &index, unsigned int &n_collectors,
                            unsigned int &n_defensors, unsigned int &n_predators,
                            unsigned int &n_food, bool order);

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
    std::vector<std::unique_ptr<T>> parse_ants(std::ifstream &file, unsigned int n,
                                               unsigned int index_anthill);

    void check_overlapping_anthills();
    void check_generator_defensors_inside_anthills();

    unsigned int n_foods = 0;
    unsigned int n_anthills = 0;

    /**
     * @brief The current index used by get_info_prev_anthill and get_info_next_anthill
     *
     */
    int index_anthill = 0;

    std::vector<std::unique_ptr<Anthill>> anthills;
    std::vector<std::unique_ptr<Food>> foods;
};

/**
 * @brief Gets the next non-empty line
 *
 * @param file
 * @return string
 */
std::string get_next_line(std::ifstream &file);

#endif