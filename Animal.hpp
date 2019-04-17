//
// Created by Dominic Rutkowski on 2019-04-10.
//

#ifndef CS3210_COURSE_PROJECT_ANIMAL_HPP
#define CS3210_COURSE_PROJECT_ANIMAL_HPP

#include "Organism.hpp"

#include <vector>

namespace cs3210 {

    enum class AnimalType {
        HERBIVORE, OMNIVORE
    };

    class Animal : public Organism {
    protected:
        const AnimalType animalType;
        const std::vector<std::string> foodChain;
    public:
        Animal(const std::string& symbol, const unsigned int maxEnergy, unsigned int energy, const AnimalType& animalType, const std::vector<std::string>& foodChain);
        const std::string toString() const override;
        const AnimalType getAnimalType() const;
    };

}

#endif //CS3210_COURSE_PROJECT_ANIMAL_HPP
