/**
 * @file anthill.cc
 * @author Daniel Panero [+90/-14], Layane Wazen [+10/-0], Andrea Diez [+193/-135]
 * @version 0.1
 * @date 2022-04-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <algorithm>
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

// ====================================================================================
// Initialization - Misc

Anthill::Anthill(unsigned int x, unsigned int y, unsigned int side, unsigned int xg,
                 unsigned int yg, unsigned int n_food, unsigned int n_collectors,
                 unsigned int n_defensors, unsigned int n_predators,
                 unsigned int color_index)
    : Element{x, y, side, false, color_index}, n_food(n_food),
      generator(new Generator(xg, yg, 0, color_index))
{
    Squarecell::test_square(*this);

    // We preallocate the vectors, so when get_number_of... is called it return the
    // right size
    collectors.resize(n_collectors);
    defensors.resize(n_defensors);
    predators.resize(n_predators);
}

Anthill::~Anthill() { undraw(); }

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

unsigned int Anthill::get_number_of_collectors() const { return collectors.size(); };
unsigned int Anthill::get_number_of_defensors() const { return defensors.size(); };
unsigned int Anthill::get_number_of_predators() const { return predators.size(); };
unsigned int Anthill::get_number_of_food() const { return n_food; }

void Anthill::draw()
{
    Squarecell::draw_only_border(*this, get_color_index());
    generator->draw();
}

void Anthill::undraw() { Squarecell::undraw_thick_border_square(*this); }

string Anthill::get_as_string()
{
    using std::to_string;

    string tmp = "\n" + to_string(x) + " " + to_string(y) + " " + to_string(side) +
                 " " + generator->get_as_string() + " " + to_string(n_food) + " " +
                 to_string(get_number_of_collectors()) + " " +
                 to_string(get_number_of_defensors()) + " " +
                 to_string(get_number_of_predators()) + "\n";

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

// ====================================================================================
// Simulation

bool Anthill::step(vector<unique_ptr<Food>> &foods)
{
    undraw();

    // TODO(@danielpanero): implement sizeF
    // TODO(@danielpanero): implement food consuption food <= 0 --> return false

    if (!generator->step(*this))
    {
        for (auto &collector : collectors)
        {
            drop_food_collector(foods, collector);
        }

        return false;
    }

    update_collectors(foods);
    update_defensors();
    update_predators();

    draw();

    return true;
}

void Anthill::update_collectors(vector<unique_ptr<Food>> &foods)
{
    for (auto &collector : collectors)
    {
        if (!collector->step())
        {
            drop_food_collector(foods, collector);

            dead_ants.push_back(std::move(collector));
            continue;
        }

        if (collector->get_state() == EMPTY)
        {
            size_t target = 0;
            if (collector->find_target_food(foods, target))
            {
                if (collector->search_food(foods.at(target)))
                {
                    std::swap(foods.at(target), foods.back());
                    foods.pop_back();
                };
            }
            else
            {
                // TODO(@danielpanero) implement secondary goal
            }
        }
        else
        {
            if (collector->return_to_anthill(*this))
            {
                n_food++;
            }
        }
    }

    collectors.erase(std::remove(collectors.begin(), collectors.end(), nullptr),
                     collectors.end());
}

void Anthill::update_defensors()
{
    for (auto &defensor : defensors)
    {
        if (!defensor->step(*this))
        {
            dead_ants.push_back(std::move(defensor));
            continue;
        }
    }

    defensors.erase(std::remove(defensors.begin(), defensors.end(), nullptr),
                    defensors.end());
}

void Anthill::update_predators()
{
    for (auto &predator : predators)
    {
        if (!predator->step())
        {
            dead_ants.push_back(std::move(predator));
            continue;
        }

        if (state == CONSTRAINED)
        {
            // TODO(@danielpanero): implement attacking nearest ant
        }
        else
        {
            // TODO(@danielpanero): implement checking if any ants from other anthills
            // is inside
            predator->remain_inside(*this);
        }
    }

    predators.erase(std::remove(predators.begin(), predators.end(), nullptr),
                    predators.end());
}

void Anthill::drop_food_collector(vector<unique_ptr<Food>> &foods,
                                  unique_ptr<Collector> &collector)
{
    if (collector->get_state() == LOADED)
    {
        // In order to add the new food we have first to empty the grid
        auto collector_square = collector->get_as_square();
        Squarecell::remove_square(collector_square);

        unique_ptr<Food> food(new Food{collector_square.x, collector_square.y});
        foods.push_back(std::move(food));

        /** We have to add back the square as the zone will be free only after
         * all the Anthills are updated */
        Squarecell::add_square(collector_square);
    }
}

void Anthill::clear_dead_ants()
{
    undraw();

    dead_ants.clear();

    draw();
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
