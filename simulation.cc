/**
 * @file simulation.cc
 * @author Daniel Panero [+261/-125], Layane Wazen [+133/-96], Andrea Diez [+21/-21]
 * @version 0.1
 * @date 2022-04-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "algorithm"
#include "fstream"
#include "iostream"
#include "sstream"
#include "stdexcept"
#include "vector"

#include "anthill.h"
#include "ants.h"
#include "food.h"
#include "message.h"

#include "simulation.h"

using std::ifstream;
using std::istringstream;
using std::string;
using std::vector;

bool Simulation::read_file(string &path)
{
    reset();

    try
    {
        ifstream file(path);
        if (file.fail())
        {
            exit(EXIT_FAILURE);
        }

        parse_foods(file);
        parse_anthills(file);

        check_overlapping_anthills();
        check_generator_defensors_inside_anthills();

        std::cout << message::success();
        file.close();

        return true;
    }
    catch (std::invalid_argument &e)
    {
        std::cout << e.what() << std::endl;
    }

    reset();
    return false;
}

void Simulation::save_file(std::string &path)
{
    std::ofstream file(path);
    if (file.fail())
    {
        exit(EXIT_FAILURE);
    }

    file << n_foods << std::endl;
    for (const auto &food : foods)
    {
        file << food->get_as_string() << std::endl;
    }

    file << n_anthills << std::endl;
    for (const auto &anthill : anthills)
    {
        file << anthill->get_as_string() << std::endl;
    }

    file.close();
}

unsigned int Simulation::get_n_foods() const { return n_foods; }

bool Simulation::get_info_prev_anthill(unsigned int &index, unsigned int &n_collectors,
                                       unsigned int &n_defensors,
                                       unsigned int &n_predators, unsigned int &n_food)
{
    /** This expression prevents the index from exiting the boundaries [0,
     * anthills.size() - 1]
     */
    index_anthill = (index_anthill - 1) % anthills.size();

    /** Since the index of anthill must be invariant during the simulation, the
     * Anthills who were killed are removed and replaced by a nullptr. Therefore we
     * continue to reduce index till we find an non-nullptr element. After one cycle we
     * exit and return that there are no Anthill left
     */
    int tmp = index_anthill;
    while (anthills.at(tmp) == nullptr)
    {
        tmp = (tmp - 1) % anthills.size();
        if (tmp == index_anthill)
        {
            return false;
        }
    }

    index = tmp;
    n_collectors = anthills[tmp]->get_number_of_collectors();
    n_defensors = anthills[tmp]->get_number_of_defensors();
    n_predators = anthills[tmp]->get_number_of_predators();
    n_food = anthills[tmp]->get_number_of_food();

    return true;
}

bool Simulation::get_info_next_anthill(unsigned int &index, unsigned int &n_collectors,
                                       unsigned int &n_defensors,
                                       unsigned int &n_predators, unsigned int &n_food)
{
    index_anthill = (index_anthill + 1) % anthills.size();

    /** Since the index of anthill must be invariant during the simulation, the
     * Anthills who were killed are removed and replaced by a nullptr. Therefore we
     * continue to increase index till we find an non-nullptr element. After one cycle
     * we exit and return that there are no Anthill left
     */
    int tmp = index_anthill;
    while (anthills.at(tmp) == nullptr)
    {
        tmp = (tmp + 1) % anthills.size();
        if (tmp == index_anthill)
        {
            return false;
        }
    }

    index = tmp;
    n_collectors = anthills[tmp]->get_number_of_collectors();
    n_defensors = anthills[tmp]->get_number_of_defensors();
    n_predators = anthills[tmp]->get_number_of_predators();
    n_food = anthills[tmp]->get_number_of_food();

    return true;
}

void Simulation::parse_foods(ifstream &file)
{
    string line(get_next_line(file));
    istringstream stream(line);

    stream >> n_foods;
    foods.resize(n_foods);

    unsigned int i(0);
    while (i < n_foods)
    {
        line = get_next_line(file);
        foods[i] = Food::parse_line(line);
        i++;
    }
}

void Simulation::parse_anthills(ifstream &file)
{
    string line(get_next_line(file));
    istringstream stream(line);

    stream >> n_anthills;
    anthills.resize(n_anthills);

    unsigned int i(0);
    while (i < n_anthills)
    {
        line = get_next_line(file);
        anthills[i] = Anthill::parse_line(line);

        auto collectors =
            parse_ants<Collector>(file, anthills[i]->get_number_of_collectors());
        auto defensors =
            parse_ants<Defensor>(file, anthills[i]->get_number_of_defensors());
        auto predators =
            parse_ants<Predator>(file, anthills[i]->get_number_of_predators());

        anthills[i]->set_collectors(collectors);
        anthills[i]->set_defensors(defensors);
        anthills[i]->set_predators(predators);

        i++;
    }
}

template <typename T>
vector<std::unique_ptr<T>> Simulation::parse_ants(std::ifstream &file, unsigned int n)
{
    string line;
    vector<std::unique_ptr<T>> ants(n);

    unsigned int j(0);
    while (j < n)
    {
        line = get_next_line(file);
        ants[j] = T::parse_line(line);
        j++;
    }

    return ants;
}

void Simulation::reset()
{
    n_foods = 0;
    n_anthills = 0;

    anthills.clear();
    foods.clear();

    clear_grid();
}

void Simulation::check_overlapping_anthills()
{
    for (size_t i = 0; i < anthills.size(); i++)
    {
        for (size_t j = 0; j < i; j++)
        {
            Square square1(anthills[i]->get_as_square());
            Square square2(anthills[j]->get_as_square());

            if (test_if_superposed_two_square(square1, square2))
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