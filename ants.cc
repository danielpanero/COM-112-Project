/**
 * @file ants.cc
 * @author Daniel Panero [+135/-10], Layane Wazen [+10/-0], Andrea Diez [+367/-204]
 * @version 0.1
 * @date 2022-04-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <cmath>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>

#include "constantes.h"
#include "element.h"
#include "message.h"
#include "squarecell.h"

#include "ants.h"

using std::istringstream;
using std::string;
using std::unique_ptr;
using std::vector;

unsigned int const g_max(128);

// ====================================================================================
// Ant
Ant::Ant(unsigned int x, unsigned int y, unsigned int side, unsigned int age,
         unsigned int color_index)
    : Element{x, y, side, true, color_index}, age(age)
{
}

string Ant::get_as_string()
{
    using std::to_string;
    return to_string(x) + " " + to_string(y) + " " + to_string(age);
}

// ====================================================================================
// Generator
Generator::Generator(unsigned int x, unsigned int y, unsigned int age,
                     unsigned int color_index)
    : Ant{x, y, sizeG, age, color_index}
{
    Squarecell::test_square(*this);
    add_to_grid();
}

void Generator::add_to_grid()
{
    unsigned int superposed_x(0);
    unsigned int superposed_y(0);

    if (Squarecell::test_if_superposed_grid(*this, superposed_x, superposed_y))
    {
        throw std::invalid_argument(
            message::generator_overlap(x, y, superposed_x, superposed_y));
    }

    Squarecell::add_square(*this);
}

void Generator::remove_from_grid() { Squarecell::remove_square(*this); }

void Generator::draw() { Squarecell::draw_filled(*this, get_color_index()); }

void Generator::undraw() { Squarecell::undraw_square(*this); }

string Generator::get_as_string()
{
    return std::to_string(x) + " " + std::to_string(y);
}

// ====================================================================================
// Collector

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

void Collector::step(Square anthill, vector<unique_ptr<Food>> &foods)
{
    // TODO(@danielpanero): if false secondary goal (exit anthill, away from border)
    if (state == EMPTY)
    {
        search_food(foods);
    }
    else
    {
        return_to_anthill(anthill);
    }
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

// ====================================================================================
// Defensor

Defensor::Defensor(unsigned int x, unsigned int y, unsigned int age,
                   unsigned int color_index)
    : Ant{x, y, sizeD, age, color_index}
{
    Squarecell::test_square(*this);
    add_to_grid();
}

void Defensor::add_to_grid()
{
    unsigned int superposed_x(0);
    unsigned int superposed_y(0);

    if (Squarecell::test_if_superposed_grid(*this, superposed_x, superposed_y))
    {
        throw std::invalid_argument(
            message::defensor_overlap(x, y, superposed_x, superposed_y));
    }

    Squarecell::add_square(*this);
}

void Defensor::remove_from_grid() { Squarecell::remove_square(*this); }

void Defensor::draw() { Squarecell::draw_plus_pattern(*this, get_color_index()); }

void Defensor::undraw() { Squarecell::undraw_square(*this); }

void Defensor::step(Square anthill)
{
    auto origin = get_as_square();

    remove_from_grid();
    undraw();

    auto move =
        Squarecell::lee_algorithm(origin, anthill, &Defensor::generate_hv_moves,
                                  &Defensor::test_if_confined_and_near_border);

    x = move.x;
    y = move.y;

    add_to_grid();
    draw();
}

bool Defensor::test_if_confined_and_near_border(Square &origin, Square &anthill)
{
    return Squarecell::test_if_completely_confined(origin, anthill) &&
           Squarecell::test_if_border_touches(origin, anthill);
}

vector<Squarecell::Square> Defensor::generate_hv_moves(Square origin)
{
    vector<Squarecell::Square> moves;

    for (int i(1); i <= 2; i++)
    {
        for (int j(1); j <= 2; j++)
        {
            Squarecell::Square move(origin);

            if (i == 1)
            {
                move.x += j == 1 ? 1 : -1;
            }
            else
            {

                move.y += j == 1 ? 1 : -1;
            }

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

unique_ptr<Defensor> Defensor::parse_line(string &line, unsigned int color_index)
{
    unsigned int x(0);
    unsigned int y(0);
    unsigned int age(0);

    istringstream stream(line);
    stream >> x;
    stream >> y;
    stream >> age;

    return unique_ptr<Defensor>(new Defensor(x, y, age, color_index));
}

// ====================================================================================
// Predator

Predator::Predator(unsigned int x, unsigned int y, unsigned int age,
                   unsigned int color_index)
    : Ant{x, y, sizeP, age, color_index}
{
    Squarecell::test_square(*this);
    add_to_grid();
}

void Predator::add_to_grid()
{
    if (Squarecell::test_if_superposed_grid(*this))
    {
        throw std::invalid_argument(message::predator_overlap(x, y));
    }

    Squarecell::add_square(*this);
}

void Predator::remove_from_grid() { Squarecell::remove_square(*this); }

void Predator::draw() { Squarecell::draw_filled(*this, get_color_index()); }

void Predator::undraw() { Squarecell::undraw_square(*this); }

void Predator::remain_inside(Squarecell::Square anthill)
{
    auto origin = get_as_square();

    remove_from_grid();
    undraw();

    auto move = Squarecell::lee_algorithm(origin, anthill, &Predator::generate_l_moves,
                                          &Squarecell::test_if_completely_confined);

    x = move.x;
    y = move.y;

    add_to_grid();
    draw();
}

void Predator::attack(unique_ptr<Ant> &ant) {}

vector<Squarecell::Square> Predator::generate_l_moves(Square origin)
{
    vector<Squarecell::Square> moves;

    // x: +-3, y: +- 1

    for (int i(1); i <= 2; i++)
    {
        for (int j(1); j <= 2; j++)
        {
            Squarecell::Square move(origin);

            move.x += j == 1 ? 3 : -3;
            move.y += i == 1 ? 1 : -1;

            unsigned int x = Squarecell::get_coordinate_x(move);
            unsigned int y = Squarecell::get_coordinate_y(move);

            // We check if the proposed new positions are inside the model
            if (x >= 0 && y >= 0 && y <= g_max - 1 && x <= g_max - 1 &&
                x + move.side <= g_max && y + move.side <= g_max)
            {
                moves.push_back(move);
            }
        }
    }

    // x: +-1, y: +-2

    for (int i(1); i <= 2; i++)
    {
        for (int j(1); j <= 2; j++)
        {
            Squarecell::Square move(origin);

            move.x += j == 1 ? 1 : -1;
            move.y += i == 1 ? 3 : -3;

            unsigned int x = Squarecell::get_coordinate_x(move);
            unsigned int y = Squarecell::get_coordinate_y(move);

            // We check if the proposed new positions are inside the model
            if (x >= 0 && y >= 0 && y <= g_max - 1 && x <= g_max - 1 &&
                x + move.side <= g_max && y + move.side <= g_max)
            {
                moves.push_back(move);
            }
        }
    }

    return moves;
}

unique_ptr<Predator> Predator::parse_line(string &line, unsigned int color_index)
{
    unsigned int x(0);
    unsigned int y(0);
    unsigned int age(0);

    istringstream stream(line);
    stream >> x;
    stream >> y;
    stream >> age;

    return unique_ptr<Predator>(new Predator(x, y, age, color_index));
}
