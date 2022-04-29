/**
 * @file graphic-private.h
 * @brief Private interface only for retrieving the background_grid_surface's,
 * model_surface's pointers and the relative transformation matrices
 *
 * @author Daniel Panero [+66/-4], Andrea Diez [+0/-0]
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

/**
 * @brief Creates the background grid surface, draws the grid and returns the pointer
 * to it
 *
 * @return Cairo::RefPtr<Cairo::ImageSurface> background_grid_surface
 */
Cairo::RefPtr<Cairo::ImageSurface> create_background_grid_surface();

/**
 * @brief Creates the model surface and returns the pointer to it
 *
 * @return Cairo::RefPtr<Cairo::ImageSurface> model_surface
 */
Cairo::RefPtr<Cairo::ImageSurface> create_model_surface();

/**
 * @brief Returns the CTM scaled appropriately to fit in a square of size @p width / @p
 * height
 *
 * @param ctm current trasformation matrix
 * @param width of the allocation
 * @param height of the allocation
 * @return Cairo::Matrix
 */
Cairo::Matrix scale_to_allocation_size(Cairo::Matrix ctm, int width, int height);

/**
 * @brief Return the CTM translated by one row / column, since the background surface
 * grid has a row / column of empty / white cells
 *
 * @param ctm current trasformation matrix
 * @return Cairo::Matrix
 */
Cairo::Matrix shift_from_border(Cairo::Matrix ctm);

/**
 * @brief This is a convenience function for erasing the model_surface, it calls
 * Graphic::clear_model_surface()
 *
 */
void clear_model_surface();

#endif