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
    virtual std::string export_to_line() = 0;

protected:
<<<<<<< HEAD
    Square Ant get_as_square();
=======
    Square get_as_square();
>>>>>>> 89f499b60e79cf59c2e4ccda38e27c4a0cd1ac51

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
        // FIXME(@andreadiez): implement export_line
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
    
    static std::unique_ptr<Collector> parse_line(std::string &line);

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
    Predator(unsigned int &x, unsigned int &y);

    // static Predator *parse_line(std::string &line);

    void add_to_grid();
};

#endif