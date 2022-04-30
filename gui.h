/**
 * @file gui.h
 * @author Daniel Panero [+145/-29], Andrea Diez [+0/-0]
 * @version 0.1
 * @date 2022-04-20
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef GUI_H
#define GUI_H

#include <gtkmm-3.0/gtkmm/button.h>
#include <gtkmm-3.0/gtkmm/drawingarea.h>
#include <gtkmm-3.0/gtkmm/frame.h>
#include <gtkmm-3.0/gtkmm/grid.h>
#include <gtkmm-3.0/gtkmm/label.h>
#include <gtkmm-3.0/gtkmm/window.h>

#include "simulation.h"

class MainWindow : public Gtk::Window
{

public:
    /**
     * @brief Construct a new Main Window instance
     *
     * @param simulation pointer to the current simulation
     */
    explicit MainWindow(Simulation *simulation);
    ~MainWindow() override = default;

private:
    Simulation *simulation;

    void build_layout_general_box();
    void build_layout_food_box();
    void build_layout_anthill_box();
    void build_layout_graphic();
    void reset_layout();

    void on_open_button_click();
    void on_save_button_click();

    void on_start_stop();
    void on_prev();
    void on_next();

    bool on_iteration();

    /**
     * @brief Signal handler for the keyboard shortcuts:
     * @b s start/stop the simulation,
     * @b 1 one step in the simulation,
     * @b p retrieves the information of the prev anthill
     * @b n retrieves the information of the next anthill
     *
     * @param event
     */
    bool on_key_release_complete(GdkEventKey *event);

    /**
     * @brief Signal handler for the keyboard shortcuts when the simulation is running:
     * @b s start/stop the simulation
     *
     * @param event
     */
    bool on_key_release_reduced(GdkEventKey *event);

    /**
     * @brief Signal handler for drawing_area: applies the right ctms and draws
     * background_grid_surface and model_surface
     *
     * @param cc
     */
    bool on_draw_request(const Cairo::RefPtr<Cairo::Context> &cc);

    /**
     * @brief Pointer to background_grid_surface define in the graphic module
     *
     */
    Cairo::RefPtr<Cairo::ImageSurface> background_grid_surface;

    /**
     * @brief Pointer to model_surface defined in the graphic module
     *
     */
    Cairo::RefPtr<Cairo::ImageSurface> model_surface;

    /**
     * keyboard_shortcuts_reduced and complete are needed for connecting/disconnecting
     * the signal handlers when needed, i.e: while empty disconnecting both, while
     * running only reduced
     */
    sigc::connection keyboard_shortcuts_reduced;
    sigc::connection keyboard_shortcuts_complete;

    sigc::connection idle;

    Gtk::Grid grid;
    Gtk::Frame general_button_frame, food_frame, anthill_frame;
    Gtk::DrawingArea drawing_area;
    Gtk::Button exit_button, open_button, save_button, start_stop_button, step_button,
        next_anthill_button, prev_anthill_button;
    Gtk::Label food_count_label, anthill_info_label;

    unsigned int iteration = 0;
};

#endif