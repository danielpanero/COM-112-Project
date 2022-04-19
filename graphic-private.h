#ifndef GRAPHICS_PRIVATE_H
#define GRAPHICS_PRIVATE_H

#include "cairomm/matrix.h"
#include "cairomm/surface.h"

Cairo::RefPtr<Cairo::ImageSurface> create_background_grid_surface();
Cairo::RefPtr<Cairo::ImageSurface> create_model_surface();

Cairo::Matrix get_scaling_matrix(Cairo::Matrix ctm, int width, int height);
Cairo::Matrix get_translating_matrix(Cairo::Matrix ctm);

void clear_model_surface();

#endif