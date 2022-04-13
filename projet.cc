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
#include "gtkmm/window.h"

#include "simulation.h"

using std::string;

int main(int argc, char *argv[])
{
    auto app = Gtk::Application::create(argc, argv, "org.com112.project");

    Gtk::Window window;
    window.set_default_size(200, 200);
    window.set_title("Main");

    return app->run(window);
    
    // if (argc == 1)
    // {
    //     exit(EXIT_FAILURE);
    // }

    // string path(argv[1]);

    // Simulation simulation;
    // simulation.read_file(path);

    // exit(0);
}
