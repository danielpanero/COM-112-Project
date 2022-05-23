/**
 * @file graphic.h
 * @author Daniel Panero, Andrea Diez
 * @brief Public interface for drawing on the current surface using either the standard
 * X11 color or the provided color (0 red, 1 green, 2 blue, 3 yellow, 4 magenta, 5
 * cyan)
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
 * @brief Provides all the methods needed to draw on the current surface
 */
namespace Graphic
{
    /**
     * @brief Erases everything from the surface
     */
    void clear_surface();

    /**
     * @brief Draws a transparent grid mesh with that spans all over the
     * surface
     *
     * @param grid_lines_color X11 color
     * @param cell_size
     */
    void draw_grid_mesh(const std::string &grid_lines_color, int cell_size);

    /**
     * @brief Draws a filled diamond with size @p side at (@p x, @p y) and color @p
     * color
     *
     * @param x bottom-left x coordinate
     * @param y bottom-left y coordinate
     * @param side size of the square
     * @param color X11 color
     */
    void draw_filled_diamond(unsigned int x, unsigned int y, double side,
                             const std::string &color);
    /**
     * @brief Same as \b draw_filled_diamond
     *
     * @param x
     * @param y
     * @param side
     * @param color_index (0 red, 1 green, 2 blue, 3 yellow, 4 magenta, 5 cyan)
     */
    void draw_filled_diamond(unsigned int x, unsigned int y, unsigned int side,
                             unsigned int color_index);

    /**
     * @brief Draws a thick border in the center of the cells next to the border of a
     * square with size @p side and color @p color_index
     *
     * @param x bottom-left x coordinate
     * @param y bottom-left y coordinate
     * @param side size of the square
     * @param color X11 color
     */
    void draw_thick_border_square(unsigned int x, unsigned int y, unsigned int side,
                                  const std::string &color);
    /**
     * @brief Same as \b draw_thick_border_square
     *
     * @param x
     * @param y
     * @param side
     * @param color_index (0 red, 1 green, 2 blue, 3 yellow, 4 magenta, 5 cyan)
     */
    void draw_thick_border_square(unsigned int x, unsigned int y, unsigned int side,
                                  unsigned int color_index);

    /**
     * @brief Draws a filled square with size @p side at (@p x, @p y) and color @p
     * color
     *
     * @param x bottom-left x coordinate
     * @param y bottom-left y coordinate
     * @param side size of the square
     * @param color X11 color
     */
    void draw_filled_square(unsigned int x, unsigned int y, unsigned int side,
                            const std::string &color);

    /**
     * @brief Same as \b draw_filled_square
     *
     * @param x
     * @param y
     * @param side
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

    /**
     * @brief Erases everything in the area of the square (x,y) with length \b side
     *
     * @param x
     * @param y
     * @param side
     */
    void undraw_square(unsigned int x, unsigned int y, unsigned int side);

    /**
     * @brief Erases the border of the square (x,y) with length \b side
     *
     * @param x
     * @param y
     * @param side
     */
    void undraw_thick_border_square(unsigned int x, unsigned int y, unsigned int side);

} // namespace Graphic

#endif