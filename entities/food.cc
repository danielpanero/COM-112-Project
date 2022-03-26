#include "iostream"
#include "sstream"

#include "message.h"
#include "squarecell.h"

#include "food.h"

using std::string;
using std::istringstream;
using std::cout;

Food::Food(string &line)
{
    istringstream stream(line);
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