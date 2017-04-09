#pragma once
#include <inttypes.h>

const static uint8_t INPUT_CELLS     = 11;
const static uint8_t OUTPUT_CELLS    = 5;
const static uint8_t NEURONS_PER_LAYER  = INPUT_CELLS;
const static uint8_t NUMBER_OF_LAYERS   = 3; // FIXME change to the real number of layer
const static uint8_t MUTATION_PERCENTAGE = 25;
const static float   MUTATION_CHANGE    = 0.02;
