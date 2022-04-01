#include "algorithm"
#include "fstream"
#include "iostream"
#include "sstream"
#include "vector"

#include "entities/anthill.h"
#include "entities/ants.h"
#include "entities/food.h"
#include "message.h"

#include "simulation.h"

using std::cout;
using std::ifstream;
using std::istringstream;
using std::string;
using std::vector;

string get_next_line(ifstream &file);

void Simulation::read_file(string &path)
{
    ifstream file(path);
    if (file.fail())
    {
        exit(EXIT_FAILURE);
    }

    parse_foods(file);
    parse_anthills(file);

    check_overlapping_anthills();
    generator_defensor(file);
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
        foods[i] = new Food(line);
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
        anthills[i] = new Anthill(line);
        parse_predators(file, *(anthills[i]));
        parse_collectors(file, *(anthills[i]));
        parse_defensors(file, *(anthills[i]));
    }
}

void Simulation::parse_collectors(ifstream &file, Anthill &anthill)
{
    string line(get_next_line(file));
    istringstream stream(line);

    unsigned int n_collectors = anthill.get_number_of_collectors();
    vector<Collector *> collectors(n_collectors);

    unsigned int j = 0;
    while (j < n_collectors)
    {
        line = get_next_line(file);
        collectors[j] = new Collector(line);
        j++;
    }
    anthill.set_collectors(collectors);
}
void Simulation::parse_defensors(ifstream &file, Anthill &anthill)
{
    string line(get_next_line(file));
    istringstream stream(line);
    line = get_next_line(file);

    unsigned int n_defensors = anthill.get_number_of_defensors();
    vector<Defensor *> defensors(n_defensors);

    unsigned int j = 0;
    while (j < n_defensors)
    {
        line = get_next_line(file);
        defensors[j] = new Defensor(line);
        j++;
    }
    anthill.set_defensors(defensors);
}

void Simulation::parse_predators(ifstream &file, Anthill &anthill)
{
    string line(get_next_line(file));
    istringstream stream(line);
    line = get_next_line(file);

    unsigned int n_predators = anthill.get_number_of_predators();
    vector<Predator *> predators(n_predators);

    unsigned int j(0);
    while (j < n_predators)
    {
        line = get_next_line(file);
        predators[j] = new Predator(line);
        j++;
    }
    anthill.set_predators(predators);
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
void Simulation::generator_defensor(ifstream &file)
{
    for (size_t i = 0; i != anthills.size(); ++i)
    {
        anthills[i]->test_if_generator_defensors_perimeter(i);
    }

    cout << message::success();
}

string get_next_line(ifstream &stream)
{
    string line;
    while (getline(stream >> std::ws, line))
    {
        if (line[0] == '#')
        {
            continue;
        }
        return line;
    }
    return "";
}