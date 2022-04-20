/**
 * @file graphic-private.h
 * @author Daniel Panero, Layane Wazen, Andrea Diez
 * @version 0.1
 * @date 2022-04-20
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef GRAPHICS_PRIVATE_H
#define GRAPHICS_PRIVATE_H

#include "cairomm/matrix.h"
#include "cairomm/surface.h"

// TODO(@danielpanero): transform also this in namespace?
Cairo::RefPtr<Cairo::ImageSurface> create_background_grid_surface();
Cairo::RefPtr<Cairo::ImageSurface> create_model_surface();

Cairo::Matrix scale_to_allocation_size(Cairo::Matrix ctm, int width, int height);
Cairo::Matrix shift_from_border(Cairo::Matrix ctm);

void clear_model_surface();

#endif