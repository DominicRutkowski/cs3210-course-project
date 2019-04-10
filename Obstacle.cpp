//
// Created by Dominic Rutkowski on 2019-04-10.
//

#include "Obstacle.hpp"

namespace cs3210 {

    Obstacle::Obstacle(const std::string& symbol):
    symbol{symbol} {}

    const std::string Obstacle::toString() const {
        return symbol;
    }

    const UnitType Obstacle::getType() const {
        return UnitType::OBSTACLE;
    }

}