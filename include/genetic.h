#pragma once

#include "settings.h"
#include "Individu.h"

#include <array>
#include <utility>

using Agent = Individu<NumberOfVectors, Dimension>;
using Population = std::array<Agent, PopulationSize>;
using HalfPopulation = std::array<Agent, HalfPopulationSize>;



real eval_agent (const Agent& a);

HalfPopulation selection_tournoi(const Population& p);
std::pair<Agent, Agent> cross_over (const Agent& p1, const Agent& p2);
Population cross_over_half_pop (HalfPopulation& hp);
void mutations (Population* p);
void create_generation (size_t indice, Population* p, bool is_saving_in_file = false);

void print_best_agent(const Population& p);

void genetic_algorithm ();