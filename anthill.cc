/**
 * @file anthill.cc
 * @author Daniel Panero [+90/-14], Layane Wazen [+10/-0], Andrea Diez [+193/-135]
 * @version 0.1
 * @date 2022-04-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <cmath>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "element.h"
#include "message.h"
#include "squarecell.h"

#include "anthill.h"

using std::istringstream;
using std::string;
using std::unique_ptr;
using std::vector;

constexpr double g_max(128);

Anthill::Anthill(unsigned int x, unsigned int y, unsigned int side, unsigned int xg,
                 unsigned int yg, unsigned int n_food, unsigned int n_collectors,
                 unsigned int n_defensors, unsigned int n_predators,
                 unsigned int color_index)
    : Element{x, y, side, false, color_index}, n_food(n_food),
      n_collectors(n_collectors), n_defensors(n_defensors), n_predators(n_predators),
      generator(new Generator(xg, yg, 0, color_index))
{
    Squarecell::test_square(*this);
}

void Anthill::test_if_generator_defensors_perimeter(unsigned int index)
{
    auto generator_square = generator->get_as_square();
    if (!Squarecell::test_if_completely_confined(generator_square, *this))
    {
        throw std::invalid_argument(message::generator_not_within_home(
            generator_square.x, generator_square.y, index));
    }

    for (auto &defensor : defensors)
    {
        auto defensor_square = defensor->get_as_square();
        if (!Squarecell::test_if_completely_confined(defensor_square, *this))
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

    string tmp = "\n" + to_string(x) + " " + to_string(y) + " " + to_string(side) +
                 " " + generator->get_as_string() + " " + to_string(n_food) + " " +
                 to_string(n_collectors) + " " + to_string(n_defensors) + " " +
                 to_string(n_predators) + "\n";

    for (auto const &collector : collectors)
    {
        tmp += collector->get_as_string() + "\n";
    }

    for (auto const &defensor : defensors)
    {
        tmp += defensor->get_as_string() + "\n";
    }

    for (auto const &predator : predators)
    {
        tmp += predator->get_as_string() + "\n";
    }

    return tmp;
}

void Anthill::draw()
{
    Squarecell::draw_only_border(*this, get_color_index());
    generator->draw();
}

unique_ptr<Anthill> Anthill::parse_line(string &line, unsigned int color_index)
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
                                           n_defensors, n_predators, color_index));
}

bool Anthill::step(std::vector<std::unique_ptr<Food>> foods,
                   std::vector<std::unique_ptr<Anthill>> anthills)
{
    
}

bool Anthill::try_test_corner_expansion(std::vector<std::unique_ptr<Anthill>> anthills)
{
    unsigned int sizeF =
        sqrt(4 * (sizeG * sizeG + sizeC * sizeC * n_collectors +
                  sizeD * sizeD * n_defensors + sizeP * sizeP * n_predators));

    for (int k = 0; k <= 2; k++)
    {
        Square anthill(*this);
        anthill.side = sizeF + 2;

        for (int t = 0; t <= 2; t++)
        {
            if (k == 1)
            {
                anthill.x += t == 1 ? 1 : -1;
            }
            else
            {

                anthill.y += t == 1 ? 1 : -1;
            }
            if (!test_corner_expansion(anthills, anthill))
            {
                continue;
            }
            else
            {
                side = sizeF + 2;
                x = anthill.x;
                y = anthill.y;
                return true;
            }
        }
    }
    return false;
}

bool Anthill::test_corner_expansion(std::vector<std::unique_ptr<Anthill>> anthills,
                                    Square anthill)
{
    try
    {
        Squarecell::test_square(anthill);
        for (size_t i = 0; i < anthills.size(); i++)
        {
            auto anthill2 = anthills[i]->get_as_square();
            if (Squarecell::test_if_superposed_two_square(anthill, anthill2))
            {
                return false;
            }
        }
    }
    catch (const std::invalid_argument e)
    {
        return true;
    }
}
