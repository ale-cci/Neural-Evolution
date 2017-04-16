#pragma once
#include <inttypes.h>
#include "neural_network_constant.h"
#include "neuron.h"


struct NEURAL_NETWORK {

        LAYER input_layer;
        LAYER hidden_layer[NUMBER_OF_LAYERS];
        LAYER output_layer;
        double last_output[OUTPUT_CELLS];

        void input(_PRECISION *in); // puts the input in the input layer
        void execute(); // spread the input layer and send the output to the output_layer
        void init();     // calls the init function of all the layer
        void refresh();
        void output(const uint16_t id); // calls the output functions
        void duplicate(struct NEURAL_NETWORK* father);    // copy the data from father
        void mutate();  // mutate the neural network
        void checknan();

        LAYER* nlayer(const uint16_t layer_id);
        LAYER* getlayer(const uint16_t layer_id);
        LAYER* player(const uint16_t layer_id);

        void spread_data(const uint16_t layer_id, const double value);
        NEURAL_NETWORK& operator=(const NEURAL_NETWORK& ) noexcept;
        void destroy();
        void print(std::ofstream& out);
        void write(std::ofstream& out);
        void read(std::ifstream& in);
};
