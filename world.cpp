#include <cstdlib>
#include "debug.h"
#include "release.h"
#include "world.h"
#include "mysdl.h"
#include "agent.h"

int8_t area[AREA_HEIGHT][AREA_WIDTH];
int POPULATION_COUNT = START_POPULATION;
AGENT agent[MAX_POPULATION];

void printworld() {
    // print background color;
    // for print cell

    for (int y = 0; y < AREA_HEIGHT; ++y)
        for (int x = 0; x < AREA_WIDTH; ++x) {
            uint16_t intensity = (256  * area[y][x] / MAX_FOOD_IN_AREA);
            if (intensity >= 256)
                intensity = 255;
            SDL_SetRenderDrawColor(renderer, 0, intensity, 0, 0xFF);
            SDL_Rect rekt = {x * SQUARE_SIZE, y * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE};
            SDL_RenderFillRect(renderer, &rekt);
        }
}

void init_food() {
    for (int y = 0; y < AREA_HEIGHT; ++y)
        for (int x = 0; x < AREA_WIDTH; ++x) {
            area[y][x] = rand() % MAX_FOOD_IN_AREA;
            if (area[y][x] < 0)
                area[y][x] = 0;
        }
    return ;
}

void init_agents() {
    for (int i = 0; i < START_POPULATION; ++i)
        agent[i] = *makeagent();
}

void print_agents() {
    for (int i=0; i < POPULATION_COUNT; ++i)
        printagent(&agent[i]);
}

void move_agents() {
    for (int i=0; i < POPULATION_COUNT; ++i)
        moveagent(&agent[i]);

}
