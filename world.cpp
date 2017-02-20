#include <cstdlib>
#include "world.h"
#include "mysdl.h"

int8_t area[AREA_WIDTH][AREA_HEIGHT];

void printworld(uint8_t background_visible) {
    // print background color;
    // for print cell

    for (int y = 0; y < AREA_HEIGHT; ++y)
        for (int x = 0; x < AREA_WIDTH; ++x) {
            uint16_t intensity = (256  * area[y][x] / MAX_FOOD_IN_AREA);
            if (intensity >= 256)
                intensity = 255;
            SDL_SetRenderDrawColor(renderer, 0, intensity, 0, background_visible);
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
