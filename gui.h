#ifndef GUI_H
#define GUI_H

#include "gtkmm/button.h"
#include "gtkmm/frame.h"
#include "gtkmm/grid.h"
#include "gtkmm/label.h"
#include "gtkmm/window.h"

#include "simulation.h"

class MainWindow : public Gtk::Window
{

public:
    MainWindow(Simulation *simulation);
    ~MainWindow() = default;

private:
    Simulation *simulation;

    void build_layout_general_box();
    void build_layout_food_box();
    void build_layout_anthill_box();
    void build_layout_graphic();
    void reset_layout();

    void on_open_button_click();
    void on_save_button_click();
    void on_start_stop_button_click();

    bool on_custom_draw(const Cairo::RefPtr<Cairo::Context> & cr);

    Gtk::Grid grid;

    Gtk::Frame general_button_frame, food_frame, anthill_frame;

    Gtk::Button exit_button, open_button, save_button, start_stop_button, step_button;
    Gtk::Button next_anthill_button, prev_anthill_button;

    Gtk::Label food_count_label, anthill_info_label;
};

#endif