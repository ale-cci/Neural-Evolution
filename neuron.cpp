#include <algorithm>
#include <fstream>
#include <iostream>
#include "neuron.h"
#include "extmath.h"
#include "generic_functions.h"


void NEURON::execute(struct NEURON* next_layer) {
    for (int i=0; i < nneurons(layer_id + 1); ++i) {
        next_layer[i].input_sum += (spread_value()*synapsis[i]);
    }
    return ;
}

void NEURON::refresh() {
    input_sum = 0;
}

double NEURON::spread_value() {
    uint8_t in = 0;
    in = (anneurons(layer_id) > 1) ? anneurons(layer_id) : 1;
    return special_one(prop_value + input_sum / in);
}
void NEURON::input(_PRECISION in) {
    input_sum += in;
    return ;
}

void NEURON::allocate(const uint8_t _layer_id) {
    layer_id = _layer_id;

    if (nneurons(layer_id+1) > 0)
        synapsis = new _PRECISION[nneurons(layer_id + 1)];

}

void NEURON::init(const uint8_t layer_id) {
    allocate(layer_id);
    prop_value = rand(-1., 1.);
    for (int i = 0; i < nneurons(layer_id+1); ++i)
        synapsis[i] = rand(-0.5, +0.5);
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
/// FIXME: legge di Hebb
void NEURON::mutate() {
    if (rand(0, 100) < MUTATION_PERCENTAGE)
        prop_value += rand(-MUTATION_CHANGE, +MUTATION_CHANGE);
    prop_value = bound(prop_value, -1, +1);
    for (int i = 0; i < nneurons(layer_id+1); ++i)
        if (rand(0, 100) < MUTATION_PERCENTAGE) {
            double mutation = rand(-MUTATION_CHANGE,+MUTATION_CHANGE);
            synapsis[i] += bound(synapsis[i] + mutation, 0, 1);
        }
}

NEURON& NEURON::operator=(const NEURON& target) noexcept{
    this->layer_id = target.layer_id;
    this->prop_value = target.prop_value;
    this->input_sum = target.input_sum;
    for (int i=0; i < nneurons(layer_id+1); ++i)
        this->synapsis[i] = target.synapsis[i];
    return *this;
}
