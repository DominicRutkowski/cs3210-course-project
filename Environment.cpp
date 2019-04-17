//
// Created by Dominic Rutkowski on 2019-03-30.
//

#include "Environment.hpp"

namespace cs3210 {

    Environment::Environment(const std::vector<std::string>& mapLines, const std::vector<std::string>& speciesLines):
    grid{std::vector<std::vector<std::unique_ptr<Unit>>>(mapLines.size())} {
        for (int i = 0; i < mapLines.size(); ++i) {
            grid[i] = std::vector<std::unique_ptr<Unit>>(mapLines[i].size());
            for (int j = 0; j < mapLines[i].size(); ++j) {
                grid[i][j] = std::move(parseUnit(mapLines[i][j], speciesLines));
            }
        }
    }

    std::string Environment::toString() const {
        std::string result;
        for (int i = 0; i < grid.size(); ++i) {
            for (int j = 0; j < grid[i].size(); ++j) {
                result += grid[i][j]->toString();
            }
            result += "\n";
        }
        return result;
    }

    std::unique_ptr<Unit> Environment::parseUnit(char ch, const std::vector<std::string>& speciesLines) const {
        for (auto& speciesDefinition : speciesLines) {
            unsigned int typeCharIndex = speciesDefinition.find(' ') + 1;
            char typeChar = speciesDefinition.at(typeCharIndex);
            if (ch == typeChar) { // Occupied space
                std::string organismClassification = speciesDefinition.substr(0, typeCharIndex - 1);
                return std::unique_ptr<Obstacle>(new Obstacle("X"));
            }
        }
        if (ch == ' ') { // Empty space
            return std::unique_ptr<Obstacle>(new Obstacle(" ")); // Issue with empty spaces being default constructed.. add constructor
        } else { // Obstacle
            return std::unique_ptr<Obstacle>(new Obstacle(std::string(1, ch)));
        }
    }

}