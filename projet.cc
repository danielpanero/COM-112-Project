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

#include "simulation.h"

using std::string;

int main(int argc, char const *argv[])
{
    auto app = Gtk::Application::create(argc, argv, "org.com112.project");

    Simulation simulation;
    MainWindow main(&simulation);

    return app->run(main);
}
