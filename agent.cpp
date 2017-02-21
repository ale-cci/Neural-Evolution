#include <cstdlib>
#include <iostream>
#include <cmath>
#include "debug.h"
#include "release.h"
#include "world.h"
#include "mysdl.h"
#include "agent.h"

SDL_Image agent_texture;

void printagent(AGENT* agent) {
    SDL_SetTextureColorMod(agent_texture.texture, AGENT_RED, AGENT_GREEN, AGENT_BLUE);
    SDL_Rect rekt = {agent->X, agent->Y, agent_texture.width, agent_texture.height};
    SDL_Point pnt = {agent_texture.width/2, agent_texture.height/2};
    SDL_RenderCopyEx(renderer, agent_texture.texture,NULL, &rekt, agent->rotation, &pnt , SDL_FLIP_NONE);
    //printg(&agent_texture, agent->X, agent->Y);
}

AGENT* makeagent() {
    AGENT agent;
    agent_texture = *load_trsp("IMAGES/debug00.bmp");
    agent.energy = rand(0, 100);
    agent.rotation = rand(0,360);
    agent.f_left = 0;
    agent.f_right = 0;
    agent.X = rand(agent_texture.width, SCREEN_WIDTH - agent_texture.width);
    agent.Y = rand(agent_texture.height, SCREEN_HEIGHT - agent_texture.height);
    return &agent;
}


void moveagent(AGENT* agnt) {
    double difference = agnt->f_left - agnt->f_right;
    int distance = log2(agnt->boost_strenght);
    if (agnt->f_left > agnt->f_right) {
        agnt->rotation++;
    }
    else
    if (agnt->f_left < agnt->f_right) {
        agnt->rotation--;
    }
    if (agnt->rotation > 360)
        agnt->rotation -= 360;
    else
    if (agnt->rotation < 0)
        agnt->rotation += 360;
    agnt->X = agnt->X + distance * sin(agnt->rotation * PI / 180);
    agnt->Y = agnt->Y - distance * cos(agnt->rotation * PI / 180);
    agnt->boost_strenght -= distance;
}

void eat(AGENT* ag) {
    ag->energy--;

    int32_t cellX = (ag->X + agent_texture.width/2) / SQUARE_SIZE;
    int32_t cellY = (ag->Y + agent_texture.height/4)/ SQUARE_SIZE; // TODO change this to height/2
    std::cout << cellX << " " << cellY << std::endl;
    if (area[cellY][cellX] > 0) {
        ag->energy += 5;
        area[cellY][cellX] --;
    }
}
