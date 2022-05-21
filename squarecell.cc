/**
 * @file squarecell.cc
 * @author Daniel Panero [+373/-132], Layane Wazen [+0/-0], Andrea Diez [+80/-80]
 * @version 0.1
 * @date 2022-04-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <queue>
#include <stdexcept>
#include <vector>

#include "error_squarecell.h"
#include "graphic.h"

#include "squarecell.h"

using std::invalid_argument;
using std::vector;

constexpr double g_max(128);

static vector<vector<bool>> grid(g_max, vector<bool>(g_max));

// ====================================================================================
// Grid / Utils

void Squarecell::grid_clear()
{
    grid = vector<vector<bool>>(g_max, vector<bool>(g_max));

    Graphic::clear_surface();
}

unsigned int Squarecell::get_coordinate_x(Square const &square)
{
    if (square.centered)
    {
        return square.x - (square.side - 1) / 2;
    }
    return square.x;
}

unsigned int Squarecell::get_coordinate_y(Square const &square)
{
    if (square.centered)
    {
        return square.y - (square.side - 1) / 2;
    }

    return square.y;
}

void Squarecell::test_square(Square const &square)
{
    unsigned int x = get_coordinate_x(square);
    unsigned int y = get_coordinate_y(square);

    if ((x < 0 || x > g_max - 1))
    {
        throw invalid_argument(error_squarecell::print_index(square.x, g_max - 1));
    }

    if ((y < 0 || y > g_max - 1))
    {
        throw invalid_argument(error_squarecell::print_index(square.y, g_max - 1));
    }

    if ((x + square.side > g_max))
    {
        throw invalid_argument(
            error_squarecell::print_outside(square.x, square.side, g_max - 1));
    }

    if ((y + square.side > g_max))
    {
        throw invalid_argument(
            error_squarecell::print_outside(square.y, square.side, g_max - 1));
    }
}

bool Squarecell::test_square_without_message(Square const &square)
{
    unsigned int x = get_coordinate_x(square);
    unsigned int y = get_coordinate_y(square);

    if ((x < 0 || x > g_max - 1))
    {
        return false;
    }

    if ((y < 0 || y > g_max - 1))
    {
        return false;
    }

    if ((x + square.side > g_max))
    {
        return false;
    }

    if ((y + square.side > g_max))
    {
        return false;
    }
    if ((square.x < 0 || square.x > g_max - 1))
    {
        return false;
    }

    if ((square.y < 0 || square.y > g_max - 1))
    {
        return false;
    }
    return true;
}

void Squarecell::add_square(Square const &square)
{
    unsigned int x = get_coordinate_x(square);
    unsigned int y = get_coordinate_y(square);

    size_t start(g_max - (y + square.side));
    size_t end(g_max - 1 - y);

    for (size_t i(start); i <= end; i++)
    {
        auto &row = grid.at(i);
        fill(row.begin() + x, row.begin() + x + square.side, true);
    }
}

void Squarecell::remove_square(Square const &square)
{
    unsigned int x = get_coordinate_x(square);
    unsigned int y = get_coordinate_y(square);

    size_t start(g_max - (y + square.side));
    size_t end(g_max - 1 - y);

    for (size_t i(start); i <= end; i++)
    {
        auto &row = grid.at(i);
        fill(row.begin() + x, row.begin() + x + square.side, false);
    }
}

// ====================================================================================
// Tests

bool Squarecell::test_if_superposed_grid(Square const &square,
                                         unsigned int &superposed_x,
                                         unsigned int &superposed_y)
{
    unsigned int x = get_coordinate_x(square);
    unsigned int y = get_coordinate_y(square);

    size_t start(g_max - (y + square.side));
    size_t end(g_max - 1 - y);

    for (size_t i(start); i <= end; i++)
    {
        auto &row = grid.at(i);
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

bool Squarecell::test_if_superposed_grid(Square const &square)
{
    unsigned int superposed_x(0);
    unsigned int superposed_y(0);

    return test_if_superposed_grid(square, superposed_x, superposed_y);
}

bool Squarecell::test_if_superposed_two_square(Square const &square1,
                                               Square const &square2)
{
    unsigned int x1 = get_coordinate_x(square1);
    unsigned int y1 = get_coordinate_y(square1);

    unsigned int x2 = get_coordinate_x(square2);
    unsigned int y2 = get_coordinate_y(square2);

    if (x1 >= x2 + square2.side || x2 >= x1 + square1.side)
    {
        return false;
    }

    if (y1 >= y2 + square2.side || y2 >= y1 + square1.side)
    {
        return false;
    }

    return true;
}

bool Squarecell::test_if_completely_confined(Square const &square1,
                                             Square const &square2)
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

bool Squarecell::test_if_border_touches(Square const &square1, Square const &square2)
{
    unsigned int x1 = get_coordinate_x(square1);
    unsigned int y1 = get_coordinate_y(square1);

    unsigned int x2 = get_coordinate_x(square2);
    unsigned int y2 = get_coordinate_y(square2);

    if (test_if_completely_confined(square1, square2))
    {
        if (x1 == x2 + 1 || x1 + square1.side == x2 + square2.side - 1)
        {
            return true;
        }

        if (y1 == y2 + 1 || y1 + square1.side == y2 + square2.side - 1)
        {
            return true;
        }
    }
    else if (test_if_completely_confined(square2, square1))
    {
        if (x1 + 1 == x2 || x1 + square1.side - 1 == x2 + square2.side)
        {
            return true;
        }

        if (y1 + 1 == y2 || y1 + square1.side - 1 == y2 + square2.side)
        {
            return true;
        }
    }
    else
    {
        if (x1 > x2 + square2.side || x2 > x1 + square1.side)
        {
            return false;
        }

        if (y1 > y2 + square2.side || y2 > y1 + square1.side)
        {
            return false;
        }
    }

    return false;
}

// ====================================================================================
// Search algorithms

struct BFSNode
{
    // The coordinate (x,y) of the parent, ie the first move
    unsigned int x_i : 7;
    unsigned int y_i : 7;

    // The current coordinate (x,y) of the node
    unsigned int x : 7;
    unsigned int y : 7;
};

Squarecell::Square Squarecell::lee_algorithm(
    Square const &origin, Square const &target,
    const std::function<vector<Square>(const Square)> &generate_moves,
    const std::function<bool(Square const &, Square const &)> &test)
{
    vector<vector<bool>> bfs_visited_nodes(g_max, vector<bool>(g_max));
    std::queue<BFSNode> bfs_queue;

    bfs_queue.push({.x_i = origin.x, .y_i = origin.y, .x = origin.x, .y = origin.y});

    /** The first time that we run the lee_algorithm algorithm, we store all the
     * valid moves / directions, so for next iterations depending on the path
     * taken, we will always have the first moves / direction needed for taking the
     * path
     */
    bool first_iteration = true;

    while (!bfs_queue.empty())
    {
        auto bfs_current_node = bfs_queue.front();
        bfs_queue.pop();

        Squarecell::Square current_square(origin);
        current_square.x = bfs_current_node.x;
        current_square.y = bfs_current_node.y;

        // We test if we have reached the target
        if (test(current_square, target))
        {
            return {.x = bfs_current_node.x_i,
                    .y = bfs_current_node.y_i,
                    .side = origin.side,
                    .centered = origin.centered};
        }

        auto proposed_moves = generate_moves(current_square);
        for (auto &move : proposed_moves)
        {
            // If the proposed position is valid and we have not already visited
            // it, we push into the queue
            if (!Squarecell::test_if_superposed_grid(move))
            {

                unsigned int x = get_coordinate_x(move);
                unsigned int y = get_coordinate_y(move);

                bool visited = bfs_visited_nodes.at(g_max - 1 - y).at(x);
                if (!visited)
                {
                    if (first_iteration)
                    {
                        bfs_queue.push(
                            {.x_i = move.x, .y_i = move.y, .x = move.x, .y = move.y});
                    }
                    else
                    {
                        bfs_queue.push({.x_i = bfs_current_node.x_i,
                                        .y_i = bfs_current_node.y_i,
                                        .x = move.x,
                                        .y = move.y});
                    }

                    bfs_visited_nodes[g_max - 1 - y][x] = true;
                }
            }
        }

        if (first_iteration)
        {
            first_iteration = false;
        }
    }

    return origin;
}

// ====================================================================================
// Draw

void Squarecell::draw_as_diamond(Square const &square, std::string color)
{
    unsigned int x = get_coordinate_x(square);
    unsigned int y = get_coordinate_y(square);

    Graphic::draw_filled_diamond(x, y, square.side, color);
}

void Squarecell::draw_only_border(Square const &square, unsigned int color_index)
{
    unsigned int x = get_coordinate_x(square);
    unsigned int y = get_coordinate_y(square);

    Graphic::draw_thick_border_square(x, y, square.side, color_index);
}

void Squarecell::draw_filled(Square const &square, unsigned int color_index)
{
    unsigned int x = get_coordinate_x(square);
    unsigned int y = get_coordinate_y(square);

    Graphic::draw_filled_square(x, y, square.side, color_index);
}

void Squarecell::draw_diagonal_pattern(Square const &square, unsigned int color_index)
{
    unsigned int x = get_coordinate_x(square);
    unsigned int y = get_coordinate_y(square);

    Graphic::draw_diagonal_pattern_square(x, y, square.side, color_index);
}

void Squarecell::draw_plus_pattern(Square const &square, unsigned int color_index)
{
    unsigned int x = get_coordinate_x(square);
    unsigned int y = get_coordinate_y(square);

    Graphic::draw_plus_pattern_square(x, y, square.side, color_index);
}

// ====================================================================================
// Undraw

void Squarecell::undraw_square(Square const &square)
{
    unsigned int x = get_coordinate_x(square);
    unsigned int y = get_coordinate_y(square);

    Graphic::undraw_square(x, y, square.side);
}

void Squarecell::undraw_thick_border_square(Square const &square)
{
    unsigned int x = get_coordinate_x(square);
    unsigned int y = get_coordinate_y(square);

    Graphic::undraw_thick_border_square(x, y, square.side);
}