/**
 * @file ants.h
 * @author Daniel Panero [+57/-7], Layane Wazen [+34/-0], Andrea Diez [+22/-14]
 * @version 0.1
 * @date 2022-04-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef ENTITIES_ANTS_H
#define ENTITIES_ANTS_H

#include "memory"

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
     */
    Ant(unsigned int &x, unsigned int &y, unsigned int side, unsigned int &age);

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
     */
    Generator(unsigned int &x, unsigned int &y, unsigned int &age);

    void add_to_grid() override;
    std::string get_as_string() override;
    void draw(unsigned int &color_index) override;
    
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
     */
    Collector(unsigned int &x, unsigned int &y, unsigned int &age,
              StateCollector &state);

    std::string get_as_string() override;
    void add_to_grid() override;
    void draw(unsigned int &color_index) override;

    /**
     * @brief Creates a new pointed instance of Collector from its string
     * representation
     *
     * @param line
     * @return std::unique_ptr<Collector>
     */
    static std::unique_ptr<Collector> parse_line(std::string &line);

private:
    StateCollector state;
};

class Defensor : public Ant
{
public:
    /**
     * @brief Constructs a new Defensor instance
     *
     * @param x position of defensor in the x-axis
     * @param y position of defensor in the y-axis
     */
    Defensor(unsigned int &x, unsigned int &y, unsigned int &age);

    /**
     * @brief Creates a new pointed instance of Defensor from its string representation
     *
     * @param line
     * @return std::unique_ptr<Defensor>
     */
    static std::unique_ptr<Defensor> parse_line(std::string &line);

    void add_to_grid() override;
    void draw(unsigned int &color_index) override;
};

class Predator : public Ant
{
public:
    /**
     * @brief Constructs a new Predator instance
     *
     * @param x position of predator in the x-axis
     * @param y position of predator in the y-axis
     */
    Predator(unsigned int &x, unsigned int &y, unsigned int &age);

    void add_to_grid() override;

    /**
     * @brief Creates a new pointed instance of Predator from its string representation
     *
     * @param line
     * @return std::unique_ptr<Predator>
     */
    static std::unique_ptr<Predator> parse_line(std::string &line);
    void draw(unsigned int &color_index) override;
};

#endif