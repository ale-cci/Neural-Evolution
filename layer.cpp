#include <fstream>
#include <iostream>
#include "layer.h"

void LAYER::init(LAYER* nextlayer, int8_t neuron_per_layer) {
    if (nextlayer == nullptr)
        warning("LAYER", "initializing null ptr...");
    else
        warning("LAYER", "initializing...");
    neuron_number = neuron_per_layer;
    neuron = new NEURON[neuron_number];
    for (int i = 0; i < neuron_number; ++i)
        neuron[i].init(nextlayer);
    return ;
}


void LAYER::save(std::ofstream& out, LAYER* nextlayer) {
    out << neuron_number <<std::endl;
    // TODO: reverse this cycle
    for (int i=0; i < neuron_number; ++i)
        neuron[i].save(out, nextlayer);
}
void LAYER::load(std::ifstream& in, LAYER* nextlayer) {
    // TODO: reverse this cycle
    in >> neuron_number;
    for (int i=0; i < neuron_number; ++i)
        neuron[i].load(in, nextlayer);
}
void LAYER::execute(LAYER* nextlayer) {
    for (int i=0; i < neuron_number; ++i)
        neuron[i].execute(nextlayer);
}
void LAYER::refresh() {
    for (int i=0; i < neuron_number; ++i)
        neuron[i].refresh();
}
void LAYER::inherit_from(LAYER* father, LAYER* nextlayer) {
    neuron_number = father->neuron_number;
    for (int i=0; i < neuron_number; ++i)
        neuron[i].inherit_from(&father->neuron[i], nextlayer);
}
void LAYER::mutate(LAYER* nextlayer) {
    for (int i=0; i < neuron_number; ++i)
        neuron[i].mutate(nextlayer);
}
