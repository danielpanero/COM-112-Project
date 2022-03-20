#include "sstream"
#include "vector"
#include "iostream"

#include "../squarecell.h"
#include "../shared/message.h"

#include "anthill.h"

using namespace std;

Anthill::Anthill(string &line)
{
    istringstream stream(line);

    stream >> x;
    stream >> y;
    stream >> side;
    centered = false;
    test_square(*this);

    unsigned int xg;
    unsigned int yg;
    stream >> xg;
    stream >> yg;
    generator = new Generator(xg, yg);

    stream >> total_food;
    stream >> nbC;
    stream >> nbD;
    stream >> nbP;
}

void Anthill::test_if_generator_defensors_perimeter(unsigned int index)
{
    Square generator_square = generator->get_as_square();
    if (!test_if_completely_confined(generator_square, *this))
    {
        cout << message::generator_not_within_home(generator_square.x, generator_square.y, index);
        exit(EXIT_FAILURE);
    }

    for (auto defensor : defensors)
    {
        Square defensor_square = defensor->get_as_square();
        if (!test_if_completely_confined(defensor_square, *this))
        {
            cout << message::defensor_not_within_home(defensor_square.x, defensor_square.y, index);
            exit(EXIT_FAILURE);
        }
    }
}

void Anthill::set_collectors(vector<Collector *> &collectors) { this->collectors = collectors; }
void Anthill::set_defensors(vector<Defensor *> &defensors) { this->defensors = defensors; }
void Anthill::set_predators(vector<Predator *> &predators) { this->predators = predators; }

int Anthill::get_number_of_collectors() { return nbC; };
int Anthill::get_number_of_defensors() { return nbD; };
int Anthill::get_number_of_predators() { return nbP; };

Square Anthill::get_as_square() { return Square(*this); }