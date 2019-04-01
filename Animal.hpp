//
// Created by Dominic Rutkowski on 2019-04-01.
//

#ifndef CS3210_COURSE_PROJECT_ANIMAL_HPP
#define CS3210_COURSE_PROJECT_ANIMAL_HPP

#include "Organism.hpp"

namespace cs3210 {

    class Animal : public Organism {
    protected:
    public:
        virtual void update() override;
    };

}

#endif //CS3210_COURSE_PROJECT_ANIMAL_HPP
