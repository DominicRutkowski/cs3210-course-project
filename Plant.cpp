//
// Created by Dominic Rutkowski on 2019-04-10.
//

#include "Plant.hpp"

namespace cs3210 {

    Plant::Plant(const std::string& symbol, const unsigned int maxEnergy, const unsigned int regrowthCoefficient):
    Organism(symbol, maxEnergy, maxEnergy), regrowthCoefficient{regrowthCoefficient}, iterationsUntilGrowth{0} {}

    void Plant::iterate() {
        if (iterationsUntilGrowth > 0) {
            --iterationsUntilGrowth;
        }
    }

    unsigned int Plant::consume() {
        iterationsUntilGrowth = regrowthCoefficient;
        return maxEnergy;
    }

    const unsigned int Plant::getIterationsUntilGrowth() const {
        return iterationsUntilGrowth;
    }

    const std::string Plant::toString() const {
        if (iterationsUntilGrowth <= 0) {
            return symbol;
        }
        return " ";
    }

}