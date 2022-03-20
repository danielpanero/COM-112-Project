#ifndef MESSAGE_H_INCLUDED
#define MESSAGE_H_INCLUDED

#include <string>

/**
 * Module: message
 * This module is used to generate the strings
 * for you to have consistent error messages.
 *
 * the first string documents a successful reading
 */

namespace message
{
    // Everything went well => file reading and all validation checks
	std::string success();

	// superposition de deux fourmilières
    std::string homes_overlap(unsigned h1, unsigned h2);

	// superposition d'un élément de la simulation avec un autre élément exclusif
    std::string food_overlap(unsigned fx, unsigned fy);
    std::string predator_overlap(unsigned px, unsigned py);

	// même chose avec indication de la première cellule superposée (x,y)
    std::string defensor_overlap(unsigned dx, unsigned dy, unsigned x, unsigned y);
    std::string collector_overlap(unsigned cx, unsigned cy, unsigned x, unsigned y);
    std::string generator_overlap(unsigned gx, unsigned gy, unsigned x, unsigned y);

    // les entités generator et defensor doivent être à l'intérieur de leur fourmilière
    std::string generator_not_within_home(unsigned gx, unsigned gy, unsigned h);
    std::string defensor_not_within_home(unsigned dx, unsigned dy, unsigned h);
}

#endif // MESSAGE_H_INCLUDED
