//
// Created by Dominic Rutkowski on 2019-04-10.
//

#include "Animal.hpp"

namespace cs3210 {

    Animal::Animal(const std::string& symbol, const unsigned int maxEnergy, unsigned int energy,
                   const AnimalType& animalType, const std::vector<std::string>& foodChain):
                   Organism(symbol, maxEnergy, energy), animalType{animalType}, foodChain{foodChain} {}

    const std::string Animal::toString() const {
        return symbol;
    }

    const AnimalType Animal::getAnimalType() const {
        return animalType;
    }

}