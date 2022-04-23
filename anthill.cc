/**
 * @file anthill.cc
 * @author Daniel Panero [+90/-14], Layane Wazen [+10/-0], Andrea Diez [+40/-39]
 * @version 0.1
 * @date 2022-04-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "iostream"
#include "sstream"
#include "vector"

#include "message.h"
#include "squarecell.h"

#include "anthill.h"

using std::cout;
using std::istringstream;
using std::string;
using std::vector;

Anthill::Anthill(unsigned int &x, unsigned int &y, unsigned int &side,
                 unsigned int &xg, unsigned int &yg, unsigned int total_food,
                 unsigned int &n_collectors, unsigned int &n_defensors,
                 unsigned int &n_predators)
    : Square({x, y, side}), total_food(total_food), n_collectors(n_collectors),
      n_defensors(n_defensors), n_predators(n_predators),
      generator(new Generator(xg, yg))
{
    test_square(*this);
}

Anthill *Anthill::parse_line(string &line)
{
    unsigned int x(0);
    unsigned int y(0);
    unsigned int side(0);
    unsigned int xg(0);
    unsigned int yg(0);
    unsigned int total_food(0);
    unsigned int n_collectors(0);
    unsigned int n_defensors(0);
    unsigned int n_predators(0);

    istringstream stream(line);

    stream >> x;
    stream >> y;
    stream >> side;
    stream >> xg;
    stream >> yg;
    stream >> total_food;
    stream >> n_collectors;
    stream >> n_defensors;
    stream >> n_predators;

    return std::unique_ptr<Anthill>(new Anthill(
        x, y, side, xg, yg, total_food, n_collectors, n_defensors, n_predators));
}

void Anthill::test_if_generator_defensors_perimeter(unsigned int index)
{
    Square generator_square = generator->get_as_square();
    if (!test_if_completely_confined(generator_square, *this))
    {
        cout << message::generator_not_within_home(generator_square.x,
                                                   generator_square.y, index);
        exit(EXIT_FAILURE);
    }

    for (auto defensor : defensors)
    {
        Square defensor_square = defensor->get_as_square();
        if (!test_if_completely_confined(defensor_square, *this))
        {
            cout << message::defensor_not_within_home(defensor_square.x,
                                                      defensor_square.y, index);
            exit(EXIT_FAILURE);
        }
    }
}

void Anthill::set_collectors(vector<Collector *> &collectors)
{
    this->collectors = collectors;
}
void Anthill::set_defensors(vector<Defensor *> &defensors)
{
    this->defensors = defensors;
}
void Anthill::set_predators(vector<Predator *> &predators)
{
    this->predators = predators;
}

unsigned int Anthill::get_number_of_collectors() const { return n_collectors; };
unsigned int Anthill::get_number_of_defensors() const { return n_defensors; };
unsigned int Anthill::get_number_of_predators() const { return n_predators; };

Square Anthill::get_as_square() { return {*this}; }