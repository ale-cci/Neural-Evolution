#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>
#include "generic_functions.h"
#include "world.h"
#include "mysdl.h"
#include "agent.h"

double area[AREA_HEIGHT][AREA_WIDTH];
int POPULATION_COUNT = 0;
AGENT agent[MAX_POPULATION];
int MEAT_COUNT = 0;
COORD meat[MAX_MEAT];

void printworld() {
    // print background color;
    // for print cell

    for (int y = 0; y < AREA_HEIGHT; ++y)
        for (int x = 0; x < AREA_WIDTH; ++x) {
            double intensity = area[y][x] / MAX_FOOD_IN_AREA;
            SDL_SetRenderDrawColor(renderer, 0, 255 * intensity, 0, 0xFF);
            SDL_Rect rekt = {x * SQUARE_SIZE, y * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE};
            SDL_RenderFillRect(renderer, &rekt);
        }
}
int t_init_food(void* args) {
    uint32_t delay = 1 * 1000;
    while(1) {
        addfood();
        SDL_Delay(delay);
    }
    return 0;
}
void init_food() {
    for (int y = 0; y < AREA_HEIGHT; ++y)
            for (int x = 0; x < AREA_WIDTH; ++x)
                area[y][x] =  rand(0.0,(double) MAX_FOOD_IN_AREA);

}

void init_agents() {
    while (POPULATION_COUNT < START_POPULATION) {
        insert_random_agent();
    }
}

void print_agents() {
    for (int i=0; i < POPULATION_COUNT; ++i)
        printagent(i);
}

void move_agents() {
    for (int i=0; i < POPULATION_COUNT; ++i)
        moveagent(i);
}

void input_agents(){
    for (int i=0; i < POPULATION_COUNT; ++i)
        give_agent_input(i);
}

void execute_agents() {
    for (int i=0; i < POPULATION_COUNT; ++i)
       execute_agent_input(i);
}

void output_agents() {
    for (int i=0; i < POPULATION_COUNT; ++i)
        execute_agent_output(i);
}

void addfood() {
    int X = rand(0, AREA_WIDTH-1),
        Y = rand(0, AREA_HEIGHT-1);
    area[Y][X] = std::min(area[X][Y]+1,(double) MAX_FOOD_IN_AREA);
}

void reproduction() {
    for (int i = POPULATION_COUNT -1; i >= 0; i--) {
        if ((agent[i].food_bar >= FOOD_FOR_REPRODUCTION) && (POPULATION_COUNT < MAX_POPULATION -1)){
            makeagent(POPULATION_COUNT);
            agent[POPULATION_COUNT].food_category = agent[i].food_category;
            agent[POPULATION_COUNT].brain = agent[POPULATION_COUNT].brain;
            agent[POPULATION_COUNT].brain.mutate();
            agent[i].food_bar = agent[i].food_bar / 3;
            POPULATION_COUNT++;
        }
    }
}
void update_world() {
    // update the age
    static uint64_t tick = time(NULL);
    uint64_t now = time(NULL);

    if (now - tick >= 1) {
        tick = now;
        check_agent_status();
    }
    while (POPULATION_COUNT < MIN_POPULATION)
        insert_random_agent();
    input_agents();
    execute_agents();
    output_agents();
    move_agents();
    reproduction();
    print_agents();
    /*
    static time_t actualtime = time(NULL);

    int ACTUAL_POPULATION = POPULATION_COUNT;
    for (int i=0; i < ACTUAL_POPULATION; ++i) {
        if ((agent[i].food_bar >= FOOD_FOR_REPRODUCTION) && (POPULATION_COUNT < MAX_POPULATION )){
            agent[i].food_bar = 0;
            if(POPULATION_COUNT < MAX_POPULATION -1) {
                makeagent(POPULATION_COUNT);
                agent[POPULATION_COUNT].food_category = agent[i].food_category + rand(-1., +1.);
                agent[POPULATION_COUNT].brain = agent[i].brain; // FIXME
                agent[POPULATION_COUNT].brain.mutate();
                POPULATION_COUNT++;
            }
        }
    }

    if ((((time(NULL) - actualtime) % (20 +1)) == 0) && (time(NULL) != actualtime)) {   // create random agent each N second
        actualtime = time(NULL);
        insert_random_agent();
    }
    */
}
void insert_random_agent() {
    if (POPULATION_COUNT < MAX_POPULATION) {
        makeagent(POPULATION_COUNT);
        POPULATION_COUNT++;
    }
}
void check_agent_status() {
    for (int i=0; i < POPULATION_COUNT; ++i) {
        agent[i].energy = agent[i].energy - (1 + agent[i].boost_strenght);
        if ((agent[i].energy < MAX_HEALTH) && (agent[i].food_bar > 0)) {
            double discard = std::min(agent[i].food_bar, 2.);
            agent[i].food_bar -= discard;
            agent[i].energy = bound(agent[i].energy + pow(discard, 2), 0, MAX_HEALTH);
        }
        if (agent[i].energy <= 0 ) {
            if (MEAT_COUNT < MAX_MEAT) {
                meat[MEAT_COUNT] = getcenter(i);
                MEAT_COUNT++;
            }
            kill(i);
            i--;
        }
    }
}
