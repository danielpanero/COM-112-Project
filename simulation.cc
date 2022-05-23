/**
 * @file simulation.cc
 * @author Daniel Panero [+613/-290], Layane Wazen [+133/-96], Andrea Diez [+42/-42]
 * @version 0.1
 * @date 2022-04-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "anthill.h"
#include "collector.h"
#include "defensor.h"
#include "food.h"
#include "message.h"
#include "predator.h"

#include "simulation.h"

using std::endl;
using std::ifstream;
using std::istringstream;
using std::string;
using std::vector;

unsigned int const g_max(128);

bool Simulation::read_file(string &path)
{
    reset();

    ifstream file(path);
    try
    {
        if (file.fail())
        {
            return false;
        }

        parse_foods(file);
        parse_anthills(file);

        check_overlapping_anthills();
        check_generator_defensors_inside_anthills();

        file.close();
        std::cout << message::success();

        return true;
    }
    catch (std::invalid_argument &e)
    {
        std::cout << e.what() << endl;
    }

    file.close();

    reset();
    return false;
}

void Simulation::save_file(string &path)
{
    std::ofstream file(path);
    if (file.fail())
    {
        return;
    }

    file << get_n_foods() << endl;
    for (const auto &food : foods)
    {
        file << food->get_as_string() << endl;
    }

    file << get_n_anthills() << endl;
    for (const auto &anthill : anthills)
    {
        file << anthill->get_as_string() << endl;
    }

    file.close();
}

bool Simulation::step()
{
    generate_foods();

    for (auto &anthill : anthills)
    {
        if (!anthill->step(foods, anthills))
        {
            dead_anthills.push_back(std::move(anthill));
        };
    }

    anthills.erase(std::remove(anthills.begin(), anthills.end(), nullptr),
                   anthills.end());

    for (auto const &anthill : anthills)
    {
        anthill->clear_dead_ants();
    }

    dead_anthills.clear();

    // TODO(@danielpanero): return true / false if anthill.size=0
    return false;
}

void Simulation::reset()
{
    index_anthill = 0;
    first_execution = true;

    // We safely deallocate anthills and foods as they are unique_ptr
    anthills.clear();
    dead_anthills.clear();
    foods.clear();

    // We reset the squarecell grid and clear the model_surface
    Squarecell::grid_clear();
}

unsigned int Simulation::get_n_foods() const { return foods.size(); }
unsigned int Simulation::get_n_anthills() const { return anthills.size(); }

bool Simulation::get_info_prev_anthill(unsigned int &index, unsigned int &n_collectors,
                                       unsigned int &n_defensors,
                                       unsigned int &n_predators, double &n_food)
{
    return cycle_info_anthill(index, n_collectors, n_defensors, n_predators, n_food,
                              false);
}

bool Simulation::get_info_next_anthill(unsigned int &index, unsigned int &n_collectors,
                                       unsigned int &n_defensors,
                                       unsigned int &n_predators, double &n_food)
{
    return cycle_info_anthill(index, n_collectors, n_defensors, n_predators, n_food,
                              true);
}

bool Simulation::cycle_info_anthill(unsigned int &index, unsigned int &n_collectors,
                                    unsigned int &n_defensors,
                                    unsigned int &n_predators, double &n_food,
                                    bool order)
{
    if (anthills.empty())
    {
        return false;
    }

    if (first_execution)
    {
        first_execution = false;
    }
    else
    {
        index_anthill += order ? +1 : -1;
    }
    /** This expression prevents the index from exiting the boundaries [0,
     * anthills.size() - 1], e.g: for a vector of size 3:
     * 0 --> 0, -1 --> 2, 2 --> 2, 1 --> 1 ... (descending order)
     * 0 --> 0, 1 --> 1, 2 --> 2, 3 --> 0 ...  (ascending order)
     */
    index_anthill = (index_anthill + anthills.size()) % anthills.size();

    index = index_anthill;
    n_collectors = anthills[index_anthill]->get_number_of_collectors();
    n_defensors = anthills[index_anthill]->get_number_of_defensors();
    n_predators = anthills[index_anthill]->get_number_of_predators();
    n_food = anthills[index_anthill]->get_number_of_food();
    return true;
}

void Simulation::parse_foods(ifstream &file)
{
    string line(get_next_line(file));
    istringstream stream(line);

    unsigned int n_foods(0);
    stream >> n_foods;

    foods.resize(n_foods);

    unsigned int i(0);
    while (i < n_foods)
    {
        line = get_next_line(file);

        foods[i] = Food::parse_line(line);
        foods[i]->draw();

        i++;
    }
}

void Simulation::parse_anthills(ifstream &file)
{
    string line(get_next_line(file));
    istringstream stream(line);

    unsigned int n_anthills(0);
    stream >> n_anthills;

    anthills.resize(n_anthills);

    unsigned int i(0);
    while (i < n_anthills)
    {
        line = get_next_line(file);
        anthills[i] = Anthill::parse_line(line, i);

        auto collectors =
            parse_ants<Collector>(file, anthills[i]->get_number_of_collectors(), i);
        auto defensors =
            parse_ants<Defensor>(file, anthills[i]->get_number_of_defensors(), i);
        auto predators =
            parse_ants<Predator>(file, anthills[i]->get_number_of_predators(), i);

        anthills[i]->set_collectors(collectors);
        anthills[i]->set_defensors(defensors);
        anthills[i]->set_predators(predators);

        anthills[i]->draw();

        i++;
    }
}

template <typename T>
vector<std::unique_ptr<T>> Simulation::parse_ants(ifstream &file, unsigned int n,
                                                  unsigned int index_anthill)
{
    string line;
    vector<std::unique_ptr<T>> ants(n);

    unsigned int j(0);
    while (j < n)
    {
        line = get_next_line(file);

        ants[j] = T::parse_line(line, index_anthill);
        ants[j]->draw();

        j++;
    }

    return ants;
}

void Simulation::check_overlapping_anthills()
{
    for (size_t i = 0; i < anthills.size(); i++)
    {
        for (size_t j = 0; j < i; j++)
        {
            auto square1 = anthills[i]->get_as_square();
            auto square2 = anthills[j]->get_as_square();

            if (Squarecell::test_if_superposed_two_square(square1, square2))
            {
                throw std::invalid_argument(message::homes_overlap(i, j));
            }
        }
    }
}

void Simulation::check_generator_defensors_inside_anthills()
{
    for (size_t i = 0; i != anthills.size(); ++i)
    {
        anthills[i]->test_if_generator_defensors_perimeter(i);
    }
}

void Simulation::generate_foods()
{

    static std::uniform_int_distribution<unsigned> generate_coordinate(1, g_max - 2);
    static std::bernoulli_distribution b_distribution(food_rate);
    static std::default_random_engine random_num;

    unsigned int x = 0;
    unsigned int y = 0;
    unsigned int i = 0;
    bool found = false;

    while (i < max_food_trial && !found)
    {
        x = generate_coordinate(random_num);
        y = generate_coordinate(random_num);

        Squarecell::Square square{x, y, 1, true};

        if (!Squarecell::test_if_superposed_grid(square))
        {
            found = true;
            for (auto const &anthill : anthills)
            {
                if (Squarecell::test_if_superposed_two_square(
                        square, anthill->get_as_square()))
                {
                    found = false;
                }
            }
        }

        i++;
    }

    if (found && b_distribution(random_num))
    {
        std::unique_ptr<Food> food(new Food(x, y));
        food->draw();
        foods.push_back(std::move(food));
    }
}

string get_next_line(ifstream &file)
{
    string line;
    while (getline(file >> std::ws, line))
    {
        if (line[0] == '#')
        {
            continue;
        }
        return line;
    }
    return "";
}
