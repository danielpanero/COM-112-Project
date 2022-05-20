/**
 * @file collector.cc
 * @author Daniel Panero [+135/-10], Layane Wazen [+10/-0], Andrea Diez [+367/-204]
 * @version 0.1
 * @date 2022-04-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <cmath>
#include <memory>
#include <sstream>
#include <vector>

#include "constantes.h"
#include "message.h"
#include "squarecell.h"

#include "collector.h"

using std::istringstream;
using std::string;
using std::unique_ptr;
using std::vector;

using Squarecell::Square;

// TODO(@danielpanero): implement secondary goal

// ====================================================================================
// Initialization - Misc

Collector::Collector(unsigned int x, unsigned int y, unsigned int age,
                     State_collector state, unsigned int color_index)
    : Ant{x, y, sizeC, age, color_index}, state(state)
{
    Squarecell::test_square(*this);
    add_to_grid();
}

Collector::~Collector()
{
    remove_from_grid();
    undraw();

    /**
     * If it was carrying some food, we have to add it back to the simulation. This was
     * automatically done by the anthill which has direct access to the food, but since
     * we are clearing the grid and the surface, this would eliminate the food.
     * Therefore we have to manually set the position as true and redraw it
     *
     */
    if (state == LOADED)
    {
        Square food{x, y, 1, true};

        Squarecell::add_square(food);
        Squarecell::draw_as_diamond(food, "white");
    }
}

void Collector::add_to_grid()
{
    unsigned int superposed_x(0);
    unsigned int superposed_y(0);

    if (Squarecell::test_if_superposed_grid(*this, superposed_x, superposed_y))
    {
        throw std::invalid_argument(
            message::collector_overlap(x, y, superposed_x, superposed_y));
    }

    Squarecell::add_square(*this);
}

void Collector::remove_from_grid() { Squarecell::remove_square(*this); }

void Collector::draw() { Squarecell::draw_diagonal_pattern(*this, get_color_index()); }

void Collector::undraw() { Squarecell::undraw_square(*this); }

string Collector::get_as_string()
{
    return Ant::get_as_string() + " " +
           (state == State_collector::LOADED ? "true" : "false");
}

State_collector Collector::get_state() { return state; }

// ====================================================================================
// Simulation

bool Collector::step() { return increase_age(); }

bool Collector::return_to_anthill(Square &anthill_square)
{
    remove_from_grid();
    undraw();

    auto move =
        Squarecell::lee_algorithm(*this, anthill_square, &Collector::generate_moves,
                                  &Squarecell::test_if_border_touches);

    x = move.x;
    y = move.y;

    add_to_grid();
    draw();

    if (Squarecell::test_if_border_touches(*this, anthill_square))
    {
        state = EMPTY;
        return true;
    }

    return false;
}

bool Collector::search_food(std::unique_ptr<Food> &food)
{
    auto food_square = food->get_as_square();

    remove_from_grid();
    undraw();

    food->remove_from_grid();

    auto move =
        Squarecell::lee_algorithm(*this, food_square, &Collector::generate_moves,
                                  &Squarecell::test_if_superposed_two_square);

    x = move.x;
    y = move.y;

    add_to_grid();
    draw();

    if (Squarecell::test_if_superposed_two_square(*this, food_square))
    {
        state = LOADED;
        return true;
    }

    food->add_to_grid();

    return false;
}

bool Collector::find_target_food(vector<unique_ptr<Food>> &foods, size_t &target)
{
    // This is given by sqrt((g_max-1)^2 + (g_max - 1)^2), ie the diagonal
    unsigned int best_distance = M_SQRT2 * g_max;
    bool found = false;

    for (size_t i(0); i < foods.size(); i++)
    {
        Square food_square = foods[i]->get_as_square();
        if ((food_square.y - food_square.x) % 2 == (y - x) % 2)
        {
            unsigned int distance =
                std::max(food_square.x >= x ? food_square.x - x : x - food_square.x,
                         food_square.y >= y ? food_square.y - y : y - food_square.y);

            if (distance < best_distance)
            {
                target = i;
                best_distance = distance;
                found = true;
            }
        }
    }

    return found;
}

void Collector::drop_food(vector<unique_ptr<Food>> &foods)
{
    if (state == LOADED)
    {
        // In order to add the new food we have first to empty the grid
        Squarecell::remove_square(*this);

        unique_ptr<Food> food(new Food{x, y});
        foods.push_back(std::move(food));

        /** We have to add back the square as the zone will be free only after
         * all the Anthills are updated */
        Squarecell::add_square(*this);
    }
}

vector<Square> Collector::generate_moves(Square origin)
{
    // All the possible shifts combination: TOP-RIGHT,  BOTTOM-RIGHT...
    vector<int> x_shift{1, 1, -1, -1};
    vector<int> y_shift{1, 1, -1, -1};

    return Ant::generate_moves(origin, x_shift, y_shift);
}

unique_ptr<Collector> Collector::parse_line(string &line, unsigned int color_index)
{
    unsigned int x(0);
    unsigned int y(0);
    unsigned int age(0);
    string tmp("false");

    istringstream stream(line);
    stream >> x;
    stream >> y;
    stream >> age;
    stream >> tmp;

    auto state(State_collector::EMPTY);
    if (tmp == "true")
    {
        state = State_collector::LOADED;
    }
    return unique_ptr<Collector>(new Collector(x, y, age, state, color_index));
}
