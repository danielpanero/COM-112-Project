/**
 * @file predator.cc
 * @author Daniel Panero [+135/-10], Layane Wazen [+10/-0], Andrea Diez [+367/-204]
 * @version 0.1
 * @date 2022-04-01
 *
 * @copyright Copyright (c) 2022
 *
 */

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

unsigned int const g_max(128);

// TODO(@danielpanero): multiple ants near + possible segfault maybe pass directly
// anthill with a function near ants? Predator vs Defensor -> nothing and Defensor
// + Defensor ... define function attacked for each ants...

// ====================================================================================
// Initialization - Misc

Predator::Predator(unsigned int x, unsigned int y, unsigned int age,
                   unsigned int color_index)
    : Ant{x, y, sizeP, age, color_index}
{
    Squarecell::test_square(*this);
    add_to_grid();
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
        Squarecell::lee_algorithm(*this, anthill_square, &Predator::generate_l_moves,
                                  &Squarecell::test_if_completely_confined);

    x = move.x;
    y = move.y;

    add_to_grid();
    draw();
}

vector<Squarecell::Square> Predator::generate_l_moves(Square origin)
{
    // All the possible shifts combinations:
    constexpr static int x_shift[8] = {1, 1, -1, -1, 3, 3, -3, -3};
    constexpr static int y_shift[8] = {3, -3, 3, -3, 1, -1, 1, -1};

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
        if (move.x >= 0 && move.y >= 0 && x + move.side <= g_max - 1 &&
            y + move.side <= g_max - 1)
        {
            moves.push_back(move);
        }
    }

    return moves;
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
