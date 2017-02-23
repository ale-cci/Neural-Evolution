#pragma once
#include "generic_functions.h"
#include "neural_network_constant.h"

struct NEURON {
    int input_nums = 0;
    _PRECISION input_sum;
    _PRECISION prop_value; // precision value of the neuron
    _PRECISION synapsy[NEURONS_PER_LAYER];
    void input(_PRECISION in);
    void init(NEURON* nextlayer);
    void execute(NEURON* nextlayer);
    void refresh();
    NEURON();
    void inherit_from(NEURON* father);
    void save(std::ofstream& out);
    void load(std::ifstream& in);
};
