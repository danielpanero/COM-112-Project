#include "iostream"

#include "gtkmm/buttonbox.h"
#include "gtkmm/filechooserdialog.h"
#include "gtkmm/frame.h"
#include "gtkmm/grid.h"

#include "gui.h"

constexpr unsigned int XS_MARGIN = 3;
constexpr unsigned int SM_MARGIN = 5;
constexpr unsigned int MD_MARGIN = 10;
constexpr unsigned int LG_MARGIN = 10;

using std::string;

using Gtk::make_managed;

MainWindow::MainWindow()
    : open_button("Open"), save_button("Save"), start_stop_button("Start"),
      step_button("Step"), next_anthill_button("Next"), prev_anthill_button("Prev")
{
    this->set_title("Main");

    grid.set_margin_top(MD_MARGIN);
    grid.set_margin_left(MD_MARGIN);
    grid.set_margin_bottom(MD_MARGIN);
    grid.set_margin_right(MD_MARGIN);

    grid.set_row_spacing(LG_MARGIN);

    this->build_layout_general_box();
    this->build_layout_food_anthill_box();

    this->add(grid);
    this->show_all_children();
}

void MainWindow::build_layout_general_box()
{
    // Layout
    auto *general_button_box = make_managed<Gtk::ButtonBox>(Gtk::ORIENTATION_VERTICAL);
    general_button_box->set_spacing(XS_MARGIN);
    general_button_box->set_margin_left(SM_MARGIN);
    general_button_box->set_margin_right(SM_MARGIN);
    general_button_box->set_margin_bottom(SM_MARGIN);

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

void MainWindow::build_layout_food_anthill_box()
{
    auto *food_frame = make_managed<Gtk::Frame>();
    food_frame->set_label("Food count:");

    food_count_label.set_markup("<b>0</b>");
    food_frame->add(food_count_label);

    auto *anthill_box = make_managed<Gtk::Box>(Gtk::ORIENTATION_VERTICAL);
    anthill_box->set_spacing(XS_MARGIN);
    anthill_box->set_margin_left(SM_MARGIN);
    anthill_box->set_margin_right(SM_MARGIN);
    anthill_box->set_margin_bottom(SM_MARGIN);

    anthill_info_label.set_markup("<b>No selection</b>");

    anthill_box->pack_end(anthill_info_label);
    anthill_box->pack_end(prev_anthill_button);
    anthill_box->pack_end(next_anthill_button);

    auto *anthill_frame = make_managed<Gtk::Frame>();
    anthill_frame->set_label("Anthill report:");
    anthill_frame->add(*anthill_box);

    grid.attach(*food_frame, 0, 1);
    grid.attach(*anthill_frame, 0, 2);
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
    }
}