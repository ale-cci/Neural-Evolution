#pragma once
#include <inttypes.h>
#include "neural_network.h"

const static double AGENT_MASS     = 1.0;
const static int AGENT_RED      = 0; // 235
const static int AGENT_GREEN    = 206;
const static int AGENT_BLUE     = 209;
const static double MAX_SPEED   = 1.0;
extern SDL_Image agent_texture;
struct COORD {
    double X;
    double Y;
};

struct AGENT {
    double X;
    double Y;
    int16_t energy;
    double rotation;
    double speed;
    STRENGHT_PRECISION f_left;
    STRENGHT_PRECISION f_right;
    int32_t boost_strenght = 1;
    NEURAL_NETWORK brain;
};

void printagent(AGENT* agent);

bool randomize_agent_positions();
//bool randomize_agent_neural_network();

AGENT* makeagent();

void moveagent(AGENT* _agent);
void give_agent_input(AGENT * _agent);
void eat(AGENT* _agent);
int32_t getcellX(AGENT * _agent);
int32_t getcellY(AGENT * _agent);
int32_t get_neightbours(AGENT* _agent);
COORD centerof(AGENT* _agent);
