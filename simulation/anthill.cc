#include "sstream"

#include "../squarecell.h"

#include "anthill.h"

using namespace std;

Anthill::Anthill(istringstream &stream)
{
    stream >> x;
    stream >> y;
    stream >> side;
    centered = false;
    test_square(*this);

    unsigned int xg;
    unsigned int yg;
    stream >> xg;
    stream >> yg;
    generator = new Generator(xg, yg);

    stream >> total_food;
    stream >> nbC;
    stream >> nbD;
    stream >> nbP;
}

int Anthill::get_number_of_collectors()
{
    return nbC;
};

int Anthill::get_number_of_defensors()
{
    return nbD;
};

int Anthill::get_number_of_predators()
{
    return nbP;
};

Square Anthill::get_as_square()
{
    return Square(*this);
}