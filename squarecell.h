#include "vector"
#include "iostream"
#include "algorithm"
#include "math.h"

struct Square
{
    unsigned int x;
    unsigned int y;
    unsigned int side;
    bool centered;
};

bool test_square(Square &square);

void add_square(Square &square);
void remove_square(Square &square);
bool test_if_superposed_grid(Square &square);
bool test_if_superposed_two_square(Square &square1, Square &square2);