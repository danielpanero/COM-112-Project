/**
 * @file graphic.h
 * @author Daniel Panero, Layane Wazen, Andrea Diez
 * @brief Public interface for drawing on model_surface
 * @version 0.1
 * @date 2022-04-20
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef GRAPHICS_H
#define GRAPHICS_H

/**
 * @namespace Graphic
 * @brief Provides all the methods needed to draw on the model_surface of size (g_max -
 * 1, g_max - 1)
 */
namespace Graphic
{
    /**
     * @brief Erases everything from the model_surface
     */
    void clear_model_surface();

    /**
     * @brief Draws a white diamond with size 1 at (@p x, @p y)
     *
     * @param x bottom-left x coordinate
     * @param y bottom-left y coordinate
     */
    void draw_diamond(unsigned int x, unsigned int y);

    /**
     * @brief Draws a thick border in the center of the cells next to the border of a
     * square with size @p side and color @p color_index
     *
     * @param x bottom-left x coordinate
     * @param y bottom-left y coordinate
     * @param side size of the square
     * @param color_index (0 red, 1 green, 2 blue, 3 yellow, 4 magenta, 5 cyan)
     */
    void draw_thick_border_square(unsigned int x, unsigned int y, unsigned int side,
                                  unsigned int color_index);

    /**
     * @brief Draws a filled square with size @p side at (@p x, @p y) and color @p
     * color_index
     *
     * @param x bottom-left x coordinate
     * @param y bottom-left y coordinate
     * @param side size of the square
     * @param color_index (0 red, 1 green, 2 blue, 3 yellow, 4 magenta, 5 cyan)
     */
    void draw_filled_square(unsigned int x, unsigned int y, unsigned int side,
                            unsigned int color_index);

    /**
     * @brief Draws a square with size @p side at (@p x, @p y) filled with |X O||O
     * X|... pattern where X has color @p color_index and O a lighter version of @p
     * color_index
     *
     * @param x bottom-left x coordinate
     * @param y bottom-left y coordinate
     * @param side size of the square
     * @param color_index (0 red, 1 green, 2 blue, 3 yellow, 4 magenta, 5 cyan)
     */
    void draw_diagonal_pattern_square(unsigned int x, unsigned int y,
                                      unsigned int side, unsigned int color_index);

    /**
     * @brief Draws a square with size @p side at (@p x, @p y) filled with a lighter
     * version of @p color_index and a plus sign in the center with color @p
     * color_index
     *
     * @param x bottom-left x coordinate
     * @param y bottom-left y coordinate
     * @param side size of the square
     * @param color_index (0 red, 1 green, 2 blue, 3 yellow, 4 magenta, 5 cyan)
     */
    void draw_plus_pattern_square(unsigned int x, unsigned int y, unsigned int side,
                                  unsigned int color_index);
} // namespace Graphic

#endif