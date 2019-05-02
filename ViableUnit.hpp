//
// Created by Dominic Rutkowski on 2019-04-10.
//

#ifndef CS3210_COURSE_PROJECT_VIABLE_UNIT_HPP
#define CS3210_COURSE_PROJECT_VIABLE_UNIT_HPP

#include "Animal.hpp"
#include "Plant.hpp"
#include "Unit.hpp"

#include <iostream>
#include <memory>

namespace cs3210 {

    class ViableUnit : public Unit {
    public:
        std::unique_ptr<Plant> plant;
        std::unique_ptr<Animal> animal;

        void iteratePlant();

        std::string toString() const override;
        UnitType getUnitType() const override;
    };

}

#endif //CS3210_COURSE_PROJECT_VIABLE_UNIT_HPP
