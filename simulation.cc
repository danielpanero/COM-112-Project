#include "iostream"
#include "fstream"
#include "sstream"
#include "algorithm"
#include "vector"

#include "food.h"
#include "anthill.h"
#include "message.h"
#include "ants.h"

#include "simulation.h"

using namespace std;

class Simulation
{
private:
    unsigned int nbN;
    unsigned int nbF;

public:
    void readFile(string path);
};

void Simulation::readFile(string path)
{

    string line;
    ifstream file(path);
    if (file.fail())
    {
        exit(EXIT_FAILURE);
    }

    // 1. Parsing the foods
    get_next_line(file) >> nbN;
    vector<Food *> foods(nbN);

    int i = 1;
    while (i <= nbN)
    {
        foods[i] = new Food(get_next_line(file));
        i++;
    }

    // 2. Parsing the anthills
    get_next_line(file) >> nbF;
    vector<Anthill *> anthills(nbF);

    int i = 1;
    while (i <= nbF)
    {
        anthills[i] = new Anthill(get_next_line(file));

        // 3. Parsing the ants
        // 3.1 Parsing the collectors
        unsigned int nbC = anthills[i]->get_number_of_collectors();
        vector<Collector *> collectors(nbC);

        int j = 1;
        while (j <= nbC)
        {
            collectors[j] = new Collector(get_next_line(file));
            j++;
        }

        // 3.2 Parsing the defendors
        unsigned int nbD = anthills[i]->get_number_of_defensors();
        vector<Defensor *> defensors(nbD);

        int j = 1;
        while (j <= nbD)
        {
            defensors[j] = new Defensor(get_next_line(file));
            j++;
        }

        // 3.3 Parsing the predators
        unsigned int nbP = anthills[i]->get_number_of_predators();
        vector<Predator *> predators(nbP);

        int j = 1;
        while (j <= nbP)
        {
            predators[i] = new Predator(get_next_line(file));
            j++;
        }

        i++;
    }

    // 4. Testing that the anthils don't overlap
    bool lambda = [](Anthill *a1, Anthill *a2)
    {
        return test_if_superposed_two_square(*a1, *a2);
    };

    auto it = adjacent_find(anthills.begin(), anthills.end(), lambda);
    if (it != anthills.end())
    {
        cout << message::homes_overlap((*it)->x, (*it)->y);
        exit(EXIT_FAILURE);
    }

    file.close();
}

istringstream &get_next_line(ifstream &stream)
{
    string line;
    while (getline(stream >> ws, line))
    {
        if (line[0] == '#')
        {
            continue;
        }

        istringstream linestream(line);
        return linestream;
    }
}