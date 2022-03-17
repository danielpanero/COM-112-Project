#include "sstream"

#include "../squarecell.h"
#include "../shared/message.h"

#include "ants.h"

using namespace std;

Collector::Collector(istringstream &stream)
{
    stream >> x;
    stream >> y;

    side = 1;
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

    side = 1;
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

Predator::Predator(istringstream &stream)
{
    stream >> x;
    stream >> y;

    side = 1;
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