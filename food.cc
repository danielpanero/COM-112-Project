/**
 * @file food.cc
 * @author Daniel Panero [+46/-6], Layane Wazen [+10/-0], Andrea Diez [+30/-25]
 * @version 0.1
 * @date 2022-04-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "iostream"
#include "sstream"
#include "stdexcept"

#include "element.h"
#include "message.h"
#include "squarecell.h"

#include "food.h"

using std::istringstream;
using std::string;
using std::unique_ptr;

Food::Food(unsigned int x, unsigned int y) : Element{x, y, 1, true}
{
    Squarecell::test_square(*this);
    add_to_grid();
}

void Food::add_to_grid()
{
    if (Squarecell::test_if_superposed_grid(*this))
    {
        throw std::invalid_argument(message::food_overlap(x, y));
    }

    Squarecell::add_square(*this);
}

string Food::get_as_string() { return std::to_string(x) + " " + std::to_string(y); }

void Food::draw(unsigned int &color_index) { Squarecell::draw_as_diamond(*this); }

unique_ptr<Food> Food::parse_line(string &line)
{
    unsigned int x(0);
    unsigned int y(0);

    istringstream stream(line);
    stream >> x;
    stream >> y;

    return unique_ptr<Food>(new Food(x, y));
}
