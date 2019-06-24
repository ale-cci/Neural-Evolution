#ifndef __AGENT_H__
#define __AGENT_H__
#include <inttypes.h>
#include "mysdl.h"
#include "extmath.h"
#include "neural_network.h"
#include "generic_agent.h"
#define AGENT_HERBIVORE 0
#define AGENT_CARNIVORE 1
#define NUMBER_OF_SPECIES 2
#define LAST_DEATH_NUMBER 16

const static double AGENT_MASS     = 1.;
const static int    AGENT_RED      = 0; // 235
const static int    AGENT_GREEN    = 206;
const static int    AGENT_BLUE     = 209;
const static double MAX_SPEED      = 2.;
extern SDL_Image agent_texture;

extern uint8_t lastdeath[2][LAST_DEATH_NUMBER];
extern uint16_t cnt_deaths[NUMBER_OF_SPECIES];

class Agent {
public:
    uint64_t age;
    double X;
    double Y;
    double energy;
    double rotation;
    double food_bar;
    uint8_t food_category;
    uint8_t children;
    _PRECISION f_left;
    _PRECISION f_right;
    double boost_strenght;
    NEURAL_NETWORK brain;
    SDL_Color col;

    void destroy();
    void write(std::ofstream& out);
    void read(std::ifstream& in);

    Agent& operator=(const Agent& temp) noexcept;
};

void init_lastdeath();

bool randomize_agent_positions();
//bool randomize_agent_neural_network();


// erbivore function
void crunch(const uint16_t id, const double strenght);
void bite(const uint16_t id, const double strenght);

bool in_agent(COORD P, const uint16_t id);
int32_t smell(const uint16_t id);
void food_sensor(const uint16_t id, double *distances);
void blood_sensor(const uint16_t id, double* sensors, double* distances);
SDL_Color look(const uint16_t id,const double direction, const double alpha);
void draw_sensor(const uint16_t id, double direction, double lenght, SDL_Color C);
double dist_sensor(const uint16_t id, double direction, double lenght, COORD T);
uint32_t max_food_radius(const uint16_t id);
COORD getcenter(const uint16_t id);
double average_death(const uint8_t species);
void add_death(uint8_t age);
#endif
