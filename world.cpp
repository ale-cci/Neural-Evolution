#include <cstdlib>
#include <cmath>
#include <ctime>
#include <cassert>
#include <iostream>
#include <algorithm>

#include <cstdio>

#include "generic_functions.h"
#include "world.h"
#include "mysdl.h"
#include "agent.h"

double area[AREA_HEIGHT][AREA_WIDTH];
int POPULATION_COUNT = 0;
Agent agent[MAX_POPULATION];
int MEAT_COUNT = 0;
COORD meat[MAX_MEAT];
int32_t triggered = -1;
uint8_t printoutput = false;
float fast_forward = 1;


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
    for (int i=0; i < POPULATION_COUNT; ++i) {
        #ifdef DEBUG
        assert(!std::isnan(agent[i].food_bar));
        #endif
        printagent(i);
    }
}

void move_agents() {
    for (int i=0; i < POPULATION_COUNT; ++i) {
        #ifdef DEBUG
        assert(!std::isnan(agent[i].food_bar));
        #endif
        moveagent(i);
    }
}

void input_agents(){
    for (int i=0; i < POPULATION_COUNT; ++i) {
        #ifdef DEBUG
        assert(!std::isnan(agent[i].food_bar));
        #endif
        give_agent_input(i);
    }
}

void execute_agents() {
    for (int i=0; i < POPULATION_COUNT; ++i) {
        #ifdef DEBUG
        assert(!std::isnan(agent[i].food_bar));
        #endif
        execute_agent_input(i);
    }
}

void output_agents() {
    for (int i=0; i < POPULATION_COUNT; ++i) {
        #ifdef DEBUG
        assert(!std::isnan(agent[i].food_bar));
        #endif
        execute_agent_output(i);
    }
}

void addfood() {
    int X = rand(0, AREA_WIDTH-1),
        Y = rand(0, AREA_HEIGHT-1);
    area[Y][X] = std::min(area[X][Y]+1,(double) MAX_FOOD_IN_AREA);
}

void reproduction() {

    for (int i = POPULATION_COUNT -1; (i >= 0) && (POPULATION_COUNT < MAX_POPULATION-1); i--) {
        agent[i].age++;
        if(agent[i].age / (agent[i].children+1) >= average_death(agent[i].food_category)) {
            insert_random_agent();
            agent[POPULATION_COUNT-1].food_category = agent[i].food_category;
            agent[POPULATION_COUNT-1].brain = agent[i].brain;
            agent[POPULATION_COUNT-1].brain.mutate();
            agent[i].children++;
        }
    }
}
void update_world() {
    // update the age
    static uint64_t clock1 = 0;
    static uint64_t clock2 = 0;

    uint64_t tick = clock();

    if (tick - clock1 >= 1000*(1./fast_forward)) {
        clock1 = tick;
        addfood();
        check_agent_status();
        reproduction();
    }

    if (tick - clock2 >= 10000*(1./fast_forward)) {
        insert_random_agent();
        clock2 = tick;
    }

    while (POPULATION_COUNT < MIN_POPULATION)
        insert_random_agent();
    input_agents();
    execute_agents();
    output_agents();
    move_agents();
    print_agents();
    const uint32_t START_X = SCREEN_WIDTH -140;
    const uint32_t START_Y = 70;
    const uint32_t SPACE   = 10;

    write(START_X, 20, "MAX_POPUL.: %d", MAX_POPULATION);
    write(START_X, 30, "POPULATION: %d", POPULATION_COUNT);
    write(START_X, 40, "FAST_FRWRD: %.1fx", fast_forward);
    write(START_X, 50, "AVG_HERBIV: %.2f", average_death(AGENT_HERBIVORE));
    write(START_X, 60, "AVG_CARNIV: %.2f", average_death(AGENT_CARNIVORE));

    if (triggered != -1) {
        write(START_X, START_Y + SPACE*1, "ID:   %d", triggered);
        write(START_X, START_Y + SPACE*2, "AGE:  %d", agent[triggered].age);
        write(START_X, START_Y + SPACE*3, "HLTH: %f", agent[triggered].energy);
        write(START_X, START_Y + SPACE*4, "FOOD: %f", agent[triggered].food_bar);
        write(START_X, START_Y + SPACE*5, "B.ST: %f", agent[triggered].boost_strenght);
        write(START_X, START_Y + SPACE*6, "F_LF: %f", agent[triggered].f_left / MAX_STRENGHT);
        write(START_X, START_Y + SPACE*7, "F_RG: %f", agent[triggered].f_right / MAX_STRENGHT);

    }
    else
        write(START_X, START_Y + SPACE*1, "NOTHING SELECTED");
}

void insert_random_agent() {
    if (POPULATION_COUNT < MAX_POPULATION){
        makeagent(POPULATION_COUNT++);
    }
}
double age_handicap(const uint16_t id) {
    const uint64_t x = agent[id].age;
    return 1./(1 + exp((6. - x)));
}

double food_lost(const uint16_t id) {
    const double x = agent[id].food_bar / MAX_FOOD_BAR;
    return (2.914 * pow(x, 2) - 3.414 * x +1);
}

double boost_lost(const uint16_t id) {
    const double x = agent[id].boost_strenght;
    const float disc = ((agent[id].food_category == AGENT_CARNIVORE)?0.5:1);
    return pow(x, 2) * disc;
}

void check_agent_status() {
    for (int i=0; i < POPULATION_COUNT; ++i) {
        agent[i].energy = agent[i].energy - (1 + (food_lost(i) + boost_lost(i))*age_handicap(i));

        if ((agent[i].energy < MAX_HEALTH) && (agent[i].food_bar > 0)) {
            double discard = std::min(agent[i].food_bar, 1.);
            agent[i].food_bar = bound(agent[i].food_bar - sqrt(discard), 0, MAX_FOOD_BAR);
            agent[i].energy = bound(agent[i].energy + pow(discard, 2), 0, MAX_HEALTH);
        }

        if (agent[i].energy <= 0 ) {
            if (MEAT_COUNT < MAX_MEAT) {
                meat[MEAT_COUNT] = agent[i].center();
                MEAT_COUNT++;
            }
            kill(i);
            i--;
        }
    }
}
