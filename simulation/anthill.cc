#include "sstream"

#include "squarecell.h"

#include "anthill.h"

using namespace std;

class Anthill : public Square
{
private:
    unsigned int xg;
    unsigned int yg;
    unsigned int total_food;
    unsigned int nbC;
    unsigned int nbD;
    unsigned int nbP;

public:
    Anthill(istringstream &stream);

    int get_number_of_collectors() { return nbC; }
    int get_number_of_defensors() { return nbD; }
    int get_number_of_predators() { return nbP; }
};

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