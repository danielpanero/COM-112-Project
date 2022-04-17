#ifndef GRAPHICS_PRIVATE_H
#define GRAPHICS_PRIVATE_H

#include "cairomm/matrix.h"
#include "cairomm/surface.h"

Cairo::RefPtr<Cairo::ImageSurface> create_background_grid_surface();
Cairo::Matrix get_background_grid_matrix(Cairo::Matrix ctm, int width, int height);

#endif