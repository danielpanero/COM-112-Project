#include "iostream"
#include "sstream"
#include "vector"

#include "message.h"
#include "squarecell.h"

#include "anthill.h"

using std::cout;
using std::istringstream;
using std::string;
using std::vector;

Anthill::Anthill(unsigned int &x, unsigned int &y, unsigned int &side, bool &centered)
    : Square({x, y, side, false})
{
    test_square(*this);
}

Anthill *Anthill::parse_line(std::string &line)
{
    unsigned int x;
    unsigned int y;
    unsigned int side_;
    bool centered;

    istringstream stream(line);
    stream >> x;
    stream >> y;

    side = side_;
    centered = false;

    unsigned int xg(0);
    unsigned int yg(0);
    stream >> xg;
    stream >> yg;
    new Generator(xg, yg);

    unsigned int total_food;
    unsigned int nbC;
    unsigned int nbD;
    unsigned int nbP;
    stream >> total_food;
    stream >> nbC;
    stream >> nbD;
    stream >> nbP;

    return new Anthill(x, y, side_, centered);
}

void Anthill::test_if_generator_defensors_perimeter(unsigned int index)
{
    Square generator_square = generator->get_as_square();
    if (!test_if_completely_confined(generator_square, *this))
    {
        cout << message::generator_not_within_home(generator_square.x,
                                                   generator_square.y, index);
        exit(EXIT_FAILURE);
    }

    for (auto *defensor : defensors)
    {
        Square defensor_square = defensor->get_as_square();
        if (!test_if_completely_confined(defensor_square, *this))
        {
            cout << message::defensor_not_within_home(defensor_square.x,
                                                      defensor_square.y, index);
            exit(EXIT_FAILURE);
        }
    }
}

void Anthill::set_collectors(vector<Collector *> &collectors)
{
    this->collectors = collectors;
}
void Anthill::set_defensors(vector<Defensor *> &defensors)
{
    this->defensors = defensors;
}
void Anthill::set_predators(vector<Predator *> &predators)
{
    this->predators = predators;
}

unsigned int Anthill::get_number_of_collectors() const { return nbC; };
unsigned int Anthill::get_number_of_defensors() const { return nbD; };
unsigned int Anthill::get_number_of_predators() const { return nbP; };

Square Anthill::get_as_square() { return Square(*this); }