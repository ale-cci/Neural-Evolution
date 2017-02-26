#include <cstdlib>
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


void moveagent(AGENT* agnt) {
    //int distance = log2(agnt->boost_strenght);m
    double acceleration = 2*std::min(agnt->f_left, agnt->f_right) / AGENT_MASS;
    acceleration *= (1 + agnt->boost_strenght);
    double moveX = -std::min(acceleration, MAX_SPEED)* sin(agnt->rotation * PI / 180);
    double moveY = +std::min(acceleration, MAX_SPEED)* cos(agnt->rotation * PI/180);

    agnt->X = agnt->X + moveX;
    agnt->Y = agnt->Y - moveY;
    agnt->rotation = agnt->rotation + ((agnt->f_right - agnt->f_left) / (agent_texture.width * AGENT_MASS)) * 180/PI;
    agnt->rotation = mod360(agnt->rotation);

    if (getcenter(agnt).X < 0)
        agnt->X = SCREEN_WIDTH -1 - agent_texture.width/2;
    if (getcenter(agnt).Y < 0)
        agnt->Y = SCREEN_HEIGHT -1 -agent_texture.height/2;
    if (getcenter(agnt).X > SCREEN_WIDTH)
        agnt->X = 0;
    if (getcenter(agnt).Y > SCREEN_HEIGHT)
        agnt->Y = 0;
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
void bite(const uint16_t ID, const double strenght) {
    //warning("BITE", "Biting...");
    agent[ID].energy -= BITE_ENERGY * strenght;
    if (agent[ID].digesting)
        return ;
    uint8_t number_of_collisions = 0;
    double direction = (agent[ID].rotation + 90) * PI / 180;
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
    double ang1 = direction + alpha/2;
    double ang2 = direction - alpha/2;
    SDL_Color bound1 = segment_look(id, ang1);
    SDL_Color bound2 = segment_look(id, ang2);
    SDL_Color middle = {0, 0, 0, 255};

    if (bound1.a < bound2.a)
        return bound1;
    else
    if (bound2.a != 255)
        return bound2;

    double act_dist, m_line, angle;
    ang1 = modX(ang1, PI);
    ang2 = modX(ang2, PI);

    COORD F = {P.X + AGENT_SEEN_RADIUS * cos(direction), -tan(direction) * (P.X + AGENT_SEEN_RADIUS * cos(direction)) + (tan(direction) * P.X +P.Y)};
    for (int i = 0; i < POPULATION_COUNT; ++i) {
        if (i == id )
            continue;
        COORD T = getcenter(&agent[i]) ;
        act_dist = distance(&P, &T) - agent_texture.width / 2;

        if ((act_dist < middle.a) && (act_dist < AGENT_SEEN_RADIUS)){
            // check if the circumference is contained in the range
            //if (distance(T, P) )
            m_line = -(P.Y - T.Y) / (P.X - T.X);
            angle = modX(atan(m_line), PI);
            uint8_t behind = (distance(&P, &F) <= distance(&T, &F));
            if ((angle > ang1 && angle < ang2) || (angle < ang1 && angle > ang2))
                    if (!behind) {
                        middle.a = act_dist;
                        middle.r = AGENT_RED;
                        middle.g = AGENT_GREEN;
                        middle.b = AGENT_BLUE;
                    }
        }
    }

    return middle;
}
SDL_Color segment_look(const uint16_t id, double direction) {
    COORD P = getcenter(&agent[id]);
    double m_line = -tan(direction);
    double q_line = +tan(direction) * P.X +P.Y;
    //std::cout << direction * 180 / PI << std::endl;
    COORD EYE = {P.X + AGENT_SEEN_RADIUS * cos(direction), m_line * (P.X + AGENT_SEEN_RADIUS * cos(direction)) + q_line};
    SDL_Color result = {0, 0, 0, 255};

    #ifdef DEBUG
    SDL_SetRenderDrawColor(renderer, 0xff, 0, 0, 0);
    SDL_RenderDrawLine(renderer, P.X, P.Y, EYE.X, EYE.Y);
    #endif // DEBUG*/
    COORD collisions[2];
    uint8_t number_of_collisions;
    double dist;
    for (int i = 0; i <POPULATION_COUNT; ++i) {
        if (i == id)
            continue;
        line_circle_collision(m_line, q_line, getcenter(&agent[i]).X, getcenter(&agent[i]).Y, agent_texture.width/2, collisions, &number_of_collisions);

        if (number_of_collisions == 0)
            continue;
        dist = std::min(distance(&collisions[0], &P), distance(&collisions[1], &P));

        if (dist  <= AGENT_SEEN_RADIUS+ agent_texture.width / 2) {
            uint8_t behind = (distance(&P, &EYE) <= distance(&collisions[0], &EYE));
            if (behind)
                continue;
            if (dist < result.a) {
                result.a = dist;
                result.r = AGENT_RED;
                result.g = AGENT_GREEN;
                result.b = AGENT_BLUE;
            }
        }
    }
    return result;
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
    SDL_Color eye_1  = look(id, ( 61 + agent[id].rotation )* PI / 180, 30 * PI / 180);
    SDL_Color eye_2  = look(id, (119 + agent[id].rotation )* PI / 180, 30 * PI / 180);
    SDL_Color eye_3  = look(id, (270 + agent[id].rotation )* PI / 180, 30 * PI / 180);
   // bool collision = in_agent({x + _agent->X + agent_texture.width / 2, y + _agent->Y + agent_texture.height / 2}, _agent);
    //std::cout << (int) eye_1.a << std::endl;
    _PRECISION inputs[INPUT_CELLS] = {
            food / MAX_FOOD_IN_AREA, health / MAX_HEALTH, neightbours / MAX_POPULATION,
            eye_1.r / 255.0, eye_1.g / 255.0, eye_1.b / 255.0, 1 - eye_1.a / AGENT_SEEN_RADIUS,
            eye_2.r / 255.0, eye_2.g / 255.0, eye_2.b / 255.0, 1 - eye_2.a / AGENT_SEEN_RADIUS,

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
