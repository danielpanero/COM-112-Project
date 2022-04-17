#include "vector"

#include "gdkmm/general.h"
#include "gdkmm/rgba.h"
#include "gtkmm/drawingarea.h"

// TODO(@danielpanero) check if is better add g_max in constant file
constexpr double g_max(128);

const std::vector<typename Gdk::RGBA>
    colors({Gdk::RGBA("red"), Gdk::RGBA("green"), Gdk::RGBA("blue"),
            Gdk::RGBA("yellow"), Gdk::RGBA("magenta"), Gdk::RGBA("cyan")});

const Gdk::RGBA grid_lines_color("grey");

// TODO(@danielpanero): these variables depends only on the scale_factor we
// could also inject it together with cairo context
const double grid_linewidth(0.2);
const double thick_border_linewidth(0.3);

// TODO(@danielpanero): refptr vs pointer on a refptr
static Cairo::RefPtr<Cairo::Context> cc(nullptr);
void inject_cairo_context(const Cairo::RefPtr<Cairo::Context> &cairo_context)
{
    cc = cairo_context;
}

Gdk::RGBA get_color(unsigned int &color_index)
{
    return colors[color_index % colors.size()];
}

void draw_empty_grid()
{
    // White background
    cc->set_source_rgb(1.0, 1.0, 1.0);
    cc->paint();

    // Black rectangle (1,1) - (g_max - 1, g_max -1)
    cc->set_source_rgb(0.0, 0.0, 0.0);
    cc->rectangle(1, 1, g_max - 2, g_max - 2);
    cc->fill();

    // Grid lines
    Gdk::Cairo::set_source_rgba(cc, grid_lines_color);
    cc->set_line_width(grid_linewidth);
    for (int i(0); i < g_max; i++)
    {
        // Horinzontal lines
        cc->move_to(0.0, i);
        cc->line_to(g_max, i);
        cc->stroke();

        // Vertical lines
        cc->move_to(i, 0.0);
        cc->line_to(i, g_max);
        cc->stroke();
    }
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
    Gdk::Cairo::set_source_rgba(cc, get_color(color_index));
    cc->set_line_width(thick_border_linewidth);

    cc->rectangle(x + 0.5, y + 0.5, side - 1, side - 1);
    cc->stroke();
}

void draw_filled_square(unsigned int &x, unsigned int &y, unsigned int &side,
                        unsigned int &color_index)
{
    Gdk::Cairo::set_source_rgba(cc, get_color(color_index));

    cc->rectangle(x, y, side, side);
    cc->fill();
}

Cairo::RefPtr<Cairo::SurfacePattern> create_diagonal_pattern(unsigned int &color_index)
{
    Gdk::RGBA color1(get_color(color_index));
    Gdk::RGBA color2(get_color(color_index));

    color2.set_alpha(0.5);

    auto surface = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, 2, 2);
    auto cc = Cairo::Context::create(surface);

    cc->set_source_rgb(1.0, 1.0, 1.0);
    cc->paint();

    Gdk::Cairo::set_source_rgba(cc, color1);
    cc->rectangle(0, 0, 1, 1);
    cc->rectangle(1, 1, 1, 1);
    cc->fill();

    Gdk::Cairo::set_source_rgba(cc, color2);
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
    Gdk::RGBA color1(get_color(color_index));
    Gdk::RGBA color2(get_color(color_index));

    color2.set_alpha(0.5);

    cc->set_source_rgb(1.0, 1.0, 1.0);
    cc->rectangle(x, y, side, side);
    cc->fill();

    Gdk::Cairo::set_source_rgba(cc, color2);
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
    Gdk::Cairo::set_source_rgba(cc, color1);
    cc->rectangle(x1, y, 1, side);
    cc->rectangle(x, y1, side, 1);
    cc->fill();
}