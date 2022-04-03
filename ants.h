/**
 * @file ants.h
 * @author Daniel Panero [+57/-7], Layane Wazen [+34/-0], Andrea Diez Leboffe
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

class Generator : protected Square
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

class Collector : protected Square
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

class Defensor : Square
{
public:
    /**
     * @brief Constructs a new Defensor instance
     *
     * @param x position of defensor in the x-axis
     * @param y position of defensor in the y-axis
     */
    Defensor(unsigned int &x, unsigned int &y);

    static Defensor *parse_line(std::string &line);

    void add_to_grid();

    Square get_as_square();
};

class Predator : protected Square
{
public:
    /**
     * @brief Constructs a new Predator instance
     *
     * @param x position of predator in the x-axis
     * @param y position of predator in the y-axis
     */
    Predator(unsigned int &x, unsigned int &y);

    static Predator *parse_line(std::string &line);

    void add_to_grid();
};

#endif