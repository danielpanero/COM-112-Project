#include "vector"

#include "gdkmm/general.h"
#include "gdkmm/rgba.h"
#include "gtkmm/drawingarea.h"

// TODO(@danielpanero) check if is better add g_max in constant file
constexpr double g_max(128);

const std::vector<typename Gdk::RGBA>
    colors({Gdk::RGBA("red"), Gdk::RGBA("green"), Gdk::RGBA("blu"),
            Gdk::RGBA("yellow"), Gdk::RGBA("magenta"), Gdk::RGBA("cyan")});

const Gdk::RGBA grid_lines_color("grey");
const double grid_linewidth(0.02);

// TODO(@danielpanero): refptr vs pointer on a refptr
static Cairo::RefPtr<Cairo::Context> cc(nullptr);
void inject_cairo_context(const Cairo::RefPtr<Cairo::Context> &cairo_context)
{
    cc = cairo_context;
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
    cc->set_source_rgb(1.0, 1.0, 1.0);
    cc->move_to(x + 0.5, y);
    cc->line_to(x + 1, y + 0.5);
    cc->line_to(x + 0.5, y + 1);
    cc->line_to(x, y + 0.5);
    cc -> fill();
}