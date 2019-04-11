//
// Created by Dominic Rutkowski on 2019-04-10.
//

#include "Plant.hpp"

namespace cs3210 {

    Plant::Plant(const std::string& symbol, const unsigned int maxEnergy, unsigned int energy, const unsigned int regrowthCoefficient):
    Organism(symbol, maxEnergy, energy), regrowthCoefficient{regrowthCoefficient} {}

    const std::string Plant::toString() const {
        if (iterationsUntilGrowth <= 0) {
            return symbol;
        }
        return " ";
    }

}