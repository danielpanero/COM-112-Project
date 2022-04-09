#include "iostream"

#include "gtkmm/aspectframe.h"
#include "gtkmm/buttonbox.h"
#include "gtkmm/drawingarea.h"
#include "gtkmm/filechooserdialog.h"
#include "gtkmm/frame.h"
#include "gtkmm/grid.h"

#include "gui.h"

constexpr unsigned int xs_margin = 3;
constexpr unsigned int sm_margin = 5;
constexpr unsigned int md_margin = 10;
constexpr unsigned int lg_margin = 10;

constexpr unsigned drawing_area_size = 500;

using std::string;

using Gtk::make_managed;

MainWindow::MainWindow(Simulation *simulation)
    : simulation(simulation), open_button("Open"), save_button("Save"),
      start_stop_button("Start"), step_button("Step"), next_anthill_button("Next"),
      prev_anthill_button("Prev")
{
    this->set_title("Main");

    grid.set_margin_top(md_margin);
    grid.set_margin_left(md_margin);
    grid.set_margin_bottom(md_margin);
    grid.set_margin_right(md_margin);

    grid.set_column_spacing(lg_margin);
    grid.set_row_spacing(lg_margin);

    this->build_layout_general_box();
    this->build_layout_anthill_box();
    this->build_layout_graphic();

    // This frame is invisible and expandable, so the others controlls don't resize
    auto *resizable_frame = make_managed<Gtk::Frame>();
    resizable_frame->set_vexpand();
    resizable_frame->set_shadow_type(Gtk::SHADOW_NONE);

    grid.attach(*resizable_frame, 0, 3);

    this->add(grid);
    this->show_all_children();
}

void MainWindow::build_layout_general_box()
{
    // Layout
    auto *general_button_box = make_managed<Gtk::ButtonBox>(Gtk::ORIENTATION_VERTICAL);
    general_button_box->set_spacing(xs_margin);
    general_button_box->set_margin_left(sm_margin);
    general_button_box->set_margin_right(sm_margin);
    general_button_box->set_margin_bottom(sm_margin);

    general_button_box->pack_end(open_button);
    general_button_box->pack_end(save_button);
    general_button_box->pack_end(start_stop_button);
    general_button_box->pack_end(step_button);

    auto *general_button_frame = make_managed<Gtk::Frame>();
    general_button_frame->set_label("General:");
    general_button_frame->add(*general_button_box);

    grid.attach(*general_button_frame, 0, 0);

    // Signals Binding
    open_button.signal_clicked().connect(
        sigc::mem_fun(*this, &MainWindow::on_open_button_click));
    save_button.signal_clicked().connect(
        sigc::mem_fun(*this, &MainWindow::on_save_button_click));
}

void MainWindow::build_layout_food_box()
{
    auto *food_frame = make_managed<Gtk::Frame>();
    food_frame->set_label("Food count:");
    food_count_label.set_markup("<b>0</b>");

    food_frame->add(food_count_label);

    grid.attach(*food_frame, 0, 1);
}

void MainWindow::build_layout_anthill_box()
{
    auto *anthill_box = make_managed<Gtk::Box>(Gtk::ORIENTATION_VERTICAL);
    anthill_box->set_spacing(xs_margin);
    anthill_box->set_margin_left(sm_margin);
    anthill_box->set_margin_right(sm_margin);
    anthill_box->set_margin_bottom(sm_margin);

    anthill_info_label.set_markup("<b>No selection</b>");

    anthill_box->pack_end(anthill_info_label);
    anthill_box->pack_end(prev_anthill_button);
    anthill_box->pack_end(next_anthill_button);

    auto *anthill_frame = make_managed<Gtk::Frame>();
    anthill_frame->set_label("Anthill report:");
    anthill_frame->add(*anthill_box);

    grid.attach(*anthill_frame, 0, 2);
}

void MainWindow::build_layout_graphic()
{
    auto *aspect_frame = make_managed<Gtk::AspectFrame>();
    aspect_frame->set(Gtk::ALIGN_START, Gtk::ALIGN_START, 1, false);
    aspect_frame->set_hexpand();
    aspect_frame->set_vexpand();

    aspect_frame->unset_label();
    aspect_frame->set_shadow_type(Gtk::SHADOW_NONE);

    auto *drawing_area = make_managed<Gtk::DrawingArea>();
    drawing_area->set_size_request(drawing_area_size, drawing_area_size);
    aspect_frame->add(*drawing_area);

    drawing_area->signal_draw().connect(
        sigc::mem_fun(*this, &MainWindow::on_custom_draw));

    grid.attach(*aspect_frame, 1, 0, 1, 4);
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
    if (result == Gtk::RESPONSE_OK)
    {
        string filename = dialog.get_filename();

        simulation->reset();
        simulation->read_file(filename);
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

bool MainWindow::on_custom_draw(const Cairo::RefPtr<Cairo::Context> &cr)
{
    cr->set_source_rgb(0, 0, 0);
    cr->paint();

    return true;
}