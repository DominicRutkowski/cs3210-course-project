//
// Created by Dominic Rutkowski on 2019-04-10.
//

#include "Obstacle.hpp"

namespace cs3210 {

    Obstacle::Obstacle(const std::string& symbol):
    symbol{symbol} {}

    std::string Obstacle::toString() const {
        return symbol;
    }

    UnitType Obstacle::getUnitType() const {
        return UnitType::OBSTACLE;
    }

}