/**
 * @file graphic.h
 * @author Daniel Panero [+127/-27], Andrea Diez [+9/-9]
 * @brief Public interface for drawing on the current surface
 * @version 0.1
 * @date 2022-04-20
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "gdkmm/rgba.h"

/**
 * @namespace Graphic
 * @brief Provides all the methods needed to draw on the current surface
 */
namespace Graphic
{
    /**
     * @brief Erases everything from the surface
     */
    void clear_surface();

    /**
     * @brief Draws a transparent grid mesh with cell size 1 that spans all over the
     * surface
     *
     */
    void draw_grid_mesh();

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
     * @brief Same as \b draw_filled_square
     *
     * @param x
     * @param y
     * @param side
     * @param color RGBA color from the library Gdkmm
     */
    void draw_filled_square(unsigned int x, unsigned int y, unsigned int side,
                            Gdk::RGBA color);

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