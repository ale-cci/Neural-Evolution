#pragma once
#include "neuron.h"

uint8_t  nneurons(const uint8_t layer_id);
uint8_t anneurons(const uint8_t layer_id);
uint8_t add_neuron(const uint8_t layer_id);

struct LAYER {
    uint8_t layer_id;
    struct NEURON* neuron;

    void init(const uint8_t layer_id);
    void execute(NEURON* nextlayer);
    void duplicate(LAYER* father);
    void destroy();

    void mutate();
    LAYER& operator=(const LAYER& ) noexcept;
};
