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
void draw_filled_square(unsigned int &x, unsigned int &y, unsigned int &side,
                        unsigned int &color_index);
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