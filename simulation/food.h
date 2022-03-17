#include "sstream"
#include "../squarecell.h"

class Food : Square
{
public:
    Food(std::istringstream &stream);

    void add_to_grid();
};