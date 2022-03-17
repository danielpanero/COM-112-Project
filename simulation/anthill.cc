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

    stream >> xg;
    stream >> yg;
    stream >> total_food;
    stream >> nbC;
    stream >> nbD;
    stream >> nbP;
}

int Anthill::get_number_of_collectors(){
    return nbC;
};

int Anthill::get_number_of_defensors(){
    return nbD;
};

int Anthill::get_number_of_predators(){
    return nbP;
};