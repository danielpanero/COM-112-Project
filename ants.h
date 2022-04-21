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

#include "constantes.h"

#include "squarecell.h"

class Ant : protected Square
{
public:
    Ant(unsigned int &x, unsigned int &y, unsigned int &age);
    virtual Ant *parse_line(std::string &line);
    virtual Square Ant::add_to_grid() = 0;

protected:
    Square Ant::get_as_square();

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
     */
    Generator(unsigned int &x, unsigned int &y);

    void add_to_grid();

    Square get_as_square();
};

class Collector : public Ant
{
public:
    /**
     * @brief Constructs a new Collector instance
     *
     * @param x position of collector in the x-axis
     * @param y position of collector in the y-axis
     */
    Collector(unsigned int &x, unsigned int &y);

    static Collector *parse_line(std::string &line);

    void add_to_grid();
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
    Defensor(unsigned int &x, unsigned int &y);

   // static Defensor *parse_line(std::string &line);

    void add_to_grid();

    Square get_as_square();
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
    Predator(unsigned int &x, unsigned int &y);

    //static Predator *parse_line(std::string &line);

    void add_to_grid();
};

#endif