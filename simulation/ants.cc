#include "sstream"

#include "squarecell.h"
#include "message.h"

#include "ants.h"

using namespace std;

class Collector : Square
{
public:
    Collector(istringstream &stream);

    void add_to_grid();
};

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

class Defensor : Square
{
public:
    Defensor(istringstream &stream);

    void add_to_grid();
};

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

class Predator : Square
{
public:
    Predator(istringstream &stream);

    void add_to_grid();
};

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