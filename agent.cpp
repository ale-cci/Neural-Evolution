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
    SDL_Rect rekt = {(int) agent->X,(int) agent->Y, agent_texture.width, agent_texture.height};
    SDL_Point pnt = {agent_texture.width/2, agent_texture.height/4};
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
    //int distance = log2(agnt->boost_strenght);m
    double acceleration = std::min(agnt->f_left, agnt->f_right) / AGENT_MASS;
    double moveX = std::min(acceleration, MAX_SPEED)* sin(agnt->rotation * PI / 180);
    double moveY = std::min(acceleration, MAX_SPEED)* cos(agnt->rotation * PI/180);
    if (acceleration > 0 ) {
        //acceleration -= FRICTION_CONSTANT * GRAVITY_CONSTANT;
    }
    agnt->X = agnt->X + moveX;
    agnt->Y = agnt->Y - moveY;
    agnt->rotation = agnt->rotation + ((agnt->f_right - agnt->f_left) / (agent_texture.width * AGENT_MASS)) * 180/PI;
    if (agnt->rotation > 360)
        agnt->rotation -= 360;
    else
    if (agnt->rotation < 0)
        agnt->rotation += 360;
    std::cout << moveX << " " << moveY << " " << acceleration << std::endl;
    //agnt->speed = acceleration + agnt->speed;
    //agnt->boost_strenght -= distance;

}
int32_t getcellX(AGENT * _agent) {
    return (_agent->X + agent_texture.width/2) / SQUARE_SIZE;
}
int32_t getcellY(AGENT * _agent) {
    // TODO change this to height/2
    return (_agent->Y + agent_texture.height/4)/ SQUARE_SIZE;
}
void eat(AGENT* _agent) {
    _agent->energy--;

    int32_t cellX = getcellX(_agent);
    int32_t cellY = getcellY(_agent);
    std::cout << cellX << " " << cellY << std::endl;
    if (area[cellY][cellX] > 0) {
        _agent->energy += 5;
        area[cellY][cellX] --;
    }
}
int32_t get_neightbours(AGENT* _agent) {
    int32_t neightbours = 0;
    // FIXME : keep in mind the sin & cos prop. to calculate the centers
    for (int i=0; i < POPULATION_COUNT; ++i)
        if ( pow(agent[i].X - _agent->X, 2) + pow(agent[i].Y - _agent->Y, 2) < pow(AGENT_SEEN_RADIUS, 2))
            ++neightbours;
    return neightbours;
}

bool in_agent(COORD P, AGENT* _agent) {
    return pow((P.X - _agent->X), 2) + pow((P.Y - _agent->Y), 2) <= pow(agent_texture.width/2, 2);
}
SDL_Color look(COORD P, double direction) {
    // FIXME if tan(90)
    while (direction < 0)
        direction += 360;
    while (direction >= 360)
        direction -= 360;

    SDL_Color result = {0, 0, 0, 255};
    int8_t facing = (tan(direction) > 0)? 1 : -1;
    for (double x = 0; x <= AGENT_SEEN_RADIUS * cos(direction); x += 0.1) {

        double y = tan(direction) * x;
        for (int i = 0; i < POPULATION_COUNT; ++i)
            if (in_agent({P.X + x * facing, P.Y + y * facing }, &agent[i])) {
                result.r = AGENT_RED;
                result.g = AGENT_GREEN;
                result.b = AGENT_BLUE;
                result.a = sqrt(x * x + y * y);
                return result;
            }
    }
    return result;
}

COORD centerof(AGENT* _agent) {
    return {_agent->X + agent_texture.width/2, agent->Y + agent_texture.height/2};
}

void give_agent_input(AGENT * _agent) {
    double food = area[getcellY(_agent)][getcellX(_agent)],
        neightbours = get_neightbours(_agent);

    double health = _agent->energy;
    double foodeyeleft, foodeyeright;

    SDL_Color left_eye = look(centerof(_agent), 60 - _agent->rotation),
            right_eye  = look(centerof(_agent), 60 + _agent->rotation);
   // bool collision = in_agent({x + _agent->X + agent_texture.width / 2, y + _agent->Y + agent_texture.height / 2}, _agent);

    _PRECISION inputs[INPUT_CELLS] = {food, neightbours, health};
    _agent->brain.input(inputs);
}
