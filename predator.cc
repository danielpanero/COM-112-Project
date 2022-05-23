/**
 * @file predator.cc
 * @author Daniel Panero 55%, Andrea Diez 45%
 * @version 0.1
 * @date 2022-04-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <cmath>
#include <memory>
#include <sstream>
#include <stdexcept>

#include "message.h"
#include "squarecell.h"

#include "predator.h"

using std::istringstream;
using std::string;
using std::unique_ptr;
using std::vector;

// ====================================================================================
// Initialization - Misc

Predator::Predator(unsigned int x, unsigned int y, unsigned int age,
                   unsigned int color_index)
    : Ant{x, y, sizeP, age, color_index}
{
    Squarecell::test_square(*this);
    add_to_grid();
}

Predator::~Predator()
{
    remove_from_grid();
    undraw();
}

void Predator::add_to_grid()
{
    if (Squarecell::test_if_superposed_grid(*this))
    {
        throw std::invalid_argument(message::predator_overlap(x, y));
    }

    Squarecell::add_square(*this);
}

void Predator::remove_from_grid() { Squarecell::remove_square(*this); }

void Predator::draw() { Squarecell::draw_filled(*this, get_color_index()); }

void Predator::undraw() { Squarecell::undraw_square(*this); }

// ====================================================================================
// Simulation

bool Predator::step() { return increase_age(); }

void Predator::remain_inside(Squarecell::Square &anthill_square)
{
    remove_from_grid();
    undraw();

    auto move =
        Squarecell::lee_algorithm(*this, anthill_square, &Predator::generate_moves,
                                  &Squarecell::test_if_completely_confined);

    x = move.x;
    y = move.y;

    add_to_grid();
    draw();
}

void Predator::move_toward_nearest_ant(vector<Squarecell::Square> &ants)
{
    auto target = find_target_ant(ants);

    remove_from_grid();
    undraw();

    Squarecell::remove_square(target);

    auto move = Squarecell::lee_algorithm(*this, target, &Predator::generate_moves,
                                          &Predator::test_if_reached_ant);

    x = move.x;
    y = move.y;

    if (!test_if_reached_ant(*this, target))
    {
        Squarecell::add_square(target);
    }

    add_to_grid();
    draw();
}

Squarecell::Square Predator::find_target_ant(vector<Squarecell::Square> &ants)
{
    auto target = ants.front();

    unsigned int best_distance = std::pow(x - target.x, 2) + std::pow(y - target.y, 2);
    for (auto &ant : ants)
    {
        unsigned int distance = std::pow(x - ant.x, 2) + std::pow(y - ant.y, 2);
        if (distance < best_distance)
        {
            target = ant;
            best_distance = distance;
        }
    }

    return target;
}

bool Predator::filter_ants(State_anthill state, Squarecell::Square &anthill,
                           Squarecell::Square &ant)
{
    if (state == CONSTRAINED)
    {
        return true;
    }

    return Squarecell::test_if_completely_confined(ant, anthill);
}

vector<Squarecell::Square> Predator::generate_moves(Square origin)
{
    // All the possible shifts combinations:
    vector<int> x_shift{1, 1, -1, -1, 3, 3, -3, -3};
    vector<int> y_shift{3, -3, 3, -3, 1, -1, 1, -1};

    return Ant::generate_moves(origin, x_shift, y_shift);
}

bool Predator::test_if_reached_ant(Squarecell::Square const &origin,
                                   Squarecell::Square const &ant)
{
    return Squarecell::test_if_border_touches(origin, ant) ||
           Squarecell::test_if_superposed_two_square(origin, ant);
}

unique_ptr<Predator> Predator::parse_line(string &line, unsigned int color_index)
{
    unsigned int x(0);
    unsigned int y(0);
    unsigned int age(0);

    istringstream stream(line);
    stream >> x;
    stream >> y;
    stream >> age;

    return unique_ptr<Predator>(new Predator(x, y, age, color_index));
}
