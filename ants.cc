/**
 * @file ants.cc
 * @author Daniel Panero [+135/-10], Layane Wazen [+10/-0], Andrea Diez [+367/-204]
 * @version 0.1
 * @date 2022-04-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "element.h"

#include "ants.h"

using std::string;

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