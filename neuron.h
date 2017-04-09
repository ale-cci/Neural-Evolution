#pragma once
#include "generic_functions.h"
#include "neural_network_constant.h"
#include "layer.h"

struct NEURON {
    uint8_t layer_id;
    _PRECISION input_sum;
    _PRECISION prop_value; // precision value of the neuron
    _PRECISION *synapsis;
    void input(_PRECISION in);
    void init(const uint8_t layer_id);
    void allocate(const uint8_t layer_id);
    void execute(struct NEURON* nextlayer);
    void refresh();
    void duplicate(NEURON* father);
    void mutate();
    double spread_value();
    void destroy();

    NEURON& operator=(const NEURON& target) noexcept;
};
