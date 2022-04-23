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
#include "squarecell.h"

class Ant : protected Square
{
public:
    Ant(unsigned int &x, unsigned int &y, unsigned int &side, unsigned int &age);

    virtual void add_to_grid() = 0;
    virtual std::string get_as_string() = 0;

protected:
    Square get_as_square();

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
    Generator(unsigned int &x, unsigned int &y, unsigned int &age);

    void add_to_grid() override;
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
     */
    Collector(unsigned int &x, unsigned int &y, unsigned int &age,
              enum &Etat_collector);

    static std::unique_ptr<Collector> parse_line(std::string &line);

    std::string get_as_string() override;

    void add_to_grid() override;

private:
    enum Etat_collector;
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

    static std::unique_ptr<Defensor> parse_line(std::string &line);

    std::string get_as_string() override;

    void add_to_grid() override;
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

    static std::unique_ptr<Predator> parse_line(std::string &line);

    std::string get_as_string() override;

    void add_to_grid() override;
};

#endif