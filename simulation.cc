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

using std::endl;
using std::ifstream;
using std::istringstream;
using std::string;
using std::vector;

bool Simulation::read_file(string &path)
{
    reset();

    ifstream file(path);
    try
    {
        if (file.fail())
        {
            exit(EXIT_FAILURE);
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
        // TODO(@danielpanero): check if we want really to exit the program
        exit(EXIT_FAILURE);
    }

    file << n_foods << endl;
    for (const auto &food : foods)
    {
        file << food->get_as_string() << endl;
    }

    file << n_anthills << endl;
    for (const auto &anthill : anthills)
    {
        file << anthill->get_as_string() << endl;
    }

    file.close();
}

void Simulation::reset()
{
    n_foods = 0;
    n_anthills = 0;
    index_anthill = 0;

    // We safely disallocate anthills and foods as they are unique_ptr
    anthills.clear();
    foods.clear();

    // We reset the squarecell grid and clear the model_surface
    grid_clear();
}

unsigned int Simulation::get_n_foods() const { return n_foods; }

bool Simulation::get_info_prev_anthill(unsigned int &index, unsigned int &n_collectors,
                                       unsigned int &n_defensors,
                                       unsigned int &n_predators, unsigned int &n_food)
{
    return cycle_info_anthill(index, n_collectors, n_defensors, n_predators, n_food,
                              false);
}

bool Simulation::get_info_next_anthill(unsigned int &index, unsigned int &n_collectors,
                                       unsigned int &n_defensors,
                                       unsigned int &n_predators, unsigned int &n_food)
{
    return cycle_info_anthill(index, n_collectors, n_defensors, n_predators, n_food,
                              true);
}

bool Simulation::cycle_info_anthill(unsigned int &index, unsigned int &n_collectors,
                                    unsigned int &n_defensors,
                                    unsigned int &n_predators, unsigned int &n_food,
                                    bool order)
{
    if (order)
    {
        index_anthill++;
    }
    else
    {
        index_anthill--;
    }

    /** This expression prevents the index from exiting the boundaries [0,
     * anthills.size() - 1], e.g: for a vector of size 3:
     * 0 --> 0, -1 --> 2, 2 --> 2, 1 --> 1 ... (descending order)
     * 0 --> 0, 1 --> 1, 2 --> 2, 3 --> 0 ...  (ascending order)
     */
    index_anthill = (index_anthill + anthills.size()) % anthills.size();

    /** Since the index of an anthill must be invariant during the simulation, the
     * Anthills who were killed are removed and replaced by a nullptr. Therefore we
     * continue to reduce index till we find an non-nullptr element. After one cycle we
     * exit and return that there are no Anthill left
     */
    int tmp = index_anthill;
    while (anthills.at(tmp) == nullptr)
    {
        if (order)
        {
            tmp++;
        }
        else
        {
            tmp--;
        }
        tmp = (tmp + anthills.size()) % anthills.size();

        // After one cycle, we exit as there no anthills left
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