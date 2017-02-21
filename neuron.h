#pragma once

struct NEURON {
    SYNAPSYS syanpsy[NEURONS_PER_LAYER];
    _PRECISON prop_value; // precision value of the neuron
    void input(DATATYPE in);
    DATATYPE output();
    void save(std::ofstream& out);
    void load(std::ifstream& in);
};
