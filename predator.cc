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

bool Predator::step()
{
    if (!increase_age())
    {
        return false;
    }

    return true;
}

void Predator::remain_inside(Squarecell::Square anthill)
{
    auto origin = get_as_square();

    remove_from_grid();
    undraw();

    auto move = Squarecell::lee_algorithm(origin, anthill, &Predator::generate_l_moves,
                                          &Squarecell::test_if_completely_confined);

    x = move.x;
    y = move.y;

    add_to_grid();
    draw();
}

void Predator::attack(unique_ptr<Ant> &ant) {}

vector<Squarecell::Square> Predator::generate_l_moves(Square origin)
{
    vector<Squarecell::Square> moves;

    // x: +-3, y: +- 1

    for (int i(1); i <= 2; i++)
    {
        for (int j(1); j <= 2; j++)
        {
            Squarecell::Square move(origin);

            move.x += j == 1 ? 3 : -3;
            move.y += i == 1 ? 1 : -1;

            unsigned int x = Squarecell::get_coordinate_x(move);
            unsigned int y = Squarecell::get_coordinate_y(move);

            // We check if the proposed new positions are inside the model
            if (x >= 0 && y >= 0 && y <= g_max - 1 && x <= g_max - 1 &&
                x + move.side <= g_max && y + move.side <= g_max)
            {
                moves.push_back(move);
            }
        }
    }

    // x: +-1, y: +-2

    for (int i(1); i <= 2; i++)
    {
        for (int j(1); j <= 2; j++)
        {
            Squarecell::Square move(origin);

            move.x += j == 1 ? 1 : -1;
            move.y += i == 1 ? 3 : -3;

            unsigned int x = Squarecell::get_coordinate_x(move);
            unsigned int y = Squarecell::get_coordinate_y(move);

            // We check if the proposed new positions are inside the model
            if (x >= 0 && y >= 0 && y <= g_max - 1 && x <= g_max - 1 &&
                x + move.side <= g_max && y + move.side <= g_max)
            {
                moves.push_back(move);
            }
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
