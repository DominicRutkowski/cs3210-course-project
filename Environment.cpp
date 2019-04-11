//
// Created by Dominic Rutkowski on 2019-03-30.
//

#include "Environment.hpp"

namespace cs3210 {

    Environment::Environment(const std::vector<std::string>& mapLines, const std::vector<std::string>& speciesLines):
    grid{std::vector<std::vector<std::unique_ptr<Unit>>>()} {
        for (int i = 0; i < mapLines.size(); ++i) {
            for (int j = 0; j < mapLines[i].size(); ++j) {
                grid[i][j] = std::move(parseUnit(mapLines[i][j], speciesLines));
            }
        }
    }

    std::unique_ptr<Unit> Environment::parseUnit(char ch, const std::vector<std::string>& speciesLines) const {
        for (auto& speciesDefinition : speciesLines) {
            unsigned int typeCharIndex = speciesDefinition.find(' ') + 1;
            char typeChar = speciesDefinition.at(typeCharIndex);
            if (ch == typeChar) { // Occupied space
                std::string organismClassification = speciesDefinition.substr(0, typeCharIndex - 2);
                std::cout << organismClassification << std::endl;
            }
        }
        if (ch == ' ') { // Empty space
            return std::unique_ptr<ViableUnit>();
        } else { // Obstacle
            return std::unique_ptr<Obstacle>(new Obstacle(std::string(1, ch)));
        }
    }

}