/**
 * @file gui.cc
 * @author Daniel Panero [+646/-209], Andrea Diez [+0/-0]
 * @version 0.1
 * @date 2022-04-20
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <iostream>

#include <glibmm.h>
#include <gtkmm-3.0/gtkmm/aspectframe.h>
#include <gtkmm-3.0/gtkmm/buttonbox.h>
#include <gtkmm-3.0/gtkmm/drawingarea.h>
#include <gtkmm-3.0/gtkmm/filechooserdialog.h>
#include <gtkmm-3.0/gtkmm/frame.h>
#include <gtkmm-3.0/gtkmm/grid.h>

#include "graphic-private.h"
#include "graphic.h"

#include "gui.h"

constexpr unsigned int xs_margin(3);
constexpr unsigned int sm_margin(5);
constexpr unsigned int md_margin(10);
constexpr unsigned int lg_margin(10);

constexpr unsigned int drawing_area_size(500);

using std::string;

string format_anthill_info_markup(unsigned int &index, unsigned int &n_collectors,
                                  unsigned int &n_defensors, unsigned int &n_predators,
                                  double &n_foods);

MainWindow::MainWindow(Simulation *simulation)
    : simulation(simulation), exit_button("Exit"), open_button("Open"),
      save_button("Save"), start_stop_button("Start"), step_button("Step"),
      next_anthill_button("Next"), prev_anthill_button("Prev")
{
    // Layout
    set_title("Main");

    grid.set_margin_top(md_margin);
    grid.set_margin_left(md_margin);
    grid.set_margin_bottom(md_margin);
    grid.set_margin_right(md_margin);
    grid.set_column_spacing(lg_margin);
    grid.set_row_spacing(lg_margin);

    build_layout_general_box();
    build_layout_food_box();
    build_layout_anthill_box();
    build_layout_graphic();

    // This frame is invisible and expandable, so the others controlls don't resize
    auto *resizable_frame = Gtk::manage(new Gtk::Frame());
    resizable_frame->set_vexpand();
    resizable_frame->set_shadow_type(Gtk::SHADOW_NONE);
    resizable_frame->unset_label();
    grid.attach(*resizable_frame, 0, 3, 1, 1);

    add(grid);
    show_all_children();

    // We initialize the surface for DrawingImage
    unsigned int const g_max(128);
    unsigned int const cell_size(1);

    background_grid_surface = create_surface(g_max);
    /** First we fill the surface with white, then with black and we let empty / white
     * the cells next to the border and finally we draw a grid mesh*/
    Graphic::draw_filled_square(0, 0, g_max, "white");
    Graphic::draw_filled_square(cell_size, cell_size, g_max - 2 * cell_size, "black");
    Graphic::draw_grid_mesh("grey", cell_size);
    model_surface = create_surface(g_max);

    // When closing, we have to stop idle, otherwise it will not stop
    signal_hide().connect(sigc::mem_fun(*this, &MainWindow::on_exit));
}

// ====================================================================================
// Layout initialization / disposition

void MainWindow::enable_layout()
{
    save_button.set_sensitive(true);
    start_stop_button.set_sensitive(true);
    step_button.set_sensitive(true);

    food_frame.set_sensitive(true);
    anthill_frame.set_sensitive(true);

    food_count_label.set_markup("<b>" + std::to_string(simulation->get_n_foods()) +
                                "</b>");
    anthill_info_label.set_markup("<small><b>No selection</b></small>");

    keyboard_shortcuts_complete = signal_key_release_event().connect(
        sigc::mem_fun(*this, &MainWindow::on_key_release_complete));
    keyboard_shortcuts_reduced = signal_key_release_event().connect(
        sigc::mem_fun(*this, &MainWindow::on_key_release_reduced));
}

void MainWindow::build_layout_general_box()
{
    // Layout
    auto *general_button_box =
        Gtk::manage(new Gtk::ButtonBox(Gtk::ORIENTATION_VERTICAL));
    general_button_box->set_spacing(xs_margin);
    general_button_box->set_margin_left(sm_margin);
    general_button_box->set_margin_right(sm_margin);
    general_button_box->set_margin_bottom(sm_margin);

    general_button_box->pack_end(exit_button);
    general_button_box->pack_end(open_button);
    general_button_box->pack_end(save_button);
    general_button_box->pack_end(start_stop_button);
    general_button_box->pack_end(step_button);

    general_button_frame.set_label("General:");
    general_button_frame.add(*general_button_box);

    grid.attach(general_button_frame, 0, 0, 1, 1);

    // Disabling buttons at start
    save_button.set_sensitive(false);
    start_stop_button.set_sensitive(false);
    step_button.set_sensitive(false);

    // Signals Binding
    exit_button.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_exit));
    open_button.signal_clicked().connect(
        sigc::mem_fun(*this, &MainWindow::on_open_button_click));
    save_button.signal_clicked().connect(
        sigc::mem_fun(*this, &MainWindow::on_save_button_click));
    start_stop_button.signal_clicked().connect(
        sigc::mem_fun(*this, &MainWindow::on_start_stop));
    prev_anthill_button.signal_clicked().connect(
        sigc::mem_fun(*this, &MainWindow::on_prev));
    next_anthill_button.signal_clicked().connect(
        sigc::mem_fun(*this, &MainWindow::on_next));
    step_button.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_step));
}

void MainWindow::build_layout_food_box()
{
    food_count_label.set_markup("<small><b>No simulation</b></small>");

    food_frame.set_label("Food count:");
    food_frame.set_sensitive(false); // We disable the frame at start

    food_frame.add(food_count_label);

    grid.attach(food_frame, 0, 1, 1, 1);
}

void MainWindow::build_layout_anthill_box()
{
    auto *anthill_box = manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL));
    anthill_box->set_spacing(xs_margin);
    anthill_box->set_margin_left(sm_margin);
    anthill_box->set_margin_right(sm_margin);
    anthill_box->set_margin_bottom(sm_margin);

    anthill_info_label.set_markup("<small><b>No simulation</b></small>");

    anthill_box->pack_end(anthill_info_label);
    anthill_box->pack_end(prev_anthill_button);
    anthill_box->pack_end(next_anthill_button);

    anthill_frame.set_label("Anthill report:");
    anthill_frame.add(*anthill_box);
    anthill_frame.set_sensitive(false); // We disable the frame at start

    grid.attach(anthill_frame, 0, 2, 1, 1);
}

void MainWindow::build_layout_graphic()
{
    // Layout
    auto *aspect_frame = Gtk::manage(new Gtk::AspectFrame());
    aspect_frame->set(Gtk::ALIGN_START, Gtk::ALIGN_START, 1, false);
    aspect_frame->set_hexpand();
    aspect_frame->set_vexpand();

    aspect_frame->unset_label();

    drawing_area.set_size_request(drawing_area_size, drawing_area_size);
    aspect_frame->add(drawing_area);

    grid.attach(*aspect_frame, 1, 0, 1, 4);

    // Signals Binding
    drawing_area.signal_draw().connect(
        sigc::mem_fun(*this, &MainWindow::on_draw_request));
}

void MainWindow::reset_layout()
{
    keyboard_shortcuts_complete.disconnect();
    keyboard_shortcuts_reduced.disconnect();
    idle.disconnect();

    save_button.set_sensitive(false);
    start_stop_button.set_sensitive(false);
    step_button.set_sensitive(false);

    food_frame.set_sensitive(false);
    anthill_frame.set_sensitive(false);

    start_stop_button.set_label("Start");

    food_count_label.set_markup("<small><b>No simulation</b></small>");
    anthill_info_label.set_markup("<small><b>No simulation</b></small>");

    Graphic::clear_surface();
    drawing_area.queue_draw();

    iteration = 0;
}

// ====================================================================================
// Signal handlers

void MainWindow::on_open_button_click()
{
    Gtk::FileChooserDialog dialog("Choose a file", Gtk::FILE_CHOOSER_ACTION_OPEN);
    dialog.set_transient_for(*this);
    dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
    dialog.add_button("_Open", Gtk::RESPONSE_OK);

    auto filter_text = Gtk::FileFilter::create();
    filter_text->set_name("Text files");
    filter_text->add_mime_type("text/plain");
    dialog.add_filter(filter_text);

    int result = dialog.run();

    reset_layout();

    if (result == Gtk::RESPONSE_OK)
    {
        string filename = dialog.get_filename();
        if (simulation->read_file(filename))
        {
            enable_layout();

            return;
        }
    }
}

void MainWindow::on_save_button_click()
{
    Gtk::FileChooserDialog dialog("Save the file", Gtk::FILE_CHOOSER_ACTION_SAVE);
    dialog.set_transient_for(*this);

    dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
    dialog.add_button("_Save", Gtk::RESPONSE_OK);

    int result = dialog.run();
    if (result == Gtk::RESPONSE_OK)
    {
        string filename = dialog.get_filename();

        simulation->save_file(filename);
    }
}

void MainWindow::on_start_stop()
{
    if (idle.connected())
    {
        idle.disconnect();

        anthill_frame.set_sensitive(true);
        open_button.set_sensitive(true);
        save_button.set_sensitive(true);
        step_button.set_sensitive(true);

        anthill_info_label.set_markup("<small><b>No selection</b></small>");
        start_stop_button.set_label("Start");

        keyboard_shortcuts_complete.unblock();
        keyboard_shortcuts_reduced.block();
    }
    else
    {
        anthill_frame.set_sensitive(false);
        open_button.set_sensitive(false);
        save_button.set_sensitive(false);
        step_button.set_sensitive(false);

        anthill_info_label.set_markup("<small><b>Running...</b></small>");
        start_stop_button.set_label("Stop");

        keyboard_shortcuts_complete.block();
        keyboard_shortcuts_reduced.unblock();

        idle = Glib::signal_idle().connect(
            sigc::mem_fun(*this, &MainWindow::on_iteration), Glib::PRIORITY_LOW);
    }
}

void MainWindow::on_prev()
{
    unsigned int index(0);
    unsigned int n_collectors(0);
    unsigned int n_defensors(0);
    unsigned int n_predators(0);
    double n_foods(0);

    if (simulation->get_info_prev_anthill(index, n_collectors, n_defensors,
                                          n_predators, n_foods))
    {
        anthill_info_label.set_markup(format_anthill_info_markup(
            index, n_collectors, n_defensors, n_predators, n_foods));
    }
    else
    {
        anthill_frame.set_sensitive(false);
        anthill_info_label.set_markup("<small><b>No anthill left</b></small>");
    }
}

void MainWindow::on_next()
{
    unsigned int index(0);
    unsigned int n_collectors(0);
    unsigned int n_defensors(0);
    unsigned int n_predators(0);
    double n_foods(0);

    if (simulation->get_info_next_anthill(index, n_collectors, n_defensors,
                                          n_predators, n_foods))
    {
        anthill_info_label.set_markup(format_anthill_info_markup(
            index, n_collectors, n_defensors, n_predators, n_foods));
    }
    else
    {
        anthill_frame.set_sensitive(false);
        anthill_info_label.set_markup("<small><b>No anthill left</b></small>");
    }
}

void MainWindow::on_step()
{ // TODO: change next prev text
    on_iteration();
}

void MainWindow::on_exit()
{
    idle.disconnect();

    std::exit(0);
}

bool MainWindow::on_iteration()
{
    if (simulation->step())
    {
        // TODO(@danielpanero): what to do when simulation reaches the end
    }

    iteration++;
    std::cout << "Iteration: " << iteration << "\n";

    food_count_label.set_markup("<b>" + std::to_string(simulation->get_n_foods()) +
                                "</b>");

    drawing_area.queue_draw();

    return true;
}

bool MainWindow::on_key_release_reduced(GdkEventKey *event)
{
    if (event->type == GDK_KEY_RELEASE && event->keyval == GDK_KEY_s)
    {
        on_start_stop();
        return true;
    }

    return false;
}

bool MainWindow::on_key_release_complete(GdkEventKey *event)
{
    if (event->type == GDK_KEY_RELEASE && event->keyval == GDK_KEY_s)
    {
        on_start_stop();
        return true;
    }

    if (event->type == GDK_KEY_RELEASE && event->keyval == GDK_KEY_1)
    {
        on_iteration();
        return true;
    }

    if (event->type == GDK_KEY_RELEASE && event->keyval == GDK_KEY_p)
    {
        on_prev();
        return true;
    }

    if (event->type == GDK_KEY_RELEASE && event->keyval == GDK_KEY_n)
    {
        on_next();
        return true;
    }

    return false;
}

bool MainWindow::on_draw_request(const Cairo::RefPtr<Cairo::Context> &cc)
{
    auto allocation = drawing_area.get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();

    // (CTM: current trasformation matrix)
    auto ctm = calculate_trasformation_matrix(cc->get_matrix(), width, height);
    cc->set_matrix(ctm);

    if (background_grid_surface)
    {
        cc->set_source(background_grid_surface, 0, 0);
        cc->paint();
    }

    if (model_surface)
    {
        cc->set_source(model_surface, 0, 0);
        cc->paint();
    }

    return true;
}

// ====================================================================================
// Misc

string format_anthill_info_markup(unsigned int &index, unsigned int &n_collectors,
                                  unsigned int &n_defensors, unsigned int &n_predators,
                                  double &n_foods)
{
    using std::to_string;

    std::string n_foods_trimmed =
        to_string(n_foods).substr(0, to_string(n_foods).find(".") + 3);

    return "<small><b>No. ID:</b>  <tt>" + to_string(index + 1) + "</tt>\r\r" +
           "<b>No. food:   </b>  <tt>" + n_foods_trimmed + "</tt>\r" +
           "<b>No. cols:   </b>  <tt>" + to_string(n_collectors) + "</tt>\r" +
           "<b>No. defs:   </b>  <tt>" + to_string(n_defensors) + "</tt>\r" +
           "<b>No. preds:</b> <tt>" + to_string(n_predators) + "</tt></small>";
}