//
// Created by Dominic Rutkowski on 2019-04-10.
//

#include "ViableUnit.hpp"

namespace cs3210 {

    const std::string ViableUnit::toString() const {
        if (animal != nullptr) {
            return animal->toString();
        }
        return plant->toString();
    }

    const UnitType ViableUnit::getUnitType() const {
        return UnitType::VIABLE_UNIT;
    }

}