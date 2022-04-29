/**
 * @file projet.cc
 * @author Daniel Panero [+21/-1], Layane Wazen [+0/-0], Andrea Diez [+0/-0]
 * @version 0.1
 * @date 2022-04-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "iostream"

#include "gtkmm/application.h"

#include "gui.h"
#include "simulation.h"

using std::string;

int main(int argc, char *argv[])
{

/** When the preprocessor directive HEADLESS is present, the program will be compiled
 * and ran without the GUI, as such it is used mainly for automatic testing (any error
 * will kill the program). In order to compile the program in HEADLESS mode, it
 * suffices to compile it using: make HEADLESS=true
 */
#ifndef HEADLESS

    // TODO(@danielpanero): check if we need to parse the arguments in any case
    auto app = Gtk::Application::create(argc, argv, "org.com112.project");

    Simulation simulation;
    MainWindow main(&simulation);

    return app->run(main);

#else

    if (argc == 1)
    {
        exit(EXIT_FAILURE);
    }

    string path(argv[1]);

    Simulation simulation;
    simulation.read_file(path);

    exit(0);

#endif
}