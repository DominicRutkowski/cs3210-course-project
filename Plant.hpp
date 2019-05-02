//
// Created by Dominic Rutkowski on 2019-04-10.
//

#ifndef CS3210_COURSE_PROJECT_PLANT_HPP
#define CS3210_COURSE_PROJECT_PLANT_HPP

#include "Organism.hpp"

namespace cs3210 {

    class Plant : public Organism {
    private:
        const unsigned int regrowthCoefficient;
        unsigned int iterationsUntilGrowth;
    public:
        Plant(const std::string& symbol, const unsigned int maxEnergy, const unsigned int regrowthCoefficient);
        void iterate();
        unsigned int consume();
        unsigned int getIterationsUntilGrowth() const;
        std::string toString() const override;
    };

}

#endif //CS3210_COURSE_PROJECT_PLANT_HPP
