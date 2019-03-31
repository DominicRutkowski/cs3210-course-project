//
// Created by Dominic Rutkowski on 2019-03-30.
//

#include "Environment.hpp"

namespace cs3210 {

    void Environment::update() {
        update(1);
    }

    void Environment::update(unsigned int cycles) {
        for (auto& row : grid) {
            for (auto& unit : row) {
                unit.update();
            }
        }
    }

}