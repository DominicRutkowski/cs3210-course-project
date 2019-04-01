//
// Created by Dominic Rutkowski on 2019-04-01.
//

#ifndef CS3210_COURSE_PROJECT_OBSTACLE_HPP
#define CS3210_COURSE_PROJECT_OBSTACLE_HPP

#include "Unit.hpp"

namespace cs3210 {

    class Obstacle : public Unit {
    private:
    public:
        void update() override;
    };

}

#endif //CS3210_COURSE_PROJECT_OBSTACLE_HPP
