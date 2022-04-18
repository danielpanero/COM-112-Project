#include "vector"

#include "cairomm/matrix.h"
#include "cairomm/surface.h"
#include "gdkmm/general.h"
#include "gdkmm/rgba.h"

#include "graphic-private.h"
#include "graphic.h"

using Gdk::RGBA;
using Gdk::Cairo::set_source_rgba;

// TODO(@danielpanero) check if is better add g_max in constant file
constexpr double g_max(128);
constexpr double surface_size(g_max + 2);
constexpr unsigned int scale_factor(10);
constexpr double grid_linewidth(0.2);
constexpr double thick_border_linewidth(0.3);

const std::vector<typename Gdk::RGBA>
    colors({RGBA("red"), RGBA("green"), RGBA("blue"),
            RGBA("yellow"), RGBA("magenta"), RGBA("cyan")});

const RGBA grid_lines_color("grey");
const RGBA background_color("black");

static Cairo::RefPtr<Cairo::Context> cc(nullptr);

Cairo::RefPtr<Cairo::ImageSurface> create_background_grid_surface()
{
    auto surface =
        Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, surface_size * scale_factor,
                                    surface_size * scale_factor);
    auto cc = Cairo::Context::create(surface);

    cc->scale(scale_factor, scale_factor);
    cc->set_antialias(Cairo::Antialias::ANTIALIAS_NONE);

    // White background
    cc->set_source_rgb(1.0, 1.0, 1.0);
    cc->paint();

    // Black rectangle
    cc->set_source_rgb(0.0, 0.0, 0.0);
    cc->rectangle(1, 1, g_max, g_max);
    cc->fill();

    // Grid lines
    set_source_rgba(cc, grid_lines_color);
    cc->set_line_width(grid_linewidth);
    for (int i(0); i <= surface_size; i++)
    {
        // Horinzontal lines
        cc->move_to(0.0, i);
        cc->line_to(surface_size, i);
        cc->stroke();

        // Vertical lines
        cc->move_to(i, 0.0);
        cc->line_to(i, surface_size);
        cc->stroke();
    }

    surface->flush();
    return surface;
}

Cairo::RefPtr<Cairo::ImageSurface> create_model_surface()
{
    auto surface = Cairo::ImageSurface::create(
        Cairo::FORMAT_ARGB32, g_max * scale_factor, g_max * scale_factor);
    cc = Cairo::Context::create(surface);

    surface->flush();
    return surface;
}

Cairo::Matrix get_scaling_matrix(Cairo::Matrix ctm, int width, int height)
{
    ctm.scale(width / ((g_max + 2) * scale_factor),
              height / ((g_max + 2) * scale_factor));
    return ctm;
}

RGBA get_color(unsigned int &color_index)
{
    return colors[color_index % colors.size()];
}

void clear_model_surface()
{
    cc->save();
    cc->set_source_rgba(0, 0, 0, 0);
    cc->set_operator(Cairo::Operator::OPERATOR_CLEAR);
    cc->paint();
    cc->restore();
}

void draw_diamond(unsigned int &x, unsigned int &y)
{
    // TODO(@danielpanero) replace color with rgba and rectangle plus rotation is a
    // better solution
    cc->set_source_rgb(1.0, 1.0, 1.0);
    cc->move_to(x + 0.5, y);
    cc->line_to(x + 1, y + 0.5);
    cc->line_to(x + 0.5, y + 1);
    cc->line_to(x, y + 0.5);
    cc->fill();
}

void draw_thick_border_square(unsigned int &x, unsigned int &y, unsigned int &side,
                              unsigned int &color_index)
{
    set_source_rgba(cc, get_color(color_index));
    cc->set_line_width(thick_border_linewidth);

    cc->rectangle(x + 0.5, y + 0.5, side - 1, side - 1);
    cc->stroke();
}

void draw_filled_square(unsigned int &x, unsigned int &y, unsigned int &side,
                        unsigned int &color_index)
{
    set_source_rgba(cc, get_color(color_index));

    cc->rectangle(x, y, side, side);
    cc->fill();
}

Cairo::RefPtr<Cairo::SurfacePattern> create_diagonal_pattern(unsigned int &color_index)
{
    RGBA color1(get_color(color_index));
    RGBA color2(get_color(color_index));

    color2.set_alpha(0.5);

    auto surface = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, 2, 2);
    auto cc = Cairo::Context::create(surface);

    cc->set_source_rgb(1.0, 1.0, 1.0);
    cc->paint();

    set_source_rgba(cc, color1);
    cc->rectangle(0, 0, 1, 1);
    cc->rectangle(1, 1, 1, 1);
    cc->fill();

    set_source_rgba(cc, color2);
    cc->rectangle(1, 0, 1, 1);
    cc->rectangle(0, 1, 1, 1);
    cc->fill();

    surface->flush();

    auto pattern = Cairo::SurfacePattern::create(surface);
    pattern->set_filter(Cairo::Filter::FILTER_NEAREST);
    pattern->set_extend(Cairo::Extend::EXTEND_REPEAT);

    return pattern;
}

void draw_diagonal_pattern(unsigned int &x, unsigned int &y, unsigned int &side,
                           unsigned int &color_index)
{
    auto diagonal_pattern = create_diagonal_pattern(color_index);

    cc->set_source(diagonal_pattern);
    cc->rectangle(x, y, side, side);
    cc->fill();
}

void draw_plus_pattern(unsigned int &x, unsigned int &y, unsigned int &side,
                       unsigned int &color_index)
{
    RGBA color1(get_color(color_index));
    RGBA color2(get_color(color_index));

    color2.set_alpha(0.5);

    cc->set_source_rgb(1.0, 1.0, 1.0);
    cc->rectangle(x, y, side, side);
    cc->fill();

    set_source_rgba(cc, color2);
    cc->rectangle(x, y, side, side);
    cc->fill();

    double x1(0), y1(0);
    if ((side % 2) == 0)
    {
        x1 = x + static_cast<double>(side) / 2 - 0.5;
        y1 = y + static_cast<double>(side) / 2 - 0.5;
    }
    else
    {
        x1 = x + (side - 1) / 2;
        y1 = y + (side - 1) / 2;
    }
    set_source_rgba(cc, color1);
    cc->rectangle(x1, y, 1, side);
    cc->rectangle(x, y1, side, 1);
    cc->fill();
}