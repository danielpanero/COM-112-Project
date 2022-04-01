#include "error_squarecell.h"

#include <iostream>
#include <string>

using namespace std;

// violation du domaine autorisé d'un indice [0, g_max-1]
string error_squarecell::print_index(unsigned index, unsigned max)
{
    return string("coordinate ") + to_string(index)
           + string(" does not belong to [ 0, ") + to_string(max) + string(" ]\n");
}

// violation du domaine couvert [0, g_max-1]
// par la combinaison de l'origine et du coté d'un carré
string error_squarecell::print_outside(unsigned index, unsigned side, unsigned max)
{
    return string("combined coordinate ") + to_string(index)
           + string(" and square side ") + to_string(side)
           + string(" do not belong to [ 0, ") + to_string(max) + string(" ]\n");
}

