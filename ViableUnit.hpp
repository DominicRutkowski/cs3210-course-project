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
    private:
        std::unique_ptr<Plant> plant;
        std::unique_ptr<Animal> animal;
    public:
        void iteratePlant();
        void iterateAnimal();

        void setPlant(std::unique_ptr<Plant> plant);
        void setAnimal(std::unique_ptr<Animal> animal);

        const std::unique_ptr<Plant>& getPlant();
        const std::unique_ptr<Animal>& getAnimal();

        const std::string toString() const override;
        const UnitType getUnitType() const override;
    };

}

#endif //CS3210_COURSE_PROJECT_VIABLE_UNIT_HPP
