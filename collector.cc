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

#include "constantes.h"
#include "message.h"
#include "squarecell.h"

#include "collector.h"

using std::istringstream;
using std::string;
using std::unique_ptr;
using std::vector;

unsigned int const g_max(128);

Collector::Collector(unsigned int x, unsigned int y, unsigned int age,
                     State_collector state, unsigned int color_index)
    : Ant{x, y, sizeC, age, color_index}, state(state)
{
    Squarecell::test_square(*this);
    add_to_grid();
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

bool Collector::step(Square anthill, vector<unique_ptr<Food>> &foods)
{
    if (!increase_age())
    {
        return false;
    }

    // TODO(@danielpanero): if false secondary goal (exit anthill, away from border)
    if (state == EMPTY)
    {
        search_food(foods);
    }
    else
    {
        return_to_anthill(anthill);
    }

    return true;
}

bool Collector::return_to_anthill(Square target)
{
    auto origin = get_as_square();

    remove_from_grid();
    undraw();

    auto move =
        Squarecell::lee_algorithm(origin, target, &Collector::generate_diagonal_moves,
                                  &Squarecell::test_if_border_touches);

    x = move.x;
    y = move.y;

    if (Squarecell::test_if_border_touches(*this, target))
    {
        // TODO(@danielpanero): increments food anthill
        state = EMPTY;
    }

    add_to_grid();
    draw();

    return true;
}

bool Collector::search_food(vector<unique_ptr<Food>> &foods)
{
    // TODO(@danielpanero) check that foods > 0 and return false when there is no food
    // to search

    size_t food_index = find_target_food(foods);

    auto origin = get_as_square();
    auto target = foods[food_index]->get_as_square();

    remove_from_grid();
    undraw();

    foods.at(food_index)->remove_from_grid();

    auto move =
        Squarecell::lee_algorithm(origin, target, &Collector::generate_diagonal_moves,
                                  &Squarecell::test_if_superposed_two_square);

    x = move.x;
    y = move.y;

    if (Squarecell::test_if_superposed_two_square(*this, target))
    {
        std::swap(foods.at(food_index), foods.back());
        foods.pop_back();

        state = LOADED;
    }
    else
    {
        foods.at(food_index)->add_to_grid();
    }

    add_to_grid();
    draw();

    return true;
}

size_t Collector::find_target_food(vector<unique_ptr<Food>> &foods)
{
    size_t target(0);
    unsigned int best_distance(g_max - 1);

    for (size_t i(0); i < foods.size(); i++)
    {
        Square food_square = foods[i]->get_as_square();

        // TODO(@danielpanero): check if it is the right implementation and what do
        // when it didn't find any..
        if ((food_square.x + food_square.y) % 2 == (x + y) % 2)
        {
            // TODO(@danielpanero): fix bug that sometimes it doesn't target the
            // nearest one
            unsigned int distance =
                std::max(food_square.x >= x ? food_square.x - x : x - food_square.x,
                         food_square.y >= y ? food_square.y - y : y - food_square.y);

            if (distance < best_distance)
            {
                target = i;
                best_distance = distance;
            }
        }
    }

    return target;
}

vector<Squarecell::Square> Collector::generate_diagonal_moves(Square origin)
{
    vector<Squarecell::Square> moves;

    for (int i(1); i <= 2; i++)
    {
        for (int j(1); j <= 2; j++)
        {
            Squarecell::Square move(origin);

            move.x += i == 1 ? 1 : -1;
            move.y += j == 1 ? 1 : -1;

            unsigned int x = Squarecell::get_coordinate_x(move);
            unsigned int y = Squarecell::get_coordinate_y(move);

            // We check if the proposed new positions are inside the model
            if (x >= 1 && y >= 1 && y <= g_max - 2 && x <= g_max - 2 &&
                x + move.side <= g_max - 1 && y + move.side <= g_max - 1)
            {
                moves.push_back(move);
            }
        }
    }

    return moves;
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
