#include "sstream"

#include "../squarecell.h"

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
    Anthill(std::istringstream &stream);

    int get_number_of_collectors();
    int get_number_of_defensors();
    int get_number_of_predators();
};
