/**
 * @file graphic-private.h
 * @brief Private interface only for retrieving the current surface and the relative
 * transformation matrice
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

#include <cairomm/matrix.h>
#include <cairomm/surface.h>

/**
 * @brief Creates a new Cairo surface, sets as the current surface and returns the
 * pointer to it
 *
 * @param size size of the surface
 * @return Cairo::RefPtr<Cairo::ImageSurface>
 */
Cairo::RefPtr<Cairo::ImageSurface> create_surface(unsigned int size);

/**
 * @brief Returns the CTM scaled appropriately to fit in a square of size @p width / @p
 * height
 *
 * @param ctm current trasformation matrix
 * @param width of the allocation
 * @param height of the allocation
 * @return Cairo::Matrix
 */
Cairo::Matrix calculate_trasformation_matrix(Cairo::Matrix ctm, double width,
                                             double height);

#endif