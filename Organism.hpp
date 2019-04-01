//
// Created by Dominic Rutkowski on 2019-04-01.
//

#ifndef CS3210_COURSE_PROJECT_ORGANISM_HPP
#define CS3210_COURSE_PROJECT_ORGANISM_HPP

#include "Unit.hpp"

namespace cs3210 {

    class Organism : Unit {
    protected:
    public:
        virtual void update() override;
    };

}

#endif //CS3210_COURSE_PROJECT_ORGANISM_HPP
