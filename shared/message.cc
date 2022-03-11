#include "message.h"

#include <string>

using namespace std;

string message::success()
{
	return string("Correct file\n");
}

// superposition de deux fourmilières
string message::homes_overlap(unsigned h1, unsigned h2)
{
    //maintain_ascending_order of h1 and h2:
    if(h1>h2)
    {
		unsigned tmp(h2);
		h2 = h1;
		h1 = tmp;
	}
    return string("home ") + to_string(h1) + string(" overlaps with home ")
           + to_string(h2) + string("\n");
}

// superposition d'un élément de la simulation avec un autre élément exclusif
string message::food_overlap(unsigned fx, unsigned fy)
{
    return string("food with coordinates ") + to_string(fx) + string(" ")
           + to_string(fy) + string(" overlaps with another exclusive entity\n");
}

string message::predator_overlap(unsigned px, unsigned py)
{
    return string("predator with coordinates ") + to_string(px) + string(" ")
           + to_string(py) + string(" overlaps with another exclusive entity\n");
}

// même chose avec indication de la première cellule superposée (x,y)
string message::defensor_overlap(unsigned dx, unsigned dy, unsigned x, unsigned y)
{
    return string("defensor with coordinates ") + to_string(dx) + string(" ")
           + to_string(dy)
           + string(" overlaps with another exclusive entity at least on ")
           + to_string(x) + string(" ") + to_string(y) + string("\n");
}
string message::collector_overlap(unsigned cx, unsigned cy, unsigned x, unsigned y)
{
    return string("collector with coordinates ") + to_string(cx) + string(" ")
           + to_string(cy)
           + string(" overlaps with another exclusive entity at least on ")
           + to_string(x) + string(" ") + to_string(y) + string("\n");
}
string message::generator_overlap(unsigned gx, unsigned gy, unsigned x, unsigned y)
{
    return string("generator with coordinates ") + to_string(gx) + string(" ")
           + to_string(gy)
           + string(" overlaps with another exclusive entity at least on ")
           + to_string(x) + string(" ") + to_string(y) + string("\n");
}

// les entités generator et defensor doivent être à l'intérieur de leur fourmilière
string message::generator_not_within_home(unsigned gx, unsigned gy, unsigned h)
{
    return string("generator with coordinates ") + to_string(gx) + string(" ")
           + to_string(gy) + string(" is not fully within its home: ") + to_string(h)
           + string("\n");
}
string message::defensor_not_within_home(unsigned dx, unsigned dy, unsigned h)
{
    return string("defensor with coordinates ") + to_string(dx) + string(" ")
           + to_string(dy) + string(" is not fully within its home: ") + to_string(h)
           + string("\n");
}

