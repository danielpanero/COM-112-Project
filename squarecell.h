#ifndef SQUARE_H
#define SQUARE_H

struct Square
{
    unsigned int x;
    unsigned int y;
    unsigned int side;
    bool centered;
};

unsigned int get_coordinate_x(Square &square);
unsigned int get_coordinate_y(Square &square);

void test_square(Square &square);

void add_square(Square &square);
void remove_square(Square &square);

bool test_if_superposed_grid(Square &square);
bool test_if_superposed_grid(Square &square, unsigned int &superposed_x, unsigned int &superposed_y);
bool test_if_superposed_two_square(Square &square1, Square &square2);
bool test_if_completely_confined(Square &square1, Square &square2);

#endif