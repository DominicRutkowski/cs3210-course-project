//
// Created by Dominic Rutkowski on 2019-04-10.
//

#ifndef CS3210_COURSE_PROJECT_UNIT_HPP
#define CS3210_COURSE_PROJECT_UNIT_HPP

#include <string>

namespace cs3210 {

    enum class UnitType {
        OBSTACLE, VIABLE_UNIT
    };

    class Unit {
    private:
    public:
        virtual const std::string toString() const = 0;
        virtual const UnitType getType() const = 0;
    };

}

#endif //CS3210_COURSE_PROJECT_UNIT_HPP
