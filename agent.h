#pragma once
#include <inttypes.h>

const static int AGENT_RADIUS   = 16;
const static int AGENT_RED      = 255;
const static int AGENT_GREEN    = 0;
const static int AGENT_BLUE     = 0;

extern SDL_Image agent_texture;

struct AGENT {
    int16_t X;
    int16_t Y;
    int16_t energy;
    double rotation;
    STRENGHT_PRECISION f_left;
    STRENGHT_PRECISION f_right;
    int32_t boost_strenght = 1;
};

void printagent(AGENT* agent);

bool randomize_agent_positions();
//bool randomize_agent_neural_network();

AGENT* makeagent();

void moveagent(AGENT* ag);
void eat(AGENT* ag);
