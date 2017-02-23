#pragma once
#include <vector>
#include "mysdl.h"
#include "agent.h"


const static int SQUARE_SIZE = 32;
const static int MAX_FOOD_IN_AREA =8;
const static int PAUSE_DELAY    = 30;
const static int MAX_POPULATION = 128;
const static int START_POPULATION = 32;
const static int MIN_POPULATION = 32;
const static int AREA_WIDTH     = SCREEN_WIDTH / SQUARE_SIZE;
const static int AREA_HEIGHT    = SCREEN_HEIGHT / SQUARE_SIZE;
const static STRENGHT_PRECISION PROPULSOR_FORCE = 1;
const static double FRICTION_CONSTANT = 1;
const static double GRAVITY_CONSTANT = 9.81;
const static double AGENT_SEEN_RADIUS = 64;
extern int8_t area[AREA_HEIGHT][AREA_WIDTH];
extern AGENT  agent[MAX_POPULATION];
extern int POPULATION_COUNT;

void init_food();
void init_agents();
void printworld();
void print_agents();
void move_agents();
