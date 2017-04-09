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
        double last_output[OUTPUT_CELLS];
        uint8_t creating = true;
        void allocate(const uint8_t layer_id);

        void input(_PRECISION *in); // puts the input in the input layer
        void execute(); // spread the input layer and send the output to the output_layer
        void init();     // calls the init function of all the layer
        void refresh();
        void output(const uint8_t id); // calls the output functions
        void duplicate(struct NEURAL_NETWORK* father);    // copy the data from father
        void mutate();  // mutate the neural network

        LAYER* nlayer(const uint8_t layer_id);
        LAYER* getlayer(const uint8_t layer_id);
        LAYER* player(const uint8_t layer_id);

        void spread_data(const uint8_t layer_id, const double value);
        NEURAL_NETWORK& operator=(const NEURAL_NETWORK& )noexcept;
        void destroy();
};
