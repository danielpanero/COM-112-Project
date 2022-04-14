#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "gtkmm/drawingarea.h"

void inject_cairo_context(const Cairo::RefPtr<Cairo::Context> &cairo_context);
void draw_empty_grid();

#endif