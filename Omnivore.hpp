//
// Created by Dominic Rutkowski on 2019-04-01.
//

#ifndef CS3210_COURSE_PROJECT_OMNIVORE_HPP
#define CS3210_COURSE_PROJECT_OMNIVORE_HPP

#include "Animal.hpp"

namespace cs3210 {

    class Omnivore : public Animal {
    private:
    public:
        void update() override;
    };

}

#endif //CS3210_COURSE_PROJECT_OMNIVORE_HPP
