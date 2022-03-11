#include "vector"
#include "iostream"
#include "algorithm"
#include "math.h"

#include "squarecell/error_squarecell.h"
#include "squarecell.h"

using namespace std;

constexpr short unsigned g_dim(7);
constexpr short unsigned g_max(pow(2, g_dim));
static vector<vector<bool>> grid(g_max, vector<bool>(g_max));

struct Square
{
    unsigned int x;
    unsigned int y;
    unsigned int side;
    bool centered;
};

unsigned int get_coordinate_x(Square &square)
{
    if (square.centered)
    {
        return square.x - (square.side - 1) / 2;
    }
}

unsigned int get_coordinate_y(Square &square)
{
    if (square.centered)
    {
        return square.y - (square.side - 1) / 2;
    }
}

bool test_square(Square &square)
{
    unsigned int x = get_coordinate_x(square);
    unsigned int y = get_coordinate_y(square);

    if ((x < 0 || x > g_max - 1))
    {
        cout << error_squarecell::print_index(x, g_max);
        exit(EXIT_FAILURE);
    }

    if ((y < 0 || y > g_max - 1))
    {
        cout << error_squarecell::print_index(y, g_max);
        exit(EXIT_FAILURE);
    }

    if ((x + square.side > g_max - 1))
    {
        cout << error_squarecell::print_outside(y, square.side, g_max);
        exit(EXIT_FAILURE);
    }

    if ((y + square.side > g_max - 1))
    {
        cout << error_squarecell::print_outside(y, square.side, g_max);
        exit(EXIT_FAILURE);
    }
}

void add_square(Square &square)
{
    unsigned int x = get_coordinate_x(square);
    unsigned int y = get_coordinate_y(square);

    for (auto it = grid.begin() + g_max - 1 - y; it != grid.end() + g_max - 1; ++it)
    {
        auto &row = *it;
        fill(row.begin() + x, row.begin() + x + square.side, true);
    }
}

void remove_square(Square &square)
{
    unsigned int x = get_coordinate_x(square);
    unsigned int y = get_coordinate_y(square);

    for (auto it = grid.begin() + g_max - 1 - y; it != grid.end() + g_max - 1; ++it)
    {
        auto &row = *it;
        fill(row.begin() + x, row.begin() + x + square.side, false);
    }
}

bool test_if_superposed_grid(Square &square)
{
    unsigned int x = get_coordinate_x(square);
    unsigned int y = get_coordinate_y(square);

    for (auto it = grid.begin() + g_max - 1 - y; it != grid.end() + g_max - 1; ++it)
    {
        auto &row = *it;
        vector<bool>::iterator cell = find(row.begin() + x, row.begin() + x + square.side, true);

        if (cell != row.begin() + x + square.side)
        {
            return true;
        }
    }
    return false;
}

bool test_if_superposed_two_square(Square &square1, Square &square2)
{
    unsigned int x1 = get_coordinate_x(square1);
    unsigned int y1 = get_coordinate_y(square1);

    unsigned int x2 = get_coordinate_x(square2);
    unsigned int y2 = get_coordinate_y(square2);

    if (x1 == x2 || y1 == y2)
    {
        return true;
    }

    if (x1 < x2)
    {
        if (x1 + square1.side >= x2)
        {
            return true;
        }
    }
    else
    {
        if (x2 + square2.side >= x1)
        {
            return true;
        }
    }

    if (y1 < y2)
    {
        if (y1 + square1.side >= y2)
        {
            return true;
        }
    }
    else
    {
        if (y2 + square2.side >= y1)
        {
            return true;
        }
    }

    return false;
}