#include "sstream"
#include "squarecell.h"

class Food : Square
{
public:
    Food(istringstream &stream);

    void addToGrid();
};