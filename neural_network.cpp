#include "neural_network.h"
#include <iostream>
#include <cmath>
#include "world.h"

void NEURAL_NETWORK::input(_PRECISION *in) {
    refresh();
    for (int i = 0; i < INPUT_CELLS; ++i)
        input_layer.neuron[i].input(in[i]);
}

void NEURAL_NETWORK::output(const uint8_t id) {
    // TODO: connect output layer
    if (output_layer.neuron[0].spread_value() > 0.4)
        crunch(id); // eat
    if (output_layer.neuron[1].spread_value() > 0.3); // boost
    agent[id].f_left = MAX_STRENGHT * output_layer.neuron[2].spread_value();
    agent[id].f_right = MAX_STRENGHT * output_layer.neuron[3].spread_value();

    if (output_layer.neuron[4].spread_value() > 0.4)
        bite(id);

    //std::cout << (int)id << " " << output_layer.neuron[0].input_sum / output_layer.neuron[0].input_nums << std::endl;
}
void NEURAL_NETWORK::init() {
    warning("INIT_NEURAL_NETWORK", "Initializing...");
    output_layer.init(nullptr, OUTPUT_CELLS);
    hidden_layer[NUMBER_OF_LAYERS-1].init(&output_layer, NEURONS_PER_LAYER);
    for (int l = NUMBER_OF_LAYERS -2; l >= 0; --l)
        hidden_layer[l].init(&hidden_layer[l+1], NEURONS_PER_LAYER);
    input_layer.init(&hidden_layer[0], INPUT_CELLS);
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

void NEURAL_NETWORK::save(std::ofstream& out) {
    input_layer.save(out, &hidden_layer[0]);
    for (int l = 0; l < NUMBER_OF_LAYERS - 1; ++l)
        hidden_layer[l].save(out, &hidden_layer[l+1] );
    hidden_layer[NUMBER_OF_LAYERS-1].save(out, &output_layer);
    output_layer.save(out, nullptr);
}

void NEURAL_NETWORK::load(std::ifstream& in) {
    input_layer.load(in, &hidden_layer[0]);
    for (int l = 0; l < NUMBER_OF_LAYERS-1; ++l)
        hidden_layer[l].load(in, &hidden_layer[l+1]);
    hidden_layer[NUMBER_OF_LAYERS-1].load(in, &output_layer);
    output_layer.load(in, nullptr);
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

