#pragma once
// TODO: Fix the number of neurons
const static uint8_t INPUT_CELLS     = 18;
const static uint8_t OUTPUT_CELLS    = 18;
const static uint8_t NEURONS_PER_LAYER  = 18;
const static uint8_t NUMBER_OF_LAYERS   = 4;
typedef double DATATYPE;

class NEURAL_NETWORK {
    private:
        NEURON input_later[INPUT_CELLS];
        NEURON hidden_layer[NUMBER_OF_LAYERS][NEURONS_PER_LAYER];
        NEURON output_layer[OUTPUT_CELLS]
    public:
        void input(DATATYPE in[INPUT_CELLS]);
        void output();
};
