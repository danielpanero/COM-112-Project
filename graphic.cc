#include "vector"

#include "gdkmm/rgba.h"
#include "gtkmm/drawingarea.h"

// TODO(@danielpanero) check if is better add g_max in constant file
constexpr double g_max(128);

// TODO(@danielpanero): refptr vs pointer on a refptr
static Cairo::RefPtr<Cairo::Context> cc(nullptr);
static const std::vector<typename Gdk::RGBA>
    colors({Gdk::RGBA("red"), Gdk::RGBA("green"), Gdk::RGBA("blu"),
            Gdk::RGBA("yellow"), Gdk::RGBA("magenta"), Gdk::RGBA("cyan")});

void inject_cairo_context(const Cairo::RefPtr<Cairo::Context> &cairo_context)
{
    cc = cairo_context;
}

void draw_empty_grid()
{
    cc->set_source_rgb(0.0, 0.0, 0.0);
    cc->paint();

    cc->set_source_rgb(1.0, 1.0, 1.0);
    cc->set_line_width(0.2);
    for (int i(0); i < g_max; i++)
    {
        // Horinzotal lines
        cc->move_to(0.0, i);
        cc->line_to(g_max, i);
        cc->stroke();

        // Vertical lines
        cc->move_to(i, 0.0);
        cc->line_to(i, g_max);
        cc->stroke();
    }

    cc ->move_to(0.0, 0.0);
    cc ->line_to(g_max, g_max);
    cc -> stroke();

}
