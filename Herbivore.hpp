//
// Created by Dominic Rutkowski on 2019-04-01.
//

#ifndef CS3210_COURSE_PROJECT_HERBIVORE_HPP
#define CS3210_COURSE_PROJECT_HERBIVORE_HPP

#include "Animal.hpp"

namespace cs3210 {

    class Herbivore : public Animal {
    private:
    public:
        void update() override;
    };

}

#endif //CS3210_COURSE_PROJECT_HERBIVORE_HPP
