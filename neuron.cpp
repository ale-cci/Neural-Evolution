#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include "neuron.h"
#include "extmath.h"
#include "generic_functions.h"


void NEURON::execute(struct NEURON* next_layer) {
    for (uint16_t i=0; i < nneurons(layer_id + 1); ++i) {
        assert(!std::isnan(input_sum));
        next_layer[i].input_sum += (spread_value()*synapsis[i]);
        assert(!std::isnan(next_layer[i].input_sum));
    }
    return ;
}

void NEURON::refresh() {
    input_sum = 0;
    assert(!std::isnan(input_sum));
}

double NEURON::spread_value() {
    uint16_t in = (anneurons(layer_id-1) > 1) ? anneurons(layer_id-1) : 1;
    assert(in > 0);
    assert(!std::isnan(input_sum));
    return special_one(prop_value + input_sum / in);
}
void NEURON::read(std::ifstream& in) {
    in.read((char*) &layer_id, sizeof(layer_id));
    in.read((char*) &prop_value, sizeof(prop_value));
    in.read((char*) synapsis, nneurons(layer_id+1) * sizeof(synapsis));
}

void NEURON::write(std::ofstream& out) {
    out.write((char*) &layer_id, sizeof(layer_id));
    out.write((char*) &prop_value, sizeof(prop_value));
    out.write((char*) synapsis, nneurons(layer_id+1) * sizeof(synapsis));
}

void NEURON::input(_PRECISION in) {
    assert(!std::isnan(in));
    input_sum += in;
    return ;
}

void NEURON::allocate(const uint16_t _layer_id) {
    layer_id = _layer_id;

    if (nneurons(layer_id+1) > 0)
        synapsis = new _PRECISION[nneurons(layer_id + 1)];

}

void NEURON::init(const uint16_t layer_id) {
    allocate(layer_id);
    prop_value = rand(-1., 1.);
    for (int i = 0; i < nneurons(layer_id+1); ++i)
        synapsis[i] = rand(-1., +1.);
}

void NEURON::destroy() {
    if (anneurons(layer_id+1) > 0)
        delete[] synapsis;
}

void NEURON::duplicate(NEURON* father) {
    allocate(father->layer_id);
    prop_value = father->prop_value;
    for (int i=0; i < nneurons(layer_id+1); ++i)
        synapsis[i] = father->synapsis[i];
}
/// TODO: legge di Hebb
void NEURON::mutate(struct NEURON* nextlayer) {
    for (int i = 0; i < nneurons(layer_id+1); ++i) {
        double delta = MUTATION_PERCENTAGE * spread_value() * nextlayer[i].spread_value();
        synapsis[i] = bound(synapsis[i] + delta, -1, +1);
    }
}

NEURON& NEURON::operator=(const NEURON& target) noexcept{
    this->layer_id = target.layer_id;
    this->prop_value = target.prop_value;
    this->input_sum = target.input_sum;
    for (uint16_t i=0; i < nneurons(layer_id+1); ++i)
        this->synapsis[i] = target.synapsis[i];
    return *this;
}

void NEURON::print(std::ofstream& out) {
    out << "[" << prop_value << "]   ";
    for(int i=0; i < nneurons(layer_id+1); ++i)
        out << synapsis[i] << " / ";
    out << std::endl;
}
