//
// Created by Dominic Rutkowski on 2019-03-30.
//

#include "Environment.hpp"

namespace cs3210 {

    void Environment::update() {
        update(1);
    }

    void Environment::update(unsigned int cycles) {
        for (int i = 0; i < cycles; ++i) {
            for (auto &row : grid) {
                for (auto &unit : row) {
                    unit.update();
                }
            }
        }
    }

}