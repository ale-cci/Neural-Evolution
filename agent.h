#pragma once
#include <inttypes.h>

const static int AGENT_RADIUS   = 16;
const static int AGENT_RED      = 255;
const static int AGENT_GREEN    = 0;
const static int AGENT_BLUE     = 0;

extern SDL_Image* agent_texture;

struct AGENT {
    int16_t X;
    int16_t Y;
    int16_t energy;
    int16_t rotation;
    // neural network
};

void printagent(AGENT* agent);
bool init_agent(std::string PATH);
