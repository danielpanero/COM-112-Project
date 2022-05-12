/**
 * @file ants.cc
 * @author Daniel Panero [+135/-10], Layane Wazen [+10/-0], Andrea Diez [+367/-204]
 * @version 0.1
 * @date 2022-04-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>

#include "constantes.h"
#include "element.h"
#include "message.h"
#include "squarecell.h"

#include "ants.h"

using std::istringstream;
using std::string;
using std::unique_ptr;

// ====================================================================================
// Ant
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

// ====================================================================================
// Generator
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

string Generator::get_as_string()
{
    return std::to_string(x) + " " + std::to_string(y);
}

void Generator::draw() { Squarecell::draw_filled(*this, get_color_index()); }

// ====================================================================================
// Collector

Collector::Collector(unsigned int x, unsigned int y, unsigned int age,
                     State_collector state, unsigned int color_index)
    : Ant{x, y, sizeC, age, color_index}, state(state)
{
    Squarecell::test_square(*this);
    add_to_grid();
}

void Collector::add_to_grid()
{
    unsigned int superposed_x(0);
    unsigned int superposed_y(0);

    if (Squarecell::test_if_superposed_grid(*this, superposed_x, superposed_y))
    {
        throw std::invalid_argument(
            message::collector_overlap(x, y, superposed_x, superposed_y));
    }

    Squarecell::add_square(*this);
}

void Collector::draw() { Squarecell::draw_diagonal_pattern(*this, get_color_index()); }

string Collector::get_as_string()
{
    return Ant::get_as_string() + " " +
           (state == State_collector::LOADED ? "true" : "false");
}

void Collector::remove_from_grid() { Squarecell::remove_square(*this); }
void Collector::undraw() { Squarecell::undraw_square(*this); }

std::vector<Squarecell::Square> Collector::generate_diagonal_moves(Square origin)
{
    std::vector<Squarecell::Square> moves;

    for (int i(1); i <= 2; i++)
    {
        for (int j(1); j <= 2; j++)
        {
            Squarecell::Square move(origin);

            move.x += i == 1 ? 1 : -1;
            move.y += j == 1 ? 1 : -1;

            unsigned int x = Squarecell::get_coordinate_x(move);
            unsigned int y = Squarecell::get_coordinate_y(move);

            // We check if the proposed new positions are inside the model
            if (x >= 0 && y >= 0 && y <= 127 && x <= 127 && x + move.side <= 127 &&
                y + move.side <= 127)
            {
                moves.push_back(move);
            }
        }
    }

    return moves;
}

unique_ptr<Collector> Collector::parse_line(string &line, unsigned int color_index)
{
    unsigned int x(0);
    unsigned int y(0);
    unsigned int age(0);
    string tmp("false");

    istringstream stream(line);
    stream >> x;
    stream >> y;
    stream >> age;
    stream >> tmp;

    auto state(State_collector::EMPTY);
    if (tmp == "true")
    {
        state = State_collector::LOADED;
    }
    return unique_ptr<Collector>(new Collector(x, y, age, state, color_index));
}

// ====================================================================================
// Defensor

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

void Defensor::draw() { Squarecell::draw_plus_pattern(*this, get_color_index()); }

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

// ====================================================================================
// Predator

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

void Predator::draw() { Squarecell::draw_filled(*this, get_color_index()); }

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
