/**
 * @file graphic.h
 * @author Daniel Panero, Layane Wazen, Andrea Diez
 * @version 0.1
 * @date 2022-04-20
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef GRAPHICS_H
#define GRAPHICS_H

namespace Graphic
{
    void clear_model_surface();
    void draw_diamond(unsigned int &x, unsigned int &y);
    void draw_thick_border_square(unsigned int &x, unsigned int &y, unsigned int &side,
                                  unsigned int &color_index);
    void draw_filled_square(unsigned int &x, unsigned int &y, unsigned int &side,
                            unsigned int &color_index);
    void draw_diagonal_pattern_square(unsigned int &x, unsigned int &y,
                                      unsigned int &side, unsigned int &color_index);
    void draw_plus_pattern_square(unsigned int &x, unsigned int &y, unsigned int &side,
                                  unsigned int &color_index);
} // namespace Graphic

#endif