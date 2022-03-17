#include "sstream"
#include "iostream"

#include "../squarecell.h"
#include "../shared/message.h"
#include "../shared/constantes.h"

#include "ants.h"

using namespace std;

Generator::Generator(unsigned int x, unsigned int y)
{
    this->x = x;
    this->y = y;

    side = sizeG;
    centered = true;

    test_square(*this);
    add_to_grid();
}

void Generator::add_to_grid()
{
    if (test_if_superposed_grid(*this))
    {
        cout << message::generator_overlap(x, y, x, y);
        exit(EXIT_FAILURE);
    }
    else
    {
        add_square(*this);
    }
}

Square Generator::get_as_square() { return Square(*this); }

Collector::Collector(istringstream &stream)
{
    stream >> x;
    stream >> y;

    side = sizeC;
    centered = true;

    test_square(*this);
    add_to_grid();
}

void Collector::add_to_grid()
{
    if (test_if_superposed_grid(*this))
    {
        cout << message::collector_overlap(x, y, x, y);
        exit(EXIT_FAILURE);
    }
    else
    {
        add_square(*this);
    }
}

Defensor::Defensor(istringstream &stream)
{
    stream >> x;
    stream >> y;

    side = sizeD;
    centered = true;

    test_square(*this);
    add_to_grid();
}

void Defensor::add_to_grid()
{
    if (test_if_superposed_grid(*this))
    {
        cout << message::defensor_overlap(x, y, x, y);
        exit(EXIT_FAILURE);
    }
    else
    {
        add_square(*this);
    }
}

Square Defensor::get_as_square() { return Square(*this); }

Predator::Predator(istringstream &stream)
{
    stream >> x;
    stream >> y;

    side = sizeP;
    centered = true;

    test_square(*this);
    add_to_grid();
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