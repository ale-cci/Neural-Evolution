#include "neural_network.h"
#include <iostream>
#include <cmath>
#include "world.h"

void NEURAL_NETWORK::input(_PRECISION *in) {
    refresh();
    for (int i = 0; i < INPUT_CELLS; ++i)
        input_layer.neuron[i].input(in[i]);
    for (int i = 0; i < OUTPUT_CELLS; ++i)
        input_layer.neuron[i+ INPUT_CELLS].input(in[i + INPUT_CELLS]);
}

void NEURAL_NETWORK::output(const uint8_t id) {
    // TODO: connect output layer
    for (int i = 0; i< OUTPUT_CELLS; ++i)
        last_output[i] = output_layer.neuron[i].spread_value();

    switch (agent[id].food_category) {
        case 0:
            crunch(id, last_output[0]); // eat
            break;
        case 1:
            bite(id, last_output[0]);
            break;
        case 2:
            break;
    }

    agent[id].f_left = MAX_STRENGHT * last_output[1];
    agent[id].f_right = MAX_STRENGHT * last_output[2];
    /*
    agent[id].col.R = 255*last_output[3];
    agent[id].col.G = 255*last_output[4];
    agent[id].col.B = 255*last_output[5];
    */
    //if (last_output[4] > 1 - agent[id].food_category / MAX_FOOD_CATEGORY);

    //std::cout << (int)id << " " << output_layer.neuron[0].input_sum / output_layer.neuron[0].input_nums << std::endl;
}
void NEURAL_NETWORK::init() {
    warning("INIT_NEURAL_NETWORK", "Initializing...");
    output_layer.init(nullptr, OUTPUT_CELLS);
    hidden_layer[NUMBER_OF_LAYERS-1].init(&output_layer, NEURONS_PER_LAYER);
    for (int i=0; i< OUTPUT_CELLS; ++i)
        last_output[i] = 0;
    for (int l = NUMBER_OF_LAYERS -2; l >= 0; --l)
        hidden_layer[l].init(&hidden_layer[l+1], NEURONS_PER_LAYER);
    input_layer.init(&hidden_layer[0], INPUT_CELLS + OUTPUT_CELLS);
}
void NEURAL_NETWORK::refresh() {
    input_layer.refresh();
    for (int l = 0; l < NUMBER_OF_LAYERS; ++l)
        hidden_layer[l].refresh();
    output_layer.refresh();
}
void NEURAL_NETWORK::execute() {
    input_layer.execute(&hidden_layer[0]);
    for (int l = 0; l < NUMBER_OF_LAYERS-1; ++l)
        hidden_layer[l].execute(&hidden_layer[l+1]);
    hidden_layer[NUMBER_OF_LAYERS-1].execute(&output_layer);
}

void NEURAL_NETWORK::inherit(struct NEURAL_NETWORK* father) {
    input_layer.inherit_from(&father->input_layer, &father->hidden_layer[0]);
    for (int l = 0; l < NUMBER_OF_LAYERS-1; ++l)
        hidden_layer[l].inherit_from(&father->hidden_layer[l], &father->hidden_layer[l +1] );

    hidden_layer[NUMBER_OF_LAYERS -1].inherit_from(&father->hidden_layer[NUMBER_OF_LAYERS -1], &father->output_layer);
    output_layer.inherit_from(&father->output_layer, nullptr);
}

void NEURAL_NETWORK::mutate() {
    input_layer.mutate(&hidden_layer[0]);
    for (int l = 0; l < NUMBER_OF_LAYERS-1; ++l)
        hidden_layer[l].mutate(&hidden_layer[l+1]);
    hidden_layer[NUMBER_OF_LAYERS-1].mutate(&output_layer);
    output_layer.mutate(nullptr);
}

