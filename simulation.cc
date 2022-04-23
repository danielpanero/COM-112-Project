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
#include "vector"

#include "anthill.h"
#include "ants.h"
#include "food.h"
#include "message.h"

#include "simulation.h"

using std::cout;
using std::ifstream;
using std::istringstream;
using std::string;
using std::vector;

bool Simulation::read_file(string &path)
{
    return true;
    ifstream file(path);
    if (file.fail())
    {
        exit(EXIT_FAILURE);
    }

    parse_foods(file);
    parse_anthills(file);

    check_overlapping_anthills();
    check_generator_defensors_inside_anthills();

    cout << message::success();
    file.close();
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
                cout << message::homes_overlap(i, j);
                exit(EXIT_FAILURE);
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