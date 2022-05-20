/**
 * @file ants.cc
 * @author Daniel Panero [+135/-10], Layane Wazen [+10/-0], Andrea Diez [+367/-204]
 * @version 0.1
 * @date 2022-04-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <string>
#include <vector>

#include "element.h"
#include "squarecell.h"

#include "ants.h"

using std::string;
using std::vector;

using Squarecell::Square;

Ant::Ant(unsigned int x, unsigned int y, unsigned int side, unsigned int age,
         unsigned int color_index)
    : Element{x, y, side, true, color_index}, age(age)
{
}

string Ant::get_as_string()
{
    using std::to_string;
    return to_string(x) + " " + to_string(y) + " " + to_string(age);
}

bool Ant::increase_age()
{
    age++;
    if (age >= bug_life)
    {
        return false;
    }

    return true;
}

vector<Square> Ant::generate_moves(Square origin, vector<int> x_shift,
                                   vector<int> y_shift)
{
    vector<Square> moves;

    for (size_t i(0); i <= x_shift.size(); i++)
    {
        Square move(origin);

        move.x += x_shift.at(i);
        move.y += y_shift.at(i);

        // We check if the proposed new positions are inside the model
        if (Squarecell::test_square_without_message(move))
        {
            moves.push_back(move);
        }
    }

    return moves;
}