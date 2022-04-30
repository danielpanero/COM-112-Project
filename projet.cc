/**
 * @file projet.cc
 * @author Daniel Panero [+104/-55], Layane Wazen [+0/-0], Andrea Diez [+0/-0]
 * @version 0.1
 * @date 2022-04-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <gtkmm-3.0/gtkmm/application.h>

#include "gui.h"
#include "simulation.h"

using std::string;

int main(int argc, char *argv[])
{
    auto app = Gtk::Application::create("org.com112.project");

    Simulation simulation;
    MainWindow main(&simulation);

    if (argc == 2)
    {
        string path(argv[1]);
        if (simulation.read_file(path))
        {
            main.enable_layout();
        }
    }

/** When the preprocessor directive HEADLESS is present, the program will be compiled
 * and ran without the GUI, as such it is used mainly for automatic testing (any error
 * will kill the program). In order to compile the program in HEADLESS mode, it
 * suffices to compile it using: make HEADLESS=true
 */
#ifndef HEADLESS

    return app->run(main);

#else

    return 0;

#endif
}