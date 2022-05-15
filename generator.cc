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

// TODO(@danielpanero): check if we want to add check move in squarecell as they are
// all pretty much the same
unsigned int const g_max(128);

// ====================================================================================
// Initialization - Misc

Generator::Generator(unsigned int x, unsigned int y, unsigned int age,
                     unsigned int color_index)
    : Ant{x, y, sizeG, age, color_index}
{
    Squarecell::test_square(*this);
    add_to_grid();
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

bool Generator::step(Square anthill)
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
    constexpr static int x_shift[8] = {1, -1, 0, 0, 1, 1, -1, -1};
    constexpr static int y_shift[8] = {0, 0, 1, -1, 1, -1, 1, -1};

    vector<Squarecell::Square> moves;

    for (int i(0); i <= 8; i++)
    {
        Squarecell::Square move(origin);

        move.x += x_shift[i];
        move.y += y_shift[i];

        unsigned int x = Squarecell::get_coordinate_x(move);
        unsigned int y = Squarecell::get_coordinate_y(move);

        // We check if the proposed new positions are inside the model
        // TODO(@danielpanero): when replaced unsigned int with x, it will suffice to
        // check that x >= 0 and we can group all this function in squarecell
        if (move.x >= 2 && move.y >= 2 && x + move.side <= g_max - 1 &&
            y + move.side <= g_max - 1)
        {
            moves.push_back(move);
        }
    }

    return moves;
}

bool Generator::test_if_confined_and_not_near_border(Square &origin, Square &anthill)
{
    if (Squarecell::test_if_completely_confined(origin, anthill))
    {
        // TODO(@danielpanero): fix test_if_border_touches for internal usage
        return !Squarecell::test_if_border_touches(origin, anthill);
    }

    return false;
}
