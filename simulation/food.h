#include "sstream"
#include "../squarecell.h"

class Food : Square
{
public:
    Food(std::string &line);

    void add_to_grid();
};