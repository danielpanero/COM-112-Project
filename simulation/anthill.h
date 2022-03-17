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

    Generator *generator;
    std::vector<Collector *> collectors;
    std::vector<Defensor *> defendors;
    std::vector<Predator *> predators;

public:
    Anthill(std::istringstream &stream);

    void set_collectors(std::vector<Collector *> &collectors);
    void set_defensors(std::vector<Defensor *> &defensor);
    void set_predators(std::vector<Predator *> &predators);

    int get_number_of_collectors();
    int get_number_of_defensors();
    int get_number_of_predators();

    Square get_as_square();
};
