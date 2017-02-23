#include "neural_network.h"

void NEURAL_NETWORK::input(_PRECISION *in) {
    for (int i = 0; i < INPUT_CELLS; ++i)
        input_layer[i].input(in[i]);
}

void NEURAL_NETWORK::output(struct AGENT* agent) {
    // TODO: connect output layer
}

void NEURAL_NETWORK::init() {
    for (int i = 0; i < INPUT_CELLS; ++i)
        input_layer[i].init(hidden_layer[0]);
    for (int l = 0; l < NUMBER_OF_LAYERS; ++l)
        for (int i = 0; i < NEURONS_PER_LAYER; ++i)
            if (l == NUMBER_OF_LAYERS -1)
                hidden_layer[l][i].init(output_layer);
            else
                hidden_layer[l][i].init(hidden_layer[l+1]);
}

void NEURAL_NETWORK::execute() {
    for (int i = 0; i < INPUT_CELLS; ++i)
        input_layer[i].init(hidden_layer[0]);
    for (int l = 0; l < NUMBER_OF_LAYERS; ++l)
        for (int i = 0; i < NEURONS_PER_LAYER; ++i)
            if (l == NUMBER_OF_LAYERS -1)
                hidden_layer[l][i].init(output_layer);
            else
                hidden_layer[l][i].init(hidden_layer[l+1]);
}

void NEURAL_NETWORK::save(std::ofstream& out) {
    for (int i = 0; i < INPUT_CELLS; ++i)
        input_layer[i].save(out);
    for (int l = 0; l < NUMBER_OF_LAYERS; ++l)
        for (int i = 0; i < NEURONS_PER_LAYER; ++i)
            hidden_layer[l][i].save(out);
}

void NEURAL_NETWORK::load(std::ifstream& in) {
    for (int i = 0; i < INPUT_CELLS; ++i)
        input_layer[i].load(in);
    for (int l = 0; l < NUMBER_OF_LAYERS; ++l)
        for (int i = 0; i < NEURONS_PER_LAYER; ++i)
            hidden_layer[l][i].load(in);
}
