#ifndef GRAPHICS_H
#define GRAPHICS_H

void clear_model_surface();
void draw_diamond(unsigned int &x, unsigned int &y);
void draw_thick_border_square(unsigned int &x, unsigned int &y, unsigned int &side,
                              unsigned int &color_index);
void draw_filled_square(unsigned int &x, unsigned int &y, unsigned int &side,
                        unsigned int &color_index);
void draw_diagonal_pattern_square(unsigned int &x, unsigned int &y, unsigned int &side,
                                  unsigned int &color_index);
void draw_plus_pattern_square(unsigned int &x, unsigned int &y, unsigned int &side,
                              unsigned int &color_index);

#endif