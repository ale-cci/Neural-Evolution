#pragma once
#include "mysdl.h"
const static int SQUARE_SIZE = 32;
const static int MAX_FOOD_IN_AREA = 8;

const static int AREA_WIDTH = SCREEN_WIDTH / SQUARE_SIZE;
const static int AREA_HEIGHT = SCREEN_HEIGHT / SQUARE_SIZE;

extern int8_t area[AREA_WIDTH][AREA_HEIGHT];

void init_food();
void printworld(uint8_t ALPHA);
