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

    /**
     * @brief Checks that position in the grid is empty and either throw an error or
     * fills the grid
     *
     */
    virtual void add_to_grid() = 0;
    std::string get_as_string() override;

private:
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
    void draw() override;

    std::string get_as_string() override;
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
    void draw() override;

    std::string get_as_string() override;

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

    void add_to_grid() override;
    void draw() override;
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
    void draw() override;

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