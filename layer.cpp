#include <fstream>
#include <iostream>
#include <cassert>
#include "layer.h"

void LAYER::init(const uint8_t _layer_id) {
    this->layer_id = _layer_id;
    if (anneurons(layer_id) <= 0)
        return ;
    neuron = new NEURON[anneurons(layer_id)];
    for (int i = 0; i < anneurons(layer_id); ++i)
        neuron[i].init(layer_id);
    return ;
}

void LAYER::destroy() {
    for (int i = 0; i < anneurons(layer_id); ++i)
        neuron[i].destroy();
    if (anneurons(layer_id))
        delete[] neuron;
}

void LAYER::execute(NEURON* nextlayer) {
    for (int i=0; i < anneurons(layer_id); ++i)
        neuron[i].execute(nextlayer);
}

void LAYER::duplicate(LAYER* father) {
    layer_id = father->layer_id;
    for (int i=0; i < anneurons(layer_id); ++i)
        neuron[i].duplicate(&father->neuron[i]);
}

void LAYER::mutate() {
    for (int i=0; i < anneurons(layer_id); ++i)
        neuron[i].mutate();
}

LAYER& LAYER::operator=(const LAYER& target) noexcept {
    this->layer_id = target.layer_id;
    for (int i=0; i < anneurons(layer_id); ++i)
        neuron[i] = target.neuron[i];
    return *this;
}

uint8_t  nneurons(const uint8_t layer_id) {
    if (layer_id == 0)
        return INPUT_CELLS;
    if (layer_id >=1 && layer_id <= NUMBER_OF_LAYERS)
        return NEURONS_PER_LAYER;
    if (layer_id == NUMBER_OF_LAYERS + 1)
        return OUTPUT_CELLS;
    return 0;
}

uint8_t add_neuron(const uint8_t layer_id) {
    return ((layer_id >= 1) && (layer_id <= NUMBER_OF_LAYERS));
}

uint8_t anneurons(const uint8_t layer_id) {
    return nneurons(layer_id) + add_neuron(layer_id);
}
