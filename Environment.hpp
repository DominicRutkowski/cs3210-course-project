//
// Created by Dominic Rutkowski on 2019-03-30.
//

#ifndef CS3210_COURSE_PROJECT_ENVIRONMENT_HPP
#define CS3210_COURSE_PROJECT_ENVIRONMENT_HPP

#include "Obstacle.hpp"
#include "Unit.hpp"
#include "ViableUnit.hpp"

#include <algorithm>
#include <exception>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace cs3210 {

    // Use Grid as a type alias for a 2D std::vector
    template <typename T>
    using Grid = std::vector<std::vector<T>>;

    class Environment {
    private:
        Grid<std::shared_ptr<Unit>> grid;

        void iterateAnimals(AnimalType animalType);

        bool canMoveTo(const Animal& animal, const std::shared_ptr<Unit> unit);
        bool canConsume(const Animal& animal, const Plant& plant);
        bool canConsume(const Animal& animal, const Animal& prey);
        bool isPredatorOf(const Animal& animal, const std::shared_ptr<Unit> unit);

        std::shared_ptr<Unit> parseUnit(char ch, const std::vector<std::string>& speciesLines) const;
        std::shared_ptr<Unit> getUnit(int x, int y);
    public:
        Environment(const std::vector<std::string>& mapLines, const std::vector<std::string>& speciesLines);

        void iterate(unsigned int iterations = 1);

        std::string toString() const;
    };

}

#endif //CS3210_COURSE_PROJECT_ENVIRONMENT_HPP
