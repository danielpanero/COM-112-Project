#include "cmath"
#include "vector"

#include "cairomm/matrix.h"
#include "cairomm/surface.h"
#include "gdkmm/general.h"
#include "gdkmm/rgba.h"

#include "graphic-private.h"
#include "graphic.h"

using Gdk::RGBA;
using Gdk::Cairo::set_source_rgba;

// ====================================================================================
// Constants

// TODO(@danielpanero) check if is better add g_max in constant file
constexpr double g_max(128);
constexpr double cell_size(1);
constexpr double surface_size((g_max + 2) * cell_size);

/* The scale factor controls "image quality / resolution". As we increase it, the
 * quality improves, but rendering will be slower. 1/scale_factor correspondes to 1
 * pixel in the surface */
constexpr double scale_factor(5);
constexpr double grid_linewidth(1 / scale_factor);
constexpr double thick_border_linewidth(2 * 1 / scale_factor);

const RGBA grid_lines_color("grey");
const RGBA background_color("white");
const RGBA background_grid_color("black");
const RGBA diamond_color("white");

const std::vector<typename Gdk::RGBA> dark_colors({RGBA("red"), RGBA("green"),
                                                   RGBA("blue"), RGBA("yellow"),
                                                   RGBA("magenta"), RGBA("cyan")});
const std::vector<typename Gdk::RGBA>
    light_colors({RGBA("tomato"), RGBA("sea green"), RGBA("SkyBlue"),
                  RGBA("LightYellow"), RGBA("plum"), RGBA("LightCyan")});

// ====================================================================================
// Surfaces definition

// TODO(@danielpanero) create here or in the function below
const auto model_surface = Cairo::ImageSurface::create(
    Cairo::FORMAT_ARGB32, g_max *scale_factor, g_max *scale_factor);

Cairo::RefPtr<Cairo::ImageSurface> create_background_grid_surface()
{
    auto surface =
        Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, surface_size * scale_factor,
                                    surface_size * scale_factor);
    auto cc = Cairo::Context::create(surface);

    cc->scale(scale_factor, scale_factor);
    cc->set_antialias(Cairo::Antialias::ANTIALIAS_NONE);

    set_source_rgba(cc, background_color);
    cc->paint();

    set_source_rgba(cc, background_grid_color);
    // We are letting empty / white the cells next to the border
    cc->rectangle(cell_size, cell_size, g_max, g_max);
    cc->fill();

    // Grid lines
    set_source_rgba(cc, grid_lines_color);
    cc->set_line_width(grid_linewidth);
    for (int i(0); i < surface_size; i += cell_size)
    {
        // Horinzontal lines
        cc->move_to(0, i);
        cc->line_to(surface_size, i);
        cc->stroke();

        // Vertical lines
        cc->move_to(i, 0);
        cc->line_to(i, surface_size);
        cc->stroke();
    }

    surface->flush();
    return surface;
}

Cairo::RefPtr<Cairo::ImageSurface> create_model_surface() { return model_surface; }

// ====================================================================================
// Misc section

/**
 *
 * @brief This is a convenience function for creating the cairo context, applying the
 * scale_factor and disabling any type of aliasing
 *
 * @param surface
 * @return Cairo::RefPtr<Cairo::Context>
 */
Cairo::RefPtr<Cairo::Context>
create_default_cc(const Cairo::RefPtr<Cairo::ImageSurface> &surface)
{
    auto cc = Cairo::Context::create(surface);

    /** This ctm (current trasformation matrix) scales the context by the scale_factor,
     * flips the Y-axis and finally shift the Y-axis by g_max * scale_factor. At the
     * end the origin (0,0) will be in the left-bottom corner
     */
    Cairo::Matrix ctm{scale_factor, 0, 0, -scale_factor, 0, g_max * scale_factor};

    cc->set_matrix(ctm);
    cc->set_antialias(Cairo::Antialias::ANTIALIAS_NONE);

    return cc;
}

Cairo::Matrix scale_to_allocation_size(Cairo::Matrix ctm, int width, int height)
{
    ctm.scale(width / (surface_size * scale_factor),
              height / (surface_size * scale_factor));
    return ctm;
}

Cairo::Matrix shift_from_border(Cairo::Matrix ctm)
{
    ctm.translate(cell_size * scale_factor, cell_size * scale_factor);
    return ctm;
}

/**
 * @brief Retrieves the right color from either dark_colors or their lighter version
 *
 * @param color_index
 * @param light
 * @return RGBA
 */
RGBA get_color(unsigned int &color_index, bool light = false)
{
    if (light)
    {
        return light_colors[color_index % dark_colors.size()];
    }
    return dark_colors[color_index % dark_colors.size()];
}

// ====================================================================================
// Draw functions

void Graphic::clear_model_surface()
{
    auto cc = create_default_cc(model_surface);

    cc->save();
    cc->set_source_rgba(0, 0, 0, 0);
    cc->set_operator(Cairo::Operator::OPERATOR_CLEAR);
    cc->paint();
    cc->restore();

    model_surface->flush();
}

void clear_model_surface() { Graphic::clear_model_surface(); }

void Graphic::draw_diamond(unsigned int x, unsigned int y)
{
    auto cc = create_default_cc(model_surface);

    set_source_rgba(cc, diamond_color);
    cc->move_to(x + cell_size / 2, y);
    cc->line_to(x + cell_size, y + cell_size / 2);
    cc->line_to(x + cell_size / 2, y + cell_size);
    cc->line_to(x, y + cell_size / 2);
    cc->fill();

    model_surface->flush();
}

void Graphic::draw_thick_border_square(unsigned int x, unsigned int y,
                                       unsigned int side, unsigned int color_index)
{
    auto cc = create_default_cc(model_surface);

    set_source_rgba(cc, get_color(color_index));
    cc->set_line_width(thick_border_linewidth);

    // Margin of half cell_size from the border cells
    cc->rectangle(x + cell_size / 2, y + cell_size / 2, side - cell_size,
                  side - cell_size);
    cc->stroke();

    model_surface->flush();
}

void Graphic::draw_filled_square(unsigned int x, unsigned int y, unsigned int side,
                                 unsigned int color_index)
{
    auto cc = create_default_cc(model_surface);

    set_source_rgba(cc, get_color(color_index));

    cc->rectangle(x, y, side, side);
    cc->fill();

    model_surface->flush();
}

/**
 * @brief Creates the diagonal pattern: one square 2x2 |X O||X O| where X has color @p
 * color_index and O a lighter version of @p color_index
 *
 * @param color_index
 * @return Cairo::RefPtr<Cairo::SurfacePattern>
 */
Cairo::RefPtr<Cairo::SurfacePattern> create_diagonal_pattern(unsigned int &color_index)
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
    auto cc = create_default_cc(model_surface);

    /* Instead of creating each square independently, we create a square of 2x2, with
     * the diagonal pattern and use this as filling pattern */
    auto diagonal_pattern = create_diagonal_pattern(color_index);

    cc->set_source(diagonal_pattern);
    cc->rectangle(x, y, side, side);
    cc->fill();

    model_surface->flush();
}

void Graphic::draw_plus_pattern_square(unsigned int x, unsigned int y,
                                       unsigned int side, unsigned int color_index)
{
    auto cc = create_default_cc(model_surface);

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

    model_surface->flush();
}