/**
 * @file defensor.cc
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

#include "defensor.h"

using std::istringstream;
using std::string;
using std::unique_ptr;
using std::vector;

// ====================================================================================
// Initialization - Misc

Defensor::Defensor(unsigned int x, unsigned int y, unsigned int age,
                   unsigned int color_index)
    : Ant{x, y, sizeD, age, color_index}
{
    Squarecell::test_square(*this);
    add_to_grid();
}

Defensor::~Defensor()
{
    remove_from_grid();
    undraw();
}

void Defensor::add_to_grid()
{
    unsigned int superposed_x(0);
    unsigned int superposed_y(0);

    if (Squarecell::test_if_superposed_grid(*this, superposed_x, superposed_y))
    {
        throw std::invalid_argument(
            message::defensor_overlap(x, y, superposed_x, superposed_y));
    }

    Squarecell::add_square(*this);
}

void Defensor::remove_from_grid() { Squarecell::remove_square(*this); }

void Defensor::draw() { Squarecell::draw_plus_pattern(*this, get_color_index()); }

void Defensor::undraw() { Squarecell::undraw_square(*this); }

// ====================================================================================
// Simulation

bool Defensor::step(Square &anthill_square)
{
    if (!increase_age())
    {
        return false;
    }

    remove_from_grid();
    undraw();

    auto move =
        Squarecell::lee_algorithm(*this, anthill_square, &Defensor::generate_moves,
                                  &Defensor::test_if_confined_and_near_border);

    x = move.x;
    y = move.y;

    if (!Squarecell::test_if_completely_confined(*this, anthill_square))
    {
        return false;
    }

    add_to_grid();
    draw();

    return true;
}

bool Defensor::test_if_contact_collector(Squarecell::Square &collector_square)
{
    return Squarecell::test_if_border_touches(*this, collector_square);
}

bool Defensor::test_if_confined_and_near_border(Square const &origin,
                                                Square const &anthill)
{
    return Squarecell::test_if_completely_confined(origin, anthill) &&
           Squarecell::test_if_border_touches(origin, anthill);
}

vector<Squarecell::Square> Defensor::generate_moves(Square origin)
{
    // All the possible shifts combination: RIGHT, LEFT, TOP, BOTTOM
    vector<int> x_shift{1, -1, 0, 0};
    vector<int> y_shift{0, 0, 1, -1};

    return Ant::generate_moves(origin, x_shift, y_shift);
}

unique_ptr<Defensor> Defensor::parse_line(string &line, unsigned int color_index)
{
    unsigned int x(0);
    unsigned int y(0);
    unsigned int age(0);

    istringstream stream(line);
    stream >> x;
    stream >> y;
    stream >> age;

    return unique_ptr<Defensor>(new Defensor(x, y, age, color_index));
}
