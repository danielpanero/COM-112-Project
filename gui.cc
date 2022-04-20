/**
 * @file gui.cc
 * @author Daniel Panero, Layane Wazen, Andrea Diez
 * @version 0.1
 * @date 2022-04-20
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "iostream"

#include "gtkmm/aspectframe.h"
#include "gtkmm/buttonbox.h"
#include "gtkmm/drawingarea.h"
#include "gtkmm/filechooserdialog.h"
#include "gtkmm/frame.h"
#include "gtkmm/grid.h"

#include "graphic-private.h"

#include "gui.h"

constexpr unsigned int xs_margin(3);
constexpr unsigned int sm_margin(5);
constexpr unsigned int md_margin(10);
constexpr unsigned int lg_margin(10);

constexpr unsigned int drawing_area_size(500);

using std::string;

string format_anthill_info_markup(unsigned int &index, unsigned int &n_collectors,
                                  unsigned int &n_defensors, unsigned int &n_predators,
                                  unsigned int &n_foods);

MainWindow::MainWindow(Simulation *simulation)
    : simulation(simulation), exit_button("Exit"), open_button("Open"),
      save_button("Save"), start_stop_button("Start"), step_button("Step"),
      next_anthill_button("Next"), prev_anthill_button("Prev")
{
    // Layout
    this->set_title("Main");

    grid.set_margin_top(md_margin);
    grid.set_margin_left(md_margin);
    grid.set_margin_bottom(md_margin);
    grid.set_margin_right(md_margin);

    grid.set_column_spacing(lg_margin);
    grid.set_row_spacing(lg_margin);

    this->build_layout_general_box();
    this->build_layout_food_box();
    this->build_layout_anthill_box();
    this->build_layout_graphic();

    // This frame is invisible and expandable, so the others controlls don't resize
    auto *resizable_frame = Gtk::make_managed<Gtk::Frame>();
    resizable_frame->set_vexpand();
    resizable_frame->set_shadow_type(Gtk::SHADOW_NONE);
    resizable_frame->unset_label();

    grid.attach(*resizable_frame, 0, 3);

    this->add(grid);
    this->show_all_children();

    // We initialize the surface for DrawingImage
    background_grid_surface = create_background_grid_surface();
    model_surface = create_model_surface();
}

void MainWindow::build_layout_general_box()
{
    // Layout
    auto *general_button_box =
        Gtk::make_managed<Gtk::ButtonBox>(Gtk::ORIENTATION_VERTICAL);
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

    grid.attach(general_button_frame, 0, 0);

    // Disabling buttons at start
    save_button.set_sensitive(false);
    start_stop_button.set_sensitive(false);
    step_button.set_sensitive(false);

    // Signals Binding
    exit_button.signal_clicked().connect([]() { std::exit(0); });
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
}

void MainWindow::build_layout_food_box()
{
    food_count_label.set_markup("<small><b>No simulation</b></small>");

    food_frame.set_label("Food count:");
    food_frame.set_sensitive(false); // We disable the frame at start

    food_frame.add(food_count_label);

    grid.attach(food_frame, 0, 1);
}

void MainWindow::build_layout_anthill_box()
{
    auto *anthill_box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_VERTICAL);
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

    grid.attach(anthill_frame, 0, 2);
}

void MainWindow::build_layout_graphic()
{
    // Layout
    auto *aspect_frame = Gtk::make_managed<Gtk::AspectFrame>();
    aspect_frame->set(Gtk::ALIGN_START, Gtk::ALIGN_START, 1, false);
    aspect_frame->set_hexpand();
    aspect_frame->set_vexpand();

    aspect_frame->unset_label();
    aspect_frame->set_shadow_type(Gtk::SHADOW_NONE);

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

    save_button.set_sensitive(false);
    start_stop_button.set_sensitive(false);
    step_button.set_sensitive(false);

    food_frame.set_sensitive(false);
    anthill_frame.set_sensitive(false);

    start_stop_button.set_label("Start");

    food_count_label.set_markup("<small><b>No simulation</b></small>");
    anthill_info_label.set_markup("<small><b>No simulation</b></small>");

    clear_model_surface();
}

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

    // TODO(@danielpanero) check what do when no files was choosen
    this->reset_layout();

    if (result == Gtk::RESPONSE_OK)
    {
        string filename = dialog.get_filename();
        if (simulation->read_file(filename))
        {
            save_button.set_sensitive(true);
            start_stop_button.set_sensitive(true);
            step_button.set_sensitive(true);

            food_frame.set_sensitive(true);
            anthill_frame.set_sensitive(true);

            food_count_label.set_markup(
                "<b>" + std::to_string(simulation->get_n_foods()) + "</b>");
            anthill_info_label.set_markup("<small><b>No selection</b></small>");

            // We connect the keyshorcuts only when the simulation is ready
            keyboard_shortcuts_complete = this->signal_key_release_event().connect(
                sigc::mem_fun(*this, &MainWindow::on_key_release_complete));
            keyboard_shortcuts_reduced = this->signal_key_release_event().connect(
                sigc::mem_fun(*this, &MainWindow::on_key_release_reduced));

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
        // TODO(@danielpanero): after saving what we do disable? Clean simulation?...
        simulation->save_file(filename);
    }
}

void MainWindow::on_start_stop()
{
    static bool running(false);

    if (running)
    {
        running = false;

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
        running = true;

        // TODO(@danielpanero) check if we want really to disable open file
        anthill_frame.set_sensitive(false);
        open_button.set_sensitive(false);
        save_button.set_sensitive(false);
        step_button.set_sensitive(false);

        anthill_info_label.set_markup("<small><b>Running...</b></small>");
        start_stop_button.set_label("Stop");

        keyboard_shortcuts_complete.block();
        keyboard_shortcuts_reduced.unblock();
    }
}

void MainWindow::on_prev()
{
    unsigned int index(0);
    unsigned int n_collectors(0);
    unsigned int n_defensors(0);
    unsigned int n_predators(0);
    unsigned int n_foods(0);

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
    unsigned int n_foods(0);

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

bool MainWindow::on_key_release_reduced(GdkEventKey *event)
{
    // TODO(@danielpanero): check if we want only to allow start and stop
    if (event->type == GDK_KEY_RELEASE && event->keyval == GDK_KEY_s)
    {
        this->on_start_stop();
        return true;
    }

    return false;
}

bool MainWindow::on_key_release_complete(GdkEventKey *event)
{
    if (event->type == GDK_KEY_RELEASE && event->keyval == GDK_KEY_s)
    {
        this->on_start_stop();
        return true;
    }

    if (event->type == GDK_KEY_RELEASE && event->keyval == GDK_KEY_1)
    {
        return true;
    }

    if (event->type == GDK_KEY_RELEASE && event->keyval == GDK_KEY_p)
    {
        this->on_prev();
        return true;
    }

    if (event->type == GDK_KEY_RELEASE && event->keyval == GDK_KEY_n)
    {
        this->on_next();
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
    Cairo::Matrix ctm0 = cc->get_matrix();
    Cairo::Matrix ctm1 = scale_to_allocation_size(ctm0, width, height);
    Cairo::Matrix ctm2 = shift_from_border(ctm1);

    cc->set_matrix(ctm1);
    if (background_grid_surface)
    {
        cc->set_source(background_grid_surface, 0, 0);
        cc->paint();
    }

    cc->set_matrix(ctm2);
    if (model_surface)
    {
        cc->set_source(model_surface, 0, 0);
        cc->paint();
    }

    return true;
}

string format_anthill_info_markup(unsigned int &index, unsigned int &n_collectors,
                                  unsigned int &n_defensors, unsigned int &n_predators,
                                  unsigned int &n_foods)
{
    using std::to_string;

    return "<small><b>No. ID:</b>  <tt>" + to_string(index) + "</tt>\r\r" +
           "<b>No. food:   </b>  <tt>" + to_string(n_foods) + "</tt>\r" +
           "<b>No. cols:   </b>  <tt>" + to_string(n_collectors) + "</tt>\r" +
           "<b>No. defs:   </b>  <tt>" + to_string(n_defensors) + "</tt>\r" +
           "<b>No. preds:</b> <tt>" + to_string(n_predators) + "</tt></small>";
}