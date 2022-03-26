#ifndef SIMULATION_H
#define SIMULATION_H

class Simulation
{
private:
    unsigned int nbN;
    unsigned int nbF;

public:
    void read_file(std::string &path);
};

#endif