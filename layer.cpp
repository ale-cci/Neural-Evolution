#include <fstream>
#include <iostream>
#include "layer.h"

void LAYER::init(LAYER* nextlayer, int8_t neuron_per_layer) {
    neuron_number = neuron_per_layer;
    neuron = new NEURON[neuron_number];
    for (int i = 0; i < neuron_number; ++i)
        neuron[i].init(nextlayer);
    return ;
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
