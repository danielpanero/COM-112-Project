#include "iostream"
#include "sstream"

#include "constantes.h"
#include "message.h"
#include "squarecell.h"

#include "ants.h"

using std::cout;
using std::istringstream;
using std::string;

Generator::Generator(unsigned int &x, unsigned int &y) : Square({x, y, sizeG, true})
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
    else
    {
        add_square(*this);
    }
}

Square Generator::get_as_square() { return {*this}; }

Collector::Collector(unsigned int &x, unsigned int &y) : Square({x, y, sizeC, true})
{
    test_square(*this);
    add_to_grid();
}

Collector *Collector::parse_line(string &line)
{
    unsigned int x(0);
    unsigned int y(0);

    istringstream stream(line);
    stream >> x;
    stream >> y;

    return new Collector(x, y);
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
    else
    {
        add_square(*this);
    }
}

Defensor::Defensor(unsigned int &x, unsigned int &y) : Square({x, y, sizeD, true})
{
    test_square(*this);
    add_to_grid();
}

Defensor *Defensor::parse_line(string &line)
{
    unsigned int x(0);
    unsigned int y(0);

    istringstream stream(line);
    stream >> x;
    stream >> y;

    return new Defensor(x, y);
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
    else
    {
        add_square(*this);
    }
}

Square Defensor::get_as_square() { return {*this}; }

Predator::Predator(unsigned int &x, unsigned int &y) : Square({x, y, sizeP, true})
{
    test_square(*this);
    add_to_grid();
}

Predator *Predator::parse_line(string &line)
{
    unsigned int x(0);
    unsigned int y(0);

    istringstream stream(line);
    stream >> x;
    stream >> y;

    return new Predator(x, y);
}

void Predator::add_to_grid()
{
    if (test_if_superposed_grid(*this))
    {
        cout << message::predator_overlap(x, y);
        exit(EXIT_FAILURE);
    }
    else
    {
        add_square(*this);
    }
}