#ifndef GUI_H
#define GUI_H

#include "gtkmm/button.h"
#include "gtkmm/label.h"
#include "gtkmm/window.h"

class MainWindow : public Gtk::Window
{

public:
    MainWindow();
    ~MainWindow() = default;

private:
    Gtk::Button open_button, save_button, start_stop_button, step_button;
    Gtk::Button next_anthill_button, prev_anthill_button;

    Gtk::Label food_count_label, anthill_info_label;
};

#endif