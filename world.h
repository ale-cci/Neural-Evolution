#pragma once
#include <vector>
#include "mysdl.h"
#include "agent.h"


const static int SQUARE_SIZE = 32;
const static int MAX_FOOD_IN_AREA =3;
const static int PAUSE_DELAY    = 30;
const static int MAX_POPULATION = 16; // 24
const static uint16_t MAX_HEALTH     = 50;
const static int START_POPULATION = 2;
const static int MIN_POPULATION = 8; // 12
const static int AREA_WIDTH     = SCREEN_WIDTH / SQUARE_SIZE;
const static int AREA_HEIGHT    = SCREEN_HEIGHT / SQUARE_SIZE;
const static double FRICTION_CONSTANT = 1;
const static double GRAVITY_CONSTANT = 9.81;
const static double AGENT_SEEN_RADIUS = 96; /// 96
const static double AGENT_BITE_RADIUS = 32;
const static double VEGETABLE_ENERGY = 5;
const static int MEAT_ENERGY = 10;
const static uint8_t AGENT_RADIUS = 10;
const static int BITE_DAMAGE = 5;
const static int BITE_ENERGY = 0.;
const static int CRUNCH_ENERGY = 0.;
const static uint8_t VEGETABLE_DIGEST_TIME = 4;
const static uint8_t MEAT_DIGEST_TIME  = 2;
const static double CHANGING_FOOD_RATE = 0.8;
const static double MAX_STRENGHT = 2;
const static double FOOD_FOR_REPRODUCTION = 40;
const static int MAX_MEAT = MAX_POPULATION;
const static float MAX_FOOD_CATEGORY = 2;
const static uint16_t AGENT_BLOOD_RADIUS = 120;
const static uint16_t AGENT_GRASS_RADIUS = 90;
const static uint8_t FOOD_SENSORS = 3;
const static uint16_t MAX_FOOD_BAR = 100;

extern double area[AREA_HEIGHT][AREA_WIDTH];
extern Agent agent[MAX_POPULATION];
extern int POPULATION_COUNT;
extern COORD meat[MAX_MEAT];
extern int MEAT_COUNT;

extern int32_t triggered;
extern float   fast_forward;
static int REAL_WIDTH;
static int REAL_HEIGHT;

int t_init_food(void* );
void init_food();
void init_agents();
void addfood();
void printworld();
void print_agents();
void move_agents();
void input_agents();
void execute_agents();
void output_agents();
void update_world();
void insert_random_agent();
void check_agent_status();
