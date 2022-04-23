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
#include "memory"
#include "sstream"
#include "stdexcept"
#include "vector"

#include "element.h"
#include "message.h"
#include "squarecell.h"

#include "anthill.h"

using std::istringstream;
using std::string;
using std::unique_ptr;
using std::vector;

Anthill::Anthill(unsigned int &x, unsigned int &y, unsigned int &side,
                 unsigned int &xg, unsigned int &yg, unsigned int n_food,
                 unsigned int &n_collectors, unsigned int &n_defensors,
                 unsigned int &n_predators)
    : Element{x, y, side, false}, n_food(n_food), n_collectors(n_collectors),
      n_defensors(n_defensors), n_predators(n_predators)
{
    unsigned int generator_age(0);
    generator = unique_ptr<Generator>(new Generator(xg, yg, generator_age));

    test_square(*this);
}

void Anthill::test_if_generator_defensors_perimeter(unsigned int index)
{
    Square generator_square = generator->get_as_square();
    if (!test_if_completely_confined(generator_square, *this))
    {
        throw std::invalid_argument(message::generator_not_within_home(
            generator_square.x, generator_square.y, index));
    }

    for (auto &defensor : defensors)
    {
        Square defensor_square = defensor->get_as_square();
        if (!test_if_completely_confined(defensor_square, *this))
        {
            throw std::invalid_argument(message::defensor_not_within_home(
                defensor_square.x, defensor_square.y, index));
        }
    }
}

void Anthill::set_collectors(vector<unique_ptr<Collector>> &collectors)
{
    this->collectors = std::move(collectors);
}
void Anthill::set_defensors(vector<unique_ptr<Defensor>> &defensors)
{
    this->defensors = std::move(defensors);
}
void Anthill::set_predators(vector<unique_ptr<Predator>> &predators)
{
    this->predators = std::move(predators);
}

unsigned int Anthill::get_number_of_collectors() const { return n_collectors; };
unsigned int Anthill::get_number_of_defensors() const { return n_defensors; };
unsigned int Anthill::get_number_of_predators() const { return n_predators; };
unsigned int Anthill::get_number_of_food() const { return n_food; }

string Anthill::get_as_string()
{
    using std::to_string;

    return to_string(x) + " " + to_string(y) + " " + to_string(side) + " " +
           generator->get_as_string() + " " + to_string(n_food) + " " +
           to_string(n_collectors) + " " + to_string(n_defensors) + " " +
           to_string(n_predators);
}

void Anthill::draw(unsigned int &color_index) { draw_only_border(*this, color_index); }

unique_ptr<Anthill> Anthill::parse_line(string &line)
{
    unsigned int x(0);
    unsigned int y(0);
    unsigned int side(0);
    unsigned int xg(0);
    unsigned int yg(0);

    unsigned int n_food(0);
    unsigned int n_collectors(0);
    unsigned int n_defensors(0);
    unsigned int n_predators(0);

    istringstream stream(line);

    stream >> x;
    stream >> y;
    stream >> side;
    stream >> xg;
    stream >> yg;
    stream >> n_food;
    stream >> n_collectors;
    stream >> n_defensors;
    stream >> n_predators;

    return unique_ptr<Anthill>(new Anthill(x, y, side, xg, yg, n_food, n_collectors,
                                           n_defensors, n_predators));
}
