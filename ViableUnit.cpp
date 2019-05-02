//
// Created by Dominic Rutkowski on 2019-04-10.
//

#include "ViableUnit.hpp"

namespace cs3210 {

    void ViableUnit::iteratePlant() {
        if (animal == nullptr) {
            plant->iterate();
        }
    }

    std::string ViableUnit::toString() const {
        if (animal != nullptr) {
            return animal->toString();
        } else if (plant != nullptr) {
            return plant->toString();
        } else {
            return " ";
        }
    }

    UnitType ViableUnit::getUnitType() const {
        return UnitType::VIABLE_UNIT;
    }

}