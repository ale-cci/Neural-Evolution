#include <cstdlib>
#include <ctime>
#include "generic_functions.h"
#include "world.h"
#include "mysdl.h"
#include "agent.h"

int8_t area[AREA_HEIGHT][AREA_WIDTH];
int POPULATION_COUNT = START_POPULATION;
AGENT agent[MAX_POPULATION];

void printworld() {
    // print background color;
    // for print cell

    for (int y = 0; y < AREA_HEIGHT; ++y)
        for (int x = 0; x < AREA_WIDTH; ++x) {
            uint16_t intensity = (256  * area[y][x] / MAX_FOOD_IN_AREA);
            if (intensity >= 256)
                intensity = 255;
            SDL_SetRenderDrawColor(renderer, 0, intensity, 0, 0xFF);
            SDL_Rect rekt = {x * SQUARE_SIZE, y * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE};
            SDL_RenderFillRect(renderer, &rekt);
        }
}

void init_food() {
    for (int y = 0; y < AREA_HEIGHT; ++y)
        for (int x = 0; x < AREA_WIDTH; ++x) {
            area[y][x] = rand() % MAX_FOOD_IN_AREA;
            if (area[y][x] < 0)
                area[y][x] = 0;
        }
    return ;
}

void init_agents() {
    for (int i = 0; i < START_POPULATION; ++i) {
        agent[i] = *makeagent();
        agent[i].brain.init();
    }
}

void print_agents() {
    for (int i=0; i < POPULATION_COUNT; ++i)
        printagent(&agent[i]);
}

void move_agents() {
    for (int i=0; i < POPULATION_COUNT; ++i)
        moveagent(&agent[i]);
}

void input_agents() {
    for (int i=0; i < POPULATION_COUNT; ++i)
        give_agent_input(&agent[i]);
}

void execute_agents() {
    for (int i=0; i < POPULATION_COUNT; ++i)
       execute_agent_input(i);
}

void output_agents() {
    for (int i=0; i < POPULATION_COUNT; ++i)
        execute_agent_output(i);
}

void update_world() {
    // update the age
    static time_t actualtime = time(NULL);
    static time_t lose_energy = time(NULL);
    if (lose_energy != time(NULL)) {
        lose_energy = time(NULL);
        for (int i=0; i < POPULATION_COUNT; ++i)
            agent[i].energy--;
    }
    for (int i=0; i < POPULATION_COUNT; ++i) {
        if (agent[i].energy <= 0) {
            agent[i] = agent[POPULATION_COUNT-1];
            POPULATION_COUNT--;
        }
    }
    while(POPULATION_COUNT < MIN_POPULATION) {
        agent[POPULATION_COUNT] = *makeagent();
        agent[POPULATION_COUNT].brain.init();
        POPULATION_COUNT++;
    }

    int ACTUAL_POPULATION = POPULATION_COUNT;
    for (int i=0; i < ACTUAL_POPULATION; ++i) {
        if ((agent[i].food_bar >= 100) && (POPULATION_COUNT < MAX_POPULATION )){
            agent[i].food_bar = 0;
            agent[i].energy   = MAX_HEALTH;
            agent[POPULATION_COUNT] = *makeagent();
            agent[POPULATION_COUNT].brain.init();
            agent[POPULATION_COUNT].brain.inherit(&agent[i].brain);
            agent[POPULATION_COUNT].brain.mutate();
            POPULATION_COUNT++;
        }
    }
    if ((((time(NULL) - actualtime) % (20 +1)) == 0) && (time(NULL) != actualtime)) {
        init_food();
        actualtime = time(NULL);
        if (POPULATION_COUNT < MAX_POPULATION) {

            agent[POPULATION_COUNT] = *makeagent();
            agent[POPULATION_COUNT].brain.init();
            POPULATION_COUNT++;
        }

    }
}
