//
// Created by Dominic Rutkowski on 2019-04-10.
//

#ifndef CS3210_COURSE_PROJECT_ANIMAL_HPP
#define CS3210_COURSE_PROJECT_ANIMAL_HPP

#include "Organism.hpp"

#include <vector>

namespace cs3210 {

    class Animal : public Organism {
    protected:
        const std::vector<std::string> foodChain;
    public:
        Animal(const std::string& symbol, const unsigned int maxEnergy, unsigned int energy, const std::vector<std::string>& foodChain);
        const std::string toString() const override;
    };

}

#endif //CS3210_COURSE_PROJECT_ANIMAL_HPP
