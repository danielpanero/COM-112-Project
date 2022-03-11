class Collector : Square
{
public:
    Collector(istringstream &stream);

    void add_to_grid();
};

class Defensor : Square
{
public:
    Defensor(istringstream &stream);

    void add_to_grid();
};

class Predator : Square
{
public:
    Predator(istringstream &stream);

    void add_to_grid();
};