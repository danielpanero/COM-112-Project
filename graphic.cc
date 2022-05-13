/**
 * @file graphic.cc
 * @author Daniel Panero [+498/-221], Andrea Diez [+9/-10]
 * @brief Public interface for drawing on surface
 * @version 0.1
 * @date 2022-04-20
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <cmath>
#include <vector>

#include <cairomm/matrix.h>
#include <cairomm/surface.h>
#include <gdkmm/general.h>
#include <gdkmm/rgba.h>

#include "graphic-private.h"
#include "graphic.h"

using std::string;

using Gdk::RGBA;
using Gdk::Cairo::set_source_rgba;

// ====================================================================================
// Constants

constexpr double cell_size(1);
/* The scale factor controls "image quality / resolution". As we increase it, the
 * quality improves, but rendering will be slower. 1/scale_factor correspondes to 1
 * pixel in the surface */
constexpr double scale_factor(7);
constexpr double grid_linewidth(1 / scale_factor);
constexpr double thick_border_linewidth(2 * 1 / scale_factor);

const std::vector<string> dark_colors{"red",    "green",   "blue",
                                      "yellow", "magenta", "cyan"};
const std::vector<string> light_colors{"tomato",      "seaGreen", "skyBlue",
                                       "lightYellow", "plum",     "lightCyan"};

// ====================================================================================
// Surface definition

/** This is the current surface onto we will draw using all methods provided. At start
 * it is initialized to a surface of size 0, in order to prevent any segmentation fault
 * caused by calling the draw functions without initializing the surface
 */
auto surface = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, 0, 0);

Cairo::RefPtr<Cairo::ImageSurface> create_surface(unsigned int size)
{
    surface = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, size * scale_factor,
                                          size * scale_factor);
    return surface;
}

Cairo::Matrix calculate_trasformation_matrix(Cairo::Matrix ctm, double width,
                                             double height)
{
    ctm.scale(width / surface->get_width(), height / surface->get_height());
    return ctm;
}

// ====================================================================================
// Utils

/**
 *
 * @brief This is a convenience function for creating the cairo context, applying the
 * scale_factor and disabling any type of aliasing
 *
 * @param surface
 * @return Cairo::RefPtr<Cairo::Context>
 */
Cairo::RefPtr<Cairo::Context> create_default_cc()
{
    auto cc = Cairo::Context::create(surface);

    /** This ctm (current trasformation matrix) scales the context by the scale_factor,
     * flips the Y-axis and finally shift the Y-axis. At the
     * end the origin (0,0) will be in the left-bottom corner
     */
    double height(surface->get_height());
    Cairo::Matrix ctm{scale_factor, 0, 0, -scale_factor, 0, height};

    cc->set_matrix(ctm);
    cc->set_antialias(Cairo::Antialias::ANTIALIAS_NONE);

    return cc;
}

/**
 * @brief Retrieves the right color from either dark_colors or their lighter version
 *
 * @param color_index
 * @param light
 * @return RGBA
 */
string get_color(unsigned int color_index, bool light = false)
{
    if (light)
    {
        return light_colors[color_index % dark_colors.size()];
    }
    return dark_colors[color_index % dark_colors.size()];
}

// ====================================================================================
// Draw functions

void Graphic::clear_surface()
{
    auto cc = create_default_cc();

    cc->save();
    cc->set_source_rgba(0, 0, 0, 0);
    cc->set_operator(Cairo::Operator::OPERATOR_CLEAR);
    cc->paint();
    cc->restore();

    surface->flush();
}

void Graphic::draw_grid_mesh(string grid_lines_color, int cell_size)
{
    auto cc = create_default_cc();

    set_source_rgba(cc, RGBA(grid_lines_color));
    cc->set_line_width(grid_linewidth);

    for (int i(0); i < surface->get_height() / scale_factor; i += cell_size)
    {
        // Horinzontal lines
        cc->move_to(0, i);
        cc->line_to(surface->get_width() / scale_factor, i);
        cc->stroke();

        // Vertical lines
        cc->move_to(i, 0);
        cc->line_to(i, surface->get_height() / scale_factor);
        cc->stroke();
    }

    surface->flush();
}

void Graphic::draw_filled_diamond(unsigned int x, unsigned int y, double side,
                                  string color)
{
    auto cc = create_default_cc();

    set_source_rgba(cc, RGBA(color));

    cc->move_to(x + side / 2, y);
    cc->line_to(x + side, y + side / 2);
    cc->line_to(x + side / 2, y + side);
    cc->line_to(x, y + side / 2);
    cc->fill();

    surface->flush();
}

void Graphic::draw_filled_diamond(unsigned int x, unsigned int y, unsigned int side,
                                  unsigned int color_index)
{
    Graphic::draw_filled_diamond(x, y, side, get_color(color_index));
}

void Graphic::draw_thick_border_square(unsigned int x, unsigned int y,
                                       unsigned int side, string color)
{
    auto cc = create_default_cc();

    set_source_rgba(cc, RGBA(color));
    cc->set_line_width(thick_border_linewidth);

    // Margin of half cell_size from the border cells
    cc->rectangle(x + cell_size / 2, y + cell_size / 2, side - cell_size,
                  side - cell_size);
    cc->stroke();

    surface->flush();
}

void Graphic::draw_thick_border_square(unsigned int x, unsigned int y,
                                       unsigned int side, unsigned int color_index)
{
    Graphic::draw_thick_border_square(x, y, side, get_color(color_index));
}

void Graphic::draw_filled_square(unsigned int x, unsigned int y, unsigned int side,
                                 string color)
{
    auto cc = create_default_cc();

    set_source_rgba(cc, RGBA(color));

    cc->rectangle(x, y, side, side);
    cc->fill();

    surface->flush();
}

void Graphic::draw_filled_square(unsigned int x, unsigned int y, unsigned int side,
                                 unsigned int color_index)
{
    Graphic::draw_filled_square(x, y, side, get_color(color_index));
}

/**
 * @brief Creates the diagonal pattern: one square 2x2 |X O||O X| where X has color @p
 * color_index and O a lighter version of @p color_index
 *
 * @param color_index
 * @return Cairo::RefPtr<Cairo::SurfacePattern>
 */
Cairo::RefPtr<Cairo::SurfacePattern> create_diagonal_pattern(unsigned int color_index)
{
    RGBA dark_color(get_color(color_index));
    RGBA light_color(get_color(color_index, true));

    auto surface = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, 2, 2);
    auto cc = Cairo::Context::create(surface);

    set_source_rgba(cc, dark_color);
    cc->rectangle(0, 0, cell_size, cell_size);
    cc->rectangle(cell_size, cell_size, cell_size, cell_size);
    cc->fill();

    set_source_rgba(cc, light_color);
    cc->rectangle(cell_size, 0, cell_size, cell_size);
    cc->rectangle(0, cell_size, cell_size, cell_size);
    cc->fill();

    surface->flush();

    auto pattern = Cairo::SurfacePattern::create(surface);
    pattern->set_filter(Cairo::Filter::FILTER_NEAREST);
    pattern->set_extend(Cairo::Extend::EXTEND_REPEAT);

    return pattern;
}

void Graphic::draw_diagonal_pattern_square(unsigned int x, unsigned int y,
                                           unsigned int side, unsigned int color_index)
{
    auto cc = create_default_cc();

    /* Instead of creating each square independently, we create a square of 2x2, with
     * the diagonal pattern and use this as filling pattern */
    auto diagonal_pattern = create_diagonal_pattern(color_index);

    cc->set_source(diagonal_pattern);
    cc->rectangle(x, y, side, side);
    cc->fill();

    surface->flush();
}

void Graphic::draw_plus_pattern_square(unsigned int x, unsigned int y,
                                       unsigned int side, unsigned int color_index)
{
    auto cc = create_default_cc();

    RGBA dark_color(get_color(color_index));
    RGBA light_color(get_color(color_index, true));

    set_source_rgba(cc, light_color);
    cc->rectangle(x, y, side, side);
    cc->fill();

    double x_center = x + (side - cell_size) / 2;
    double y_center = y + (side - cell_size) / 2;

    set_source_rgba(cc, dark_color);
    cc->rectangle(x_center, y, cell_size, side);
    cc->rectangle(x, y_center, side, cell_size);
    cc->fill();

    surface->flush();
}

// ====================================================================================
// Undraw functions

void Graphic::undraw_square(unsigned int x, unsigned int y, unsigned int side)
{
    auto cc = create_default_cc();

    cc->save();
    cc->set_source_rgba(0, 0, 0, 0);
    cc->set_operator(Cairo::Operator::OPERATOR_CLEAR);
    cc->rectangle(x, y, side, side);
    cc->fill();
    cc->restore();

    surface->flush();
}

void Graphic::undraw_thick_border_square(unsigned int x, unsigned int y,
                                         unsigned int side)
{
    auto cc = create_default_cc();

    cc->save();
    cc->set_source_rgba(0, 0, 0, 0);
    cc->set_operator(Cairo::Operator::OPERATOR_CLEAR);

    cc->set_line_width(thick_border_linewidth);

    cc->rectangle(x + cell_size / 2, y + cell_size / 2, side - cell_size,
                  side - cell_size);
    cc->stroke();

    cc->restore();

    surface->flush();
}
