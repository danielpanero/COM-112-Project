/**
 * @file ants.cc
 * @author Daniel Panero [+135/-10], Layane Wazen [+10/-0], Andrea Diez [+79/-70]
 * @version 0.1
 * @date 2022-04-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "iostream"
#include "memory"
#include "sstream"
#include "stdexcept"

#include "constantes.h"
#include "element.h"
#include "message.h"
#include "squarecell.h"

#include "ants.h"

using std::istringstream;
using std::string;
using std::unique_ptr;

Ant::Ant(unsigned int &x, unsigned int &y, unsigned int side, unsigned int &age)
    : Element{x, y, side, true}, age(age)
{
}

string Ant::get_as_string()
{
    using std::to_string;
    return to_string(x) + " " + to_string(y) + " " + to_string(age);
}

Generator::Generator(unsigned int &x, unsigned int &y, unsigned int &age)
    : Ant{x, y, sizeG, age}
{
    test_square(*this);
    add_to_grid();
}

void Generator::add_to_grid()
{
    unsigned int superposed_x(0);
    unsigned int superposed_y(0);

    if (test_if_superposed_grid(*this, superposed_x, superposed_y))
    {
        throw std::invalid_argument(
            message::generator_overlap(x, y, superposed_x, superposed_y));
    }

    add_square(*this);
}

string Generator::get_as_string()
{
    return std::to_string(x) + " " + std::to_string(y);
}

void Generator::draw(unsigned int &color_index) { draw_filled(*this, color_index); }

Collector::Collector(unsigned int &x, unsigned int &y, unsigned int &age,
                     StateCollector &state)
    : Ant{x, y, sizeC, age}, state(state)
{
    test_square(*this);
    add_to_grid();
}

void Collector::add_to_grid()
{
    unsigned int superposed_x(0);
    unsigned int superposed_y(0);

    if (test_if_superposed_grid(*this, superposed_x, superposed_y))
    {
        throw std::invalid_argument(
            message::collector_overlap(x, y, superposed_x, superposed_y));
    }

    add_square(*this);
}

string Collector::get_as_string()
{
    return Ant::get_as_string() + " " + std::to_string(state);
}

void Collector::draw(unsigned int &color_index)
{
    draw_diagonal_pattern(*this, color_index);
}

unique_ptr<Collector> Collector::parse_line(string &line)
{
    unsigned int x(0);
    unsigned int y(0);
    unsigned int age(0);
    unsigned int tmp(0);

    istringstream stream(line);
    stream >> x;
    stream >> y;
    stream >> age;
    stream >> tmp;

    auto state = static_cast<StateCollector>(tmp);
    return unique_ptr<Collector>(new Collector(x, y, age, state));
}

Defensor::Defensor(unsigned int &x, unsigned int &y, unsigned int &age)
    : Ant{x, y, sizeD, age}
{
    test_square(*this);
    add_to_grid();
}

void Defensor::add_to_grid()
{
    unsigned int superposed_x(0);
    unsigned int superposed_y(0);

    if (test_if_superposed_grid(*this, superposed_x, superposed_y))
    {
        throw std::invalid_argument(
            message::defensor_overlap(x, y, superposed_x, superposed_y));
    }

    add_square(*this);
}

void Defensor::draw(unsigned int &color_index)
{
    draw_plus_pattern(*this, color_index);
}

unique_ptr<Defensor> Defensor::parse_line(string &line)
{
    unsigned int x(0);
    unsigned int y(0);
    unsigned int age(0);

    istringstream stream(line);
    stream >> x;
    stream >> y;
    stream >> age;

    return unique_ptr<Defensor>(new Defensor(x, y, age));
}

Predator::Predator(unsigned int &x, unsigned int &y, unsigned int &age)
    : Ant{x, y, sizeP, age}
{
    test_square(*this);
    add_to_grid();
}

void Predator::add_to_grid()
{
    if (test_if_superposed_grid(*this))
    {
        throw std::invalid_argument(message::predator_overlap(x, y));
    }

    add_square(*this);
}

void Predator::draw(unsigned int &color_index) { draw_filled(*this, color_index); }

unique_ptr<Predator> Predator::parse_line(string &line)
{
    unsigned int x(0);
    unsigned int y(0);
    unsigned int age(0);

    istringstream stream(line);
    stream >> x;
    stream >> y;
    stream >> age;

    return unique_ptr<Predator>(new Predator(x, y, age));
}
