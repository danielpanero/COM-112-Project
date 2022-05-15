/**
 * @file ants.h
 * @author Daniel Panero [+57/-7], Layane Wazen [+34/-0], Andrea Diez [+204/-110]
 * @version 0.1
 * @date 2022-04-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef ENTITIES_ANTS_H
#define ENTITIES_ANTS_H

#include <memory>

#include "constantes.h"
#include "element.h"
#include "food.h"
#include "squarecell.h"

/**
 * @brief Abstract class Ant (Base class for: Collector, Defensor, Predator)
 *
 */
class Ant : public Element
{
public:
    /**
     * @brief Construct a new Ant instance
     *
     * @param x position of generator in the x-axis
     * @param y position of generator in the y-axis
     * @param side size of element
     * @param age
     * @param color_index (0 red, 1 green, 2 blue, 3 yellow, 4 magenta, 5 cyan) same as
     * graphic.h
     */
    Ant(unsigned int x, unsigned int y, unsigned int side, unsigned int age,
        unsigned int color_index);
    ~Ant() override = default;

    // TODO(@danielpanero): attacked as virtual

    /**
     * @brief Checks that position in the grid is empty and either throw an error or
     * fills the grid
     *
     */
    virtual void add_to_grid() = 0;
    virtual void remove_from_grid() = 0;

    std::string get_as_string() override;

    /**
     * @brief Increases age by one
     * 
     * @return false if age >= bug life and therefore the Ant should be dead
     */
    bool increase_age();

protected:
    unsigned int age;
};

class Generator : public Ant
{
public:
    /**
     * @brief Constructs a new Generator instance
     *
     * @param x position of generator in the x-axis
     * @param y position of generator in the y-axis
     * @param age
     * @param color_index (0 red, 1 green, 2 blue, 3 yellow, 4 magenta, 5 cyan) same as
     * graphic.h
     */
    Generator(unsigned int x, unsigned int y, unsigned int age,
              unsigned int color_index);

    void add_to_grid() override;
    void remove_from_grid() override;

    void draw() override;
    void undraw() override;

    std::string get_as_string() override;

    bool step();
};

class Collector : public Ant
{
public:
    /**
     * @brief Constructs a new Collector instance
     *
     * @param x position of collector in the x-axis
     * @param y position of collector in the y-axis
     * @param age
     * @param state state of collector: EMPTY / LOADED
     * @param color_index (0 red, 1 green, 2 blue, 3 yellow, 4 magenta, 5 cyan) same as
     * graphic.h
     */
    Collector(unsigned int x, unsigned int y, unsigned int age, State_collector state,
              unsigned int color_index);

    void add_to_grid() override;
    void remove_from_grid() override;

    void draw() override;
    void undraw() override;

    std::string get_as_string() override;

    bool step(Square anthill, std::vector<std::unique_ptr<Food>> &foods);

    bool return_to_anthill(Square target);

    /**
     * @brief Moves the collector the nearest foods attainable, if there is no food
     * return false
     *
     * @param foods
     */
    bool search_food(std::vector<std::unique_ptr<Food>> &foods);

    /**
     * @brief Finds the nearest attainable food and return the index to it
     *
     * @param foods
     * @return size_t
     */
    size_t find_target_food(std::vector<std::unique_ptr<Food>> &foods);

    /**
     * @brief Generates all the possible new positions / moves based on the origin
     *
     * @param origin
     * @return std::vector<Square>
     */
    static std::vector<Square> generate_diagonal_moves(Squarecell::Square origin);

    /**
     * @brief Creates a new pointed instance of Collector from its string
     * representation
     *
     * @param line
     * @param color_index (0 red, 1 green, 2 blue, 3 yellow, 4 magenta, 5 cyan) same as
     * graphic.h
     * @return std::unique_ptr<Collector>
     */
    static std::unique_ptr<Collector> parse_line(std::string &line,
                                                 unsigned int color_index);

private:
    State_collector state;
};

class Defensor : public Ant
{
public:
    /**
     * @brief Constructs a new Defensor instance
     *
     * @param x position of defensor in the x-axis
     * @param y position of defensor in the y-axis
     * @param age
     * @param color_index (0 red, 1 green, 2 blue, 3 yellow, 4 magenta, 5 cyan) same as
     * graphic.h
     */
    Defensor(unsigned int x, unsigned int y, unsigned int age,
             unsigned int color_index);

    void add_to_grid() override;
    void remove_from_grid() override;

    void draw() override;
    void undraw() override;

    bool step(Squarecell::Square anthill);

    static bool test_if_confined_and_near_border(Squarecell::Square &origin,
                                                 Squarecell::Square &anthill);

    /**
     * @brief Generates all the possible new positions / moves based on the origin
     *
     * @param origin
     * @return std::vector<Square>
     */
    static std::vector<Square> generate_hv_moves(Squarecell::Square origin);

    /**
     * @brief Creates a new pointed instance of Defensor from its string representation
     *
     * @param line
     * @param color_index (0 red, 1 green, 2 blue, 3 yellow, 4 magenta, 5 cyan) same as
     * graphic.h
     * @return std::unique_ptr<Defensor>
     */
    static std::unique_ptr<Defensor> parse_line(std::string &line,
                                                unsigned int color_index);
};

class Predator : public Ant
{
public:
    /**
     * @brief Constructs a new Predator instance
     *
     * @param x position of predator in the x-axis
     * @param y position of predator in the y-axis
     * @param age
     * @param color_index (0 red, 1 green, 2 blue, 3 yellow, 4 magenta, 5 cyan) same as
     * graphic.h
     */
    Predator(unsigned int x, unsigned int y, unsigned int age,
             unsigned int color_index);

    void add_to_grid() override;
    void remove_from_grid() override;

    void draw() override;
    void undraw() override;

    bool step();

    void remain_inside(Squarecell::Square anthill);

    // TODO(@danielpanero): multiple ants near + possible segfault maybe pass directly
    // anthill with a function near ants? Predator vs Defensor -> nothing and Defensor
    // + Defensor ... define function attacked for each ants...
    void attack(std::unique_ptr<Ant> &ant);

    /**
     * @brief Generates all the possible new positions / moves based on the origin
     *
     * @param origin
     * @return std::vector<Square>
     */
    static std::vector<Square> generate_l_moves(Squarecell::Square origin);

    /**
     * @brief Creates a new pointed instance of Predator from its string representation
     *
     * @param line
     * @param color_index
     * @return std::unique_ptr<Predator>
     */
    static std::unique_ptr<Predator> parse_line(std::string &line,
                                                unsigned int color_index);
};

#endif