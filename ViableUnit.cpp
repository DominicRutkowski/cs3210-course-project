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

    void ViableUnit::setPlant(std::unique_ptr<Plant> plant) {
        this->plant = std::move(plant);
    }

    void ViableUnit::setAnimal(std::unique_ptr<Animal> animal) {
        this->animal = std::move(animal);
    }

    const std::unique_ptr<Plant>& ViableUnit::getPlant() {
        return plant;
    }

    const std::unique_ptr<Animal>& ViableUnit::getAnimal() {
        return animal;
    }

    const std::string ViableUnit::toString() const {
        if (animal != nullptr) {
            return animal->toString();
        } else if (plant != nullptr) {
            return plant->toString();
        } else {
            return " ";
        }
    }

    const UnitType ViableUnit::getUnitType() const {
        return UnitType::VIABLE_UNIT;
    }

}