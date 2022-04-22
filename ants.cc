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

#include "constantes.h"
#include "message.h"
#include "squarecell.h"

#include "ants.h"

using std::cout;
using std::istringstream;
using std::string;

Ant::Ant(unsigned int &x, unsigned int &y, unsigned int &side, unsigned int &age)
    : Square({x, y, side, false}), age(age)
{}

Generator::Generator(unsigned int &x, unsigned int &y, unsigned int &age)
    :Ant(x,y,side,age)
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
        cout << message::generator_overlap(x, y, superposed_x, superposed_y);
        exit(EXIT_FAILURE);
    }

    add_square(*this);
}

Collector::Collector(unsigned int &x, unsigned int &y, unsigned int &age,double &food)
    :Ant(x,y,side,age), food (food)
{
    test_square(*this);
    add_to_grid();
}

Collector unique_ptr<Collector>::parse_line(string &line)
{
    unsigned int x(0);
    unsigned int y(0);
    unsigned int age(0);
    double food(0);

    istringstream stream(line);
    stream >> x;
    stream >> y;
    stream >> age;
    stream >> food;

    return std::unique_ptr<Collector>(new Collector(x,y,age,food));
}

void Collector::add_to_grid()
{
    unsigned int superposed_x(0);
    unsigned int superposed_y(0);

    if (test_if_superposed_grid(*this, superposed_x, superposed_y))
    {
        cout << message::collector_overlap(x, y, superposed_x, superposed_y);
        exit(EXIT_FAILURE);
    }

    add_square(*this);
}

Defensor::Defensor(unsigned int &x, unsigned int &y, unsigned int &age)
    :Ant(x,y,side,age)
{
    test_square(*this);
    add_to_grid();
}

Defensor *Defensor::parse_line(string &line)
{
    unsigned int x(0);
    unsigned int y(0);
    unsigned int age(0);

    istringstream stream(line);
    stream >> x;
    stream >> y;
    stream >> age;

    return std::unique_ptr<Defensor>(new Defensor (x,y,age));
}

void Defensor::add_to_grid()
{
    unsigned int superposed_x(0);
    unsigned int superposed_y(0);

    if (test_if_superposed_grid(*this, superposed_x, superposed_y))
    {
        cout << message::defensor_overlap(x, y, superposed_x, superposed_y);
        exit(EXIT_FAILURE);
    }

    add_square(*this);
}

Predator::Predator(unsigned int &x, unsigned int &y, unsigned int &age)
    :Ant(x,y,side,age)
{
    test_square(*this);
    add_to_grid();
}

Predator *Predator::parse_line(string &line)
{
    unsigned int x(0);
    unsigned int y(0);
    unsigned int age (0);

    istringstream stream(line);
    stream >> x;
    stream >> y;
    stream >> age;

    return std::unique_ptr<Predator>(new Predator(x,y,age));
} 

void Predator::add_to_grid()
{
    if (test_if_superposed_grid(*this))
    {
        cout << message::predator_overlap(x, y);
        exit(EXIT_FAILURE);
    }

    add_square(*this);
}