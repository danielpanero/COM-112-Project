/**
 * @file defensor.cc
 * @author Daniel Panero [+135/-10], Layane Wazen [+10/-0], Andrea Diez [+367/-204]
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

unsigned int const g_max(128);

Defensor::Defensor(unsigned int x, unsigned int y, unsigned int age,
                   unsigned int color_index)
    : Ant{x, y, sizeD, age, color_index}
{
    Squarecell::test_square(*this);
    add_to_grid();
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

bool Defensor::step(Square anthill)
{
    if (!increase_age())
    {
        return false;
    }

    auto origin = get_as_square();

    remove_from_grid();
    undraw();

    auto move =
        Squarecell::lee_algorithm(origin, anthill, &Defensor::generate_hv_moves,
                                  &Defensor::test_if_confined_and_near_border);

    x = move.x;
    y = move.y;

    add_to_grid();
    draw();

    return true;
}

bool Defensor::test_if_confined_and_near_border(Square &origin, Square &anthill)
{
    return Squarecell::test_if_completely_confined(origin, anthill) &&
           Squarecell::test_if_border_touches(origin, anthill);
}

vector<Squarecell::Square> Defensor::generate_hv_moves(Square origin)
{
    vector<Squarecell::Square> moves;

    for (int i(1); i <= 2; i++)
    {
        for (int j(1); j <= 2; j++)
        {
            Squarecell::Square move(origin);

            if (i == 1)
            {
                move.x += j == 1 ? 1 : -1;
            }
            else
            {

                move.y += j == 1 ? 1 : -1;
            }

            unsigned int x = Squarecell::get_coordinate_x(move);
            unsigned int y = Squarecell::get_coordinate_y(move);

            // We check if the proposed new positions are inside the model
            if (x >= 1 && y >= 1 && y <= g_max - 2 && x <= g_max - 2 &&
                x + move.side <= g_max - 1 && y + move.side <= g_max - 1)
            {
                moves.push_back(move);
            }
        }
    }

    return moves;
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
