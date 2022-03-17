class Collector : Square
{
public:
    Collector(std::istringstream &stream);

    void add_to_grid();
};

class Defensor : Square
{
public:
    Defensor(std::istringstream &stream);

    void add_to_grid();
};

class Predator : Square
{
public:
    Predator(std::istringstream &stream);

    void add_to_grid();
};