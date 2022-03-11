#ifndef CONSTANTES_H_INCLUDED
#define CONSTANTES_H_INCLUDED

enum Etat_fourmiliere{FREE, CONSTRAINED};
enum Etat_collector{EMPTY, LOADED};

constexpr short unsigned maxF(25);
constexpr short unsigned sizeG(5);
constexpr short unsigned sizeC(3);
constexpr short unsigned sizeD(3);
constexpr short unsigned sizeP(1);

constexpr short unsigned bug_life(300);

constexpr short unsigned val_food(50);

constexpr double food_rate(0.1);
constexpr double max_food_trial(10);
constexpr double birth_rate(0.00005);
constexpr double prop_free_collector(0.85);
constexpr double prop_free_defensor(0.10);
constexpr double prop_constrained_collector(0.6);
constexpr double prop_constrained_defensor(0.1);

#endif // CONSTANTES_H_INCLUDED
