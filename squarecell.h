/**
 * @file squarecell.h
 * @author Daniel Panero [+34/-9], Layane Wazen [+0/-0], Andrea Diez [+65/-1]
 * @version 0.1
 * @date 2022-04-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef SQUARECELL_H
#define SQUARECELL_H

struct Square
{
    unsigned int x;
    unsigned int y;
    unsigned int side;
    bool centered;
};

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

#endif