#include <cmath>
#include <cassert>
#include "generic_agent.h"

uint8_t lastdeath[NUMBER_OF_SPECIES][LAST_DEATH_NUMBER];
uint16_t cnt_deaths[NUMBER_OF_SPECIES];

void init_lastdeath() {
    for (int y =0; y < NUMBER_OF_SPECIES; ++y)
        for (int x =0; x < LAST_DEATH_NUMBER; ++x)
            lastdeath[y][x] = MAX_HEALTH*3/5;
}

void add_death(const uint16_t id) {
    /* Log death of agent id */
    assert(agent[id].food_category < NUMBER_OF_SPECIES);
    auto position = cnt_deaths[agent[id].food_category];
    lastdeath[agent[id].food_category][position] = agent[id].age;
    position = (position + 1) % LAST_DEATH_NUMBER;
    cnt_deaths[agent[id].food_category] = position;
}

double average_death(const uint8_t species) {
    double sum = 0;
    for (int i=0; i < LAST_DEATH_NUMBER; ++i)
        sum += lastdeath[species][i];
    return sum / LAST_DEATH_NUMBER;
}

void kill(const uint16_t id) {
    if (id == triggered)
        triggered = -1;
    else
    if (POPULATION_COUNT -1 == triggered)
        triggered = id;

    add_death(id);
    agent[id]  = agent[POPULATION_COUNT -1];
    agent[POPULATION_COUNT -1].destroy();
    POPULATION_COUNT--;
}

