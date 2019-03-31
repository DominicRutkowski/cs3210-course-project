//
// Created by Dominic Rutkowski on 2019-03-30.
//

#ifndef CS3210_COURSE_PROJECT_ENVIRONMENT_HPP
#define CS3210_COURSE_PROJECT_ENVIRONMENT_HPP

#include "Unit.hpp"

#include <vector>

namespace cs3210 {

    // Use Grid as a type alias for a 2D std::vector
    template <typename T>
    using Grid = std::vector<std::vector<T>>;

    class Environment {
    private:
        Grid<Unit> grid;
    public:;
        void update();
        void update(unsigned int cycles);
    };

}

#endif //CS3210_COURSE_PROJECT_ENVIRONMENT_HPP
