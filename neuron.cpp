#include "neuron.h"
#include <algorithm>
#include <fstream>

void NEURON::execute(NEURON* next_layer) {
    input_sum = input_sum / std::max(1, input_nums); // average of inputs
    for (int i=0; i < NEURONS_PER_LAYER; ++i) {
        if (!synapsy[i]) // not connected
            continue;
        // broadcast his value
        next_layer[i].input(prop_value * input_sum * synapsy[i]);
    }
    return ;
}

void NEURON::input(_PRECISION in) {
    input_sum += in;
    return ;
}

void NEURON::refresh() {
    input_sum = 0;
    return ;
}
NEURON::NEURON() {
    input_nums = 0;
    for (int i=0; i < NEURONS_PER_LAYER; ++i)
        synapsy[i] = 0;
}
void NEURON::init(NEURON* nextlayer) {
    for (int i = 0; i < NEURONS_PER_LAYER; ++i) {
        synapsy[i] = std::max(0.0, rand(-0.5, 1.0));
        if (synapsy[i])
            nextlayer[i].input_nums++;
    }
}
void NEURON::inherit_from(NEURON* father) {
    input_nums = father->input_nums;
    prop_value = father->prop_value;

    for (int i=0; i < NEURONS_PER_LAYER; ++i)
        synapsy[i] = father->synapsy[i];
}

void NEURON::save(std::ofstream& out) {
    out << prop_value << " " << input_nums << std::endl;
    for (int i = 0; i < NEURONS_PER_LAYER; ++i)
        out << synapsy[i] << " ";
    out << std::endl;
}

void NEURON::load(std::ifstream& in) {
    in >> prop_value >> input_nums;
    for (int i = 0; i < NEURONS_PER_LAYER; ++i)
        in >> synapsy[i];
}
