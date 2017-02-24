#pragma once
#include <vector>
#include "mysdl.h"
#include "agent.h"


const static int SQUARE_SIZE = 16;
const static int MAX_FOOD_IN_AREA =3;
const static int PAUSE_DELAY    = 30;
const static int MAX_POPULATION = 32;
const static int MAX_HEALTH     = 50;
const static int START_POPULATION = 3;
const static int MIN_POPULATION = 8;
const static int AREA_WIDTH     = SCREEN_WIDTH / SQUARE_SIZE;
const static int AREA_HEIGHT    = SCREEN_HEIGHT / SQUARE_SIZE;
const static STRENGHT_PRECISION PROPULSOR_FORCE = 1;
const static double FRICTION_CONSTANT = 1;
const static double GRAVITY_CONSTANT = 9.81;
const static double AGENT_SEEN_RADIUS = 64;
const static double AGENT_BITE_RADIUS = 32;
const static double VEGETABLE_ENERGY = 3;
const static int MEAT_ENERGY = 6;
const static int BITE_DAMAGE = 5;
const static int BITE_ENERGY = 1;
const static double CHANGING_FOOD_RATE = 0.8;
const static double MAX_STRENGHT = 12;

extern int8_t area[AREA_HEIGHT][AREA_WIDTH];
extern AGENT  agent[MAX_POPULATION];
extern int POPULATION_COUNT;

void init_food();
void init_agents();
void printworld();
void print_agents();
void move_agents();
void input_agents();
void execute_agents();
void output_agents();
void update_world();
