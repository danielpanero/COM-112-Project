/**
 * @file squarecell.h
 * @author Daniel Panero [+205/-132], Layane Wazen [+0/-0], Andrea Diez [+140/-12]
 * @version 0.1
 * @date 2022-04-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef SQUARECELL_H
#define SQUARECELL_H

#include <functional>
#include <vector>

namespace Squarecell
{
    struct Square
    {
        unsigned int x;
        unsigned int y;
        unsigned int side;
        bool centered;
    };

    /**
     * @brief Resets the grid and clears the surface of Graphic
     *
     */
    void grid_clear();

    /**
     * @brief Calculates the bottom left x-coordinate.
     *
     * @param square
     * @return unsigned int
     */
    unsigned int get_coordinate_x(Square &square);

    /**
     * @brief Calculates the bottom left y-coordinate.
     *
     * @param square
     * @return unsigned int
     */
    unsigned int get_coordinate_y(Square &square);

    /**
     * @brief Checks if the square respects the constraints.
     *
     * @param square
     */
    void test_square(Square &square);

    void add_square(Square &square);
    void remove_square(Square &square);

    /**
     * @brief Checks if the grid is already occupied
     *
     * @param[in] square
     * @param[out] superposed_x the x position where it is superposing
     * @param[out] superposed_y the y position where it is superposing
     * @return true if grid is already occupied
     * @return false if the grid is free
     */
    bool test_if_superposed_grid(Square &square);

    /**
     * @brief Checks if the grid is already occupied
     *
     * @param square
     * @return true if grid is already occupied
     * @return false if the grid is free
     */
    bool test_if_superposed_grid(Square &square, unsigned int &superposed_x,
                                 unsigned int &superposed_y);

    /**
     * @brief Checks if squares are superposing
     *
     * @param[out] square1
     * @param[out] square2
     * @return true if squares are superposing
     * @return false if squares aren't superposing
     */
    bool test_if_superposed_two_square(Square &square1, Square &square2);

    /**
     * @brief Checks if the square1 is completly contained within square2
     *
     * @param[out] square1 (to be contained)
     * @param[out] square2 (container)
     * @return true if square1 is completly contained within square2
     * @return false if square1 is not completly contained within square2
     */
    bool test_if_completely_confined(Square &square1, Square &square2);

    /**
     * @brief Implementation of a lee algorithm, used for solving maze routing problems
     * based on breadth-first search. It always gives an optimal solution, if one
     * exists, but is slow and requires considerable memory. The algorithm search all
     * possible routes generates via the function \b generate_moves and when it has
     * reached its goal ( \b test true), it returns the first move / position for
     * taking the optmal path
     *
     * @param origin
     * @param target
     * @param generate_moves a function that returns all the possible moves based on
     * the current position
     * @param test a function which that if the algorithm has reached its goal
     * @return Squarecell::Square
     */
    Squarecell::Square
    lee_algorithm(Square &origin, Square &target,
                  const std::function<std::vector<Square>(Square)> &generate_moves,
                  const std::function<bool(Square, Square)> &test);

    /**
     * @brief Draws @p square as diamond using the module Graphic
     *
     * @param square
     * @param color
     */
    void draw_as_diamond(Square &square, std::string color);

    /**
     * @brief Draws @p square as a thick border of color @p color_index using the
     * module Graphic
     *
     * @param square
     * @param color_index (0 red, 1 green, 2 blue, 3 yellow, 4 magenta, 5 cyan)
     */
    void draw_only_border(Square &square, unsigned int color_index);

    /**
     * @brief Fills @p square with the color @p color_index using the
     * module Graphic
     *
     * @param square
     * @param color_index (0 red, 1 green, 2 blue, 3 yellow, 4 magenta, 5 cyan)
     */
    void draw_filled(Square &square, unsigned int color_index);

    /**
     * @brief Draws @p square filled with |X O||X O|... pattern where X has color @p
     * color_index and O a lighter version of @p color_index using the
     * module Graphic
     *
     * @param square
     * @param color_index (0 red, 1 green, 2 blue, 3 yellow, 4 magenta, 5 cyan)
     */
    void draw_diagonal_pattern(Square &square, unsigned int color_index);

    /**
     * @brief Draws @p square filled with a lighter version of @p color_index and a
     * plus sign in the center with color @p color_index using the
     * module Graphic
     *
     * @param square
     * @param color_index (0 red, 1 green, 2 blue, 3 yellow, 4 magenta, 5 cyan)
     */
    void draw_plus_pattern(Square &square, unsigned int color_index);
} // namespace Squarecell

#endif