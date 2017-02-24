#pragma once
#include <inttypes.h>
#include "neural_network_constant.h"
#include "neuron.h"
// TODO: Fix the number of neurons
// TODO: check the output range of each cell
// TODO: create the output neuron class, child of neuron, to create a recurrent nn

struct NEURAL_NETWORK {
        LAYER input_layer;
        LAYER hidden_layer[NUMBER_OF_LAYERS];
        LAYER output_layer;

        void input(_PRECISION *in); // puts the input in the input layer
        void execute(); // spread the input layer and send the output to the output_layer
        void init();     // calls the init function of all the layer
        void refresh();
        void output(const uint8_t id); // calls the output functions
        void save(std::ofstream& file); // save the neural network in a file
        void load(std::ifstream& file); // load the neural network from a file
        void inherit(struct NEURAL_NETWORK* father);    // copy the data from father
        void mutate();  // mutate the neural network
};
