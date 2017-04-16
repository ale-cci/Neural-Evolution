#include <fstream>
#include <iostream>
#include <cassert>
#include <cmath>
#include "layer.h"

void LAYER::init(const uint16_t _layer_id) {
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

void LAYER::refresh() {
    for (int i=0; i < anneurons(layer_id); ++i) {
        neuron[i].refresh();
    }
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
void LAYER::checknan() {
    for (int i=0; i < anneurons(layer_id); ++i) {
        assert(!std::isnan(neuron[i].input_sum));
        assert(!std::isnan(neuron[i].prop_value));
        assert(!std::isnan(neuron[i].spread_value()));

    }
}
void LAYER::mutate(NEURON* nextlayer) {
    for (int i=0; i < anneurons(layer_id); ++i)
        neuron[i].mutate(nextlayer);
}

LAYER& LAYER::operator=(const LAYER& target) noexcept {
    this->layer_id = target.layer_id;
    for (int i=0; i < anneurons(layer_id); ++i)
        this->neuron[i] = target.neuron[i];
    return *this;
}

uint16_t  nneurons(const uint16_t layer_id) {
    if (layer_id == 0)
        return INPUT_CELLS;
    if (layer_id >=1 && layer_id <= NUMBER_OF_LAYERS)
        return NEURONS_PER_LAYER;
    if (layer_id == NUMBER_OF_LAYERS + 1)
        return OUTPUT_CELLS;
    return 0;
}

void LAYER::print(std::ofstream& out) {
    out << "LAYER [" << layer_id << "]" << std::endl;
    for (int i=0; i < anneurons(layer_id); ++i) {
        out << i << "-> ";
        neuron[i].print(out);
    }
    out << std::endl;
}

uint16_t add_neuron(const uint16_t layer_id) {
    return ((layer_id >= 1) && (layer_id <= NUMBER_OF_LAYERS));
}

uint16_t anneurons(const uint16_t layer_id) {
    return nneurons(layer_id) + add_neuron(layer_id);
}


void LAYER::read(std::ifstream& in) {
    in.read((char*) &layer_id, sizeof(layer_id));
    for (int i=0; i < anneurons(layer_id); ++i)
        neuron[i].read(in);
}

void LAYER::write(std::ofstream& out) {
    out.write((char*) &layer_id, sizeof(layer_id));
    for (int i=0; i < anneurons(layer_id); ++i)
        neuron[i].write(out);
}
