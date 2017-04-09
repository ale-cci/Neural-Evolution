#include <cstdlib>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <cmath>
#include "generic_functions.h"
#include "world.h"
#include "mysdl.h"
#include "extmath.h"
#include "agent.h"

SDL_Image agent_texture;

void makeagent(const uint16_t id) {
    agent[id].energy = MAX_HEALTH;
    agent[id].food_category = rand(0, 1);
    agent[id].food_bar = 0;
    agent[id].rotation = rand(0,360);
    agent[id].f_left = 0;
    agent[id].f_right = 0;
    agent[id].X = rand(agent_texture.width, SCREEN_WIDTH - agent_texture.width);
    agent[id].Y = rand(agent_texture.height, SCREEN_HEIGHT - agent_texture.height);
    agent[id].brain.init();
}

AGENT& AGENT::operator=(const AGENT& temp) noexcept {
    energy = temp.energy;
    food_category = temp.food_category;
    food_bar = temp.food_bar;
    rotation = temp.rotation;
    f_left = temp.f_left;
    f_right = temp.f_right;
    X = temp.X;
    Y = temp.Y;
    boost_strenght = temp.boost_strenght;
    brain = temp.brain;
    speed = temp.speed;
    return *this;
}

SDL_Color food_color(const uint16_t id) {
    switch (agent[id].food_category) {
        case AGENT_HERBIVORE:
            return SDL_GREEN;
            break;
        case AGENT_CARNIVORE:
            return SDL_BLUE;
            break;
    }
    return SDL_WHITE;
}
bool agent_intersction(const uint16_t ID1, const uint16_t ID2) {
    COORD C1 = getcenter(ID1);
    COORD C2 = getcenter(ID2);
    return distance(&C1, &C2) < agent_texture.width;
}
void moveagent(const uint16_t ID) {
    _PRECISION acceleration = 2*std::min(agent[ID].f_left, agent[ID].f_right) / AGENT_MASS;
    acceleration *= (1 + agent[ID].boost_strenght);
    _PRECISION moveX = -std::min(acceleration, MAX_SPEED)* sin(sex_rad(agent[ID].rotation));
    _PRECISION moveY = +std::min(acceleration, MAX_SPEED)* cos(sex_rad(agent[ID].rotation));

    agent[ID].X = agent[ID].X + moveX;
    agent[ID].Y = agent[ID].Y - moveY;
    agent[ID].rotation = mod360(agent[ID].rotation + rad_sex((agent[ID].f_right - agent[ID].f_left) / (agent_texture.width * AGENT_MASS)));

    if (getcenter(ID).X < 0)
        agent[ID].X = SCREEN_WIDTH -1 -AGENT_RADIUS;
    if (getcenter(ID).Y < 0)
        agent[ID].Y = SCREEN_HEIGHT -1 -AGENT_RADIUS;
    if (getcenter(ID).X > SCREEN_WIDTH)
        agent[ID].X = 0;
    if (getcenter(ID).Y > SCREEN_HEIGHT)
        agent[ID].Y = 0;
    //agnt->speed = acceleration + agnt->speed;
    //agnt->boost_strenght -= distance;

}
int32_t getcellX(const uint16_t ID) {
    return getcenter(ID).X / SQUARE_SIZE;
}
int32_t getcellY(const uint16_t ID) {
    return getcenter(ID).Y / SQUARE_SIZE;
}

void crunch(const uint16_t id, const _PRECISION strenght) {
    agent[id].energy -= CRUNCH_ENERGY * strenght;
    int32_t cellX = getcellX(id);
    int32_t cellY = getcellY(id);
    if (area[cellY][cellX] > 0) {
        area[cellY][cellX] -= strenght;
        if (area[cellY][cellX] < 0)
            area[cellY][cellX] = 0;
        agent[id].food_bar = bound(agent[id].food_bar + VEGETABLE_ENERGY * strenght, 0, MAX_FOOD_BAR);
    }
}

void blood_sensor(const uint16_t ID, _PRECISION* sensors, _PRECISION* distances) {
    uint16_t target_id = ID^1;
    COORD P = getcenter(ID);
    COORD Min;
    uint16_t i = 0;
    do{
        if ((i != ID) && (distance(getcenter(i), P) < distance(getcenter(target_id), P)))
            target_id = i;

        ++i;
    }while (i < POPULATION_COUNT);
    for (i = 0; i < FOOD_SENSORS; ++i) {
        _PRECISION direction = sex_rad(sensors[i]);
        draw_sensor(ID, direction, AGENT_BLOOD_RADIUS, food_color(ID));
        COORD Sensor = {P.X + AGENT_BLOOD_RADIUS*cos(direction), P.Y - AGENT_BLOOD_RADIUS*sin(direction) };
        distances[i] = std::min(distance(&Min, &Sensor) / AGENT_BLOOD_RADIUS, 1.);
    }
    return ;
}
void grass_sensor(const uint16_t ID, _PRECISION sensor[3], _PRECISION distances[3]) {
    COORD near = {0, 0};

    for (int y = 0; y < AREA_HEIGHT; ++y)
        for (int x = 0; x < AREA_WIDTH; ++x)
            if (int(area[y][x]) > 0)
                if (distance(near, getcenter(ID)) > distance({x + SQUARE_SIZE/2., y + SQUARE_SIZE/2.}, getcenter(ID))) {
                    near.X = x + SQUARE_SIZE/2;
                    near.Y = y + SQUARE_SIZE/2;
                }
    for (int i=0; i < 3; ++i) {
        _PRECISION direction = sex_rad(sensor[i]);
        draw_sensor(ID, direction, AGENT_GRASS_RADIUS, SDL_GREEN);
        distances[i] = std::min(uint16_t(dist_sensor(ID, direction, AGENT_GRASS_RADIUS, near)), AGENT_GRASS_RADIUS);
    }
    return;
}
void food_sensor(const uint16_t id, _PRECISION distances[3]) {
    // find the nearest source of food and returns the distances from the sensors
    _PRECISION sensors[3] = {61, 119, 270};
    for (int i=0; i < 3; ++i)
        sensors[i] = mod360(agent[id].rotation + sensors[i]);

    for (uint8_t i = 0; i < 3; ++i)
        switch(agent[id].food_category) {
            case AGENT_HERBIVORE:
                grass_sensor(id, sensors, distances);
                break;
            case AGENT_CARNIVORE:
                blood_sensor(id, sensors, distances);
                break;
            default:
                break;
        }
    return;
}

void bite(const uint16_t id, const _PRECISION strenght) {
    agent[id].energy -= BITE_ENERGY * strenght;

    int16_t nearest = -1;
    _PRECISION min_dist = AGENT_BITE_RADIUS;
    _PRECISION direction = sex_rad(agent[id].rotation + 90);

    COORD P = getcenter(id);
    #ifdef DEBUG
    draw_sensor(id, direction, AGENT_BITE_RADIUS, SDL_PINK);
    #endif // DEBUG

    for (int i = 0; i <POPULATION_COUNT; ++i) {
        _PRECISION act_dist = distance(getcenter(i), getcenter(id));
        if (i == id)
            continue;
        if (act_dist - agent_texture.width / 2 > min_dist)
            continue;
        COORD T = getcenter(i);
        _PRECISION add = mod2PI(asin((agent_texture.width / 2) / sqrt(pow(act_dist, 2) + pow(agent_texture.width/2, 2))));
        _PRECISION angle = getangle((P.Y-T.Y) /act_dist, (T.X - P.X)/act_dist);
        _PRECISION a1 = mod2PI(direction - add);
        _PRECISION a2 = mod2PI(direction + add);
        if ((a1 < a2) && (a2 >= angle && angle >= a1)) {
            nearest = i;
            min_dist = act_dist;
        }
        else
        if ((a1 > a2) && ((a1 <= angle) ^ (a2 >= angle))) {
            nearest = i;
            min_dist = act_dist;
        }
    }

    if (nearest != -1) {
        agent[nearest].energy -= BITE_DAMAGE * strenght;
        agent[id].food_bar = bound(MEAT_ENERGY * strenght + agent[id].food_bar, 0, MAX_FOOD_BAR);
    }
    return ;
}

bool in_agent(COORD P, AGENT* _agent) {
    return pow((P.X - _agent->X), 2) + pow((P.Y - _agent->Y), 2) <= pow(agent_texture.width/2, 2);
}


COORD getcenter(const uint16_t ID) {
    return {agent[ID].X + agent_texture.width/2, agent[ID].Y + agent_texture.height/2};
}
SDL_Color look(const uint16_t id, const _PRECISION direction, const _PRECISION alpha) {
    COORD P = getcenter(id);
    SDL_Color middle = {0, 0, 0, AGENT_SEEN_RADIUS};

    _PRECISION ang1 = direction + alpha/2;
    _PRECISION ang2 = direction - alpha/2;
    draw_sensor(id, ang1, AGENT_SEEN_RADIUS, SDL_RED);
    draw_sensor(id, ang2, AGENT_SEEN_RADIUS, SDL_RED);

    _PRECISION angle = 255;
    ang1 = mod2PI(ang1);
    ang2 = mod2PI(ang2);

    for (int i = 0; i < POPULATION_COUNT; ++i) {
        if (i == id )
            continue;

        COORD T = getcenter(i) ;
        _PRECISION act_dist = distance(&P, &T);

        if (act_dist  - agent_texture.width / 2< middle.a){
            angle = getangle((P.Y-T.Y) /act_dist, (T.X - P.X)/act_dist) ;//modX(asin(abs(T.Y - P.Y) / distance(&P, &T)), 2*PI);
            _PRECISION add = mod2PI(asin((agent_texture.width/2) / sqrt(pow(act_dist, 2) + pow(agent_texture.width / 2, 2))));
            _PRECISION ang4 = mod2PI(ang1 + add);
            _PRECISION ang5 = mod2PI(ang2 - add);
            if (((ang4 >= angle && angle >= ang5) && (ang4 > ang5)) || (((ang5 <= angle) ^ (ang4 >= angle)) && (ang5 > ang4))) {
                middle.a = act_dist - agent_texture.width / 2;
                middle.r = AGENT_RED;
                middle.g = AGENT_GREEN;
                middle.b = AGENT_BLUE;
            }
        }
    }
    return middle;
}

void draw_sensor(const uint16_t id, _PRECISION direction, _PRECISION lenght, SDL_Color C) {
    COORD P = getcenter(id);
    COORD T = {P.X + lenght * cos(direction), P.Y - lenght * sin(direction)};
    SDL_SetRenderDrawColor(renderer, C.r, C.g, C.b, C.a);
    SDL_RenderDrawLine(renderer, P.X, P.Y, T.X, T.Y);
}

_PRECISION dist_sensor(const uint16_t id, _PRECISION direction, _PRECISION lenght, COORD T) {
    COORD P = getcenter(id);
    COORD E = {P.X + lenght * cos(direction), P.Y - lenght * sin(direction)};
    return distance(T, E);
}

int32_t smell(const uint16_t id) {
    COORD center1  = getcenter(id);
    COORD center2;
    int32_t neightbours = 0;
    for (int i=0; i < POPULATION_COUNT; ++i) {
        center2 = getcenter(id);
        if ( distance(&center1, &center2 ) <= AGENT_SEEN_RADIUS + agent_texture.width);
            ++neightbours;


    }
    return neightbours;
}

void give_agent_input(const uint16_t id) {
    _PRECISION food_distances[3];
    food_sensor(id, food_distances);
    SDL_Color eye1 = look(id, sex_rad( 61 + agent[id].rotation), sex_rad(30));
    SDL_Color eye2 = look(id, sex_rad(119 + agent[id].rotation), sex_rad(30));
    SDL_Color eye3 = look(id, sex_rad(270 + agent[id].rotation), sex_rad(60));
    _PRECISION health  = _PRECISION(agent[id].energy) / MAX_HEALTH;
    _PRECISION neightbours_number = _PRECISION(smell(id)) / MAX_POPULATION;

    _PRECISION inputs[INPUT_CELLS] = {
        food_distances[0],
        food_distances[1],
        food_distances[2],
        eye1.r / 255.,
        1 - _PRECISION(eye1.a) / AGENT_SEEN_RADIUS,
        eye2.r / 255.,
        1 - _PRECISION(eye2.a) / AGENT_SEEN_RADIUS,
        eye3.r / 255.,
        1 - _PRECISION(eye3.a) / AGENT_SEEN_RADIUS,
        health,
        neightbours_number,
    };
    agent[id].brain.input(inputs);
}


void execute_agent_input(const uint16_t id) {
    agent[id].brain.execute();
}

void execute_agent_output(const uint16_t id) {
    agent[id].brain.output(id);
}
void AGENT::destroy() {
    brain.destroy();
    X = 0;
    Y = 0;
    energy = 0;
    rotation = 0;
    speed = 0;
    food_bar = 0;
    food_category = 0;
    f_left = 0;
    f_right = 0;
    boost_strenght = 0;
    col = {0,0,0,0};
}
