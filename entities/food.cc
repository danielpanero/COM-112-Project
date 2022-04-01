#include "iostream"
#include "sstream"

#include "message.h"
#include "squarecell.h"

#include "food.h"

using std::cout;
using std::istringstream;
using std::string;

Food::Food(unsigned int &x, unsigned int &y) : Square({x, y, 1, true})
{
    test_square(*this);
    add_to_grid();
}

Food *Food::parse_line(string &line)
{
    unsigned int x(0);
    unsigned int y(0);

    istringstream stream(line);
    stream >> x;
    stream >> y;

    return new Food(x, y);
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