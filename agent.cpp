#include <cstdlib>
#include <iostream>
#include <cmath>
#include "generic_functions.h"
#include "world.h"
#include "mysdl.h"
#include "extmath.h"
#include "agent.h"

SDL_Image agent_texture;

AGENT* makeagent() {
    AGENT agent;
    agent_texture = *load_trsp("IMAGES/debug01.bmp");
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
    //std::cout << moveX << " " << moveY << " " << acceleration << std::endl;
    //agnt->speed = acceleration + agnt->speed;
    //agnt->boost_strenght -= distance;

}
int32_t getcellX(AGENT * _agent) {
    return (_agent->X + agent_texture.width/2) / SQUARE_SIZE;
}
int32_t getcellY(AGENT * _agent) {
    // TODO change this to height/2
    return (_agent->Y + agent_texture.height/2)/ SQUARE_SIZE;
}
void eat(AGENT* _agent) {
    _agent->energy--;

    int32_t cellX = getcellX(_agent);
    int32_t cellY = getcellY(_agent);
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
SDL_Color look(AGENT *_agent, double direction) {
    while(_agent->rotation < 0)
        _agent->rotation += 360;
    while(_agent->rotation >= 360)
        _agent->rotation -=360;
    // FIXME tan(90)
    COORD P = getcenter(_agent);
    double m_line = tan(direction);
    double q_line = -tan(direction) * P.X + P.Y;
    SDL_Color result = {0, 0, 0, 255};

    #ifdef DEBUG
    SDL_SetRenderDrawColor(renderer, 0xff, 0, 0, 0);
    SDL_RenderDrawLine(renderer, P.X, P.Y, P.X + AGENT_SEEN_RADIUS * cos(direction),m_line * (P.X + AGENT_SEEN_RADIUS * cos(direction)) + q_line);
    #endif // DEBUG
    COORD collisions[2];
    uint8_t number_of_collisions;
    double dist = 0;
    for (int i = 1; i <POPULATION_COUNT; ++i) {
        line_circle_collision(m_line, q_line, getcenter(&agent[i]).X, getcenter(&agent[i]).Y, agent_texture.width/2, collisions, &number_of_collisions);

        if (number_of_collisions == 0)
            continue;
        dist = std::min(distance(&collisions[0], &P), distance(&collisions[1], &P));

        if (dist <= AGENT_SEEN_RADIUS ) {
            // FIXME set rotation interval to 0,360
            if ((_agent->rotation > 270) && (_agent->rotation < 90))
                if (collisions[0].Y > P.Y)
                    continue;
            if ((_agent->rotation > 90) && (_agent->rotation < 270))
                if (collisions[0].Y < P.Y)
                    continue;
            if (_agent->rotation == 90)
                if (collisions[0].X < P.X)
                    continue;
            if (_agent->rotation == 270)
                if (collisions[0].X > P.X)
                    continue;
            for (int i = 0; i < number_of_collisions; ++i)
            std::cout << "{" << collisions[0].Y << " " << P.Y << "}" << "[" << _agent->rotation << "] " ;
            result.r = AGENT_RED;
            result.g = AGENT_GREEN;
            result.b = AGENT_BLUE;
            break;
        }
    }
    return result;
}

COORD getcenter(AGENT* _agent) {
    return {_agent->X + agent_texture.width/2, _agent->Y + agent_texture.height/2};
}

void give_agent_input(AGENT * _agent) {
    double food = area[getcellY(_agent)][getcellX(_agent)],
        neightbours = get_neightbours(_agent);

    double health = _agent->energy;
    double foodeyeleft, foodeyeright;

    SDL_Color left_eye = look(_agent, (120 + _agent->rotation + 90)* PI / 180);
    SDL_Color right_eye  = look(_agent, (240 + _agent->rotation + 90)* PI / 180);
    std::cout << " " <<(int)left_eye.b << " " << (int)right_eye.b << " " << std::endl;
   // bool collision = in_agent({x + _agent->X + agent_texture.width / 2, y + _agent->Y + agent_texture.height / 2}, _agent);

    _PRECISION inputs[INPUT_CELLS] = {food, neightbours, health};
    _agent->brain.input(inputs);
}

