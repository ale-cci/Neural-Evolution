#include "generic_agent.h"

void kill(const uint16_t id) {
    agent[id] = agent[POPULATION_COUNT -1];
    agent[POPULATION_COUNT -1].destroy();
    POPULATION_COUNT--;
}

void printagent(const uint16_t id);
