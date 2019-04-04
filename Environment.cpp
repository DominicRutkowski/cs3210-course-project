//
// Created by Dominic Rutkowski on 2019-03-30.
//

#include "Environment.hpp"

namespace cs3210 {

    Environment::Environment(const std::vector<std::string>& mapLines, const std::vector<std::string>& speciesLines):
    grid{std::vector<std::vector<Unit>>()} {
        for (int i = 0; i < mapLines.size(); ++i) {
            for (int j = 0; j < mapLines[i].size(); ++j) {
                grid[i][j] = parseUnit(mapLines[i][j], speciesLines);
            }
        }
    }

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

    Unit Environment::parseUnit(char ch, const std::vector<std::string>& speciesLines) const {
        for (auto& speciesDefinition : speciesLines) {
            unsigned int typeCharIndex = speciesDefinition.find(' ') + 1;
            char typeChar = speciesDefinition.at(typeCharIndex);
            if (ch == typeChar) {
                // Organism type
            } else {
                continue;
            }
        }
        if (ch == ' ') {
            // Empty space
        } else {
            // Obstacle
        }
    }

}