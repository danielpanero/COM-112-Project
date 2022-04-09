#ifndef GUI_H
#define GUI_H

#include "gtkmm/button.h"
#include "gtkmm/label.h"
#include "gtkmm/window.h"
#include "gtkmm/grid.h"

class MainWindow : public Gtk::Window
{

public:
    MainWindow();
    ~MainWindow() = default;

private:
    void build_layout_general_box();
    void build_layout_food_anthill_box();


    void on_open_button_click();
    void on_save_button_click();

    Gtk::Grid grid;

    Gtk::Button open_button, save_button, start_stop_button, step_button;
    Gtk::Button next_anthill_button, prev_anthill_button;

    Gtk::Label food_count_label, anthill_info_label;
};

#endif