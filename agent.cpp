#include <cstdlib>
#include <cassert>
#include <iostream>
#include <cmath>
#include "generic_functions.h"
#include "world.h"
#include "mysdl.h"
#include "extmath.h"
#include "agent.h"

SDL_Image agent_texture;

AGENT makeagent() {
    AGENT agent;
    agent_texture = *load_trsp("IMAGES/debug01.bmp");
    agent.energy = MAX_HEALTH;
    agent.food_category = rand(0, 1);
    agent.food_bar = 0;
    agent.rotation = rand(0,360);
    agent.f_left = 0;
    agent.f_right = 0;
    agent.digesting = 0;
    agent.X = rand(agent_texture.width, SCREEN_WIDTH - agent_texture.width);
    agent.Y = rand(agent_texture.height, SCREEN_HEIGHT - agent_texture.height);
    return agent;
}

SDL_Color food_color(const uint16_t id) {
    switch (agent[id].food_category) {
        case AGENT_HERBIVORE:
            return SDL_GREEN;
            break;
        case AGENT_CARNIVORE:
            return SDL_BLUE;
1            break;
    }
    return SDL_WHITE;
}
bool agent_intersction(const uint16_t ID1, const uint16_t ID2) {
    COORD C1 = getcenter(&agent[ID1]);
    COORD C2 = getcenter(&agent[ID2]);
    return distance(&C1, &C2) < agent_texture.width;
}
void moveagent(const uint16_t ID) {
    //int distance = log2(agnt->boost_strenght);m
    COORD backup = {agent[ID].X, agent[ID].Y};
    double acceleration = 2*std::min(agent[ID].f_left, agent[ID].f_right) / AGENT_MASS;
    acceleration *= (1 + agent[ID].boost_strenght);
    double moveX = -std::min(acceleration, MAX_SPEED)* sin(agent[ID].rotation * PI / 180);
    double moveY = +std::min(acceleration, MAX_SPEED)* cos(agent[ID].rotation * PI/180);

    agent[ID].X = agent[ID].X + moveX;
    agent[ID].Y = agent[ID].Y - moveY;
    agent[ID].rotation = agent[ID].rotation + ((agent[ID].f_right - agent[ID].f_left) / (agent_texture.width * AGENT_MASS)) * 180/PI;
    agent[ID].rotation = mod360(agent[ID].rotation);

    if (getcenter(&agent[ID]).X < 0)
        agent[ID].X = SCREEN_WIDTH -1 - agent_texture.width/2;
    if (getcenter(&agent[ID]).Y < 0)
        agent[ID].Y = SCREEN_HEIGHT -1 -agent_texture.height/2;
    if (getcenter(&agent[ID]).X > SCREEN_WIDTH)
        agent[ID].X = 0;
    if (getcenter(&agent[ID]).Y > SCREEN_HEIGHT)
        agent[ID].Y = 0;
    /*
    for (int i=0; i <POPULATION_COUNT; ++i) {
        if (i == ID)
            continue;
        if (agent_intersction(ID, i)) {
            agent[ID].X = backup.X;
            agent[ID].Y = backup.Y;
            break;
        }
    }*/
    //std::cout << moveX << " " << moveY << " " << acceleration << std::endl;
    //agnt->speed = acceleration + agnt->speed;
    //agnt->boost_strenght -= distance;

}
int32_t getcellX(AGENT * _agent) {
    return getcenter(_agent).X / SQUARE_SIZE;
}
int32_t getcellY(AGENT * _agent) {
    return getcenter(_agent).Y / SQUARE_SIZE;
}

void crunch(const uint16_t id, const double strenght) {
    if (agent[id].digesting) {
        agent[id].energy -= CRUNCH_ENERGY * strenght;
        return ;
    }
    int32_t cellX = getcellX(&agent[id]);
    int32_t cellY = getcellY(&agent[id]);
    if (area[cellY][cellX] > 0) {
        area[cellY][cellX] -= strenght;
        if (area[cellY][cellX] < 0)
            area[cellY][cellX] = 0;
        agent[id].digesting = VEGETABLE_DIGEST_TIME * strenght;
        agent[id].food_bar += VEGETABLE_ENERGY * strenght;
    }
}

void blood_sensor(const uint16_t id, double* distances) {
    double sensors[FOOD_SENSORS]= {61, 119, 270};
    uint16_t target_id = id^1;
    COORD P = getcenter(&agent[id]);
    COORD Min;
    uint16_t i = 0;
    do {
        Min = getcenter(&agent[target_id]);
        COORD Tmp = getcenter(&agent[i]);
        if ((i != id) && (distance(&Tmp, &P) < distance(&Min, &P))) {
            target_id = i;
        }
        ++i;
    }while (i < POPULATION_COUNT);
    for (i = 0; i < FOOD_SENSORS; ++i) {
        double direction = sex_rad(sensors[i] + agent[id].rotation);
        draw_sensor(id, direction, AGENT_BLOOD_RADIUS, food_color(id));
        COORD Sensor = {P.X + AGENT_BLOOD_RADIUS*cos(direction), P.Y - AGENT_BLOOD_RADIUS*sin(direction) };
        distances[i] = std::min(distance(&Min, &Sensor) / AGENT_BLOOD_RADIUS, 1.);
    }
    return ;
}
void grass_sensor(const uint16_t id, double *distances) {
    for (int i=0; i < FOOD_SENSORS; ++i)
        distances[i] = 0;
    return;
}
void food_sensor(const uint16_t id, double *distances) {
    // find the nearest source of food and returns the distances from the sensors
    double sensors[3] = {61, 119, 270};
    switch(agent[id].food_category) {
        case AGENT_HERBIVORE:
            grass_sensor(id, distances);
            break;
        case AGENT_CARNIVORE:
            blood_sensor(id, distances);
            break;
        default:
            break;
    }
    return;
}
void bite(const uint16_t ID, const double strenght) {
    //warning("BITE", "Biting...");
    agent[ID].energy -= BITE_ENERGY * strenght;
    if (agent[ID].digesting)
        return ;
    uint8_t number_of_collisions = 0;
    double direction = sex_rad(agent[ID].rotation + 90);
    COORD collisions[2];
    COORD P = getcenter(&agent[ID]);
    COORD MOUTH = {P.X + AGENT_BITE_RADIUS * cos(direction), -AGENT_BITE_RADIUS*sin(direction) + P.Y};
    double m_line = tan(direction);
    double q_line = -tan(direction) * P.X + P.Y;
    #ifdef DEBUG
    SDL_SetRenderDrawColor(renderer, 0xff, 0, 0xff, 0);
    SDL_RenderDrawLine(renderer, P.X, P.Y, MOUTH.X, MOUTH.Y);
    #endif // DEBUG
    for (int i = 0; i <POPULATION_COUNT; ++i) {
        if (i == ID)
            continue;
        line_circle_collision(m_line, q_line, getcenter(&agent[i]).X, getcenter(&agent[i]).Y, agent_texture.width/2, collisions, &number_of_collisions);
        if (number_of_collisions == 0)
            continue;
        double dist = std::min(distance(&collisions[0], &P), distance(&collisions[1], &P));
        if (dist <= AGENT_BITE_RADIUS ) {
            uint8_t behind = (distance(&P, &MOUTH) <= distance(&collisions[0], &MOUTH));
            if (behind)
                continue;
            warning("BITE", "YUMMY!");
            agent[i].energy -= BITE_DAMAGE * strenght;
            agent[ID].digesting = MEAT_DIGEST_TIME * strenght;
            agent[ID].food_bar += (MEAT_ENERGY * strenght);
            return ;
        }
    }
}

bool in_agent(COORD P, AGENT* _agent) {
    return pow((P.X - _agent->X), 2) + pow((P.Y - _agent->Y), 2) <= pow(agent_texture.width/2, 2);
}


COORD getcenter(AGENT* _agent) {
    return {_agent->X + agent_texture.width/2, _agent->Y + agent_texture.height/2};
}
SDL_Color look(const uint16_t id,const double direction, const double alpha) {
    COORD P = getcenter(&agent[id]);
    SDL_Color middle = {0, 0, 0, 255};

    double ang1 = direction + alpha/2;
    double ang2 = direction - alpha/2;
    draw_sensor(id, ang1, AGENT_SEEN_RADIUS, SDL_RED);
    draw_sensor(id, ang2, AGENT_SEEN_RADIUS, SDL_RED);

    double angle = 255;
    ang1 = mod2PI(ang1);
    ang2 = mod2PI(ang2);

    for (int i = 0; i < POPULATION_COUNT; ++i) {
        if (i == id )
            continue;

        COORD T = getcenter(&agent[i]) ;
        double act_dist = distance(&P, &T);
        if (act_dist - agent_texture.width / 2 > AGENT_SEEN_RADIUS)
            continue;

        if (act_dist  - agent_texture.width / 2< middle.a){
            angle = getangle((P.Y-T.Y) /act_dist, (T.X - P.X)/act_dist) ;//modX(asin(abs(T.Y - P.Y) / distance(&P, &T)), 2*PI);
            double add = mod2PI(asin((agent_texture.width/2) / sqrt(pow(act_dist, 2) + pow(agent_texture.width / 2, 2))));
            double ang4 = mod2PI(ang1 + add);
            double ang5 = mod2PI(ang2 - add);
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
void draw_sensor(const uint16_t id, double direction, double lenght, SDL_Color C) {
    COORD P = getcenter(&agent[id]);
    COORD T = {P.X + lenght * cos(direction), P.Y - lenght * sin(direction)};
    //std::cout << direction * 180 / PI << std::endl;
    SDL_SetRenderDrawColor(renderer, C.r, C.g, C.b, C.a);
    SDL_RenderDrawLine(renderer, P.X, P.Y, T.X, T.Y);
}

int32_t smell(AGENT* _agent) {
    COORD center1  = getcenter(_agent);
    COORD center2;
    int32_t neightbours = 0;
    for (int i=0; i < POPULATION_COUNT; ++i) {
        center2 = getcenter(&agent[0]);
        if ( distance(&center1, &center2 ) <= AGENT_SEEN_RADIUS + agent_texture.width);
            ++neightbours;


    }
    return neightbours;
}

void give_agent_input(const uint16_t id) {

    double food = area[getcellY(&agent[id])][getcellX(&agent[id])],
        neightbours = smell(&agent[id]);

    double health = agent[id].energy;
    double foodeyeleft, foodeyeright;
    double digesting_complete = 1 - (double)agent[id].digesting / std::max(MEAT_DIGEST_TIME, VEGETABLE_DIGEST_TIME);
    // blood_sensor(id, sex_rad( 61 + agent[id].rotation));
    // blood_sensor(id, sex_rad(119 + agent[id].rotation));
    // blood_sensor(id, sex_rad(270 + agent[id].rotation));
    double food_sensors[FOOD_SENSORS];
    food_sensor(id, food_sensors);
    SDL_Color eye_1  = look(id, sex_rad( 61 + agent[id].rotation ), sex_rad(30));
    SDL_Color eye_2  = look(id, sex_rad(119 + agent[id].rotation ), sex_rad(30));
    //SDL_Color eye_3  = look(id, (270 + agent[id].rotation )* PI / 180, 30 * PI / 180);
   // bool collision = in_agent({x + _agent->X + agent_texture.width / 2, y + _agent->Y + agent_texture.height / 2}, _agent);
    //std::cout << (int) eye_1.a << std::endl;
    _PRECISION inputs[INPUT_CELLS] = {
            food / MAX_FOOD_IN_AREA, health / MAX_HEALTH, neightbours / MAX_POPULATION,
            eye_1.r / 255.0, eye_1.g / 255.0, eye_1.b / 255.0, 1 - eye_1.a / AGENT_SEEN_RADIUS,
            food_sensors[0], food_sensors[1], food_sensors[2],
      //      eye_2.r / 255.0, eye_2.g / 255.0, eye_2.b / 255.0, 1 - eye_2.a / AGENT_SEEN_RADIUS,

            digesting_complete,
            };
    agent[id].brain.input(inputs);
}


void execute_agent_input(const uint16_t id) {
    agent[id].brain.execute();
}

void execute_agent_output(const uint16_t id) {
    agent[id].brain.output(id);
}
