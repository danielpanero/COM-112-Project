#include "iostream"
#include "fstream"
#include "sstream"
#include "algorithm"
#include "vector"

#include "simulation/food.h"
#include "simulation/anthill.h"
#include "simulation/ants.h"
#include "shared/message.h"

#include "simulation.h"

using namespace std;

void get_next_line(ifstream &stream, istringstream &linestream);

void Simulation::readFile(string path)
{

    istringstream line;
    ifstream file(path);
    if (file.fail())
    {
        exit(EXIT_FAILURE);
    }

    // 1. Parsing the foods
    get_next_line(file, line);
    line >> nbN;
    vector<Food *> foods(nbN);

    unsigned int i = 0;
    while (i < nbN)
    {
        get_next_line(file, line);
        foods[i] = new Food(line);
        i++;
    }

    // 2. Parsing the anthills
    get_next_line(file, line);
    line >> nbF;
    vector<Anthill *> anthills(nbF);

    i = 0;
    while (i < nbF)
    {
        get_next_line(file, line);
        anthills[i] = new Anthill(line);

        // 3. Parsing the ants
        // 3.1 Parsing the collectors
        unsigned int nbC = anthills[i]->get_number_of_collectors();
        vector<Collector *> collectors(nbC);

        unsigned int j = 0;
        while (j < nbC)
        {
            get_next_line(file, line);
            collectors[j] = new Collector(line);
            j++;
        }

        // 3.2 Parsing the defendors
        unsigned int nbD = anthills[i]->get_number_of_defensors();
        vector<Defensor *> defensors(nbD);

        j = 0;
        while (j < nbD)
        {
            get_next_line(file, line);
            defensors[j] = new Defensor(line);
            j++;
        }

        // 3.3 Parsing the predators
        unsigned int nbP = anthills[i]->get_number_of_predators();
        vector<Predator *> predators(nbP);

        j = 0;
        while (j < nbP)
        {
            get_next_line(file, line);
            predators[i] = new Predator(line);
            j++;
        }

        i++;
    }

    // 4. Testing that the anthils don't overlap
    auto predicate = [](Anthill *a1, Anthill *a2) -> bool
    {
        Square square1(a1->get_as_square());
        Square square2(a2->get_as_square());
        return test_if_superposed_two_square(square1, square2);
    };

    auto it = search(anthills.begin(), anthills.end(), anthills.begin(), anthills.end(), predicate);
    if (it != anthills.end())
    {
        cout << message::homes_overlap(5, 5);
        exit(EXIT_FAILURE);
    }

    file.close();
}

void get_next_line(ifstream &stream, istringstream &linestream)
{
    string line;
    while (getline(stream >> ws, line))
    {
        if (line[0] == '#')
        {
            continue;
        }
        linestream.clear();
        linestream.str(line);
        return;
    }
}