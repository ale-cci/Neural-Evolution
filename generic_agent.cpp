#include <cmath>
#include <cassert>
#include "generic_agent.h"
uint8_t lastdeath[16];

void init_lastdeath() {
    for (int i =0; i < 16; ++i)
        lastdeath[i] = MAX_HEALTH*3/5;
}

void add_death(uint8_t age) {
    static uint8_t position = 0;
    position = (position + 1) % 16;
    lastdeath[position] = age;
}

uint8_t average_death() {
    uint8_t sum = 0;
    for (int i=0; i < 16; ++i)
        sum += lastdeath[i];
    return sum / 16;
}

void kill(const uint16_t id) {
    if (id == triggered)
        triggered = -1;
    else
    if (POPULATION_COUNT -1 == triggered)
        triggered = id;
    add_death(agent[id].age);
    agent[id]  = agent[POPULATION_COUNT -1];
    agent[POPULATION_COUNT -1].destroy();
    POPULATION_COUNT--;
}

