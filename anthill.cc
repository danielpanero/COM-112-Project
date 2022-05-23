/**
 * @file anthill.cc
 * @author Daniel Panero [+90/-14], Layane Wazen [+10/-0], Andrea Diez [+193/-135]
 * @version 0.1
 * @date 2022-04-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <algorithm>
#include <cmath>
#include <iostream>
#include <memory>
#include <random>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "element.h"
#include "message.h"
#include "squarecell.h"

#include "anthill.h"

using std::bind;
using std::function;
using std::istringstream;
using std::move;
using std::remove;
using std::string;
using std::unique_ptr;
using std::vector;

using Squarecell::Square;

// ====================================================================================
// Initialization - Misc

Anthill::Anthill(unsigned int x, unsigned int y, unsigned int side, unsigned int xg,
                 unsigned int yg, double n_food, unsigned int n_collectors,
                 unsigned int n_defensors, unsigned int n_predators,
                 unsigned int color_index)
    : Element{x, y, side, false, color_index}, n_food(n_food),
      generator(new Generator(xg, yg, 0, color_index))
{
    Squarecell::test_square(*this);

    // We preallocate the vectors, so when get_number_of... is called it return the
    // right size
    collectors.resize(n_collectors);
    defensors.resize(n_defensors);
    predators.resize(n_predators);
}

Anthill::~Anthill() { undraw(); }

void Anthill::test_if_generator_defensors_perimeter(unsigned int index)
{
    auto generator_square = generator->get_as_square();
    if (!Squarecell::test_if_completely_confined(generator_square, *this))
    {
        throw std::invalid_argument(message::generator_not_within_home(
            generator_square.x, generator_square.y, index));
    }

    for (auto &defensor : defensors)
    {
        auto defensor_square = defensor->get_as_square();
        if (!Squarecell::test_if_completely_confined(defensor_square, *this))
        {
            throw std::invalid_argument(message::defensor_not_within_home(
                defensor_square.x, defensor_square.y, index));
        }
    }
}

void Anthill::set_collectors(vector<unique_ptr<Collector>> &collectors)
{
    this->collectors = move(collectors);
}
void Anthill::set_defensors(vector<unique_ptr<Defensor>> &defensors)
{
    this->defensors = move(defensors);
}
void Anthill::set_predators(vector<unique_ptr<Predator>> &predators)
{
    this->predators = move(predators);
}

unsigned int Anthill::get_number_of_collectors() const { return collectors.size(); };
unsigned int Anthill::get_number_of_defensors() const { return defensors.size(); };
unsigned int Anthill::get_number_of_predators() const { return predators.size(); };
double Anthill::get_number_of_food() const { return n_food; }

void Anthill::draw()
{
    Squarecell::draw_only_border(*this, get_color_index());
    generator->draw();
}

void Anthill::undraw() { Squarecell::undraw_thick_border_square(*this); }

string Anthill::get_as_string()
{
    using std::to_string;

    string tmp = "\n" + to_string(x) + " " + to_string(y) + " " + to_string(side) +
                 " " + generator->get_as_string() + " " + to_string(n_food) + " " +
                 to_string(get_number_of_collectors()) + " " +
                 to_string(get_number_of_defensors()) + " " +
                 to_string(get_number_of_predators()) + "\n";

    for (auto const &collector : collectors)
    {
        tmp += collector->get_as_string() + "\n";
    }

    for (auto const &defensor : defensors)
    {
        tmp += defensor->get_as_string() + "\n";
    }

    for (auto const &predator : predators)
    {
        tmp += predator->get_as_string() + "\n";
    }

    return tmp;
}

// ====================================================================================
// Simulation

bool Anthill::step(vector<unique_ptr<Food>> &foods,
                   vector<unique_ptr<Anthill>> &anthills)
{
    undraw();

    try_to_expand(anthills);

    if (!(generator->step(*this) && reduce_food()))
    {
        for (auto &collector : collectors)
        {
            collector->drop_food(foods);
        }

        return false;
    }

    generate_new_ants();

    update_collectors(foods);
    update_defensors(anthills);
    update_predators(anthills);

    draw();

    return true;
}

void Anthill::update_collectors(vector<unique_ptr<Food>> &foods)
{
    for (auto &collector : collectors)
    {
        if (!collector->step())
        {
            collector->drop_food(foods);

            dead_ants.push_back(move(collector));
            continue;
        }

        if (collector->get_state() == EMPTY)
        {
            size_t target = 0;
            if (collector->find_target_food(foods, target))
            {
                if (collector->search_food(foods.at(target)))
                {
                    std::swap(foods.at(target), foods.back());
                    foods.pop_back();
                };
            }
            else
            {
                collector->go_outside(*this);
            }
        }
        else
        {
            if (collector->return_to_anthill(*this))
            {
                n_food += val_food;
            }
        }
    }

    collectors.erase(remove(collectors.begin(), collectors.end(), nullptr),
                     collectors.end());
}

void Anthill::update_defensors(vector<unique_ptr<Anthill>> &anthills)
{
    for (auto &defensor : defensors)
    {
        if (!defensor->step(*this))
        {
            dead_ants.push_back(move(defensor));
            continue;
        }

        for (auto const &anthill : anthills)
        {
            if (anthill && anthill.get() != this)
            {
                auto test = bind(&Defensor::test_if_contact_collector,
                                 std::ref(*defensor), std::placeholders::_1);

                anthill->mark_collectors_as_dead(test);
            }
        }
    }

    defensors.erase(remove(defensors.begin(), defensors.end(), nullptr),
                    defensors.end());
}

void Anthill::update_predators(vector<unique_ptr<Anthill>> &anthills)
{
    for (auto &predator : predators)
    {
        if (!predator->step())
        {
            dead_ants.push_back(move(predator));
            continue;
        }

        vector<Square> targets;

        auto anthill_square = get_as_square();
        auto predator_square = predator->get_as_square();

        auto filter =
            bind(&Predator::filter_ants, state, anthill_square, std::placeholders::_1);
        auto test = bind(&Squarecell::test_if_border_touches, std::placeholders::_1,
                         predator_square);

        bool dead = false;
        for (auto const &anthill : anthills)
        {
            if (anthill && anthill.get() != this)
            {
                anthill->get_attackable_ants(filter, targets);

                anthill->mark_collectors_as_dead(test);
                if (anthill->mark_predators_as_dead(test))
                {
                    dead_ants.push_back(move(predator));
                    dead = true;
                    break;
                }
            }
        }

        if (dead)
        {
            continue;
        }

        if (targets.empty())
        {
            predator->remain_inside(*this);
            continue;
        }

        predator->move_toward_nearest_ant(targets);
    }

    predators.erase(remove(predators.begin(), predators.end(), nullptr),
                    predators.end());
}

bool Anthill::get_attackable_ants(const function<bool(Square &)> &test,
                                  vector<Square> &targets)
{
    bool found = false;

    for (auto const &collector : collectors)
    {
        if (test(*collector))
        {
            targets.push_back(*collector);
        }
    }

    for (auto const &predator : predators)
    {
        if (test(*predator))
        {
            targets.push_back(*predator);
        }
    }

    return found;
}

bool Anthill::mark_collectors_as_dead(const function<bool(Square &)> &test)
{
    bool found = false;

    for (auto &collector : collectors)
    {
        if (collector && test(*collector))
        {
            dead_ants.push_back(move(collector));
            found = true;
        }
    }

    collectors.erase(remove(collectors.begin(), collectors.end(), nullptr),
                     collectors.end());

    return found;
}

bool Anthill::mark_predators_as_dead(const function<bool(Square &)> &test)
{
    bool found = false;

    for (auto &predator : predators)
    {
        if (predator && test(*predator))
        {
            dead_ants.push_back(move(predator));
            found = true;
        }
    }

    predators.erase(remove(predators.begin(), predators.end(), nullptr),
                    predators.end());

    return found;
}

void Anthill::clear_dead_ants()
{
    undraw();

    dead_ants.clear();

    draw();
}

unique_ptr<Anthill> Anthill::parse_line(string &line, unsigned int color_index)
{
    unsigned int x(0);
    unsigned int y(0);
    unsigned int side(0);
    unsigned int xg(0);
    unsigned int yg(0);

    double n_food(0);
    unsigned int n_collectors(0);
    unsigned int n_defensors(0);
    unsigned int n_predators(0);

    istringstream stream(line);

    stream >> x;
    stream >> y;
    stream >> side;
    stream >> xg;
    stream >> yg;
    stream >> n_food;
    stream >> n_collectors;
    stream >> n_defensors;
    stream >> n_predators;

    return unique_ptr<Anthill>(new Anthill(x, y, side, xg, yg, n_food, n_collectors,
                                           n_defensors, n_predators, color_index));
}

void Anthill::try_to_expand(vector<unique_ptr<Anthill>> &anthills)
{
    vector<int> xshift{-1, -1, 0, 1};
    vector<int> yshift{-1, 0, 0, 0};

    Square successfull_square{};
    bool successfull = false;

    for (size_t i = 0; i <= 3 && !successfull; i++)
    {
        auto origin = get_as_square();

        origin.side = calculate_side();

        origin.x += xshift.at(i) *
                    (origin.side > side ? origin.side - side : side - origin.side);
        origin.y += yshift.at(i) *
                    (origin.side > side ? origin.side - side : side - origin.side);

        if (Squarecell::test_square_without_message(origin))
        {
            if (anthills.size() == 1)
            {
                successfull = true;
                successfull_square = origin;
                break;
            }
            for (auto const &anthill : anthills)
            {
                if (anthill && anthill.get() != this &&
                    !Squarecell::test_if_superposed_two_square(origin, *anthill))
                {
                    successfull = true;
                    successfull_square = origin;
                    break;
                }

                successfull = false;
                break;
            }
        }
    }

    if (successfull)
    {
        state = FREE;

        x = successfull_square.x;
        y = successfull_square.y;
        side = successfull_square.side;
    }
    else
    {
        state = CONSTRAINED;
    }
}

void Anthill::generate_new_ants()
{
    static std::bernoulli_distribution b_distribution(
        std::min(1.0, n_food * birth_rate));
    static std::default_random_engine random_num;

    if (!b_distribution(random_num))
    {
        return;
    }

    double n_ants = get_number_of_collectors() + get_number_of_defensors() +
                    get_number_of_predators();
    double current_prop_collectors = 0;
    double current_prop_defensors = 0;

    if (n_ants != 0)
    {
        current_prop_collectors = get_number_of_collectors() / n_ants;
        current_prop_defensors = get_number_of_defensors() / n_ants;
    }

    Square position{};

    if ((state == FREE && current_prop_collectors < prop_free_collector) ||
        (state == CONSTRAINED && current_prop_collectors < prop_constrained_collector))
    {
        if (find_suitable_position_for_ant(sizeC, position))
        {
            unique_ptr<Collector> collector(
                new Collector{position.x, position.y, 0, EMPTY, get_color_index()});
            collector->draw();

            collectors.push_back(move(collector));
        }
    }
    else if ((state == FREE && current_prop_defensors < prop_free_defensor) ||
             (state == CONSTRAINED &&
              current_prop_defensors < prop_constrained_defensor))
    {
        if (find_suitable_position_for_ant(sizeD, position))
        {
            unique_ptr<Defensor> defensor(
                new Defensor{position.x, position.y, 0, get_color_index()});
            defensor->draw();

            defensors.push_back(move(defensor));
        }
    }
    else
    {
        if (find_suitable_position_for_ant(sizeP, position))
        {
            unique_ptr<Predator> predator(
                new Predator{position.x, position.y, 0, get_color_index()});
            predator->draw();

            predators.push_back(move(predator));
        }
    }
}

unsigned int Anthill::calculate_side()
{
    return sqrt(4 * (sizeG * sizeG + sizeC * sizeC * get_number_of_collectors() +
                     sizeD * sizeD * get_number_of_defensors() +
                     sizeP * sizeP * get_number_of_predators())) +
           2;
}

bool Anthill::reduce_food()
{
    n_food -= food_rate * (1 + get_number_of_collectors() + get_number_of_defensors() +
                           get_number_of_predators());

    return n_food > 0;
}

bool Anthill::find_suitable_position_for_ant(unsigned int side_ant, Square &position)
{
    for (unsigned int x_shift = (side_ant - 1) / 2 + 1;
         x_shift < side - (side_ant - 1) / 2 - 1; x_shift++)
    {
        for (unsigned int y_shift = (side_ant - 1) / 2 + 1;
             y_shift < side - (side_ant - 1) / 2 - 1; y_shift++)
        {
            Square square{.x = x + x_shift,
                          .y = y + y_shift,
                          .side = side_ant,
                          .centered = true};

            if (Squarecell::test_square_without_message(square) &&
                !Squarecell::test_if_superposed_grid(square))
            {
                position = square;
                return true;
            }
        }
    }

    return false;
}