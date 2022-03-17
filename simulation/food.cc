#include "sstream"

#include "../squarecell.h"
#include "../shared/message.h"

#include "food.h"

using namespace std;

Food::Food(istringstream &stream)
{
    stream >> x;
    stream >> y;

    side = 1;
    centered = true;

    test_square(*this);
    add_to_grid();
}

void Food::add_to_grid()
{
    if (test_if_superposed_grid(*this))
    {
        cout << message::food_overlap(x, y);
        exit(EXIT_FAILURE);
    }
    else
    {
        add_square(*this);
    }
}