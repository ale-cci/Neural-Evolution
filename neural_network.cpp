#include "neural_network.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <cmath>
#include "world.h"

void NEURAL_NETWORK::input(_PRECISION *in) {
    refresh();
    for (int i = 0; i < INPUT_CELLS; ++i) {
        assert(!std::isnan(in[i]));
        input_layer.neuron[i].input(in[i]);
        assert(!std::isnan(input_layer.neuron[i].input_sum));
    }
    #ifdef DEBUG
    checknan();
    #endif
}

void NEURAL_NETWORK::output(const uint16_t id) {
    for (int i = 0; i< OUTPUT_CELLS; ++i) {
        last_output[i] = output_layer.neuron[i].spread_value();
        assert(!std::isnan(last_output[i]));
    }

    if (last_output[0] > 0) {
        if (agent[id].food_category == AGENT_HERBIVORE) {
                crunch(id, last_output[0]); // eat
        }
        else
        if (agent[id].food_category == AGENT_CARNIVORE) {
                bite(id, last_output[0]);
        }
    }

    agent[id].f_left = MAX_STRENGHT * last_output[1];
    agent[id].f_right = MAX_STRENGHT * last_output[2]; //*/

    agent[id].boost_strenght = 1 + last_output[3];
}
void NEURAL_NETWORK::init() {
    warning("INIT_NEURAL_NETWORK", "Initializing...");
    input_layer.init(0);
    for (int i=0; i < NUMBER_OF_LAYERS; ++i)
        hidden_layer[i].init(i+1);
    output_layer.init(NUMBER_OF_LAYERS+1);
}

void NEURAL_NETWORK::destroy() {
    warning("NEURAL_NETWORK_DESTROY", "Deallocating...");
    input_layer.destroy();
    for (int i=0; i < NUMBER_OF_LAYERS; ++i)
        hidden_layer[i].destroy();
    output_layer.destroy();
}

void NEURAL_NETWORK::execute() {
    input_layer.execute(hidden_layer[0].neuron);
    for (int l = 0; l < NUMBER_OF_LAYERS-1; ++l) {
        hidden_layer[l].execute(hidden_layer[l+1].neuron);
    }
    hidden_layer[NUMBER_OF_LAYERS-1].execute(output_layer.neuron);
}
void NEURAL_NETWORK::checknan() {
    input_layer.checknan();
    for (int i=0; i < NUMBER_OF_LAYERS; ++i)
        hidden_layer[i].checknan();
    output_layer.checknan();
}

void NEURAL_NETWORK::refresh() {
    input_layer.refresh();
    for (int i=0; i < NUMBER_OF_LAYERS; ++i)
        hidden_layer[i].refresh();
    output_layer.refresh();
}

NEURAL_NETWORK& NEURAL_NETWORK::operator=(const NEURAL_NETWORK& target) noexcept{
    this->input_layer = target.input_layer;
    for (int i=0; i < NUMBER_OF_LAYERS; ++i)
        this->hidden_layer[i] = target.hidden_layer[i];
    this->output_layer = target.output_layer;

    for (int i=0; i < OUTPUT_CELLS; ++i)
        this->last_output[i] = target.last_output[i];
    return *this;
}
void NEURAL_NETWORK::duplicate(struct NEURAL_NETWORK* father) {

    input_layer.duplicate(&father->input_layer);
    for (int l = 0; l < NUMBER_OF_LAYERS; ++l)
        hidden_layer[l].duplicate(&father->hidden_layer[l]);
    output_layer.duplicate(&father->output_layer);
}

void NEURAL_NETWORK::mutate() {
    input_layer.mutate(hidden_layer[0].neuron);
    for (int l = 0; l < NUMBER_OF_LAYERS-1; ++l)
        hidden_layer[l].mutate(hidden_layer[l+1].neuron);
    hidden_layer[NUMBER_OF_LAYERS-1].mutate(output_layer.neuron);
}

LAYER* NEURAL_NETWORK::nlayer(const uint16_t layer_id) {
    return getlayer(layer_id + 1);
}

LAYER* NEURAL_NETWORK::player(const uint16_t layer_id) {
    return getlayer(layer_id - 1);
}

LAYER* NEURAL_NETWORK::getlayer(const uint16_t layer_id) {
    assert(layer_id >= 0);
    if (layer_id == 0)
        return &input_layer;
    if (layer_id >= 1 && layer_id <= NUMBER_OF_LAYERS)
        return &hidden_layer[layer_id -1];
    if (layer_id == NUMBER_OF_LAYERS +1)
        return &output_layer;
    return nullptr;
}
void NEURAL_NETWORK::print(std::ofstream& out) {
    out << "PRINTING NNET: " << std::endl;
    input_layer.print(out);
    for (int i = 0; i < NUMBER_OF_LAYERS; ++i)
        hidden_layer[i].print(out);
    output_layer.print(out);
    out << std::endl;
}

void NEURAL_NETWORK::write(std::ofstream& out) {
    input_layer.write(out);
    for (int i=0; i < NUMBER_OF_LAYERS; ++i)
        hidden_layer[i].write(out);
    output_layer.write(out);
}

void NEURAL_NETWORK::read(std::ifstream& in) {
    init();
    input_layer.read(in);
    for (int i=0; i < NUMBER_OF_LAYERS; ++i)
        hidden_layer[i].read(in);
    output_layer.read(in);

}

