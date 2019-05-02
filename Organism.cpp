//
// Created by Dominic Rutkowski on 2019-04-10.
//

#include "Organism.hpp"

namespace cs3210 {

    Organism::Organism(const std::string &symbol, const unsigned int maxEnergy):
    Organism(symbol, maxEnergy, maxEnergy) {}

    Organism::Organism(const std::string& symbol, const unsigned int maxEnergy, unsigned int energy):
    symbol{symbol}, maxEnergy{maxEnergy}, energy{energy} {}

    void Organism::setEnergy(unsigned int energy) {
        if (energy >= maxEnergy) {
            energy = maxEnergy;
        } else {
            this->energy = energy;
        }
    }

    unsigned int Organism::getMaxEnergy() const {
        return maxEnergy;
    }

    unsigned int Organism::getEnergy() const {
        return energy;
    }

}