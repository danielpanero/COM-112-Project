#include "iostream"

#include "gtkmm/buttonbox.h"
#include "gtkmm/frame.h"
#include "gtkmm/grid.h"

#include "gui.h"

constexpr unsigned int XS_MARGIN = 3;
constexpr unsigned int SM_MARGIN = 5;
constexpr unsigned int MD_MARGIN = 10;
constexpr unsigned int LG_MARGIN = 10;

using Gtk::make_managed;

MainWindow::MainWindow()
    : open_button("Open"), save_button("Save"), start_stop_button("Start"),
      step_button("Step"), next_anthill_button("Next"), prev_anthill_button("Prev")
{
    this->set_title("Main");

    auto *grid = make_managed<Gtk::Grid>();

    grid->set_margin_top(MD_MARGIN);
    grid->set_margin_left(MD_MARGIN);
    grid->set_margin_bottom(MD_MARGIN);
    grid->set_margin_right(MD_MARGIN);

    grid->set_row_spacing(LG_MARGIN);

    // General Button
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

    // Food count
    auto *food_frame = make_managed<Gtk::Frame>();
    food_frame->set_label("Food count:");

    food_count_label.set_markup("<b>0</b>");
    food_frame->add(food_count_label);

    // Anthill report
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

    grid->attach(*general_button_frame, 0, 0);
    grid->attach(*food_frame, 0, 1);
    grid->attach(*anthill_frame, 0, 2);

    this->add(*grid);
    this->show_all_children();
}