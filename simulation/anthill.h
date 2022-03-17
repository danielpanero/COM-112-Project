#include "sstream"

#include "../squarecell.h"
#include "ants.h"

class Anthill : Square
{
private:
    unsigned int total_food;
    unsigned int nbC;
    unsigned int nbD;
    unsigned int nbP;

    Generator* generator;

public:
    Anthill(std::istringstream &stream);

    int get_number_of_collectors();
    int get_number_of_defensors();
    int get_number_of_predators();

    Square get_as_square();
};
