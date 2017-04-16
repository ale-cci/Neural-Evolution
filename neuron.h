#pragma once
#include "generic_functions.h"
#include "neural_network_constant.h"
#include "layer.h"

struct NEURON {
    uint16_t layer_id;
    _PRECISION input_sum;
    _PRECISION prop_value; // precision value of the neuron
    _PRECISION *synapsis;

    void input(_PRECISION in);
    void init(const uint16_t layer_id);
    void allocate(const uint16_t layer_id);
    void execute(struct NEURON* nextlayer);
    void refresh();
    void duplicate(NEURON* father);
    void mutate(struct NEURON* nextlayer);
    double spread_value();
    void destroy();

    void print(std::ofstream& out);
    void write(std::ofstream& out);
    void read(std::ifstream& in);

    NEURON& operator=(const NEURON& target) noexcept;
};
