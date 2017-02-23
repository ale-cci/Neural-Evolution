#pragma once
#include <inttypes.h>
#include "neural_network_constant.h"
#include "neuron.h"
// TODO: Fix the number of neurons
// TODO: check the output range of each cell
// TODO: create the output neuron class, child of neuron, to create a recurrent nn
class NEURAL_NETWORK {
    private:
        NEURON input_layer[INPUT_CELLS];
        NEURON hidden_layer[NUMBER_OF_LAYERS][NEURONS_PER_LAYER];
        NEURON output_layer[OUTPUT_CELLS];
    public:
        void input(_PRECISION *in);
        void execute();
        void init();
        void output(struct AGENT* agent);
        void save(std::ofstream& file);
        void load(std::ifstream& file);
};
