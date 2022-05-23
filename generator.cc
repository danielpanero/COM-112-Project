/**
 * @file generator.cc
 * @author Daniel Panero [+135/-10], Layane Wazen [+10/-0], Andrea Diez [+367/-204]
 * @version 0.1
 * @date 2022-04-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <stdexcept>

#include "message.h"
#include "squarecell.h"

#include "generator.h"

using std::istringstream;
using std::string;
using std::vector;

// ====================================================================================
// Initialization - Misc

Generator::Generator(unsigned int x, unsigned int y, unsigned int age,
                     unsigned int color_index)
    : Ant{x, y, sizeG, age, color_index}
{
    Squarecell::test_square(*this);
    add_to_grid();
}

Generator::~Generator()
{
    remove_from_grid();
    undraw();
}

void Generator::add_to_grid()
{
    unsigned int superposed_x(0);
    unsigned int superposed_y(0);

    if (Squarecell::test_if_superposed_grid(*this, superposed_x, superposed_y))
    {
        throw std::invalid_argument(
            message::generator_overlap(x, y, superposed_x, superposed_y));
    }

    Squarecell::add_square(*this);
}

void Generator::remove_from_grid() { Squarecell::remove_square(*this); }

void Generator::draw() { Squarecell::draw_filled(*this, get_color_index()); }

void Generator::undraw() { Squarecell::undraw_square(*this); }

string Generator::get_as_string()
{
    return std::to_string(x) + " " + std::to_string(y);
}

// ====================================================================================
// Simulation

bool Generator::step(const Square &anthill)
{
    remove_from_grid();
    undraw();

    auto move =
        Squarecell::lee_algorithm(*this, anthill, &Generator::generate_moves,
                                  &Generator::test_if_confined_and_not_near_border);

    x = move.x;
    y = move.y;

    add_to_grid();
    draw();

    return Generator::test_if_confined_and_not_near_border(*this, anthill);
}

vector<Squarecell::Square> Generator::generate_moves(Square origin)
{
    // All the possible shifts combination: RIGHT, LEFT, TOP, BOTTOM, TOP-RIGHT...
    vector<int> x_shift{1, -1, 0, 0, 1, 1, -1, -1};
    vector<int> y_shift{0, 0, 1, -1, 1, -1, 1, -1};

    return Ant::generate_moves(origin, x_shift, y_shift);
}

bool Generator::test_if_confined_and_not_near_border(Square const &origin,
                                                     Square const &anthill)
{
    return Squarecell::test_if_completely_confined(origin, anthill);
}
