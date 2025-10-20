#include "randomizer.h"

std::mt19937 Randomizer::generator;
std::uniform_real_distribution<real> Randomizer::distrib_proba;
bool Randomizer::initialized = false;