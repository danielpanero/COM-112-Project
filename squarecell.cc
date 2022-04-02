#include "algorithm"
#include "cmath"
#include "iostream"
#include "vector"

#include "error_squarecell.h"

#include "squarecell.h"

using std::cout;
using std::vector;

constexpr short unsigned g_dim(7);             // NOLINT
constexpr short unsigned g_max(pow(2, g_dim)); // NOLINT

static vector<vector<bool>> grid(g_max, vector<bool>(g_max)); // NOLINT

unsigned int get_coordinate_x(Square &square)
{
    if (square.centered)
    {
        return square.x - (square.side - 1) / 2;
    }
    return square.x;
}

unsigned int get_coordinate_y(Square &square)
{
    if (square.centered)
    {
        return square.y - (square.side - 1) / 2;
    }

    return square.y;
}

void test_square(Square &square)
{
    unsigned int x = get_coordinate_x(square);
    unsigned int y = get_coordinate_y(square);

    if ((x < 0 || x > g_max - 1))
    {
        cout << error_squarecell::print_index(square.x, g_max - 1);
        exit(EXIT_FAILURE);
    }

    if ((y < 0 || y > g_max - 1))
    {
        cout << error_squarecell::print_index(square.y, g_max - 1);
        exit(EXIT_FAILURE);
    }

    if ((x + square.side > g_max - 1))
    {
        cout << error_squarecell::print_outside(square.x, square.side, g_max - 1);
        exit(EXIT_FAILURE);
    }

    if ((y + square.side > g_max - 1))
    {
        cout << error_squarecell::print_outside(square.y, square.side, g_max - 1);
        exit(EXIT_FAILURE);
    }
}

void add_square(Square &square)
{
    unsigned int x = get_coordinate_x(square);
    unsigned int y = get_coordinate_y(square);

    size_t start(g_max - 1 - y);
    size_t end(g_max - 1 - (y + square.side));

    for (size_t i(start); i > end; i--)
    {
        auto &row = grid[i];
        fill(row.begin() + x, row.begin() + x + square.side, true);
    }
}

void remove_square(Square &square)
{
    unsigned int x = get_coordinate_x(square);
    unsigned int y = get_coordinate_y(square);

    size_t start(g_max - 1 - y);
    size_t end(g_max - 1 - (y + square.side));

    for (size_t i(start); i > end; i--)
    {
        auto &row = grid[i];
        fill(row.begin() + x, row.begin() + x + square.side, false);
    }
}

bool test_if_superposed_grid(Square &square, unsigned int &superposed_x,
                             unsigned int &superposed_y)
{
    unsigned int x = get_coordinate_x(square);
    unsigned int y = get_coordinate_y(square);

    size_t start(g_max - 1 - y);
    size_t end(g_max - 1 - (y + square.side));

    for (size_t i(start); i > end; i--)
    {
        auto &row = grid[i];
        auto cell = find(row.begin() + x, row.begin() + x + square.side, true);

        /* We are comparing the iterator (cell) with the last element of the loop,
        since C++ returns the last element when it doesn't find any matching element */
        if (cell != row.begin() + x + square.side)
        {
            superposed_x = cell - row.begin();
            superposed_y = g_max - 1 - i;
            return true;
        }
    }
    return false;
}

bool test_if_superposed_grid(Square &square)
{
    unsigned int superposed_x(0);
    unsigned int superposed_y(0);

    return test_if_superposed_grid(square, superposed_x, superposed_y);
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

    if (x1 < x2 && x1 + square1.side >= x2)
    {
        return true;
    }

    if (x1 > x2 && x2 + square2.side >= x1)
    {
        return true;
    }

    if (y1 < y2 && y1 + square1.side >= y2)
    {
        return true;
    }

    if (y1 > y2 && y2 + square2.side >= y1)
    {
        return true;
    }

    return false;
}

bool test_if_completely_confined(Square &square1, Square &square2)
{
    unsigned int x1 = get_coordinate_x(square1);
    unsigned int y1 = get_coordinate_y(square1);

    unsigned int x2 = get_coordinate_x(square2);
    unsigned int y2 = get_coordinate_y(square2);

    if (x1 <= x2 || y1 <= y2)
    {
        return false;
    }

    if (x1 + square1.side >= x2 + square2.side)
    {
        return false;
    }

    if (y1 + square1.side >= y2 + square2.side)
    {
        return false;
    }

    return true;
}