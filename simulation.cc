#include "iostream"
#include "fstream"
#include "sstream"
#include "algorithm"
#include "vector"

#include "entities/food.h"
#include "entities/anthill.h"
#include "entities/ants.h"
#include "message.h"

#include "simulation.h"

using namespace std;

string get_next_line(ifstream &stream);

void Simulation::read_file(string path)
{

    string line;
    istringstream streamed_line;

    ifstream file(path);
    if (file.fail())
    {
        exit(EXIT_FAILURE);
    }

    // 1. Parsing the food
    line = get_next_line(file);
    streamed_line.clear();
    streamed_line.str(line);
    streamed_line >> nbN;
    vector<Food *> foods(nbN);

    unsigned int i = 0;
    while (i < nbN)
    {
        line = get_next_line(file);
        foods[i] = new Food(line);
        i++;
    }

    // 2. Parsing the anthills
    line = get_next_line(file);
    streamed_line.clear();
    streamed_line.str(line);

    streamed_line >> nbF;
    vector<Anthill *> anthills(nbF);

    i = 0;
    while (i < nbF)
    {
        line = get_next_line(file);
        anthills[i] = new Anthill(line);

        // 3. Parsing the ants
        // 3.1 Parsing the collectors
        unsigned int nbC = anthills[i]->get_number_of_collectors();
        vector<Collector *> collectors(nbC);

        unsigned int j = 0;
        while (j < nbC)
        {
            line = get_next_line(file);
            collectors[j] = new Collector(line);
            j++;
        }
        anthills[i]->set_collectors(collectors);

        // 3.2 Parsing the defendors
        unsigned int nbD = anthills[i]->get_number_of_defensors();
        vector<Defensor *> defensors(nbD);

        j = 0;
        while (j < nbD)
        {
            line = get_next_line(file);
            defensors[j] = new Defensor(line);
            j++;
        }
        anthills[i]->set_defensors(defensors);

        // 3.3 Parsing the predators
        unsigned int nbP = anthills[i]->get_number_of_predators();
        vector<Predator *> predators(nbP);

        j = 0;
        while (j < nbP)
        {
            line = get_next_line(file);
            predators[i] = new Predator(line);
            j++;
        }
        anthills[i]->set_predators(predators);

        i++;
    }

    // 4. Testing that the anthils don't overlap
    for (size_t i = 0; i < anthills.size(); i++)
    {
        for (size_t j = 0; j < i; j++)
        {
            Square square1(anthills[i]->get_as_square());
            Square square2(anthills[j]->get_as_square());

            if (test_if_superposed_two_square(square1, square2))
            {
                cout << message::homes_overlap(i, j);
                exit(EXIT_FAILURE);
            }
        }
    }

    // 5. Testing that generator and defensor are contained in their anthill
    for (size_t i = 0; i != anthills.size(); ++i)
    {
        anthills[i]->test_if_generator_defensors_perimeter(i);
    }

    file.close();

    cout << message::success();
}

string get_next_line(ifstream &stream)
{
    string line;
    while (getline(stream >> ws, line))
    {
        if (line[0] == '#')
        {
            continue;
        }
        return line;
    }
    return "";
}